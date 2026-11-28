/* The game's ending (docs/MISION6.md "El final"), manual translation of ROM
 * 056FCC..0585FA (labels are provenance only): the results and ranking scene
 * 0570A0 that the mission actor creates after Mission 6 (0189D8: 0040BC,
 * A6 = 101364), in the end list of neo_end.c. RAM outside the nodes goes
 * through neo_end_ram_t rd/wr (byte addresses of the original); the text
 * engine is neo_text.c.
 *
 * Root 0570A0: 056FCC (palette slots 009E6C/03D4AC/009D8C, layers 01AA28 with
 * 056EF4: the environment's, ram->setup) and the fade 009C0A(0,0,0,1); music
 * 0x2E; the dial 0577E2; the ranks +7C (player 2) and +7A (player 1) of
 * 057006; 30 updates, voice 0x1151 and the title 0576B6 (it sets +60); 30,
 * the frame 05764A (+60); 15, one result 05731C per player (+74 = the player:
 * the second one gets 0 unless 10C46A < 10C472); when both have added 1 to +60: fade
 * (-31,-31,-31,16), the audio command 01228C(0x80), 20 updates and the
 * mission actor's +60.
 *
 * Dial 0577E2: four 05785A/057862 (programs 24BA2A/24BA34, palette 0x35F) at
 * angles 0/4000/8000/C000 (two flipped, +15 bit 0); its +76 turns by -0x100 a
 * update. 0578A8: with the angle a = (creator +76 + own +76) >> 8 (none when
 * negative) +12 = (255 sin a) >> 8 and x = (0x88 cos a) >> 8 + 0xA0.
 *
 * Result 05731C (+74 player): absent (RAM 10FDB6[p] = 0) it only counts; else
 * the rank +78 (its creator's +7A/+7C), "1P/2P RESULT", CONTINUE, the
 * continues 10E9E4[p] and the prisoners 10E9E8[p] (03CFBA: 30 updates of
 * random numbers below 1000 first, 0029F2), RECAPTURED PRISONER, SCORE and
 * the score (8 BCD digits), then the entry 05752E: rank 10 or worse waits 60
 * updates; better, the name entry 05812A (+76 the rank, +70 the VRAM column,
 * +72 the name 10C46E/10C476) and the table insert 057C82 (10F010: 10 rows
 * of score and name).
 *
 * Name entry 05812A: 10C468/10C469 = FF while it runs (057B44 counts the
 * other player's score above), the name "A  ", 03CAB4(0x2004), 600 updates
 * at most and 300 without a press; A (new press bit 4) takes the letter
 * (the end mark 7F ends), B (bit 5) goes back, up/down (held bits 3/2) step
 * the letter of the charset 0585D0; the rank and the name are drawn every
 * update with the cursor blinking (+7C); at the end an untouched name is
 * "SNK" and the nine names of 0585FC become "SNK", 7F become blanks, 60
 * updates of the name blinking, the creator's +60 and 10C468/9 = 0.
 *
 * The credits (section 22 of the script, stage 0x2C; the nodes below are
 * created by the sub-scripts 019E72 of the two actors 0A8C62):
 * - Page 0646FE + 0x0A k (k = 0..47, +84): times +74 (the script's +6E in
 *   updates, 60 by default, FFFF none), +78 and +7C (+70 / +72, 30 by
 *   default) through 0A4996 (50 Hz: times 0.D600); +8C = 20.0000; six lines
 *   064AC0 at its place with the descriptors and consecutive palettes of the
 *   record 0E1446 + 26 k. 0649E8: +8C falls to 0 in +78 updates (04E962 /
 *   04E5D0 with the step 0A49B0: 1, or 4 while RAM 10C604), waits +74, rises
 *   to 20.0000 in +7C and deletes itself with its lines (0041AC). A line
 *   sets its palette slot's level to the page's +8C word (009CCE) and draws
 *   its descriptor (011FF6).
 * - 0A4408: RAM 10C604 = FF while a player in play holds A, B or C (0190BC
 *   & 70); after +6E updates the music 0x31, once. 0A4478: deleted after +6E
 *   updates (a script's wait). 0A453A: the audio command +6E (01228C).
 * - Picture 0A454C (0A4744): program 2A4B16 with the picture +A8 = +7A - 1
 *   (two slots of palette 0x845, 0x125 from picture 0x3E), speed +76 << 8
 *   (50 Hz: plus 0.2A00 of it) along the angle +78 (002E7C; +6E = 2: down,
 *   until +2C reaches +78), kept in +94 / +98. +6E != 1 (0A45EC): after +84
 *   (the script's +70) updates the slots' level falls from 20 by +8C (+74 <<
 *   8) to 0, then after +88 (+72) rises again and the picture is deleted
 *   (0041AC; also outside the rectangle 0E61B4). +6E = 1 (0A4698): a
 *   swaying picture (x by sine(+A4) * +72, the size +12 by sine(+9C)): in no
 *   capture (the TAS's credits have none), translated from the listing.
 *
 * The title 0A7FFE (after the ranking 057E00): 002976 (the random ring) and
 * the layers 0A7D4A (01AA28: the environment's, ram->setup); every +6E
 * updates (table 0A7FD6) a piece 0A81A4 with +70 = 0..9 and its flight time
 * +72; the last piece sets +60: fade to white (1F,1F,1F at once), then
 * 0A8096 deletes every other node (004192), 0A863C (layers 0A7D52, texts
 * 03E2A8 / 0C81A6 / 03CF60: ram->setup), the picture 0A8602 (descriptor
 * 1D5B22, palette 0x840), its own program 2A7368 (palette 0xDE) at (120,130),
 * fade in 16, 0x96 updates and its creator's +60.
 * Piece 0A81A4: two palette slots (0x134, 0x145), the second blended towards
 * 0x159 from 1F down by its progress (009CCE); it starts at its place of
 * 0A7FAE mirrored around (80,170) and spirals in (0A8278), drawn through the
 * zoom +12 / +13 and the scale D7 of 0A824E around (A0,110), depth +1E by
 * its place. Arrived (+74 = +72): sound 0x10E1 and 24 sparks 0A83D6 (the
 * last piece: 0x10E5 and the root's +60), program 2A7082 at its place.
 * Spark 0A83D6: one of three programs (0A83CA), speed +26, flips by 0029C6
 * (bit 0 by the N flag it returns with: ram->rand_negative), two trails
 * around its creator's place; it is deleted when its program ends.
 *
 * Not reproduced: the cartridge checks 0571FC/0575BC/0576EE/0A4496/0A8116 (they only act
 * on a failed check). Explicit stops: 057B44 with a player byte +7 above 3
 * (TRAP #F), the formats '%' of 03D2B0. */
#include "neo_end.h"
#include <string.h>
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define S16(v) ((int16_t)(uint16_t)(v))
#define STOP(at) do { ga_fail(e,(at));return GA_UNSUPPORTED; } while (0)
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
#define STEP() do { if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED; } while (0)
#define DEC72() (SW(0x72,W(0x72)-1u),W(0x72)!=0)   /* subq.w #1,+72; beq */
#define NEO_ENDING_CREDITS 1
bool neo_ending_routine(uint32_t r) {
    return (r>=0x56fcc && r<=0x585fa) || neo_ending_world_routine(r) || (r>=0xa7ffe && r<=0xa8622) || (r>=0x1959c && r<=0x195d0);
}
/* The credits' nodes, created by the sub-scripts of the stage 0x2C in the world's list. */
bool neo_ending_world_routine(uint32_t r) {return (r>=0x646fe && r<=0x64ae0) || (r>=0xa4408 && r<=0xa4724);}
static uint16_t rw(uint32_t address) {const uint8_t *p=ga_rom(address);return p?(uint16_t)(p[0]<<8|p[1]):0;}
static uint32_t rl(uint32_t address) {return (uint32_t)rw(address)<<16|rw(address+2);}
static uint8_t rb(uint32_t address) {const uint8_t *p=ga_rom(address);return p?p[0]:0;}
/* RAM of the original. */
static uint8_t mb(neo_end_ram_t *m,uint32_t ad) {return m->rd?m->rd(m->ram_ctx,ad):0;}
static uint16_t mw(neo_end_ram_t *m,uint32_t ad) {return (uint16_t)(mb(m,ad)<<8|mb(m,ad+1));}
static uint32_t ml(neo_end_ram_t *m,uint32_t ad) {return (uint32_t)mw(m,ad)<<16|mw(m,ad+2);}
static void sbm(neo_end_ram_t *m,uint32_t ad,uint8_t v) {if (m->wr) m->wr(m->ram_ctx,ad,v);}
static void swm(neo_end_ram_t *m,uint32_t ad,uint16_t v) {sbm(m,ad,(uint8_t)(v>>8));sbm(m,ad+1,(uint8_t)v);}
static void slm(neo_end_ram_t *m,uint32_t ad,uint32_t v) {swm(m,ad,(uint16_t)(v>>16));swm(m,ad+2,(uint16_t)v);}
static void sound(neo_end_ram_t *m,uint16_t id) {if (m->sound_count<NEO_END_SOUNDS) m->sounds[m->sound_count++]=id; else m->sounds_dropped++;}
static neo_text_t text_of(neo_end_ram_t *m) {return (neo_text_t){m->hud,m->on_write,m->write_ctx,0};}
static bool text_done(neo_genv_t *e,neo_end_ram_t *m,neo_text_t *t,uint32_t at) {
    m->unsupported_text+=t->unsupported;
    if (t->unsupported) {ga_fail(e,at);return false;}
    return true;
}
/* 0040BC from a node. */
static neo_gactor_t *spawn(neo_gactor_t *a,neo_genv_t *e,uint32_t routine) {return e->spawn(e->ctx,a,routine,false);}
/* st.b / addq.b on the creator's +60 (the mission actor: the end list's flag). */
static void creator60(neo_gactor_t *a,neo_genv_t *e,neo_end_ram_t *m,bool add) {
    neo_gactor_t *c=e->creator(e->ctx,a);
    if (c) ga_sb(c,0x60,add?(uint8_t)(ga_b(c,0x60)+1u):0xffu); else m->mission60=1;
}
/* ---- text: 03D292 / 03D2B0 / 03D3B2 with the state block a3 in RAM --------
 * +0 d0, +2 d3, +4 d1, +6 a0, +A a1, +E a2, +12 the a0 of '%s', +16 count. */
static void type_start(neo_end_ram_t *m,uint32_t a3,uint16_t d0,uint16_t d1,uint32_t a0) {
    slm(m,a3+6,a0);slm(m,a3+0xa,0x3dd96);   /* +E: the caller's dead A2, not reproduced (test: from the record) */
    swm(m,a3,d0);swm(m,a3+2,d0);swm(m,a3+4,d1);swm(m,a3+0x16,0);
}
/* 03D2B0: one glyph; 1 at the end of the string, -1 on a stop. */
static int type_step(neo_end_ram_t *m,neo_text_t *t,uint32_t a3) {
    uint32_t a0=ml(m,a3+6);uint16_t d0=mw(m,a3),d1=mw(m,a3+4),d3=mw(m,a3+2);
    const uint8_t *base=ga_rom(a0),*p=base;
    if (!base) {t->unsupported++;return -1;}
    uint8_t d2=neo_text_next(t,&p,&d0,&d1,&d3);
    if (!d2) return 1;
    swm(m,a3+2,d3);swm(m,a3+4,d1);swm(m,a3,d0);
    if (d2=='%') {t->unsupported++;return -1;}
    d0=neo_text_glyph(t,mw(m,a3),d1,d2,&p);
    swm(m,a3,d0);slm(m,a3+6,a0+(uint32_t)(p-base));
    return 0;
}
/* 03D3B2: the whole string with the state at 10C384. */
static void print(neo_end_ram_t *m,neo_text_t *t,uint16_t d0,uint32_t a0) {
    type_start(m,0x10c384,d0,0,a0);   /* d1: the caller's dead D1 (03D2B0 stores it again after the escapes) */
    for (unsigned guard=0;guard<256;guard++) {int r=type_step(m,t,0x10c384);if (r) return;}
    t->unsupported++;
}
/* 05777E: d0 (binary, up to 4 digits) to BCD. */
static uint16_t bcd(uint16_t d0) {
    uint16_t d3=0;uint32_t v=d0;
    if (!v) return 0;
    d3=(uint16_t)(v%10u&15u);v/=10u;if (!v) return d3;
    d3|=(uint16_t)((v%10u&15u)<<4);v/=10u;if (!v) return d3;
    d3|=(uint16_t)((v%10u&15u)<<8);v/=10u;if (!v) return d3;
    d3|=(uint16_t)((v%10u&15u)<<12);
    return d3;
}
/* 057B44 (adjust: the other player's entry, 4468/4469) and 057C18: the rank
 * of the score at a0 in the table 10F010 (d0, d1; FFFF when off the table). */
static bool rank_of(neo_genv_t *e,neo_end_ram_t *m,uint32_t a0,bool adjust,uint16_t *d0,uint16_t *d1) {
    if ((int8_t)mb(m,a0+7)>3) {ga_fail(e,adjust?0x57b4e:0x57c22);return false;}   /* TRAP #F */
    uint32_t a1=0x10f010,d3=ml(m,a0);*d0=1;*d1=1;
    for (;;) {
        uint32_t d4=ml(m,a1);
        if (d3<d4) *d0=(uint16_t)(*d1+1u);
        if (d3>d4) break;
        if (d3==d4 && *d1==10) {*d0=*d1=0xffff;return true;}
        a1+=8;*d1=(uint16_t)(*d1+1u);
        if (S16(*d1)>10) {*d0=*d1=0xffff;return true;}
    }
    *d1=(uint16_t)(*d1-1u);
    if (!adjust) return true;
    if ((int8_t)mb(m,a0+7)>3) {ga_fail(e,0x57bb6);return false;}
    if (a0==0x10c46a && mb(m,0x10c469) && d3<ml(m,0x10c472)) {*d0=(uint16_t)(*d0+1u);*d1=(uint16_t)(*d1+1u);}
    if (a0==0x10c472 && mb(m,0x10c468) && d3<ml(m,0x10c46a)) {*d0=(uint16_t)(*d0+1u);*d1=(uint16_t)(*d1+1u);}
    return true;
}
/* 057006: the rank of player +74 (0x3E7: off the table or not played, RAM 10FDB6[p] = 0). */
static bool rank(neo_gactor_t *a,neo_genv_t *e,neo_end_ram_t *m,uint16_t *out) {
    uint16_t d0=0xffff,d1=0xffff,p=W(0x74);
    if (!mb(m,0x10fdb6u+p)) {*out=0x3e7;return true;}   /* 057092 with d1 = FFFF */
    uint32_t a0=p?0x10c472:0x10c46a,a1=p?0x10c46a:0x10c472;
    if (!rank_of(e,m,a0,true,&d0,&d1)) return false;
    d0=(uint16_t)(d0-1u);
    if (d1<10 && mb(m,0x10fdb6u+(p?0u:1u))) {
        uint32_t o=ml(m,a1),s=ml(m,a0);
        if (o>=s) {
            if (o!=s) {d0=(uint16_t)(d0+1u);d1=(uint16_t)(d1+1u);}
            else if (p) d1=(uint16_t)(d1+1u);
        }
    }
    if (d1>=10) d0=0x3e7;
    *out=d0;return true;
}
/* 057A98: d0 bytes from a0 to a1 (ROM or RAM source). */
static void copy_rom(neo_end_ram_t *m,uint32_t a1,uint32_t a0,unsigned n) {for (unsigned i=0;i<n;i++) sbm(m,a1+i,rb(a0+i));}
static void copy_ram(neo_end_ram_t *m,uint32_t a1,uint32_t a0,unsigned n) {for (unsigned i=0;i<n;i++) sbm(m,a1+i,mb(m,a0+i));}
/* 057C82: the score and name at a0 into the table 10F010. */
static bool insert(neo_genv_t *e,neo_end_ram_t *m,uint32_t a0) {
    if ((int8_t)mb(m,a0+7)>3) {ga_fail(e,0x57c8c);return false;}
    uint16_t d0,d1;
    if (!rank_of(e,m,a0,false,&d0,&d1)) return false;
    if (d0>10) return true;
    for (;;) {
        uint16_t k=d1;   /* 057D06 */
        if (S16(k)<=10) {
            k=(uint16_t)(k-1u);
            if (S16(k)>=0) copy_ram(m,0x10f010u+8u*k,0x10f010u+8u*k-8u,8);
        }
        if (d1==d0) break;
        d1=(uint16_t)(d1-1u);
    }
    copy_ram(m,0x10f010u+8u*(uint16_t)(d0-1u),a0,8);
    return true;
}
/* 03CF12: d2 columns x d3 rows of d1 from d0, then the modulo 0 and VRAM 8200 = 0. */
static uint16_t fill(neo_text_t *t,uint16_t d0,uint16_t d1,uint16_t d2,uint16_t d3) {
    for (;d2;d2--) {
        uint32_t d7=(uint32_t)d0<<16|d1;
        for (uint16_t d6=d3;d6;d6--) {neo_text_word(t,(uint16_t)(d7>>16),(uint16_t)d7);d7+=0x10000u;}
        d0=(uint16_t)(d0+0x20u);
    }
    if (t->on_write) t->on_write(t->ctx,0x8200,0);   /* SCB2 of sprite 0: the port's renderer keeps its own */
    return d0;
}
/* ---- name entry 05812A ------------------------------------------------------- */
#define CHARSET 0x585d0u
static uint32_t name_at(const neo_gactor_t *a) {return ga_l(a,0x72);}
static void letter(neo_gactor_t *a,neo_end_ram_t *m) {   /* 058530 */
    uint8_t d7=mb(m,name_at(a)+W(0x78));uint16_t d6=0;
    while (d6<64 && rb(CHARSET+d6)!=d7) d6++;
    SW(0x7a,d6);
}
static bool take(neo_gactor_t *a,neo_end_ram_t *m) {      /* 0583EA: carry = done */
    SW(0x7c,0xf);SW(0x82,0x12c);
    uint16_t d7=W(0x7a);
    if (rb(CHARSET+d7)==0x7f) {SW(0x7a,0);sbm(m,name_at(a)+W(0x78),rb(CHARSET));return true;}
    SW(0x78,W(0x78)+1u);
    if (S16(W(0x78))>3) return true;
    if (W(0x78)==3) return false;
    uint16_t d5=W(0x78);
    if (d5) {d5--;sbm(m,name_at(a)+d5+1u,mb(m,name_at(a)+d5));}
    letter(a,m);return false;
}
static void back(neo_gactor_t *a,neo_end_ram_t *m) {      /* 05845A */
    SW(0x7c,0xf);SW(0x82,0x12c);
    if (W(0x78)!=3) sbm(m,name_at(a)+W(0x78),rb(CHARSET));
    uint16_t v=W(0x78);SW(0x78,v-1u);
    if (!v) {SW(0x78,0);sbm(m,name_at(a)+W(0x78),rb(CHARSET+1));}
    if (!W(0x78)) SB(0x85,0);
    letter(a,m);
}
static void up(neo_gactor_t *a,neo_end_ram_t *m) {        /* 0584AE */
    if (W(0x78)==3) return;
    SW(0x7c,0xf);SW(0x82,0x12c);SW(0x7a,W(0x7a)+1u);
    uint16_t d7=W(0x7a);
    if (!rb(CHARSET+d7)) {SW(0x7a,0);d7=0;}
    sbm(m,name_at(a)+W(0x78),rb(CHARSET+d7));
}
static void down(neo_gactor_t *a,neo_end_ram_t *m) {      /* 0584F2 */
    if (W(0x78)==3) return;
    SW(0x7c,0xf);SW(0x82,0x12c);
    uint16_t v=W(0x7a);SW(0x7a,v-1u);
    if (!v) SW(0x7a,0x1f);
    sbm(m,name_at(a)+W(0x78),rb(CHARSET+W(0x7a)));
}
/* 05821A: the rank and the name with the cursor. */
static bool show(neo_gactor_t *a,neo_genv_t *e,neo_end_ram_t *m) {
    uint16_t d0;
    if (!B(0x84)) {uint16_t d1;if (!rank_of(e,m,name_at(a)-4u,true,&d0,&d1)) return false;d0=(uint16_t)(d0-1u);}
    else d0=W(0x76);
    neo_text_t t=text_of(m);
    uint32_t s=rl(0x57a2eu+4u*d0);
    const uint8_t *p=ga_rom(s);
    if (!p) {ga_fail(e,0x58240);return false;}
    uint16_t c=W(0x70);
    c=neo_text_glyph(&t,c,0x441,*p++,&p);
    c=(uint16_t)(c+1u);
    c=neo_text_glyph(&t,c,3,*p++,&p);c=neo_text_glyph(&t,c,3,*p++,&p);neo_text_glyph(&t,c,3,*p++,&p);
    c=(uint16_t)(W(0x70)+0xa0u);
    uint32_t n=name_at(a);
    for (uint16_t d7=0;d7<=3;d7++) {
        uint8_t d2=mb(m,n++);
        if (d7==W(0x78) && S16(W(0x7c))<=7) {
            const uint8_t *q=ga_rom(0x585fa);
            if (!q) return text_done(e,m,&t,0x5829e);
            neo_text_glyph(&t,c,0x441,*q,&q);   /* movem: d0 kept */
            c=(uint16_t)(c+0x40u);continue;
        }
        if (d7==3) {
            if (W(0x78)==3) {const uint8_t *q=ga_rom(0x585ef);if (q) neo_text_glyph(&t,c,0x441,*q,&q);return text_done(e,m,&t,0x582ce);}
            c=fill(&t,c,0xff,2,2);
            neo_text_glyph(&t,c,0xff,0,&p);   /* 0582E2 after 03CF12: D2 = 0, D1 = FF */
            return text_done(e,m,&t,0x582e2);
        }
        c=neo_text_glyph(&t,c,0x441,d2,&p);
    }
    return text_done(e,m,&t,0x5821a);
}
#ifdef NEO_ENDING_CREDITS
/* ---- the credits (docs/MISION6.md "Los créditos") ---------------------------
 * 04E5D0: the signed 16.16 product (both below 1.0: the product of the low
 * words; else the 64-bit product 04E57A, a TRAP #F when it passes 2^63). */
static uint32_t fmul(neo_genv_t *e,uint32_t d0,uint32_t d1) {
    bool neg=false;
    if (d0&0x80000000u) {d0=0u-d0;neg=!neg;}
    if (d1&0x80000000u) {d1=0u-d1;neg=!neg;}
    if (d0<0x10000u && d1<0x10000u) {uint32_t r=(d0*d1)>>16;return neg?0u-r:r;}
    uint64_t p=(uint64_t)d0*d1;
    if (p>>63) {ga_fail(e,0x4e578);return 0;}
    if (neg) p=0u-p;
    return (uint32_t)(p>>16);
}
/* 04E962: the signed 16.16 quotient (04E648: 48 steps of long division; 0 / x = 0, x / 0 is a TRAP #F). */
static uint32_t fdiv(neo_genv_t *e,uint32_t d0,uint32_t d1) {
    bool neg=false;
    if (d0&0x80000000u) {d0=0u-d0;neg=!neg;}
    if (d1&0x80000000u) {d1=0u-d1;neg=!neg;}
    if (!d0) return 0;
    if (!d1 || (d1&0x80000000u)) {ga_fail(e,0x4e578);return 0;}   /* a divisor of 2^31: the 32-bit remainder would overflow (not followed) */
    uint32_t r=(uint32_t)(((uint64_t)d0<<16)/d1);
    return neg?0u-r:r;
}
/* 0A4996: a time or speed for the 50 Hz machine (RAM 100002 = FF: times 0.D600). */
static uint32_t pal(neo_genv_t *e,neo_end_ram_t *m,uint32_t d0) {return mb(m,0x100002)==0xffu?fmul(e,d0,0xd600u):d0;}
/* 0A49B0: the step of an update (RAM 10C604: a button held, four times faster). */
static uint32_t step_of(neo_end_ram_t *m) {return mb(m,0x10c604)?0x40000u:0x10000u;}
/* 009CCE(slot, target, level): a palette slot of the node blended towards the palette `target` (level 0: its own
 * colours; the credits: target 1, level 0x20 = black). */
static void slot_blend(neo_end_ram_t *m,neo_gactor_t *a,uint8_t handle,uint16_t target,uint16_t level) {
    m->slot_levels++;
    if (m->slot_level) m->slot_level(m->slot_ctx,a,handle,target,(uint8_t)level);
}
static void slot_level(neo_end_ram_t *m,neo_gactor_t *a,uint8_t handle,uint16_t level) {slot_blend(m,a,handle,1,level);}
/* asr.l by a register count (modulo 64). */
static int32_t asr(int32_t v,uint16_t count) {count&=63u;return count>=32u?(v<0?-1:0):v>>count;}
/* 0029C6 and the N flag it returns with (the ring word it changed). */
static uint16_t rnd(neo_end_ram_t *m) {return m->rand?m->rand(m->rand_ctx):0u;}
static bool rnd_negative(neo_end_ram_t *m) {return m->rand_negative?m->rand_negative(m->rand_ctx):false;}
/* 0A489A: (sine(d0) * d1) >> 8. */
static uint32_t sine_times(uint16_t d0,uint16_t d1) {return (uint32_t)(((int32_t)ga_sine((uint8_t)d0)*S16(d1))>>8);}
/* 0646FE + 0x0A k: the page k of the table 0E1446 (26 bytes: the first palette and six descriptors). */
static ga_result_t page(neo_gactor_t *a,neo_genv_t *e,neo_end_ram_t *m) {
    SW(0x84,(L(0)-0x646feu)/10u);
    SL(0x74,!W(0x6e)?pal(e,m,0x3c0000u):W(0x6e)==0xffffu?0u:pal(e,m,(uint32_t)W(0x6e)<<16));
    SL(0x78,pal(e,m,W(0x70)?(uint32_t)W(0x70)<<16:0x1e0000u));
    SL(0x7c,pal(e,m,W(0x72)?(uint32_t)W(0x72)<<16:0x1e0000u));
    SL(0x8c,0x200000u);
    uint32_t a1=0xe1446u+0x1au*W(0x84);
    SW(0x94,rw(a1));a1+=2;SW(0x86,0);
    for (;W(0x86)<6;SW(0x86,W(0x86)+1u)) {
        neo_gactor_t *c=spawn(a,e,0x64ac0);
        if (!c) STOP(0x649b8);
        ga_sl(c,0x3c,L(0x3c));ga_sl(c,0x40,L(0x40));
        ga_sl(c,0x80,rl(a1));a1+=4;
        ga_sw(c,0x94,W(0x94));SW(0x94,W(0x94)+1u);
    }
    SL(0,0x649e8);
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* 0A4744: the parade picture's start. */
static bool parade_start(neo_gactor_t *a,neo_genv_t *e,neo_end_ram_t *m) {
    SW(0x66,0xfff1);SW(0x1e,0xfff1);
    if (!ga_anim_start(a,e,0x2a4b16,0x100)) return false;
    uint8_t d0=(uint8_t)(W(0x7a)-1u);SB(0xa8,d0);
    uint16_t res=(int8_t)d0<0x3d?0x845:0x125;
    ga_palette(a,res);ga_palette(a,res);                                    /* 009EA8 with D1 = 1: slots of its own */
    uint32_t v=(uint32_t)(int32_t)S16(W(0x76))<<8;
    if (mb(m,0x100002)==0xffu) v=fmul(e,v,0x10000u-0xd600u)+v;
    if (W(0x6e)==2) {SL(0x34,v);SL(0x30,0);}
    else {uint32_t vx,vy;ga_polar(v,(uint8_t)W(0x78),&vx,&vy);SL(0x30,vx);SL(0x34,vy);}   /* 002E7C */
    if (W(0x6e)==1) {
        SL(0x84,pal(e,m,(uint32_t)(int32_t)S16(W(0x70))<<8));
        SL(0x8c,pal(e,m,(uint32_t)(int32_t)S16(W(0x74))<<8));
    } else {
        SL(0x84,pal(e,m,(uint32_t)W(0x70)<<16));
        SL(0x88,pal(e,m,(uint32_t)W(0x72)<<16));
        SL(0x8c,pal(e,m,(uint32_t)(int32_t)S16(W(0x74))<<8));
    }
    SL(0x94,L(0x30));SL(0x98,L(0x34));
    return !e->unsupported;
}
/* 0A49CC / 0A4A1C: the speeds of this update (four times with RAM 10C604). */
static void parade_speed(neo_gactor_t *a,neo_genv_t *e,neo_end_ram_t *m,bool wave) {
    bool fast=mb(m,0x10c604)!=0;
    if (wave) SL(0x84,pal(e,m,(uint32_t)(int32_t)S16(fast?(uint16_t)(W(0x70)<<2):W(0x70))<<8));
    SL(0x8c,pal(e,m,(uint32_t)(int32_t)S16(fast?(uint16_t)(W(0x74)<<2):W(0x74))<<8));
    SL(0x30,fast?L(0x94)<<2:L(0x94));SL(0x34,fast?L(0x98)<<2:L(0x98));
}
#endif
/* ---- the update ---------------------------------------------------------- */
static ga_result_t ending_tick(struct neo_end *s,neo_gactor_t *a,neo_genv_t *e,neo_end_ram_t *m) {
    (void)s;
    if (!m) STOP(L(0));
#ifdef NEO_ENDING_CREDITS
    if (L(0)>=0x646feu && L(0)<=0x648d4u && (L(0)-0x646feu)%10u==0) return page(a,e,m);
#endif
    switch (L(0)) {
    /* ---- 01959C: the line the mission actor leaves on the last screen (018B9E, stage 0x2C) ----
     * 16 updates, then the string 019582 typed a glyph an update (03D292 / 03D2B0 with the state block in the node,
     * +6E..+85: +0 d0, +2 d3, +4 d1, +6 a0, +A a1, +E a2, +16 count); deleted at its end. D0 / D1 at 03D292 are the
     * dispatcher's dead registers: the string's own escapes set the place and the colour before the first glyph. */
    case 0x1959c:
        SW(0x26,0x0f);SL(0,0x195a8);
        /* fall through */
    case 0x195a8: {
        uint16_t v=W(0x26);SW(0x26,v-1u);
        if (v) return GA_MORE;                                               /* subq.w #1; bcs */
        SL(0x74,0x19582);SL(0x78,0x3dd96);SL(0x7c,0);SW(0x84,0);
        SL(0,0x195d0);
    }   /* fall through */
    case 0x195d0: {
        neo_text_t t=text_of(m);
        uint32_t a0=L(0x74);uint16_t d0=W(0x6e),d1=W(0x72),d3=W(0x70);
        const uint8_t *base=ga_rom(a0),*p=base;
        if (!base) STOP(0x19582);
        uint8_t d2=neo_text_next(&t,&p,&d0,&d1,&d3);
        if (!d2) return GA_DELETE;                                           /* 0195DE: 004188 */
        SW(0x70,d3);SW(0x72,d1);SW(0x6e,d0);
        if (d2=='%') STOP(0x3d2b0);
        d0=neo_text_glyph(&t,W(0x6e),d1,d2,&p);
        SW(0x6e,d0);SL(0x74,a0+(uint32_t)(p-base));
        if (!text_done(e,m,&t,0x195d0)) return GA_UNSUPPORTED;
        return GA_MORE;
    }
    /* ---- root 0570A0 ---- */
    case 0x570a0: {
        if (m->setup) m->setup(m->ram_ctx);                                  /* 056FCC */
        m->setups++;
        if (m->fade) neo_fade_start(m->fade,0,0,0,1);
        sound(m,0x2e);
        if (!spawn(a,e,0x577e2)) STOP(0x570b8);
        uint16_t r;
        SW(0x74,1);if (!rank(a,e,m,&r)) return GA_UNSUPPORTED;SW(0x7c,r);
        SW(0x74,0);if (!rank(a,e,m,&r)) return GA_UNSUPPORTED;SW(0x7a,r);
        SW(0x72,0x1e);SL(0,0x570e6);
    }   /* fall through */
    case 0x570e6:
        if (DEC72()) return GA_MORE;
        sound(m,0x1151);
        if (!spawn(a,e,0x576b6)) STOP(0x57104);
        SB(0x60,0);SL(0,0x57114);
        /* fall through */
    case 0x57114:
        if (!B(0x60)) return GA_MORE;
        SW(0x72,0x1e);SL(0,0x5712a);
        /* fall through */
    case 0x5712a:
        if (DEC72()) return GA_MORE;
        if (!spawn(a,e,0x5764a)) STOP(0x5713e);
        SB(0x60,0);SL(0,0x5714e);
        /* fall through */
    case 0x5714e:
        if (!B(0x60)) return GA_MORE;
        SW(0x72,0xf);SL(0,0x57164);
        /* fall through */
    case 0x57164: {
        if (DEC72()) return GA_MORE;
        neo_gactor_t *c1=spawn(a,e,0x5731c),*c2=c1?spawn(a,e,0x5731c):NULL;
        if (!c2) STOP(0x57178);
        uint16_t d0=0,d1=1;
        if (ml(m,0x10c46a)<ml(m,0x10c472)) {d0=1;d1=0;}
        ga_sw(c2,0x74,d0);ga_sw(c1,0x74,d1);   /* 0571A6: A0 = the second, A1 = the first */
        SB(0x60,0);SL(0,0x571ba);
    }   /* fall through */
    case 0x571ba:
        if (B(0x60)!=2) return GA_MORE;
        if (m->fade) neo_fade_start(m->fade,-31,-31,-31,0x10);
        m->audio_commands++;m->audio_command=0x80;                           /* 01228C(0x80) */
        SW(0x72,0x14);SL(0,0x571ea);
        /* fall through */
    case 0x571ea:
        if (DEC72()) return GA_MORE;
        creator60(a,e,m,false);
        return GA_DELETE;
    /* ---- dial ---- */
    case 0x577e2: {
        static const uint32_t r[4]={0x5785a,0x57862,0x5785a,0x57862};
        for (unsigned i=0;i<4;i++) {
            neo_gactor_t *c=spawn(a,e,r[i]);
            if (!c) STOP(0x577ec);
            if (i==1 || i==2) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|1u));
            ga_sw(c,0x76,(uint16_t)(0x4000u*i));
        }
        SW(0x76,0x4000);SL(0,0x57852);
    }   /* fall through */
    case 0x57852:
        SW(0x76,W(0x76)-0x100u);
        return GA_MORE;
    case 0x5785a:
    case 0x57862:
        START(rl(0x56fc4u+(L(0)==0x57862?4u:0u)));
        ga_palette(a,0x35f);
        SL(0x28,0);SL(0x2c,0x01800000u);SW(0x66,0);SW(0x1e,0);SL(0,0x578a8);
        /* fall through */
    case 0x578a8: {
        ga_project(a,e);
        neo_gactor_t *c=e->creator(e->ctx,a);
        uint16_t d1=(uint16_t)((c?ga_w(c,0x76):0u)+W(0x76));
        if (d1&0x8000u) return GA_MORE;
        uint8_t angle=(uint8_t)(d1>>8);
        int32_t y=(int32_t)0xff*ga_sine(angle);
        SB(0x12,(uint8_t)((uint16_t)y>>8));
        int32_t x=(int32_t)0x88*ga_cosine(angle);
        SW(0x28,(uint16_t)(((uint32_t)x>>8)+0xa0u));
        STEP();
        return GA_MORE;
    }
    /* ---- title 0576B6 and frame 05764A ---- */
    case 0x576b6:
        type_start(m,0x10c450,0x708f,0,0x56f16);
        SL(0,0x576d6);
        /* fall through */
    case 0x576d6: {
        neo_text_t t=text_of(m);
        int r=type_step(m,&t,0x10c450);
        if (!text_done(e,m,&t,0x3d2b0) || r<0) return GA_UNSUPPORTED;
        if (!r) return GA_MORE;
        creator60(a,e,m,false);
        return GA_DELETE;
    }
    case 0x5764a:
        SW(0x28,4);SW(0x2c,0xf);SL(0,0x5765c);
        /* fall through */
    case 0x5765c: {
        neo_text_t t=text_of(m);
        print(m,&t,(uint16_t)((uint16_t)(W(0x28)<<5)+W(0x2c)+0x7000u),0x56f3a);
        SW(0x2c,W(0x2c)-1u);
        print(m,&t,(uint16_t)((uint16_t)(W(0x28)<<5)+W(0x2c)+0x7000u),0x56f16);
        if (!text_done(e,m,&t,0x3d3b2)) return GA_UNSUPPORTED;
        if (W(0x2c)!=5) return GA_MORE;
        creator60(a,e,m,false);
        return GA_DELETE;
    }
    /* ---- result 05731C ---- */
    case 0x5731c: {
        if (!mb(m,0x10fdb6u+(W(0x74)&1u))) {creator60(a,e,m,true);return GA_DELETE;}   /* 057520 */
        neo_gactor_t *c=e->creator(e->ctx,a);
        SW(0x78,c?ga_w(c,W(0x74)?0x7c:0x7a):0u);
        neo_text_t t=text_of(m);
        print(m,&t,W(0x74)?0x72aa:0x706a,W(0x74)?0x56f66:0x56f52);         /* 0572AA */
        if (!text_done(e,m,&t,0x3d3b2)) return GA_UNSUPPORTED;
        SW(0x72,0xf);SL(0,0x5735c);
    }   /* fall through */
    case 0x5735c: {
        if (DEC72()) return GA_MORE;
        neo_text_t t=text_of(m);
        print(m,&t,W(0x74)?0x72ad:0x706d,0x56f7a);                          /* 05728A */
        if (!text_done(e,m,&t,0x3d3b2)) return GA_UNSUPPORTED;
        SW(0x72,8);SL(0,0x57376);
    }   /* fall through */
    case 0x57376:
        if (DEC72()) return GA_MORE;
        SW(0x72,0x1e);SL(0,0x5738c);
        /* fall through */
    case 0x5738c:
    case 0x5742c: {
        bool second=L(0)==0x5742c;
        uint16_t d0;
        if (DEC72()) d0=(uint16_t)(((uint32_t)(m->rand?m->rand(m->rand_ctx):0u)*0x3e8u)>>16);   /* 0029F2 */
        else {
            SL(0,second?0x57490:0x573f0);
            d0=mw(m,(second?0x10e9e8u:0x10e9e4u)+2u*(W(0x74)&1u));         /* 0C8C7E */
            if (d0>0x3e7) d0=0x3e7;
        }
        SW(0x6e,bcd(d0));
        neo_text_t t=text_of(m);
        neo_text_bcd(&t,second?(W(0x74)?0x7430:0x71f0):(W(0x74)?0x742d:0x71ed),0x461,&a->m[0x6e],0,3);
        return text_done(e,m,&t,0x3cfba)?GA_MORE:GA_UNSUPPORTED;
    }
    case 0x573f0:
        SW(0x72,0xf);SL(0,0x573fc);
        /* fall through */
    case 0x573fc: {
        if (DEC72()) return GA_MORE;
        neo_text_t t=text_of(m);
        print(m,&t,W(0x74)?0x72b0:0x7070,mb(m,0x100000)?0x56f9c:0x56f8a);   /* 0572EE */
        if (!text_done(e,m,&t,0x3d3b2)) return GA_UNSUPPORTED;
        SW(0x72,8);SL(0,0x57416);
    }   /* fall through */
    case 0x57416:
        if (DEC72()) return GA_MORE;
        SW(0x72,0x1e);SL(0,0x5742c);
        return ending_tick(s,a,e,m);   /* 057426 -> 05742C in the same update */
    case 0x57490:
        SW(0x72,0xf);SL(0,0x5749c);
        /* fall through */
    case 0x5749c: {
        if (DEC72()) return GA_MORE;
        neo_text_t t=text_of(m);
        print(m,&t,W(0x74)?0x72b4:0x7074,0x56fb8);                          /* 0572CE */
        uint32_t sc=W(0x74)?0x10c472:0x10c46a;uint8_t digits[4];
        for (unsigned i=0;i<4;i++) digits[i]=mb(m,sc+i);
        neo_text_bcd(&t,W(0x74)?0x7394:0x7154,0x461,digits,0,8);
        if (!text_done(e,m,&t,0x3cfba)) return GA_UNSUPPORTED;
        SW(0x72,0x1e);SL(0,0x574e6);
    }   /* fall through */
    case 0x574e6: {
        if (DEC72()) return GA_MORE;
        neo_gactor_t *c=spawn(a,e,0x5752e);
        if (!c) STOP(0x574fa);
        ga_sw(c,0x74,W(0x74));ga_sw(c,0x78,W(0x78));
        SB(0x60,0);SL(0,0x57516);
    }   /* fall through */
    case 0x57516:
        if (!B(0x60)) return GA_MORE;
        creator60(a,e,m,true);
        return GA_DELETE;
    /* ---- entry 05752E ---- */
    case 0x5752e:
        if (W(0x78)>=10) {SW(0x72,0x3c);SL(0,0x57544);}
        else {
            neo_gactor_t *c=spawn(a,e,0x5812a);                              /* 058112 */
            if (!c) STOP(0x5811e);
            ga_sw(c,0x76,W(0x78));ga_sw(c,0x6e,W(0x74));
            ga_sw(c,0x70,W(0x74)?0x7318:0x70d8);ga_sl(c,0x72,W(0x74)?0x10c476:0x10c46e);
            SB(0x60,0);SL(0,0x57590);
            if (!B(0x60)) return GA_MORE;
            if (!insert(e,m,W(0x74)?0x10c472:0x10c46a)) return GA_UNSUPPORTED;
            creator60(a,e,m,false);return GA_DELETE;
        }
        /* fall through */
    case 0x57544:
        if (DEC72()) return GA_MORE;
        creator60(a,e,m,false);
        return GA_DELETE;                                                   /* 0575B4 */
    case 0x57590:
        if (!B(0x60)) return GA_MORE;
        if (!insert(e,m,W(0x74)?0x10c472:0x10c46a)) return GA_UNSUPPORTED;
        creator60(a,e,m,false);
        return GA_DELETE;
    /* ---- name entry 05812A ---- */
    case 0x5812a: {
        SB(0x84,0xff);
        uint32_t s4=name_at(a)-4u;
        if (s4==0x10c46a) sbm(m,0x10c468,0xff);
        if (s4==0x10c472) sbm(m,0x10c469,0xff);
        SW(0x78,0);
        copy_rom(m,name_at(a),0x585f2,3);
        letter(a,m);
        SW(0x7c,0xf);swm(m,0x10c380,0x2004);                                 /* 03CAB4 */
        SW(0x80,0x258);SW(0x82,0x12c);SB(0x85,0);SL(0,0x58198);
    }   /* fall through */
    case 0x58198: {
        if (!mb(m,0x10c2e6)) return GA_MORE;
        if (W(0x6e)>1) STOP(0x5859a);
        uint8_t d0=mb(m,(W(0x6e)?0x10c360u:0x10c35au)+3u);
        bool done=false;
        if (d0&0x10u) {SB(0x85,0xff);done=take(a,m);}
        else if (d0&0x20u) back(a,m);
        else {
            d0=mb(m,W(0x6e)?0x10c364u:0x10c35eu);
            if (d0&8u) up(a,m); else if (d0&4u) down(a,m);
        }
        if (!done) {
            SW(0x7c,W(0x7c)-1u);
            if (!W(0x7c)) SW(0x7c,0xf);
            SW(0x80,W(0x80)-1u);
            if (W(0x80)) {SW(0x82,W(0x82)-1u);if (W(0x82)) return show(a,e,m)?GA_MORE:GA_UNSUPPORTED;}
        }
        /* 0582F2 */
        SW(0x78,0xffff);SW(0x7c,0);SW(0x7e,0x3c);
        uint32_t n=name_at(a);
        if (!B(0x85)) copy_rom(m,n,0x585f6,3);                              /* 057AE0 */
        for (uint32_t k=0x585fc;rl(k);k+=4) {
            uint32_t w=rl(k);bool same=true;
            for (unsigned i=0;i<3;i++) if (rb(w+i)!=mb(m,n+i)) same=false;   /* 057AA6 */
            if (same) copy_rom(m,n,0x585f6,3);
        }
        for (unsigned i=0;i<3;i++) if (mb(m,n+i)==0x7f) sbm(m,n+i,0x20);     /* 057AC4 */
        neo_text_t t=text_of(m);
        fill(&t,(uint16_t)(W(0x70)+0xa2u),0xff,8,2);
        if (!text_done(e,m,&t,0x58358)) return GA_UNSUPPORTED;
        SL(0,0x58364);
    }   /* fall through */
    case 0x58364:
        SW(0x7e,W(0x7e)-1u);
        if (S16(W(0x7e))>0x24) {
            SW(0x7c,W(0x7c)+1u);
            if (S16(W(0x7c))>=6) SW(0x7c,0);
            if (S16(W(0x7c))<4) return show(a,e,m)?GA_MORE:GA_UNSUPPORTED;
            neo_text_t t=text_of(m);
            fill(&t,W(0x70),0xff,0xb,2);
            return text_done(e,m,&t,0x5839e)?GA_MORE:GA_UNSUPPORTED;
        }
        if (!show(a,e,m)) return GA_UNSUPPORTED;
        SL(0,0x583ae);
        /* fall through */
    case 0x583ae: {
        uint16_t v=W(0x7e);SW(0x7e,v-1u);
        if (v) return GA_MORE;
        creator60(a,e,m,false);
        uint32_t s4=name_at(a)-4u;
        if (s4==0x10c46a) sbm(m,0x10c468,0);
        if (s4==0x10c472) sbm(m,0x10c469,0);
        return GA_DELETE;
    }
    /* ---- the ranking 057E00 (kernel 101154, 016D3C) ---- */
    case 0x57e00: {
        if (m->setup) m->setup(m->ram_ctx);                                  /* 057F34: 009E6C, 03D4EE, 0C81A6(0579BA), 017FD0 */
        m->setups++;
        if (m->fade) neo_fade_start(m->fade,0,0,0,8);
        neo_gactor_t *c=spawn(a,e,0x580a2);
        if (!c) STOP(0x57e0e);
        ga_place_at(c,a,e,0xa0,(int16_t)0xff80);                             /* 01ACF2 */
        SW(0x6e,0x5a);SW(0x76,0);SW(0x70,0);SW(0x72,0);SB(0x78,0);SW(0x74,1);SL(0,0x57e44);
    }   /* fall through */
    case 0x57e44: {
        if (!B(0x78)) return GA_MORE;
        neo_text_t t=text_of(m);uint16_t d1=0x441;                            /* 058074 */
        neo_text_string(&t,0x7085,&d1,ga_rom(0x57a86));
        neo_text_string(&t,0x71a5,&d1,ga_rom(0x57a8b));
        neo_text_string(&t,0x72c5,&d1,ga_rom(0x57a90));
        uint16_t v=W(0x74);SW(0x74,v-1u);
        if (v) return text_done(e,m,&t,0x3e2a8)?GA_MORE:GA_UNSUPPORTED;
        SW(0x74,0);SW(0x72,W(0x72)+1u);
        if (W(0x72)>1) {if (W(0x70)!=9) SW(0x70,W(0x70)+1u);SW(0x72,0);}
        uint16_t row=W(0x70);                                                 /* 057F60 */
        if (row>=10) {ga_fail(e,0x58012);return GA_UNSUPPORTED;}
        uint32_t a0=0x10f010u+8u*row,a1=0x10f010;uint16_t d0=1,d1r=1;        /* 05800A */
        for (;;) {
            if (ml(m,a0)<ml(m,a1)) d0=(uint16_t)(d1r+1u);
            if (d1r==row+1u) break;
            a1+=8;d1r++;
            if (S16(d1r)>10) {ga_fail(e,0x58072);return GA_UNSUPPORTED;}
        }
        const uint8_t *p=ga_rom(rl(0x57a2eu+4u*(uint16_t)(d0-1u)));
        if (!p) {ga_fail(e,0x57f72);return GA_UNSUPPORTED;}
        uint16_t c=(uint16_t)(row*2u+0x70a8u);
        c=neo_text_glyph(&t,c,0x441,*p++,&p);c=(uint16_t)(c+1u);
        c=neo_text_glyph(&t,c,3,*p++,&p);c=neo_text_glyph(&t,c,3,*p++,&p);neo_text_glyph(&t,c,3,*p++,&p);
        uint8_t digits[4];for (unsigned i=0;i<4;i++) digits[i]=mb(m,a0+i);
        neo_text_bcd(&t,(uint16_t)(row*2u+0x7168u),0x441,digits,0,8);
        c=(uint16_t)(row*2u+0x73a8u);
        for (unsigned i=0;i<3;i++) {uint8_t ch=mb(m,a0+4u+i);const uint8_t *q=&ch;c=neo_text_glyph(&t,c,0x441,ch,&q);}
        if (!text_done(e,m,&t,0x57f60)) return GA_UNSUPPORTED;
        if (row==9) SL(0,0x57e94);   /* 057E94 counts in every update that drew */
    }   /* fall through */
    case 0x57e94: {
        SW(0x6e,W(0x6e)-1u);
        if (W(0x6e)) return GA_MORE;
        creator60(a,e,m,false);
        return GA_DELETE;
    }
    case 0x580a2: {
        uint16_t d0=(uint16_t)((m->rand?m->rand(m->rand_ctx):0u)&1u);         /* 0029C6 */
        START(rl(0x24ba3eu+4u*d0));
        ga_palette(a,0x35f);
        SW(0x66,0);SW(0x1e,0);SB(0x12,0);ga_project(a,e);SL(0,0x580ee);
    }   /* fall through */
    case 0x580ee:
        if ((unsigned)B(0x12)+0x10u>0xffu) {
            SB(0x12,0xff);
            neo_gactor_t *c=e->creator(e->ctx,a);
            if (c) ga_sb(c,0x78,0xff);
            SL(0,0x5810a);
        } else SB(0x12,B(0x12)+0x10u);
        /* fall through */
    case 0x5810a:
        STEP();
        return GA_MORE;
#ifdef NEO_ENDING_CREDITS
    /* ---- credits: a page 0646FE + 0x0A k (page(): +84 = k) ---- */
    case 0x649e8: {
        uint32_t d1=L(0x78);if (!d1) d1=0x10000u;
        SL(0x90,fdiv(e,0xffe00000u,d1));SL(0,0x64a0c);
    }   /* fall through */
    case 0x64a0c: {
        uint32_t d2=L(0x8c)+fmul(e,step_of(m),L(0x90));
        if ((int32_t)d2<=0) {d2=0;SL(0,0x64a3a);}
        SL(0x8c,d2);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x64a3a:
        SL(0x88,L(0x74));SL(0,0x64a46);
        /* fall through */
    case 0x64a46:
        if (L(0x88)) {
            uint32_t v=L(0x88)-step_of(m);SL(0x88,v);
            if ((int32_t)v<=0) SL(0,0x64a64);
        }
        return GA_MORE;
    case 0x64a64: {
        uint32_t d1=L(0x7c);if (!d1) d1=0x10000u;
        SL(0x90,fdiv(e,0x200000u,d1));SL(0,0x64a88);
    }   /* fall through */
    case 0x64a88: {
        uint32_t d2=L(0x8c)+fmul(e,step_of(m),L(0x90));
        if (d2>=0x200000u) {d2=0x200000u;SL(0,0x64aba);}
        SL(0x8c,d2);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x64aba:
        return GA_DELETE_TREE;                                               /* 0041AC */
    case 0x64ac0:
        ga_palette(a,W(0x94));SW(0x66,0);SW(0x1e,0);SL(0,0x64ae0);
        /* fall through */
    case 0x64ae0: {
        neo_gactor_t *c=e->creator(e->ctx,a);
        slot_level(m,a,B(0x14),c?ga_w(c,0x8c):0u);                           /* 009CCE(+14, 1, creator +8C) */
        ga_draw(a,e,L(0x80));                                                /* 011FF6 */
        return GA_MORE;
    }
    /* ---- the music 0A4408, the timers 0A4478, the audio command 0A453A ---- */
    case 0xa4408:
        sbm(m,0x10c604,0);SB(0x61,0);
        SL(0x80,pal(e,m,(uint32_t)W(0x6e)<<16));
        if (e->unsupported) return GA_UNSUPPORTED;
        SL(0,0xa4426);
        /* fall through */
    case 0xa4426: {
        uint8_t d0=0;                                                        /* 0190BC: D0 only */
        if (mb(m,0x10fdb6)) d0|=mb(m,0x10c35c);
        if (mb(m,0x10fdb7)) d0|=mb(m,0x10c362);
        sbm(m,0x10c604,(d0&0x70u)?0xffu:0u);                                 /* andi.w #$7070 */
        if (B(0x61)) return GA_MORE;
        if (!L(0x80)) {sound(m,0x31);SB(0x61,0xff);return GA_MORE;}
        uint32_t v=L(0x80)-step_of(m);SL(0x80,(int32_t)v<0?0u:v);
        return GA_MORE;
    }
    case 0xa4478:
        SL(0x80,pal(e,m,(uint32_t)W(0x6e)<<16));
        if (e->unsupported) return GA_UNSUPPORTED;
        SL(0,0xa448e);
        /* fall through */
    case 0xa448e: {
        if (!L(0x80)) return GA_DELETE;                                      /* 0A4496: the cartridge check, then 004188 */
        uint32_t v=L(0x80)-step_of(m);SL(0x80,(int32_t)v<0?0u:v);
        return GA_MORE;
    }
    case 0xa453a:
        m->audio_commands++;m->audio_command=W(0x6e);                        /* 01228C(+6E) */
        return GA_DELETE;
    /* ---- the parade's pictures 0A454C ---- */
    case 0xa454c:
        ga_project(a,e);
        if (!parade_start(a,e,m)) return GA_UNSUPPORTED;
        if (W(0x6e)==1) {
            uint32_t d0=0x1780000u-L(0x40);
            if ((int32_t)L(0x34)<0) d0=0u-d0;
            uint32_t t=fdiv(e,d0,L(0x34));
            uint32_t p=fmul(e,t,L(0x8c))+0x400000u;
            if (fdiv(e,p,0x800000u)&0x10000u) SB(0x15,B(0x15)|1u);
            SL(0x9c,0x800000u-(p&0x7fffffu));
            SL(0xa4,0u-fmul(e,t,L(0x84)));
            SL(0,0xa4698);
        } else {SL(0xa0,0x200000u);SL(0,0xa45ec);}
        SB(0x60,0);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0xa45ec:
        parade_speed(a,e,m,false);
        ga_move(a,e);
        if (!(B(0x60)&1u)) {
            if (!L(0x84)) {
                STEP();
                if ((int32_t)L(0xa0)>0) {                                    /* 0A48C2 */
                    uint32_t v=L(0xa0)-L(0x8c);SL(0xa0,(int32_t)v<0?0u:v);
                    slot_level(m,a,B(0x19),W(0xa0));slot_level(m,a,B(0x1a),W(0xa0));
                } else {SB(0x60,B(0x60)|1u);slot_level(m,a,B(0x19),0);slot_level(m,a,B(0x1a),0);}
            } else {uint32_t v=L(0x84)-step_of(m);SL(0x84,(int32_t)v<0?0u:v);}
        } else {
            STEP();
            if (B(0x60)&2u) SL(0,0xa4724);
            else if (!L(0x88)) {                                             /* 0A492E */
                if ((int32_t)L(0xa0)<=0x200000) {
                    SL(0xa0,L(0xa0)+L(0x8c));
                    slot_level(m,a,B(0x19),W(0xa0));slot_level(m,a,B(0x1a),W(0xa0));
                } else {SB(0x60,B(0x60)|2u);slot_level(m,a,B(0x19),0x20);slot_level(m,a,B(0x1a),0x20);}
            } else {uint32_t v=L(0x88)-step_of(m);SL(0x88,(int32_t)v<0?0u:v);}
        }
        if (W(0x6e)==2 && S16(W(0x78))<=S16(W(0x2c))) {SL(0x30,0);SL(0x34,0);SL(0x94,0);SL(0x98,0);SW(0x6e,0);}
        if (e->unsupported) return GA_UNSUPPORTED;
        return ga_cull(a,e,0xe61b4)?GA_DELETE_TREE:GA_MORE;                  /* 0A472C: 01945C, 0041AC */
    case 0xa4698: {   /* +6E = 1: the swaying picture (no capture: the TAS's credits have none) */
        parade_speed(a,e,m,true);
        ga_move(a,e);
        uint32_t keep=L(0x3c);
        SW(0x3c,W(0x3c)+(uint16_t)(S16((uint16_t)sine_times(W(0xa4),W(0x72)))>>8));
        STEP();
        SL(0x3c,keep);
        SL(0xa4,L(0xa4)+L(0x84));
        uint16_t d0=(uint16_t)sine_times(W(0x9c),0xff);
        if (d0&0x8000u) d0=(uint16_t)(0u-d0);
        SB(0x12,d0);
        SL(0x9c,L(0x9c)+L(0x8c));
        if (L(0x9c)>=0x800000u) {SL(0x9c,L(0x9c)&0x7fffffu);SB(0x15,B(0x15)^1u);}
        SB(0x14,(B(0x15)&1u)?B(0x1a):B(0x19));
        if (e->unsupported) return GA_UNSUPPORTED;
        return ga_cull(a,e,0xe61b4)?GA_DELETE_TREE:GA_MORE;
    }
    case 0xa4724:
        return GA_DELETE_TREE;                                               /* 0041AC */
    /* ---- the title 0A7FFE (kernel 101154, 01674A) ---- */
    case 0xa7ffe:
        if (m->setup) m->setup(m->ram_ctx);                                  /* 002976(FFFF): the random ring; 01AA28(0A7D4A) */
        m->setups++;
        SB(0x60,0);SB(0x70,0);SW(0x6e,0);SL(0,0xa802a);
        /* fall through */
    case 0xa802a: {
        uint16_t v=W(0x6e);SW(0x6e,v-1u);
        if (!v) {
            neo_gactor_t *c=spawn(a,e,0xa81a4);
            if (!c) STOP(0xa803c);
            ga_sb(c,0x70,B(0x70));
            ga_sw(c,0x72,rw(0xa7fd6u+4u*B(0x70)));SW(0x6e,rw(0xa7fd8u+4u*B(0x70)));
            SB(0x70,B(0x70)+1u);
            if (B(0x70)>=0xau) SL(0,0xa8076);
        }
    }   /* fall through */
    case 0xa8076:
        if (!B(0x60)) return GA_MORE;
        if (m->fade) neo_fade_start(m->fade,0x1f,0x1f,0x1f,0);
        SL(0,0xa8096);
        return GA_MORE;
    case 0xa8096: {
        for (unsigned i=0;i<neo_end_capacity(s);i++) {                        /* 004192: every other node */
            neo_gactor_t *o=&neo_end_nodes(s)[i];
            if (o!=a && o->active && !o->placeholder) {ga_sl(o,0,0x3f1e);o->placeholder=true;}
        }
        /* 0A863C: the layers 0A7D52 (01AA28) and their maps and palettes (0C81A6: the list 0A7F02, or 0A7E5E when RAM
         * 100001 = 0) are the environment's (ram->setup); its fix texts are written here: the line 0A7E3A (03E2A8 =
         * 03D25A with 03DD96; the string sets its own place, palette and page) unless RAM 100001 = 0, and the 6 x 3
         * block of tiles from 0x6E5A at 709A (03CF60). */
        if (m->setup) m->setup(m->ram_ctx);
        m->setups++;
        {
            neo_text_t t=text_of(m);
            if (mb(m,0x100001)) {uint16_t d1=0;neo_text_string(&t,0,&d1,ga_rom(0xa7e3a));}
            uint16_t d0=0x709a,d1=0x6e5a;
            for (unsigned d2=6;d2;d2--) {                                    /* 03CF60 */
                uint16_t ad=d0,w=d1;
                for (unsigned d6=3;d6;d6--) {
                    neo_text_word(&t,ad,w);ad=(uint16_t)(ad+1u);w=(uint16_t)((w&0xff00u)|(uint8_t)(w+0x10u));
                }
                d1=(uint16_t)((d1&0xff00u)|(uint8_t)(d1+1u));d0=(uint16_t)(d0+0x20u);
            }
            if (!text_done(e,m,&t,0xa863c)) return GA_UNSUPPORTED;
        }
        if (!spawn(a,e,0xa8602)) STOP(0xa80aa);
        SL(0,0xa80b8);
        return GA_MORE;
    }
    case 0xa80b8:
        START(0x2a7368);
        ga_palette(a,0xde);
        SW(0x3c,0x120);SW(0x40,0x130);
        if (m->fade) neo_fade_start(m->fade,0,0,0,0x10);
        SW(0x6e,0x96);SL(0,0xa80fc);
        /* fall through */
    case 0xa80fc: {
        STEP();
        uint16_t v=W(0x6e);SW(0x6e,v-1u);
        if (v) return GA_MORE;
        creator60(a,e,m,false);                                              /* then the cartridge check 0A8116 */
        return GA_DELETE;
    }
    /* A piece of the title (+70 = 0..9: its place in the table 0A7FAE, +72 the updates of its flight). */
    case 0xa81a4: {
        ga_palette(a,0x134);ga_palette(a,0x145);                             /* slots of its own */
        slot_blend(m,a,B(0x14),0x159,0x1f);
        START(0x2a702c);
        uint32_t tab=0xa7faeu+4u*B(0x70);
        SW(0x28,0x100u-rw(tab));SW(0x2c,0x2e0u-rw(tab+2));
        SL(0,0xa821a);
        return GA_MORE;
    }
    case 0xa821a: {
        uint32_t tab=0xa7faeu+4u*B(0x70);
        SW(0x74,W(0x74)+1u);
        if (!W(0x72)) STOP(0xa8226);
        slot_blend(m,a,B(0x14),0x159,(uint16_t)(0x1fu-(uint32_t)W(0x74)*0x1fu/W(0x72)));
        uint16_t d0=W(0x72),d1=W(0x74),d7=0x300;
        bool arrived=d1>=d0;
        if (!arrived) {
            d1=(uint16_t)(d1*2u+d0-3u);
            if (S16(d1)>0) {
                uint32_t n=(uint32_t)(uint16_t)(d0-1u)<<8,q=n/d1,q7=3u*n/d1;
                if (q>0xffffu || q7>0xffffu) STOP(0xa8266);                  /* divu overflow: not followed */
                if (!q) arrived=true;
                else {SB(0x12,q-1u);SB(0x13,q-1u);d7=(uint16_t)q7;}
            }
        }
        if (arrived) {                                                       /* 0A8340 */
            if (B(0x70)==9) {sound(m,0x10e5);creator60(a,e,m,false);}
            else {
                sound(m,0x10e1);
                for (unsigned i=0;i<0x18;i++) if (!spawn(a,e,0xa83d6)) STOP(0xa837a);
            }
            START(0x2a7082);
            SW(0x3c,rw(tab));SW(0x40,rw(tab+2));SB(0x12,0xff);SB(0x13,0xff);SL(0,0xa83c2);
            STEP();
            return GA_MORE;
        }
        int32_t dx=(int32_t)(((uint32_t)rw(tab)<<16)-L(0x28)),dy=(int32_t)(((uint32_t)rw(tab+2)<<16)-L(0x2c));
        uint16_t d4=(uint16_t)((uint16_t)(W(0x72)-W(0x74))>>1),d5=(uint16_t)(W(0x74)>>1);
        if (d4>=4) d4=4;
        if (d5<3) d5=3;
        SL(0x28,L(0x28)+(uint32_t)(asr(dx,d4)-asr(dy,d5)));
        SL(0x2c,L(0x2c)+(uint32_t)(asr(dy,d4)+asr(dx,d5)));
        SW(0x3c,(uint16_t)(((int32_t)S16(W(0x28)-0xa0u)*S16(d7))>>8)+0xa0u);
        SW(0x40,(uint16_t)(((int32_t)S16(W(0x2c)-0x110u)*S16(d7))>>8)+0x110u);
        uint16_t d=(uint16_t)(W(0x3c)-0xa0u);
        if (!(d&0x8000u)) d=(uint16_t)(0u-d);
        d=(uint16_t)((d>>2)+0x8000u-W(0x40));
        uint16_t r=(uint16_t)(W(0x72)-W(0x74));
        if (r) r=(uint16_t)(r+0x10u);
        SW(0x1e,d+(uint16_t)(r<<8));
        if (W(0x40)<0x280u) STEP();
        return GA_MORE;
    }
    case 0xa83c2:
        STEP();
        return GA_MORE;
    /* A spark of an arrived piece: two trails (+6E.., 16 bytes each: x, y and their speeds) drawn with one program. */
    case 0xa83d6: {
        uint32_t d0=(uint32_t)rnd(m)*3u;d0=d0<<16|d0>>16;                     /* 0029F2(3): the high word stays in D0 */
        START(rl(0xa83cau+4u*(uint16_t)d0));
        ga_palette(a,0x134);
        SW(0x26,(rnd(m)&0xffu)+0x80u);
        uint16_t f=rnd(m);
        if (rnd_negative(m)) SB(0x15,B(0x15)|1u);
        if (f&0x80u) SB(0x15,B(0x15)|2u);
        neo_gactor_t *c=e->creator(e->ctx,a);
        /* The high words of D1, D2 and D3 as this routine is entered: FFFF, 0 and 0 in every one of the capture's
         * 216 sparks (what the node before it in the list left; not derived). */
        uint32_t d1=0xffff0000u,d2=0,d3=0;
        for (unsigned i=0;i<2;i++) {
            d1=(d1&0xffff0000u)|rnd(m);
            d0=(d0&0xffff0000u)|(uint16_t)((rnd(m)&0x1fu)-0xfu);
            d1=(d1&0xffff0000u)|(uint16_t)(((uint16_t)d1&0x3fu)-0x1fu);
            d2=(d2&0xffff0000u)|(uint16_t)((c?ga_w(c,0x3c):0u)+(uint16_t)d0);
            d3=(d3&0xffff0000u)|(uint16_t)((c?ga_w(c,0x40):0u)+(uint16_t)d1);
            d2=d2<<16|d2>>16;d3=d3<<16|d3>>16;
            SL(0x6eu+16u*i,d2);SL(0x72+16*i,d3);
            d0=d0<<16|d0>>16;d1=d1<<16|d1>>16;
            d0=(uint32_t)((int32_t)d0>>2);d1=(uint32_t)((int32_t)d1>>2);
            SL(0x76+16*i,d0);SL(0x7a+16*i,d1);
        }
        SW(0x1e,(rnd(m)&0xffu)+1u);SL(0,0xa8498);
    }   /* fall through */
    case 0xa8498: {
        uint16_t keep=W(0x26);
        for (unsigned i=0;i<2;i++) {
            SL(0x28,L(0x6eu+16u*i));SL(0x2c,L(0x72+16*i));SL(0x30,L(0x76+16*i));SL(0x34,L(0x7a+16*i));
            ga_move(a,e);
            int r=ga_anim_step(a,e,NULL);
            if (r<0) return GA_UNSUPPORTED;
            if (r>0) SB(0x5f,B(0x5f)|0x80u);
            SW(0x26,0);
            SL(0x6eu+16u*i,L(0x28));SL(0x72+16*i,L(0x2c));
        }
        SW(0x26,keep);
        return (B(0x5f)&0x80u)?GA_DELETE:GA_MORE;
    }
    case 0xa8602:
        ga_palette(a,0x840);SW(0x1e,0);SB(0x15,B(0x15)|4u);SL(0,0xa8622);
        /* fall through */
    case 0xa8622:
        SW(0x3c,0xa0);SW(0x40,0x160);
        ga_draw(a,e,0x1d5b22);                                               /* 011FF6 */
        return GA_MORE;
#endif
    }
    STOP(L(0));
}
ga_result_t neo_ending_tick(neo_gactor_t *a,neo_genv_t *e) {return ending_tick(e->end_list,a,e,e->end_ram);}
