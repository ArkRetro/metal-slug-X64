#include "retro25d.h"
#include <math.h>
static float clamp01(float v) {return v<0?0:v>1?1:v;}
retro_vec3_t retro_normal(float dx,float dy) {
    float scale=1.0f/sqrtf(dx*dx+dy*dy+1);
    // Depth increases away from the camera. Normal-map +Z points at it.
    return (retro_vec3_t){dx*scale,dy*scale,scale};
}
bool retro_project(retro_camera_t c,retro_vec3_t p,retro_projected_t *out) {
    float distance=c.focal+p.z-c.z;
    if (!(c.focal>0) || !(distance>=8)) return false;
    float scale=c.focal/distance;
    *out=(retro_projected_t){160+(p.x-c.x-160)*scale,120+(p.y-c.y-120)*scale,1/distance};
    return true;
}
retro_vec3_t retro_billboard_point(float x,float y,float z,float focal) {
    float inverse_scale=(focal+z)/focal;
    return (retro_vec3_t){160+(x-160)*inverse_scale,120+(y-120)*inverse_scale,z};
}
bool retro_plane_projection(retro_camera_t c,float z,retro_plane_view_t *out) {
    float distance=c.focal+z-c.z;
    if (!(c.focal>0) || !(distance>=8)) return false;
    float ratio=c.focal/distance,scale=(c.focal+z)/distance;
    *out=(retro_plane_view_t){scale,160*(1-scale)-c.x*ratio,120*(1-scale)-c.y*ratio};return true;
}
/* Authored 2D transforms: constant scale, independent scroll, no perspective.
 * Terrain/props stay in the gameplay plane for contact and collision. */
bool retro_layered_2d_projection(retro_camera_t c,unsigned layer,retro_plane_view_t *out) {
    if(layer>=5 || !out) return false;
    static const float speed[5]={.04f,.45f,1,1,1};
    static const float size[5]={1.08f,1.04f,1,1,1};
    float scale=size[layer];
    *out=(retro_plane_view_t){scale,160*(1-scale)-c.x*speed[layer],120*(1-scale)-c.y*speed[layer]};
    return true;
}
bool retro_background_projection(retro_camera_t c,unsigned layer,float depth,retro_plane_view_t *out) {
    if (layer>=5) return false;
    if (layer>=2) {
        *out=(retro_plane_view_t){1,-c.x,-c.y};return true;
    }
    return retro_plane_projection(c,depth,out);
}
float retro_light_energy(retro_vec3_t p,retro_vec3_t n,const retro_light_t *l) {
    if (!(l->radius>0) || !(l->intensity>0)) return 0;
    float x=l->position.x-p.x,y=l->position.y-p.y,z=p.z-l->position.z;
    float d2=x*x+y*y+z*z,r2=l->radius*l->radius;
    if (d2>=r2) return 0;
    float inverse=d2>0.0001f?1/sqrtf(d2):0;
    float dot=d2>0.0001f?(n.x*x+n.y*y+n.z*z)*inverse:1;
    float attenuation=1-d2/r2;
    return clamp01(dot)*attenuation*attenuation*l->intensity;
}
void retro_lighting(const retro_frame_t *f,retro_vec3_t p,retro_vec3_t n,float out[4]) {
    // Keep the artwork's baked ambient lighting. Only dynamic contributions
    // require a blending pass, so unlit wrappers can be culled entirely.
    float weight=0,r=0,g=0,b=0;
    for (unsigned i=0;i<f->light_count && i<RETRO_MAX_LIGHTS;i++) {
        float glow=clamp01(retro_light_energy(p,n,&f->lights[i]))*.30f;
        weight+=glow;r+=f->lights[i].color.x*glow;
        g+=f->lights[i].color.y*glow;b+=f->lights[i].color.z*glow;
    }
    out[0]=weight?clamp01(r/weight/255):0;out[1]=weight?clamp01(g/weight/255):0;out[2]=weight?clamp01(b/weight/255):0;
    out[3]=clamp01(weight);
}
unsigned retro_mesh_budget(unsigned segments,unsigned planes,unsigned shadows) {
    return segments*RETRO_GROUND_ROWS*2+planes*RETRO_PLANE_TRIANGLES+shadows*RETRO_SHADOW_SEGMENTS;
}

/* ---- Frame builders ---------------------------------------------------- */
retro_shadow_t retro_actor_shadow(float x,float feet,float ground,float half_width) {
    float radius=half_width*1.15f;
    radius=radius<6?6:radius>64?64:radius;
    return (retro_shadow_t){x,feet,ground,radius,.25f};
}
bool retro_frame_add_shadow(retro_frame_t *f,retro_shadow_t s) {
    if (!(s.radius>0) || !(s.radius<=256) || f->shadow_count>=RETRO_MAX_SHADOWS) return false;
    f->shadows[f->shadow_count++]=s;return true;
}
bool retro_frame_add_light(retro_frame_t *f,retro_light_t l) {
    if (!(l.radius>0) || !(l.intensity>0)) return false;
    if (f->light_count<RETRO_MAX_LIGHTS) {f->lights[f->light_count++]=l;return true;}
    unsigned weakest=0;
    for (unsigned i=1;i<RETRO_MAX_LIGHTS;i++) if (f->lights[i].intensity<f->lights[weakest].intensity) weakest=i;
    if (!(l.intensity>f->lights[weakest].intensity)) return false;
    f->lights[weakest]=l;return true;
}

/* ---- Pulses -------------------------------------------------------------- */
bool retro_pulse_light(const retro_pulse_t *p,unsigned tick,retro_light_t *out) {
    unsigned age=tick-p->start; // Unsigned: a tick before start (reset) is expired.
    if (!p->duration || age>=p->duration || !(p->intensity>0) || !(p->radius>0)) return false;
    float t=(float)age/(float)p->duration,k=(1-t)*(1-t);
    *out=(retro_light_t){p->position,p->color,p->radius*(.75f+.25f*t),p->intensity*k};
    return true;
}
static float pulse_energy(const retro_pulse_t *p,unsigned tick) {
    retro_light_t l;return retro_pulse_light(p,tick,&l)?l.intensity:0;
}
void retro_pulses_spawn(retro_pulses_t *pool,retro_pulse_t p,unsigned tick) {
    p.start=tick;
    if (pool->count<RETRO_MAX_PULSES) {pool->pulses[pool->count++]=p;return;}
    unsigned weakest=0;
    for (unsigned i=1;i<RETRO_MAX_PULSES;i++)
        if (pulse_energy(&pool->pulses[i],tick)<pulse_energy(&pool->pulses[weakest],tick)) weakest=i;
    if (pulse_energy(&pool->pulses[weakest],tick)<=p.intensity) pool->pulses[weakest]=p;
}
unsigned retro_pulses_emit(retro_pulses_t *pool,unsigned tick,retro_frame_t *f) {
    unsigned kept=0,lit=0;
    for (unsigned i=0;i<pool->count && i<RETRO_MAX_PULSES;i++) {
        retro_light_t l;
        if (!retro_pulse_light(&pool->pulses[i],tick,&l)) continue;
        pool->pulses[kept++]=pool->pulses[i];
        lit+=retro_frame_add_light(f,l);
    }
    pool->count=kept;return lit;
}

/* ---- Fog density ----------------------------------------------------------- */
unsigned retro_fog_alpha(unsigned slice,float focus) {
    focus=clamp01(focus);
    // Far sheets gain at most 4/255 (mid 2/255); the foreground sheet over the
    // sprites never changes, so character pixels see the same tint as before.
    return slice==2?12:18+(unsigned)((slice==0?4:2)*focus+.5f);
}
float retro_focus_step(float focus,float dx) {
    float target=clamp01(fabsf(dx)*.5f);
    focus+=(target-focus)*(target>focus?.12f:.04f);
    return clamp01(focus);
}

/* ---- Mesh ---------------------------------------------------------------- */
float retro_ground_height(const retro_scene_t *s,float x) {
    if (!s->ground || !(x>=0) || x>=s->ground_width) return 192;
    unsigned index=(unsigned)x/8;
    float t=(x-index*8)*.125f;
    return (s->ground[index]+((int)s->ground[index+1]-(int)s->ground[index])*t)*.00390625f;
}
bool retro_plane_in_view(const retro_plane_t *p,float camera_x) {
    return p->right>=camera_x-RETRO_CULL_MARGIN && p->left<=camera_x+320+RETRO_CULL_MARGIN;
}
unsigned retro_shadow_fan(retro_camera_t c,const retro_shadow_t *s,float center[6],float rim[RETRO_SHADOW_SEGMENTS+1][6]) {
    // Unit circle at 45-degree steps; no trigonometry per frame.
    static const float dir[RETRO_SHADOW_SEGMENTS+1][2]={{1,0},{.70710678f,.70710678f},{0,1},
        {-.70710678f,.70710678f},{-1,0},{-.70710678f,-.70710678f},{0,-1},{.70710678f,-.70710678f},{1,0}};
    float height=s->ground_y-s->feet_y;if (!(height>0)) height=0;
    float alpha=height>96?0:25*(1-height/128);if (alpha<=0) return 0;
    retro_projected_t q;
    if (!retro_project(c,(retro_vec3_t){s->world_x,s->ground_y+1,0},&q)) return 0;
    float squash=s->squash>0?s->squash:.25f;
    // Airborne actors cast a slightly smaller, lighter blob (up to 25% smaller).
    float radius=(s->radius>0?s->radius:12)*c.focal*q.inverse_w*(1-.25f*(height>96?96:height)/96);
    // Sit the ellipse mostly on the receding ground strip below the contact line.
    float ry=radius*squash,x=q.x,y=q.y+ry*.4f;
    if (x+radius<8 || x-radius>=312 || y+ry<8 || y-ry>=232) return 0;
    static const float ink[3]={4/255.0f,7/255.0f,15/255.0f};
    float v[6]={x,y,ink[0],ink[1],ink[2],alpha/255};
    for (unsigned i=0;i<6;i++) center[i]=v[i];
    for (unsigned j=0;j<=RETRO_SHADOW_SEGMENTS;j++) {
        float e[6]={x+dir[j][0]*radius,y+dir[j][1]*ry,ink[0],ink[1],ink[2],0};
        for (unsigned i=0;i<6;i++) rim[j][i]=e[i];
    }
    return RETRO_SHADOW_SEGMENTS;
}
typedef struct { retro_emit_fn emit;void *context;unsigned count; } emitter_t;
static bool emit(emitter_t *e,const float *a,const float *b,const float *c) {
    if (a[5]<=0 && b[5]<=0 && c[5]<=0) return false; // Unlit: baked art untouched.
    if (e->count>=RETRO_MAX_TRIANGLES) return false;
    e->emit(e->context,a,b,c);e->count++;return true;
}
static bool vertex(const retro_frame_t *f,retro_vec3_t p,retro_vec3_t n,float out[6]) {
    retro_projected_t q;if (!retro_project(f->camera,p,&q)) return false;
    out[0]=q.x;out[1]=q.y;retro_lighting(f,p,n,out+2);return true;
}
retro_mesh_stats_t retro_build_mesh(const retro_frame_t *f,const retro_scene_t *s,retro_emit_fn fn,void *context) {
    retro_mesh_stats_t stats={0};emitter_t e={fn,context,0};
    // Eight coarse strips: 16 triangles. Collision remains 2D.
    if (f->light_count) {
        float left=floorf(f->camera.x/48)*48;
        float grid[RETRO_GROUND_SEGMENTS+1][RETRO_GROUND_ROWS+1][6];bool ground=true;
        for (unsigned i=0;i<=RETRO_GROUND_SEGMENTS;i++) {
            float x=left+i*48,floor=retro_ground_height(s,x);
            float slope=(retro_ground_height(s,x+1)-retro_ground_height(s,x-1))*.5f;
            for (unsigned row=0;row<=RETRO_GROUND_ROWS;row++) {
                float portion=(float)row/RETRO_GROUND_ROWS,y=floor+(232-floor)*portion;
                float dy=-6/(232-floor),dx=6*(232-y)/((232-floor)*(232-floor))*slope;
                ground&=vertex(f,(retro_vec3_t){x,y,-6*portion},retro_normal(dx,dy),grid[i][row]);
            }
        }
        if (ground) for (unsigned i=0;i<RETRO_GROUND_SEGMENTS;i++) for (unsigned row=0;row<RETRO_GROUND_ROWS;row++) {
            stats.ground+=emit(&e,grid[i][row],grid[i+1][row],grid[i+1][row+1]);
            stats.ground+=emit(&e,grid[i][row],grid[i+1][row+1],grid[i][row+1]);
        }
        // Planes are sorted by left edge: stop at the first one past the view.
        for (unsigned i=0;i<s->plane_count;i++) {
            const retro_plane_t *p=&s->planes[i];
            if (p->left>f->camera.x+320+RETRO_CULL_MARGIN) break;
            if (!retro_plane_in_view(p,f->camera.x)) {stats.culled_planes++;continue;}
            if (stats.visible_planes>=RETRO_MAX_VISIBLE_PLANES) break;
            stats.visible_planes++;
            float verts[3][3][6];bool visible=true;
            for (unsigned j=0;j<3;j++) for (unsigned k=0;k<3;k++) {
                float x=p->left+(p->right-p->left)*j*.5f,y=p->top+(p->bottom-p->top)*k*.5f;
                float z=p->depth+(x-p->left)*p->dx+(y-p->top)*p->dy;
                visible&=vertex(f,(retro_vec3_t){x,y,z},retro_normal(p->dx,p->dy),verts[j][k]);
            }
            if (visible) for (unsigned j=0;j<2;j++) for (unsigned k=0;k<2;k++) {
                stats.planes+=emit(&e,verts[j][k],verts[j+1][k],verts[j+1][k+1]);
                stats.planes+=emit(&e,verts[j][k],verts[j+1][k+1],verts[j][k+1]);
            }
        }
    }
    // Contact shadows: eight-triangle feathered fans, with no shadow-map pass.
    for (unsigned i=0;i<f->shadow_count && i<RETRO_MAX_SHADOWS;i++) {
        float center[6],rim[RETRO_SHADOW_SEGMENTS+1][6];
        unsigned n=retro_shadow_fan(f->camera,&f->shadows[i],center,rim);
        for (unsigned j=0;j<n;j++) stats.shadows+=emit(&e,center,rim[j],rim[j+1]);
    }
    return stats;
}
