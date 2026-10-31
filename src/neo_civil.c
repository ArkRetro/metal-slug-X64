#include "neo_region.h"
/* Village civilians 031D80 (camera 440, script record 2D641E, the random
 * alternative of the porter 03264E) and 031D92 (camera 1984, record
 * 2D6C76), group 9 not counted. Manual translation of ROM 031D80..032148;
 * ROM labels are provenance only.
 *
 * The script node is a group leader without graphics: +83 (0 / 1) and +76
 * its command sequence (031D74: 0,1,2,3 / 031D7A: 0,1,3; a negative byte
 * jumps back by the next byte, 031F32), +72 the figures' speed (word << 11),
 * +7A itself. It creates two figures 031F52 (program tables 031D40 and
 * 031D34; 031D92 only the second) after itself (0040BC), counting them in
 * +81, and while any is alive: command 0 counts +7E (0029F2 of 0x1E plus
 * 0x14) down to the next command, command 3 takes the next one at once.
 * Commands 1/2 make the figures whose table word +0A is 0/nonzero play their
 * second program; its end takes the next command (031F32 on the leader).
 *
 * A figure (palette 0x92, hurtbox 031D54) stands in place (01436A). Once
 * the camera progress passes its +70 (leader +70 + progress at creation) it
 * registers its hurtbox (004C24); a hit whose kind +5A is in 031D68 sets the
 * leader's +82, and with +82 set every figure flees (032040): program
 * 237CC0 at the speed of 031B86 by its +62, a random direction (the sign of
 * the RNG's mixed word), the scream 0x104F (0120F8), then walks (08CCD4),
 * turning at walls (+5F bit 0). A figure leaves outside 031D4C (01945C,
 * leader +81 - 1); the leader once +81 is 0 (the cartridge check before
 * 004188 is not reproduced). */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define START(t,s) do { if (!ga_anim_start(a,e,(t),(s))) return GA_UNSUPPORTED; } while (0)
static uint8_t rb(uint32_t address) {const uint8_t *p=ga_rom(address);return p?p[0]:0;}
static uint16_t rw(uint32_t address) {const uint8_t *p=ga_rom(address);return p?(uint16_t)(p[0]<<8|p[1]):0;}
static uint32_t rl(uint32_t address) {return (uint32_t)rw(address)<<16|rw(address+2);}
static neo_gactor_t *creator(neo_gactor_t *a,neo_genv_t *e) {return e->creator?e->creator(e->ctx,a):NULL;}
/* 031EDA: +7E = 0029F2(0x1E) + 0x14. */
static void k31eda(neo_gactor_t *a,neo_genv_t *e) {SW(0x7e,ga_rand_range(e,0x1e)+0x14u);}
/* 031F32: the next command of the leader `l` (+76 sequence) into its +80. */
static void k31f32(neo_gactor_t *l) {
    uint32_t p=ga_l(l,0x76);uint8_t d0=rb(p++);
    if (d0&0x80u) {p-=rb(p);d0=rb(p++);}
    ga_sl(l,0x76,p);ga_sb(l,0x80,d0);
}
/* 031EEE: a figure 031F52 with the program table `list` after the leader. */
static void k31eee(neo_gactor_t *a,neo_genv_t *e,uint32_t list) {
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,a,0x31f52,false):NULL;
    if (!c) return;
    ga_sl(c,0x76,list);ga_sw(c,0x6e,W(0x6e));ga_sw(c,0x70,(uint16_t)(W(0x70)+e->camera));
    ga_sl(c,0x62,L(0x72));ga_sl(c,0x7a,L(0x7a));SB(0x81,B(0x81)+1u);
}
/* 0320D0: outside 031D4C the figure goes (leader +81 - 1). */
static bool k320d0(neo_gactor_t *a,neo_genv_t *e) {
    if (!ga_cull(a,e,0x31d4c)) return false;
    neo_gactor_t *p=creator(a,e);if (p) ga_sb(p,0x81,(uint8_t)(ga_b(p,0x81)-1u));
    return true;
}
/* 0320F8: hurtbox once the camera passed +70; a panic hit marks the leader;
 * a marked leader turns the figure to 032040. */
static void k320f8(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *p=creator(a,e);
    if (W(0x70)<e->camera && ga_register(a,e,NULL,NULL)) {
        for (uint32_t t=0x31d68;rb(t);t++) if (rb(t)==B(0x5a)) {if (p) ga_sb(p,0x82,0xff);SB(0x5a,0);return;}
    }
    if (p && ga_b(p,0x82)) SL(0,0x32040);
    SB(0x5a,0);
}
/* 08CCD4: ground step on the floor, else gravity + air. */
static void k8ccd4(neo_gactor_t *a,neo_genv_t *e) {
    if (B(0x15)&0x40u) {uint32_t kind;ga_ground(a,e,&kind);}
    else {SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);}
}
ga_result_t neo_civil_tick(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *p;int r;
    switch (L(0)) {
    case 0x31d80: case 0x31d92:
        if (L(0)==0x31d80) {SB(0x83,0);SL(0x76,0x31d74);} else {SB(0x83,1);SL(0x76,0x31d7a);}
        SB(0x69,0xff);                                                  /* 019DFC */
        if (!L(0x70)) {ga_fail(e,0x31db6);return GA_UNSUPPORTED;}       /* TRAP #F "EX02 Option Error speed" */
        SB(0x82,0);SB(0x80,0);
        SL(0x72,((uint32_t)W(0x72)<<16)>>5);
        SL(0x7a,NEO_GA_TAG(a->index));
        if (B(0x83)!=1u) k31eee(a,e,0x31d40);
        k31eee(a,e,0x31d34);
        SL(0,0x31e10);
        return GA_MORE;
    case 0x31e10:
        k31eda(a,e);SL(0,0x31e1a);
        /* fall through */
    case 0x31e1a:
        if (!B(0x81)) return GA_DELETE;                                 /* 004188 */
        if (!B(0x80)) {SW(0x7e,W(0x7e)-1u);if (W(0x7e)&0x8000u) k31f32(a);}
        if (B(0x80)==3u) {k31f32(a);k31eda(a,e);}
        return GA_MORE;
    case 0x31f52: {
        SL(0x50,0x31d54);ga_palette(a,0x92);SW(0x66,0x8820);SW(0x1e,0x8820);
        if (!W(0x6e)) SB(0x15,B(0x15)|1u);
        uint16_t d7=(uint16_t)(((uint16_t)(rw(L(0x76)+0xa)<<1)+W(0x6e))<<1);
        p=creator(a,e);
        ga_place(a,e,(int16_t)rw(0x31d60u+d7),0,p?NEO_GA_TAG(p->index):0);   /* 01AD6E */
        ga_floor(a,e);
        START(rl(L(0x76)),0x100);
        SL(0,0x31fbe);
        goto stand;
    }
    case 0x31fa8:
        START(rl(L(0x76)),0x100);SL(0,0x31fbe);
        /* fall through */
    case 0x31fbe:
    stand:
        if (k320d0(a,e)) return GA_DELETE;
        k320f8(a,e);
        p=creator(a,e);
        if (p && ga_b(p,0x80)==(rw(L(0x76)+0xa)?2u:1u)) SL(0,0x32004);
        ga_stationary(a,e);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x32004:
        START(rl(L(0x76)+4u),0x100);SL(0,0x3201c);
        /* fall through */
    case 0x3201c:
        if (k320d0(a,e)) return GA_DELETE;
        k320f8(a,e);
        ga_stationary(a,e);
        r=ga_anim_step(a,e,NULL);
        if (r<0) return GA_UNSUPPORTED;
        if (r) {p=creator(a,e);if (p) k31f32(p);SL(0,0x31fa8);}
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x32040: {
        uint32_t d7=L(0x62);
        if ((uint16_t)d7>0x8000u) d7+=0x10000u;
        uint16_t i=(uint16_t)(d7>>16);if (i>10u) i=10u;
        START(0x237cc0,rw(0x31b86u+2u*i));
        neo_rng_result_t rr=neo_rng_next_result(e->rng);
        if (rr.mixed_word&0x8000u) SL(0x62,0u-L(0x62));
        SB(0x15,B(0x15)&~1u);
        if (!(W(0x62)&0x8000u)) SB(0x15,B(0x15)|1u);
        if (e->sound) e->sound(e->ctx,0x104f);                          /* 0120F8 */
        SL(0,0x320aa);
    }
        /* fall through */
    case 0x320aa:
        if (k320d0(a,e)) return GA_DELETE;
        k8ccd4(a,e);
        if (B(0x5f)&1u) {SL(0x62,0u-L(0x62));SB(0x15,B(0x15)^1u);}
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
bool neo_civil_routine(uint32_t r) {
    return r==0x31d80 || r==0x31d92 || r==0x31e10 || r==0x31e1a || r==0x31f52 || r==0x31fa8 || r==0x31fbe || r==0x32004 || r==0x3201c || r==0x32040 || r==0x320aa;
}
