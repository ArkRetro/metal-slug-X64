#ifndef MSX_MARCO_CROUCH_H
#define MSX_MARCO_CROUCH_H
#include "marco_upper.h"
/* Native destinations. External tail calls must be executed by the body owner
 * in the same update; they are not an extra frame or a successful idle pose. */
typedef enum {
    MCR_ENTER,MCR_ENTER_WAIT,MCR_AFTER_GRENADE,MCR_AFTER_FIRE,MCR_IDLE_START,
    MCR_IDLE,MCR_TURN_START,MCR_TURN,MCR_MOVE_START,MCR_MOVE,
    MCR_GRENADE,MCR_FIRE,MCR_ATTACK,
    MCR_STAND_REQUEST,MCR_JUMP_REQUEST,MCR_FALL_ACTION_REQUEST,MCR_FALL_REQUEST
} marco_crouch_mode_t;
typedef enum {
    MCT_2224,MCT_2294,MCT_229C,MCT_22A4,MCT_233E,MCT_23CA,MCT_245E,
    MCT_248C,MCT_2494,MCT_24A6,MCT_24AE,MCT_FIRE,MCT_COUNT
} marco_crouch_table_t;
typedef struct {
    marco_crouch_mode_t mode;
    marco_upper_t upper;
    uint8_t field6c,field80,field81,field9d;
    int32_t motion62,x,y,vx,vy;
    bool failed;
} marco_crouch_t;
typedef struct {
    void *context;
    bool (*select)(void*,marco_crouch_table_t,marco_crouch_t*,marco_animation_t*);
    /* Original 014594, including world support/contact. Not just x += vx. */
    bool (*move)(void*,marco_crouch_t*,marco_animation_t*);
    /* Original 00427A carry, including the complete result of a NULL script. */
    bool (*animate)(void*,marco_crouch_t*,marco_animation_t*,bool *complete);
} marco_crouch_services_t;
/* C2214..C263E: body crouch entry, turn, movement, fire and grenade states.
 * On return the owner must run C32F4 common processing, or dispatch the
 * requested external body transition. Missing services poison both clocks. */
bool marco_crouch_update(marco_crouch_t*,marco_animation_t*,const marco_crouch_services_t*);
/* Native body service with the original carry/NULL completion semantics. */
bool marco_crouch_animation_step(marco_crouch_t*,marco_animation_t*,marco_animation_view_t*,
    neo_anim_emit_t,void*,unsigned,neo_anim_event_t,void*,bool *complete);
#endif
