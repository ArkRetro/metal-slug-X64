#include "neo_mission.h"
#include <string.h>
#include "neo_gactor.h"   /* ga_rom: region blobs 016DCE.. and 017D44 */

/* Translation of the mission actor's stage flow, docs/MISION2.md. The
 * transition actors of kind 4, 3AED8 (entry) and 3AEF0 (exit), are native
 * (neo_wipe.c): created after the mission actor (0040BC), updated after it
 * in the same update, and their +60 = 1 ends the wait. Kind C (3B2E4 entry,
 * 3B2A2 exit) are explicit stops, counted, and the flow goes on in the next
 * update (as the tally convention of gameplay.c). */
static uint16_t rw(uint32_t a) {const uint8_t *p=ga_rom(a);return p?(uint16_t)(p[0]<<8|p[1]):0xffffu;}
static uint32_t rl(uint32_t a) {return (uint32_t)rw(a)<<16|rw(a+2);}
static void stop(neo_mission_t *m,uint32_t at) {if (!m->stops++) m->stop_at=at;}

/* 01824A: the record of index 42FA among the type-0 records of mission
 * 42F8 (the first record when the list ends first, type 8). */
static uint32_t find(uint16_t mission,uint16_t index) {
    uint32_t first=rl(0x17d44u+4u*mission),a=first;uint16_t n=0;
    for (unsigned guard=0;guard<64;guard++) {
        uint16_t t=rw(a);
        if (t==8) return first;
        if (t==0) {if (n==index) return a;n++;}
        a+=(uint32_t)(int16_t)rw(a+2);
    }
    return first;
}
static void stage_init(neo_mission_t *m);
static void exit_kind(neo_mission_t *m);
/* 0182B6: dispatch on the record type. */
static void dispatch(neo_mission_t *m) {
    for (unsigned guard=0;guard<64;guard++) {
        uint16_t t=rw(m->record);
        if (t==4) {                                         /* 0182DA */
            uint16_t to=rw(m->record+4);
            if (!to) break;
            m->record_index=to;m->record=find(m->index,to);continue;
        }
        if (t!=0) break;                                    /* 8: 018846 */
        /* 0182E8: c8cd6, c09d8, 4302 = 0, 10C2E6 = 0, c8eaa; entry kind. */
        m->descriptor=rl(m->record+0xa);
        m->entry=(uint8_t)rw(m->record+4);m->exit=(uint8_t)rw(m->record+6);
        m->state=NEO_MISSION_STAGE_ENTRY;
        switch (m->entry) {
        case 0:  m->fade_black=true;m->fade_steps=1;stage_init(m);return;   /* 018324: 164F0, 9C0A(E1,E1,E1,0) */
        case 4:  m->routine=0x18364;neo_wipe_create(&m->wipe,false);m->wiping=true;m->signal=false;return; /* 01833E: 15418, +60 = 0, 3AED8 */
        case 8:  m->fade_black=true;m->fade_steps=0x10;m->timer=0x10;       /* 018380 */
                 m->routine=0x1839a;m->timer--;return;                     /* 01839A: first subq */
        default: m->routine=0x183d0;stop(m,0x3b2e4);return;                 /* 0183AC: 3B2E4 */
        }
    }
    m->state=NEO_MISSION_END;m->routine=0x18846;
}
/* 0183D0: the stage loads (3D4EE, 4192, the script actor 101414, c0cd6,
 * players 01805C/018080: 4303 = FF, ca2f2, music +C, 42F6 = +E, the
 * script 019E04 (+8), the map 0C8774 (+0), the players 019604 (+4),
 * +72/+76 = 0xA0000, 01963A), then +7C = 1 and 0184B4 in the same update. */
static void stage_init(neo_mission_t *m) {
    m->end=0xff;m->stage=rw(m->descriptor+0xe);m->load=true;
    m->timer=1;m->routine=0x184b4;m->timer--;            /* 0184B4: 1 -> 0, bcc */
}
/* 0184BC: exit kind (+6). */
static void exit_kind(neo_mission_t *m) {
    m->state=NEO_MISSION_STAGE_EXIT;
    switch (m->exit) {
    case 0: m->fade_in=true;m->fade_steps=1;m->routine=0x1860a;break;                 /* 0184D8 */
    case 4: m->fade_in=true;m->fade_steps=1;m->routine=0x18554;neo_wipe_create(&m->wipe,true);m->wiping=true;m->signal=false;return; /* 018526: +60 = 0, 3AEF0 */
    case 8: m->fade_in=true;m->fade_steps=0x10;m->timer=0x10;m->routine=0x1851a;m->timer--;return; /* 0184F6 */
    default: m->routine=0x1857c;stop(m,0x3b2a2);return;                              /* 018566 */
    }
}
/* 01860A: 4302 = FF, MISSION n START 0607DE when 42FA = 0 (and the AES
 * mode 100088 = 2 of the port), 5287C, 10C2E6 = FF, c8e50, c8ce6, 15422,
 * the continue program 018120, then the stage loop 0186A0. */
static void stage_loop(neo_mission_t *m) {
    m->letters=m->record_index==0;
    m->state=NEO_MISSION_PLAYING;m->routine=0x186a0;
}
/* The transition actor's update after the mission actor's (its node follows
 * it in the list); the fix-map writes stay in m->wipe.fix. */
static void wipe_step(neo_mission_t *m) {
    if (!m->wiping) return;
    if (!neo_wipe_update(&m->wipe,0,0)) {m->wiping=false;if (m->wipe.signal) m->signal=true;}
}
void neo_mission_start(neo_mission_t *m,uint16_t mission) {
    unsigned stops=m->stops;uint32_t at=m->stop_at;uint8_t cleared[6];memcpy(cleared,m->cleared,6);
    uint16_t current=m->mission;
    memset(m,0,sizeof(*m));m->stops=stops;m->stop_at=at;memcpy(m->cleared,cleared,6);
    m->mission=current;m->index=mission;m->record_index=0;
    m->record=find(m->index,m->record_index);dispatch(m);   /* 01829E */
    wipe_step(m);
}
uint8_t neo_mission_update(neo_mission_t *m) {
    m->load=m->letters=m->fade_black=m->fade_in=false;
    switch (m->routine) {
    case 0x18364: if (!m->signal) break;                                   /* 018364: +60 */
                  m->fade_black=true;m->fade_steps=1;stage_init(m);break;   /* 01836C */
    case 0x1839a: if (m->timer--) break;stage_init(m);break;                 /* 0183A2: 164F0 */
    case 0x183d0: stage_init(m);break;
    case 0x184b4: if (m->timer--) break;exit_kind(m);if (m->routine==0x1860a) stage_loop(m);break;
    case 0x1851a: if (m->timer--) break;stage_loop(m);break;
    case 0x18554: if (!m->signal) break;stage_loop(m);break;                  /* 018554 +60, 01855C: 16502 */
    case 0x1857c: stage_loop(m);break;                                       /* 018600: 16502 */
    default: break;
    }
    wipe_step(m);
    return m->state;
}
void neo_mission_stage_done(neo_mission_t *m) {
    m->load=m->letters=m->fade_black=m->fade_in=false;
    if (m->stage<0x30) m->cleared[m->stage>>3]|=(uint8_t)(1u<<(m->stage&7));   /* 018234 */
    if (m->end) {m->record_index=m->end;m->record=find(m->index,m->record_index);}                  /* 018730 */
    else {m->record_index++;m->record+=(uint32_t)(int16_t)rw(m->record+2);}                         /* 0182C8 */
    dispatch(m);wipe_step(m);
}
void neo_mission_next(neo_mission_t *m) {
    m->mission++;                                           /* 01891E */
    if ((uint8_t)m->mission>=6) {m->state=NEO_MISSION_GAME_END;m->routine=0x1893e;stop(m,0x1893e);return;}
    neo_mission_start(m,m->mission);
}
bool neo_mission_ends_after_stage(const neo_mission_t *m) {
    uint32_t a=m->record+(uint32_t)(int16_t)rw(m->record+2);           /* 0182C8 */
    for (unsigned guard=0;guard<64;guard++) {
        uint16_t t=rw(a);
        if (t!=4) return t!=0;                                           /* 0182B6 */
        uint16_t to=rw(a+4);
        if (!to) return true;                                            /* 0182DA */
        a=find(m->index,to);
    }
    return false;
}
uint32_t neo_mission_map_list(const neo_mission_t *m) {return rl(m->descriptor);}
uint32_t neo_mission_players(const neo_mission_t *m) {return rl(m->descriptor+4);}
uint16_t neo_mission_script_entry(const neo_mission_t *m) {return rw(m->descriptor+0xa);}
uint16_t neo_mission_music(const neo_mission_t *m) {return rw(m->descriptor+0xc);}
