#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "../src/retro25d.h"
static void near(float a,float b) {assert(fabsf(a-b)<.0001f);}
int main(void) {
    retro_camera_t camera={64,0,0,320};retro_projected_t q;
    const float depths[]={0,320.0f/3,320},ratios[]={1,.75,.5};
    for (unsigned i=0;i<3;i++) {
        retro_vec3_t p=retro_billboard_point(100,80,depths[i],320);
        assert(retro_project(camera,p,&q));near(q.x,100-64*ratios[i]);near(q.y,80);
        retro_plane_view_t plane;assert(retro_plane_projection(camera,depths[i],&plane));
        near(100*plane.scale+plane.offset_x,q.x);near(80*plane.scale+plane.offset_y,q.y);
    }
    assert(!retro_project(camera,(retro_vec3_t){0,0,-313},&q));
    retro_vec3_t p={200,100,0};assert(retro_project(camera,p,&q));float before=q.x;
    camera.z=32;assert(retro_project(camera,p,&q));assert(q.x<before); // Forward camera motion changes perspective.
    retro_vec3_t n=retro_normal(.25f,-.125f);
    near(n.x-.25f*n.z,0);near(n.y+.125f*n.z,0);near(n.x*n.x+n.y*n.y+n.z*n.z,1);
    retro_light_t l={{0,0,-16},{255,160,64},64,1};
    float close=retro_light_energy((retro_vec3_t){0,0,0},(retro_vec3_t){0,0,1},&l);
    float far=retro_light_energy((retro_vec3_t){30,0,0},(retro_vec3_t){0,0,1},&l);
    assert(close>far && far>0);
    l.position.z=16;near(retro_light_energy((retro_vec3_t){0,0,0},(retro_vec3_t){0,0,1},&l),0);
    l.radius=0;near(retro_light_energy(p,n,&l),0);
    retro_frame_t f={0};f.light_count=2;f.lights[0]=(retro_light_t){{0,0,-16},{255,160,64},64,1};f.lights[1]=f.lights[0];
    float rgba[4];retro_lighting(&f,(retro_vec3_t){0,0,0},n,rgba);
    for (unsigned i=0;i<4;i++) assert(rgba[i]>=0 && rgba[i]<=1);
    f.light_count=0;retro_lighting(&f,p,n,rgba);near(rgba[3],0); // Preserve baked art; skip unlit overlays.
    assert(retro_mesh_budget(RETRO_GROUND_SEGMENTS,4,2)==64 && retro_mesh_budget(RETRO_GROUND_SEGMENTS,4,2)<=RETRO_MAX_TRIANGLES);
    assert(retro_mesh_budget(RETRO_GROUND_SEGMENTS,20,2)>RETRO_MAX_TRIANGLES);
    puts("PASS: perspective/parallax, near plane, depth normals, light falloff/backfaces, normalized RDP shades and 96-triangle budget");
}
