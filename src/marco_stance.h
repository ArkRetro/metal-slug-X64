#ifndef MSX_MARCO_STANCE_H
#define MSX_MARCO_STANCE_H
#include "marco_motion.h"
typedef enum {
    MST_STAND,MST_LAND,MST_IDLE_START,MST_IDLE_FAST,MST_BRAKE,MST_IDLE
} marco_stance_mode_t;
typedef enum {MST_1E3C,MST_1EA2,MST_1F20,MST_1F8A,MST_TABLE_COUNT} marco_stance_table_t;
typedef struct {
    marco_stance_mode_t mode;
    uint8_t field7b,field96;
    uint16_t field1e,field66;
} marco_stance_t;
typedef struct {
    void *context;
    bool (*select)(void*,marco_stance_table_t,marco_stance_t*,marco_motion_t*,marco_animation_t*);
    /* C1EE0..C1EF4: original 0040BC spawn followed by 01ACF2 placement.
     * Required only for form93==2. Missing implementation stops the actor. */
    bool (*land_effect)(void*,marco_stance_t*,marco_motion_t*,marco_animation_t*);
} marco_stance_services_t;
/* Entry paths C1E34, C1E94, C1EBE, C1EFA, C1F4A -> C1FA6.
 * Does not advance animation or run the C1FA6 idle body. The owner must enter
 * that body in this same tick; MST_IDLE is an explicit boundary, not a frame. */
bool marco_stance_enter(marco_stance_t*,marco_motion_t*,marco_animation_t*,const marco_stance_services_t*);
#endif
