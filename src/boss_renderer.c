#include "boss_renderer.h"
#include <libdragon.h>
#include "gameplay.h"
#include "scene_assets.h"
#include "scene_renderer.h"
#include "boss_columns.h"

static boss_render_stats_t stats;
boss_render_stats_t boss_render_stats(void) {return stats;}
static const boss_column_run_t *columns(uint32_t tag) {
    unsigned lo=0,hi=sizeof(boss_column_runs)/sizeof(boss_column_runs[0]);
    while (lo<hi) {unsigned m=(lo+hi)/2;if (boss_column_runs[m].tag<tag) lo=m+1; else hi=m;}
    return lo<sizeof(boss_column_runs)/sizeof(boss_column_runs[0]) && boss_column_runs[lo].tag==tag?&boss_column_runs[lo]:NULL;
}
static bool empty_descriptor(uint32_t tag) {
    static const uint32_t empty[]=BOSS_EMPTY_DESCRIPTORS;
    for (unsigned i=0;i<sizeof(empty)/sizeof(empty[0]);i++) if (empty[i]==tag) return true;
    return false;
}
bool boss_render_draw(const neo_anim_draw_t *d,unsigned resource,int y_offset,bool flash,bool request_only) {
    if (empty_descriptor(d->descriptor)) return true;
    const scene_native_descriptor_t *desc=scene_native_descriptor(d->descriptor);
    const boss_column_run_t *run=columns(d->descriptor);
    if (!desc || !run) {stats.missing_descriptor++;return false;}
    int palette=scene_native_palette(resource);
    if (palette<0) {stats.missing_palette++;return false;}
    bool mirror=(d->attributes&1u)!=0;
    int origin_x=(int)(int16_t)d->x;
    int origin_y=MSX_SCREEN_BASE_Y-((int)(int16_t)d->y-MSX_PROJECTED_BASE_Y)+y_offset;
    /* Visible columns, as screen intervals [x0,x1). */
    int span0[64],span1[64];unsigned spans=0;
    for (unsigned c=0;c<run->count && spans<64;c++) {
        int cx=boss_column_starts[run->first+c];
        int x0=mirror?origin_x-cx-16-1:origin_x+cx;
        if (x0<-8 || x0>311) continue;
        unsigned k=spans++;
        while (k>0 && span0[k-1]>x0) {span0[k]=span0[k-1];span1[k]=span1[k-1];k--;}
        span0[k]=x0;span1[k]=x0+16;
    }
    for (unsigned i=0;i<desc->count;i++) {
        const scene_image_t *im=scene_frame(SCENE_NATIVE,desc->first+i);
        if (request_only) {scene_request_image(im,2);continue;}
        scene_image_t part=*im;part.palette=(uint16_t)palette;
        int x=mirror?origin_x-(im->x+im->width)-1:origin_x+im->x;
        int y=origin_y+im->y;
        if (x+im->width<=0 || x>=320 || y+im->height<=0 || y>=240) continue;
        /* Draw the part once per visible column interval it overlaps; adjacent
         * columns are merged so an unclipped part is a single draw. */
        int a=x,b=x+im->width;
        for (unsigned s=0;s<spans;) {
            int lo=span0[s],hi=span1[s];
            for (s++;s<spans && span0[s]<=hi;s++) if (span1[s]>hi) hi=span1[s];
            if (hi<=a || lo>=b) continue;
            int c0=lo>a?lo:a,c1=hi<b?hi:b;
            bool clip=c0>a || c1<b;
            if (clip) {
                /* Inside the 304x224 active window as every sprite. */
                int s0=c0<8?8:c0,s1=c1>312?312:c1;
                if (s0>=s1) continue;
                rdpq_set_scissor(s0,8,s1,232);stats.clipped++;
            }
            scene_render_image(&part,x,y,mirror,flash);stats.parts++;
            if (clip) rdpq_set_scissor(8,8,312,232);
        }
    }
    return true;
}
