/* Player death and respawn (src/marco_death.c) against the boss-arena deaths
 * of a capture_weapons.lua run with MSX_WEAPONS_MORTAL_CAMERA
 * (data/death_fixture.json, tools/extract_death_fixture.py). Per death,
 * from the update of 0C2956 (one per 10000E value): the velocities +30/+34
 * of every airborne update, the speed +26, the update of 0C2F06 (program
 * end), of the actor's end (003F1E) and, after the panel's new life, the
 * respawn x of 0196E8 (screen 56 from the 2D5C52 record at progress
 * 0x0D30) and the entrance length (0C229C). The ground flag of each update
 * (+15 bit 6) comes from the capture: the motion is the caller's. */
#include <stdio.h>
#include "../src/marco_death.h"
#include "death_fixture.h"

int main(void) {
    unsigned bad=0,skipped=0,velocities=0,speeds=0,ends=0,respawns=0,entrances=0;
    for (unsigned i=0;i<DEATHS;i++) {
        const death_case_t *c=&death_cases[i];const death_update_t *u=&death_updates[c->first];
        marco_death_t d;int32_t vx=(int32_t)u[0].v30,vy=(int32_t)u[0].v34;int facing=0;
        bool ground=(u[0].f15&0x40u)!=0;
        marco_death_start(&d,(uint8_t)c->kind,ground,(c->f5b&0x80u)!=0,&vx,&vy,&facing);
        if (c->kind!=2) ground=false;                          /* 0C2E7E */
        unsigned k=1,end=0,gone=0;
        for (;k<c->count && d.phase!=MARCO_GONE;k++) {
            marco_death_velocity(&d,ground,&vx,&vy);
            bool now=(u[k].f15&0x40u)!=0;
            /* Updates whose motion the sweep changed (the position moved by
             * other than +34, another parent, or the ceiling bit +15 bit 7) and kind 7's first update
             * (+30/+34 = -/+3.80 written outside 0C2C7A, not located:
             * explicit stop) are not compared. */
            bool free=u[k].p38==u[k-1].p38 && !(u[k].f15&0x80u) && u[k].y2c-u[k-1].y2c==u[k].v34 && !(c->kind==7 && k==1);
            if (!free) skipped++;
            else if (!now && d.phase==MARCO_DYING) {
                if ((uint32_t)vx!=u[k].v30 || (uint32_t)vy!=u[k].v34) {if (bad<20) fprintf(stderr,"death %u update %u: +30/+34 %08X %08X, capture %08X %08X\n",c->frame,k,(unsigned)vx,(unsigned)vy,u[k].v30,u[k].v34);bad++;}
                else velocities++;
            }
            vx=(int32_t)u[k].v30;vy=(int32_t)u[k].v34;        /* the motion (and landing) of the capture */
            marco_life_phase_t before=d.phase;
            marco_death_after(&d,now,&vx,(int16_t)(u[k].x3c>>16),(int16_t)(u[k].y40>>16));
            if (before==MARCO_DYING) {
                if (d.anim.speed!=u[k].f26) {if (bad<20) fprintf(stderr,"death %u update %u: +26 %04X, capture %04X\n",c->frame,k,d.anim.speed,u[k].f26);bad++;}
                else speeds++;
            }
            if (before==MARCO_DYING && d.phase==MARCO_LYING) end=k;
            if (d.phase==MARCO_GONE) gone=k;
            ground=now;
        }
        /* The capture: 0C2F06 written at the end update, 003F1E at the actor's end. */
        unsigned want_end=0,want_gone=0,want_entry=0,want_normal=0;
        for (unsigned j=1;j<c->count;j++) {
            if (!want_end && u[j].routine==0xc2f06) want_end=j;
            if (!want_gone && u[j].routine==0x3f1e) want_gone=j;
            if (!want_entry && u[j].routine==0xc149e) want_entry=j;
            if (u[j].routine==0xc229c) want_normal=j;
        }
        if (end!=want_end || gone!=want_gone) {fprintf(stderr,"death %u (kind %X): end %u gone %u, capture %u %u\n",c->frame,c->kind,end,gone,want_end,want_gone);bad++;}
        else ends++;
        /* The new life: one update after the end (panel 0C95AE). */
        if (want_entry!=gone+1) {fprintf(stderr,"death %u: new life at %u, capture %u\n",c->frame,gone+1,want_entry);bad++;}
        const death_update_t *e=&u[want_entry];
        int32_t root_x=(int32_t)(e->x3c-e->x28),root_y=(int32_t)(e->y40-e->y2c);
        marco_respawn_t r=marco_respawn_point((uint16_t)c->progress,root_x,root_y);
        if ((uint32_t)(r.x+root_x)!=e->x3c || r.type) {fprintf(stderr,"death %u: respawn x %08X, capture %08X\n",c->frame,(unsigned)(r.x+root_x),e->x3c);bad++;}
        else respawns++;
        marco_entrance_start(&d);unsigned n=0;
        while (marco_entrance_step(&d)) n++;
        if (want_entry+n!=want_normal) {fprintf(stderr,"death %u: entrance %u updates, capture %u\n",c->frame,n+1,want_normal-want_entry);bad++;}
        else entrances++;
    }
    printf("death: %u deaths, %u airborne velocities, %u speeds, %u end/gone updates, %u respawn points, %u entrances equal (%u updates with swept motion not compared); %u mismatches\n",DEATHS,velocities,speeds,ends,respawns,entrances,skipped,bad);
    if (bad) return 1;
    printf("PASS: player death 0C2956..0C2F96 and respawn 0196E8/0C1438\n");
    return 0;
}
