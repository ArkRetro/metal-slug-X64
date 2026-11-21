/* Code overlays of the console cartridge (src/msx_overlay.h, tools/link_overlays.py). */
#include "msx_overlay.h"
#include <libdragon.h>
#include <string.h>
uint32_t rompak_search_ext(const char *ext);   /* libdragon rompak.c (rompak_internal.h) */
/* The images are linked at this address: global, tools/link_overlays.py reads it from the ELF. */
uint8_t msx_overlay_region[MSX_OVERLAY_BYTES] __attribute__((aligned(32)));
typedef struct { uint32_t magic,id; ga_result_t (*tick)(neo_gactor_t *a,neo_genv_t *e); uint32_t bytes; } image_head_t;
static uint32_t rom;   /* the `.ovl` file */
static struct { uint32_t offset,bytes; } table[MSX_OVL_COUNT];
static int loaded=-1;
static unsigned depth;
static unsigned loads;
static void open_file(void) {
    rom=rompak_search_ext(".ovl");
    assertf(rom && !(rom&7),"MSX overlays: no .ovl file in the cartridge");
    static uint32_t head[4+2*MSX_OVL_COUNT] __attribute__((aligned(16)));
    data_cache_hit_writeback_invalidate(head,sizeof(head));dma_read(head,rom,sizeof(head));
    assertf(head[0]==0x4d53584fu && head[1]==MSX_OVL_COUNT && head[2]==(uint32_t)(uintptr_t)msx_overlay_region && head[3]==MSX_OVERLAY_BYTES,
        "MSX overlays: the .ovl file is not this program's (%08lx %lu %08lx %lu)",(unsigned long)head[0],(unsigned long)head[1],(unsigned long)head[2],(unsigned long)head[3]);
    for (unsigned i=0;i<MSX_OVL_COUNT;i++) {
        table[i].offset=head[4+2*i];table[i].bytes=head[5+2*i];
        assertf(!(table[i].offset&15) && table[i].bytes<=MSX_OVERLAY_BYTES,"MSX overlays: bad entry %u",i);
    }
}
static void load(msx_overlay_id_t id) {
    assertf(id<MSX_OVL_COUNT,"MSX overlays: id %u",(unsigned)id);
    assertf(depth==0,"MSX overlays: overlay %u needed while overlay %d runs",(unsigned)id,loaded);
    if (!rom) open_file();
    uint32_t t0=TICKS_READ(),n=(table[id].bytes+7u)&~7u;
    loaded=-1;
    data_cache_hit_writeback_invalidate(msx_overlay_region,n);
    dma_read(msx_overlay_region,rom+table[id].offset,n);
    inst_cache_hit_invalidate(msx_overlay_region,n);
    const image_head_t *h=(const image_head_t *)msx_overlay_region;
    assertf(h->magic==0x4d53586fu && h->id==id && h->bytes==table[id].bytes,"MSX overlays: bad image %u",(unsigned)id);
    loaded=(int)id;loads++;
    debugf("MSX OVL %u loaded: %lu bytes, %lu us (load %u)\n",(unsigned)id,(unsigned long)h->bytes,(unsigned long)TICKS_TO_US(TICKS_SINCE(t0)),loads);
}
ga_result_t msx_overlay_tick(msx_overlay_id_t id,neo_gactor_t *a,neo_genv_t *e) {
    if (loaded!=(int)id) load(id);
    depth++;
    ga_result_t r=((const image_head_t *)msx_overlay_region)->tick(a,e);
    depth--;
    return r;
}
void msx_overlay_mission(unsigned mission) {
    if (mission>=3 && mission<=6 && loaded!=(int)(MSX_OVL_M3BOSS+mission-3u)) load((msx_overlay_id_t)(MSX_OVL_M3BOSS+mission-3u));
}
