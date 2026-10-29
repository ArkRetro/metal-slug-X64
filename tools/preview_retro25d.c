/* Offline preview of the depth25d effects using the real mesh/light/pulse code.
 *
 * Draws NO original artwork: the level is a flat proxy (sky gradient, the
 * authored ground profile and plane rectangles) and actors are grey boxes.
 * On top of it this program rasterises exactly the triangles that
 * retro_build_mesh() would hand to rdpq_triangle(), and the fog sheets of
 * retro_effects_fog(), with the RDP's alpha blend. Output: binary PPM.
 * The lower half of each image is the effect alone: 0.5 + 4*(with - without)
 * per channel, against the same frame without lights/shadows (grey = none).
 *   preview_retro25d <scenario> <out.ppm>   (scenario: vehicle|impact|decay)
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/retro25d.h"
#include "../src/retro_events.h"
#include "retro_assets.h"

enum { W=320,H=240 };
static float image[H][W][3];
static void blend(int x,int y,float r,float g,float b,float a) {
    if (x<8 || x>=312 || y<8 || y>=232 || a<=0) return; // Same scissor as the N64 passes.
    if (a>1) a=1;
    float *p=image[y][x];p[0]=r*a+p[0]*(1-a);p[1]=g*a+p[1]*(1-a);p[2]=b*a+p[2]*(1-a);
}
static void fill(int x0,int y0,int x1,int y1,float r,float g,float b,float a) {
    for (int y=y0;y<y1;y++) for (int x=x0;x<x1;x++) blend(x,y,r,g,b,a);
}
static void triangle(void *context,const float a[6],const float b[6],const float c[6]) {
    unsigned *count=context;(*count)++;
    float minx=fminf(a[0],fminf(b[0],c[0])),maxx=fmaxf(a[0],fmaxf(b[0],c[0]));
    float miny=fminf(a[1],fminf(b[1],c[1])),maxy=fmaxf(a[1],fmaxf(b[1],c[1]));
    float area=(b[0]-a[0])*(c[1]-a[1])-(b[1]-a[1])*(c[0]-a[0]);
    if (fabsf(area)<1e-6f) return;
    for (int y=(int)floorf(miny);y<=(int)ceilf(maxy);y++) for (int x=(int)floorf(minx);x<=(int)ceilf(maxx);x++) {
        float px=x+.5f,py=y+.5f;
        float w0=((b[0]-px)*(c[1]-py)-(b[1]-py)*(c[0]-px))/area;
        float w1=((c[0]-px)*(a[1]-py)-(c[1]-py)*(a[0]-px))/area;
        float w2=1-w0-w1;
        if (w0<0 || w1<0 || w2<0) continue;
        float v[4];for (unsigned i=0;i<4;i++) v[i]=a[2+i]*w0+b[2+i]*w1+c[2+i]*w2;
        blend(x,y,v[0],v[1],v[2],v[3]);
    }
}
static float fog_texel(int u,int v,bool alpha) {
    uint8_t t=retro_fog[((v&31)*32)+(u&31)];
    return alpha?(t&15)/15.0f:(t>>4)/15.0f;
}
static void fog(const retro_frame_t *f,unsigned slice) {
    // Mirrors retro_effects_fog(): TEX_FLAT combiner, bilinear IA8, same sheets.
    static const float depths[3]={240,64,-4};float depth=depths[slice];
    float sample[4];retro_lighting(f,(retro_vec3_t){f->camera.x+160,175,depth},(retro_vec3_t){0,0,1},sample);
    float prim[4]={(70+(unsigned)(sample[0]*sample[3]*30))/255.0f,(80+(unsigned)(sample[1]*sample[3]*25))/255.0f,
        108/255.0f,retro_fog_alpha(slice,f->focus)/255.0f};
    float drift=(float)((f->tick/(slice+2))%96);
    float projected=f->camera.x*f->camera.focal/(f->camera.focal+depth);
    float first=floorf((projected+drift)/96)*96-drift-96;
    for (unsigned i=0;i<6;i++) {
        float x=first+i*96;retro_projected_t p,q;
        retro_vec3_t a=retro_billboard_point(x,150+slice*12,depth,f->camera.focal);
        retro_vec3_t b=retro_billboard_point(x+96,214+slice*5,depth,f->camera.focal);
        if (!retro_project(f->camera,a,&p) || !retro_project(f->camera,b,&q)) continue;
        for (int y=(int)p.y;y<(int)q.y;y++) for (int xx=(int)p.x;xx<(int)q.x;xx++) {
            float s=(xx-p.x)/(q.x-p.x)*32-.5f,t=(y-p.y)/(q.y-p.y)*32-.5f;
            int u=(int)floorf(s),v=(int)floorf(t);float fu=s-u,fv=t-v,I=0,A=0;
            for (int k=0;k<4;k++) {
                int du=k&1,dv=k>>1;float w=(du?fu:1-fu)*(dv?fv:1-fv);
                int cu=u+du<0?0:u+du>31?31:u+du,cv=v+dv<0?0:v+dv>31?31:v+dv; // Clamped like the RDP tile.
                I+=w*fog_texel(cu,cv,false);A+=w*fog_texel(cu,cv,true);
            }
            blend(xx,y,prim[0]*I,prim[1]*I,prim[2]*I,prim[3]*A);
        }
    }
}
static const retro_scene_t scene={retro_ground,RETRO_GROUND_WIDTH,retro_planes,RETRO_PLANE_COUNT};
static void proxy_level(void) {
    // Neutral proxy: no original pixels are reproduced.
    for (int y=0;y<H;y++) for (int x=0;x<W;x++) {
        float t=y/240.0f;image[y][x][0]=.62f+.20f*t;image[y][x][1]=.70f+.14f*t;image[y][x][2]=.82f-.05f*t;
    }
    fill(0,0,W,8,0,0,0,1);fill(0,232,W,H,0,0,0,1);fill(0,0,8,H,0,0,0,1);fill(312,0,W,H,0,0,0,1);
}
static void proxy_ground(float camera) {
    for (int x=8;x<312;x++) {
        float top=retro_ground_height(&scene,camera+x);
        for (int y=(int)top;y<232;y++) blend(x,y,.78f,.66f,.46f,1);
        blend(x,(int)top,.45f,.36f,.24f,1);
    }
    for (unsigned i=0;i<RETRO_PLANE_COUNT;i++) {
        const retro_plane_t *p=&retro_planes[i];
        int l=(int)(p->left-camera),r=(int)(p->right-camera),t=(int)p->top,b=(int)p->bottom;
        fill(l,t,r,b,.74f,.70f,.62f,1);
        fill(l,t,r,t+1,.40f,.36f,.30f,1);fill(l,b-1,r,b,.40f,.36f,.30f,1);
        fill(l,t,l+1,b,.40f,.36f,.30f,1);fill(r-1,t,r,b,.40f,.36f,.30f,1);
    }
}
static void proxy_actor(float camera,float x,float feet,int half,int height) {
    int l=(int)(x-camera)-half,r=(int)(x-camera)+half,t=(int)feet-height,b=(int)feet;
    fill(l,t,r,b,.35f,.38f,.42f,1);fill(l+1,t+1,r-1,b-1,.55f,.58f,.62f,1);
}
static unsigned render(const retro_frame_t *f,const float actors[][4],unsigned actor_count) {
    unsigned count=0;proxy_level();fog(f,0);fog(f,1);proxy_ground(f->camera.x);
    retro_build_mesh(f,&scene,triangle,&count);
    if (count>RETRO_MAX_TRIANGLES) {fprintf(stderr,"budget\n");exit(1);}
    for (unsigned i=0;i<actor_count;i++) proxy_actor(f->camera.x,actors[i][0],actors[i][1],(int)actors[i][2],(int)actors[i][3]);
    fog(f,2);return count;
}
static void write_ppm(const char *path,int width,int height,const float (*pixels)[3]) {
    FILE *out=fopen(path,"wb");if (!out) {perror(path);exit(1);}
    fprintf(out,"P6\n%d %d\n255\n",width,height);
    for (int i=0;i<width*height;i++) for (int c=0;c<3;c++) {
        float v=pixels[i][c];fputc((int)lrintf((v<0?0:v>1?1:v)*255),out);
    }
    fclose(out);
}
/* Renders the frame and, below it, the effect alone (grey = unchanged). */
static float composite[H*2][W*2][3];
static unsigned compose(const retro_frame_t *f,const float actors[][4],unsigned n,int ox,int oy,int half,int stride_h) {
    static float with[H][W][3];
    retro_frame_t plain={.camera=f->camera,.tick=f->tick};
    render(&plain,actors,n);memcpy(with,image,sizeof(image));
    unsigned triangles=render(f,actors,n);
    // Effect view: per-channel change on mid grey, 0.5 + 4*(with - without).
    for (int y=0;y<H;y+=half) for (int x=0;x<W;x+=half) for (int c=0;c<3;c++) {
        float a=0,d=0;
        for (int k=0;k<half*half;k++) {int yy=y+k/half,xx=x+k%half;a+=image[yy][xx][c];d+=image[yy][xx][c]-with[yy][xx][c];}
        composite[oy+y/half][ox+x/half][c]=a/(half*half);
        composite[oy+stride_h+y/half][ox+x/half][c]=.5f+4*d/(half*half);
    }
    return triangles;
}
static void write_composite(const char *path,int width,int height) {
    static float packed[H*2*W*2][3];
    for (int y=0;y<height;y++) for (int x=0;x<width;x++) memcpy(packed[y*width+x],composite[y][x],sizeof(packed[0]));
    write_ppm(path,width,height,packed);
}
/* Vehicle scene at the observed reference position, driven through retro_events. */
static const retro_frame_t *vehicle_frame(unsigned age,float actors[][4],unsigned *n) {
    static msx_game_t g;static retro_events_t e;static retro_frame_t f;
    memset(&g,0,sizeof(g));memset(&e,0,sizeof(e));g.camera=872;
    g.vehicle=(neo_vehicle_t){.x=937*65536+32768,.y=64*65536,.health=300,.max_health=300,.facing=1,.active=true};
    for (unsigned tick=100;tick<=101+age;tick++) {
        g.ticks=tick;
        if (tick==101) {g.vehicle.health=0;g.vehicle_events|=NEO_VEHICLE_EXPLOSION;}
        // Built like actor.c: the player's shadow first, then the observed events.
        f=(retro_frame_t){.camera={872,0,0,320},.tick=tick};
        retro_frame_add_shadow(&f,retro_actor_shadow(1020,184,184,12));
        retro_events_frame(&e,&g,&f);
    }
    actors[0][0]=937.5f;actors[0][1]=184-2;actors[0][2]=46;actors[0][3]=36;
    actors[1][0]=1020;actors[1][1]=184;actors[1][2]=9;actors[1][3]=36;*n=2;
    return &f;
}
int main(int argc,char **argv) {
    if (argc!=3) {fprintf(stderr,"usage: %s vehicle|impact|decay out.ppm\n",argv[0]);return 2;}
    float actors[4][4];unsigned n=0,triangles=0;
    if (!strcmp(argv[1],"vehicle")) {
        const retro_frame_t *f=vehicle_frame(3,actors,&n);
        triangles=compose(f,(const float(*)[4])actors,n,0,0,1,H);write_composite(argv[2],W,H*2);
    } else if (!strcmp(argv[1],"impact")) {
        // Marco mid-jump fires at a soldier standing in front of the first wall.
        retro_frame_t f={.camera={560,0,0,320},.tick=200};
        retro_frame_add_shadow(&f,retro_actor_shadow(640,184-26,retro_ground_height(&scene,640),12));
        float soldier_ground=retro_ground_height(&scene,780);
        retro_frame_add_shadow(&f,retro_actor_shadow(780,soldier_ground,soldier_ground,10));
        retro_frame_add_light(&f,(retro_light_t){{664,168-26,-18},{255,174,72},72,.66f});
        retro_pulses_t pool={0};
        retro_pulses_spawn(&pool,(retro_pulse_t){{773,142,-12},{255,214,140},36,.55f,0,6},199);
        retro_pulses_emit(&pool,200,&f);
        actors[0][0]=640;actors[0][1]=184-26;actors[0][2]=9;actors[0][3]=36;
        actors[1][0]=780;actors[1][1]=soldier_ground;actors[1][2]=8;actors[1][3]=36;n=2;
        triangles=compose(&f,(const float(*)[4])actors,n,0,0,1,H);write_composite(argv[2],W,H*2);
    } else if (!strcmp(argv[1],"decay")) {
        // Four half-resolution panels: explosion age 0, 6, 12 and 24 ticks.
        static const unsigned ages[4]={0,6,12,24};
        for (unsigned k=0;k<4;k++) {
            const retro_frame_t *f=vehicle_frame(ages[k],actors,&n);
            unsigned t=compose(f,(const float(*)[4])actors,n,k*160,0,2,H/2);if (t>triangles) triangles=t;
        }
        write_composite(argv[2],640,H);
    } else return 2;
    printf("{\"scenario\":\"%s\",\"triangles\":%u,\"limit\":%u}\n",argv[1],triangles,RETRO_MAX_TRIANGLES);
    return 0;
}
