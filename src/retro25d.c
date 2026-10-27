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
    return segments*RETRO_GROUND_ROWS*2+planes*8+shadows*8;
}
