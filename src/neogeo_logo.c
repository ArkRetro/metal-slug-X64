#include <libdragon.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "neogeo_logo.h"
#include "msx_audio.h"
#include "msx_perf.h"

/* Native boot logo: the NEOGEO eyecatch letters as a closed extruded mesh
 * (tools/build_neogeo_logo.py), projected and lit on the CPU and drawn as
 * z-buffered Gouraud triangles that the rdpq RSP overlay sets up for the RDP.
 * The animation follows the Mario Kart 64 boot logo as measured in ares 146
 * (docs/LOGO_NEOGEO.md); nothing from that game is used. Times are in NTSC
 * video ticks (59.826 Hz) and follow the clock, not the frame count.
 *
 * The mesh is stored as its parts: the cap outline points and triangles
 * (front and back caps share them) and the side edges. A cap faces the camera
 * or not as a whole, and a side quad is tested with one multiply against a
 * constant stored per edge, so only visible geometry is projected and lit,
 * reading memory in order (the CPU cost is dominated by cache misses). */

#define VI_HZ 59.826f
#define T_SPIN_END 373.0f    /* fade to white starts (MK64: ~6.2 s after the logo appears) */
#define T_WHITE 443.0f       /* fully white after 70 ticks */
#define T_WHITE_END 509.0f   /* white held 66 ticks */
#define T_END 545.0f         /* white -> black in 36 ticks, then the title */
#define START_YAW (-55.0f)   /* first visible frame: turned away, unlit */
#define CAM_DIST 300.0f
#define FOCAL 380.0f

/* Measured MK64 spin speed in degrees per tick: slow start, accelerating
 * until the fade (about 3.4 turns before the white). */
static const float speed_keys[][2]={{0,1.3f},{79,1.36f},{138,1.7f},{187,2.0f},{224,3.2f},{260,4.1f},{300,5.2f},{330,8.0f},{373,10.0f},{T_END,10.0f}};
#define YAW_STEPS 560
static float yaw_table[YAW_STEPS];

typedef struct { float ax,ay,bx,by,nax,nay,nbx,nby,fnx,k; } logo_edge_t;
static float (*cap_points)[2];
static logo_edge_t *edges;
static uint16_t *cap_tris;
static unsigned cap_count,edge_count,cap_tri_count;
static float logo_w,logo_h,logo_d;
static float (*cap_screen)[7];

static float speed_at(float t) {
    unsigned n=sizeof(speed_keys)/sizeof(speed_keys[0]);
    if (t<=speed_keys[0][0]) return speed_keys[0][1];
    for (unsigned i=1;i<n;i++) if (t<=speed_keys[i][0]) {
        float u=(t-speed_keys[i-1][0])/(speed_keys[i][0]-speed_keys[i-1][0]);
        return speed_keys[i-1][1]+(speed_keys[i][1]-speed_keys[i-1][1])*u;
    }
    return speed_keys[n-1][1];
}
static float yaw_at(float t) {
    if (t<=0) return yaw_table[0];
    unsigned i=(unsigned)t;
    if (i+1>=YAW_STEPS) return yaw_table[YAW_STEPS-1];
    float u=t-(float)i;return yaw_table[i]+(yaw_table[i+1]-yaw_table[i])*u;
}
static float smooth(float a,float b,float t) {
    if (t<=a) return 0;
    if (t>=b) return 1;
    float u=(t-a)/(b-a);return u*u*(3-2*u);
}
static float clamp1(float v) { return v<0?0:(v>1?1:v); }

static void free_mesh(void) {
    free(cap_points);free(edges);free(cap_tris);free(cap_screen);
    cap_points=NULL;edges=NULL;cap_tris=NULL;cap_screen=NULL;
}
static bool load_mesh(void) {
    FILE *f=fopen("rom:/neogeo-logo.mesh","rb");if (!f) return false;
    struct { char magic[4];uint16_t version,caps,edges,tris;float w,h,d; } __attribute__((packed)) head;
    bool ok=fread(&head,sizeof(head),1,f)==1 && !memcmp(head.magic,"NGLM",4) && head.version==2 && head.caps && head.edges && head.tris;
    if (ok) {
        cap_count=head.caps;edge_count=head.edges;cap_tri_count=head.tris;logo_w=head.w;logo_h=head.h;logo_d=head.d;
        cap_points=malloc(8*cap_count);edges=malloc(sizeof(logo_edge_t)*edge_count);
        cap_tris=malloc(6*cap_tri_count);cap_screen=malloc(28*cap_count);
        ok=cap_points && edges && cap_tris && cap_screen && fread(cap_points,8,cap_count,f)==cap_count &&
           fread(edges,sizeof(logo_edge_t),edge_count,f)==edge_count && fread(cap_tris,6,cap_tri_count,f)==cap_tri_count;
        for (unsigned i=0;ok && i<cap_tri_count*3;i++) ok=cap_tris[i]<cap_count;
    }
    fclose(f);
    if (!ok) free_mesh();
    return ok;
}

/* Frame state shared by the helpers below. */
static float rc,rs,focal,light;
static const float LX=-0.36f,LY=0.52f,LZ=0.775f;
static float HX,HY,HZ;

/* Gold metal, view direction taken as constant (the camera is far from the
 * logo): ambient + diffuse from an upper-left key light, a sky/ground term
 * from the reflected view ray and a white Blinn highlight. Normal already
 * rotated. `grad` darkens toward the bottom of the letters. */
static void metal(float nx,float ny,float nz,float grad,float *o) {
    float ndl=nx*LX+ny*LY+nz*LZ;if (ndl<0) ndl=0;
    /* ^16, cut below 0.25 so the powers never reach denormals (the VR4300 traps on them). */
    float ndh=nx*HX+ny*HY+nz*HZ,spec=0;
    if (ndh>0.25f) {spec=ndh*ndh;spec*=spec;spec*=spec;spec*=spec;}
    float env=0.5f+ny*nz;env*=env;
    float k=(0.30f+0.62f*ndl+0.60f*env)*light*grad,w=spec*0.85f*light;
    o[3]=clamp1(k+w);o[4]=clamp1(0.82f*k+w);o[5]=clamp1(0.34f*k+0.9f*w);o[6]=1.0f;
}
static void project(float x,float y,float z,float *o) {
    float xr=x*rc+z*rs,zr=-x*rs+z*rc,depth=CAM_DIST-zr,k=focal/depth;
    o[0]=160.0f+xr*k;o[1]=120.0f-y*k;o[2]=clamp1((depth-(CAM_DIST-170.0f))/340.0f);
}
static float gradient(float y) { return 0.78f+0.44f*clamp1(y/logo_h+0.5f); }

static unsigned draw_cap(float z,float side) {
    /* Cap normal (0,0,side) rotated: (side*s, 0, side*c). */
    float base[7];metal(side*rs,0,side*rc,1.0f,base);
    for (unsigned i=0;i<cap_count;i++) {
        float *o=cap_screen[i];project(cap_points[i][0],cap_points[i][1],z,o);
        float g=gradient(cap_points[i][1]);
        o[3]=clamp1(base[3]*g);o[4]=clamp1(base[4]*g);o[5]=clamp1(base[5]*g);o[6]=1.0f;
    }
    for (unsigned t=0;t<cap_tri_count;t++) {
        const uint16_t *i=&cap_tris[t*3];
        rdpq_triangle(&TRIFMT_ZBUF_SHADE,cap_screen[i[0]],cap_screen[i[1]],cap_screen[i[2]]);
    }
    return cap_tri_count;
}
static unsigned draw_sides(void) {
    float zf=logo_d*0.5f,zb=-zf,limit=rs*CAM_DIST;unsigned n=0;
    float v[4][7];
    for (unsigned e=0;e<edge_count;e++) {
        const logo_edge_t *d=&edges[e];
        if (d->k-d->fnx*limit<=0) continue;   /* faces away: k - n.x*s*dist is the facing test */
        project(d->ax,d->ay,zf,v[0]);project(d->ax,d->ay,zb,v[1]);
        project(d->bx,d->by,zf,v[2]);project(d->bx,d->by,zb,v[3]);
        metal(d->nax*rc,d->nay,-d->nax*rs,gradient(d->ay),v[0]);memcpy(&v[1][3],&v[0][3],16);
        metal(d->nbx*rc,d->nby,-d->nbx*rs,gradient(d->by),v[2]);memcpy(&v[3][3],&v[2][3],16);
        rdpq_triangle(&TRIFMT_ZBUF_SHADE,v[0],v[1],v[3]);
        rdpq_triangle(&TRIFMT_ZBUF_SHADE,v[0],v[3],v[2]);
        n+=2;
    }
    return n;
}

static bool (*idle_hook)(void);
void msx_neogeo_logo_set_idle(bool (*idle)(void)) { idle_hook=idle; }

bool msx_neogeo_logo(void) {
    if (!load_mesh()) {debugf("MSX LOGO skipped: no rom:/neogeo-logo.mesh (tools/build_neogeo_logo.py)\n");return false;}
    float yaw=START_YAW;
    for (unsigned i=0;i<YAW_STEPS;i++) {yaw_table[i]=yaw;yaw+=speed_at((float)i+0.5f);}
    {float hx=LX,hy=LY,hz=LZ+1.0f,l=1.0f/sqrtf(hx*hx+hy*hy+hz*hz);HX=hx*l;HY=hy*l;HZ=hz*l;}
    bool audio=msx_audio_open(MSX_AUDIO_INTRO,"rom:/neogeo-jingle.wav64");
    surface_t zbuf=surface_alloc(FMT_RGBA16,320,240);
    {heap_stats_t h;sys_get_heap_stats(&h);debugf("MSX MEM logo: heap used=%d KB free=%d KB\n",h.used/1024,(h.total-h.used)/1024);}
    debugf("MSX LOGO start caps=%u edges=%u cap_tris=%u size=%.1fx%.1fx%.1f audio=%u\n",cap_count,edge_count,cap_tri_count,(double)logo_w,(double)logo_h,(double)logo_d,audio);
    /* Prime the first frame before starting the clock and the jingle. */
    {surface_t *s=display_get();rdpq_attach_clear(s,NULL);rdpq_detach_show();}
    rspq_wait();
    if (audio) msx_audio_play(MSX_AUDIO_INTRO,false,1.0f);
    uint32_t began=TICKS_READ(),last_frame=began,frame_max=0,cpu_max=0;uint64_t cpu_sum=0;
    unsigned frames=0,late=0,max_drawn=0;bool previous=true,skipped=false;
    uint32_t idle_sum=0;
    surface_t *prev=NULL;
    for (;;) {
        msx_audio_service();joypad_poll();
        bool start=joypad_get_buttons(JOYPAD_PORT_1).start;
        float t=(float)TICKS_TO_US(TICKS_READ()-began)*(VI_HZ/1000000.0f);
        if (start && !previous && t>=15.0f) {skipped=true;break;}
        previous=start;
        if (t>=T_END) break;
        surface_t *surface=display_get();
        uint32_t now=TICKS_READ(),cpu_began=now;
        if (frames) {uint32_t d=now-last_frame;if (d>frame_max) frame_max=d;if (d>TICKS_FROM_US(25000)) late++;}
        last_frame=now;
        t=(float)TICKS_TO_US(now-began)*(VI_HZ/1000000.0f);
        float white=t<T_SPIN_END?0:(t<T_WHITE?(t-T_SPIN_END)/(T_WHITE-T_SPIN_END):1);
        unsigned drawn=0;
        if (t<T_WHITE) {
            float trail=0.72f*smooth(170,300,t);
            rdpq_attach(surface,&zbuf);
            rdpq_clear_z(ZBUF_MAX);
            if (trail>0 && prev) {
                /* Frame feedback: the previous image, darkened toward black, under the new one. */
                rdpq_set_mode_standard();
                rdpq_mode_combiner(RDPQ_COMBINER1((TEX0,ENV,PRIM,ENV),(0,0,0,1)));
                uint8_t k=(uint8_t)(trail*255.0f);
                rdpq_set_prim_color(RGBA32(k,k,k,255));rdpq_set_env_color(RGBA32(0,0,0,255));
                rdpq_tex_blit(prev,0,0,NULL);
            } else rdpq_clear(RGBA32(0,0,0,255));
            rdpq_set_mode_standard();
            rdpq_mode_combiner(RDPQ_COMBINER_SHADE);
            rdpq_mode_zbuf(true,true);
            rdpq_mode_antialias(AA_STANDARD);
            float a=yaw_at(t)*(float)M_PI/180.0f;rc=cosf(a);rs=sinf(a);
            light=0.12f+0.88f*smooth(0,60,t);
            focal=FOCAL*(0.92f+0.08f*clamp1(t/T_SPIN_END));
            float half=logo_d*0.5f;
            if (rc*CAM_DIST>half) drawn+=draw_cap(half,1.0f);
            if (-rc*CAM_DIST>half) drawn+=draw_cap(-half,-1.0f);
            drawn+=draw_sides();
            if (drawn>max_drawn) max_drawn=drawn;
            if (white>0) {
                rdpq_set_mode_standard();
                rdpq_mode_combiner(RDPQ_COMBINER_FLAT);
                rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
                rdpq_set_prim_color(RGBA32(255,255,255,(uint8_t)(white*255.0f)));
                rdpq_fill_rectangle(0,0,320,240);
            }
        } else {
            float g=t<T_WHITE_END?1:1-clamp1((t-T_WHITE_END)/(T_END-T_WHITE_END));
            uint8_t v=(uint8_t)(g*255.0f);
            rdpq_attach_clear(surface,NULL);rdpq_set_mode_fill(RGBA32(v,v,v,255));rdpq_fill_rectangle(0,0,320,240);
        }
        rdpq_detach_show();msx_frames_present();prev=surface;frames++;
        uint32_t cpu=TICKS_READ()-cpu_began;if (cpu>cpu_max) cpu_max=cpu;
        cpu_sum+=cpu;
        /* Spare time until the next blank: the caller's background work (the
         * game's scene warm-up, scene_assets_idle), never past the blank. */
        if (idle_hook) {uint32_t i0=TICKS_READ();while (idle_hook()) {}idle_sum+=TICKS_READ()-i0;}
        if (frames==60 || frames==150 || frames==230 || frames==330)
            debugf("MSX LOGO shot frame=%u t=%u yaw=%d triangles=%u cpu_us=%lu\n",frames,(unsigned)t,(int)yaw_at(t),drawn,(unsigned long)TICKS_TO_US(cpu));
    }
    rspq_wait();
    /* Leave a black screen for the title. */
    for (int i=0;i<2;i++) {surface_t *s=display_get();rdpq_attach_clear(s,NULL);rdpq_detach_show();}
    rspq_wait();
    msx_audio_stop(MSX_AUDIO_INTRO);
    surface_free(&zbuf);free_mesh();
    float seconds=(float)TICKS_TO_US(TICKS_READ()-began)/1e6f;
    debugf("MSX LOGO timing frames=%u seconds=%.2f frame_max_us=%lu cpu_max_us=%lu cpu_mean_us=%lu late=%u max_triangles=%u idle_mean_us=%lu\n",
        frames,(double)seconds,(unsigned long)TICKS_TO_US(frame_max),(unsigned long)TICKS_TO_US(cpu_max),
        (unsigned long)TICKS_TO_US(cpu_sum/(frames?frames:1)),late,max_drawn,(unsigned long)TICKS_TO_US(idle_sum/(frames?frames:1)));
    debugf("MSX LOGO complete skipped=%u t=%lu ms\n",skipped,(unsigned long)get_ticks_ms());
    return true;
}
