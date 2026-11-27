/* Code overlays of the console cartridge (src/msx_overlay.h, tools/link_overlays.py). */
#include "msx_overlay.h"
#include <libdragon.h>
#include <string.h>
uint32_t rompak_search_ext(const char *ext);   /* libdragon rompak.c (rompak_internal.h) */
/* The images are linked at these addresses: global, tools/link_overlays.py reads them from the ELF. */
uint8_t msx_overlay_region[MSX_OVERLAY_BYTES] __attribute__((aligned(32)));
uint8_t msx_overlay_code_region[MSX_OVERLAY_CODE_BYTES] __attribute__((aligned(32)));
enum { TICKS_MAX=MSX_MOD_COUNT, REGIONS=2 };   /* tools/link_overlays.py */
typedef ga_result_t (*tick_fn)(neo_gactor_t *a,neo_genv_t *e);
typedef struct { uint32_t magic,id,bytes,count; tick_fn tick[TICKS_MAX]; } image_head_t;
static uint32_t rom;   /* the `.ovl` file */
static struct { uint32_t offset,bytes; } table[MSX_OVL_COUNT];
static struct { uint8_t *base; uint32_t bytes; int loaded; unsigned depth; } region[REGIONS]={
    {msx_overlay_region,MSX_OVERLAY_BYTES,-1,0},{msx_overlay_code_region,MSX_OVERLAY_CODE_BYTES,-1,0}};
static unsigned loads;
static unsigned region_of(msx_overlay_id_t id) {return id>=MSX_OVL_M2CODE;}
static void open_file(void) {
    rom=rompak_search_ext(".ovl");
    assertf(rom && !(rom&7),"MSX overlays: no .ovl file in the cartridge");
    static uint32_t head[2+2*REGIONS+2*MSX_OVL_COUNT] __attribute__((aligned(16)));
    data_cache_hit_writeback_invalidate(head,sizeof(head));dma_read(head,rom,sizeof(head));
    bool ok=head[0]==0x4d53584fu && head[1]==MSX_OVL_COUNT;
    for (unsigned r=0;r<REGIONS;r++) ok=ok && head[2+2*r]==(uint32_t)(uintptr_t)region[r].base && head[3+2*r]==region[r].bytes;
    assertf(ok,"MSX overlays: the .ovl file is not this program's (%08lx %lu %08lx %lu)",(unsigned long)head[0],(unsigned long)head[1],(unsigned long)head[2],(unsigned long)head[3]);
    for (unsigned i=0;i<MSX_OVL_COUNT;i++) {
        table[i].offset=head[2+2*REGIONS+2*i];table[i].bytes=head[3+2*REGIONS+2*i];
        assertf(!(table[i].offset&15) && table[i].bytes<=region[region_of(i)].bytes,"MSX overlays: bad entry %u",i);
    }
}
static void load(msx_overlay_id_t id) {
    assertf(id<MSX_OVL_COUNT,"MSX overlays: id %u",(unsigned)id);
    unsigned r=region_of(id);
    assertf(region[r].depth==0,"MSX overlays: overlay %u needed while overlay %d runs",(unsigned)id,region[r].loaded);
    if (!rom) open_file();
    uint8_t *base=region[r].base;
    uint32_t t0=TICKS_READ(),n=(table[id].bytes+7u)&~7u;
    region[r].loaded=-1;
    data_cache_hit_writeback_invalidate(base,n);
    dma_read(base,rom+table[id].offset,n);
    inst_cache_hit_invalidate(base,n);
    const image_head_t *h=(const image_head_t *)base;
    assertf(h->magic==0x4d53586fu && h->id==id && h->bytes==table[id].bytes && h->count==TICKS_MAX,"MSX overlays: bad image %u",(unsigned)id);
    region[r].loaded=(int)id;loads++;
    debugf("MSX OVL %u loaded: %lu bytes, %lu us (load %u)\n",(unsigned)id,(unsigned long)h->bytes,(unsigned long)TICKS_TO_US(TICKS_SINCE(t0)),loads);
}
/* Each module's own mission's image. */
static const uint8_t home[MSX_MOD_COUNT]={MSX_OVL_M2CODE,MSX_OVL_M2CODE,MSX_OVL_M2CODE,MSX_OVL_M3CODE,MSX_OVL_M3CODE,
    MSX_OVL_M3CODE,MSX_OVL_M4CODE,MSX_OVL_M4CODE,MSX_OVL_M5CODE,MSX_OVL_M5CODE,MSX_OVL_M5CODE,MSX_OVL_M5CODE,MSX_OVL_M5CODE,
    MSX_OVL_M6CODE,MSX_OVL_M6CODE};
static ga_result_t run(unsigned r,unsigned slot,neo_gactor_t *a,neo_genv_t *e) {
    const image_head_t *h=(const image_head_t *)region[r].base;
    assertf(slot<h->count && h->tick[slot],"MSX overlays: slot %u not in overlay %d",slot,region[r].loaded);
    region[r].depth++;
    ga_result_t res=h->tick[slot](a,e);
    region[r].depth--;
    return res;
}
ga_result_t msx_overlay_code_tick(msx_overlay_module_t mod,neo_gactor_t *a,neo_genv_t *e) {
    assertf(mod<MSX_MOD_COUNT,"MSX overlays: module %u",(unsigned)mod);
    const image_head_t *h=(const image_head_t *)region[1].base;
    if (region[1].loaded<0 || !h->tick[mod]) {
        load((msx_overlay_id_t)home[mod]);
        debugf("MSX OVL %u for module %u, routine %06lX\n",(unsigned)home[mod],(unsigned)mod,(unsigned long)ga_l(a,0));
    }
    return run(1,mod,a,e);
}
ga_result_t msx_overlay_tick(msx_overlay_id_t id,neo_gactor_t *a,neo_genv_t *e) {
    if (region[0].loaded!=(int)id) load(id);
    return run(0,0,a,e);
}
void msx_overlay_mission(unsigned mission) {
    if (mission>=3 && mission<=6 && region[0].loaded!=(int)(MSX_OVL_M3BOSS+mission-3u)) load((msx_overlay_id_t)(MSX_OVL_M3BOSS+mission-3u));
    if (mission>=2 && mission<=6 && region[1].loaded!=(int)(MSX_OVL_M2CODE+mission-2u)) load((msx_overlay_id_t)(MSX_OVL_M2CODE+mission-2u));
}
