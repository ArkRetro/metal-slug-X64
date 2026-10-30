#ifndef MSX_MARCO_ENVIRONMENT_H
#define MSX_MARCO_ENVIRONMENT_H
#include "marco_common.h"
/* Native translations of C3078..C32E0. State names remain neutral: the world
 * semantics are not inferred from animation appearance. No ROM CPU executes. */
typedef enum {MEN_ENTER,MEN_BOUND,MEN_EXIT,MEN_LAUNCH,MEN_LAUNCH_UPDATE,
 MEN_LAUNCH_FINISH,MEN_ESCAPE,MEN_ESCAPE_UPDATE,MEN_FALL} marco_environment_mode_t;
typedef enum {MEN_FAILED,MEN_COMMON,MEN_IDLE_FAST,MEN_FALL_271C,
 MEN_DEATH,MEN_EXTERNAL_160A} marco_environment_result_t;
typedef enum {MEQ_19424,MEQ_18F16,MEQ_AAA88,MEQ_AA5AC} marco_environment_query_t;
typedef enum {MEW_AA550,MEW_AA4C8} marco_environment_world_t;
typedef enum {MET_3192,MET_3232} marco_environment_table_t;
typedef enum {MEM_SUPPORT_143EE,MEM_AIR_C109C} marco_environment_move_t;
typedef struct {
 marco_environment_mode_t mode;
 uint16_t player_slot,field82; /* +70; low word of shared +80 long. */
 uint8_t palette1b;
 bool palette_variant,land_next_tick;
} marco_environment_t;
typedef struct {
 void *context;
 /* 19424 returns D1.byte and carry; the other queries return D0.byte/carry. */
 bool (*query)(void*,marco_environment_query_t,marco_environment_t*,marco_body_t*,uint16_t*,bool*);
 bool (*world)(void*,marco_environment_world_t,marco_environment_t*,marco_body_t*,marco_animation_t*);
 /* 3192 uses C0FB6; 3232 uses C0F32. Caller must bind converted programs. */
 bool (*select)(void*,marco_environment_table_t,marco_body_t*,marco_animation_t*);
 bool (*palette)(void*,uint8_t,uint16_t);
 bool (*move)(void*,marco_environment_move_t,marco_body_t*,marco_animation_t*);
 bool (*animate)(void*,marco_body_t*,marco_animation_t*);
 /* C3214: native equivalent of 0040BC for resource C4A58, argument 0x1C.
  * The world owner retains the allocated native handle for destination C160A. */
 bool (*spawn)(void*,marco_body_t*,bool*);
} marco_environment_services_t;
/* Maps the requests emitted by common_forms; other requests are rejected. */
bool marco_environment_request(marco_environment_t*,marco_common_destination_t);
/* Routes the two implemented same-tick body continuations. C271C maps to
 * MMR_FALL, not MMR_FALL_ACTION/C2706; the latter would overwrite velocity.
 * Other destinations still require their explicit native owner. */
bool marco_environment_route_tail(marco_environment_result_t,marco_body_t*);
/* COMMON requires common processing in this tick; other successful results are
 * same-tick destinations owned by the caller. land_next_tick is a deferred
 * C1EBE request after COMMON, never an immediate landing. Services are required
 * when reached, never replaced with harmless no-ops. Not campaign integration. */
marco_environment_result_t marco_environment_update(marco_environment_t*,marco_common_t*,
 marco_body_t*,marco_animation_t*,const marco_environment_services_t*);
#endif
