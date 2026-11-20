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
 * Not reproduced: the cartridge checks 0571FC/0575BC/0576EE (they only act
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
bool neo_ending_routine(uint32_t r) {return r>=0x56fcc && r<=0x585fa;}
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
/* ---- the update ---------------------------------------------------------- */
ga_result_t neo_ending_tick(struct neo_end *s,neo_gactor_t *a,neo_genv_t *e,neo_end_ram_t *m) {
    (void)s;
    if (!m) STOP(L(0));
    switch (L(0)) {
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
        m->audio_commands++;                                                /* 01228C(0x80) */
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
        return neo_ending_tick(s,a,e,m);   /* 057426 -> 05742C in the same update */
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
    }
    STOP(L(0));
}
