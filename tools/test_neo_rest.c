/* Closed-loop test of the robed swordsman 0A8C46 (src/neo_rest.c) against a
 * MAME family trace of the boss arena (data/robe_fixture.json.gz): the native
 * generic actor starts from the constructor's recorded node and is never
 * reloaded. The inputs are the RAM words the original update read (scene
 * root 10E97C/10E980, camera progress 10E99E, player 1 +3C, group 6 count,
 * epoch 10C33A); after every update the routine and the node bytes must
 * match, except the allocator's palette slot numbers (+19..+1C, +14, +16:
 * native handles), the animation pointer +20 (native program/cursor) and the
 * cached position +44/+48 (previous scene root, not in the trace); the
 * write to RAM 10C606 must happen in the same update with the same value. */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#define MSX_SOLDIER_PROGRAMS 1
#include "../src/neo_region.h"
#include "soldier_ai_tables.h"
#include "robe_fixture.h"
#define N (sizeof(robe_updates)/sizeof(robe_updates[0]))
static neo_contact_parent_t root;
static bool parent(void *ctx,uint32_t tag,neo_contact_parent_t *out) {(void)ctx;if (tag!=0x10e968u) return false;*out=root;return true;}
static const neo_anim_program_t *program(uint32_t tag) {
    for (unsigned i=0;i<sizeof(soldier_programs)/sizeof(soldier_programs[0]);i++) if (soldier_programs[i].tag==tag) return &soldier_programs[i].program;
    return NULL;
}
static unsigned draws,children;
static neo_gactor_t child,*parent_actor;
static neo_gactor_t *spawn(void *ctx,neo_gactor_t *creator,uint32_t routine,bool after_script) {
    (void)ctx;assert(!after_script && creator==parent_actor && routine==0x19e72 && !children);
    children++;memset(&child,0,sizeof(child));child.active=true;child.index=1;ga_sl(&child,0,routine);ga_sl(&child,0x0c,0x40000000u);
    return &child;
}
static neo_gactor_t *creator_of(void *ctx,neo_gactor_t *a) {(void)ctx;(void)a;return parent_actor;}
static bool creator_alive(void *ctx,neo_gactor_t *a) {(void)ctx;(void)a;return true;}
static void emit(void *ctx,const neo_anim_draw_t *d) {(void)ctx;(void)d;draws++;}
static bool skip(unsigned o) {return o==0x14 || o==0x16 || (o>=0x19 && o<=0x1c) || (o>=0x20 && o<=0x23) || (o>=0x44 && o<=0x4b);}
int main(void) {
    neo_gactor_t a;memset(&a,0,sizeof(a));a.active=true;
    memcpy(a.m,robe_updates[0].pre,0xb0);
    uint8_t flag606=0xff,groups[9]={0};
    neo_rng_t rng;memset(&rng,0,sizeof(rng));
    neo_genv_t e;memset(&e,0,sizeof(e));
    e.rng=&rng;e.parent=parent;e.program=program;e.emit=emit;e.groups=groups;e.flag606=&flag606;
    e.spawn=spawn;e.creator=creator_of;e.creator_alive=creator_alive;parent_actor=&a;
    e.bounds=(neo_cull_bounds_t){false,0,0,0,0,0,256,320,512};
    e.players.mode88=2;e.players.rank=3;e.players.active_mask=1;
    unsigned toggles=0,compared=0;int32_t progress=0,p1x=0;uint8_t epoch=1;
    for (unsigned k=0;k<N;k++) {
        const robe_update_t *u=&robe_updates[k];
        if (ga_l(&a,0)!=u->routine) {fprintf(stderr,"frame %u routine %05x expected %05x\n",u->frame,ga_l(&a,0),u->routine);return 1;}
        root=(neo_contact_parent_t){0x10e968u,u->root_x,u->root_y,u->root_x,u->root_y,0};
        if (u->progress>=0) progress=u->progress;
        if (u->p1x>=0) p1x=u->p1x;
        if (u->epoch>=0) epoch=(uint8_t)u->epoch;
        if (u->groups6>=0) groups[6]=(uint8_t)u->groups6;
        e.camera=(uint16_t)progress;e.players.p1x=(uint16_t)p1x;e.epoch=epoch;
        uint8_t before=flag606;
        ga_result_t r=neo_rest_tick(&a,&e);
        if (r!=GA_MORE) {fprintf(stderr,"frame %u result %d (unsupported %05x)\n",u->frame,r,(unsigned)e.unsupported_at);return 1;}
        if ((flag606!=before)!=(u->write606!=0) || (u->write606 && flag606!=u->value606)) {fprintf(stderr,"frame %u 10C606 %02x expected write=%u %02x\n",u->frame,flag606,u->write606,u->value606);return 1;}
        toggles+=flag606!=before;
        for (unsigned o=0x10;o<0xb0;o++) {
            if (skip(o)) continue;
            if (a.m[o]!=u->post[o]) {fprintf(stderr,"frame %u routine %05x +%02X=%02x expected %02x\n",u->frame,u->routine,o,a.m[o],u->post[o]);return 1;}
            compared++;
        }
        /* The sub-script runs right after its creator in the same pass. */
        if (children) {ga_result_t s=neo_rest_tick(&child,&e);assert(s==GA_MORE);}
    }
    assert(toggles==1 && children==1 && (ga_b(&child,0x5f)&0x80u));
    printf("PASS: robed swordsman 0A8C46: %u closed-loop updates (frames %u..%u), %u bytes, %u draws, RAM 10C606 toggled once (boss start), sub-script 019E72 (op 3) ended\n",
        (unsigned)N,(unsigned)robe_updates[0].frame,(unsigned)robe_updates[N-1].frame,compared,draws);
    return 0;
}
