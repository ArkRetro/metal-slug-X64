#include "neo_region.h"
/* Mission 2 helpers, manual translation (ROM labels as provenance):
 * - the GO arrow 05287C (made by the mission actor at 01860A for every
 *   stage): anchored at (108, A0) of the scene root 10E968, palette 0x125,
 *   idle program 24953C. Once the camera progress (10E99E) has been still
 *   for 0xD2 updates with no camera stop reached (0C7B26: 10E9BC), no live
 *   group (019DE4: 10C326..10C32D), no halt (018F24 10C301, 019424 10C300)
 *   and a player in play (01AEB0), it plays the arrow of table 05285C by
 *   RAM 10C44A (set by 0529C0) twice, then waits 60 updates.
 * - the stage-1 exit marker 0B6F6A: program 2A8B70, palette 0x461, deleted
 *   (004188) once the camera progress reaches +6E = 0x200.
 * - the stage-1 exit 0B6ED2 (+6E = rectangle 0B6ECA, +72 = 0x2A0): with a
 *   player in play (0193FE) it ends the mission's control (018F92: kill all
 *   004BFE, flash 004C06, the level script actor deleted, 0C09D8, halt
 *   10C300, handlers 0C0538), locks the players (0C8CD6), 10C00B = FF and
 *   walks both players to screen X +72 (0CA340: handler 0CA3EC); then
 *   0B6F24 every update: once every player in play is inside the rectangle
 *   (01B2CE / 01AF3C) the stage ends (018F40). 0C8D0C (no player alive:
 *   node 101204) is an explicit stop. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
static bool call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL || routine!=0x12022u) return false;      /* 012022 */
    bool front=e->front_layer && e->front_layer(e->ctx);
    SB(0x15,front?(B(0x15)|4u):(B(0x15)&0xfbu));
    return true;
}
static const uint32_t arrows[8]={0x249546,0x2495fc,0x2496b2,0x249768,0x24981e,0x2498d4,0x24998a,0x249546}; /* 05285C */
static ga_result_t go_arrow(neo_gactor_t *a,neo_genv_t *e) {
    if (ga_l(a,0)==0x5287cu) {
        if (e->arrow) *e->arrow=0;                                     /* 10C44A */
        ga_place(a,e,0x108,0xa0,0x10e968u);ga_project(a,e);             /* 01AB98, 01ABC8 */
        ga_palette(a,0x125);SW(0x66,0xffff);SW(0x1e,0xffff);
        if (!ga_anim_start(a,e,0x24953c,0x100)) return GA_UNSUPPORTED;
        SW(0x70,0);SW(0x72,0);SB(0x74,0);SB(0x75,0);SL(0,0x528de);
    }
    SW(0x70,W(0x70)+1u);                                               /* 0528DE */
    if (e->camera!=W(0x72)) SW(0x70,0);
    SW(0x72,e->camera);
    bool idle=false;
    uint32_t progress=(uint32_t)e->camera<<16|e->camera_low;
    if (e->camera_stop!=0xffffffffu && e->camera_stop<=progress) idle=true;   /* 0C7B26 */
    if (!idle && e->groups) {uint16_t s=0;for (unsigned i=0;i<8;i++) s=(uint16_t)((s&0xff00u)|((s+e->groups[i])&0xffu));if (s&0xffu) idle=true;} /* 019DE4 */
    if (!idle && e->vehicle && (e->vehicle->halted || e->vehicle->system)) idle=true;   /* 018F24, 019424 */
    if (!idle && !(e->players.active_mask&3u)) idle=true;              /* 01AEB0 x2 */
    if (!idle && W(0x70)<0xd2u) idle=true;                             /* 05293E */
    if (idle) {                                                        /* 05299C */
        int r=ga_anim_step(a,e,call);
        if (r<0) return GA_UNSUPPORTED;
        if (r>0 && !ga_anim_start(a,e,0x24953c,0x100)) return GA_UNSUPPORTED;
        SB(0x74,0);SB(0x75,0);
        return GA_MORE;
    }
    if (!B(0x74)) {
        if (!ga_anim_start(a,e,arrows[(e->arrow?*e->arrow:0)&7u],0x100)) return GA_UNSUPPORTED;
        SB(0x74,0xff);
    }
    int r=ga_anim_step(a,e,call);
    if (r<0) return GA_UNSUPPORTED;
    if (r>0) {
        SB(0x74,0);SB(0x75,B(0x75)+1u);
        if (B(0x75)>=2u) {SW(0x70,0x96);SB(0x75,0);}
    }
    return GA_MORE;
}
static ga_result_t exit_marker(neo_gactor_t *a,neo_genv_t *e) {
    if (ga_l(a,0)==0xb6f6au) {
        if (!ga_anim_start(a,e,0x2a8b70,0x100)) return GA_UNSUPPORTED;
        ga_palette(a,0x461);SW(0x66,0xfff1);SW(0x1e,0xfff1);
        SB(0x15,B(0x15)&0xfeu);
        call(a,e,NEO_ANIM_CALL,0x12022u,0);                             /* 012022 */
        SW(0x6e,0x200);SL(0,0xb6fac);
    }
    ga_project(a,e);                                                   /* 0B6FAC */
    if (ga_anim_step(a,e,call)<0) return GA_UNSUPPORTED;
    if (e->camera>=W(0x6e)) return GA_DELETE;                          /* 004188 */
    return GA_MORE;
}
/* 0193C0: carry when a player node exists with +5F bit 7 clear. */
static bool player_up(const neo_genv_t *e) {
    if (!e->vehicle) return (e->players.active_mask&3u)!=0;
    for (unsigned p=0;p<2;p++) if (e->vehicle->exists[p] && e->vehicle->player[p] && !(e->vehicle->player[p][0x5f]&0x80u)) return true;
    return false;
}
/* 0C09D8: each existing player's +84 raised to FFFF, +5F bit 6, +9A bit 2 cleared. */
static void guard(neo_genv_t *e) {
    if (e->flags) e->flags->player_guard=1;
    if (!e->vehicle) return;
    for (unsigned p=0;p<2;p++) {
        uint8_t *n=e->vehicle->player[p];
        if (!e->vehicle->exists[p] || !n) continue;
        n[0x84]=0xff;n[0x85]=0xff;n[0x5f]|=0x40u;n[0x9a]&=(uint8_t)~4u;
    }
}
/* 0AAA88: 0 without a vehicle slot (the vehicle path is not observed). */
static bool no_vehicle(neo_genv_t *e) {
    if (!e->vehicle || !e->vehicle->slots) return true;
    for (unsigned p=0;p<2;p++) if (e->vehicle->exists[p] && neo_vslot_node(&e->vehicle->slots->slot[p])) return false;
    return true;
}
static ga_result_t stage_exit(neo_gactor_t *a,neo_genv_t *e) {
    if (ga_l(a,0)==0xb6ed2u) {
        if (!player_up(e)) {ga_fail(e,0xc8d0c);return GA_UNSUPPORTED;}
        if (!e->flags || !no_vehicle(e)) {ga_fail(e,0xaaa88);return GA_UNSUPPORTED;}
        neo_ga_flags_t *f=e->flags;
        f->kill_request=0xff;f->flash_request=0xff;                    /* 004BFE, 004C06 */
        guard(e);                                                      /* 018F92: 0041D0 (101414), 0C09D8 */
        f->mission_end=0xff;f->end_kind=B(0x5e);f->player_handlers=2;  /* 10C300, 10C304, 0C0538 */
        guard(e);                                                      /* 0C09D8 */
        f->player_lock=0xff;                                           /* 0C8CD6 */
        f->scroll_lock=0xff;                                           /* 10C00B */
        f->walk_to[0]=f->walk_to[1]=W(0x72);f->player_handlers=3;      /* 0CA340 x2 */
        SL(0,0xb6f1e);
    }
    ga_project(a,e);                                                   /* 0B6F24 */
    /* 01B2CE: every player in play (10C33C) inside the rectangle +6E. */
    const uint8_t *r=ga_rom(ga_l(a,0x6e));
    if (!r) {ga_fail(e,ga_l(a,0x6e));return GA_UNSUPPORTED;}
    int16_t rx=(int16_t)(r[0]<<8|r[1]),ry=(int16_t)(r[2]<<8|r[3]),rw=(int16_t)(r[4]<<8|r[5]),rh=(int16_t)(r[6]<<8|r[7]);
    for (unsigned p=0;p<2;p++) {
        if (!((e->players.active_mask>>p)&1u)) continue;
        int16_t dy=(int16_t)((p?e->players.p2y:e->players.p1y)-W(0x40)),dx=(int16_t)((p?e->players.p2x:e->players.p1x)-W(0x3c));
        if (B(0x15)&2u) dy=(int16_t)-dy;
        if (B(0x15)&1u) dx=(int16_t)-dx;
        if (dy<ry || dy>(int16_t)(ry+rh) || dx<rx || dx>(int16_t)(rx+rw)) return GA_MORE;
    }
    if (e->flags) e->flags->stage_done=1;                              /* 018F40 */
    return GA_MORE;
}
bool neo_goarrow_routine(uint32_t r) {return r==0x5287cu || r==0x528deu || r==0xb6f6au || r==0xb6facu || r==0xb6ed2u || r==0xb6f1eu;}
ga_result_t neo_goarrow_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=ga_l(a,0);
    if (r==0xb6f6au || r==0xb6facu) return exit_marker(a,e);
    if (r==0xb6ed2u || r==0xb6f1eu) return stage_exit(a,e);
    return go_arrow(a,e);
}
