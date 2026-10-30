#include "neo_region.h"
#include "neo_kernel.h"
#include "neo_soldier_ai.h"
/* Domed building of Mission 1 (camera 1920), manual translation; ROM labels
 * are provenance only.
 *
 * 054904 (script record 2D698A, camera 1600) builds three domes 054C2C
 * (+6E = 2, 1, 0) and two banners 054B1C, then waits for the record 2D6A48
 * (spawn-table entry 05486A: sets RAM 10444E/10444F and leaves) to take its
 * group (+69), sets the camera autoscroll mode 2 (0C7B48) and lets the
 * domes rise: dome 0 lifts the camera through RAM 10E960 (vy). Each dome
 * fires missiles 055638 from its launcher 055056 (animation callbacks
 * 055364/0553D2/055388/055448/055454) until its hurtbox (+50) takes enough
 * damage. With the three domes down the building collapses: player lock
 * (0C8CD6, 0CA2E0 with the no-op handler 054B18), debris, white flashes
 * (0551E0 through 009BB0), shake (0C801A), kill-all (004BFE), the map
 * swap actor (0552CC -> 0B595A -> 0B5972/0B5982 -> 0B58CC) and the release.
 *
 * Every code path that would branch to the cartridge check (2FFFE0, a jump
 * to 0546EA = 003C64 on a mismatch) ends in 004188 (delete) on genuine
 * hardware; that is what is translated. Services outside the actor list
 * (camera, flash, player lock, kill-all, map swap) go through neo_genv_t. */
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
#define STEP() do { if (ga_anim_step(a,e,dome_call)<0) return GA_UNSUPPORTED; } while (0)
static int16_t S16(uint16_t v) {return (int16_t)v;}
static uint16_t rw(uint32_t address) {const uint8_t *p=ga_rom(address);return p?(uint16_t)(p[0]<<8|p[1]):0;}
static uint32_t rl(uint32_t address) {return (uint32_t)rw(address)<<16|rw(address+2);}
static neo_gactor_t *parent(neo_gactor_t *a,neo_genv_t *e) {return e->creator?e->creator(e->ctx,a):NULL;}
static uint16_t rank(neo_gactor_t *a,neo_genv_t *e,uint32_t table) {
    const uint8_t *t=ga_rom(table);
    if (!t) {ga_fail(e,table);return 0;}
    return ga_rank_word(a,e,t);
}
/* 0040BC + 01B220 (0,0) from the running actor. */
static neo_gactor_t *child(neo_gactor_t *a,neo_genv_t *e,uint32_t routine) {return ga_spawn_from(a,e,routine,0,0);}
/* 01DA40: debris list emitter 01D764 (+86 = list). */
static neo_gactor_t *debris(neo_gactor_t *a,neo_genv_t *e,uint32_t list) {
    neo_gactor_t *c=child(a,e,0x1d764);
    if (!c) return NULL;
    if (B(0x15)&4u) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|4u));
    ga_sl(c,0x86,list);
    return c;
}
/* 0552F8: number of the main actor's live domes (+60 bits 0..2). */
static unsigned live(const neo_gactor_t *m) {uint8_t v=ga_b(m,0x60);return (v&1u)+((v>>1)&1u)+((v>>2)&1u);}
static uint32_t by_live(unsigned n,uint32_t t3,uint32_t t2,uint32_t t1) {return n==3?t3:n==2?t2:t1;}
static bool dome_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter);
/* ---- main 054904 ------------------------------------------------------------ */
static ga_result_t main_tick(neo_gactor_t *a,neo_genv_t *e) {
    for (;;) switch (L(0)) {
    case 0x54904: {
        if (e->flags) e->flags->dome_signal=0;                       /* RAM 10444E */
        static const uint16_t index[3]={2,1,0};
        for (unsigned i=0;i<3;i++) {neo_gactor_t *d=e->spawn?e->spawn(e->ctx,a,0x54c2c,false):NULL;if (d) ga_sw(d,0x6e,index[i]);}
        static const uint16_t bx[2]={0x7c0,0x870};static const uint8_t bt[2]={5,10};
        for (unsigned i=0;i<2;i++) {neo_gactor_t *b=e->spawn?e->spawn(e->ctx,a,0x54b1c,false):NULL;if (b) {ga_sw(b,0x28,bx[i]);ga_sb(b,0x8a,bt[i]);}}
        SB(0x61,0);SB(0x8f,0);SB(0x91,0);SB(0x90,0);SB(0x60,7);SB(0x74,0);SB(0x75,0xff);
        SB(0x93,ga_rand(e)&3u);
        SL(0,0x549b8);
    }   /* fall through */
    case 0x549b8:
        if (!e->flags || !e->flags->dome_signal) {ga_project(a,e);return GA_MORE;}
        SB(0x69,e->flags->dome_group);
        if (e->camera_mode) e->camera_mode(e->ctx,2);               /* 0C7B48 */
        SB(0x8a,0x14);SL(0,0x549dc);
        /* fall through */
    case 0x549dc:
        SB(0x8a,B(0x8a)-1u);
        if (!(B(0x8a)&0x80u)) {ga_project(a,e);return GA_MORE;}
        SB(0x61,0xff);SB(0x90,0);SL(0,0x549f4);
        /* fall through */
    case 0x549f4:
        if (!B(0x60)) {SL(0,0x54a1c);continue;}
        if (B(0x90)!=7u) {ga_project(a,e);return GA_MORE;}
        SL(0,0x54a0c);
        /* fall through */
    case 0x54a0c:
        if (!B(0x60)) {SL(0,0x54a1c);continue;}
        ga_project(a,e);return GA_MORE;
    case 0x54a1c:
        SET(0x5f,7);SB(0x8a,0x1e);SL(0,0x54a2e);
        /* fall through */
    case 0x54a2e: {
        uint8_t t=B(0x8a);SB(0x8a,t-1u);
        if (t) {ga_project(a,e);return GA_MORE;}   /* subq; bcc: until it wraps */
        if (e->flags) {e->flags->player_lock=0xff;e->flags->player_handlers=1;}  /* 0C8CD6, 0CA2E0 x2 */
        if (e->flags) e->flags->flash_request=0xff;                     /* 0C8CD6 -> 004C06 */
        debris(a,e,0x547be);
        if (e->sound) e->sound(e->ctx,0x1026);
        SB(0x8a,0x32);SL(0,0x54a76);
    }   /* fall through */
    case 0x54a76: {
        uint8_t t=B(0x8a);SB(0x8a,t-1u);
        if (t) {ga_project(a,e);return GA_MORE;}
        if (e->spawn) e->spawn(e->ctx,a,0x551e0,false);                /* 0040BC only */
        if (e->shake) e->shake(e->ctx);                        /* 0C801A */
        if (e->flags) e->flags->kill_request=0xff;                       /* 004BFE */
        SB(0x91,0xff);
        neo_gactor_t *d=debris(a,e,0x54810);
        if (d) {ga_sw(d,0x66,0xffed);ga_sw(d,0x1e,0xffed);}
        SB(0x8a,0x32);SL(0,0x54abe);
    }   /* fall through */
    case 0x54abe: {
        uint8_t t=B(0x8a);SB(0x8a,t-1u);
        if (t) {ga_project(a,e);return GA_MORE;}
        SL(0,0x54ac6);
    }   /* fall through */
    case 0x54ac6:
        /* 0552CC: +8F = 1, then 0B595A inserts the map swap actor (carry
         * when it was allocated). */
        SB(0x8f,1);
        if (!e->map_swap || !e->map_swap(e->ctx)) {ga_project(a,e);return GA_MORE;}
        SB(0x69,0xff);                                               /* 019DFC */
        if (e->camera_mode) e->camera_mode(e->ctx,0);
        if (e->flags) {e->flags->player_handlers=0;e->flags->player_lock=0;}  /* 0CA2F2 x2, 0C8CE6 */
        SB(0x8a,0x64);SL(0,0x54b0a);
        /* fall through */
    case 0x54b0a:
        SB(0x8a,B(0x8a)-1u);
        if (!(B(0x8a)&0x80u)) {ga_project(a,e);return GA_MORE;}
        return GA_DELETE_TREE;                                       /* 0041AC */
    default: ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
}
/* ---- banners 054B1C ---------------------------------------------------------- */
static ga_result_t banner_tick(neo_gactor_t *a,neo_genv_t *e) {
    for (;;) switch (L(0)) {
    case 0x54b1c:
        ga_palette(a,0x472);
        ga_place(a,e,S16(W(0x28)),0x40,0x10c72eu);                   /* 01AB98 on 10C72E */
        if (e->front_layer) {if (e->front_layer(e->ctx)) SET(0x15,2); else CLR(0x15,2);}  /* 012022 */
        SW(0x66,0);SW(0x1e,0);START(0x24ac42);SL(0,0x54b66);
        /* fall through */
    case 0x54b66: {
        neo_gactor_t *m=parent(a,e);
        if (!m || !ga_b(m,0x91)) {ga_project(a,e);STEP();return GA_MORE;}
        SL(0,0x54b86);
    }   /* fall through */
    case 0x54b86:
        SB(0x8a,B(0x8a)-1u);
        if (B(0x8a)) {ga_project(a,e);STEP();return GA_MORE;}
        SB(0x1d,0);ga_palette(a,0x4b9);START(0x24abb0);              /* 009F88, 009EA8 */
        SL(0x30,0);SL(0x34,0x80000);SB(0x12,0x48);SB(0x13,0x48);SW(0x2c,W(0x2c)+8u);
        SW(0x66,0xfff1);SW(0x1e,0xfff1);SL(0,0x54bf0);
        /* fall through */
    case 0x54bf0: {
        if (B(0x13)==0xffu) return GA_DELETE;
        unsigned z=B(0x12)+5u;SB(0x12,z>0xffu?0xff:z);SB(0x13,B(0x12));
        SL(0x34,L(0x34)+0xffffb000u);ga_move(a,e);STEP();return GA_MORE;
    }
    default: ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
}
/* ---- dome 054C2C --------------------------------------------------------------- */
/* 0554A8 / 055526 / 05530E: timings for the number of live domes. */
static void k554a8(neo_gactor_t *a,neo_genv_t *e,neo_gactor_t *m) {
    unsigned n=live(m);
    SW(0x78,rank(a,e,by_live(n,0x2d1f08,0x2d1f22,0x2d1f3c)));SW(0x76,rank(a,e,by_live(n,0x2d1e8a,0x2d1ea4,0x2d1ed6)));
}
static void k55526(neo_gactor_t *a,neo_genv_t *e,neo_gactor_t *m) {SW(0x7a,rank(a,e,by_live(live(m),0x2d1df4,0x2d1e26,0x2d1e58)));}
static void k5530e(neo_gactor_t *a,neo_genv_t *e,neo_gactor_t *m) {
    if (!(W(0x70)&0x8000u)) return;
    SW(0x70,rank(a,e,by_live(live(m),0x2d1d58,0x2d1d72,0x2d1d8c)));
}
static void register_hit(neo_gactor_t *a,neo_genv_t *e) {if (ga_register(a,e,NULL,NULL)) SB(0x5a,0);}
static ga_result_t dome_dead(neo_gactor_t *a,neo_genv_t *e,neo_gactor_t *m);
static ga_result_t dome_tick(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *m=parent(a,e);
    if (!m) {ga_fail(e,0x54ca0);return GA_UNSUPPORTED;}
    for (;;) switch (L(0)) {
    case 0x54c2c:
        SET(0x15,2);ga_palette(a,0x475);ga_flash_palette(a,0x475);
        SW(0x66,0);SW(0x1e,0);SW(0x4e,rank(a,e,0x2d1cf4));
        SET(0x5f,6);SL(0x50,0x5473c);                                 /* 0552D8 */
        ga_place(a,e,S16(rw(0x54714u+2u*W(0x6e))),0x3f,NEO_GA_TAG(m->index)); /* 01AD6E */
        START(rl(0x546f0u+4u*W(0x6e)));SL(0,0x54ca0);
        /* fall through */
    case 0x54ca0:
        if (!ga_b(m,0x61)) {ga_project(a,e);STEP();return GA_MORE;}
        {
            int16_t dx=(int16_t)(S16(rw(0x5471au+2u*W(0x6e)))-S16(W(0x28))),dy=(int16_t)(0x5f-S16(W(0x2c)));
            uint8_t angle=(uint8_t)neo_angle(dx,dy);uint32_t vx,vy;
            ga_polar(0x18000,angle,&vx,&vy);SL(0x30,vx);SL(0x34,vy);
        }
        SW(0x84,0);SW(0x86,W(0x2c));
        if (e->sound) e->sound(e->ctx,0x1074);
        SL(0,0x54d0a);
        /* fall through */
    case 0x54d0a:
        if (S16((uint16_t)(W(0x2c)-W(0x86)))<0x20) {
            if (W(0x2c)<0x5fu) ga_move(a,e);
            if (!W(0x6e) && e->camera_rise) {
                /* RAM 10E960 = the rise of dome 0 since the last update,
                 * (y - 0x3F) * 0x80 >> 8 (mulu.w, lsr.w). */
                uint16_t d0=(uint16_t)((uint16_t)((uint16_t)(W(0x2c)-0x3fu)*0x80u)>>8);
                uint16_t d1=W(0x84);SW(0x84,d0);
                e->camera_rise(e->ctx,(int32_t)((uint32_t)(uint16_t)(d0-d1)<<16));
            }
            STEP();return GA_MORE;
        }
        ga_sb(m,0x90,(uint8_t)(ga_b(m,0x90)|(1u<<(W(0x6e)&7u))));
        if (!W(0x6e) && e->camera_rise) e->camera_rise(e->ctx,0);
        SW(0x28,rw(0x5471au+2u*W(0x6e)));SW(0x2c,0x5f);
        k55526(a,e,m);
        START(rl(0x54708u+4u*W(0x6e)));
        child(a,e,0x54f60);
        SB(0x8b,0);SB(0x60,0);
        {const uint8_t *t=ga_rom(0x54720u+(((unsigned)ga_b(m,0x93)&3u)<<2)+W(0x6e));SW(0x70,t?t[0]&0x7fu:0);}
        SL(0,0x54e00);
        /* fall through */
    case 0x54e00: ga_project(a,e);STEP();return GA_MORE;
    case 0x54e0e:
        CLR(0x5f,6);SL(0x50,0x54730);                                 /* 0552E8 */
        k554a8(a,e,m);k55526(a,e,m);SW(0x82,0);k5530e(a,e,m);
        SL(0,0x54e28);
        /* fall through */
    case 0x54e28:
        SW(0x70,W(0x70)-1u);
        if (W(0x70)&0x8000u) {SW(0x7c,W(0x78));SL(0,0x54e62);continue;}
        if (BIT(0x5f,7)) return dome_dead(a,e,m);
        register_hit(a,e);ga_project(a,e);STEP();return GA_MORE;
    case 0x54e62:
        if (BIT(0x5f,7)) return dome_dead(a,e,m);
        SW(0x7c,W(0x7c)-1u);
        if (W(0x7c)&0x8000u) {
            SW(0x82,W(0x82)+1u);SW(0x7a,W(0x7a)-1u);
            if (!W(0x7a)) SL(0,0x54e0e);
            SW(0x7c,W(0x76));
        }
        register_hit(a,e);ga_project(a,e);STEP();return GA_MORE;
    case 0x54f40:
        if (ga_b(m,0x8f)) return GA_DELETE_TREE;
        ga_project(a,e);STEP();return GA_MORE;
    default: ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
}
/* 054EA8: the dome is destroyed. */
static ga_result_t dome_dead(neo_gactor_t *a,neo_genv_t *e,neo_gactor_t *m) {
    unsigned i=W(0x6e)&7u;
    ga_sb(m,0x60,(uint8_t)(ga_b(m,0x60)&~(1u<<i)));ga_sb(m,0x74,(uint8_t)(ga_b(m,0x74)&~(1u<<i)));
    CLR(0x15,2);
    if (e->sound) e->sound(e->ctx,0x1021);
    child(a,e,0x407a2);
    ga_spawn_from(a,e,0x1c71e,0,(int16_t)0xfff0);
    debris(a,e,0x54764);
    SET(0x15,2);
    START(rl(0x546fcu+4u*i));SL(0,0x54f40);
    ga_project(a,e);STEP();return GA_MORE;
}
/* ---- children of the domes ------------------------------------------------------ */
static ga_result_t hatch_tick(neo_gactor_t *a,neo_genv_t *e) {           /* 054F60 */
    neo_gactor_t *p=parent(a,e);
    if (L(0)==0x54f60) {
        ga_palette(a,0x475);CLR(0x15,2);
        ga_place(a,e,0,0,p?NEO_GA_TAG(p->index):0);START(0x24a610);
        if (e->sound) e->sound(e->ctx,0x1075);
        SL(0,0x54f9e);
    }
    if (!p || (ga_b(p,0x5f)&0x80u) || ga_b(p,0x8b)) return GA_DELETE;  /* via the check, 004188 */
    ga_project(a,e);STEP();return GA_MORE;
}
static ga_result_t launcher_tick(neo_gactor_t *a,neo_genv_t *e) {        /* 055056 */
    neo_gactor_t *p=parent(a,e);
    if (L(0)==0x55056) {
        ga_palette(a,0x478);ga_flash_palette(a,0x87f);CLR(0x15,2);
        ga_place(a,e,0,0,p?NEO_GA_TAG(p->index):0);START(0x24a6f2);
        SW(0x7a,p?ga_w(p,0x7a):0);SL(0,0x5509e);
    }
    if (!p || (ga_b(p,0x5f)&0x80u)) return GA_DELETE;
    SW(0x82,ga_w(p,0x82));
    uint16_t d0=ga_w(p,0x7a);
    if (!(d0&0x8000u) && d0!=W(0x7a)) {SW(0x7a,d0);START(0x24a742);}
    ga_project(a,e);
    /* 0044F6: share the creator's flash timer. */
    {uint8_t f=ga_b(p,0x17);if (f) {if (!(f&0x80u)) f++;SB(0x17,f);}}
    int r=ga_anim_step(a,e,dome_call);
    if (r<0) return GA_UNSUPPORTED;
    return r?GA_DELETE:GA_MORE;
}
static ga_result_t ender_tick(neo_gactor_t *a,neo_genv_t *e) {           /* 05518A */
    if (L(0)==0x5518a) {
        neo_gactor_t *p=parent(a,e);
        ga_palette(a,0x476);CLR(0x15,2);ga_place(a,e,0,0,p?NEO_GA_TAG(p->index):0);START(0x24a7ce);SL(0,0x551be);
    }
    ga_project(a,e);
    int r=ga_anim_step(a,e,dome_call);
    if (r<0) return GA_UNSUPPORTED;
    if (!r) return GA_MORE;
    neo_gactor_t *p=parent(a,e);if (p) ga_sb(p,0x60,0xff);
    return GA_DELETE;
}
static ga_result_t flash_tick(neo_gactor_t *a,neo_genv_t *e) {           /* 0551E0 */
    if (L(0)==0x551e0) {
        SB(0x8c,4);SB(0x8d,0x0b);SB(0x8e,0);
        if (e->sound) e->sound(e->ctx,0x1023);
        SL(0,0x55200);
    }
    if (B(0x8e)) {
        SB(0x8e,0);if (e->flags) e->flags->flash=0;                 /* 009BB0(0) */
        SB(0x8d,B(0x8d)-1u);
        if (!B(0x8d)) return GA_DELETE;
    }
    SB(0x8c,B(0x8c)-1u);
    if (!B(0x8c)) {SB(0x8e,1);if (e->flags) e->flags->flash=1;SB(0x8c,4);}
    return GA_MORE;
}
/* ---- missile 055638 ------------------------------------------------------------ */
static void k55478(neo_gactor_t *a,uint16_t d0) {                      /* frame and zoom by height */
    if (d0>0xc000u) d0=0xc000u;
    unsigned k=(unsigned)(d0/0xec4u)&15u;
    const uint8_t *t=ga_rom(0x5545cu+2u*k);
    if (!t) return;
    SB(0x60,t[0]);SB(0x12,t[1]);SB(0x13,t[1]);
}
static neo_gactor_t *grand(neo_gactor_t *a,neo_genv_t *e) {neo_gactor_t *p=parent(a,e);return p?parent(p,e):NULL;}
static void k55574(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *m=grand(a,e);unsigned n=m?live(m):1;
    if (n==3) SW(0x80,rank(a,e,0x2d1fa4));
    if (n==2) SW(0x80,rank(a,e,0x2d1fbe));
    if (n<=1) SW(0x80,rank(a,e,0x2d1fd8));
    if (!ga_rand_range(e,5)) SW(0x80,(uint16_t)(W(0x80)<<1));
}
static void k555e6(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *m=grand(a,e);unsigned n=m?live(m):1;
    SW(0x88,rank(a,e,by_live(n,0x2d1ff2,0x2d200c,0x2d2026)));
}
static void k558e8(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *m=grand(a,e);unsigned n=m?live(m):1;
    SL(0x6e,(uint32_t)((int32_t)S16(rank(a,e,by_live(n,0x2d1f56,0x2d1f70,0x2d1f8a)))<<8));
}
/* 0366AC: angle toward the nearest player (+0x20 above its +40). */
static bool aim(neo_gactor_t *a,neo_genv_t *e,uint8_t *angle) {
    uint8_t mask=e->players.active_mask&3u;
    if (!mask) return false;
    uint16_t px=e->players.p1x,py=e->players.p1y;
    if (mask==2u) {px=e->players.p2x;py=e->players.p2y;}
    else if (mask==3u) {      /* 01AF9E: the horizontally nearest, P1 on ties */
        int16_t d1=(int16_t)(e->players.p1x-W(0x3c)),d2=(int16_t)(e->players.p2x-W(0x3c));
        if (d1<0) d1=(int16_t)-d1;
        if (d2<0) d2=(int16_t)-d2;
        if (d2<d1) {px=e->players.p2x;py=e->players.p2y;}
    }
    int16_t dx=(int16_t)(px-W(0x3c)),dy=(int16_t)(py+0x20u-W(0x40));
    *angle=(uint8_t)neo_angle(dx,dy);return true;
}
static bool low(neo_gactor_t *a) {return S16(W(0x40))<0x140;}            /* 05588C: carry */
static void k558a2(neo_gactor_t *a) {
    uint32_t d0=L(0x6e);
    if (low(a) && (B(0x74)&0x80u)) d0>>=1;
    uint32_t vx,vy;ga_polar(d0,B(0x74),&vx,&vy);SL(0x30,vx);SL(0x34,vy);
    if (B(0x74)&0x80u) SL(0x34,L(0x34)+0xffffb000u);
}
static void k55820(neo_gactor_t *a,neo_genv_t *e) {
    SW(0x72,W(0x72)+1u);
    if (!(S16(W(0x72))>0x320)) {
        SW(0x80,W(0x80)-1u);
        if (!(S16(W(0x80))>0)) {
            k55574(a,e);
            uint8_t angle;
            if (low(a)) SB(0x76,0x40); else if (aim(a,e,&angle)) SB(0x76,angle);
        }
    }
    if (!(W(0x72)&7u)) {int8_t d=(int8_t)(B(0x76)-B(0x74));SB(0x75,d<0?(uint8_t)-6:6);}
    SB(0x74,B(0x74)+B(0x75));
    k558a2(a);
}
static ga_result_t missile_tick(neo_gactor_t *a,neo_genv_t *e) {
    if (L(0)==0x55638) {
        ga_palette(a,0x477);ga_flash_palette(a,0x883);CLR(0x15,2);SET(0x15,5);SW(0x1e,W(0x1e)+1u);
        SB(0x5e,B(0x5e)|0x80u);                                       /* 004C1C */
        START(0x24a910);if (e->sound) e->sound(e->ctx,0x10a3);SL(0,0x55686);
    }
    if (L(0)==0x55686) {
        SB(0x74,B(0x74)+6u);
        if (B(0x74)<0xc0u) {
            k55478(a,(uint16_t)(B(0x74)<<8));
            uint32_t vx,vy;ga_polar(0x20000,(uint8_t)(0xc0u+B(0x74)),&vx,&vy);
            int32_t x=(int32_t)vx;if (x<0) x=-x;if (B(0x15)&1u) x=-x;
            SL(0x30,(uint32_t)x);SL(0x34,0xfffff000u);ga_move(a,e);
            if (ga_anim_step(a,e,dome_call)<0) return GA_UNSUPPORTED;
            return GA_MORE;
        }
        START(0x24aa18);SW(0x66,0x1110);SW(0x1e,0x1110);SW(0x4e,rank(a,e,0x2d1d0e));SL(0x50,0x54748);
        k558e8(a,e);k55574(a,e);
        uint8_t d0=0;if (B(0x15)&1u) {CLR(0x15,0);d0=0x80;}
        SB(0x74,d0);SB(0x12,0xff);SB(0x13,0xff);
        SW(0x72,ga_rand_range(e,8)&7u);SB(0x75,0);
        uint8_t angle;SB(0x76,aim(a,e,&angle)?angle:B(0x76));
        SB(0x5a,0);k555e6(a,e);SL(0,0x55772);
    }
    neo_gactor_t *m=grand(a,e);
    bool end=(m && (ga_b(m,0x5f)&0x80u));
    if (!end) {SW(0x88,W(0x88)-1u);end=(W(0x88)&0x8000u)!=0;}
    if (!end) end=BIT(0x15,6);
    if (!end) {
        k55820(a,e);
        SB(0x60,(uint8_t)((uint8_t)(B(0x74)+8u)>>4));
        if (ga_cull(a,e,0x54862)) return GA_DELETE;
        if (ga_attack(a,e,0x54754)) end=true;
        else {
            register_hit(a,e);
            if (BIT(0x5f,7)) end=true;
            else {ga_air(a,e);if (ga_anim_step(a,e,dome_call)<0) return GA_UNSUPPORTED;return GA_MORE;}
        }
    }
    if (e->sound) e->sound(e->ctx,0x1022);
    ga_spawn_from(a,e,0x1c6ee,0,0);
    return GA_DELETE;
}
/* ---- animation callbacks (op 0x14) ------------------------------------------------ */
static bool dome_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    switch (routine) {
    case 0x55364: child(a,e,0x55056);return true;
    case 0x55388: child(a,e,0x5518a);SB(0x60,0);return true;
    case 0x55448: {neo_gactor_t *p=parent(a,e);if (p) ga_sb(p,0x60,0xff);return true;}
    case 0x55454: SL(0,0x54e0e);return true;
    case 0x553d2: {
        neo_gactor_t *p=parent(a,e);
        neo_gactor_t *c=e->spawn?e->spawn(e->ctx,p?p:a,0x55638,false):NULL;   /* 00411C after the dome */
        if (!c) return true;
        ga_place_from(c,a,e,0,0);
        unsigned k=((unsigned)W(0x82)-1u)&7u;
        ga_sw(c,0x28,(uint16_t)(ga_w(c,0x28)+rw(0x553b2u+4u*k)));ga_sw(c,0x2c,(uint16_t)(ga_w(c,0x2c)+rw(0x553b4u+4u*k)));
        if (!(W(0x82)&1u)) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|1u));
        if (p && ga_w(p,0x6e)==2u) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|1u));
        if (p && ga_w(p,0x6e)==1u) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)&~1u));
        return true;
    }
    case 0x55a6c: SL(0x62,L(0x80));if (!(B(0x15)&1u)) SL(0x62,0u-L(0x62));return true;
    default: return false;
    }
}
bool neo_dome_routine(uint32_t r) {
    return r==0x54904 || (r>=0x549b8 && r<=0x54b0a) || (r>=0x54b1c && r<=0x54bf0) || (r>=0x54c2c && r<=0x54f40) ||
        r==0x54f60 || r==0x54f9e || r==0x55056 || r==0x5509e || r==0x5518a || r==0x551be || r==0x551e0 || r==0x55200 ||
        r==0x55638 || r==0x55686 || r==0x55772;
}
ga_result_t neo_dome_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=L(0);
    if (r==0x54904 || (r>=0x549b8 && r<=0x54b0a)) return main_tick(a,e);
    if (r>=0x54b1c && r<=0x54bf0) return banner_tick(a,e);
    if (r>=0x54c2c && r<=0x54f40) return dome_tick(a,e);
    if (r==0x54f60 || r==0x54f9e) return hatch_tick(a,e);
    if (r==0x55056 || r==0x5509e) return launcher_tick(a,e);
    if (r==0x5518a || r==0x551be) return ender_tick(a,e);
    if (r==0x551e0 || r==0x55200) return flash_tick(a,e);
    return missile_tick(a,e);
}
