/* Stage flow of the mission actor (src/neo_mission.c) against the full-game
 * capture run4: every routine change of 101364 from the end of Mission 1 to
 * Mission 6 and the flow words of every RAM dump between two changes
 * (data/mission_flow_fixture.json). The waits on the transition actors
 * 3AED8/3AEF0 (src/neo_wipe.c, tools/test_neo_wipe.c) last 35 updates: the
 * actor sets +60 in its 34th update after the constructor's and the mission
 * actor sees it in the next one. (MAME's 37 frames-pairs include the stage
 * load's lag.) */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "../src/neo_mission.h"
#include "mission_flow_fixture.h"
static int fails;
#define CHECK(c,...) do{if(!(c)){fails++;printf("FAIL: ");printf(__VA_ARGS__);printf("\n");}}while(0)
int main(void) {
    neo_mission_t m;memset(&m,0,sizeof m);
    neo_mission_start(&m,0);
    CHECK(m.record==0x1714E && m.routine==0x184B4 && m.load && m.stage==0,"Mission 1 start record %X routine %X",m.record,m.routine);
    for (unsigned k=0;k<40 && m.state!=NEO_MISSION_PLAYING;k++) neo_mission_update(&m);
    CHECK(m.state==NEO_MISSION_PLAYING && m.letters,"Mission 1 stage loop");
    unsigned n=sizeof mission_flow_events/sizeof mission_flow_events[0],mission_ends=0,checked=0,rams=0,stages=0,missions=0,wipes=0;
    for (unsigned i=0;i<n;i++) {
        const mission_flow_event_t *e=&mission_flow_events[i];
        if (e->previous==0x186A0) {
            /* gameplay.c routes a 018F40 that ends the mission (Mission 2's
             * boss 026254, Mission 3) to mission_end_step: the peek must
             * agree with the arcade's choice at every stage end. */
            CHECK(neo_mission_ends_after_stage(&m)==(e->routine==0x18870),"frame %u: mission-end peek %d, routine %X",e->frame,neo_mission_ends_after_stage(&m),e->routine);
            if (e->routine==0x18870) mission_ends++;
            m.end=0;neo_mission_stage_done(&m);stages++;                       /* 018F40, 018722 */
            if (e->routine==0x18870) {CHECK(m.state==NEO_MISSION_END && m.routine==0x18846,"frame %u: mission end expected (routine %X)",e->frame,m.routine);m.routine=e->routine;}
            else CHECK(m.routine==e->routine,"frame %u: routine %X expected %X",e->frame,m.routine,e->routine);
        } else if (e->previous==0x18916) {
            neo_mission_next(&m);missions++;
            CHECK(m.routine==e->routine && m.load,"frame %u: next mission routine %X expected %X",e->frame,m.routine,e->routine);
        } else if (e->previous==0x18870 || e->previous==0x188B0 || e->previous==0x188F4) {
            m.routine=e->routine;                                               /* gameplay.c mission_end_step */
        } else {
            unsigned k=0;
            uint32_t from=m.routine;
            while (m.routine!=e->routine && k<40) {neo_mission_update(&m);k++;}
            CHECK(m.routine==e->routine,"frame %u: routine %X not reached (at %X)",e->frame,e->routine,m.routine);
            if (from==0x18364 || from==0x18554) {wipes++;CHECK(k==35 && !m.wiping,"frame %u: wipe wait %u updates",e->frame,k);}
        }
        checked++;
        if (e->has_ram) {
            rams++;
            CHECK(m.mission==e->mission && m.stage==e->stage && m.index==e->index && m.record_index==e->record_index && m.record==e->record && m.end==e->end && !memcmp(m.cleared,e->cleared,6),
                  "frame %u: mission %u/%u stage %u/%u index %u/%u record %u/%u %X/%X end %u/%u",e->frame,m.mission,e->mission,m.stage,e->stage,m.index,e->index,m.record_index,e->record_index,m.record,e->record,m.end,e->end);
        }
    }
    /* The game's end (the TAS capture captures-m6/fam/T1, the mission actor's updates 441, 1067 and 1068 and the RAM
     * dump of update 28967: 42F4 = 6, 42F6 = 0x2C, 42F8 = 0xC, 42FA = 0): after Mission 6 01891E goes to 01893E;
     * 0189FA starts mission 0xC, whose first record is the credits' stage 017C54 (descriptor 01709E, list 025F3E,
     * script 0x858, stage 0x2C, no music, no players), and the stage loop is 018AC6. */
    {
        neo_mission_t e;memset(&e,0,sizeof(e));e.mission=5;e.pause_off=0xff;
        neo_mission_next(&e);
        CHECK(e.state==NEO_MISSION_GAME_END && e.routine==0x1893e && e.mission==6 && !e.stops,"game end: state %u routine %X mission %u",e.state,e.routine,e.mission);
        neo_mission_start(&e,0xc);
        CHECK(e.mission==6 && e.index==0xc && e.record_index==0 && e.record==0x17c54 && e.descriptor==0x1709e && e.stage==0x2c && e.load && e.routine==0x184b4 && e.fade_black,
            "credits: mission %u index %u record %X descriptor %X stage %X routine %X",e.mission,e.index,e.record,e.descriptor,e.stage,e.routine);
        CHECK(neo_mission_map_list(&e)==0x25f3e && neo_mission_script_entry(&e)==0x858 && neo_mission_music(&e)==0 && neo_mission_players(&e)==0,
            "credits descriptor: list %X script %X music %X players %X",neo_mission_map_list(&e),neo_mission_script_entry(&e),neo_mission_music(&e),neo_mission_players(&e));
        neo_mission_update(&e);
        CHECK(e.state==NEO_MISSION_PLAYING && e.routine==0x18ac6 && !e.letters && e.fade_in && e.fade_steps==1 && e.pause_off==0xff && !e.stops,
            "credits stage loop: state %u routine %X letters %d fade %d/%u pause_off %X",e.state,e.routine,e.letters,e.fade_in,e.fade_steps,e.pause_off);
    }
    CHECK(m.stops==0 && wipes==12,"stops %u, wipes %u",m.stops,wipes);
    printf("%s: mission flow %u routine changes, %u RAM samples, %u stage ends (%u mission ends predicted), %u mission starts, %u transition waits of 35 updates, %u explicit stops\n",fails?"FAIL":"PASS",checked,rams,stages,mission_ends,missions,wipes,m.stops);
    return fails!=0;
}
