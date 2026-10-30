#include "neo_region.h"
/* Covered wagon of Mission 1 (camera ~2240..2336, spawn-table entry 033C98),
 * manual translation; ROM labels are provenance only.
 *
 * 033C98 drives (+62, fffd4000 towards the scene centre) until the distance
 * +7A passes +70, stops (033D8A/033DB4, the literal sign tests of the
 * original stop it in the same update unless the speed crosses zero), opens
 * the canvas (program 239E30, callback 239ECE: child 034022 and +81) and
 * waits with its hurtbox (004C24) while +74 counts the invulnerable time
 * (+80: health restored from the rank table 2D3E5C). Destroyed (+5F bit 7):
 * the children are deleted (004192), 16 explosions (01DBAA), six pieces
 * 034094 after the script actor (00411C), debris 01DA40 (list 033C46),
 * 019DFC, and the wreck 033F2E keeps its platform (0140C0, maps 033ADC /
 * 033B6E); with +72 = 0 it leaves after +7E updates (cartridge check, 004188
 * on genuine hardware). Its body is a dynamic collision object every update
 * (034260, maps 033924/033992 closed, 033A00/033A6E open) and the roof child
 * 03415A carries another one (033C00/033C12) until the wagon dies. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define SET(o,b) SB(o,B(o)|(1u<<(b)))
#define CLR(o,b) SB(o,B(o)&~(1u<<(b)))
#define BIT(o,b) ((B(o)>>(b))&1u)
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
#define STEP() do { if (ga_anim_step(a,e,wagon_call)<0) return GA_UNSUPPORTED; } while (0)
static bool wagon_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter);
static neo_gactor_t *parent(neo_gactor_t *a,neo_genv_t *e) {return e->creator?e->creator(e->ctx,a):NULL;}
static uint32_t parent_tag(neo_gactor_t *a,neo_genv_t *e) {neo_gactor_t *p=parent(a,e);return p?NEO_GA_TAG(p->index):0;}
static uint16_t rank(neo_gactor_t *a,neo_genv_t *e,uint32_t table) {
    const uint8_t *t=ga_rom(table);
    if (!t) {ga_fail(e,table);return 0;}
    return ga_rank_word(a,e,t);
}
/* 0040BC after the running actor (the child places itself). */
static neo_gactor_t *spawn(neo_gactor_t *a,neo_genv_t *e,uint32_t routine) {return e->spawn?e->spawn(e->ctx,a,routine,false):NULL;}
static void sound(neo_genv_t *e,uint16_t id) {if (e->sound) e->sound(e->ctx,id);}
static void dyn(neo_gactor_t *a,neo_genv_t *e,uint32_t map) {if (e->dyn_set) e->dyn_set(e->ctx,a,map,0,0);}
/* 08CCD4: ground step (01456A) on the floor, else gravity + air (0143DC). */
static void k8ccd4(neo_gactor_t *a,neo_genv_t *e) {
    if (B(0x15)&0x40u) {uint32_t kind;ga_ground(a,e,&kind);}
    else {SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);}
}
/* 034260: the body as a dynamic collision object. */
static void k34260(neo_gactor_t *a,neo_genv_t *e) {
    if (!B(0x81)) dyn(a,e,BIT(0x15,0)?0x33992u:0x33924u);
    else dyn(a,e,BIT(0x15,0)?0x33a6eu:0x33a00u);
}
/* 01DBAA: 16 explosions 01D31C at (0,24). */
static void explosions(neo_gactor_t *a,neo_genv_t *e) {for (unsigned i=0;i<16;i++) ga_spawn_from(a,e,0x1d31c,0,0x18);}
/* 01DA40: debris emitter 01D764 with the list `list`, +66/+1E = 8800. */
static void debris(neo_gactor_t *a,neo_genv_t *e,uint32_t list) {
    neo_gactor_t *c=ga_spawn_from(a,e,0x1d764,0,0);
    if (!c) return;
    if (B(0x15)&4u) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|4u));
    ga_sl(c,0x86,list);ga_sw(c,0x66,0x8800);ga_sw(c,0x1e,0x8800);
}
/* 033E78..033F28: destruction. */
static ga_result_t destroyed(neo_gactor_t *a,neo_genv_t *e) {
    SW(0x7e,10);SW(0x66,0);SW(0x1e,0);SL(0x62,0);START(0x239ee4);
    if (e->kill_children) e->kill_children(e->ctx,a);               /* 004192 */
    explosions(a,e);
    for (unsigned i=0;i<6;i++) {
        /* 00411C after the script actor; the RNG is drawn either way. */
        neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x34094,true):NULL;
        uint16_t r1=ga_rand(e)&0x1fu,r2=ga_rand(e)&0x3fu;
        int16_t dx=(int16_t)(r2+(W(0x6e)?0xffecu:0xffd0u)),dy=(int16_t)(r1+0x20u);
        if (c) ga_place_at(c,a,e,dx,dy);                            /* 01ACF2 */
    }
    debris(a,e,0x33c46);
    SB(0x69,0xff);                                                  /* 019DFC */
    sound(e,0x1035);
    SL(0,0x33f2e);                                                  /* falls into 033F2E */
    return GA_MORE;
}
static ga_result_t wagon_tick(neo_gactor_t *a,neo_genv_t *e) {
    for (;;) switch (L(0)) {
    case 0x33c98:
        ga_palette(a,0x121);ga_flash_palette(a,0x122);
        SW(0x66,0x8800);SW(0x1e,0x8800);SL(0x50,0x33c24);
        SW(0x4e,rank(a,e,0x2d3e5c));
        ga_floor(a,e);SL(0x62,0xfffd4000u);ga_project(a,e);
        if ((int16_t)W(0x3c)<0xa0) SL(0x62,0u-L(0x62));
        if (W(0x6e)) {SET(0x15,0);START(0x239df6);} else START(0x239dc8);
        SB(0x80,0xff);SB(0x81,0);
        sound(e,0x105d);
        spawn(a,e,0x3415a);
        SL(0,0x33d50);
        /* fall through */
    case 0x33d50: {
        SL(0x7a,L(0x7a)+L(0x62));
        uint16_t d7=W(0x7a);if (d7&0x8000u) d7=(uint16_t)-d7;
        if ((int16_t)d7>(int16_t)W(0x70)) {
            SL(0x76,0xc000);if (!(L(0x62)&0x80000000u)) SL(0x76,0u-L(0x76));
            START(0x239e24);SL(0,0x33db4);continue;
        }
        k8ccd4(a,e);STEP();k34260(a,e);
        return ga_cull(a,e,0x33c30)?GA_DELETE_TREE:GA_MORE;
    }
    case 0x33db4: {
        uint32_t d7=L(0x76);bool stop;
        SL(0x62,L(0x62)+d7);
        if (d7&0x80000000u) stop=!(L(0x62)&0x80000000u); else stop=(L(0x62)&0x80000000u)!=0;
        if (!stop) {
            k8ccd4(a,e);STEP();k34260(a,e);
            return ga_cull(a,e,0x33c30)?GA_DELETE_TREE:GA_MORE;
        }
        SL(0x62,0);START(0x239e30);SL(0,0x33e0a);
    }   /* fall through */
    case 0x33e0a:
        if (W(0x74)) {SW(0x74,W(0x74)-1u);if (!W(0x74)) {SW(0x74,0);SB(0x80,0);}}
        ga_register(a,e,NULL,NULL);
        SB(0x5a,0);
        if (B(0x80)) {CLR(0x5f,7);SW(0x4e,rank(a,e,0x2d3e5c));}
        else if (B(0x5f)&0x80u) {ga_result_t r=destroyed(a,e);if (r!=GA_MORE) return r;continue;}
        ga_stationary(a,e);STEP();k34260(a,e);
        return ga_cull(a,e,0x33c30)?GA_DELETE_TREE:GA_MORE;
    case 0x33f2e:
        ga_stationary(a,e);STEP();
        dyn(a,e,BIT(0x15,0)?0x33b6eu:0x33adcu);
        if (!W(0x72)) {
            SW(0x7e,W(0x7e)-1u);
            if (!W(0x7e)) {sound(e,0x1035);explosions(a,e);return GA_DELETE;}  /* 004188 */
        }
        return ga_cull(a,e,0x33c30)?GA_DELETE_TREE:GA_MORE;
    default: ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
}
/* 03415A: roof platform, at the wagon's position (01AD6E). */
static ga_result_t roof_tick(neo_gactor_t *a,neo_genv_t *e) {
    if (L(0)==0x3415a) {
        ga_place(a,e,0,0,parent_tag(a,e));
        neo_gactor_t *p=parent(a,e);if (p && (ga_b(p,0x15)&1u)) SET(0x15,0);
        SL(0,0x3417e);
    }
    neo_gactor_t *p=parent(a,e);
    if ((p && (ga_b(p,0x5f)&0x80u)) || !ga_creator_alive(a,e)) {
        if (e->dyn_release) e->dyn_release(e->ctx,a);              /* 013F1E */
        return GA_DELETE;                                           /* 004188 */
    }
    ga_project(a,e);
    dyn(a,e,BIT(0x15,0)?0x33c12u:0x33c00u);
    return GA_MORE;
}
/* 034022: open canvas, created by the program callback 239ECE. */
static ga_result_t canvas_tick(neo_gactor_t *a,neo_genv_t *e) {
    if (L(0)==0x34022) {
        ga_place(a,e,0,8,parent_tag(a,e));
        ga_palette(a,0x121);ga_flash_palette(a,0x122);
        SW(0x66,0x8838);SW(0x1e,0x8838);START(0x239f26);
        neo_gactor_t *p=parent(a,e);if (p && (ga_b(p,0x15)&1u)) SET(0x15,0);
        SL(0,0x3407a);
    }
    ga_project(a,e);
    {uint16_t d0=W(0x66);if ((d0&0xf000u)==0x8000u) SW(0x1e,d0-W(0x40));}  /* 01AD8C */
    neo_gactor_t *p=parent(a,e);                                    /* 0044F6 */
    if (p) {uint8_t f=ga_b(p,0x17);if (f) {if (!(f&0x80u)) f++;SB(0x17,f);}}
    STEP();
    return GA_MORE;
}
/* 034124: random horizontal speed +30/+32. */
static void k34124(neo_gactor_t *a,neo_genv_t *e) {
    uint16_t d1=ga_rand(e)&0xfffu,d0=ga_rand(e);
    if (!(d0&1u)) {SW(0x32,d1);SW(0x30,0);} else {SW(0x32,(uint16_t)-d1);SW(0x30,0xffff);}
}
/* 034094: wreck piece (after the script actor). */
static ga_result_t piece_tick(neo_gactor_t *a,neo_genv_t *e) {
    if (L(0)==0x34094) {
        ga_palette(a,0x123);SW(0x66,0xe0ed);SW(0x1e,0xe0ed);START(0x239f32);
        SL(0x30,0);
        uint32_t d0=((uint32_t)ga_rand(e)&0x1fffu)+0x2000u;SL(0x34,0u-d0);
        SW(0x6e,0);k34124(a,e);
        SW(0x26,(ga_rand(e)&0x3fu)+0xe0u);
        SL(0,0x340fa);
    }
    SW(0x6e,(W(0x6e)+1u)&7u);
    if (!W(0x6e)) k34124(a,e);
    ga_move(a,e);
    int r=ga_anim_step(a,e,wagon_call);
    if (r<0) return GA_UNSUPPORTED;
    return r?GA_DELETE:GA_MORE;
}
static bool wagon_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    if (routine==0x239ece) {spawn(a,e,0x34022);SB(0x81,0xff);return true;}
    return false;
}
bool neo_wagon_routine(uint32_t r) {
    return r==0x33c98 || r==0x33d50 || r==0x33db4 || r==0x33e0a || r==0x33f2e ||
        r==0x3415a || r==0x3417e || r==0x34022 || r==0x3407a || r==0x34094 || r==0x340fa;
}
ga_result_t neo_wagon_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=L(0);
    if (r==0x3415a || r==0x3417e) return roof_tick(a,e);
    if (r==0x34022 || r==0x3407a) return canvas_tick(a,e);
    if (r==0x34094 || r==0x340fa) return piece_tick(a,e);
    return wagon_tick(a,e);
}
