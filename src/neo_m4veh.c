#include "neo_region.h"
#include "neo_target.h"
/* Ground vehicle of Mission 4 (script record of camera 4713, entry 0587C4),
 * manual translation of 05879A..059B9B; ROM labels are provenance only.
 *
 * Entries: 05879A (+78 = 0), 0587C4 (1), 0587D8 (3), 058806 (2, +8F = FF: no
 * turret). Record words: +6E (0/1: the wreck keeps its cell map 058656 /
 * 058672), +70 the x it brakes at (<= 0x200), +72 (0/1: the body rams with
 * the row 05871A while it drives), +74 / +76 the x range of kinds 2 / 3; the
 * constructor traps on other values ("NOT251 Option Error").
 * Body: health rank 2D105E, hurtbox 05868E, solid body 0586A6 (004E76),
 * palette 1C, flash 30B, the turret 0592C6 (0040BC, D0 = E) and the ripple
 * 03C532. State +80 picks the program of the table +7C (058752 body, 058776
 * turret; 059C2C): 0 drive (0589D2: +62 eases by +82 up to rank 2D1122 << 8),
 * 1 brake (058C66), 2 wait (058CE4), 3 / 8 turn the gun out / in (058DD4 /
 * 058E0A), 4 aim (058E48, +86 updates), 5 muzzle 059A3E / 059A62 (058F4A),
 * 6 shot 0594B0 / 0594CE (058FF2: rank 2D124C updates each, rank 2D11B8 shots,
 * then rank 2D11EA updates of state 4), 7 destroyed (059048 / 0590B8: the
 * piece 059B5C, 019DFC, sound 1021; at the program's end with +6E = 0 sound
 * 102F, 01DBAA, another piece, deleted). Every update 059214: 0143DC / 01456A,
 * deleted outside 0586C2, the hurtbox (a hit starts the flinch 05872E[+80],
 * resumed when it ends), the program (+8D = FF at its end); 05929A: +5F bit 7
 * (except while braking) -> 059048.
 * Turret 0592C6: at its creator (01AD6E), health 0x32, hurtbox 05869A, solid
 * 0586B4; follows the creator's facing, state, depth + 1 and flash (0044F6);
 * when it or its creator dies (0593B6) the creator becomes the kind 2 (+8F =
 * FF: it drives on, 0591FE) and the blast 01C6EE takes its place.
 * Shot 0594B0 / 0594CE (programs 24BFBC / 24C0FC, health rank 2D12AE, speed
 * rank 2D10C0 << 8): the program's call 059770 launches it (+30 = rank 2D1154
 * << 8, +76 = -rank 2D1186 << 8, +72 = +-4000); 0595B2: 0143E4, hurtbox, the
 * trail 05996A every third update (0599F0), homing 0597DE under +7C or 0x14
 * above a player inside 0586DA, the rows 0586E2 / 0586F2 after +7E updates;
 * a hit, the ground or its creator's +8F: sound 1022 and the blast 059852.
 *
 * The copy-protection checks before several deletions are not reproduced
 * (they fall through to 004188 on the original board). The resume point of
 * the flinch (+88 = +20 in the original) is kept natively; +88 holds the
 * interrupted program's tag. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define BIT(o,b) ((B(o)>>(b))&1u)
static int16_t S16(uint16_t v) {return (int16_t)v;}
static int32_t S32(uint32_t v) {return (int32_t)v;}
static bool rom_ok(neo_genv_t *e,uint32_t at) {if (ga_rom(at)) return true;ga_fail(e,at);return false;}
static uint32_t rl(uint32_t at) {const uint8_t *p=ga_rom(at);return p?((uint32_t)p[0]<<24|(uint32_t)p[1]<<16|(uint32_t)p[2]<<8|p[3]):0;}
static uint16_t rank(neo_gactor_t *a,neo_genv_t *e,uint32_t table) {
    const uint8_t *t=ga_rom(table);
    if (!t) {ga_fail(e,table);return 0;}
    return ga_rank_word(a,e,t);
}
static void sound(neo_genv_t *e,uint16_t id) {if (e->sound) e->sound(e->ctx,id);}
static neo_gactor_t *creator(neo_gactor_t *a,neo_genv_t *e) {return e->creator?e->creator(e->ctx,a):NULL;}
static neo_gactor_t *child(neo_gactor_t *a,neo_genv_t *e,uint32_t routine,uint16_t reserve) {   /* 0040BC */
    return e->spawn_reserve?e->spawn_reserve(e->ctx,a,routine,reserve):e->spawn?e->spawn(e->ctx,a,routine,false):NULL;
}
static neo_gactor_t *after_script(neo_gactor_t *a,neo_genv_t *e,uint32_t routine,int16_t dx,int16_t dy) {   /* 00411C, 01ACF2 */
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,routine,true):NULL;
    if (c) ga_place_at(c,a,e,dx,dy);
    return c;
}
static void flip(neo_gactor_t *a) {SB(0x15,B(0x15)^1u);}
/* 059C2C: the program of the state +80 in the table +7C. */
static bool k59c2c(neo_gactor_t *a,neo_genv_t *e) {
    SB(0x8c,0);
    uint32_t t=L(0x7c)+4u*W(0x80);
    if (!rom_ok(e,t)) return false;
    return ga_anim_start(a,e,rl(t),0x100);
}
/* 059EFC: a player inside the rectangle (01AFFC): *d0 = 0 when the vehicle
 * stands behind the middle of the screen (0xA0) for its facing, FF if not. */
static bool k59efc(neo_gactor_t *a,neo_genv_t *e,uint32_t rect,neo_target_t *t,uint8_t *d0) {
    const uint8_t *r=ga_rom(rect);
    if (!r) {ga_fail(e,rect);return false;}
    int16_t q[4];for (unsigned i=0;i<4;i++) q[i]=(int16_t)(r[2*i]<<8|r[2*i+1]);
    neo_contact_actor_t c=ga_contact(a);
    *t=neo_target_pick(&c,&e->players,q);
    if (!t->valid) return false;
    int16_t x=S16(W(0x3c));
    if (d0) *d0=!BIT(0x15,0)?(x<0xa0?0:0xff):(x>0xa0?0:0xff);
    return true;
}
/* 059214: the body's step. 1: deleted (004188), -1: explicit stop. */
static int k59214(neo_gactor_t *a,neo_genv_t *e) {
    if (!BIT(0x15,6)) {SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);}      /* 0143DC */
    else ga_ground(a,e,NULL);                                            /* 01456A */
    if (ga_cull(a,e,0x586c2)) return 1;
    if (W(0x72) && W(0x80)==0) ga_attack(a,e,0x5871a);
    if (ga_register(a,e,NULL,NULL) && !B(0x8c)) {                        /* 059C4C */
        uint32_t t=0x5872eu+4u*W(0x80);
        if (!rom_ok(e,t)) return -1;
        if (rl(t)) {
            a->resume_program=a->program;a->resume_cursor=a->cursor;a->resume_tag=a->program_tag;SL(0x88,a->program_tag);
            SB(0x8c,0xff);
            if (!ga_anim_start(a,e,rl(t),0x100)) return -1;
        }
    }
    SB(0x5a,0);
    int r=ga_anim_step(a,e,NULL);
    if (r<0) return -1;
    SB(0x8d,r?0xff:0);
    if (r && B(0x8c)) {                                                  /* 004256 with +88 */
        a->program=a->resume_program;a->program_tag=a->resume_tag;a->cursor=a->resume_cursor;
        SW(0x24,0);SW(0x26,0x100);SB(0x18,0);
        if (!a->program) {ga_fail(e,0x59286);return -1;}
        SB(0x8c,0);
    }
    return 0;
}
/* 05929A */
static void k5929a(neo_gactor_t *a) {
    if (!(B(0x5f)&0x80u) || W(0x80)==1u) return;
    SL(0,0x59048);
    if (W(0x80)<1u) flip(a);
}
/* 0591FE */
static void k591fe(neo_gactor_t *a) {if (B(0x8f)) {flip(a);SL(0,0x5899a);}}
/* The speed +62 eased by +82 up to rank 2D1122 << `shift`. *over: the limit
 * was passed. */
static int32_t ease(neo_gactor_t *a,neo_genv_t *e,unsigned shift,bool *over) {
    int32_t d7=S32(L(0x62)+L(0x82)),d6=(int32_t)((uint32_t)rank(a,e,0x2d1122)<<shift);
    *over=false;
    if (d7<0) {if (d7<-d6) {*over=true;return -d6;}}
    else if (d7>d6) {*over=true;return d6;}
    return d7;
}
static bool shot_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL || routine!=0x59770u) return false;
    SB(0x86,0xff);SB(0x82,0xff);
    SL(0x76,0u-((uint32_t)rank(a,e,0x2d1186)<<8));
    if (!BIT(0x15,0)) {SL(0x72,0xffffc000u);SL(0x30,0u-((uint32_t)rank(a,e,0x2d1154)<<8));}
    else {SL(0x72,0x4000);SL(0x30,(uint32_t)rank(a,e,0x2d1154)<<8);}
    return true;
}
/* 0597DE */
static void k597de(neo_gactor_t *a) {
    if (B(0x83)) return;
    SB(0x83,0xff);SL(0x72,BIT(0x15,0)?0x10000u:0xffff0000u);
}
#define STEP() do { int s_=k59214(a,e); if (s_>0) return GA_DELETE; if (s_<0) return GA_UNSUPPORTED; } while (0)
#define STATE(n) do { SW(0x80,(n)); if (!k59c2c(a,e)) return GA_UNSUPPORTED; } while (0)
#define DONE() return e->unsupported?GA_UNSUPPORTED:GA_MORE
ga_result_t neo_m4veh_tick(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *p,*c;neo_target_t t;uint8_t d0;bool over;int r;
    switch (L(0)) {
    case 0x5879a:
        ga_floor(a,e);SW(0x78,0);SB(0x8f,0);ga_project(a,e);
        if (S16(W(0x3c))<0xa0) SB(0x15,B(0x15)|1u);
        goto construct;
    case 0x587c4:
        ga_floor(a,e);SW(0x78,1);SB(0x8f,0);
        goto construct;
    case 0x587d8:
        ga_floor(a,e);SW(0x78,3);SB(0x8f,0);ga_project(a,e);
        SB(0x92,S16(W(0x3c))>0xa0?0xff:0);
        if (S16(W(0x3c))<0xa0) SB(0x15,B(0x15)|1u);
        goto construct;
    case 0x58806:
        ga_floor(a,e);SW(0x78,2);SB(0x8f,0xff);ga_project(a,e);
        SB(0x92,S16(W(0x70))>0xa0?0xff:0);
        if (S16(W(0x3c))<0xa0) SB(0x15,B(0x15)|1u);
    construct:                                                           /* 058836 */
        if (S16(W(0x6e))>1 || S16(W(0x70))>0x200 || S16(W(0x72))>1) {ga_fail(e,0x58840);return GA_UNSUPPORTED;}   /* TRAP #F */
        SL(0x7c,0x58752);SB(0x90,0);SB(0x93,0);SB(0x91,0);SB(0x94,0);
        SW(0x66,0x8800);SW(0x1e,0x8800);
        SW(0x4e,rank(a,e,0x2d105e));SL(0x50,0x5868e);
        if (e->solid) e->solid(e->ctx,a,0x586a6);                        /* 004E76 */
        ga_palette(a,0x1c);ga_flash_palette(a,0x30b);
        SL(0x62,0);ga_ground(a,e,NULL);ga_project(a,e);
        if (W(0x78)!=2u) child(a,e,0x592c6,0xe);
        if (W(0x78)==1u) {
            SW(0x86,1);
            if (W(0x74)==W(0x76)) {
                ga_project(a,e);
                if (S16(W(0x3c))<0xa0) SB(0x15,B(0x15)|1u);
                SL(0,0x5899a);
            } else SL(0,0x58cbe);
        } else SL(0,0x5899a);
        if (!neo_gate_ripple(a,e,3)) return GA_UNSUPPORTED;            /* 03C532 */
        DONE();
    case 0x5899a:
        STATE(0);
        SL(0x82,BIT(0x15,0)?0x8000u:0xffff8000u);
        sound(e,0x105e);SL(0,0x589d2);
        /* fall through */
    case 0x589d2: {
        SL(0x62,ease(a,e,8,&over));
        STEP();
        int16_t x=S16(W(0x3c));
        if (W(0x78)==2u) {
            if (!BIT(0x15,0)) {if (x<S16(W(0x74))) SL(0,0x58c2e);}
            else if (x>S16(W(0x76))) SL(0,0x58c2e);
        } else {
            if (!BIT(0x15,0)) {if (x<S16(W(0x70))) SL(0,0x58c2e);}
            else if (x>S16(W(0x70))) SL(0,0x58c2e);
        }
        if (BIT(0x5f,0)) SL(0,0x58c2e);
        k5929a(a);DONE();
    }
    case 0x58aa8:
    k58aa8:
        sound(e,0x105e);STATE(0);
        SL(0x82,BIT(0x15,0)?0x8000u:0xffff8000u);
        SB(0x92,B(0x92)?0:0xff);SB(0x93,0);SL(0,0x58aec);
        /* fall through */
    case 0x58aec: {
        SL(0x62,ease(a,e,8,&over));
        STEP();
        int16_t x=S16(W(0x3c));
        if (S32(L(0x62))<0) {if (x<S16(W(0x74))) SL(0,0x58c2e);}
        else if (x>S16(W(0x76))) SL(0,0x58c2e);
        if (BIT(0x5f,0)) SL(0,0x58c2e);
        k5929a(a);DONE();
    }
    case 0x58b84:
    k58b84:
        sound(e,0x105e);flip(a);STATE(0);
        SL(0x82,BIT(0x15,0)?0x8000u:0xffff8000u);
        SL(0,0x58bc2);
        /* fall through */
    case 0x58bc2: {
        /* 058BEC / 058C16 store the limit and 058C20 the sum over it. */
        int32_t d7=S32(L(0x62)+L(0x82));
        ease(a,e,7,&over);
        if (over) SL(0,0x58c2e);
        SL(0x62,d7);
        STEP();
        k5929a(a);DONE();
    }
    case 0x58c2e:
        STATE(1);
        SL(0x82,BIT(0x15,0)?0xffffb000u:0x5000u);
        sound(e,0x1060);SL(0,0x58c66);
        /* fall through */
    case 0x58c66: {
        int32_t d7=S32(L(0x62)+L(0x82));
        if (BIT(0x15,0)) {if (d7<0) {d7=0;SL(0x82,0);}}
        else if (d7>=0) {d7=0;SL(0x82,0);}
        SL(0x62,d7);
        STEP();
        if (B(0x8d) && !L(0x82)) {SW(0x86,1);SL(0,0x58cbe);}
        k5929a(a);DONE();
    }
    case 0x58cbe:
        STATE(2);
        SL(0x62,0);SW(0x7a,ga_rand_range(e,0)+1u);SL(0,0x58ce4);
        /* fall through */
    case 0x58ce4:
        STEP();
        if (W(0x78)==3u) {
            if (B(0x93)) {flip(a);goto k58aa8;}
            if (k59efc(a,e,0x586ca,&t,&d0)) {
                if (!d0) {flip(a);goto k58aa8;}
                SB(0x93,0xff);goto k58b84;
            }
            if (k59efc(a,e,0x586d2,&t,&d0)) {
                if (!d0) goto k58b84;
                uint16_t v=W(0x86);SW(0x86,v-1u);
                if (!v) goto k58dc0;
            }
        } else if (!B(0x8f)) {
            if (W(0x78)==1u) {if (S16(W(0x3c))<0x130) SL(0,0x58dc0);}
            else {uint16_t v=W(0x86);SW(0x86,v-1u);if (!v) SL(0,0x58dc0);}
        } else if (W(0x78)!=0u && W(0x78)!=1u) {
            uint16_t v=W(0x7a);SW(0x7a,v-1u);
            if (!v) {SW(0x7a,0);flip(a);SL(0,0x5899a);}
        }
        k5929a(a);DONE();
    case 0x58dc0:
    k58dc0:
        STATE(3);
        SW(0x86,0);SL(0,0x58dd4);
        /* fall through */
    case 0x58dd4:
        STEP();
        if (B(0x8d)) {SL(0,0x58e30);SW(0x86,0x1e);}
        k591fe(a);k5929a(a);DONE();
    case 0x58df6:
        STATE(8);
        SW(0x86,0);SL(0,0x58e0a);
        /* fall through */
    case 0x58e0a:
        STEP();
        if (B(0x8d)) {SW(0x86,0xf);SB(0x90,0);SL(0,0x58cbe);}
        k591fe(a);k5929a(a);DONE();
    case 0x58e30:
        STATE(4);
        SL(0x62,0);SB(0x90,0xff);SL(0,0x58e48);
        /* fall through */
    case 0x58e48: {
        STEP();
        if (W(0x78)==3u) {
            if (k59efc(a,e,0x586ca,&t,&d0)) SL(0,0x58df6);
            else if (k59efc(a,e,0x586d2,&t,&d0) && !d0) SL(0,0x58df6);
        }
        uint16_t v=W(0x86);SW(0x86,v-1u);
        if (!v) {SB(0x8e,0);SL(0,0x58efc);}
        if (B(0x8f)) {
            v=W(0x7a);SW(0x7a,v-1u);
            if (!v) {
                SW(0x7a,0);
                if (k59efc(a,e,0x586da,&t,NULL)) {
                    if (S16(W(0x3c))<0xa0) {if (S16(t.x)<0xa0) {flip(a);SL(0,0x5899a);}}
                    else if (S16(t.x)>0xa0) {flip(a);SL(0,0x5899a);}
                }
            }
        }
        k591fe(a);k5929a(a);DONE();
    }
    case 0x58efc:
        STATE(5);
        c=child(a,e,(B(0x8e)&1u)?0x59a62:0x59a3e,0x1c);
        if (c && BIT(0x15,0)) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|1u));
        SL(0,0x58f4a);
        /* fall through */
    case 0x58f4a:
        STEP();
        if (B(0x8d)) SL(0,0x58f66);
        k591fe(a);k5929a(a);DONE();
    case 0x58f66:
        SW(0x86,rank(a,e,0x2d124c));SW(0x80,6);SW(0x86,rank(a,e,0x2d124c));
        if (!k59c2c(a,e)) return GA_UNSUPPORTED;
        c=child(a,e,(B(0x8e)&1u)?0x594ce:0x594b0,0x1c);
        if (c) ga_place_at(c,a,e,BIT(0x15,0)?0x10:-0x10,0x30);          /* 01ACF2 */
        SL(0,0x58ff2);
        /* fall through */
    case 0x58ff2:
        STEP();
        SW(0x86,W(0x86)-1u);
        if (!W(0x86)) {
            SB(0x8e,B(0x8e)+1u);
            if (B(0x8e)>=(uint8_t)rank(a,e,0x2d11b8)) {SB(0x91,0);SW(0x86,rank(a,e,0x2d11ea));SL(0,0x58e30);}
            else SL(0,0x58efc);
        }
        k591fe(a);k5929a(a);DONE();
    case 0x59048:
        SB(0x91,0);STATE(7);
        SL(0x62,0);ga_ground(a,e,NULL);
        after_script(a,e,0x59b5c,0,0);                                   /* 059086: BTST, nothing written */
        SB(0x69,0xff);                                                   /* 019DFC */
        if (!W(0x6e) && e->solid) e->solid(e->ctx,a,0);
        sound(e,0x1021);SL(0,0x590b8);
        /* fall through */
    case 0x590b8:
        if (B(0x94) && e->solid) e->solid(e->ctx,a,0);
        if (!BIT(0x15,6)) {SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);} else ga_ground(a,e,NULL);
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (r && !W(0x6e)) {
            sound(e,0x102f);
            for (unsigned i=0;i<16;i++) {                                /* 01DBAA */
                c=child(a,e,0x1d31c,0x2a);
                if (c) ga_place_from(c,a,e,0,0x18);
            }
            after_script(a,e,0x59b5c,0,0);
            if (e->unsupported) return GA_UNSUPPORTED;
            return GA_DELETE;
        }
        if (W(0x6e)) {                                                   /* 0591C4 */
            if (e->dyn_set) e->dyn_set(e->ctx,a,BIT(0x15,0)?0x58672:0x58656,0,0);
            SB(0x94,0xff);
        } else if (e->dyn_release) e->dyn_release(e->ctx,a);             /* 013F1E */
        DONE();
    /* ---- turret ---- */
    case 0x592c6:
        p=creator(a,e);
        ga_place(a,e,0,0,p?NEO_GA_TAG(p->index):0);                      /* 01AD6E */
        SL(0x7c,0x58776);SW(0x4e,0x32);ga_palette(a,0x1c);SL(0x50,0x5869a);
        if (e->solid) e->solid(e->ctx,a,0x586b4);
        ga_flash_palette(a,0x30b);SL(0,0x59310);
        DONE();
    case 0x59310:
    case 0x5934c:
        if (!(p=creator(a,e))) {ga_fail(e,L(0));return GA_UNSUPPORTED;}
        for (unsigned turn=0;;turn++) {
            if (L(0)==0x59310u) {
                SB(0x15,(B(0x15)&0xfeu)|(ga_b(p,0x15)&1u));
                SW(0x80,ga_w(p,0x80));
                if (W(0x80)!=7u && !k59c2c(a,e)) return GA_UNSUPPORTED;
                SL(0,0x5934c);
            }
            if (ga_b(p,0x5f)&0x80u) goto turret_end;
            if (ga_w(p,0x80)==W(0x80)) break;
            if (turn) {ga_fail(e,0x59366);return GA_UNSUPPORTED;}
            SL(0,0x59310);                                               /* 059366: BNE 059310 (the routine word stays 05934C) */
        }
        SL(0,0x5934c);
        ga_project(a,e);
        if (ga_w(p,0x78)!=1u) {ga_register(a,e,NULL,NULL);SB(0x5a,0);}
        if (B(0x5f)&0x80u) goto turret_end;
        SW(0x1e,ga_w(p,0x1e)+1u);
        {uint8_t f=ga_b(p,0x17);if (f) {if (!(f&0x80u)) f++;SB(0x17,f);}}   /* 0044F6 */
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (ga_creator_alive(a,e)) DONE();
        /* fall through */
    turret_end:                                                          /* 0593B6 */
        SW(0x1e,ga_w(p,0x1e)+1u);
        {uint8_t f=ga_b(p,0x17);if (f) {if (!(f&0x80u)) f++;SB(0x17,f);}}
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (ga_b(p,0x91)) DONE();
        ga_sb(p,0x8f,0xff);ga_sw(p,0x78,2);ga_sb(p,0x92,S16(ga_w(p,0x3c))>0xa0?0xff:0);
        c=after_script(a,e,0x1c6ee,0,0x20);
        if (c) {ga_sw(c,0x66,0xe0ed);ga_sw(c,0x1e,0xe0ed);}
        if (e->unsupported) return GA_UNSUPPORTED;
        return GA_DELETE;
    /* ---- shots ---- */
    case 0x594b0:
    case 0x594ce: {
        bool second=L(0)==0x594ceu;
        if (!ga_anim_start(a,e,second?0x24c0fc:0x24bfbc,0x100)) return GA_UNSUPPORTED;
        SB(0x81,second?0xff:0);SW(0x7e,second?0x14:0x17);
        sound(e,0x10a3);ga_project(a,e);
        if (!(p=creator(a,e))) {ga_fail(e,L(0));return GA_UNSUPPORTED;}
        if (ga_b(p,0x15)&1u) SB(0x15,B(0x15)|1u);
        SB(0x15,B(0x15)|0x20u);
        SB(0x86,0);SL(0x30,0);SL(0x34,0);SL(0x72,0);SL(0x76,0);SW(0x7a,0);SB(0x82,0);SB(0x83,0);SB(0x84,0);SB(0x85,0);
        SW(0x7c,ga_w(p,0x40)+0x18u);
        SW(0x4e,rank(a,e,0x2d12ae));SW(0x66,0xe0ef);SW(0x1e,0xe0ef);ga_palette(a,0x1e);
        SL(0x50,second?0x5870e:0x58702);
        SL(0x6e,(uint32_t)(int32_t)S16(rank(a,e,0x2d10c0))<<8);
        SB(0x5e,B(0x5e)|0x80u);                                          /* 004C1C */
        SL(0,0x595b2);
    }   /* fall through */
    case 0x595b2: {
        ga_air(a,e);ga_register(a,e,NULL,NULL);SB(0x5a,0);              /* 0143E4, 004C24 */
        if (B(0x5f)&0x80u) goto shot_end;
        if (!(p=creator(a,e))) {ga_fail(e,0x595cc);return GA_UNSUPPORTED;}
        if (ga_b(p,0x8f)) goto shot_end;
        if (ga_anim_step(a,e,shot_call)<0) return GA_UNSUPPORTED;
        if (B(0x86)) {
            uint8_t v=B(0x85);SB(0x85,v-0x60u);
            if (v<0x60u) {                                               /* 0599F0 */
                c=child(a,e,0x5996a,0x2a);
                if (c) {
                    ga_place_from(c,a,e,B(0x81)?4:0x16,0);               /* 01B220 */
                    ga_sl(c,0x30,(uint32_t)(S32(L(0x30))>>1));ga_sl(c,0x34,(uint32_t)(S32(L(0x34))>>1));
                    ga_sl(c,0x6e,BIT(0x15,0)?0xfff40000u:0xc0000u);ga_sl(c,0x72,0xfffee000u);
                }
            }
            SB(0x85,B(0x85)+1u);
        }
        if (BIT(0x15,6) || BIT(0x5f,0)) goto shot_end;
        if (B(0x82)) {
            if (S16(W(0x7c))>S16(W(0x40))) k597de(a);
            if (k59efc(a,e,0x586da,&t,NULL) && (int16_t)(t.y+0x14u)>S16(W(0x40))) k597de(a);
        }
        {
            int32_t d6=S32(L(0x6e)),d7=S32(L(0x30)+L(0x72));
            if (d7<0) {if (d7<-d6) d7=-d6;} else if (d7>d6) d7=d6;
            SL(0x30,d7);SL(0x34,L(0x34)+L(0x76));
        }
        if (W(0x7e)) SW(0x7e,W(0x7e)-1u);
        else if (ga_attack(a,e,B(0x81)?0x586f2:0x586e2)>1u) goto shot_end;
        if (ga_cull(a,e,0x586c2)) return GA_DELETE;
        DONE();
    shot_end:                                                            /* 0596DE -> 05980C */
        sound(e,0x1022);
        after_script(a,e,0x59852,BIT(0x15,0)?0x10:-0x10,0);
        if (e->unsupported) return GA_UNSUPPORTED;
        return GA_DELETE;
    }
    case 0x59852:
        SW(0x66,0xe0ed);SW(0x1e,0xe0ed);
        if (!ga_anim_start(a,e,0x22ce6c,0x100)) return GA_UNSUPPORTED;
        ga_palette(a,0x204);
        if (ga_rand(e)&1u) SB(0x15,B(0x15)|1u); else SB(0x15,B(0x15)&0xfeu);
        SB(0x15,B(0x15)&0xfdu);SB(0x12,0xff);SB(0x13,0xff);SL(0x50,0);SL(0,0x598b2);
        /* fall through */
    case 0x598b2:
        ga_project(a,e);
        if ((r=ga_anim_step(a,e,NULL))<0 || e->unsupported) return GA_UNSUPPORTED;
        if (r || ga_cull(a,e,0x586c2)) return GA_DELETE;
        DONE();
    /* ---- the shot's trail ---- */
    case 0x5996a:
        SW(0x66,0x1001);SW(0x1e,0x1001);ga_palette(a,0x37c);
        if (!ga_anim_start(a,e,0x24c53e,0x100)) return GA_UNSUPPORTED;
        SL(0,0x5999a);
        /* fall through */
    case 0x5999a:
        SL(0x30,L(0x30)+(uint32_t)(S32(L(0x6e)-L(0x30))>>4));
        SL(0x34,L(0x34)+(uint32_t)(S32(L(0x72)-L(0x34))>>4));
        ga_move(a,e);                                                    /* 01AC38 */
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x599e2);
        if (ga_cull_rect(a,e,0x599e8)) SL(0,0x599e2);                    /* 019504 */
        DONE();
    case 0x599e2:
        return GA_DELETE;
    /* ---- muzzle flashes ---- */
    case 0x59a3e:
    case 0x59a62:
        if (!ga_anim_start(a,e,L(0)==0x59a3eu?0x24bf42:0x24c082,0x100)) return GA_UNSUPPORTED;
        p=creator(a,e);
        ga_place(a,e,BIT(0x15,0)?0x10:-0x10,0x30,p?NEO_GA_TAG(p->index):0);   /* 01AD6E */
        ga_palette(a,0x1e);SL(0,0x59a9c);
        /* fall through */
    case 0x59a9c:
        if (!(p=creator(a,e))) {ga_fail(e,0x59a9c);return GA_UNSUPPORTED;}
        if (ga_b(p,0x8f)) return GA_DELETE;
        ga_project(a,e);SW(0x1e,ga_w(p,0x1e)+2u);
        if ((r=ga_anim_step(a,e,NULL))<0 || e->unsupported) return GA_UNSUPPORTED;
        return r?GA_DELETE:GA_MORE;
    /* ---- the wreck's piece ---- */
    case 0x59b5c:
        if (!ga_anim_start(a,e,0x24c3aa,0x100)) return GA_UNSUPPORTED;
        SW(0x66,0x1001);SW(0x1e,0x1001);ga_palette(a,0x1d);SL(0,0x59b8c);
        /* fall through */
    case 0x59b8c:
        ga_project(a,e);
        if ((r=ga_anim_step(a,e,NULL))<0 || e->unsupported) return GA_UNSUPPORTED;
        return r?GA_DELETE:GA_MORE;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
bool neo_m4veh_routine(uint32_t r) {
    static const uint32_t k[]={0x5879a,0x587c4,0x587d8,0x58806,0x5899a,0x589d2,0x58aa8,0x58aec,0x58b84,0x58bc2,0x58c2e,0x58c66,0x58cbe,0x58ce4,
        0x58dc0,0x58dd4,0x58df6,0x58e0a,0x58e30,0x58e48,0x58efc,0x58f4a,0x58f66,0x58ff2,0x59048,0x590b8,
        0x592c6,0x59310,0x5934c,0x594b0,0x594ce,0x595b2,0x59852,0x598b2,0x5996a,0x5999a,0x599e2,0x59a3e,0x59a62,0x59a9c,0x59b5c,0x59b8c};
    for (unsigned i=0;i<sizeof k/sizeof k[0];i++) if (r==k[i]) return true;
    return false;
}
