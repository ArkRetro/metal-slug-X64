/* Actual RDP regression for persistent foreground edits. The independent
 * bitplane crops and the old, validated 16px overlay are both references. */
#include <libdragon.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "../src/campaign_footprint_renderer.h"
#include "../src/cinematic_codec.h"
#include "../src/mission_camera.h"
#include "../src/scene_renderer.h"
#include "footprint_expected.h"

enum { PIXELS=320*240, BYTES=PIXELS*2, MAX_EDITS=360 };
typedef struct {int column,row;} edit_t;
typedef struct {const scene_image_t *image;uint32_t crc;} source_t;
static source_t sources[MAX_EDITS];
static unsigned source_count,samples,checked_pixels,transparent,changed_edits,no_op_samples,multi_samples;
static uint16_t *baseline,*reference,*actual;

static void require(bool ok,const char *operation) {
    if (!ok) {debugf("MSX FOOTPRINT COW FAIL operation=%s\n",operation);abort();}
}
static void compare(const uint16_t *got,const uint16_t *expected,const char *operation,
    unsigned mode,unsigned camera) {
    unsigned differences=0;
    for (unsigned i=0;i<PIXELS;i++) if (got[i]!=expected[i]) {
        if (differences<8) debugf("MSX FOOTPRINT COW mismatch=%s mode=%u camera=%u xy=%u,%u got=%04x expected=%04x\n",
            operation,mode,camera,i%320,i/320,got[i],expected[i]);
        differences++;
    }
    require(!differences,operation);checked_pixels+=PIXELS;
}
static void render(uint16_t *output,unsigned mode,unsigned camera,unsigned phase,
    const edit_t *edits,unsigned count,bool overlay) {
    retro_frame_t frame={.camera={camera,-msx_camera_height(camera),0,320},.tick=phase};
    /* Shadows belong above the rewritten foreground. The old overlay needs
     * them deferred, otherwise it erases them while rebuilding its ROI. */
    if (mode) require(retro_frame_add_shadow(&frame,retro_actor_shadow(camera+280,192,192,12)),"shadow setup");
    scene_render_set_depth_mode(mode!=0);scene_render_depth_frame(&frame);
    scene_render_defer_shadows(overlay);
    surface_t *surface=display_get();scene_render_begin(surface);
    rdpq_mode_dithering(DITHER_NONE_NONE);scene_render_background(camera,phase);
    if (overlay) {
        for (unsigned i=0;i<count;i++) require(campaign_footprint_renderer_draw_stamp(
            edits[i].column,edits[i].row,camera,mode!=0),"reference overlay");
        scene_render_forget_texture();scene_render_shadows();
    }
    scene_render_defer_shadows(false);
    scene_render_end();rspq_wait();
    memcpy(output,surface->buffer,BYTES);
}
static void remember_sources(const edit_t *edits,unsigned edits_count) {
    unsigned count;const scene_map_cell_t *cells=scene_map_cells(&count);source_count=0;
    for (unsigned e=0;e<edits_count;e++) {
        int x=edits[e].column*16,y=112+edits[e].row*16;
        for (unsigned i=0;i<count;i++) {
            const scene_map_cell_t *cell=&cells[i];
            if (cell->layer!=3 || x<cell->x || y<cell->y || x+16>cell->x+cell->size || y+16>cell->y+cell->size) continue;
            const scene_image_t *image=scene_map_image(cell->phases[0]);bool found=false;
            for (unsigned s=0;s<source_count;s++) if (sources[s].image->page==image->page) found=true;
            if (!found) {
                require(source_count<MAX_EDITS,"source bookkeeping");
                sources[source_count++]=(source_t){image,msx_cine_crc32(scene_page(image),SCENE_PAGE_BYTES)};
            }
        }
    }
}
static void check_sources(void) {
    for (unsigned i=0;i<source_count;i++) require(sources[i].crc==
        msx_cine_crc32(scene_page(sources[i].image),SCENE_PAGE_BYTES),"immutable cartridge page");
}
static unsigned apply(const edit_t *edits,unsigned count) {
    unsigned changed_count=0;
    for (unsigned i=0;i<count;i++) {
        uint16_t pixels[256];bool changed=false;
        require(campaign_footprint_pixels(edits[i].column,edits[i].row,pixels,&changed),"original footprint pixels");
        if (!changed) continue;
        for (unsigned p=0;p<256;p++) if (!(pixels[p]&1)) transparent++;
        require(scene_replace_foreground_tile(edits[i].column*16,112+edits[i].row*16,pixels),"foreground replacement");
        changed_count++;
    }
    return changed_count;
}
static void sample(unsigned mode,unsigned camera,unsigned phase,const edit_t *edits,unsigned count,int golden) {
    scene_clear_replacements();render(baseline,mode,camera,phase,NULL,0,false);
    remember_sources(edits,count);
    render(reference,mode,camera,phase,edits,count,true);
    if (!mode && golden>=0) {
        int left=footprint_expected[golden].x,top=footprint_expected[golden].y;
        for (unsigned p=0;p<256;p++) require(reference[(top+p/16)*320+left+p%16]==
            footprint_expected[golden].pixels[p],"independent cartridge crop");
        checked_pixels+=256;
    }
    unsigned changed=apply(edits,count);changed_edits+=changed;
    if (count>1) {require(changed>0,"multi-edit sample changes foreground");multi_samples++;}
    if (!changed) {
        /* Some source entries already equal the current foreground. Their
         * deliberate no-op is part of B5870 behavior and must survive COW. */
        compare(reference,baseline,"no-op reference retains background",mode,camera);no_op_samples++;
    }
    check_sources();
    render(actual,mode,camera,phase,NULL,0,false);
    compare(actual,reference,"full-frame equivalence",mode,camera);
    if (!changed) compare(actual,baseline,"no-op replacement retains background",mode,camera);
    /* Reapplying identical edits must retain the same page and rendering,
     * including neighboring quadrants in the same 32px block. */
    require(apply(edits,count)==changed,"idempotent edit classification");
    render(actual,mode,camera,phase,NULL,0,false);
    compare(actual,reference,"idempotent replacement",mode,camera);check_sources();
    scene_clear_replacements();render(actual,mode,camera,phase,NULL,0,false);
    compare(actual,baseline,"reset restores background",mode,camera);check_sources();
    samples++;
    debugf("MSX FOOTPRINT COW sample=%u mode=%u camera=%u phase=%u edits=%u changed=%u crc=%08lx\n",
        samples,mode,camera,phase,count,changed,(unsigned long)msx_cine_crc32((uint8_t*)reference,BYTES));
}
int main(void) {
    debug_init_emulog();require(dfs_init(DFS_DEFAULT_LOCATION)==DFS_ESUCCESS,"DFS");
    rdpq_init();scene_assets_init();scene_renderer_init();
    require(campaign_footprint_renderer_load(),"footprint assets");
    display_init(RESOLUTION_320x240,DEPTH_16_BPP,2,GAMMA_NONE,FILTERS_RESAMPLE);
    baseline=malloc(BYTES);reference=malloc(BYTES);actual=malloc(BYTES);
    require(baseline && reference && actual,"test buffers");
    for (unsigned mode=0;mode<2;mode++) {
        for (unsigned i=0;i<sizeof(footprint_expected)/sizeof(*footprint_expected);i++) {
            edit_t edit={footprint_expected[i].column,footprint_expected[i].row};
            sample(mode,footprint_expected[i].camera,0,&edit,1,(int)i);
        }
        /* Adjacent rows/columns exercise shared source pages and all four
         * quadrants, as well as transparent replacement over lower planes. */
        edit_t edits[MAX_EDITS];unsigned count=0;
        for (int column=0;column<72;column++) for (int row=0;row<5;row++)
            edits[count++]=(edit_t){column,row};
        const unsigned cameras[]={0,31,320,640,1024};
        for (unsigned i=0;i<sizeof(cameras)/sizeof(*cameras);i++)
            sample(mode,cameras[i],0,edits,count,-1);
    }
    require(changed_edits>0 && multi_samples>0,"actual changed edits exercised");
    require(no_op_samples>0,"unchanged source entries exercised");
    require(transparent>0,"transparent erasure exercised");
    require(campaign_footprint_renderer_max_upload()<=2048,"reference TMEM budget");
    debugf("MSX FOOTPRINT COW PASS samples=%u pixels=%u transparent=%u changed=%u noops=%u multi=%u max_upload=%u\n",
        samples,checked_pixels,transparent,changed_edits,no_op_samples,multi_samples,campaign_footprint_renderer_max_upload());
    for (;;) wait_ms(100);
}
