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
#include "respawn_fixture.h"

/* B33: the Mission 2 boss tower (data/respawn_fixture.json): 019658's entry
 * of the stage-3 table for the progress, 0196E8's record after its step,
 * and 014742 driven by the captured 013DB2 answers (carry, D6): every
 * query's inputs (+28, projected x/y, D3) and the placed x must be the
 * arcade's. The floor search itself is the native kernel's (its tests). */
typedef struct { const respawn_query_t *q;unsigned n,at,bad; } replay_t;
static bool replay_floor(void *ctx,int32_t px,int32_t py,int32_t limit,int32_t *d6) {
    replay_t *r=ctx;
    if (r->at>=r->n) {r->bad++;return false;}
    const respawn_query_t *q=&r->q[r->at++];
    if ((uint32_t)px!=q->px || (uint32_t)py!=q->py || (uint32_t)limit!=q->limit) {
        if (r->bad<10) fprintf(stderr,"query %u: px %08X py %08X limit %08X, capture %08X %08X %08X\n",r->at-1,(unsigned)px,(unsigned)py,(unsigned)limit,q->px,q->py,q->limit);
        r->bad++;
    }
    *d6=(int32_t)q->d6;return q->carry!=0;
}
static unsigned respawn_cases_test(unsigned *queries) {
    unsigned bad=0,n=marco_respawn_entries(respawn_table,sizeof respawn_table);
    for (unsigned i=0;i<RESPAWNS;i++) {
        const respawn_case_t *c=&respawn_cases[i];
        unsigned e=marco_respawn_select(respawn_table,n,0,(uint16_t)c->progress);
        if (RESPAWN_TABLE_ROM+12u*e+2u!=c->record) {fprintf(stderr,"respawn %u: entry %u, capture record %06X\n",c->frame,e,c->record);bad++;continue;}
        marco_respawn_t r=marco_respawn_entry(respawn_table,e,(int32_t)c->root_x,(int32_t)c->root_y);
        if ((uint32_t)r.x!=c->x || (uint32_t)r.y!=c->y) {fprintf(stderr,"respawn %u: record %08X %08X, capture %08X %08X\n",c->frame,(unsigned)r.x,(unsigned)r.y,c->x,c->y);bad++;continue;}
        uint8_t type=r.type;int32_t x=r.x;
        replay_t rp={&respawn_queries[c->first],c->count,0,0};
        if (type==4) {
            int16_t d0=(int16_t)((int16_t)((uint32_t)(r.y+(int32_t)c->root_y)>>16)-0x100);
            if (d0<0) type=5;
            else type=marco_respawn_search(&x,(int32_t)c->root_x,r.y+(int32_t)c->root_y,(int32_t)((uint32_t)(uint16_t)(0u-(uint16_t)d0)<<16),replay_floor,&rp)?4:5;
        }
        if (rp.bad || rp.at!=rp.n || type!=c->type || (uint32_t)x!=c->placed_x) {
            fprintf(stderr,"respawn %u: %u/%u queries (%u differ), type %u x %08X, capture type %u x %08X\n",c->frame,rp.at,rp.n,rp.bad,type,(unsigned)x,c->type,c->placed_x);bad++;
        }
        *queries+=rp.at;
    }
    return bad;
}

int main(void) {
    unsigned bad=0,skipped=0,velocities=0,speeds=0,ends=0,respawns=0,entrances=0,poses=0;
    for (unsigned i=0;i<DEATHS;i++) {
        const death_case_t *c=&death_cases[i];const death_update_t *u=&death_updates[c->first];
        marco_death_t d;int32_t vx=(int32_t)u[0].v30,vy=(int32_t)u[0].v34;int facing=0;
        bool ground=(u[0].f15&0x40u)!=0;
        marco_death_start(&d,(uint8_t)c->kind,ground,(c->f5b&0x80u)!=0,&vx,&vy,&facing);
        if (c->kind!=2) ground=false;                          /* 0C2E7E */
        unsigned k=1,end=0,gone=0;uint32_t got[256]={0};marco_life_phase_t phase[256]={0};
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
            /* The pose of the update (0 none: lying, every other update). */
            {neo_anim_draw_t dd;got[k]=d.phase!=MARCO_GONE && marco_death_draw(&d,0,0,1,&dd)?dd.descriptor:0;phase[k]=before;}
            if (before==MARCO_DYING && d.phase==MARCO_LYING) end=k;
            if (d.phase==MARCO_GONE) gone=k;
            ground=now;
        }
        /* The poses (MARCO_DRAW of 004406, logged by marco-d2): every
         * update's pose while dying, in order, and the number of draws while
         * lying (every other update: the body blinks). */
        if (c->pose_count) {
            unsigned n=0,lying=0;
            for (unsigned j=1;j<k;j++) {
                if (phase[j]==MARCO_DYING) {
                    if (n<c->pose_count && got[j]==death_poses[c->pose_first+n]) poses++;
                    else {if (bad<40) fprintf(stderr,"death %u (kind %X) dying pose %u: %06X, capture %06X\n",c->frame,c->kind,n,(unsigned)got[j],n<c->pose_count?death_poses[c->pose_first+n]:0u);bad++;}
                    n++;
                } else if (phase[j]==MARCO_LYING) lying+=got[j]!=0;
            }
            if (n!=c->pose_count || lying!=c->lying_draws) {fprintf(stderr,"death %u: %u dying poses, %u lying draws; capture %u, %u\n",c->frame,n,lying,c->pose_count,c->lying_draws);bad++;}
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
    unsigned queries=0,rbad=respawn_cases_test(&queries);bad+=rbad;
    printf("respawn (Mission 2 boss tower): %u respawns, %u floor searches of 014742 replayed; %u mismatches\n",RESPAWNS,queries,rbad);
    printf("death: %u deaths, %u airborne velocities, %u speeds, %u end/gone updates, %u respawn points, %u entrances, %u update poses equal (%u updates with swept motion not compared); %u mismatches\n",DEATHS,velocities,speeds,ends,respawns,entrances,poses,skipped,bad);
    if (bad) return 1;
    printf("PASS: player death 0C2956..0C2F96 and respawn 019658/0196E8/014742/0C1438\n");
    return 0;
}
