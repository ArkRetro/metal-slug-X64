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
    unsigned n=sizeof mission_flow_events/sizeof mission_flow_events[0],checked=0,rams=0,stages=0,missions=0,wipes=0;
    for (unsigned i=0;i<n;i++) {
        const mission_flow_event_t *e=&mission_flow_events[i];
        if (e->previous==0x186A0) {
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
    CHECK(m.stops==0 && wipes==12,"stops %u, wipes %u",m.stops,wipes);
    printf("%s: mission flow %u routine changes, %u RAM samples, %u stage ends, %u mission starts, %u transition waits of 35 updates, %u explicit stops\n",fails?"FAIL":"PASS",checked,rams,stages,missions,wipes,m.stops);
    return fails!=0;
}
