#include "hud.h"
#include <libdragon.h>
#include <stdio.h>
#include "hud_assets.h"

static void text(int x,int y,const char *value) {
    for (;*value;value++,x+=8) {
        unsigned glyph=(unsigned char)*value;
        assertf(glyph>=32 && glyph<96,"Unsupported HUD glyph");
        glyph-=32;
        int s=(glyph&7u)*8,t=(glyph>>3)*8;
        rdpq_texture_rectangle(TILE0,x,y,x+8,y+8,s,t);
    }
}
static void begin(void) {
    rdpq_mode_push();
    rdpq_set_mode_standard();
    rdpq_mode_filter(FILTER_POINT);
    rdpq_mode_alphacompare(1);
    rdpq_mode_tlut(TLUT_RGBA16);
    rdpq_tex_upload_tlut((uint16_t*)msx_hud_palette,0,16);
    surface_t atlas=surface_make((void*)msx_hud_glyphs,FMT_CI4,64,64,32);
    rdpq_tex_upload(TILE0,&atlas,&(rdpq_texparms_t){.palette=0});
}
void msx_hud_text(int x,int y,const char *value) {
    assert(value);begin();text(x,y,value);rdpq_mode_pop();
}
void msx_hud_draw(const msx_hud_state_t *state) {
    assert(state);begin();
    char counters[40];
    snprintf(counters,sizeof(counters),"HP:%lu KILLS:%lu",
        (unsigned long)state->health,(unsigned long)state->kills);
    text(16,16,counters);
    text(16,24,"PISTOL");
    rdpq_mode_pop();
}
