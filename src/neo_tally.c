/* Prisoner tally of the mission end 0308C0 (docs/FIN_PARTIDA.md), manual
 * translation of ROM 030106..0311D8 (labels are provenance only). Created by
 * the mission actor at 01889A (0040BC, A6 = 101364), it runs in the
 * mission-end list of neo_end.c.
 *
 * Root 0308C0: audio command 01228C(0x60) (counted: the port's music is
 * already stopped), fade 009C0A(-8,-8,-8,5), waits 16 updates (0308E4),
 * +90 = the region byte (RAM 100000 != 0: English), RECAPTURED PRISONER at
 * column 0x0A row 6 (palette 4, two-row font), one counter 030B0A per player
 * present (01AEB0; an absent player's count is cleared), +60 = 1. 0309CC:
 * each update a counter adds 1 to the root's +60; with +60 set, A or B
 * newly pressed (RAM 10C35D/10C363 & 0x30) sets +61 = FF (the counters
 * skip their waits); with +60 clear (every counter gone) 030A34 clears the
 * 34x22 window from 7066 (030284), fades back (0,0,0,5), waits 16 updates
 * and sets the mission actor's +60 (0188B0 goes on).
 *
 * Counter 030B0A (player +80): the big prisoner (+3C/+40 = 30/190, palette
 * 0x381, program 237AEC), the box 0301AE and the rule 0302C2, "1P" (2x2
 * font), the count +82 = RAM 10C5FC (+86 a copy), the special prisoners +84
 * (RAM 10C5FE: 09AB38's tied prisoner, +8F bit 0; 10C5FF: the +74 = FFFE
 * prisoner, bit 1), the rescued digits +8C (0302C2/03035E), the result bits
 * of +8F (bit 7 a message; single player: 10 or more prisoners = GREAT!,
 * bit 4; none: NO PRISONER, bits 6/7), a holder node 030B08 (+6E: its
 * +6E.. keep the seven visible name/rank pointers), "x10000" (030374) and
 * the first parade icon (030608). 030D20: each icon that lands (+60 = FF)
 * counts a prisoner (0306D0: a random name of the 989 of 02CAD6 and rank of
 * 02C982/02C9F8 (0029F2), or the special ones of 030825..03085A, scrolled
 * list of seven rows 0304D6/030548, count and RAM 10C5FC minus 1, +8C BCD
 * plus 1 (030300)); +10000 per prisoner (03040E: +72 BCD and the score
 * 057948), +30000 for a special one (0303AA, the big prisoner becomes
 * 237BD0/0x3FA or 237B84/0x9B for 0x3C/0x3D updates); the next icon after
 * +7E updates (0xF). Then 030DBA, the result message blinking with video
 * bit 3 every 8 frames for 0x5A updates (030EA8; GREAT! adds 100000), 60
 * updates (030F76) and the node is deleted.
 *
 * Parade icon 030FB2: placed at (min(+82,14)*8 + 0x1C, 0x11C) on the screen,
 * flies toward (0x60, 0x7C) at speed 0x1000 (002EB4 angle, sine table
 * 0029FC), palette 0x2A2, program 237C48; at y >= 0x198 (or with the skip)
 * sound 0x10D6, the counter's +60 = FF and it is deleted.
 *
 * Explicit stops (never in the port, no capture): the Japanese texts
 * (+90 = 0), no player in play (03096C), player 2's counter and the
 * two-player result (DRAW!), the
 * medals of 018FEA (RAM 10C306/10C307: 03112E, 030460 and the bonus table
 * 2D0A74). Not reproduced: the cartridge checks 030A78/03109E (they only act
 * on a failed check) and the audio command 01228C. */
#include "neo_end.h"
#include <string.h>
#include "neo_soldier_ai.h"   /* neo_angle (ROM 002EB4) */
#include "neo_region.h"       /* neo_item_add_score (ROM 057948) */
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
bool neo_tally_routine(uint32_t r) {return r>=0x308c0 && r<=0x311d8;}
static uint16_t rw(uint32_t address) {const uint8_t *p=ga_rom(address);return p?(uint16_t)(p[0]<<8|p[1]):0;}
static void sound(neo_end_ram_t *m,uint16_t id) {if (m->sound_count<NEO_END_SOUNDS) m->sounds[m->sound_count++]=id; else m->sounds_dropped++;}
static void fade(neo_end_ram_t *m,int16_t d0,int16_t d1,int16_t d2,uint16_t d3) {if (m->fade) neo_fade_start(m->fade,d0,d1,d2,d3);}
static neo_text_t text_of(neo_end_ram_t *m) {return (neo_text_t){m->hud,m->on_write,m->write_ctx,0};}
/* Text failures stop the node. */
static bool text_done(neo_genv_t *e,neo_text_t *t,uint32_t at) {
    if (t->unsupported) {ga_fail(e,at);return false;}
    return true;
}
/* 030190: d2 > 0 blanks (" " 03089C) through 03D25A. */
static uint16_t pad(neo_text_t *t,uint16_t d0,uint16_t *d1,int16_t d2) {
    const uint8_t *blank=ga_rom(0x3089c);
    for (;d2>0;d2--) d0=neo_text_string(t,d0,d1,blank);
    return d0;
}
/* 030162: the string a0 at column +28, row +2C with d1; d2 > 0 blanks
 * before, d2 < 0 blanks after. */
static bool print(neo_gactor_t *a,neo_genv_t *e,neo_end_ram_t *m,uint32_t a0,uint16_t d1,int16_t d2) {
    neo_text_t t=text_of(m);
    uint16_t d0=(uint16_t)((uint16_t)(W(0x28)<<5)+W(0x2c)+0x7000u);
    d0=pad(&t,d0,&d1,d2);
    d0=neo_text_string(&t,d0,&d1,ga_rom(a0));
    pad(&t,d0,&d1,(int16_t)-d2);
    m->unsupported_text+=t.unsupported;
    return text_done(e,&t,0x30162);
}
/* 03CFBA with 03DD96 from a node field. */
static bool digits(neo_gactor_t *a,neo_genv_t *e,neo_end_ram_t *m,uint16_t d0,unsigned field,uint8_t d3,unsigned d4) {
    neo_text_t t=text_of(m);
    neo_text_bcd(&t,d0,0x541,&a->m[field],d3,d4);
    m->unsupported_text+=t.unsupported;
    return text_done(e,&t,0x3cfba);
}
/* 0301AE: box of d6 columns x d7 rows (S-ROM tiles 0BF0..0BF7, inside 0B60). */
static void box(neo_end_ram_t *m,uint16_t a,uint8_t d6,uint8_t d7) {
    neo_text_t t=text_of(m);
    uint32_t d0=(uint32_t)a<<16|0x0b00u,d3=d0;uint8_t d5=d6;
    d0=(d0&~0xffu)|0xf4u;
    neo_text_word(&t,(uint16_t)(d0>>16),(uint16_t)d0);d0+=0x200000u;
    d0=(d0&~0xffu)|0xf0u;
    for (;d6>2;d6--) {neo_text_word(&t,(uint16_t)(d0>>16),(uint16_t)d0);d0+=0x200000u;}
    d0=(d0&~0xffu)|0xf5u;neo_text_word(&t,(uint16_t)(d0>>16),(uint16_t)d0);
    for (;d7>2;d7--) {
        d3+=0x10000u;d0=(d3&~0xffu)|0xf2u;neo_text_word(&t,(uint16_t)(d0>>16),(uint16_t)d0);d0+=0x200000u;
        d0=(d0&~0xffu)|0x60u;
        for (d6=d5;d6>2;d6--) {neo_text_word(&t,(uint16_t)(d0>>16),(uint16_t)d0);d0+=0x200000u;}
        d0=(d0&~0xffu)|0xf3u;neo_text_word(&t,(uint16_t)(d0>>16),(uint16_t)d0);
    }
    d3+=0x10000u;d0=(d3&~0xffu)|0xf6u;neo_text_word(&t,(uint16_t)(d0>>16),(uint16_t)d0);d0+=0x200000u;
    d0=(d0&~0xffu)|0xf1u;
    for (d6=d5;d6>2;d6--) {neo_text_word(&t,(uint16_t)(d0>>16),(uint16_t)d0);d0+=0x200000u;}
    d0=(d0&~0xffu)|0xf7u;neo_text_word(&t,(uint16_t)(d0>>16),(uint16_t)d0);
    m->unsupported_text+=t.unsupported;
}
/* 030284: d6 columns x d7 rows of 0820 from a (column by column). */
static void clear(neo_end_ram_t *m,uint16_t a,uint8_t d6,uint8_t d7) {
    neo_text_t t=text_of(m);
    uint32_t d3=(uint32_t)a<<16|0x0800u,d0=d3;
    for (;d7;d7--) {
        d0=(d0&~0xffu)|0x20u;
        for (uint8_t c=d6;c;c--) {neo_text_word(&t,(uint16_t)(d0>>16),(uint16_t)d0);d0+=0x200000u;}
        d3+=0x10000u;d0=d3;
    }
    m->unsupported_text+=t.unsupported;
}
/* 0302C2: the rule 0BF8, d6-2 x 0BF9, 0BFA. */
static void rule(neo_end_ram_t *m,uint16_t a,uint8_t d6) {
    neo_text_t t=text_of(m);
    uint32_t d0=(uint32_t)a<<16|0x0bf8u;
    neo_text_word(&t,(uint16_t)(d0>>16),(uint16_t)d0);d0+=0x200000u;
    d0=(d0&~0xffu)|0xf9u;
    for (;d6>2;d6--) {neo_text_word(&t,(uint16_t)(d0>>16),(uint16_t)d0);d0+=0x200000u;}
    d0=(d0&~0xffu)|0xfau;neo_text_word(&t,(uint16_t)(d0>>16),(uint16_t)d0);
    m->unsupported_text+=t.unsupported;
}
static unsigned player(const neo_gactor_t *a) {return ga_w(a,0x80)&1u;}
/* 030300 / 03032C / 03035E: +8C (BCD as the original adds it) at 712C or 714C. */
static bool count_digits(neo_gactor_t *a,neo_genv_t *e,neo_end_ram_t *m,bool add,uint16_t at) {
    if (add) {
        uint8_t d0=B(0x8c),d1=(uint8_t)(d0&15u);
        if (d1==9) {d0&=0xf0u;if (d1!=0x90) d0=(uint8_t)(d0+0x10u);}   /* cmpi.b #$90,d1 (sic) */
        else d0++;
        SB(0x8c,d0);
    }
    return digits(a,e,m,at,0x8c,0xff,2);
}
/* 0303DE: the bonus +73..+75 (6 BCD digits, leading blanks) at 70EF/730F. */
static bool bonus_digits(neo_gactor_t *a,neo_genv_t *e,neo_end_ram_t *m) {
    return digits(a,e,m,player(a)?0x730f:0x70ef,0x73,0,6);
}
/* 030440: 057948 with the four BCD bytes +76..+79 for player +80. */
static void add_score(neo_gactor_t *a,neo_end_ram_t *m) {
    neo_item_add_score(m->score[player(a)],L(0x76));
}
/* 03040E: +72 plus 10000 (BCD carry by hand) and the score. */
static void plus10000(neo_gactor_t *a,neo_end_ram_t *m) {
    uint32_t d0=L(0x72);
    if ((d0&0xf0000u)==0x90000u) d0+=0x70000u; else d0+=0x10000u;
    SL(0x72,d0);SL(0x76,0x10000u);add_score(a,m);
}
/* 0303AA / 0303D8: "+30000" or blanks at column 7 (0x18) row 0x0C. */
static bool special_text(neo_gactor_t *a,neo_genv_t *e,neo_end_ram_t *m,uint32_t s) {
    SW(0x28,player(a)?0x18:7);SW(0x2c,0x0c);
    return print(a,e,m,s,0x500,0);
}
/* 03012C: name length (an '@' and the next byte, DE and DF not counted). */
static uint8_t length(uint32_t address) {
    const uint8_t *s=ga_rom(address);uint16_t d0=0;
    if (!s) return 0;
    for (;;) {
        uint8_t c=*s++;
        if (!c) return (uint8_t)d0;
        if (c=='@') {s++;continue;}
        if (c==0xde || c==0xdf) continue;
        d0++;
    }
}
/* 03010A: skip d0 strings after the count word (d0 > count: unchanged). */
static uint32_t pick(uint32_t list,uint16_t d0) {
    uint16_t d7=rw(list);list+=2;
    if (S16(d0)>S16(d7)) return list;
    for (;d0;d0--) {const uint8_t *s=ga_rom(list);if (!s) return list;while (*s++) list++;list++;}
    return list;
}
static uint16_t rand_range(neo_end_ram_t *m,uint16_t range) {
    uint16_t r=m->rand?m->rand(m->rand_ctx):0;
    return (uint16_t)(((uint32_t)r*range)>>16);
}
/* 0304D6: the name/rank pair into the holder's ring of seven, its length
 * into +91.., the ring start +88 and size +8A. */
static void remember(neo_gactor_t *a,neo_gactor_t *holder,uint32_t name,uint32_t rank) {
    uint16_t d4=(uint16_t)(W(0x88)+W(0x8a));
    if (S16(d4)>=7) d4=(uint16_t)(d4-7);
    ga_sl(holder,0x6e + 8u*d4,name);ga_sl(holder,0x72+8u*d4,rank);
    SB(0x91+d4,length(name));
    if (S16(W(0x8a))<7) SW(0x8a,W(0x8a)+1u);
    else {SW(0x88,W(0x88)+1u);if (S16(W(0x88))>=7) SW(0x88,0);}
}
/* 030548: the visible rows (rank, then the name padded to 11 columns) from
 * row 0x14 at column 4 (0x15). */
static bool list_rows(neo_gactor_t *a,neo_genv_t *e,neo_end_ram_t *m,neo_gactor_t *holder) {
    SW(0x28,player(a)?0x15:4);SW(0x2c,0x14);
    uint16_t n=W(0x8a),d1=0,column=W(0x28);
    do {
        uint16_t d0=(uint16_t)(d1+W(0x88));
        if (S16(d0)>=7) d0=(uint16_t)(d0-7);
        uint8_t d6=B(0x91+d0);
        int16_t d2=(int16_t)(10-d6);
        if (!print(a,e,m,ga_l(holder,0x72+8u*d0),0x6b,0)) return false;
        SW(0x28,W(0x28)+4u);
        if (!print(a,e,m,ga_l(holder,0x6e + 8u*d0),0x68,(int16_t)-(d2+1))) return false;
        SW(0x28,column);SW(0x2c,W(0x2c)+1u);d1++;
    } while (S16(--n)>0);
    return true;
}
/* 030608: the next parade icon; a special prisoner shows its own figure. */
static ga_result_t next_icon(neo_gactor_t *a,neo_genv_t *e) {
    SB(0x60,0);SW(0x7e,0x0f);
    if (!W(0x82)) return GA_MORE;
    neo_gactor_t *c=e->spawn(e->ctx,a,0x30fb2,false);
    if (!c) STOP(0x40bc);
    ga_sw(c,0x80,W(0x80));ga_sw(c,0x82,W(0x82));
    uint16_t d0=W(0x82),d1=W(0x84);
    if (S16(d0)>S16(d1)) {SW(0x7e,0x0f);return GA_MORE;}
    uint16_t d2=0x3c;
    if (d1==2) {if (d0!=2) d2=0x3d;}
    else if (B(0x8f)&1u) d2=0x3d;
    SW(0x7e,d2);
    SB(0x1d,0);                                                      /* 009F88 */
    if (W(0x7e)==0x3c) {ga_palette(a,0x3fa);START(0x237bd0);}
    else {ga_palette(a,0x9b);START(0x237b84);}
    return GA_MORE;
}
/* 0306D0: one prisoner counted. */
static bool count_one(neo_gactor_t *a,neo_genv_t *e,neo_end_ram_t *m,neo_gactor_t *holder) {
    uint16_t d0=W(0x82),d1=W(0x84);uint32_t name,rank;
    if (S16(d0)>S16(d1)) {
        name=pick(0x2cad6,rand_range(m,rw(0x2cad6)));                         /* 030152 */
        const uint8_t *index=ga_rom(0x2c9f8u+rand_range(m,0x40));
        rank=pick(0x2c982,(uint16_t)(index?*index&0x7fu:0));
    } else {
        unsigned d2=0;
        if (d1==2) {if (d0!=2) d2=1;}
        else if (B(0x8f)&1u) d2=1;
        name=d2?0x3083e:0x30834;rank=d2?0x3085a:0x30855;
    }
    remember(a,holder,name,rank);
    if (!list_rows(a,e,m,holder)) return false;
    SW(0x82,W(0x82)-1u);
    m->rescued[player(a)]--;
    return count_digits(a,e,m,true,player(a)?0x734c:0x712c);
}
/* +6E: the holder node (native tag NEO_GA_TAG(pool index)). */
static neo_gactor_t *holder_of(neo_end_t *s,neo_gactor_t *a) {
    uint32_t tag=L(0x6e);
    if ((tag&0xff000000u)!=NEO_GA_TAG_BASE || (tag&0xffffu)>=NEO_END_NODES) return NULL;
    return &s->nodes[tag&0xffffu];
}
/* 030FA2: the root's +60 plus 1 and 00427A. */
static ga_result_t tail(neo_gactor_t *a,neo_genv_t *e,neo_gactor_t *p) {
    if (p) ga_sb(p,0x60,(uint8_t)(ga_b(p,0x60)+1u));
    STEP();
    return GA_MORE;
}
/* The skip of the root: +61 copied, a wait of one update. */
static void skip(neo_gactor_t *a,neo_gactor_t *p) {
    SB(0x61,p?ga_b(p,0x61):0);
    if (B(0x61)) SW(0x7e,1);
    SW(0x7e,W(0x7e)-1u);
}
ga_result_t neo_tally_tick(neo_end_t *s,neo_gactor_t *a,neo_genv_t *e,neo_end_ram_t *m) {
    neo_gactor_t *p=e->creator(e->ctx,a);
    if (!m) STOP(L(0));
    switch (L(0)) {
    /* ---- root ---- */
    case 0x308c0:
        m->audio_commands++;                                        /* 01228C(0x60) */
        fade(m,-8,-8,-8,5);
        SW(0x7e,0x0f);SL(0,0x308e4);
        /* fall through */
    case 0x308e4: {
        uint16_t old=W(0x7e);SW(0x7e,old-1u);
        if (old) return GA_MORE;
        SB(0x90,m->region?1:0);
        if (!B(0x90)) STOP(0x30906);                                  /* Japanese text */
        SW(0x28,0x0a);SW(0x2c,6);
        if (!print(a,e,m,0x30800,0x400,0)) return GA_UNSUPPORTED;
        if (m->players&2u) STOP(0x30988);                             /* player 2's counter */
        if (m->players&1u) {
            neo_gactor_t *c=e->spawn(e->ctx,a,0x30b0a,false);
            if (!c) STOP(0x40bc);
            ga_sw(c,0x80,0);ga_sb(c,0x90,B(0x90));SW(0x80,W(0x80)|1u);
            m->rescued[1]=0;                                          /* 0309BA */
        } else STOP(0x3096c);                                         /* no player in play */
        SB(0x60,1);SL(0,0x309cc);
    }
        /* fall through */
    case 0x309cc:
        if (W(0x82)==W(0x80)) SW(0x86,1);
        if (B(0x60)) {
            SB(0x60,0);
            if (!B(0x61)) {
                if ((W(0x80)&1u) && (m->pressed[0]&0x30u)) SB(0x61,0xff);
                if ((W(0x80)&2u) && (m->pressed[1]&0x30u)) SB(0x61,0xff);
            }
            return GA_MORE;
        }
        clear(m,0x7066,0x22,0x16);
        fade(m,0,0,0,5);
        SW(0x7e,0x0f);SL(0,0x30a5e);
        /* fall through */
    case 0x30a5e: {
        STEP();
        uint16_t old=W(0x7e);SW(0x7e,old-1u);
        if (old) return GA_MORE;
        if (p) ga_sb(p,0x60,1); else m->mission60=1;
        return GA_DELETE;                                             /* 030B00 */
    }
    /* ---- counter ---- */
    case 0x30b0a: {
        if (player(a)) STOP(0x30b1a);
        SW(0x3c,0x30);SW(0x40,0x190);
        box(m,0x7068,0x11,0x14);
        rule(m,0x7093,0x0f);
        SW(0x28,0x0a);SW(0x2c,9);
        if (!print(a,e,m,0x30890,0x541,0)) return GA_UNSUPPORTED;
        uint16_t d0=m->rescued[0];uint8_t d1=m->special[0][0],d2=m->special[0][1];
        SW(0x82,d0);SW(0x86,d0);
        if (d1) {SB(0x8f,B(0x8f)|1u);SW(0x84,W(0x84)+1u);}
        if (d2) {SB(0x8f,B(0x8f)|2u);SW(0x84,W(0x84)+1u);}
        if (!count_digits(a,e,m,false,W(0x82)?0x712c:0x714c)) return GA_UNSUPPORTED;
        if (!W(0x82)) SB(0x8f,B(0x8f)|0xc0u);
        else {
            if ((m->players&3u)==3u) STOP(0x30c66);                   /* two players: DRAW! etc. */
            if (S16(W(0x82))>=10) SB(0x8f,B(0x8f)|0x90u);
        }
        if (m->medal[0] || m->medal[1]) STOP(0x30cae);                /* medals 018FEA */
        neo_gactor_t *h=e->spawn(e->ctx,a,0x30b08,false);
        if (!h) STOP(0x40bc);
        SL(0x6e,NEO_GA_TAG((unsigned)h->index));
        ga_palette(a,0x381);START(0x237aec);
        if (W(0x82)) {
            SW(0x28,0x0d);SW(0x2c,0x0d);
            if (!print(a,e,m,0x3089e,0x100,0)) return GA_UNSUPPORTED;  /* 030374 */
        }
        ga_result_t r=next_icon(a,e);
        if (r!=GA_MORE) return r;
        SB(0x60,0);SL(0,0x30d20);
    }
        /* fall through */
    case 0x30d20: {
        skip(a,p);
        uint8_t d0=B(0x60);
        if (d0) {
            if (d0==0xff) {
                SB(0x60,1);
                neo_gactor_t *h=holder_of(s,a);
                if (!h) STOP(0x304d6);
                if (!count_one(a,e,m,h)) return GA_UNSUPPORTED;
                if (S16(W(0x82))<S16(W(0x84))) {
                    if (!special_text(a,e,m,0x308a8)) return GA_UNSUPPORTED;
                    plus10000(a,m);plus10000(a,m);
                }
                plus10000(a,m);
                if (!bonus_digits(a,e,m)) return GA_UNSUPPORTED;
            } else if (S16(W(0x7e))<=0) {
                ga_result_t r=next_icon(a,e);
                if (r!=GA_MORE) return r;
            }
        }
        if (S16(W(0x7e))<=0) {
            SW(0x7e,0);
            if (!(m->rescued[0]|m->rescued[1])) {
                if (W(0x86) && !special_text(a,e,m,0x308b2)) return GA_UNSUPPORTED;
                SL(0,0x30dba);
            }
        }
        return tail(a,e,p);
    }
    case 0x30dba:
        if (!count_digits(a,e,m,false,0x714c)) return GA_UNSUPPORTED;
        if (B(0x8f)&4u) STOP(0x30dc8);                                 /* medal 03112E */
        SW(0x7e,0x49);SL(0,0x30de2);
        /* fall through */
    case 0x30de2:
        skip(a,p);
        if (S16(W(0x7e))<=0) {
            if (B(0x8f)&4u) STOP(0x30e0e);                             /* medal bonus 030460 */
            SL(0,0x30e3a);
        }
        if (!(B(0x8f)&8u)) SL(0,0x30e3a);
        return tail(a,e,p);
    case 0x30e3a: {
        uint32_t s=0;
        if (B(0x8f)&0x10u) {sound(m,0x1150);s=0x30867;}
        else if (B(0x8f)&0x20u) s=0x3086f;
        else if (B(0x8f)&0x40u) s=0x3087e;
        SL(0x7a,s);SW(0x7e,0x5a);
        SW(0x28,player(a)?0x15:4);SW(0x2c,0x11);SL(0,0x30ea8);
    }
        /* fall through */
    case 0x30ea8:
        if (L(0x7a) && !(m->video&7u))
            if (!print(a,e,m,(m->video&8u)?0x3085f:L(0x7a),0x541,0)) return GA_UNSUPPORTED;
        skip(a,p);
        if (S16(W(0x7e))<=0) {
            if (B(0x8f)&0x10u) {
                SL(0x72,L(0x72)+0x100000u);sound(m,0x10d8);
                if (!bonus_digits(a,e,m)) return GA_UNSUPPORTED;
                SL(0x76,0x100000u);add_score(a,m);
            }
            if (L(0x7a) && !print(a,e,m,L(0x7a),0x541,0)) return GA_UNSUPPORTED;
            SL(0,0x30f76);
        }
        if (!(B(0x8f)&0x80u)) SL(0,0x30f76);
        return tail(a,e,p);
    case 0x30f76:
        SW(0x7e,0x3c);SL(0,0x30f82);
        /* fall through */
    case 0x30f82:
        SW(0x7e,W(0x7e)-1u);
        if (S16(W(0x7e))<=0) SL(0,0x30f9a);
        return tail(a,e,p);
    case 0x30f9a:
        return GA_DELETE;
    case 0x30b08:
        return GA_MORE;                                               /* holder: rts */
    /* ---- parade icon ---- */
    case 0x30fb2: {
        if (player(a)) STOP(0x30fde);
        ga_project(a,e);
        uint16_t d1=W(0x82);
        if (S16(d1)>0x0e) d1=0x0e;
        d1=(uint16_t)(d1<<3);d1=(uint16_t)(d1+0x1c);
        uint16_t d0=0x60,d2=d1,d3=0x11c;
        d2=(uint16_t)(d2-W(0x3c));SW(0x28,W(0x28)+d2);
        d3=(uint16_t)(d3-W(0x40));SW(0x2c,W(0x2c)+d3);
        d0=(uint16_t)(d0-d1);
        uint8_t angle=neo_angle(S16(d0),0x7c);
        SL(0x30,(uint32_t)((int32_t)ga_cosine(angle)*0x1000));
        SL(0x34,(uint32_t)((int32_t)ga_sine(angle)*0x1000));
        ga_project(a,e);
        ga_palette(a,0x2a2);START(0x237c48);
        SL(0,0x31060);
    }
        /* fall through */
    case 0x31060:
        if (!(p && ga_b(p,0x61))) {
            ga_move(a,e);STEP();
            if (S16(W(0x40))>=0x198) SL(0,0x3108a);
            return GA_MORE;
        }
        SL(0,0x3108a);
        /* fall through */
    case 0x3108a:
        sound(m,0x10d6);
        if (p) ga_sb(p,0x60,0xff);
        return GA_DELETE;                                             /* 031126 */
    }
    STOP(L(0));
}
