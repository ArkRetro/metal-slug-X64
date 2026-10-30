/* Fix-layer HUD on the N64 (docs/ARMAS_VEHICULOS.md §11): the map of
 * neo_hud.c drawn from the S-ROM tiles (rom:/fixed.bin, 8x8 4bpp) with the
 * fix palettes (rom:/hud-palette.bin, RGBA5551, pen 0 clear) above every
 * layer. Fix column c, row r is Neo Geo pixel (8c, 8r); raster rows 16..239
 * are the screen rows 8..231 of the 304x224 window (columns 1..38, rows
 * 2..29). Cheap: a cell is rasterised into an RGBA16 strip only when its map
 * word changes; each frame blits the column span of the non-blank cells of
 * every visible row (a handful of rows during play). */
#include <libdragon.h>
#include <string.h>
#include "fixed.h"
#include "hud_renderer.h"

enum { FIRST_ROW=2, ROWS=28, FIRST_COLUMN=1, COLUMNS=38 };
static uint8_t *fixed_rom;
static uint16_t palette[256];
static surface_t strip;                    /* 304 x 224 RGBA16 */
static uint16_t shown[NEO_FIX_WORDS];
static uint8_t opaque[NEO_FIX_WORDS];      /* cell has a non-zero pen */
static bool ready,valid;
static uint32_t shown_serial;

bool hud_renderer_init(void) {
    FILE *f=fopen("rom:/fixed.bin","rb");
    if (!f) {debugf("MSX HUD: rom:/fixed.bin missing, fix layer off\n");return false;}
    fixed_rom=malloc(131072);
    bool ok=fixed_rom && fread(fixed_rom,1,131072,f)==131072;fclose(f);
    FILE *p=fopen("rom:/hud-palette.bin","rb");
    if (!ok || !p || fread(palette,2,256,p)!=256) {if (p) fclose(p);debugf("MSX HUD: fix assets incomplete, fix layer off\n");return false;}
    fclose(p);
    for (unsigned i=0;i<256;i++) palette[i]=(uint16_t)(((uint8_t *)&palette[i])[0]<<8|((uint8_t *)&palette[i])[1]);
    strip=surface_alloc(FMT_RGBA16,COLUMNS*8,ROWS*8);
    ready=strip.buffer!=NULL;valid=false;
    debugf("MSX HUD: fix layer from S-ROM tiles (%s)\n",ready?"on":"no memory");
    return ready;
}
static void raster(unsigned column,unsigned row,uint16_t word) {
    uint16_t *base=(uint16_t *)strip.buffer+(row-FIRST_ROW)*8*(strip.stride/2)+(column-FIRST_COLUMN)*8;
    const uint16_t *pal=&palette[(word>>12)*16];unsigned tile=word&4095,any=0;
    for (unsigned y=0;y<8;y++) {
        uint16_t *line=base+y*(strip.stride/2);
        for (unsigned x=0;x<8;x++) {unsigned pen=fixed_pixel(fixed_rom,tile,x,y);line[x]=pal[pen];any|=pen;}
        data_cache_hit_writeback(line,16);
    }
    opaque[column*32+row]=any!=0;
}
void hud_renderer_draw(const neo_hud_t *hud) {
    if (!ready) return;
    if (!valid || hud->serial!=shown_serial) {
        for (unsigned c=FIRST_COLUMN;c<FIRST_COLUMN+COLUMNS;c++) for (unsigned r=FIRST_ROW;r<FIRST_ROW+ROWS;r++) {
            uint16_t w=hud->fix[c*32+r];
            if (!valid || w!=shown[c*32+r]) {shown[c*32+r]=w;raster(c,r,w);}
        }
        valid=true;shown_serial=hud->serial;
    }
    bool mode=false;
    for (unsigned r=FIRST_ROW;r<FIRST_ROW+ROWS;r++) {
        unsigned first=COLUMNS,last=0;
        for (unsigned c=FIRST_COLUMN;c<FIRST_COLUMN+COLUMNS;c++) if (opaque[c*32+r]) {if (first==COLUMNS) first=c;last=c;}
        if (first==COLUMNS) continue;
        if (!mode) {rdpq_mode_push();rdpq_set_mode_copy(true);mode=true;}
        unsigned x0=(first-FIRST_COLUMN)*8,w=(last-first+1)*8,y0=(r-FIRST_ROW)*8;
        rdpq_tex_blit(&strip,(float)(first*8),(float)(r*8-8),&(rdpq_blitparms_t){.s0=(int)x0,.t0=(int)y0,.width=(int)w,.height=8});
    }
    if (mode) rdpq_mode_pop();
}
