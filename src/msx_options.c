/* Options screen: see msx_options.h. The table is the BIOS soft-DIP format
 * (16-byte game name; two time settings, FFFF when unused; two count
 * settings, FF when unused; ten list bytes, option count in the low nibble
 * and default in the high one, 00 ends the list; then the 12-character
 * strings: each used setting's name, each list's options after its name).
 * Count 1 is RAM 10FD88, count 2 10FD89, list n 10FD8A+n. The screen shows
 * the settings of 10FD88..10FD8F; the editing is the euro MVS BIOS's
 * (sp-s2.sp1 C1A324..C1A420): Up/Down move without wrapping, A adds one, B
 * takes one (the port's N64 B and A, as in play); a list wraps within its options, a count runs 1..99 and wraps
 * (C1A47C). Start leaves (the N64 has no BIOS menu to go back to). */
#include <libdragon.h>
#include <string.h>
#include "msx_options.h"
#include "neo_world.h"
#include "fixed.h"
#include "msx_audio.h"
#include "msx_perf.h"

enum { NAME_CHARS=12, PAGE=0x300, MAX_ROWS=NEO_DIPS };
static uint8_t table[0x400];
static unsigned table_size;
/* One row of the screen: its RAM byte, name, options (NULL for a count). */
typedef struct { uint8_t dip,count; const uint8_t *name,*options; } row_t;
static row_t rows[MAX_ROWS];
static unsigned row_count;

/* EEPROM: block 0 "MSXO", version, byte count, sum of the bytes, 0; block 1
 * the bytes of 10FD88..10FD8F. */
static const uint8_t magic[4]={'M','S','X','O'};
static bool valid(const uint8_t *dip) {
    for (unsigned i=0;i<row_count;i++) {
        const row_t *r=&rows[i];uint8_t v=dip[r->dip];
        if (r->options?v>=r->count:(v<1 || v>99)) return false;
    }
    return true;
}
static uint8_t sum(const uint8_t *dip) {uint8_t s=0;for (unsigned i=0;i<NEO_DIPS;i++) s=(uint8_t)(s+dip[i]);return s;}
static void save(void) {
    if (eeprom_present()==EEPROM_NONE) {debugf("MSX OPTIONS no EEPROM: not saved\n");return;}
    uint8_t b[16]={0};memcpy(b,magic,4);b[4]=1;b[5]=NEO_DIPS;b[6]=sum(neo_softdip);memcpy(b+8,neo_softdip,NEO_DIPS);
    eeprom_write_bytes(b,0,16);debugf("MSX OPTIONS saved\n");
}
static void load(void) {
    if (eeprom_present()==EEPROM_NONE) {debugf("MSX OPTIONS no EEPROM: defaults of the table\n");return;}
    uint8_t b[16];eeprom_read_bytes(b,0,16);
    if (memcmp(b,magic,4) || b[4]!=1 || b[5]!=NEO_DIPS || b[6]!=sum(b+8) || !valid(b+8)) {debugf("MSX OPTIONS EEPROM empty: defaults of the table\n");return;}
    for (unsigned i=0;i<row_count;i++) neo_softdip[rows[i].dip]=b[8+rows[i].dip];
    debugf("MSX OPTIONS loaded from EEPROM\n");
}
static void log_dips(const char *what) {
    debugf("MSX OPTIONS %s 10FD88=%02X 10FD8A=%02X 10FD8B=%02X 10FD8C=%02X 10FD8D=%02X 10FD8E=%02X 10FD8F=%02X\n",what,
        neo_softdip[0],neo_softdip[2],neo_softdip[3],neo_softdip[4],neo_softdip[5],neo_softdip[6],neo_softdip[7]);
}
bool msx_options_init(void) {
    FILE *f=fopen("rom:/softdip.bin","rb");
    if (!f) {debugf("MSX OPTIONS rom:/softdip.bin missing: no options screen\n");return false;}
    table_size=fread(table,1,sizeof(table),f);fclose(f);
    if (table_size<32) {table_size=0;return false;}
    /* The strings in the BIOS order: time names, count names, then each list. */
    const uint8_t *s=table+32,*end=table+table_size;
    for (unsigned t=0;t<2;t++) if (table[16+2*t]!=0xff || table[17+2*t]!=0xff) s+=NAME_CHARS;
    row_count=0;
    for (unsigned c=0;c<2;c++) {
        if (table[20+c]==0xff) continue;
        if (s+NAME_CHARS>end) return false;
        rows[row_count++]=(row_t){(uint8_t)c,0,s,NULL};
        neo_softdip[c]=table[20+c];s+=NAME_CHARS;
    }
    for (unsigned l=0;l<10 && table[22+l];l++) {
        uint8_t b=table[22+l],n=b&15,def=b>>4;
        if (s+NAME_CHARS*(1u+n)>end || !n) return false;
        if (2+l<NEO_DIPS) {rows[row_count++]=(row_t){(uint8_t)(2+l),n,s,s+NAME_CHARS};neo_softdip[2+l]=def<n?def:0;}
        s+=NAME_CHARS*(1u+n);
    }
    log_dips("defaults");
    load();
    log_dips("start");
    return row_count>0;
}

/* The fix font: page 3 of the S-ROM, palette 0 of the fix palettes. */
static uint8_t *font;
static uint16_t palette[16];
static bool font_load(void) {
    FILE *f=fopen("rom:/fixed.bin","rb"),*p=fopen("rom:/hud-palette.bin","rb");
    bool ok=f && p && (font=malloc(256*32))!=NULL && fseek(f,PAGE*32,SEEK_SET)==0 && fread(font,32,256,f)==256 && fread(palette,2,16,p)==16;
    if (f) fclose(f);
    if (p) fclose(p);
    if (!ok) {free(font);font=NULL;return false;}
    for (unsigned i=0;i<16;i++) palette[i]=(uint16_t)(((uint8_t *)&palette[i])[0]<<8|((uint8_t *)&palette[i])[1]);
    return true;
}
/* Fix cell (c, r) at screen (8c, 8r - 8), as the HUD (hud_renderer.c). */
static void glyph(surface_t *s,unsigned c,unsigned r,uint8_t ch) {
    for (unsigned y=0;y<8;y++) {
        uint16_t *line=(uint16_t *)((uint8_t *)s->buffer+(r*8-8+y)*s->stride)+c*8;
        for (unsigned x=0;x<8;x++) {unsigned pen=fixed_pixel(font,ch,x,y);if (pen) line[x]=palette[pen];}
    }
}
static void text(surface_t *s,unsigned c,unsigned r,const uint8_t *t,unsigned n) {
    for (unsigned i=0;i<n;i++) glyph(s,c+i,r,t[i]);
}
enum { CURSOR=0x1f };   /* page 3 glyph 1F: the right-pointing triangle */
static void draw(surface_t *s,unsigned cursor) {
    graphics_fill_screen(s,graphics_make_color(0,0,0,255));
    text(s,12,5,table,16);                                       /* the game's name */
    for (unsigned i=0;i<row_count;i++) {
        const row_t *r=&rows[i];unsigned y=9+2*i;
        if (i==cursor) glyph(s,4,y,CURSOR);
        text(s,6,y,r->name,NAME_CHARS);
        uint8_t v=neo_softdip[r->dip];
        if (r->options) text(s,22,y,r->options+NAME_CHARS*v,NAME_CHARS);
        else {uint8_t d[2]={(uint8_t)(v>=10?'0'+v/10:' '),(uint8_t)('0'+v%10)};text(s,22,y,d,2);}
    }
}
void msx_options_menu(void (*idle)(void)) {
    if (!row_count || !font_load()) return;
    unsigned cursor=0;bool first=true;joypad_buttons_t was={0};
    uint8_t opened[NEO_DIPS];memcpy(opened,neo_softdip,NEO_DIPS);
    debugf("MSX OPTIONS open\n");msx_frames_stage("options");
    for (;;) {
        joypad_poll();
        joypad_buttons_t b=joypad_get_buttons(JOYPAD_PORT_1);
        if (first) {was=b;first=false;}   // a button held from the title is not a choice
        /* The port's pad (actor.c): N64 B is the Neo Geo A, N64 A the Neo Geo B. */
        bool up=b.d_up && !was.d_up,down=b.d_down && !was.d_down,plus=b.b && !was.b,minus=b.a && !was.a,start=b.start && !was.start;
        was=b;
        if (start) break;
        if (up && cursor>0) cursor--;
        if (down && cursor+1<row_count) cursor++;
        if (plus || minus) {
            const row_t *r=&rows[cursor];uint8_t *v=&neo_softdip[r->dip];
            if (r->options) *v=(uint8_t)(plus?(*v+1u<r->count?*v+1u:0u):(*v?*v-1u:r->count-1u));
            else do *v=(uint8_t)(*v+(plus?1:-1)); while (!*v || *v>=100);   /* C1A48A: 1..99 */
            debugf("MSX OPTIONS row=%u value=%02X\n",cursor,*v);
        }
        surface_t *surface=display_get();
        msx_audio_service();
        draw(surface,cursor);
        display_show(surface);msx_frames_present();
        if (idle) idle();
    }
    free(font);font=NULL;
    log_dips("close");
    if (memcmp(opened,neo_softdip,NEO_DIPS)) save();   /* written only when changed */
}
