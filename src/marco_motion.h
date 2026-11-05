#ifndef MSX_MARCO_MOTION_H
#define MSX_MARCO_MOTION_H
#include "marco_crouch.h"
typedef enum {
 MMR_RUN_JUMP,MMR_JUMP,MMR_AIR,MMR_FALL_ACTION,MMR_FALL,
 MMR_TURN_START,MMR_TURN,MMR_TURN_ACTION_START,MMR_TURN_ACTION,
 MMR_RUN_START,MMR_RUN,MMR_LAND_REQUEST,MMR_CROUCH_REQUEST,
 MMR_IDLE_FAST_REQUEST,MMR_IDLE_REQUEST,MMR_MELEE_REQUEST,MMR_MELEE
} marco_motion_mode_t;
typedef enum {MMT_2642,MMT_2658,MMT_272A,MMT_2770,MMT_2778,MMT_27EC,MMT_2868,MMT_COUNT} marco_motion_table_t;
typedef enum {MMM_GROUND,MMM_AIR,MMM_FALL_SUPPORT} marco_motion_service_t;
typedef struct {
 marco_motion_mode_t mode;
 marco_crouch_t player;
 uint8_t field7a,field86;
    bool tail_call; /* True only for a same-tick branch outside this controller. */
} marco_motion_t;
typedef struct {
 void *context;
 bool (*select)(void*,marco_motion_table_t,marco_motion_t*,marco_animation_t*);
 /* Original world helpers 014594, 0C109C, 0143EE respectively. */
 bool (*move)(void*,marco_motion_service_t,marco_motion_t*,marco_animation_t*);
 bool (*animate)(void*,marco_motion_t*,marco_animation_t*,bool*);
} marco_motion_services_t;
/* C2642..C294A. Only tail_call requests dispatch in this tick; landing is scheduled for the next.
 * Source routine addresses document provenance, never executable pointers. */
bool marco_motion_update(marco_motion_t*,marco_animation_t*,const marco_motion_services_t*);
#endif
