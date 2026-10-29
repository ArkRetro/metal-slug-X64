#include "retro_effects.h"
#include "scene_renderer.h"
#include "retro_assets.h"
#include <math.h>

static unsigned triangles;
static retro_mesh_stats_t last_stats;
static const retro_scene_t scene={retro_ground,RETRO_GROUND_WIDTH,retro_planes,RETRO_PLANE_COUNT};
void retro_effects_init(void) {data_cache_hit_writeback(retro_fog,sizeof(retro_fog));}
float retro_effects_layer_depth(unsigned layer) {assert(layer<5);return retro_layer_depths[layer];}
unsigned retro_effects_triangles(void) {return triangles;}
unsigned retro_effects_visible_planes(void) {return last_stats.visible_planes;}
unsigned retro_effects_shadow_triangles(void) {return last_stats.shadows;}
static void triangle(void *context,const float *a,const float *b,const float *c) {
    (void)context;
    assertf(triangles<RETRO_MAX_TRIANGLES,"Retro mesh triangle budget");
    rdpq_triangle(&TRIFMT_SHADE,a,b,c);triangles++;
}
void retro_effects_mesh(const retro_frame_t *f) {
    // Self-contained state: push/pop the render mode, own the scissor, and
    // never touch TMEM/TLUT (shade-only combiner), so sprite residency survives.
    triangles=0;rdpq_mode_push();rdpq_mode_combiner(RDPQ_COMBINER_SHADE);
    rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);rdpq_mode_alphacompare(0);
    rdpq_set_scissor(8,8,312,232);
    last_stats=retro_build_mesh(f,&scene,triangle,NULL);
    rdpq_set_scissor(0,0,320,240);rdpq_mode_pop();
}
void retro_effects_fog(const retro_frame_t *f,unsigned slice) {
    static const float depths[3]={240,64,-4};assert(slice<3);
    float depth=depths[slice];
    rdpq_mode_push();rdpq_mode_tlut(TLUT_NONE);rdpq_mode_filter(FILTER_BILINEAR);
    rdpq_mode_combiner(RDPQ_COMBINER_TEX_FLAT);rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
    rdpq_mode_alphacompare(1);rdpq_set_scissor(8,8,312,232);
    surface_t texture=surface_make(retro_fog,FMT_IA8,32,32,32);
    int bytes=rdpq_tex_upload(TILE0,&texture,NULL);assert(bytes==RETRO_FOG_BYTES && bytes<=2048);(void)bytes;
    float sample[4];retro_lighting(f,(retro_vec3_t){f->camera.x+160,175,depth},(retro_vec3_t){0,0,1},sample);
    unsigned alpha=retro_fog_alpha(slice,f->focus);
    color_t color={70+(unsigned)(sample[0]*sample[3]*30),80+(unsigned)(sample[1]*sample[3]*25),108,alpha};
    rdpq_set_prim_color(color);
    float drift=(float)((f->tick/(slice+2))%96);
    float projected_camera=f->camera.x*f->camera.focal/(f->camera.focal+depth);
    float first=floorf((projected_camera+drift)/96)*96-drift-96;
    for (unsigned i=0;i<6;i++) {
        float x=first+i*96;
        retro_vec3_t a=retro_billboard_point(x,150+slice*12,depth,f->camera.focal);
        retro_vec3_t b=retro_billboard_point(x+96,214+slice*5,depth,f->camera.focal);
        retro_projected_t p,q;
        if (retro_project(f->camera,a,&p) && retro_project(f->camera,b,&q))
            rdpq_texture_rectangle_scaled(TILE0,p.x,p.y,q.x,q.y,0,0,32,32);
    }
    rdpq_set_scissor(0,0,320,240);rdpq_mode_pop();scene_render_forget_texture();
}
