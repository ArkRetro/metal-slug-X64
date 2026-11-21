/* Fio's actor 0C4C64 of her deaths (marco_death_cap_*, src/neo_effects.c)
 * against MAME (data/fio_cap_fixture.json, capture c3d2: tools/
 * extract_fio_cap_fixture.py): the constructor's velocities from the
 * player's, then update by update +30, +34 (not in the updates whose 0143E4
 * step landed or changed the parent), +26, the program 253322 (the
 * converted tables) with its end count +80, the blink +5F bit 3 and the
 * update the actor is gone. */
#include <stdio.h>
#include <string.h>
#include "marco_death.h"
#define MSX_SOLDIER_PROGRAMS
#include "soldier_ai_tables.h"
#include "fio_cap_fixture.h"
static const neo_anim_program_t *find(uint32_t tag) {
    for (unsigned i=0;i<sizeof soldier_programs/sizeof soldier_programs[0];i++) if (soldier_programs[i].tag==tag) return &soldier_programs[i].program;
    return NULL;
}
static void emit(void *c,const neo_anim_draw_t *d) {(void)c;(void)d;}
int main(void) {
    const neo_anim_program_t *prog=find(0x253322);
    if (!prog) {puts("FAIL: program 253322 not in the tables");return 1;}
    unsigned bad=0,vel=0,speeds=0,counts=0,skipped=0;
    int32_t vx,vy;marco_death_cap_start((int32_t)cap_creator_v30,(int32_t)cap_creator_v34,&vx,&vy);
    neo_anim_state_t s;neo_anim_start(&s,prog,0);
    uint16_t speed=0,count=0;uint8_t f5f=0;bool ground=false;unsigned gone=0;
    for (unsigned k=0;k<CAP_UPDATES+1 && !gone;k++) {
        marco_death_cap_velocity(&vx,&vy,&speed,ground);
        s.speed=speed;neo_anim_render_t r;memset(&r,0,sizeof r);
        neo_anim_result_t res=neo_anim_step(&s,&r,emit,NULL,256);
        if (res!=NEO_ANIM_MORE && res!=NEO_ANIM_COMPLETE) {printf("FAIL: program result %d at update %u\n",res,k);return 1;}
        if (marco_death_cap_after(res==NEO_ANIM_COMPLETE,&count,&f5f)) {gone=k;break;}
        if (k>=CAP_UPDATES) break;
        const cap_update_t *u=&cap_updates[k];
        bool landed=(u->f15&0x40u)!=(k?cap_updates[k-1].f15&0x40u:0u) || (k && u->p38!=cap_updates[k-1].p38);
        if ((uint32_t)vx!=u->v30) {if (bad<10) printf("update %u: +30 %08X, capture %08X\n",k,(unsigned)vx,u->v30);bad++;} else vel++;
        if (landed) skipped++;
        else if ((uint32_t)vy!=u->v34) {if (bad<10) printf("update %u: +34 %08X, capture %08X\n",k,(unsigned)vy,u->v34);bad++;} else vel++;
        if (speed!=u->f26) {if (bad<10) printf("update %u: +26 %04X, capture %04X\n",k,speed,u->f26);bad++;} else speeds++;
        if (count!=u->f80 || (f5f&8u)!=(u->f5f&8u)) {if (bad<10) printf("update %u: +80 %u +5F %02X, capture %u %02X\n",k,count,f5f,u->f80,u->f5f);bad++;} else counts++;
        vx=(int32_t)u->v30;vy=(int32_t)u->v34;ground=(u->f15&0x40u)!=0;   /* the 0143E4 step of the capture */
    }
    if (gone!=CAP_UPDATES) {printf("gone at update %u, capture %u\n",gone,(unsigned)CAP_UPDATES);bad++;}
    printf("Fio's actor 0C4C64: %u updates, %u velocities, %u speeds, %u end counts, gone at %u (%u landing updates not compared); %u mismatches\n",
        (unsigned)CAP_UPDATES,vel,speeds,counts,gone,skipped,bad);
    if (bad) {puts("FAIL: Fio's actor 0C4C64");return 1;}
    puts("PASS: Fio's actor 0C4C64/0C4CBA (0C4C28) against MAME c3d2");
    return 0;
}
