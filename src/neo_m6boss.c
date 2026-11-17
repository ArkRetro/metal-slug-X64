#include "neo_region.h"
#if MSX_M6BOSS
#include "neo_soldier_ai.h"   /* neo_angle (ROM 002EB4) */
/* Mission 6 stage 3: the last enemy 04153E (script record 2E19F8, camera 1),
 * manual translation routine by routine; ROM labels are provenance only.
 * Captures: captures-m6/fam/M1.. (runM.sh: run4 replay from sec11_322339 by
 * windows), docs/MISION6.md "Etapa 3".
 *
 * The tree. 04153E (the hub: 0C8CD6, +86..+88 and RAM 10C448 cleared) makes
 * the body 04157A (+72 = the hub) and stays at 041578. The hub's bytes are
 * the orders: +6E..+72 one per part (bit 7: done), +74 the shake offset,
 * +7E the program speed, +80 a delay, +86 bits (0 the second form, 1 the
 * end), +87 / +88 the glow / the arm, +8A..+8D the shake (041F1C), +8E the
 * mute of 0444E8.
 * The body 04157A: 0418D2 (the parts 042BDC / 0431E0 / 042E22 / 04307E /
 * 0444E8 with +83 = 0..4 and +84 = FF, the layer 10D7B6 as the body's
 * picture: 0C6D04, 0C702A, 0C703C, at (2B0, 108) of the root by 01AB9C, owner
 * 3 by 0C7BD0, +82 bits 1 / 2 off and 3 on, the body on the layer at (0, 0));
 * then the opening 045428 and the wait for its +60.
 * The body's tail 041F1C: 0446E8 (+86 / +8A by +81) or 0446F0 (+28 / +2C),
 * 01AC38, +86 += +8E, +8A += +92, the layer's speed 694E / 6952 = the
 * position less the layer's (plus the hub's shake +74 when +8D bit 1),
 * 0421E4 with the hub's +86 bit 0, the program speed +26 by 2 toward the
 * hub's +7E, 01ABC8 and 00427A with the shake on +28 / +2C, 004C24 (+5F bit 7:
 * the hub's +86 bit 1; 041634 or, in the second form, 10C448 = FF and
 * 04180A).
 * The parts end in 044860: a new order in the hub's byte +6E[+83] (against
 * +84) picks the routine of the table 0453A6[+83]; +80 is the body's. 04489E
 * marks the order done (bit 7).
 * The opening 045428 (at (200, A0) of the root): eight actors 045608 from the
 * list 045DBE (each with its timeline +86), the timeline 045D84 (pairs of
 * (cue, duration)) in +8A / +8C, the players walked by 0CA324 to x 180 / 170
 * (handler 0CA386, which clears itself on arrival) and then held by 0455E6
 * (no input); 10E9B8 kept in +8E and set to 8; 019388 (20000). At the end of
 * the timeline 10C00B / 10C00C = 0, the camera 695C = 0, 0C7B48(0), the
 * body's +60 = 1, 0455EA (10E9B8 back, 0CA2F2 x2, 019388(-1)) and 0041AC.
 * The actors 045608 run their timeline: when the opening's +8A equals the
 * next word, +8C = that word and the routine after it becomes +00. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define BIT(o,b) ((B(o)>>(b))&1u)
#define SET(o,b) SB(o,B(o)|(1u<<(b)))
#define CLR(o,b) SB(o,B(o)&~(1u<<(b)))
#define S8(v) ((int8_t)(uint8_t)(v))
#define S16(v) ((int16_t)(uint16_t)(v))
#define S32(v) ((int32_t)(uint32_t)(v))
#define STOP(at) do { ga_fail(e,(at)); return GA_UNSUPPORTED; } while (0)
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
#define STEP() fb_step(a,e)
#define LAYER 0x10d7b6u
#define ROOT 0x10e968u
#define P1_NODE 0x1014c4u
#define P2_NODE 0x101574u
static uint8_t rb(neo_genv_t *e,uint32_t at) {const uint8_t *p=ga_rom(at);if (!p) {ga_fail(e,at);return 0;}return p[0];}
static uint16_t rw(neo_genv_t *e,uint32_t at) {const uint8_t *p=ga_rom(at);if (!p) {ga_fail(e,at);return 0;}return (uint16_t)(p[0]<<8|p[1]);}
static uint32_t rl(neo_genv_t *e,uint32_t at) {return (uint32_t)rw(e,at)<<16|rw(e,at+2);}
static void sound(neo_genv_t *e,uint16_t id) {if (e->sound) e->sound(e->ctx,id);}   /* 0120F8 */
static void stop_sound(neo_genv_t *e,uint16_t id) {sound(e,(uint16_t)(id|0x8000u));}   /* 012192 */
static uint16_t rank(neo_gactor_t *a,neo_genv_t *e,uint32_t table) {   /* 0260C8 */
    const uint8_t *t=ga_rom(table);
    if (!t) {ga_fail(e,table);return 0;}
    return ga_rank_word(a,e,t);
}
static neo_gactor_t *node(neo_genv_t *e,uint32_t tag) {
    neo_gactor_t *n=e->actor?e->actor(e->ctx,tag):NULL;
    if (!n) ga_fail(e,0x10000000u|(tag&0xffffffu));
    return n;
}
#define HUB() neo_gactor_t *h=node(e,L(0x72));if (!h) return GA_UNSUPPORTED
static uint32_t self(const neo_gactor_t *a) {return NEO_GA_TAG(a->index);}
static neo_gactor_t *creator(neo_gactor_t *a,neo_genv_t *e) {neo_gactor_t *c=e->creator?e->creator(e->ctx,a):NULL;if (!c) ga_fail(e,0x4000cu);return c;}
/* 0040BC with the reserve D0 (NULL: no node). */
static neo_gactor_t *child(neo_gactor_t *a,neo_genv_t *e,uint32_t routine,uint16_t reserve) {
    return e->spawn_reserve?e->spawn_reserve(e->ctx,a,routine,reserve):e->spawn?e->spawn(e->ctx,a,routine,false):NULL;
}
/* 0040BC (reserve E) and the copy of +6E, +72 and +80 into the new node (the boss's children). */
static neo_gactor_t *part(neo_gactor_t *a,neo_genv_t *e,uint32_t routine) {
    neo_gactor_t *c=child(a,e,routine,0xe);
    if (c) {ga_sl(c,0x6e,L(0x6e));ga_sl(c,0x72,L(0x72));ga_sb(c,0x80,B(0x80));}
    return c;
}
static neo_gactor_t *after_script(neo_gactor_t *a,neo_genv_t *e,uint32_t routine) {   /* 00411C (A1 = 101414) */
    return e->spawn?e->spawn(e->ctx,a,routine,true):NULL;
}
static void on_creator(neo_gactor_t *a,neo_genv_t *e,int16_t dx,int16_t dy) {   /* 01AD6E */
    neo_gactor_t *p=creator(a,e);ga_place(a,e,dx,dy,p?self(p):0);
}
/* 01DA40: the smoke list emitter 01D764 (+86 = list) at the node. */
static neo_gactor_t *k1da40(neo_gactor_t *a,neo_genv_t *e,uint32_t list) {
    neo_gactor_t *c=child(a,e,0x1d764,0x1c);
    if (!c) return NULL;
    ga_place_from(c,a,e,0,0);
    if (B(0x15)&4u) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|4u));
    ga_sl(c,0x86,list);
    return c;
}
/* 0044F6: the creator's flash timer (+17); carry (true) on even values. */
static bool share_flash(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *p=creator(a,e);
    uint8_t d0=p?ga_b(p,0x17):0;
    if (!d0) return false;
    if (!(d0&0x80u)) d0++;
    SB(0x17,d0);
    return !(d0&1u);
}
/* 009CCE on a hardware palette slot: the fade of the slot's colours is the
 * renderer's (RAM 1068EA); counted as external until it is modelled. */
static void k9cce(neo_genv_t *e,uint8_t slot,uint16_t target,uint8_t weight) {
    (void)slot;(void)target;(void)weight;
    if (e->external) e->external(e->ctx,0x9cce);
}
static neo_m4boss_env_t *lenv(neo_genv_t *e) {if (!e->m4boss || !e->m4boss->layer) {ga_fail(e,LAYER);return NULL;}return e->m4boss;}
static uint32_t ll(const uint8_t *l,unsigned o) {return (uint32_t)(l[o]<<24|l[o+1]<<16|l[o+2]<<8|l[o+3]);}
static int fb_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter);
static bool nearest(neo_gactor_t *a,const neo_genv_t *e,uint32_t *n);
static bool fb_call_b(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {return fb_call(a,e,kind,routine,parameter)>0;}
static int fb_step(neo_gactor_t *a,neo_genv_t *e) {return ga_anim_step(a,e,fb_call_b);}   /* 00427A: 1 at the end (carry) */
/* ---- the hub's helpers (A1 = +72) ------------------------------------------------- */
static void k44666(neo_gactor_t *h) {ga_sb(h,0x8a,0);ga_sb(h,0x8b,0);ga_sb(h,0x8c,0);ga_sb(h,0x8d,0);ga_sl(h,0x74,0);}
static __attribute__((unused)) void k44682(neo_gactor_t *h) {ga_sb(h,0x8b,6);ga_sb(h,0x8c,0xa);ga_sb(h,0x8d,(uint8_t)((ga_b(h,0x8d)&~1u)|2u));}
/* 04489E: the part's order done (bit 7; D0 < 0 keeps the order). */
static void k4489e(neo_gactor_t *a,neo_gactor_t *h,int16_t d0) {
    unsigned d3=B(0x83);uint8_t d1=ga_b(h,0x6e + d3),d2=B(0x84)&0x7fu;
    if (d1!=d2) return;
    uint8_t v=d0<0?d2:(uint8_t)d0;
    ga_sb(h,0x6e + d3,(uint8_t)(v|0x80u));SB(0x84,d2|0x80u);
}
/* 044860: a new order picks the part's routine (table 0453A6); +80 = the body's. */
static ga_result_t k44860(neo_gactor_t *a,neo_genv_t *e) {
    HUB();
    unsigned d0=B(0x83);uint8_t d1=ga_b(h,0x6e + d0);
    if (d1!=B(0x84)) {
        SB(0x84,d1);
        uint32_t t=rl(e,0x453a6u+4u*d0);
        SL(0,rl(e,t+4u*(d1&0x7fu)));
    }
    neo_gactor_t *m=node(e,L(0x6e));
    if (!m) return GA_UNSUPPORTED;
    SB(0x80,ga_b(m,0x80));
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* 04460A / 04463E: the rank table index: 4 in the second form or with the
 * body's +4E at or under its +9A, plus 8 when a live player rides (044B84). */
static bool k44b84(neo_genv_t *e) {
    for (unsigned p=0;p<2;p++) {
        if (!((e->players.active_mask>>p)&1u)) continue;              /* 01AEB0 */
        if (!e->vehicle || !e->vehicle->slots || !e->vehicle->exists[p] || !neo_vslot_node(&e->vehicle->slots->slot[p])) continue;
        uint8_t k=e->vehicle->slots->slot[p].b[0x21];                  /* 0AAA88 */
        if (!k) {ga_fail(e,0xaaacc);return false;}
        return true;
    }
    return false;
}
static int k4463e(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *h=node(e,L(0x72)),*m=node(e,L(0x6e));
    if (!h || !m) return 0;
    unsigned d0=(ga_b(h,0x86)&1u)?4u:(S16(ga_w(m,0x4e))<=S16(ga_w(m,0x9a))?4u:0u);
    return (int)(d0+(k44b84(e)?8u:0u));
}
/* 0446D4: the move order of node `n` (+7A, +7C point, +7E speed, +81 mode). */
static void k446d4(neo_gactor_t *n,uint16_t d0,uint16_t d1,uint16_t d2,uint8_t d3) {
    ga_sw(n,0x7a,d0);ga_sw(n,0x7c,d1);ga_sw(n,0x7e,d2);ga_sb(n,0x81,d3);
}
/* 0446E8 (block +86, the body's picture) / 0446F0 (block +28): the move by +81. */
static void k446f4(neo_gactor_t *a,unsigned b) {
    uint8_t d0=B(0x81);
    if (d0&0x80u) {
        if (!B(0x82)) {SB(0x81,d0&0x7fu);SB(0x82,0);} else SB(0x82,B(0x82)-1u);
        SL(b+8,0);SL(b+0xc,0);return;
    }
    if (d0==0) return;
    if (d0!=4 && d0!=8 && d0!=0xc) return;   /* the jump table 044730 has four entries */
    int16_t dx=(int16_t)(W(0x7a)-W(b)),dy=(int16_t)(W(0x7c)-W(b+4));
    uint8_t angle=neo_angle(dx,dy);
    uint32_t vx,vy;
    if (d0==4) ga_polar((uint32_t)W(0x7e)<<8,angle,&vx,&vy);           /* 044742 */
    else {                                                              /* 0447B4 */
        uint16_t ax=(uint16_t)(dx<0?-dx:dx),ay=(uint16_t)(dy<0?-dy:dy);
        int16_t d2=(int16_t)(W(0x7e)-0x40);
        if (d2<0x40) d2=0x40;
        uint32_t p=(uint32_t)(uint16_t)(ax+ay)*(uint16_t)d2;
        p>>=7;
        uint16_t s=(uint16_t)((uint16_t)p+0x40u);
        if (d0==0xc && s>=W(0x7e)) s=W(0x7e);
        ga_polar((uint32_t)(int32_t)(int16_t)s<<8,angle,&vx,&vy);
    }
    uint32_t d4=vx-((uint32_t)(uint16_t)dx<<16);
    if (!(S32(d4^vx)<0)) {vx=0;SL(b,L(0x7a));}
    uint32_t d5=vy-((uint32_t)(uint16_t)dy<<16);
    if (!(S32(d5^vy)<0)) {vy=0;SL(b+4,L(0x7c));}
    SL(b+8,vx);SL(b+0xc,vy);
    if (!(vx|vy)) SB(0x81,0);
}
/* ---- the hub 04153E --------------------------------------------------------------- */
static ga_result_t hub_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0x4153e: {
        if (e->flags) {e->flags->player_lock=0xff;e->flags->flash_request=0xff;}   /* 0C8CD6 */
        SB(0x86,0);SB(0x87,0);SB(0x88,0);
        if (e->flags) e->flags->m6_boss=0;                                       /* 10C448 */
        neo_gactor_t *c=child(a,e,0x4157a,0xe);
        if (c) {ga_sl(c,0x72,self(a));ga_place_at(c,a,e,0,0);}                     /* 01ACF2 */
        SL(0,0x41578);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x41578: return GA_MORE;
    }
    STOP(L(0));
}
/* ---- the body 04157A ---------------------------------------------------------------- */
static void k418d2(neo_gactor_t *a,neo_genv_t *e) {
    SL(0x6e,self(a));
    SW(0x7e,0);SB(0xa0,0);SB(0x80,0);SL(0x30,0);SL(0x34,0);SL(0x8e,0);SL(0x92,0);
    neo_gactor_t *h=node(e,L(0x72));
    if (!h) return;
    ga_sw(h,0x7e,0x100);                                                    /* 04491C */
    SB(0xa3,0);SL(0x30,0);SL(0x34,0);
    k44666(h);
    SB(0x81,0);CLR(0x15,2);SL(0x50,0);SB(0xa5,0);
    ga_sl(h,0x78,0x2c7918);
    ga_project(a,e);
    SW(0x66,0x12c);SW(0x1e,0x12c);
    ga_palette(a,0xd7);
    if (!ga_anim_start(a,e,0x2415f8,0x100)) return;
    static const uint32_t parts[5]={0x42bdc,0x431e0,0x42e22,0x4307e,0x444e8};
    for (unsigned i=0;i<5;i++) {
        neo_gactor_t *c=part(a,e,parts[i]);
        if (c) {ga_sb(c,0x83,(uint8_t)i);ga_sb(c,0x84,0xff);}
    }
    ga_sb(h,0x6e,0);ga_sb(h,0x6f,0);ga_sb(h,0x70,0);ga_sb(h,0x71,0);ga_sb(h,0x72,0);ga_sb(h,0x8e,0);
    neo_m4boss_env_t *m=lenv(e);
    if (!m) return;
    m->speed[0]=0;m->speed[1]=0;
    uint8_t *l=m->layer;
    l[0x82]&=0xfeu;                                                        /* 0C6D04 */
    if (m->clear) m->clear(e->ctx);                                        /* 0C702A, 0C703C */
    neo_layerobj_place(e,l,0x2b0,0x108);                                   /* 01AB9C */
    l[0x82]=(uint8_t)((l[0x82]&~6u)|8u);
    m->owner=3;                                                            /* 0C7BD0 */
    ga_place(a,e,0,0,LAYER);                                               /* 01AB98 */
    SL(0x86,ll(l,0));SL(0x8a,ll(l,4));
}
/* 041F1C: the body's tail (above). */
static ga_result_t body_tail(neo_gactor_t *a,neo_genv_t *e) {
    HUB();
    neo_m4boss_env_t *m=lenv(e);
    if (!m) return GA_UNSUPPORTED;
    k446f4(a,B(0xa3)?0x28u:0x86u);
    ga_move(a,e);                                                           /* 01AC38 */
    SL(0x86,L(0x86)+L(0x8e));SL(0x8a,L(0x8a)+L(0x92));
    uint32_t d0=L(0x86)-ll(m->layer,0),d1=L(0x8a)-ll(m->layer,4);
    uint8_t d2=ga_b(h,0x8a),d3=ga_b(h,0x8b);
    ga_sb(h,0x8a,(uint8_t)(d2+d3));
    if (d2) ga_sl(h,0x74,(uint32_t)((int32_t)ga_sine(d2)<<(ga_b(h,0x8c)&0x1fu)));   /* table 0029FC, asl.l */
    if (ga_b(h,0x8d)&2u) {if (ga_b(h,0x8d)&1u) d0+=ga_l(h,0x74); else d1+=ga_l(h,0x74);}
    m->speed[0]=(int32_t)d0;m->speed[1]=(int32_t)d1;                       /* 694E / 6952 */
    if (ga_b(h,0x86)&1u) {   /* 0421E4: every 12 frames the next of 10C / 10D / 10E (DA while flashing) on slot 38 */
        if (!(e->video%12u)) SB(0xa2,B(0xa2)+1u);
        static const uint16_t pal[3]={0x10c,0x10d,0x10e};
        uint16_t d1=pal[B(0xa2)%3u];
        if (B(0x17) && !(B(0x17)&1u)) d1=0xda;
        k9cce(e,0x38,d1,0x20);
    }
    uint16_t sp=ga_w(h,0x7e);                                              /* 044926 */
    if (sp!=W(0x26)) SW(0x26,sp<W(0x26)?W(0x26)-2u:W(0x26)+2u);
    uint32_t x=L(0x28),y=L(0x2c);
    if (!(ga_b(h,0x8d)&2u)) {if (ga_b(h,0x8d)&1u) SL(0x28,x+ga_l(h,0x74)); else SL(0x2c,y+ga_l(h,0x74));}
    ga_project(a,e);
    if (STEP()<0) return GA_UNSUPPORTED;
    SL(0x2c,y);SL(0x28,x);
    int16_t dx,dy;
    if (ga_register(a,e,&dx,&dy)) {
        SB(0x5a,0);
        if (BIT(0x5f,7)) {
            ga_sb(h,0x86,(uint8_t)(ga_b(h,0x86)|2u));
            if (!(ga_b(h,0x86)&1u)) {CLR(0x5f,7);SL(0,0x41634);}
            else {if (e->flags) e->flags->m6_boss=0xff;SL(0,0x4180a);}
        }
    }
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* 01AC4C: the block +28..+37 re-expressed from its parent +38 in `to` (previous positions for +28 / +2C). */
static void to_parent(neo_gactor_t *a,neo_genv_t *e,uint32_t to) {
    uint32_t from=L(0x38);
    if (from==to) return;
    neo_contact_parent_t op,np;
    uint32_t d0=L(0x28),d1=L(0x2c),d2=L(0x30)+d0,d3=L(0x34)+d1;
    if (from) {if (!ga_parent(e,from,&op)) {ga_fail(e,0x1ac4c);return;}d0+=(uint32_t)op.previous_x;d1+=(uint32_t)op.previous_y;d2+=(uint32_t)op.x;d3+=(uint32_t)op.y;}
    if (to) {if (!ga_parent(e,to,&np)) {ga_fail(e,0x1ac4c);return;}d0-=(uint32_t)np.previous_x;d1-=(uint32_t)np.previous_y;d2-=(uint32_t)np.x;d3-=(uint32_t)np.y;}
    SL(0x28,d0);SL(0x2c,d1);SL(0x30,d2-d0);SL(0x34,d3-d1);SL(0x38,to);
}
/* 042284: the hub's +7C (the wait before the next move) = rank 045006[04463E]. */
static void k42284(neo_gactor_t *a,neo_genv_t *e,neo_gactor_t *h) {ga_sw(h,0x7c,rank(a,e,rl(e,0x45006u+(uint32_t)k4463e(a,e))));}
/* 0422C8: with no move, a random point (1F0..23F, A0..CF) at a random speed 100..F00, mode C. */
static void k422c8(neo_gactor_t *a,neo_genv_t *e) {
    if (B(0x81)) return;
    uint16_t d2=(uint16_t)((ga_rand(e)&0xf00u)+0x100u);
    uint16_t d1=ga_rand_range(e,0x30);
    uint16_t d0=ga_rand_range(e,0x50);
    k446d4(a,(uint16_t)(d0+0x1f0u),(uint16_t)(d1+0xa0u),d2,0xc);
}
/* 0422A0: carry when the hub's +7C is spent and its shake angle (+8A & 7E) is 0. */
static bool k422a0(neo_gactor_t *h) {
    if (ga_w(h,0x7c)) {ga_sw(h,0x7c,ga_w(h,0x7c)-1u);return false;}
    return !(ga_b(h,0x8a)&0x7eu);
}
/* 019DCA by 04465E: the live count of group D0 - 3. */
static uint8_t k4465e(neo_genv_t *e,uint8_t d0) {d0=(uint8_t)(d0-3u);return d0<9u && e->groups?e->groups[d0&0xfu]:0;}
/* 0431B6: rank 0451FE[04463E] less the live count of group 5 (not below 0). */
static uint8_t k431b6(neo_gactor_t *a,neo_genv_t *e) {
    uint16_t d1=rank(a,e,rl(e,0x451feu+(uint32_t)k4463e(a,e)));
    int8_t v=(int8_t)((uint8_t)d1-k4465e(e,8));
    return v<0?0:(uint8_t)v;
}
/* A word of a player node (1014C4 / 101574). */
static uint16_t pw(neo_genv_t *e,uint32_t n,unsigned o) {
    if (o==0x3c) return n==P2_NODE?e->players.p2x:e->players.p1x;
    if (o==0x40) return n==P2_NODE?e->players.p2y:e->players.p1y;
    const uint8_t *m=e->node?e->node(e->ctx,n):NULL;
    if (!m) {ga_fail(e,n);return 0;}
    return (uint16_t)(m[o]<<8|m[o+1]);
}
/* 01B04A: player 1, 2 or (both live) one at random; carry (true) when none. */
static bool k1b04a(neo_genv_t *e,uint32_t *n) {
    unsigned d0=e->players.active_mask&0xffu;
    if (d0>=3u) d0=(ga_rand(e)&1u)+1u;
    *n=d0==2u?P2_NODE:P1_NODE;
    return d0==0;
}
/* 042308: the next attack +80 by the player's place (0423AA, table 044FCA) and the rank (04463E); 4..6 need their group
 * empty, else 2 or 3 by a random sign. */
static void k42308(neo_gactor_t *a,neo_genv_t *e) {
    SB(0x9f,0);
    uint32_t n;
    if (k1b04a(e,&n)) {SB(0x80,0);return;}
    unsigned i;
    if (S16(pw(e,n,0x28))>0x190) i=2; else i=S16(pw(e,n,0x2c))<0x60?1:0;
    uint32_t t=rl(e,0x44fcau+4u*i);
    uint8_t d0=(uint8_t)rank(a,e,rl(e,t+(uint32_t)k4463e(a,e)));
    if (d0==4u || d0==5u || d0==6u) {
        if (k4465e(e,4) || k4465e(e,5) || k4465e(e,6)) d0=S16(ga_rand(e))<0?3:2;
    }
    SB(0x80,d0);
}
/* 0423D8: the second form's attack: 7, or 8 while 0431B6 allows a trooper. */
static void k423d8(neo_gactor_t *a,neo_genv_t *e) {
    SB(0x9f,0);
    if (B(0x80)==8u) {SB(0x80,7);return;}
    SB(0x80,k431b6(a,e)?8:7);
}
static uint32_t step_at(neo_gactor_t *a,neo_genv_t *e) {return rl(e,rl(e,0x44eccu+4u*B(0x80))+4u*B(0x9f));}
static void k42440(neo_gactor_t *a,neo_genv_t *e) {if (step_at(a,e)!=0xffffffffu) SB(0x9f,B(0x9f)+1u);}   /* the next step */
static void k4246e(neo_gactor_t *a,neo_genv_t *e,uint32_t r) {   /* +9F = the step `r` of the list */
    uint32_t t=rl(e,0x44eccu+4u*B(0x80));
    for (uint8_t i=0;i<64;i++) if (rl(e,t+4u*i)==r) {SB(0x9f,i);return;}
    ga_fail(e,0x4246e);
}
static void k446cc(neo_gactor_t *a) {SB(0x81,0);}
static void k446a0(neo_gactor_t *h) {ga_sb(h,0x8b,0x40);ga_sb(h,0x8c,8);ga_sb(h,0x8d,(uint8_t)((ga_b(h,0x8d)|1u)&~2u));}
/* 0A6918: D0 * (D2 - D3) / D1 + D3 (with D0's sign; 0 stays 0). */
static int16_t ka6918(neo_genv_t *e,int16_t d0,uint16_t d1,uint16_t d2,uint16_t d3) {
    if (d2<d3) {ga_fail(e,0xa691e);return 0;}                           /* TRAP #15 */
    int16_t d4=d0;uint16_t v=(uint16_t)(d0<0?-d0:d0);
    uint32_t p=(uint32_t)v*(uint16_t)(d2-d3);
    if (!d1) {ga_fail(e,0xa693e);return 0;}
    uint32_t q=p/d1;
    if (q>0xffffu) {ga_fail(e,0xa693e);return 0;}                        /* divu overflow */
    uint16_t r=(uint16_t)q;
    if (!r) return 0;
    r=(uint16_t)(r+d3);
    return d4<0?(int16_t)-r:(int16_t)r;
}
static bool k4240a(neo_gactor_t *a,neo_genv_t *e);
static bool k42aac(neo_gactor_t *a,neo_genv_t *e) {   /* 044F9C[+80]: carry: the attack may end */
    uint32_t r=rl(e,0x44f9cu+4u*B(0x80));
    if (r==0x42ac0u) return true;
    if (r!=0x42ac6u) {ga_fail(e,r);return false;}
    neo_gactor_t *h=node(e,L(0x72)),*m=node(e,L(0x6e));
    if (!h || !m) return false;
    if ((ga_b(h,0x86)&1u) || S16(ga_w(m,0x4e))<=S16(ga_w(m,0x9a))) return true;   /* 04460A */
    return !k4465e(e,B(0x80));
}
static void k4249e(neo_gactor_t *a,neo_genv_t *e) {if (k4240a(a,e)) k423d8(a,e);}
/* 0424AC: at the health of the hub's list +78 (2C7918: word, record), a sub-script actor 0424F8 after the script actor. */
static void k424ac(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *h=node(e,L(0x72));
    if (!h) return;
    uint32_t p=ga_l(h,0x78);
    if (S16(rw(e,p))<S16(W(0x4e))) return;
    neo_gactor_t *c=after_script(a,e,0x424f8);
    uint32_t r=rl(e,p+2u);
    ga_sl(h,0x78,p+6u);
    uint8_t d0=(uint8_t)(B(0x85)+1u);SB(0x85,d0);
    if (c) {ga_sl(c,0x86,r);ga_sb(c,0x85,d0);}
}
/* 0448D4: the camera glide 04445A to (D0, D1) at the speeds (D2, D3); +60 = 0. */
static void k448d4(neo_gactor_t *a,neo_genv_t *e,uint16_t d0,uint16_t d1,uint16_t d2,uint16_t d3) {
    neo_gactor_t *c=part(a,e,0x4445a);
    if (c) {ga_sw(c,0x86,d0);ga_sw(c,0x88,d1);ga_sw(c,0x8a,d2);ga_sw(c,0x8c,d3);}
    SB(0x60,0);
}
/* 041EE8: the layer 10D7B6 at (D0, D1) of the root (01AB9C); +86 / +8A its position. */
static void k41ee8(neo_gactor_t *a,neo_genv_t *e,int16_t x,int16_t y) {
    neo_m4boss_env_t *m=lenv(e);
    if (!m) return;
    neo_layerobj_place(e,m->layer,x,y);
    SL(0x86,ll(m->layer,0));SL(0x8a,ll(m->layer,4));
}
/* 041B34: the fight: music 2C, 0C8CE6, the hurtbox 044E00, health rank 2C40E6 (+4E) and 2C4148 (+9A, the second
 * threshold), flash palette DA, 042284, the hub's +72 = 4 and +8E = 0, the layer at (200, A0). */
static void k41b34(neo_gactor_t *a,neo_genv_t *e,neo_gactor_t *h) {
    sound(e,0x2c);
    if (e->flags) e->flags->player_lock=0;                                  /* 0C8CE6 */
    SL(0x50,0x44e00);SB(0x5a,0);CLR(0x5f,7);
    ga_sb(h,0x86,ga_b(h,0x86)&~2u);
    SW(0x4e,rank(a,e,0x2c40e6));SW(0x9a,rank(a,e,0x2c4148));
    ga_flash_palette(a,0xda);
    k42284(a,e,h);
    ga_sb(h,0x72,4);ga_sb(h,0x8e,0);
    k41ee8(a,e,0x200,0xa0);
    SL(0x8e,0);SL(0x92,0);
}
/* 041BCE: the first form beaten: the parts' orders, the fade, the white screen and the kills (0443BE). */
static void k41bce(neo_gactor_t *a,neo_genv_t *e,neo_gactor_t *h) {
    ga_sb(h,0x6e,4);ga_sb(h,0x6f,2);ga_sb(h,0x70,0);ga_sb(h,0x71,0);
    k446cc(a);
    SL(0x8e,0);SL(0x92,0);SL(0x30,0);SL(0x34,0);SL(0x50,0);SB(0x5a,0);CLR(0x5f,7);
    k44666(h);
    ga_sb(h,0x72,1);ga_sb(h,0x8e,0);
    neo_gactor_t *c=part(a,e,0x443be);
    if (c) ga_sw(c,0x86,0x3c);
    neo_gactor_t *w=after_script(a,e,0xae1dc);                               /* 0AE220 (1, 0) */
    if (w) {ga_sb(w,0x70,0);ga_sw(w,0x6e,1);}
    k446a0(h);
    sound(e,(uint16_t)(0x0a00u|(uint8_t)(0x20u+0x20u)));                    /* 01228C (0x20) */
}
/* 041E02: the layer's map 04522E (columns 04525A, 0C6F34), its 1A x 8 blocks (0C73CE), 0C6CFC. */
static void k41e02(neo_genv_t *e) {
    neo_m4boss_env_t *m=lenv(e);
    if (!m) return;
    uint8_t *l=m->layer;
    uint32_t map=0x4522eu;
    if (!ga_rom(map)) {ga_fail(e,map);return;}
    neo_layerobj_map(l,map,0,0,0x4525au);
    neo_layerobj_blocks(e,l,m->draw,0,0,0x1a,8);
    l[0x82]|=0x81u;
}
/* 041C78: the second form begins: 0C8CD6, the hub's +86 bit 0, +15 bit 2, the body in the root (01AD52), the move to
 * (1AB, E0), the layer at (E0, 1A0) with its map, the piece 042AEA. */
static void k41c78(neo_gactor_t *a,neo_genv_t *e,neo_gactor_t *h) {
    if (e->flags) {e->flags->player_lock=0xff;e->flags->flash_request=0xff;}   /* 0C8CD6 */
    ga_sb(h,0x86,ga_b(h,0x86)|1u);SET(0x15,2);
    to_parent(a,e,ROOT);ga_project(a,e);                                   /* 01AD52 */
    k446d4(a,0x1ab,0xe0,0x200,0xc);
    SB(0xa3,1);SL(0x8e,0);SL(0x92,0);
    ga_sw(h,0x7e,0x40);
    k41ee8(a,e,0xe0,0x1a0);
    k41e02(e);
    part(a,e,0x42aea);
    ga_sb(h,0x89,0);
}
/* 041E38: 10C00B = 1, the camera's speeds 0, 0C7B48(3), the glide to (100, 10) at (A0, A0); the hub's +82 / +84 = the
 * camera's 698E / 6992 words. */
static void k41e38(neo_gactor_t *a,neo_genv_t *e,neo_gactor_t *h) {
    if (e->flags) e->flags->scroll_lock=1;
    if (e->camera_push) e->camera_push(e->ctx,0);
    if (e->camera_rise) e->camera_rise(e->ctx,0);
    if (e->camera_mode) e->camera_mode(e->ctx,3);
    k448d4(a,e,0x100,0x10,0xa0,0xa0);
    ga_sw(h,0x82,(uint16_t)((uint32_t)e->camera_xy[0]>>16));ga_sw(h,0x84,(uint16_t)((uint32_t)e->camera_xy[1]>>16));
}
static void k41ec0(neo_genv_t *e) {   /* 10C00B / 10C00C = 0, 0C7B48(0), 0CA2F2 x2 */
    if (e->flags) {e->flags->scroll_lock=0;e->flags->player_handlers=0;e->flags->handler_p2=0;e->flags->split_handlers=0;}
    if (e->kernel) e->kernel->b400c=0;
    if (e->camera_mode) e->camera_mode(e->ctx,0);
}
/* 04493E: the body's height against the steps 04496C: on a new step (0..4) the floor breaks (044978). */
static void k4493e(neo_gactor_t *a,neo_genv_t *e) {
    int16_t d0=-1,d1=S16(W(0x2c));
    for (uint32_t p=0x4496c;;p+=2) {int16_t d2=S16(rw(e,p));if (e->unsupported) return;if (d1>=d2) break;d0++;}
    if ((uint8_t)d0==B(0xa5)) return;
    SB(0xa5,(uint8_t)d0);
    if (d0<0 || d0>=5) return;
    neo_gactor_t *c=part(a,e,0x44a08);                                      /* 0449D4 */
    if (c) ga_sb(c,0x86,(uint8_t)d0);
    int16_t d[6]={S16(rw(e,0x45278u+4u*(uint16_t)d0)),S16(rw(e,0x4527au+4u*(uint16_t)d0)),0xb,7,0x14,9};
    if (e->map_patch) e->map_patch(e->ctx,0x10c72eu,0x45244u,d); else ga_fail(e,0xc714e);   /* 0C714E on 10C72E */
    SB(0x81,B(0x81)|0x80u);SB(0x82,0xc);
    if (e->shake_pulse) e->shake_pulse(e->ctx);                             /* 0C8022 */
    sound(e,0x102f);
}
/* 042234: carry when the body's program speed is within 20 of the hub's +7E and +A4 is 0 (the program 241A4E). */
static int k42234(neo_gactor_t *a,neo_genv_t *e,neo_gactor_t *h) {
    int16_t d0=(int16_t)(ga_w(h,0x7e)-W(0x26));
    if (d0<0) d0=(int16_t)-d0;
    if ((uint16_t)d0>=0x20u || B(0xa4)) return 0;
    if (!ga_anim_start(a,e,0x241a4e,W(0x26))) return -1;
    return 1;
}
static ga_result_t body_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0x4157a:
        k418d2(a,e);
        if (e->unsupported) return GA_UNSUPPORTED;
        SL(0,0x41586);return GA_MORE;
    case 0x41586:
        part(a,e,0x45428);
        SB(0x60,0);SL(0,0x415ba);
        /* fall through */
    case 0x415ba:
        if (B(0x60)) SL(0,0x415cc);
        return body_tail(a,e);
    case 0x415cc: {HUB();k41b34(a,e,h);}
        /* fall through */
    case 0x415d0: {HUB();k42284(a,e,h);k44682(h);SL(0,0x415de);}
        /* fall through */
    case 0x415de: {HUB();k422c8(a,e);if (k422a0(h)) SL(0,0x415f4);return body_tail(a,e);}
    case 0x415f4: {HUB();k42308(a,e);k44666(h);SL(0,0x41602);}
        /* fall through */
    case 0x41602: if (k4240a(a,e)) SL(0,0x41614);return body_tail(a,e);
    case 0x41614: {HUB();k44682(h);SL(0,0x4161e);}
        /* fall through */
    case 0x4161e: k422c8(a,e);if (k42aac(a,e)) SL(0,0x415d0);return body_tail(a,e);
    case 0x41634: {HUB();k41bce(a,e,h);SL(0,0x4163e);}
        /* fall through */
    case 0x4163e: if (e->players.active_mask&3u) SL(0,0x41650);return body_tail(a,e);   /* 044B6E */
    case 0x41650: {
        HUB();k41c78(a,e,h);
        if (e->flags) {e->flags->player_handlers=8;e->flags->handler_p2=8;e->flags->split_handlers=0;}   /* 041E82: 0CA2E0 x2 (041EBC) */
        SW(0x96,0x14);SL(0,0x41664);
    }   /* fall through */
    case 0x41664: {HUB();if (k42234(a,e,h)<0) return GA_UNSUPPORTED;SW(0x96,W(0x96)-1u);if (S16(W(0x96))<0) SL(0,0x4167a);return body_tail(a,e);}
    case 0x4167a: {HUB();k41e38(a,e,h);SL(0,0x41684);}
        /* fall through */
    case 0x41684: {HUB();if (k42234(a,e,h)<0) return GA_UNSUPPORTED;if (B(0x60)) SL(0,0x4169a);return body_tail(a,e);}
    case 0x4169a: SL(0,0x416a0);
        /* fall through */
    case 0x416a0: {HUB();if (k42234(a,e,h)<0) return GA_UNSUPPORTED;if (!B(0x81)) SL(0,0x416b6);return body_tail(a,e);}
    case 0x416b6: {HUB();k446d4(a,0x1ab,0x58,0x300,4);SB(0xa3,1);SL(0x8e,0);SL(0x92,0);ga_sb(h,0x8e,0xff);SL(0,0x416c0);}   /* 041D02 */
        /* fall through */
    case 0x416c0: {HUB();k4493e(a,e);if (k42234(a,e,h)<0) return GA_UNSUPPORTED;if (!B(0x81)) SL(0,0x416da);return body_tail(a,e);}
    case 0x416da: {
        HUB();
        if (e->shake) e->shake(e->ctx);                                       /* 0C801A */
        for (unsigned i=0;i<2;i++) {neo_gactor_t *c=child(a,e,0x44b32,0x2a);if (c) ga_place_from(c,a,e,i?0x30:0,(int16_t)0xffe8);}   /* 044AE4 */
        k44666(h);sound(e,0x102f);SW(0x96,0x3c);SL(0,0x416fe);
    }   /* fall through */
    case 0x416fe: {HUB();if (k42234(a,e,h)<0) return GA_UNSUPPORTED;SW(0x96,W(0x96)-1u);if (S16(W(0x96))<0) SL(0,0x41714);return body_tail(a,e);}
    case 0x41714: k446d4(a,0xe0,0x100,0x100,0xc);SB(0xa3,0);SL(0x30,0);SL(0x34,0);sound(e,0x10cc);SL(0,0x4171e);   /* 04207C */
        /* fall through */
    case 0x4171e: {HUB();if (k42234(a,e,h)<0) return GA_UNSUPPORTED;if (!B(0x81)) SL(0,0x41734);return body_tail(a,e);}
    case 0x41734: SL(0,0x4173a);
        /* fall through */
    case 0x4173a: {HUB();int r=k42234(a,e,h);if (r<0) return GA_UNSUPPORTED;if (r) SL(0,0x4174c);return body_tail(a,e);}
    case 0x4174c: {   /* 0420A8 */
        HUB();
        k446d4(a,0x1ab,0xd8,0x100,0xc);SB(0xa3,1);SL(0x8e,0);SL(0x92,0);ga_sb(h,0x72,0);ga_sb(h,0x8e,0);
        START(0x241a5a);ga_sw(h,0x7e,0x100);sound(e,0x10c9);SL(0,0x41756);
    }   /* fall through */
    case 0x41756: if (!B(0x81)) SL(0,0x41768);return body_tail(a,e);
    case 0x41768: SW(0x96,3);sound(e,0x1075);SL(0,0x4177e);
        /* fall through */
    case 0x4177e: {
        int16_t i=S16(W(0x96));
        if (i>=0) {static const int8_t d[4]={1,-1,2,-2};SW(0x2c,W(0x2c)+(uint16_t)(int16_t)d[i&3]);}   /* the bytes 0417B4 */
        else if (i<-0x28) SL(0,0x417b8);
        SW(0x96,W(0x96)-1u);
        return body_tail(a,e);
    }
    case 0x417b8: {   /* 0420FC */
        HUB();
        sound(e,0x29);START(0x2415f8);SW(0x26,0x40);ga_sw(h,0x7e,0x100);
        to_parent(a,e,LAYER);                                                  /* 01AC4C to 10D7B6 */
        k446d4(a,0xe0,0x12a,0x180,0xc);SB(0xa3,0);SL(0x30,0);SL(0x34,0);SL(0,0x417c2);
    }   /* fall through */
    case 0x417c2: if (!B(0x81)) SL(0,0x417d4);return body_tail(a,e);
    case 0x417d4: {HUB();k448d4(a,e,ga_w(h,0x82),ga_w(h,0x84),0x100,0x100);SL(0,0x417de);}   /* 04215E */
        /* fall through */
    case 0x417de: if (B(0x60)) SL(0,0x417f0);return body_tail(a,e);
    case 0x417f0: {   /* 042178, 041EC0 */
        HUB();
        if (e->flags) e->flags->player_lock=0;                                  /* 0C8CE6 */
        ga_sb(h,0x70,2);SL(0x50,0x44de4);SB(0x5a,0);CLR(0x5f,7);ga_sb(h,0x86,ga_b(h,0x86)&~2u);
        SW(0x4e,rank(a,e,0x2c4084));
        k423d8(a,e);
        part(a,e,0x45ef6);
        k41ec0(e);
        SL(0,0x417fe);
    }   /* fall through */
    case 0x417fe: k4249e(a,e);k424ac(a,e);return body_tail(a,e);
    case 0x4180a: {   /* 041D2E */
        HUB();
        ga_sb(h,0x6e,4);ga_sb(h,0x6f,2);ga_sb(h,0x71,0);ga_sb(h,0x72,5);ga_sb(h,0x8e,0);
        k446cc(a);SL(0x8e,0);SL(0x92,0);SL(0x30,0);SL(0x34,0);SL(0x50,0);SB(0x5a,0);CLR(0x5f,7);
        neo_gactor_t *c=part(a,e,0x443be);
        if (c) ga_sw(c,0x86,0x3c);
        neo_gactor_t *w=after_script(a,e,0xae1dc);                                 /* 0AE220 (1, 0) */
        if (w) {ga_sb(w,0x70,0);ga_sw(w,0x6e,1);}
        sound(e,(uint16_t)(0x0a00u|(uint8_t)(0x80u+0x20u)));                      /* 01228C (0x80) */
        sound(e,0x10cc);
        stop_sound(e,0x10d0);stop_sound(e,0x10d2);                               /* 04226E */
        SL(0,0x41814);
    }   /* fall through */
    case 0x41814: if (e->players.active_mask&3u) SL(0,0x41826);return body_tail(a,e);
    case 0x41826: STOP(0x41826);   /* 041DD4 / 041E9A: the walk handler 0CA438 (not yet) */
    case 0x41838: if (!B(0x81)) SL(0,0x4184a);return body_tail(a,e);
    case 0x4184a: if (B(0x60)) SL(0,0x4185c);return body_tail(a,e);
    case 0x4185c: {HUB();if (ga_b(h,0x6f)&0x80u) SL(0,0x41872);return body_tail(a,e);}
    case 0x41872: STOP(0x4677a);   /* the escape 04677A (not yet) */
    }
    STOP(L(0));
}
/* 04240A: the step 044ECC[+80][+9F] (carry at the list's end). */
static void step_run(neo_gactor_t *a,neo_genv_t *e,uint32_t r);
static bool k4240a(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=step_at(a,e);
    if (r==0xffffffffu) return true;
    step_run(a,e,r);
    return false;
}
/* 043804: the angle of (D0, D1) as a direction 0..8 (TRAP #15 outside; 0xFF here). */
static uint8_t k43804(neo_gactor_t *a,neo_genv_t *e,int16_t dx,int16_t dy) {
    uint8_t v=(uint8_t)((uint8_t)((uint8_t)(neo_angle(dx,dy)+4u)>>3)-8u);
    uint8_t lo=B(0x80)==1u?0xf:0xc,hi=B(0x80)==1u?0x11:0x14;
    if (v<lo) v=lo;
    if (v>hi) v=hi;
    v=(uint8_t)(v+0xf4u);
    if (v>8u) {ga_fail(e,0x4384a);return 0xff;}                          /* TRAP #15 */
    return v;
}
/* The horizontal speed toward D0 by 0A6918 (D1 = 40, D2 = +7E, D3 = 10), at most +7E, into +8E. */
static void track(neo_gactor_t *a,neo_genv_t *e,int16_t d0) {
    uint32_t v=(uint32_t)(int32_t)ka6918(e,d0,0x40,W(0x7e),0x10);
    if (S32(v)<0) v=(v&0xffff0000u)|(uint16_t)(0u-(uint16_t)v);
    if ((uint16_t)v>W(0x7e)) v=(v&0xffff0000u)|W(0x7e);
    if (S32(v)<0) v=(v&0xffff0000u)|(uint16_t)(0u-(uint16_t)v);
    SL(0x8e,(uint32_t)((int32_t)(int16_t)(uint16_t)v<<8));
}
static void step_run(neo_gactor_t *a,neo_genv_t *e,uint32_t r) {
    neo_gactor_t *h=node(e,L(0x72));
    if (!h) return;
    switch (r) {
    case 0x42734: SB(0x9c,2);
        /* fall through */
    case 0x42516: {
        bool one=B(0x80)==1u;
        SW(0x7e,rank(a,e,rl(e,(one?0x45016u:0x45056u)+(uint32_t)k4463e(a,e))));
        SB(0x9e,(uint8_t)rank(a,e,rl(e,(one?0x45026u:0x45066u)+(uint32_t)k4463e(a,e))));
        SW(0x96,rank(a,e,rl(e,(one?0x45036u:0x45076u)+(uint32_t)k4463e(a,e))));
        k446d4(a,0x180,0x100,0x400,0xc);k42440(a,e);return;
    }
    case 0x4273e: case 0x425c2: if (!B(0x81)) {ga_sb(h,0x6e,2);ga_sb(h,0x6f,1);k42440(a,e);}return;
    case 0x42742: case 0x425e4: if (ga_b(h,0x6f)&0x80u) k42440(a,e);return;
    case 0x425f6: {
        uint32_t n;
        if (nearest(a,e,&n)) {k4246e(a,e,0x42708);return;}
        int16_t d0=(int16_t)(pw(e,n,0x3c)-W(0x3c));
        uint8_t d3=d0<0?0xff:0;
        track(a,e,d0);
        if (W(0x96)) {SW(0x98,0x96);SW(0x96,W(0x96)-1u);}
        else {
            SW(0x98,W(0x98)-1u);
            if (S16(W(0x98))<0 || ((B(0xa1)^d3)&0x80u) || !L(0x8e)) k42440(a,e);
        }
        SB(0xa1,d3);return;
    }
    case 0x42844: case 0x4268c: SL(0x8e,0);ga_sb(h,0x6f,5);k42440(a,e);return;
    case 0x42848: case 0x426a0:
        if (!(ga_b(h,0x6f)&0x80u)) return;
        SB(0x9e,B(0x9e)-1u);
        if (!B(0x9e)) {k42440(a,e);return;}
        if (B(0x80)==1u) {SW(0x96,rank(a,e,rl(e,0x45046u+(uint32_t)k4463e(a,e))));k4246e(a,e,0x425f6);}
        else {SW(0x96,rank(a,e,rl(e,0x45086u+(uint32_t)k4463e(a,e))));k4246e(a,e,0x42746);}
        return;
    case 0x4284c: case 0x42708: ga_sb(h,0x6f,2);ga_sb(h,0x6e,4);k42440(a,e);return;
    case 0x42850: case 0x42722: if (ga_b(h,0x6e)&0x80u) k42440(a,e);return;
    case 0x42746: {
        uint32_t n;
        if (nearest(a,e,&n)) {k4246e(a,e,0x4284c);return;}
        uint8_t d2=n!=P1_NODE?0xff:0;
        int16_t dx=(int16_t)(pw(e,n,0x3c)-W(0x3c)),dy=(int16_t)(pw(e,n,0x40)-W(0x40)+0x38u+0x10u);
        uint8_t d3=B(0x9c)!=d2?0xff:0;
        if (d3) SB(0x9c,d2);
        if (!(e->video&0xfu)) d3=0xff;
        uint8_t d0;
        if (d3) {d0=k43804(a,e,dx,dy);if (e->unsupported) return;}
        else d0=B(0x9d);
        if (S16(W(0x3c))<0x40 && d0>4u) d0=4;
        if (S16(W(0x3c))>0x100 && d0<4u) d0=4;
        SB(0x9d,d0);
        int16_t d1=S16(rw(e,0x44fb8u+2u*(d0&0xfu)));
        int16_t t=(int16_t)(((int32_t)d1*(int16_t)-dy)>>8);
        track(a,e,(int16_t)(dx-t));
        if (!W(0x96)) k42440(a,e); else SW(0x96,W(0x96)-1u);
        return;
    }
    case 0x42854: ga_sb(h,0x6e,2);k42440(a,e);return;
    case 0x42864: if (ga_b(h,0x6e)&0x80u) {k446d4(a,0x280,0x110,0x400,0xc);k42440(a,e);}return;
    case 0x4288a: case 0x42952: if (!B(0x81)) k42440(a,e);return;
    case 0x42898: {
        ga_sb(h,0x70,1);
        uint16_t d0=(uint16_t)(rank(a,e,rl(e,0x45096u+(uint32_t)k4463e(a,e)))<<2);
        k44b84(e);                                                         /* both answers read 2C4DA0 */
        ga_sw(h,0x80,rw(e,0x2c4da0u+d0));
        SL(0x8e,0u-(uint32_t)((int32_t)S16(rw(e,0x2c4da2u+d0))<<8));
        k42440(a,e);return;
    }
    case 0x428f2:
        if (S16(W(0x86))>0xa0) return;
        ga_sb(h,0x70,0);ga_sb(h,0x6e,4);SW(0x8a,W(0x8a)+0x40u);
        k446d4(a,0x200,W(0x8a),0x800,4);k42440(a,e);return;
    case 0x42930: if (!B(0x81)) {k446d4(a,0x200,0xc0,0x400,0xc);k42440(a,e);}return;
    case 0x42960: case 0x429e6: case 0x42a9a:
        if (ga_b(h,r==0x42a9au?0x6f:0x6e)&0x80u) k42440(a,e);
        return;
    case 0x42972: k446d4(a,0x200,0xe0,0x400,0xc);k42440(a,e);return;
    case 0x4298c: if (!B(0x81)) {ga_sb(h,0x6e,2);k42440(a,e);}return;
    case 0x429a4: if (ga_b(h,0x6e)&0x80u) {ga_sb(h,0x71,1);k42440(a,e);}return;
    case 0x429c0: if (ga_b(h,0x71)&0x80u) {ga_sb(h,0x71,0);ga_sb(h,0x6e,4);k42440(a,e);}return;
    case 0x429f8: ga_sb(h,0x6f,7);k42440(a,e);return;
    case 0x42a08: if (ga_b(h,0x6f)&0x80u) k42440(a,e);return;
    case 0x42a1a: if (ga_b(h,0x87)!=2u) ga_sb(h,0x6e,2);k42440(a,e);return;
    case 0x42a38:
        if (ga_b(h,0x87)!=2u) return;
        if (ga_b(h,0x88)) ga_sb(h,0x6f,2);
        k42440(a,e);return;
    case 0x42a60: if (!ga_b(h,0x88)) {ga_sb(h,0x71,1);k42440(a,e);}return;
    case 0x42a7e: if (ga_b(h,0x71)&0x80u) {ga_sb(h,0x6f,1);k42440(a,e);}return;
    }
    ga_fail(e,r);
}
/* ---- the body's helpers 0443BE / 04445A / 044A08 / 044B32 / 0424F8 / 042AEA ----------- */
static ga_result_t aux_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0x443be:   /* the kills (004BFE) for +86 updates */
        SW(0x86,W(0x86)-1u);
        if (S16(W(0x86))<0) return GA_DELETE;                               /* (cartridge check) 004188 */
        if (e->flags) e->flags->kill_request=0xff;                          /* 004BFE */
        return GA_MORE;
    case 0x4445a: {   /* the camera glide to (+86, +88) at (+8A, +8C) */
        if (!e->camera_place) STOP(0x4445a);
        int32_t vx=(int32_t)S16(W(0x8a))<<8,vy=(int32_t)S16(W(0x8c))<<8;
        int32_t d0=(int32_t)((uint32_t)e->camera_xy[0]-((uint32_t)W(0x86)<<16));
        if (d0>=0) vx=-vx;
        if (!((int32_t)(((uint32_t)d0+(uint32_t)vx)^(uint32_t)d0)>0)) {e->camera_place(e->ctx,0,(int32_t)((uint32_t)W(0x86)<<16));vx=0;}
        if (e->camera_push) e->camera_push(e->ctx,vx);
        d0=(int32_t)((uint32_t)e->camera_xy[1]-((uint32_t)W(0x88)<<16));
        if (d0>=0) vy=-vy;
        if (!((int32_t)(((uint32_t)d0+(uint32_t)vy)^(uint32_t)d0)>0)) {e->camera_place(e->ctx,1,(int32_t)((uint32_t)W(0x88)<<16));vy=0;}
        if (e->camera_rise) e->camera_rise(e->ctx,vy);
        if (vx|vy) return GA_MORE;
        neo_gactor_t *c=creator(a,e);if (!c) return GA_UNSUPPORTED;
        ga_sb(c,0x60,1);return GA_DELETE;                                   /* 004188 */
    }
    case 0x44a08: {   /* the floor breaking at (140, 90 - 8 * +86) of the root */
        ga_place(a,e,0x140,(int16_t)(0x90-((int16_t)B(0x86)<<3)),ROOT);
        neo_gactor_t *c=k1da40(a,e,0x45302);if (c) {ga_sw(c,0x66,0x190);ga_sw(c,0x1e,0x190);}
        c=k1da40(a,e,0x45314);if (c) {ga_sw(c,0x66,0x190);ga_sw(c,0x1e,0x190);}
        SL(0,0x44a56);return GA_MORE;
    }
    case 0x44a56: return GA_DELETE;                                         /* (cartridge check) 004188 */
    case 0x44b32: ga_palette(a,0x204);START(0x22d372);SL(0,0x44b56);
        /* fall through */
    case 0x44b56: {ga_project(a,e);int r=STEP();if (r<0) return GA_UNSUPPORTED;return r?GA_DELETE:GA_MORE;}
    case 0x424f8: {   /* the sub-script actor 019E72 with the record +86 */
        neo_gactor_t *c=child(a,e,0x19e72,0xe);
        if (c) ga_sl(c,0x6e,L(0x86));
        SL(0,0x42514);return GA_MORE;
    }
    case 0x42514: return GA_MORE;
    case 0x42aea: {   /* the piece on the layer at (CB, -50), until the hub's +89 */
        ga_place(a,e,0xcb,(int16_t)0xffb0,LAYER);
        START(0x241362);ga_palette(a,0x10c);SW(0x66,0x64);SW(0x1e,0x64);SET(0x15,2);SL(0,0x42b34);
    }   /* fall through */
    case 0x42b34: {
        neo_gactor_t *h=node(e,L(0x72));if (!h) return GA_UNSUPPORTED;
        if (ga_b(h,0x89)) return GA_DELETE;                                 /* (cartridge check) 004188 */
        ga_project(a,e);if (STEP()<0) return GA_UNSUPPORTED;
        return GA_MORE;
    }
    }
    STOP(L(0));
}
/* ---- part 0: 042BDC (the glow; table 0453BA) ----------------------------------------- */
static ga_result_t p0_tail(neo_gactor_t *a,neo_genv_t *e) {   /* 042DC0 */
    neo_gactor_t *m=node(e,L(0x6e));
    if (!m) return GA_UNSUPPORTED;
    if (ga_b(m,0x15)&4u) SET(0x15,2); else CLR(0x15,2);
    HUB();
    if (ga_b(h,0x87)) {
        uint16_t d2=(uint16_t)((W(0x86)+0x180u)&0x3fffu);
        SW(0x86,d2);
        int16_t v=(int16_t)((d2>>8)-0x20);
        if (v<0) v=(int16_t)-v;
        k9cce(e,B(0x1a),0xd9,(uint8_t)v);
    }
    return k44860(a,e);
}
static ga_result_t p0_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0x42bdc: {
        HUB();
        ga_sb(h,0x87,0);
        SW(0x66,0x64);SW(0x1e,0x64);
        ga_palette(a,0xd7);ga_palette(a,0xd8);ga_palette(a,0x10b);
        ga_flash_palette(a,0xdb);
        on_creator(a,e,0,(int16_t)0xffc8);
        SL(0,0x42be6);
    }   /* fall through */
    case 0x42be6: ga_project(a,e);return p0_tail(a,e);
    case 0x42bf0: {HUB();START(0x241ba6);k4489e(a,h,-1);SL(0,0x42c0e);}
        /* fall through */
    case 0x42c0e: ga_project(a,e);share_flash(a,e);if (STEP()<0) return GA_UNSUPPORTED;return p0_tail(a,e);
    case 0x42c22: {HUB();k4489e(a,h,-1);SL(0,0x42c30);}
        /* fall through */
    case 0x42c30: ga_project(a,e);return p0_tail(a,e);
    case 0x42c3a: START(0x241ab0);SL(0,0x42c50);
        /* fall through */
    case 0x42c50: {
        ga_project(a,e);share_flash(a,e);
        int r=STEP();if (r<0) return GA_UNSUPPORTED;
        if (r) {HUB();k4489e(a,h,3);}
        return p0_tail(a,e);
    }
    case 0x42c70: START(0x241b3a);SL(0,0x42c86);
        /* fall through */
    case 0x42c86: ga_project(a,e);share_flash(a,e);if (STEP()<0) return GA_UNSUPPORTED;return p0_tail(a,e);
    case 0x42c9a: {
        ga_project(a,e);share_flash(a,e);if (STEP()<0) return GA_UNSUPPORTED;
        HUB();
        if (!(ga_b(h,0x6f)&0x7fu)) SL(0,0x42cc8);
        return p0_tail(a,e);
    }
    case 0x42cc8: {
        ga_project(a,e);share_flash(a,e);if (STEP()<0) return GA_UNSUPPORTED;
        HUB();
        if (!ga_b(h,0x87)) {k4489e(a,h,0);return p0_tail(a,e);}
        if (ga_b(h,0x87)==2u) SL(0,0x42cfa);
        return p0_tail(a,e);
    }
    case 0x42cfa: START(0x241b4e);SL(0,0x42d10);
        /* fall through */
    case 0x42d10: {
        ga_project(a,e);share_flash(a,e);
        int r=STEP();if (r<0) return GA_UNSUPPORTED;
        if (r) {HUB();k4489e(a,h,0);}
        return p0_tail(a,e);
    }
    case 0x42d30: {HUB();START(0x241bc8);k4489e(a,h,-1);SL(0,0x42d4e);}
        /* fall through */
    case 0x42d4e: ga_project(a,e);share_flash(a,e);if (STEP()<0) return GA_UNSUPPORTED;return p0_tail(a,e);
    }
    STOP(L(0));
}
/* ---- part 1: 0431E0 (the arm; table 0453D2) and its piece 04359E ------------------------- */
static ga_result_t main_a0(neo_gactor_t *a,neo_genv_t *e,uint8_t v) {
    neo_gactor_t *m=node(e,L(0x6e));
    if (!m) return GA_UNSUPPORTED;
    ga_sb(m,0xa0,v);return GA_MORE;
}
static ga_result_t p1_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0x431e0: {
        ga_palette(a,0xd7);ga_palette(a,0xe7);ga_palette(a,0x109);
        ga_flash_palette(a,0xda);
        SW(0x66,0x12c);SW(0x1e,0x12c);
        on_creator(a,e,0,(int16_t)0xffc8);
        HUB();ga_sb(h,0x88,0);
        SL(0,0x4323e);
    }   /* fall through */
    case 0x4323e: ga_project(a,e);return k44860(a,e);
    case 0x43248: if (main_a0(a,e,0)!=GA_MORE) return GA_UNSUPPORTED;SL(0,0x43258);
        /* fall through */
    case 0x43258: SB(0x17,0);ga_project(a,e);return k44860(a,e);
    case 0x43268: if (main_a0(a,e,0)!=GA_MORE) return GA_UNSUPPORTED;SL(0,0x43278);
        /* fall through */
    case 0x43278: {
        ga_project(a,e);
        HUB();
        if ((ga_b(h,0x6e)&0x7fu)==3u) SL(0,0x4329c);
        return k44860(a,e);
    }
    case 0x4329c: START(0x241be2);SB(0x14,B(0x19));SL(0,0x432b8);
        /* fall through */
    case 0x432b8: {
        ga_project(a,e);share_flash(a,e);
        int r=STEP();if (r<0) return GA_UNSUPPORTED;
        if (r) {HUB();k4489e(a,h,3);}
        return k44860(a,e);
    }
    case 0x432d8: if (main_a0(a,e,0)!=GA_MORE) return GA_UNSUPPORTED;SL(0,0x432e8);
        /* fall through */
    case 0x432e8: {
        HUB();
        if (!ga_b(h,0x88)) goto done_43354;
        if (ga_b(h,0x88)==2u) SL(0,0x4331a);
        ga_project(a,e);share_flash(a,e);if (STEP()<0) return GA_UNSUPPORTED;
        return k44860(a,e);
    }
    case 0x4331a: START(0x241c8a);SB(0x14,B(0x19));if (main_a0(a,e,0)!=GA_MORE) return GA_UNSUPPORTED;SL(0,0x43340);
        /* fall through */
    case 0x43340: {
        ga_project(a,e);share_flash(a,e);
        int r=STEP();if (r<0) return GA_UNSUPPORTED;
        if (!r) return k44860(a,e);
    }
    done_43354: {
        SB(0x17,0);
        HUB();k4489e(a,h,0);
        return k44860(a,e);
    }
    case 0x43366: SET(0x15,2);
        /* fall through */
    case 0x4336c: START(0x241c76);SB(0x14,B(0x19));if (main_a0(a,e,0)!=GA_MORE) return GA_UNSUPPORTED;SL(0,0x43392);
        /* fall through */
    case 0x43392: ga_project(a,e);share_flash(a,e);if (STEP()<0) return GA_UNSUPPORTED;return k44860(a,e);
    case 0x433a6: START(0x241d32);SB(0x14,B(0x1a));if (main_a0(a,e,1)!=GA_MORE) return GA_UNSUPPORTED;SB(0x60,0);SL(0,0x433d0);
        /* fall through */
    case 0x433d0: {
        ga_project(a,e);SB(0x17,0);
        if (STEP()<0) return GA_UNSUPPORTED;
        if (B(0x60)) {if (main_a0(a,e,0)!=GA_MORE) return GA_UNSUPPORTED;HUB();k4489e(a,h,3);}
        return k44860(a,e);
    }
    case 0x43400: {
        START(0x24136e);SW(0x66,0x12c);SW(0x1e,0x12c);
        neo_gactor_t *c=part(a,e,0x4359e);
        if (c) ga_place_at(c,a,e,0,0);
        if (main_a0(a,e,1)!=GA_MORE) return GA_UNSUPPORTED;
        SW(0x86,rank(a,e,rl(e,0x451beu+(uint32_t)k4463e(a,e))));SB(0x88,0);SL(0,0x4347a);
    }   /* fall through */
    case 0x4347a:
        SW(0x86,W(0x86)-1u);
        if (S16(W(0x86))<0) SL(0,0x4349e);
        ga_project(a,e);SB(0x17,0);if (STEP()<0) return GA_UNSUPPORTED;return k44860(a,e);
    case 0x4349e:
        START(0x2413b4);SW(0x86,rank(a,e,rl(e,0x451ceu+(uint32_t)k4463e(a,e))));SB(0x88,1);SB(0x8a,0);SL(0,0x434d4);
        /* fall through */
    case 0x434d4:
        if (B(0x8a)) ga_attack(a,e,0x44e9c);                                  /* 0049E8 */
        SW(0x86,W(0x86)-1u);
        if (S16(W(0x86))<0) SL(0,0x4350a);
        ga_project(a,e);SB(0x17,0);if (STEP()<0) return GA_UNSUPPORTED;return k44860(a,e);
    case 0x4350a: START(0x241430);SB(0x88,2);SL(0,0x43526);
        /* fall through */
    case 0x43526: {
        ga_project(a,e);SB(0x17,0);
        int r=STEP();if (r<0) return GA_UNSUPPORTED;
        if (r) {HUB();k4489e(a,h,3);stop_sound(e,0x10d2);}
        return k44860(a,e);
    }
    case 0x43552:
        START(0x24136e);SW(0x66,0x12c);SW(0x1e,0x12c);if (main_a0(a,e,1)!=GA_MORE) return GA_UNSUPPORTED;SL(0,0x4357e);
        /* fall through */
    case 0x4357e: ga_project(a,e);SB(0x17,0);if (STEP()<0) return GA_UNSUPPORTED;return k44860(a,e);
    /* 04359E: the arm's piece; its program follows the arm's +88 (table 044DC4). */
    case 0x4359e: ga_palette(a,0x109);SW(0x66,0x64);SW(0x1e,0x64);SB(0x88,0xff);SB(0x89,0);SW(0x2c,0x30);SL(0,0x435ce);
        /* fall through */
    case 0x435ce: {
        HUB();
        if (ga_b(h,0x86)&2u) return GA_DELETE;                              /* 004188 */
        neo_gactor_t *c=creator(a,e);
        if (!c) return GA_UNSUPPORTED;
        if (ga_b(c,0x88)!=B(0x88)) {SB(0x88,ga_b(c,0x88));START(rl(e,0x44dc4u+4u*B(0x88)));}
        ga_project(a,e);
        if (STEP()<0) return GA_UNSUPPORTED;
        if (B(0x89)) return GA_DELETE;                                      /* (cartridge check) 004188 */
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    }
    STOP(L(0));
}
/* ---- part 2: 042E22 (the gun; table 0453F6) and its shells 042F04 ---------------------- */
static ga_result_t p2_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0x42e22: on_creator(a,e,0,(int16_t)0xffc8);ga_palette(a,0x176);SW(0x66,0xc8);SW(0x1e,0xc8);SL(0,0x42e50);
        /* fall through */
    case 0x42e50: return k44860(a,e);
    case 0x42e54: ga_project(a,e);return k44860(a,e);
    case 0x42e5e: {HUB();SW(0x86,ga_w(h,0x80));SW(0x88,W(0x86));SL(0,0x42e74);}
        /* fall through */
    case 0x42e74:
        ga_project(a,e);
        SW(0x88,W(0x88)-1u);
        if (S16(W(0x88))<=0) SL(0,0x42e8c);
        return k44860(a,e);
    case 0x42e8c: START(0x24235c);SL(0,0x42ea2);
        /* fall through */
    case 0x42ea2: {
        ga_project(a,e);
        int r=STEP();if (r<0) return GA_UNSUPPORTED;
        if (r) {
            SW(0x88,W(0x86));
            neo_gactor_t *c=part(a,e,0x42f04);
            if (c) ga_place_at(c,a,e,0,0);
            SL(0,0x42e74);
        }
        return k44860(a,e);
    }
    case 0x42efa: return GA_DELETE;                                         /* 004188 */
    case 0x42f04:
        START(0x242386);ga_palette(a,0x169);SW(0x66,0xc8);SW(0x1e,0xc8);SET(0x15,5);SL(0x30,0);SL(0x34,0xfffe0000u);
        SW(0x4e,rank(a,e,rl(e,0x451aeu+(uint32_t)k4463e(a,e))));SL(0,0x42f5c);
        /* fall through */
    case 0x42f5c: {
        SL(0x34,L(0x34)+0xffffc000u);
        ga_air(a,e);                                                        /* 0143E4 */
        if (STEP()<0) return GA_UNSUPPORTED;
        if (ga_cull(a,e,0x44ec4)) return GA_DELETE;                         /* 01945C, (cartridge check) 004188 */
        if (ga_attack(a,e,0x44e8c)) SL(0,0x43054);
        int16_t dx,dy;
        if (ga_register(a,e,&dx,&dy)) {SB(0x5a,0);if (BIT(0x5f,7)) SL(0,0x43054);}
        if (BIT(0x15,7)) SL(0,0x43054);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x43054:
        sound(e,0x1022);
        SB(0x1d,0);                                                         /* 009F88 */
        SW(0x66,0x1001);SW(0x1e,0x1001);SL(0x50,0);
        SL(0,0x1c87e);return neo_region_tick(a,e);                          /* JMP 01C87E */
    }
    STOP(L(0));
}
/* ---- part 3: 04307E (the hatch; table 045402): the troopers 0438A4 ---------------------- */
static void k430cc(neo_gactor_t *a,neo_genv_t *e) {
    SB(0x88,0);
    switch (B(0x80)) {
    case 4: SB(0x89,(uint8_t)rank(a,e,rl(e,0x450e6u+(uint32_t)k4463e(a,e))));return;
    case 5: SB(0x89,1);return;
    case 6: SB(0x89,(uint8_t)rank(a,e,rl(e,0x45176u+(uint32_t)k4463e(a,e))));return;
    case 8: {   /* 0431B6 */
        uint16_t d1=rank(a,e,rl(e,0x451feu+(uint32_t)k4463e(a,e)));
        uint8_t d0=e->groups?e->groups[5]:0;                               /* 04465E: 019DCA of group 8 - 3 */
        int8_t v=(int8_t)((uint8_t)d1-d0);
        SB(0x89,v<0?0:(uint8_t)v);
        return;
    }
    }
}
static ga_result_t p3_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0x4307e: on_creator(a,e,0,(int16_t)0xffc8);SL(0,0x43092);
        /* fall through */
    case 0x43092: return k44860(a,e);
    case 0x43096: ga_project(a,e);return k44860(a,e);
    case 0x430a0: k430cc(a,e);SW(0x86,0);SL(0,0x430ae);
        /* fall through */
    case 0x430ae: {
        ga_project(a,e);
        /* 04313C */
        bool carry=false;
        uint16_t c=W(0x86);SW(0x86,c-1u);
        if (!c) {
            if (B(0x88)>=B(0x89)) carry=true;
            else {
                SB(0x88,B(0x88)+1u);SW(0x86,0x3c);
                neo_gactor_t *t=after_script(a,e,0x438a4);
                if (t) {
                    ga_sl(t,0x6e,L(0x6e));ga_sl(t,0x72,L(0x72));ga_sb(t,0x80,B(0x80));
                    ga_sb(t,0x69,(uint8_t)(B(0x80)-3u));ga_sb(t,0xa0,B(0x88));
                    ga_place(t,e,0,(int16_t)0xffd0,self(a));                  /* 01AB9C on this node's block */
                }
            }
        }
        if (carry) {HUB();k4489e(a,h,0);}
        return k44860(a,e);
    }
    }
    STOP(L(0));
}
/* ---- part 4: 0444E8 (the smoke; table 04540A) ------------------------------------------ */
static ga_result_t p4_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0x444e8: on_creator(a,e,0,0);
        /* fall through */
    case 0x444f6: SW(0x8a,0);SL(0,0x44500);
        /* fall through */
    case 0x44500: ga_project(a,e);return k44860(a,e);
    case 0x4450a: case 0x44540: {
        neo_gactor_t *m=node(e,L(0x6e));
        if (!m) return GA_UNSUPPORTED;
        if (ga_b(m,0x15)&4u) SET(0x15,2); else CLR(0x15,2);
        bool first=L(0)==0x4450au;
        SL(0x86,first?0x4528cu:0x4529eu);SB(0x8c,0x18);SW(0x8a,first?0x1033:0x1021);
        goto k4458e;
    }
    case 0x44576: CLR(0x15,2);SL(0x86,0x452b0);SB(0x8c,0x40);goto k4458e;
    case 0x445de: child(a,e,0x37404,0xe);SL(0,0x444f6);goto k4458e;
    case 0x445f6: if (e->kill_children) e->kill_children(e->ctx,a);SL(0,0x444f6);return k44860(a,e);   /* 004192 */
    k4458e: SB(0x8d,0);SL(0,0x44598);
        /* fall through */
    case 0x44598: {
        ga_project(a,e);
        uint8_t d0=B(0x8c),d=B(0x8d);
        SB(0x8d,(uint8_t)(d-d0));
        if (d<d0) {
            neo_gactor_t *c=k1da40(a,e,L(0x86));
            if (c) {ga_sw(c,0x66,0x190);ga_sw(c,0x1e,0x190);}
            if (W(0x8a)) {HUB();if (!ga_b(h,0x8e)) sound(e,W(0x8a));}
        }
        return k44860(a,e);
    }
    }
    STOP(L(0));
}
/* ---- the opening 045428 and its actors 045608 ----------------------------------------- */
static bool k45572(neo_gactor_t *a,neo_genv_t *e) {   /* carry at the end of the timeline */
    uint32_t p=L(0x86);
    uint8_t d0=rb(e,p);
    if (d0&0x80u) return true;
    SW(0x8a,d0);SW(0x8c,(uint16_t)(2u*rb(e,p+1)));
    SB(0x90,(uint8_t)(W(0x8c)==0?0xff:0));SB(0x91,0);
    SL(0x86,p+2u);
    return false;
}
static ga_result_t op_tick(neo_gactor_t *a,neo_genv_t *e) {
    neo_ga_flags_t *f=e->flags;
    switch (L(0)) {
    case 0x45428: {
        ga_place(a,e,0x200,0xa0,ROOT);                                       /* 01AD3E */
        ga_floor(a,e);                                                      /* 0146E8 */
        for (uint32_t p=0x45dbe;;p+=8) {
            uint32_t r=rl(e,p);
            if (e->unsupported) return GA_UNSUPPORTED;
            if (r==0xffffffffu) break;
            neo_gactor_t *c=part(a,e,0x45608);
            if (c) {ga_sl(c,0x86,r);ga_place_at(c,a,e,S16(rw(e,p+4)),S16(rw(e,p+6)));}
        }
        SL(0x86,0x45d84);
        k45572(a,e);
        /* 0455AC */
        if (!f) STOP(0x455ac);
        SB(0x93,0);
        f->walk_to[0]=0x180;f->walk_to[1]=0x170;f->player_handlers=6;f->handler_p2=6;f->split_handlers=1;   /* 0CA324 x2 (0CA386) */
        neo_m4boss_env_t *m=lenv(e);
        if (!m || !e->mission_speed) STOP(0x455ce);
        SW(0x8e,m->point69b8);m->point69b8=8;                                /* 10E9B8 */
        e->mission_speed(e->ctx,0x20000);                                    /* 019388 */
        SL(0,0x454ac);
    }   /* fall through */
    case 0x454ac: {
        if (!f) STOP(0x454ac);
        if (!f->player_handlers) {SET(0x93,0);f->player_handlers=7;}           /* 0CA2F8, 0CA2E0 (0455E6) */
        if (!f->handler_p2) {SET(0x93,1);f->handler_p2=7;}
        uint8_t d0=(uint8_t)(e->players.active_mask);                        /* 01AE8E: RAM 10C33C */
        if (d0==B(0x93)) SL(0,0x454fa);
        return GA_MORE;
    }
    case 0x454fa:
        if (!f) STOP(0x454fa);
        f->scroll_lock=1;                                                    /* 10C00B */
        if (e->camera_push) e->camera_push(e->ctx,0x20000);                  /* 695C */
        if (e->camera_mode) e->camera_mode(e->ctx,1);                        /* 0C7B48 */
        SL(0,0x45518);
        /* fall through */
    case 0x45518:
        if (B(0x90)) {if (!B(0x91)) return GA_MORE;}
        else {uint16_t c=W(0x8c);SW(0x8c,c-1u);if (S16((uint16_t)(c-1u))>=0) return GA_MORE;}
        if (!k45572(a,e)) return e->unsupported?GA_UNSUPPORTED:GA_MORE;
        if (!e->kernel || !f) STOP(0x45540);
        f->scroll_lock=0;e->kernel->b400c=0;                                 /* 10C00B, 10C00C */
        if (e->camera_push) e->camera_push(e->ctx,0);
        if (e->camera_mode) e->camera_mode(e->ctx,0);
        {neo_gactor_t *c=creator(a,e);if (!c) return GA_UNSUPPORTED;ga_sb(c,0x60,1);}
        {neo_m4boss_env_t *m=lenv(e);
         if (!m || !e->mission_speed) STOP(0x455ea);
         m->point69b8=W(0x8e);}                                              /* 0455EA: 10E9B8 */
        f->player_handlers=0;f->handler_p2=0;f->split_handlers=0;            /* 0CA2F2 x2 */
        e->mission_speed(e->ctx,-1);                                         /* 019388(-1): A0000 */
        return GA_DELETE;                                                    /* 0041AC */
    }
    STOP(L(0));
}
/* 045608: the opening's actors. Each update ends in 04560C (the next cue). */
static void k4560c(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *c=creator(a,e);
    if (!c) return;
    uint16_t d0=ga_w(c,0x8a);
    uint32_t p=L(0x86);
    uint16_t d1=rw(e,p);
    if (d0!=d1) return;
    SB(0x8c,(uint8_t)d1);SL(0,rl(e,p+2));SL(0x86,p+6);
}
static ga_result_t op_actor(neo_gactor_t *a,neo_genv_t *e);
static ga_result_t oa_cue(neo_gactor_t *a,neo_genv_t *e) {k4560c(a,e);return e->unsupported?GA_UNSUPPORTED:GA_MORE;}
static ga_result_t oa_anim(neo_gactor_t *a,neo_genv_t *e) {   /* 045638 */
    SL(0,0x4563e);
    ga_project(a,e);if (STEP()<0) return GA_UNSUPPORTED;
    return oa_cue(a,e);
}
static ga_result_t oa_wait(neo_gactor_t *a,neo_genv_t *e) {   /* 04564C */
    SB(0x8d,0);SL(0,0x45656);
    return op_actor(a,e);
}
static ga_result_t oa_fall(neo_gactor_t *a,neo_genv_t *e) {   /* 045688 */
    SL(0x30,0);SL(0x34,0);SL(0x76,0xe00);SL(0,0x4569e);
    return op_actor(a,e);
}
static ga_result_t oa_move(neo_gactor_t *a,neo_genv_t *e,uint16_t x) {   /* 045C2A / 045C22 */
    k446d4(a,x,W(0x2c),0x200,4);
    START(0x24315a);SW(0x66,0x65);SW(0x1e,0x65);SL(0,0x45c62);
    return op_actor(a,e);
}
static ga_result_t op_actor(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0x45608: return oa_cue(a,e);
    case 0x45630: SL(0,0x45636);
        /* fall through */
    case 0x45636: return oa_cue(a,e);
    case 0x45638: case 0x4563e: return oa_anim(a,e);
    case 0x4564c: return oa_wait(a,e);
    case 0x45656: {
        if (B(0x8d)) {
            neo_gactor_t *c=creator(a,e);
            if (!c) return GA_UNSUPPORTED;
            if ((uint8_t)ga_w(c,0x8a)==B(0x8c)) ga_sb(c,0x91,1);
            SL(0,0x4567a);
        }
    }   /* fall through */
    case 0x4567a: ga_project(a,e);if (STEP()<0) return GA_UNSUPPORTED;return oa_cue(a,e);
    case 0x45688: return oa_fall(a,e);
    case 0x4569e:
        SL(0x34,L(0x34)+L(0x76));
        ga_move(a,e);
        if (STEP()<0) return GA_UNSUPPORTED;
        if ((uint16_t)(W(0x40)-0x100u)>=0x100u) SL(0,0x456c8);
        /* fall through */
    case 0x456c8: return oa_cue(a,e);
    case 0x456cc:
        START(0x242c58);SW(0x66,0xc8);SW(0x1e,0xc8);ga_palette(a,0xea);ga_palette(a,0xeb);ga_palette(a,0xec);
        SB(0x14,B(0x19));SB(0x8b,0);ga_project(a,e);SB(0x8b,2);SL(0,0x4572e);
        /* fall through */
    case 0x4572e:
        if (S16(W(0x3c))<0x128) {SB(0x8a,1);SL(0,0x45744);}
        /* fall through */
    case 0x45744:
        ga_project(a,e);if (STEP()<0) return GA_UNSUPPORTED;
        if (B(0x8a)) {
            SB(0x8a,B(0x8a)-1u);
            if (!B(0x8a)) {
                SB(0x8a,0x2c);
                uint8_t v=(uint8_t)(B(0x8b)-1u);SB(0x8b,v);
                if (!(v&0x80u)) sound(e,0x1141);
            }
        }
        return oa_cue(a,e);
    case 0x4577c: START(0x242c86);return oa_anim(a,e);
    case 0x45790: START(0x242d54);return oa_wait(a,e);
    case 0x457a4: START(0x242d90);return oa_anim(a,e);
    case 0x457b8: {   /* 0AE220 (1, 0): the white screen 0AE1DC after the script actor */
        neo_gactor_t *w=after_script(a,e,0xae1dc);
        if (w) {ga_sb(w,0x70,0);ga_sw(w,0x6e,1);}
        SL(0,0x45636);return oa_cue(a,e);
    }
    case 0x457c6: sound(e,0x10d0);ga_floor(a,e);START(0x242dbe);SW(0x66,0x65);SW(0x1e,0x65);return oa_fall(a,e);
    case 0x457f6: START(0x242e10);SW(0x66,0xc7);SW(0x1e,0xc7);ga_palette(a,0xed);ga_palette(a,0xee);SB(0x14,B(0x19));return oa_anim(a,e);
    case 0x45838: START(0x242e1c);return oa_anim(a,e);
    case 0x4584c: {
        sound(e,0x102f);START(0x242e4a);
        neo_gactor_t *c=k1da40(a,e,0x45d0e);if (c) {ga_sw(c,0x66,0x12c);ga_sw(c,0x1e,0x12c);}
        c=k1da40(a,e,0x45d20);if (c) {ga_sw(c,0x66,0x12c);ga_sw(c,0x1e,0x12c);}
        return oa_anim(a,e);
    }
    case 0x45896: {
        sound(e,0x1021);
        neo_gactor_t *c=k1da40(a,e,0x45d20);if (c) {ga_sw(c,0x66,0x12c);ga_sw(c,0x1e,0x12c);}
        c=k1da40(a,e,0x45d32);if (c) {ga_sw(c,0x66,0x12c);ga_sw(c,0x1e,0x12c);}
        SL(0,0x45636);return oa_cue(a,e);
    }
    case 0x458d0: {neo_gactor_t *m=node(e,L(0x6e));if (!m) return GA_UNSUPPORTED;ga_place(a,e,0,(int16_t)0xffc8,self(m));SL(0,0x45636);return oa_cue(a,e);}
    case 0x458ea: {
        sound(e,0x10cf);
        neo_gactor_t *c=child(a,e,0xbea82,0x1c);if (c) ga_sb(c,0x6e,0);   /* 0BEA6A */
        neo_gactor_t *m=node(e,L(0x6e));if (!m) return GA_UNSUPPORTED;
        k446d4(m,0x200,0x108,0x280,0xc);
        SL(0,0x45636);return oa_cue(a,e);
    }
    case 0x45918: {neo_gactor_t *h=node(e,L(0x72));if (!h) return GA_UNSUPPORTED;ga_sb(h,0x6e,2);ga_sb(h,0x6f,1);SL(0,0x45636);return oa_cue(a,e);}
    case 0x45930: {neo_gactor_t *h=node(e,L(0x72));if (!h) return GA_UNSUPPORTED;ga_sb(h,0x6f,6);SL(0,0x45636);return oa_cue(a,e);}
    case 0x4593e: {neo_gactor_t *h=node(e,L(0x72));if (!h) return GA_UNSUPPORTED;ga_sb(h,0x6f,2);SL(0,0x45636);return oa_cue(a,e);}
    case 0x4594c: {neo_gactor_t *h=node(e,L(0x72));if (!h) return GA_UNSUPPORTED;ga_sb(h,0x6e,5);SL(0,0x4595c);}
        /* fall through */
    case 0x4595c: {
        neo_gactor_t *h=node(e,L(0x72));if (!h) return GA_UNSUPPORTED;
        if (!(ga_b(h,0x6e)&0x80u)) return oa_cue(a,e);
        START(0x243556);SW(0x66,0x63);SW(0x1e,0x63);ga_palette(a,0x10a);
        return oa_wait(a,e);
    }
    case 0x4599a: START(0x243604);SL(0,0x459b0);
        /* fall through */
    case 0x459b0: {
        ga_project(a,e);
        int r=STEP();if (r<0) return GA_UNSUPPORTED;
        if (r) {
            stop_sound(e,0x10d0);
            neo_gactor_t *h=node(e,L(0x72));if (!h) return GA_UNSUPPORTED;
            ga_sb(h,0x6e,4);SL(0,0x459de);
        }
        return oa_cue(a,e);
    }
    case 0x459de: {
        neo_gactor_t *h=node(e,L(0x72));if (!h) return GA_UNSUPPORTED;
        if (!(ga_b(h,0x6e)&0x80u)) return oa_cue(a,e);
        neo_gactor_t *c=creator(a,e);if (!c) return GA_UNSUPPORTED;
        if ((uint8_t)ga_w(c,0x8a)==B(0x8c)) ga_sb(c,0x91,1);
        SL(0,0x45636);return oa_cue(a,e);
    }
    case 0x45a08: {
        sound(e,(uint16_t)(0x0a00u|(uint8_t)(0x40u+0x20u)));                 /* 01228C (0x40) */
        neo_gactor_t *m=node(e,L(0x6e));if (!m) return GA_UNSUPPORTED;
        k446d4(m,0x200,0xa0,0x100,4);
        SL(0,0x45636);return oa_cue(a,e);
    }
    case 0x45a30: START(0x2432a6);ga_palette(a,0x20e);SW(0x66,0x190);SW(0x1e,0x190);return oa_anim(a,e);
    case 0x45a5e: START(0x2432e8);return oa_anim(a,e);
    case 0x45a72: START(0x2433ca);SL(0,0x45a9c);goto k45a9c;
    case 0x45a86: START(0x24348e);SL(0,0x45a9c);
        /* fall through */
    case 0x45a9c: k45a9c: {
        ga_project(a,e);if (STEP()<0) return GA_UNSUPPORTED;
        if (!BIT(0x5f,3)) {
            uint8_t d0=B(0x92);
            if (d0) {d0--;if (!d0) {SET(0x5f,3);d0=0x1e;}SB(0x92,d0);}
            return oa_cue(a,e);
        }
        uint8_t d0=B(0x92);
        if (!d0) return GA_DELETE;                                          /* (cartridge check) 004188 */
        SB(0x92,d0-1u);return oa_cue(a,e);
    }
    case 0x45b72: case 0x45b78:
        if (L(0)==0x45b72u) SB(0x15,B(0x15)^1u);                            /* 01ADAA */
        START(0x242e56);ga_palette(a,0xc0);ga_palette(a,0xef);ga_palette(a,0x868);SW(0x66,0xc8);SW(0x1e,0xc8);SB(0x14,B(0x19));
        SL(0,0x45636);return oa_cue(a,e);
    case 0x45bc8: START(0x242efc);return oa_wait(a,e);
    case 0x45bdc: START(0x243004);return oa_wait(a,e);
    case 0x45bf0: START(0x24306e);return oa_wait(a,e);
    case 0x45c04: sound(e,0x10d1);START(0x243100);return oa_wait(a,e);
    case 0x45c22: return oa_move(a,e,0x1f8);
    case 0x45c2a: return oa_move(a,e,0x208);
    case 0x45c62:
        k446f4(a,0x28);ga_move(a,e);if (STEP()<0) return GA_UNSUPPORTED;
        if (!B(0x81)) {sound(e,0x10d0);START(0x243200);SL(0,0x45688);}
        return oa_cue(a,e);
    case 0x45ca0: START(0x243656);ga_palette(a,0x506);SW(0x66,0x1f4);SW(0x1e,0x1f4);return oa_anim(a,e);
    case 0x45cce: SL(0,0x45636);return oa_cue(a,e);
    case 0x45cd2: ga_palette(a,0x113);START(0x24855c);SL(0,0x45cf6);
        /* fall through */
    case 0x45cf6: {ga_project(a,e);int r=STEP();if (r<0) return GA_UNSUPPORTED;return r?GA_DELETE:GA_MORE;}
    }
    STOP(L(0));
}
/* 0BEA82 (by 0BEA6A): the arena's palette 23 fades toward 506 over 128 updates (009CCE with +6E / 4). */
static ga_result_t fade_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint8_t d2=(uint8_t)(B(0x6e)+1u);SB(0x6e,d2);
    if (d2>0x80u) return GA_DELETE;                                         /* 004188 */
    k9cce(e,0x23,0x506,(uint8_t)(d2>>2));
    return GA_MORE;
}
/* ---- the flames 0436B6 (by the arm's program call 044D0C) and 043860 (044D42) ------------- */
/* 01AF9A: the nearest player in play (P1 on ties or with none; carry (true) when none). */
static bool nearest(neo_gactor_t *a,const neo_genv_t *e,uint32_t *n) {
    unsigned m=e->players.active_mask&0xffu;
    if (!m) {*n=P1_NODE;return true;}
    if (m==1u) {*n=P1_NODE;return false;}
    if (m==2u) {*n=P2_NODE;return false;}
    uint16_t d1=(uint16_t)(e->players.p1x-W(0x3c)),d2=(uint16_t)(e->players.p2x-W(0x3c));
    if (d1&0x8000u) d1=(uint16_t)(0u-d1);
    if (d2&0x8000u) d2=(uint16_t)(0u-d2);
    *n=S16(d2)<S16(d1)?P2_NODE:P1_NODE;
    return false;
}
static bool k43792(neo_gactor_t *a,neo_genv_t *e) {   /* the flame's program */
    if (!B(0x80)) return ga_anim_start(a,e,0x242270,0x100);
    uint32_t n;uint16_t d0;
    if (nearest(a,e,&n)) d0=4;
    else {
        int16_t dx=(int16_t)((n==P2_NODE?e->players.p2x:e->players.p1x)-W(0x3c)),dy=(int16_t)((n==P2_NODE?e->players.p2y:e->players.p1y)-W(0x40));
        uint8_t v=(uint8_t)((uint8_t)((uint8_t)(neo_angle(dx,dy)+4u)>>3)-8u);   /* 043804 */
        uint8_t lo=B(0x80)==1u?0xf:0xc,hi=B(0x80)==1u?0x11:0x14;
        if (v<lo) v=lo;
        if (v>hi) v=hi;
        v=(uint8_t)(v+0xf4u);
        if (v>8u) {ga_fail(e,0x4384a);return false;}                    /* TRAP #15 */
        d0=v;
    }
    SW(0x86,d0);
    int16_t i=(int16_t)(d0-4u);
    if (i<0) {i=(int16_t)-i;CLR(0x15,0);} else SET(0x15,0);
    return ga_anim_start(a,e,rl(e,0x44dd0u+4u*(uint16_t)i),0x100);
}
static ga_result_t fl_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0x436b6:
        on_creator(a,e,0,0);
        if (!k43792(a,e)) return GA_UNSUPPORTED;
        ga_palette(a,0xe8);ga_palette(a,0xe9);
        if (!B(0x80)) {SW(0x66,0x64);SW(0x1e,0x64);} else {SW(0x66,0x12c);SW(0x1e,0x12c);}
        SB(0x88,0);SL(0,0x43712);
        /* fall through */
    case 0x43712: {
        ga_project(a,e);
        int r=STEP();if (r<0) return GA_UNSUPPORTED;
        if (r) {neo_gactor_t *c=creator(a,e);if (!c) return GA_UNSUPPORTED;ga_sb(c,0x60,1);return GA_DELETE_TREE;}   /* 0041AC */
        if (B(0x88)) {   /* 043740: four rows 044EAC along the flame */
            int16_t d2=S16(rw(e,0x44fb8u+2u*W(0x86)));
            uint16_t d1=(uint16_t)((e->video&1u)+1u);
            do {
                uint32_t x=L(0x3c),y=L(0x40);
                int16_t t=(int16_t)((int16_t)d1*d2);
                SW(0x3c,W(0x3c)+(uint16_t)(t>>4));
                SW(0x40,W(0x40)-(uint16_t)(d1<<4));
                ga_attack(a,e,0x44eac);
                SL(0x40,y);SL(0x3c,x);
                d1+=2;
            } while (d1<0xa);
        }
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x43860:
        SW(0x2c,0x30);ga_palette(a,0xe8);SW(0x66,0x190);SW(0x1e,0x190);START(0x242242);SL(0,0x43896);
        /* fall through */
    case 0x43896: ga_project(a,e);if (STEP()<0) return GA_UNSUPPORTED;return GA_MORE;
    }
    STOP(L(0));
}
/* ---- the troopers 0438A4 (by the hatch 04307E, after the script actor), their shot makers 04416E and shots 0441CA ---- */
/* 03B332: the speeds to cover (D0, D1) in D3 updates under the gravity D2. */
static void k3b332(int16_t d0,int16_t d1,uint32_t d2,uint16_t d3,uint32_t *vx,uint32_t *vy) {
    uint32_t g=(d2&0x80000000u)?0u-d2:d2;
    if ((int16_t)d3<2) d3=2;
    uint16_t q=(uint16_t)(0x10000u/d3);
    uint32_t x=(uint32_t)((int32_t)d0*(int16_t)q),y=(uint32_t)((int32_t)d1*(int16_t)q);
    uint32_t t=(g&0xffffu)*d3+(((g>>16)*d3)<<16);
    t=(uint32_t)((int32_t)t>>1);
    if (d2&0x80000000u) t=0u-t;
    *vx=x;*vy=y-t;
}
static bool in_play(neo_genv_t *e,uint8_t p,uint32_t *n) {*n=(p&1u)?P2_NODE:P1_NODE;return (e->players.active_mask>>(p&1u))&1u;}   /* 01AEB0 */
static uint8_t k43d86(uint8_t d0) {static const uint8_t t[16]={8,8,8,8,0,0,0,0,0,1,2,3,4,5,6,7};return t[((uint8_t)(d0+8u)>>4)&0xfu];}
static uint8_t k43da4(uint8_t d0) {return (uint8_t)((uint8_t)(d0+8u)<<4);}
static void k43daa(neo_gactor_t *a,neo_genv_t *e) {SB(0xa1,ga_rand(e)&3u);}
static uint8_t k43dba(neo_gactor_t *a,neo_genv_t *e,uint8_t d0,uint8_t d1) {
    static const uint8_t t1[9]={0x50,0x46,0x3c,0x32,0x28,0x1e,0x14,0x0a,0},t2[9]={0x28,0x32,0x3c,0x32,0x28,0,0,0,0};
    uint8_t d3=(uint8_t)(d1-d0);
    if (d3&0x80u) d3=(uint8_t)-d3;
    if (d3>8u) {ga_fail(e,0x43dd0);return 0;}
    uint8_t d2=t1[d3];
    d3=d0;
    if (!(B(0xa0)&1u)) d3=(uint8_t)(8u-d3);
    if (d3>8u) {ga_fail(e,0x43e02);return 0;}
    d2=(uint8_t)(d2+t2[d3]);
    if (d0==B(0xa1)) d2=(uint8_t)(d2+0x1e);
    return d2;
}
/* 043C74: the best of the nine directions toward player node n (by the walls, the floor and +9C) into +9A. */
static void k43c74(neo_gactor_t *a,neo_genv_t *e,uint32_t n) {
    uint16_t d5=0x1ff;
    int16_t x=S16(pw(e,n,0x3c));
    if (x>0xf0) d5&=0x1e0;
    if (x<0x50) d5&=0xf;
    if (S16(W(0x3c))>0x110) d5&=0x1e0;
    if (S16(W(0x3c))<0x30) d5&=0xf;
    if (S16(W(0x40))>0x1d0) d5&=0x183;
    if (B(0x9c)) d5&=0x1c7;
    if (!d5) d5=0x1ff;
    uint8_t d1=k43d86(neo_angle((int16_t)(x-W(0x3c)),(int16_t)(pw(e,n,0x40)+0x10u-W(0x40))));
    uint8_t d6=0;
    for (int d0=8;d0>=0;d0--) {
        if (!((d5>>d0)&1u)) continue;
        uint8_t d2=k43dba(a,e,(uint8_t)d0,d1);
        if (d6<d2) {d6=d2;SB(0x9a,(uint8_t)d0);}
    }
    SB(0x9a,k43da4(B(0x9a)));
}
/* 04482E: (D0, D1) of the screen in the root's coordinates (01AC4C with no parent; the root's previous position). */
static void k4482e(neo_genv_t *e,int16_t *x,int16_t *y) {
    neo_contact_parent_t r;
    if (!ga_parent(e,ROOT,&r)) {ga_fail(e,0x4482e);return;}
    *x=(int16_t)((((uint32_t)(uint16_t)*x<<16)-(uint32_t)r.previous_x)>>16);
    *y=(int16_t)((((uint32_t)(uint16_t)*y<<16)-(uint32_t)r.previous_y)>>16);
}
/* 043D22: the point at the distance (002F1A, clamped to +98..+96) from player n along +9A, as the block's words. */
static void k43d22(neo_gactor_t *a,neo_genv_t *e,uint32_t n,int16_t *ox,int16_t *oy) {
    int16_t dx=(int16_t)(pw(e,n,0x3c)-W(0x3c)),dy=(int16_t)(pw(e,n,0x40)+0x10u-W(0x40));
    uint16_t ax=(uint16_t)(dx<0?-dx:dx),ay=(uint16_t)(dy<0?-dy:dy);
    uint16_t d0=ay>ax?(uint16_t)((ax>>1)+ay):(uint16_t)(ax+(ay>>1));   /* 002F1A */
    d0&=0xfff0u;
    if (d0<W(0x98)) d0=W(0x98);
    if (d0>W(0x96)) d0=W(0x96);
    uint32_t vx,vy;ga_polar((uint32_t)d0<<8,B(0x9a),&vx,&vy);
    *ox=(int16_t)(dx-(int16_t)(S32(vx)>>8)+W(0x28));*oy=(int16_t)(dy-(int16_t)(S32(vy)>>8)+W(0x2c));
}
static ga_result_t tr_tail(neo_gactor_t *a,neo_genv_t *e) {   /* 044080 */
    if (ga_cull(a,e,0x44ebc)) return GA_DELETE_TREE;                    /* 01945C -> 0041AC */
    int16_t dx,dy;
    if (ga_register(a,e,&dx,&dy)) {
        SB(0x9c,B(0x9c)|6u);
        if (B(0x5a)==0xcu) SL(0,0x43f32);
        if (BIT(0x5f,7)) SL(0,0x43f32);
        SB(0x5a,0);
    }
    if (B(0x9c)) SB(0x9c,B(0x9c)-1u);
    k446f4(a,0x28);
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
static void tr_ranks(neo_gactor_t *a,neo_genv_t *e,uint32_t ti,uint32_t lo,uint32_t hi,uint32_t t8e,uint32_t t9d) {
    uint16_t i=rank(a,e,rl(e,ti+(uint32_t)k4463e(a,e)));
    if (i>3u) {ga_fail(e,ti);return;}                                   /* TRAP #15 */
    uint32_t t=(k44b84(e)?hi:lo)+4u*i;
    SW(0x96,rw(e,t));SW(0x98,rw(e,t+2));
    SW(0x8e,rank(a,e,rl(e,t8e+(uint32_t)k4463e(a,e))));
    SB(0x9d,(uint8_t)rank(a,e,rl(e,t9d+(uint32_t)k4463e(a,e))));
}
static ga_result_t tr_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t n;
    switch (L(0)) {
    case 0x438a4: {   /* 043F7C */
        START(0x242404);
        if (B(0x80)==6u || B(0x80)==8u) {ga_palette(a,0x184);ga_palette(a,0x185);ga_palette(a,0x186);}
        else {ga_palette(a,0xd2);ga_palette(a,0xd6);ga_palette(a,0xd4);}
        SB(0x14,B(0x19));SW(0x66,0x8800);SW(0x1e,0x8800);CLR(0x15,0);SB(0x81,0);
        bool v=k44b84(e);
        SW(0x4e,rank(a,e,rl(e,(v?0x4519au:0x45186u)+4u*(uint32_t)(uint8_t)(B(0x80)-4u))));
        SB(0x9c,0);
        uint16_t d0=rank(a,e,rl(e,0x450f6u+(uint32_t)k4463e(a,e)));
        SW(0x92,S16(d0)<0x38?0x38:d0);SW(0x90,0x5a);SL(0x86,self(a));SB(0x9f,0);
        SL(0,0x438ae);
    }   /* fall through */
    case 0x438ae: {
        ga_project(a,e);
        int r=STEP();if (r<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x438c8);
        return tr_tail(a,e);
    }
    case 0x438c8: {
        to_parent(a,e,ROOT);ga_project(a,e);ga_project(a,e);                    /* 01AD52, 01ABC8 */
        START(0x242642);SL(0x50,0x44e1c);
        {unsigned m=e->players.active_mask&0xffu;SB(0x9b,m==3u?(uint8_t)(ga_rand(e)&1u):(uint8_t)(m-1u));}   /* 0440DE */
        switch (B(0x80)) {
        case 4: tr_ranks(a,e,0x450a6,0x2c4df2,0x2c688c,0x450c6,0x450b6);break;
        case 5: SB(0x9d,(uint8_t)rank(a,e,rl(e,0x45106u+(uint32_t)k4463e(a,e))));goto k43ae4;
        case 6: tr_ranks(a,e,0x45136,0x2c566a,0x2c7104,0x45156,0x45146);break;
        default: tr_ranks(a,e,0x451de,0x2c5c04,0x2c769e,0x4520e,0x451ee);break;
        }
        if (e->unsupported) return GA_UNSUPPORTED;
        k43daa(a,e);SB(0x9a,0x80);
        if (in_play(e,B(0x9b),&n)) k43c74(a,e,n);
    }   /* fall through */
    k43ae4:
    case 0x43ae4:
        SW(0x8e,rank(a,e,rl(e,0x45116u+(uint32_t)k4463e(a,e))));
        if (!(ga_rand(e)&0x80u)) SET(0x9f,0); else CLR(0x9f,0);
        SL(0,0x43b1c);
        /* fall through */
    case 0x43b1c: {
        bool carry;   /* 043B68 */
        if (W(0x8e)) {SW(0x8e,W(0x8e)-1u);carry=false;}
        else {
            carry=true;
            if (B(0x80)==5u) {
                SW(0x90,W(0x90)-1u);
                if (S16(W(0x90))>=0) {
                    if (!in_play(e,B(0x9b),&n)) carry=false;
                    else {int16_t d=(int16_t)(pw(e,n,0x3c)-W(0x3c));if (d<0) d=(int16_t)-d;if ((uint16_t)d<0x30u) carry=false;}
                }
            }
            if (carry) SW(0x90,0x5a);
        }
        if (carry) SL(0,0x43e36);
        if (!in_play(e,B(0x9b),&n)) SL(0,0x43ee0);
        else if (B(0x80)==5u) {   /* 043BC0 */
            int16_t px=S16(pw(e,n,0x3c)),d1=(int16_t)(BIT(0x9f,1)?-(int16_t)W(0x92):(int16_t)W(0x92));
            int16_t d0=(int16_t)(px+d1);
            if (d0>0x130) SET(0x9f,1);
            if (d0<0x10) CLR(0x9f,1);
            d1=(int16_t)(BIT(0x9f,1)?-(int16_t)W(0x92):(int16_t)W(0x92));
            d0=(int16_t)(px+d1);
            int16_t y=(int16_t)(pw(e,n,0x40)+(BIT(0x9f,0)?0x1cu:8u));
            k4482e(e,&d0,&y);
            k446d4(a,(uint16_t)d0,(uint16_t)y,0x600,0xc);
        } else {   /* 043C32 */
            if ((B(0xa0)&0xfu)==(e->video&0xfu)) k43c74(a,e,n);
            int16_t x,y;k43d22(a,e,n,&x,&y);
            k446d4(a,(uint16_t)x,(uint16_t)y,0x600,0xc);
            if (!(ga_rand(e)&0x1fu)) k43daa(a,e);
        }
        ga_move(a,e);
        if (STEP()<0) return GA_UNSUPPORTED;
        return tr_tail(a,e);
    }
    case 0x43e36:
        SB(0x81,0);
        if (S16(W(0x3c))<0x10 || S16(W(0x3c))>0x130 || S16(W(0x40))>0x1f0) goto k43eb4;
        {
            neo_gactor_t *c=part(a,e,0x4416e);
            if (!c) goto k43eb4;
            ga_sb(c,0x9b,B(0x9b));ga_sl(c,0x86,L(0x86));
            sound(e,0x10d1);SB(0x60,0);SL(0,0x43ea8);
        }
        /* fall through */
    case 0x43ea8:
        if (!B(0x60)) goto k43ed0;
        SB(0x9e,B(0x9e)+1u);
    k43eb4:
        if (S8(B(0x9e))<S8(B(0x9d))) SL(0,0x43ae4); else SL(0,0x43ee0);
    k43ed0:
        ga_project(a,e);if (STEP()<0) return GA_UNSUPPORTED;
        return tr_tail(a,e);
    case 0x43ee0: {
        uint32_t vx,vy;
        k3b332(BIT(0x15,0)?0x40:-0x40,0x50,0x6000,0x28,&vx,&vy);
        SL(0x30,vx);SL(0x34,vy);SL(0x76,0x6000);SB(0x81,0);SL(0,0x43f1a);
    }   /* fall through */
    case 0x43f1a:
        SL(0x34,L(0x34)+L(0x76));ga_move(a,e);if (STEP()<0) return GA_UNSUPPORTED;
        return tr_tail(a,e);
    case 0x43f32:
        sound(e,0x102c);
        if (e->kill_children) e->kill_children(e->ctx,a);                      /* 004192 */
        START(0x242908);SB(0x14,B(0x1a));SB(0x81,0);SL(0,0x43f64);
        /* fall through */
    case 0x43f64: {
        ga_project(a,e);
        int r=STEP();if (r<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x440d6);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x440d6: return GA_DELETE_TREE;                                       /* 0041AC */
    /* 04416E: the shot maker (its program calls 044BC4); tells its trooper (+60) at the end. */
    case 0x4416e: on_creator(a,e,0,0);SW(0x66,0xe0ed);SW(0x1e,0xe0ed);START(0x242a00);ga_palette(a,0x88e);SL(0,0x441a8);
        /* fall through */
    case 0x441a8: {
        ga_project(a,e);
        int r=STEP();if (r<0) return GA_UNSUPPORTED;
        if (r) {neo_gactor_t *c=creator(a,e);if (!c) return GA_UNSUPPORTED;ga_sb(c,0x60,1);return GA_DELETE;}
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    /* 0441CA: the shot along +9A at the speed +94 (attack rows 044E28 by direction). */
    case 0x441ca: {
        START(0x242a94);ga_palette(a,0x88e);SW(0x66,0xe0ef);SW(0x1e,0xe0ef);CLR(0x15,0);SET(0x15,5);
        ga_project(a,e);
        uint8_t ang=B(0x9a)&0xf0u;
        uint8_t d0=k43d86(ang);
        if (d0>=5u) {d0=(uint8_t)(8u-d0);SET(0x15,0);}
        SB(0x9a,d0);
        if (d0>4u) STOP(0x44232);                                             /* TRAP #15 */
        SL(0x8a,rl(e,0x44e28u+4u*d0));
        uint32_t vx,vy;ga_polar((uint32_t)(int32_t)S16(W(0x94))<<8,ang,&vx,&vy);
        SL(0x30,vx);SL(0x34,vy);SL(0,0x44270);
    }   /* fall through */
    case 0x44270: {
        ga_air(a,e);                                                          /* 0143E4 */
        if (STEP()<0) return GA_UNSUPPORTED;
        if (ga_cull_rect(a,e,0x44ec4)) return GA_DELETE;                      /* 019504, (cartridge check) 004188 */
        if (ga_attack(a,e,L(0x8a))) SL(0,0x4439e);
        if (BIT(0x15,7)) SL(0,0x44356);
        {   /* its trooper's +5F (a node gone keeps its bytes: the world's or the RAM's) */
            neo_gactor_t *t=e->actor?e->actor(e->ctx,L(0x86)):NULL;
            const uint8_t *m=t?t->m:e->node?e->node(e->ctx,L(0x86)):NULL;
            if (!m) STOP(0x44340);
            if (m[0x5f]&0x80u) SL(0,0x4439e);
        }
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x44356: START(0x242b52);ga_palette(a,0xd5);SW(0x66,0x1001);SW(0x1e,0x1001);SL(0,0x44386);
        /* fall through */
    case 0x44386: {ga_project(a,e);int r=STEP();if (r<0) return GA_UNSUPPORTED;return r?GA_DELETE:GA_MORE;}
    case 0x4439e:
        SB(0x1d,0);SW(0x66,0x1001);SW(0x1e,0x1001);SL(0x50,0);                 /* 009F88 */
        SL(0,0x1c706);return neo_region_tick(a,e);                              /* JMP 01C706 */
    }
    STOP(L(0));
}
/* ---- the second form's spawner 045EF6 and its walkers 046112 / 04611C (group 6) ------------------------------- */
/* 0460F4: the new node (group 6) at (D0 - the root's x, D1) of the root. */
static void k460f4(neo_genv_t *e,neo_gactor_t *c,int16_t d0,int16_t d1) {
    ga_sb(c,0x69,6);
    neo_contact_parent_t r;
    if (!ga_parent(e,ROOT,&r)) {ga_fail(e,0x460f4);return;}
    ga_place(c,e,(int16_t)(d0-(int16_t)((uint32_t)r.x>>16)),d1,ROOT);
}
static uint16_t k460c2(neo_genv_t *e,neo_gactor_t *c) {   /* the left or right side at random */
    bool right=ga_rand(e)&1u;
    if (c) k460f4(e,c,right?0x180:-0x40,0x38);
    return right?1:0;
}
static void k45fc4(neo_gactor_t *a,neo_genv_t *e) {
    switch (ga_rand_range(e,4)) {
    case 0: k460c2(e,after_script(a,e,0x4611c));return;
    case 1: k460c2(e,after_script(a,e,0x46112));return;
    case 2: {
        neo_gactor_t *c=after_script(a,e,0x8dd2c);
        uint16_t d0=k460c2(e,c);
        if (!c) {ga_rand(e);ga_rand(e);return;}
        ga_sw(c,0x6e,d0^1u);ga_sw(c,0x70,0x708);ga_sw(c,0x72,ga_rand(e)&1u);ga_sw(c,0x74,(uint16_t)(((ga_rand(e)&3u)+1u)*0x10u));
        return;
    }
    default: {
        neo_gactor_t *c=after_script(a,e,0x9ba40);
        k460c2(e,c);
        if (c) ga_sw(c,0x6e,0);
        uint16_t d0=(uint16_t)((ga_rand(e)&0xffu)+0x20u);
        neo_contact_parent_t r;
        if (!ga_parent(e,ROOT,&r)) {ga_fail(e,0x460a6);return;}
        if (c) {ga_sw(c,0x70,(uint16_t)((((uint32_t)d0<<16)-(uint32_t)r.previous_x)>>16));ga_sw(c,0x72,0xffff);ga_sw(c,0x74,0);}
        return;
    }
    }
}
static bool k4647c(neo_gactor_t *a) {return ((S16(W(0x30))>=0?1u:0u)^B(0x15))&1u;}   /* moving against its facing */
static ga_result_t wk_tail(neo_gactor_t *a,neo_genv_t *e) {   /* 0463AC */
    if (!B(0x90)) {
        if (e->flags && e->flags->m6_boss) SL(0,0x46370);                      /* 044930 */
        int16_t dx,dy;ga_register(a,e,&dx,&dy);
    }
    if (BIT(0x5f,7)) SL(0,0x46336);
    if (ga_cull(a,e,0x4675c)) return GA_DELETE;                                 /* 01945C, 004188 */
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
static ga_result_t wk_anim(neo_gactor_t *a,neo_genv_t *e,uint32_t next) {   /* 01436A, 00427A, at the end `next` */
    ga_stationary(a,e);
    int r=STEP();if (r<0) return GA_UNSUPPORTED;
    if (r) SL(0,next);
    return wk_tail(a,e);
}
static ga_result_t wk_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0x45ef6: SB(0x86,2);SL(0,0x45f02);
        /* fall through */
    case 0x45f02: {
        if (e->flags && e->flags->m6_boss) return GA_DELETE;                    /* 044930, (cartridge check) 004188 */
        uint8_t d0=(uint8_t)(B(0x86)-k4465e(e,9));                               /* 019DCA(6) */
        while (d0) {k45fc4(a,e);if (e->unsupported) return GA_UNSUPPORTED;d0--;}
        return GA_MORE;
    }
    case 0x46112: case 0x4611c:
        SB(0x93,L(0)==0x4611cu?1:0);
        SET(0x15,5);ga_floor(a,e);to_parent(a,e,ROOT);ga_project(a,e);           /* 0463F0: 0146E8, 01AD52 */
        SL(0x30,0);SL(0x34,0);SL(0x62,0);ga_palette(a,0x102);SW(0x66,0x8820);SW(0x1e,0x8820);
        SL(0x50,0x46740);SW(0x4e,1);SB(0x90,0);
        /* fall through */
    case 0x46126: {   /* 046436 */
        SL(0x30,(uint32_t)((int32_t)(int16_t)((ga_rand(e)&0xffu)-0x7fu+0x200u)<<8));
        int16_t d0=(int16_t)ga_rand_range(e,0x70);
        if (S16(W(0x3c))>=0xa0) d0=(int16_t)-d0;
        d0=(int16_t)(d0+0xa0);SW(0x8e,(uint16_t)d0);
        if (!(d0>S16(W(0x3c)))) SL(0x30,0u-L(0x30));
    }
    k4612a:
        if (k4647c(a)) {START(0x243728);SL(0,0x4618c);goto k4618c;}
        /* fall through */
    case 0x46132: START(0x243662);SL(0,0x46148);
        /* fall through */
    case 0x46148: {
        int16_t x0=S16(W(0x3c));
        ga_air(a,e);
        if (STEP()<0) return GA_UNSUPPORTED;
        int16_t d0=(int16_t)(x0-W(0x8e)),d1=(int16_t)(W(0x3c)-W(0x8e));
        if (!((int16_t)(d0^d1)>0)) SL(0,0x461d6);
        return wk_tail(a,e);
    }
    case 0x46176: START(0x243728);SL(0,0x4618c);
        /* fall through */
    case 0x4618c: k4618c: return wk_anim(a,e,0x46132);
    case 0x461d6: {
        bool c=(((S16(W(0x3c))<=0xa0)?1u:0u)^B(0x15))&1u;                        /* 0464E2 */
        if (!c) goto k4620e;
        START(0x243728);SL(0,0x461f4);
    }   /* fall through */
    case 0x461f4: return wk_anim(a,e,0x4620e);
    case 0x4620e: k4620e:
        if (!B(0x93)) {START(0x24396a);SL(0,0x4631c);return wk_anim(a,e,0x46126);}
        START(0x243744);SL(0,0x46230);
        /* fall through */
    case 0x46230: return wk_anim(a,e,0x4624a);
    case 0x4624a: SB(0x92,0);SB(0x91,(uint8_t)((ga_rand(e)&3u)+1u));goto k4625e;
    case 0x46256: {
        uint8_t v=(uint8_t)(B(0x91)-1u);SB(0x91,v);
        if (v&0x80u) {START(0x24378e);SL(0,0x462bc);return wk_anim(a,e,0x462d6);}
    }
    k4625e:
        START(0x243764);SL(0,0x46274);
        /* fall through */
    case 0x46274: {
        ga_stationary(a,e);
        int r=STEP();if (r<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x46256);
        if (S16(W(0x3c))<=0x30) SB(0x92,B(0x92)+1u);                             /* 0464B0 */
        if (S16(W(0x3c))>=0x110) SB(0x92,B(0x92)+1u);
        if (!ga_players_in(a,e,0x46764)) SB(0x92,B(0x92)+1u);                   /* 0464CE: 01AFFC */
        if (S8(B(0x92))>0x14) SL(0,0x462d6);
        return wk_tail(a,e);
    }
    case 0x462bc: return wk_anim(a,e,0x462d6);
    case 0x462d6: START(0x2438a4);SL(0,0x462ec);
        /* fall through */
    case 0x462ec: return wk_anim(a,e,0x46126);
    case 0x4631c: return wk_anim(a,e,0x46126);
    case 0x46336: {
        SB(0x69,0xff);                                                           /* 019DFC */
        if (B(0x93)) {neo_gactor_t *c=child(a,e,0x46510,0x2a);if (c) ga_place_from(c,a,e,0,0x18);}
        if (!B(0x5a)) return GA_MORE;                                            /* 0407B6 */
        SB(0x73,0xff);
        int r=neo_m3enemy_death_406b6(a,e);
        return r<0?GA_UNSUPPORTED:r?GA_DELETE:GA_MORE;
    }
    case 0x46370:
        SL(0x30,0x20000);SW(0x8e,S16(W(0x3c))>=0xa0?0x1000:0xf000);
        if (!(S16(W(0x8e))>S16(W(0x3c)))) SL(0x30,0u-L(0x30));
        SB(0x90,1);goto k4612a;
    /* 046510: the walker's bomb falling until the ground. */
    case 0x46510: SL(0x34,0x22222);SL(0x86,0xffffdb98u);SL(0x30,0);START(0x2438c4);ga_palette(a,0x102);SL(0,0x4654c);
        /* fall through */
    case 0x4654c:
        ga_air(a,e);
        if (BIT(0x15,6)) return GA_DELETE;                                      /* (cartridge check) 004188 */
        SL(0x34,L(0x34)+L(0x86));
        if (STEP()<0) return GA_UNSUPPORTED;
        return GA_MORE;
    /* 0465FE (046718, a program call of the walkers): the thrown bomb. */
    case 0x465fe: {
        ga_palette(a,0x35d);ga_palette(a,0x364);ga_palette(a,0x365);SET(0x15,5);SW(0x66,0xe0ef);SW(0x1e,0xe0ef);START(0x243a30);
        uint32_t vx,vy;k3b332(BIT(0x15,0)?0x80:-0x80,0,0xffffb000u,0x4c,&vx,&vy);
        SL(0x30,vx);SL(0x34,vy);SL(0,0x46678);
    }   /* fall through */
    case 0x46678: {
        SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);                                 /* 0143DC */
        if (STEP()<0) return GA_UNSUPPORTED;
        if (BIT(0x15,7)) SL(0,0x466dc);
        if (ga_attack(a,e,0x4674c)) SL(0,0x466f6);
        unsigned d0=e->video&3u;if (d0>=3u) d0=1;
        SB(0x14,B(0x19+d0));
        if (ga_cull_rect(a,e,0x4676c)) return GA_DELETE;                          /* 019504, 004188 */
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x466dc: SW(0x66,0x1001);SW(0x1e,0x1001);sound(e,0x1027);goto k4670c;
    case 0x466f6: SW(0x66,0x8820);SW(0x1e,0x8820);sound(e,0);
    k4670c:
        SB(0x1d,0);SL(0,0x1c8fc);return neo_region_tick(a,e);                  /* 009F88, JMP 01C8FC */
    }
    STOP(L(0));
}
/* ---- program calls ---------------------------------------------------------------------- */
static int fb_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    if (kind!=NEO_ANIM_CALL) return 0;
    switch (routine) {
    case 0x44dae: {   /* the hub's byte (low byte of the parameter) = its high byte */
        neo_gactor_t *h=node(e,L(0x72));
        if (!h) return 0;
        ga_sb(h,parameter&0xffu,(uint8_t)(parameter>>8));
        return 1;
    }
    case 0x44d0c: sound(e,0x10ce);part(a,e,0x436b6);return 1;
    case 0x44d42: {
        if (!B(0x80)) return 1;
        neo_gactor_t *c=part(a,e,0x43860);
        neo_contact_parent_t p,r;
        if (!ga_parent(e,L(0x38),&p) || !ga_parent(e,ROOT,&r)) return 0;
        uint32_t y=L(0x2c);
        if (L(0x38)!=ROOT) y=y+(uint32_t)p.previous_y-(uint32_t)r.previous_y;   /* 01AC4C */
        int16_t d1=(int16_t)((y>>16)-0x30u);
        int32_t d0=(int32_t)S16(rw(e,0x44fb8u+2u*W(0x86)))*d1;
        if (c) ga_place_at(c,a,e,(int16_t)(d0>>8),0);
        return 1;
    }
    case 0x44bc4: {   /* the shot 0441CA (after the script actor) with the speed and direction of the attack +80 */
        neo_gactor_t *c=after_script(a,e,0x441ca);
        if (!c) return 1;
        ga_sl(c,0x6e,L(0x6e));ga_sl(c,0x72,L(0x72));ga_sb(c,0x80,B(0x80));ga_sl(c,0x86,L(0x86));
        ga_place_from(c,a,e,0,0);                                               /* 01B220 */
        uint32_t n;uint8_t d0;
        if (B(0x80)==5u) {   /* 044C14 */
            ga_sw(c,0x94,rank(a,e,rl(e,0x45126u+(uint32_t)k4463e(a,e))));
            d0=in_play(e,B(0x9b),&n) && S16((uint16_t)(pw(e,n,0x3c)-W(0x3c)))>0?0:0x80;
        } else {   /* 044C64 */
            uint32_t t=B(0x80)==4u?0x450d6u:B(0x80)==6u?0x45166u:B(0x80)==8u?0x4521eu:0;
            if (!t) {ga_fail(e,0x44cc6);return 0;}                          /* D0 left by 01B220: not reached */
            ga_sw(c,0x94,rank(a,e,rl(e,t+(uint32_t)k4463e(a,e))));
            if (in_play(e,B(0x9b),&n)) d0=k43da4(k43d86(neo_angle((int16_t)(pw(e,n,0x3c)-W(0x3c)),(int16_t)(pw(e,n,0x40)+0x10u-W(0x40)))));
            else d0=0x80;
        }
        ga_sb(c,0x9a,d0);
        return 1;
    }
    case 0x440fe: {   /* an item by the drop table (2C5E3E / 2C78D8 / 2C5E66 / 2C78F8) */
        neo_gactor_t *h=node(e,L(0x72));
        if (!h) return 0;
        bool v=k44b84(e);
        uint32_t t=(ga_b(h,0x86)&1u)?(v?0x2c78f8u:0x2c5e66u):(v?0x2c78d8u:0x2c5e3eu);
        int16_t d0=(int16_t)ga_rand_range(e,0x64);
        for (unsigned i=0;i<64;i++,t+=4) {
            d0=(int16_t)(d0-S16(rw(e,t+2)));
            if (d0<0) {neo_item_drop(a,e,0,0,rw(e,t),0x96,0,0);return e->unsupported?0:1;}
        }
        ga_fail(e,0x440fe);return 0;
    }
    case 0x4649e: SB(0x91,(uint8_t)((ga_rand(e)&3u)+1u));return 1;
    case 0x46718: {neo_gactor_t *c=child(a,e,0x465fe,0x1c);if (c) ga_place_from(c,a,e,0,0x20);return 1;}   /* the walker's bomb */
    case 0x40ed0: {   /* the blood 040F88 at (8, 0) mirrored */
        SB(0x15,B(0x15)^1u);
        neo_gactor_t *c=child(a,e,0x40f88,0x2a);
        if (c) ga_place_from(c,a,e,8,0);
        SB(0x15,B(0x15)^1u);
        return 1;
    }
    }
    return 0;
}
/* ---- dispatch ------------------------------------------------------------------------- */
static bool in(uint32_t r,uint32_t lo,uint32_t hi) {return r>=lo && r<=hi;}
static bool tr_routine(uint32_t r) {return in(r,0x438a4,0x438c8) || in(r,0x43ae4,0x43b1c) || in(r,0x43e36,0x43f64) || r==0x440d6u || in(r,0x4416e,0x441a8) || in(r,0x441ca,0x4439e);}
static bool wk_routine(uint32_t r) {return in(r,0x45ef6,0x45f02) || in(r,0x46112,0x46370) || in(r,0x46510,0x4654c) || in(r,0x465fe,0x466f6);}
static bool aux_routine(uint32_t r) {return r==0x443beu || r==0x4445au || r==0x44a08u || r==0x44a56u || r==0x44b32u || r==0x44b56u || r==0x424f8u || r==0x42514u || r==0x42aeau || r==0x42b34u;}
bool neo_m6boss_routine(uint32_t r) {
    return r==0x4153eu || r==0x41578u || in(r,0x4157a,0x418ce) || aux_routine(r) || tr_routine(r) || wk_routine(r) || in(r,0x42bdc,0x42d4e) || in(r,0x42e22,0x42f5c) || r==0x43054u
        || in(r,0x4307e,0x430ae) || in(r,0x431e0,0x4357e) || in(r,0x4359e,0x435ce) || in(r,0x444e8,0x445f6)
        || in(r,0x45428,0x45518) || in(r,0x45608,0x45cf6) || r==0xbea82u || in(r,0x436b6,0x43712) || in(r,0x43860,0x43896);
}
ga_result_t neo_m6boss_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=L(0);
    if (r==0x4153eu || r==0x41578u) return hub_tick(a,e);
    if (aux_routine(r)) return aux_tick(a,e);
    if (tr_routine(r)) return tr_tick(a,e);
    if (wk_routine(r)) return wk_tick(a,e);
    if (in(r,0x4157a,0x418ce)) return body_tick(a,e);
    if (in(r,0x42bdc,0x42d4e)) return p0_tick(a,e);
    if (in(r,0x42e22,0x42f5c) || r==0x43054u) return p2_tick(a,e);
    if (in(r,0x4307e,0x430ae)) return p3_tick(a,e);
    if (in(r,0x431e0,0x435ce)) return p1_tick(a,e);
    if (in(r,0x444e8,0x445f6)) return p4_tick(a,e);
    if (in(r,0x45428,0x45518)) return op_tick(a,e);
    if (in(r,0x45608,0x45cf6)) return op_actor(a,e);
    if (r==0xbea82u) return fade_tick(a,e);
    if (in(r,0x436b6,0x43712) || in(r,0x43860,0x43896)) return fl_tick(a,e);
    STOP(r);
}
#else
bool neo_m6boss_routine(uint32_t r) {(void)r;return false;}
ga_result_t neo_m6boss_tick(neo_gactor_t *a,neo_genv_t *e) {(void)a;ga_fail(e,0x4153e);return GA_UNSUPPORTED;}
#endif
