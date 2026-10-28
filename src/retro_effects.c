#include "retro_effects.h"
#include "scene_renderer.h"
#include "retro_assets.h"
#include <math.h>

static unsigned triangles;
static float floor_y(float x) {
    if (x<0 || x>=RETRO_GROUND_WIDTH) return 192;
    unsigned index=(unsigned)x/8;
    float t=(x-index*8)*.125f;
    return (retro_ground[index]+((int)retro_ground[index+1]-(int)retro_ground[index])*t)*.00390625f;
}
void retro_effects_init(void) {data_cache_hit_writeback(retro_fog,sizeof(retro_fog));}
float retro_effects_layer_depth(unsigned layer) {assert(layer<5);return retro_layer_depths[layer];}
unsigned retro_effects_triangles(void) {return triangles;}
static bool vertex(const retro_frame_t *f,retro_vec3_t p,retro_vec3_t n,float out[6]) {
    retro_projected_t q;if (!retro_project(f->camera,p,&q)) return false;
    out[0]=q.x;out[1]=q.y;retro_lighting(f,p,n,out+2);return true;
}
static void triangle(float *a,float *b,float *c) {
    if (a[5]<=0 && b[5]<=0 && c[5]<=0) return;
    assertf(triangles<RETRO_MAX_TRIANGLES,"Retro mesh triangle budget");
    rdpq_triangle(&TRIFMT_SHADE,a,b,c);triangles++;
}
static void ground_vertex(const retro_frame_t *f,float x,float floor,float slope,unsigned row,float out[6]) {
    float portion=(float)row/RETRO_GROUND_ROWS,y=floor+(232-floor)*portion;
    float dy=-6/(232-floor);
    float dx=6*(232-y)/((232-floor)*(232-floor))*slope;
    bool ok=vertex(f,(retro_vec3_t){x,y,-6*portion},retro_normal(dx,dy),out);assert(ok);(void)ok;
}
void retro_effects_mesh(const retro_frame_t *f) {
    triangles=0;rdpq_mode_push();rdpq_mode_combiner(RDPQ_COMBINER_SHADE);
    rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);rdpq_mode_alphacompare(0);
    rdpq_set_scissor(8,8,312,232);
    // Eight coarse strips: 16 triangles. Collision remains 2D.
    if (f->light_count) {
    float left=floorf(f->camera.x/48)*48;
    float grid[RETRO_GROUND_SEGMENTS+1][RETRO_GROUND_ROWS+1][6];
    for (unsigned i=0;i<=RETRO_GROUND_SEGMENTS;i++) {
        float x=left+i*48,height=floor_y(x),slope=(floor_y(x+1)-floor_y(x-1))*.5f;
        for (unsigned row=0;row<=RETRO_GROUND_ROWS;row++) ground_vertex(f,x,height,slope,row,grid[i][row]);
    }
    for (unsigned i=0;i<RETRO_GROUND_SEGMENTS;i++) for (unsigned row=0;row<RETRO_GROUND_ROWS;row++) {
        triangle(grid[i][row],grid[i+1][row],grid[i+1][row+1]);
        triangle(grid[i][row],grid[i+1][row+1],grid[i][row+1]);
    }
    for (unsigned i=0;i<sizeof(retro_planes)/sizeof(*retro_planes);i++) {
        const retro_plane_t *p=&retro_planes[i];
        if (p->right<f->camera.x-32 || p->left>f->camera.x+352) continue;
        float verts[3][3][6];bool visible=true;
        for (unsigned j=0;j<3;j++) for (unsigned k=0;k<3;k++) {
            float x=p->left+(p->right-p->left)*j*.5f,y=p->top+(p->bottom-p->top)*k*.5f;
            float z=p->depth+(x-p->left)*p->dx+(y-p->top)*p->dy;
            visible&=vertex(f,(retro_vec3_t){x,y,z},retro_normal(p->dx,p->dy),verts[j][k]);
        }
        if (visible) for (unsigned j=0;j<2;j++) for (unsigned k=0;k<2;k++) {
            triangle(verts[j][k],verts[j+1][k],verts[j+1][k+1]);
            triangle(verts[j][k],verts[j+1][k+1],verts[j][k+1]);
        }
    }
    }
    // Contact shadows: eight-vertex feathered fans, with no shadow-map pass.
    for (unsigned i=0;i<f->shadow_count && i<2;i++) {
        const retro_shadow_t *s=&f->shadows[i];float height=s->ground_y-s->feet_y;
        if (height<0) height=0;
        float alpha=height>96?0:25*(1-height/128);if (alpha<=0) continue;
        retro_projected_t projected;
        if (!retro_project(f->camera,(retro_vec3_t){s->world_x,s->ground_y+1,0},&projected)) continue;
        float x=projected.x,y=projected.y;
        float radius=(s->radius>0?s->radius:12)*f->camera.focal*projected.inverse_w;
        if (x+radius<8 || x-radius>=312 || y+radius*.25f<8 || y-radius*.25f>=232) continue;
        float center[6]={x,y,4/255.0f,7/255.0f,15/255.0f,alpha/255};
        for (unsigned j=0;j<8;j++) {
            float a=(float)j*.785398163f,b=(float)(j+1)*.785398163f;
            float edge1[6]={x+cosf(a)*radius,y+sinf(a)*radius*.25f,4/255.0f,7/255.0f,15/255.0f,0};
            float edge2[6]={x+cosf(b)*radius,y+sinf(b)*radius*.25f,4/255.0f,7/255.0f,15/255.0f,0};
            triangle(center,edge1,edge2);
        }
    }
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
    unsigned alpha=slice==2?12:18;
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
