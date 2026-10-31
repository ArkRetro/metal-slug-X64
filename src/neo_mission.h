#ifndef MSX_NEO_MISSION_H
#define MSX_NEO_MISSION_H
#include <stdbool.h>
#include <stdint.h>
/*
 * Stage flow of the mission actor 101364 (ROM 0x01829E..0x01860A, 0x018722
 * and 0x01891E), docs/MISION2.md. RAM words with A5 = 0x108000: 42F4
 * mission, 42F6 stage id, 42F8/42FA mission and record index of 01824A,
 * 4303 stage end (018F40 clears it), 4314.. cleared-stage bits (018234).
 * Records (table 017D44): type 0 stage (+4 entry kind, +6 exit kind, +A
 * stage descriptor: +0 map/camera command list, +8 script table entry,
 * +C music, +E stage id), type 4 jump to record +4 (0 = mission end),
 * type 8 mission end (018846).
 */
enum {
    NEO_MISSION_STAGE_ENTRY,    /* 0182E8 entry kind (+4) running */
    NEO_MISSION_STAGE_EXIT,     /* 0184B4.. exit kind (+6) running */
    NEO_MISSION_PLAYING,        /* 0186A0 stage loop */
    NEO_MISSION_END,            /* 018846 mission end (gameplay.c) */
    NEO_MISSION_GAME_END,       /* 01893E: after mission 6 (not ported) */
};
typedef struct {
    uint16_t mission,stage,index,record_index;   /* 42F4 42F6 42F8 42FA */
    uint8_t end,cleared[6];                       /* 4303, 4314.. */
    uint32_t record,descriptor,routine;           /* +6E, record +A, +00 */
    uint16_t timer;                               /* +7C */
    uint8_t state,entry,exit;
    bool load;            /* 0183D0 ran this update: load the stage */
    bool letters;         /* 018646: MISSION n START (42FA = 0) */
    bool fade_black,fade_in;uint8_t fade_steps;   /* 9C0A requests */
    unsigned stops;uint32_t stop_at;              /* explicit stops */
} neo_mission_t;
/* 01829E with 42F8 = mission, 42FA = record_index (0 at a mission start). */
void neo_mission_start(neo_mission_t *m,uint16_t mission);
/* One update of the mission actor outside the stage loop and the mission
 * end: entry and exit kinds. Returns the state. */
uint8_t neo_mission_update(neo_mission_t *m);
/* 018722: the stage loop saw the stage end (193FE carry, 4303 >= 0). */
void neo_mission_stage_done(neo_mission_t *m);
/* 01891E after the mission end: the next mission or the game end. */
void neo_mission_next(neo_mission_t *m);
/* Descriptor words of the current stage (record +A). */
uint32_t neo_mission_map_list(const neo_mission_t *m);    /* +0 */
uint32_t neo_mission_players(const neo_mission_t *m);     /* +4 */
uint16_t neo_mission_script_entry(const neo_mission_t *m);/* +8, table 0x000800 */
uint16_t neo_mission_music(const neo_mission_t *m);       /* +C */
#endif
