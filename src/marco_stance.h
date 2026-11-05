#ifndef MSX_MARCO_STANCE_H
#define MSX_MARCO_STANCE_H
#include "marco_motion.h"
typedef enum {
    MST_STAND,MST_LAND,MST_IDLE_START,MST_IDLE_FAST,MST_BRAKE,MST_IDLE,
    MST_VARIANT_E84,MST_VARIANT_EAA,MST_VARIANT_E54,MST_VARIANT_E44,
    MST_VARIANT_E64,MST_VARIANT_E74
} marco_stance_mode_t;
typedef enum {MST_1E3C,MST_1EA2,MST_1F20,MST_1F8A,MST_203A,MST_2056,MST_20B6,MST_20DA,MST_2100,MST_2124,MST_2178,MST_2180,MST_TABLE_COUNT} marco_stance_table_t;
typedef enum {MSQ_18F24,MSQ_18FFE,MSQ_14850,MSQ_191FC} marco_idle_query_t;
typedef struct {
    marco_stance_mode_t mode;
    uint8_t field7b,field96,field5e;
    uint16_t field1e,field66;
} marco_stance_t;
typedef struct {
    void *context;
    bool (*select)(void*,marco_stance_table_t,marco_stance_t*,marco_motion_t*,marco_animation_t*);
    /* C1EE0..C1EF4: original 0040BC spawn followed by 01ACF2 placement.
     * Required only for form93==2. Missing implementation stops the actor. */
    bool (*land_effect)(void*,marco_stance_t*,marco_motion_t*,marco_animation_t*);
    bool (*idle_query)(void*,marco_idle_query_t,int16_t,marco_motion_t*,uint16_t*,bool*);
    bool (*available)(void*,marco_stance_table_t,marco_motion_t*,bool*);
    bool (*random16)(void*,uint16_t*);
    /* C21E2: the body melee owner retains the selected input-buffer latch. */
    bool (*latch_input)(void*,uint16_t);
} marco_stance_services_t;
/* Entry paths C1E34, C1E94, C1EBE, C1EFA, C1F4A -> C1FA6.
 * Does not advance animation or run the C1FA6 idle body. The owner must enter
 * that body in this same tick; MST_IDLE is an explicit boundary, not a frame. */
bool marco_stance_enter(marco_stance_t*,marco_motion_t*,marco_animation_t*,const marco_stance_services_t*);
typedef enum {
    MSI_FALL,MSI_CROUCH,MSI_RUN,MSI_JUMP,MSI_MELEE,MSI_FAST,
    MSI_DRAW,MSI_WORLD
} marco_idle_branch_t;
/* C1FA6..C2022: original input priority and filter side effects. MSI_DRAW
 * continues at C2144; MSI_WORLD needs the original world/idle decisions.
 * This is a branch decision, not a completed body update. */
marco_idle_branch_t marco_stance_idle_branch(marco_motion_t*);
/* C2022..C213E: schedules optional idle variants; caller must still execute
 * support/body animation in this tick. World queries remain native services. */
bool marco_stance_idle_world(marco_stance_t*,marco_motion_t*,marco_animation_t*,const marco_stance_services_t*);
#endif
