/* The original's pause: see neo_pause.h. */
#include "neo_pause.h"
#include <stddef.h>
#include "neo_text.h"
/* 01542A and 01543B: \l column 0x12 row 0x1A (VRAM 725A), palette 0, page 3. */
static const uint8_t text_pause[]="\\l\x12\x1a\\p\x00\\c\x03PAUSE";
static const uint8_t text_blank[]="\\l\x12\x1a\\c\x03     ";
/* 03D244 (plain glyphs 03CF9A) and 03D25A with 03DD96: the same words for
 * page 3 (03DD96 sends pages below 0x10 to 03CF9A). D1 on entry is what the
 * caller left; its palette bits only matter for the blank string, whose
 * words MAME shows as 0320 (palette 0) in both calls. */
static void print(neo_hud_t *hud,const uint8_t *s) {
    neo_text_t t={hud,NULL,NULL,0};uint16_t d1=0;
    neo_text_string(&t,0x725a,&d1,s);
}
void neo_pause_init(neo_pause_t *p) {
    p->owner=0;p->last=1;
}
/* 015448 (home: 10FD82 = 0): the new presses 10C367, without the SELECT of
 * a player whose BIOS mode is 0 (never played) or 3 (game over). */
static uint8_t selects(const neo_pause_input_t *in) {
    uint8_t d0=in->select;
    if (in->mode[0]==0 || in->mode[0]==3) d0&=(uint8_t)~2u;
    if (in->mode[1]==0 || in->mode[1]==3) d0&=(uint8_t)~8u;
    return d0;
}
uint8_t neo_pause_step(neo_pause_t *p,const neo_pause_input_t *in,uint8_t pressed[2],neo_hud_t *hud) {
    uint8_t d0;
    if (!p->owner) {
        if (in->off) return 1;                      /* 01554A */
        d0=selects(in);
        if (!d0) return 1;
        if (d0&2u) p->owner|=1u;                    /* 015564 */
        else if (d0&8u) p->owner|=2u;               /* 015576 */
        else return 1;
        /* 015584: 10C2C4 = 0, 03CAAC, the blink and the held buttons. */
        p->blink=0;p->held[0]=in->held[0];p->held[1]=in->held[1];
        d0=0;                                       /* 015610 */
    } else {
        d0=selects(in);                             /* 015620 */
        if (((p->owner&1u) && (d0&2u)) || ((p->owner&2u) && (d0&8u))) d0=1;   /* 0156E4 -> 0156F8 */
        else {
            if (!(p->blink&0x18u)) {                /* 015680 */
                uint8_t d7=(uint8_t)((p->blink+1u)&7u);
                if (d7<=1u) print(hud,d7?text_pause:text_blank);   /* 03D244 */
            }
            d0=0;
        }
    }
    /* 0156FA: a button held now and not at the pause is a new press. */
    for (unsigned i=0;i<2;i++) pressed[i]|=(uint8_t)((in->held[i]^p->held[i])&in->held[i]);
    p->blink--;
    if (d0==1) {
        print(hud,text_blank);                      /* 01572C: 03D25A */
        p->owner=0;                                 /* 10C2BC = 0 too */
        hud->panel[0].f88=0;hud->panel[1].f88=0;    /* 0C8E5C: 10128A, 10133A */
    }
    return d0;
}
bool neo_pause_turn(neo_pause_t *p,const neo_pause_input_t *in,uint8_t pressed[2],neo_hud_t *hud,neo_fade_t *fade,neo_lspc_t *lspc) {
    uint8_t d0=neo_pause_step(p,in,pressed,hud),d1=p->last;
    p->last=d0;
    if (!d0) {
        if (d1) {                                   /* 003AA2: 002836, 002802 with 0 */
            p->lspc=lspc->disabled?0u:(uint16_t)(lspc->speed+1u);
            neo_lspc_set(lspc,0);
        }
        if (d1==1) {                                /* 003AC0: 009BBC, 009C0A */
            p->fade=*fade;p->faded=0xff;
            neo_fade_start(fade,(int16_t)-8,(int16_t)-8,(int16_t)-8,4);
        }
        neo_fade_step(fade);                        /* 003ADA: 00698C */
        return false;
    }
    if (!d1) {                                      /* 003AF8 */
        neo_lspc_set(lspc,p->lspc);
        if (p->faded) {                             /* 009BD4 */
            uint8_t high[3];
            for (unsigned i=0;i<3;i++) high[i]=(uint8_t)((uint16_t)fade->current[i]>>8);
            *fade=p->fade;
            if (!fade->count) {
                fade->count=1;
                for (unsigned i=0;i<3;i++) fade->current[i]=(int16_t)(uint16_t)(((uint16_t)fade->current[i]&0xffu)|(uint16_t)(high[i]<<8));
            }
            p->faded=0;
        }
        p->resumed=0xff;
    } else p->resumed=0;
    return true;
}
