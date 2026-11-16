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
#include "death_fixture_driver.h"
/* Form 2's death body (0C0518, role B2_HIT_C2A06) from the compiled catalog.
 * The catalog is derived from the ROM (never in git): without it (CI) the
 * form-2 deaths are not run and the count says so; every other case is. */
#if __has_include("marco_campaign.h")
#define HAVE_MARCO_CAMPAIGN 1
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverflow"
#pragma GCC diagnostic ignored "-Wunused-variable"
#include "marco_campaign.h"
#pragma GCC diagnostic pop
#else
#define HAVE_MARCO_CAMPAIGN 0
#endif

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
#if HAVE_MARCO_CAMPAIGN
    marco_death_use_fat_program(&marco_script,MARCO_CAMPAIGN_B2_HIT_C2A06);
#endif
    death_stats_t st={0};unsigned marco=0,no_catalog=0;
    for (unsigned i=0;i<DEATHS;i++) if (!death_cases[i].character) {
        if (!HAVE_MARCO_CAMPAIGN && death_cases[i].form==2) {no_catalog++;continue;}
        death_case_run(&death_cases[i],&st);marco++;
    }
    if (no_catalog) printf("death: %u form-2 deaths not run (build/marco_campaign.h, derived from the ROM, absent)\n",no_catalog);
    unsigned bad=st.bad,skipped=st.skipped,velocities=st.velocities,speeds=st.speeds,ends=st.ends,respawns=st.respawns,entrances=st.entrances,poses=st.poses,palettes=st.palettes,forms=st.forms;
    unsigned queries=0,rbad=respawn_cases_test(&queries);bad+=rbad;
    printf("respawn (Mission 2 boss tower): %u respawns, %u floor searches of 014742 replayed; %u mismatches\n",RESPAWNS,queries,rbad);
    printf("death: %u deaths (%u by form), %u airborne velocities, %u speeds, %u end/gone updates, %u respawn points, %u entrances, %u update poses equal, %u palettes (%u updates with swept motion not compared); %u mismatches\n",marco,forms,velocities,speeds,ends,respawns,entrances,poses,palettes,skipped,bad);
    if (bad) return 1;
    printf("PASS: player death 0C2956..0C2F96 (forms 2/3: 0C2A36, 0C2C40) and respawn 019658/0196E8/014742/0C1438\n");
    return 0;
}
