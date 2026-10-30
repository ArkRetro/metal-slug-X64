#include <libdragon.h>
#include <stdio.h>
#include "vehicle_renderer.h"
#include "scene_renderer.h"
#include "mission_camera.h"
static sprite_t *poses[9][2][2];
static bool loaded;
void vehicle_renderer_load(void) {
    if (loaded) return;
    char path[64];
    for (unsigned pose=0;pose<9;pose++) for (unsigned flash=0;flash<2;flash++) for (unsigned part=0;part<2;part++) {
        snprintf(path,sizeof(path),"rom:/vehicles/v%u-%u-%u.sprite",pose,flash,part);
        sprite_t *s=sprite_load(path);
        assertf(s && s->width==64 && s->height==64 && sprite_get_format(s)==FMT_CI4,"Vehicle CI4 sprite asset");
        surface_t pixels=sprite_get_pixels(s);
        assertf((unsigned)pixels.stride*pixels.height==2048,"Vehicle TMEM texture budget");
        poses[pose][flash][part]=s;
    }
    loaded=true;debugf("MSX vehicle sprites: 36 CI4 halves, 2048 B maximum TMEM upload\n");
}
void vehicle_renderer_draw(const neo_vehicle_t *v,int camera,int height,bool flash) {
    if (!v->active) return;
    assertf(loaded && v->pose<9,"Vehicle renderer state");
    bool mirror=v->facing>0;
    int x=v->x/65536-camera-64-(mirror?1:0),y=248-v->y/65536-64+height;
    rdpq_mode_push();rdpq_mode_filter(FILTER_POINT);rdpq_mode_alphacompare(1);
    for (unsigned part=0;part<2;part++) {
        int left=x+(mirror?64-(int)part*64:(int)part*64);
        if (left>=320 || left+64<=0) continue;
        // The pinned SDK's multi-upload return count is a TODO (always zero).
        // Validate actual format/stride at load; its uploader also checks TMEM.
        rdpq_sprite_upload(TILE0,poses[v->pose][flash][part],NULL);
        rdpq_texture_rectangle_scaled(TILE0,left,y,left+64,y+64,mirror?63:0,0,mirror?-1:64,64);
    }
    rdpq_mode_pop();scene_render_forget_texture();
}
