#ifndef MSX_MARCO_UPPER_H
#define MSX_MARCO_UPPER_H
#include "marco_controller.h"
/* Native state identifiers. The ROM addresses in the implementation document
 * provenance; none is a callable pointer or a runtime instruction cursor. */
typedef enum {
    MUP_RESET,MUP_SETTLE,MUP_POSE0,MUP_ACTIVE,MUP_MELEE_WAIT,MUP_GRENADE,
    MUP_FIRE,MUP_FIRE_WAIT,MUP_REPEAT_A_WAIT,MUP_REPEAT_B_WAIT,
    MUP_TURN_WAIT,MUP_TURN_DOWN,MUP_TURN_UP,MUP_TURN_FROM_UP,MUP_TURN_FROM_DOWN,
    MUP_AIM_WAIT,MUP_DISABLED
} marco_upper_mode_t;
/* Read-only table keys named by their verified original LEA instruction. */
typedef enum {
    MUT_383C,MUT_3850,MUT_386A,MUT_3872,MUT_3898,MUT_38A0,MUT_38C6,
    MUT_38D2,MUT_38DA,MUT_38F4,MUT_3998,MUT_39A4,MUT_39AC,MUT_39F4,
    MUT_3A3A,MUT_3B00,MUT_3B5E,MUT_3B6C,MUT_3BEC,MUT_3C04,
    MUT_3E28,MUT_3E3A,MUT_3E4C,MUT_3ED0,MUT_3EEC,MUT_3F00,
    MUT_3FAC,MUT_3FBA,MUT_3FC8,MUT_3FD6,MUT_4106,MUT_4118,
    MUT_412A,MUT_413C,MUT_414E,MUT_4160,
    MUT_FIRE_FLAT,MUT_FIRE_UP,MUT_FIRE_DOWN,MUT_COUNT
} marco_upper_table_t;
typedef struct {
    marco_upper_mode_t mode;
    marco_input_state_t input;
    marco_input_gate_t gate;
    uint32_t packed;
    uint16_t character;
    uint8_t desired90,active91,aim92,form93,combo95,idle97,action98;
    uint8_t weapon_kind,bombs;
    bool unlimited_bombs,failed;
} marco_upper_t;
typedef struct {
    void *context;
    const uint16_t *compatibility;unsigned compatibility_count;
    const uint8_t *combo_limits;unsigned combo_limit_count;
    bool (*select)(void*,marco_upper_table_t,bool direct,marco_upper_t*,marco_animation_t*);
    bool (*available)(void*,marco_upper_table_t,marco_upper_t*,bool*);
    bool (*random16)(void*,uint16_t*);
    /* complete means D7.w != 0 (C3B38 etc.), NOT helper carry. A NULL
     * animation has carry set but D7=0 and must report complete=false. */
    bool (*animate)(void*,marco_upper_t*,marco_animation_t*,bool *complete);
    /* Original input-buffer word +4 OR #2. Native owner schedules/latches it;
     * this interface never writes an original RAM address. */
    bool (*latch_input)(void*,uint16_t);
} marco_upper_services_t;
/* C0F16: preserve aim, clocks and other actor fields. */
void marco_upper_reset(marco_upper_t *state);
/* C4430 / C44A6: increment only after the native world allocator succeeds. */
void marco_upper_projectile_created(marco_upper_t *state,bool allocated);
/* C478E..C47A8: authorize/consume one grenade before subtype/spawn handling. */
bool marco_upper_consume_grenade(marco_upper_t *state);
/* C37CE..C419C: one original upper-controller call. Unknown content/services
 * stop the state, rather than substitute an idle pose or discard an effect. */
bool marco_upper_update(marco_upper_t*,marco_animation_t*,const marco_upper_services_t*);
/* Standard native animation service. The caller supplies world projection and
 * blink phase in view; this preserves the original controller's D7 test. */
bool marco_upper_animation_step(marco_upper_t*,marco_animation_t*,marco_animation_view_t*,
    neo_anim_emit_t,void*,unsigned,neo_anim_event_t,void*,bool *complete);
#endif
