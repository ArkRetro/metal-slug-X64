#ifndef NEO_LSPC_H
#define NEO_LSPC_H
/* The LSPC auto-animation counter (MISSION3_DEBUG.md G25).
 *
 * 002802 (D0 = frames per step): D0 = 0 writes 0 to the speed byte 100012
 * and sets bit 3 of 100013 (auto-animation off); else 100012 = D0 - 1 and
 * bit 3 cleared; the word goes to REG_LSPCMODE 3C0006. The stage camera
 * lists call it with op 0x38 (0C85AC, word +4; neo_camera.c
 * NEO_CAMERA_EV_ANIM_SPEED). Every frame the LSPC (MAME 0.264
 * neosprite_base_device::auto_animation_timer_callback) does: frame counter
 * 0 -> reload it with the speed and add 1 to the counter; else frame counter
 * - 1. An animated tile shows code | (counter & 7) (& 3 for 4 images) while
 * auto-animation is on; off, the code as written (phase 0 for the map cells,
 * whose ROM codes have those bits clear). Checked against the counter bits
 * of 3C0006 in run4 (117990..149790, 653 samples) and the boss capture spr
 * (144480..149280, 125 samples): tools/test_neo_lspc.c. */
#include <stdbool.h>
#include <stdint.h>

typedef struct { uint8_t speed,frame_counter,counter; bool disabled; } neo_lspc_t;

/* 002802 with D0 = frames per step. */
static inline void neo_lspc_set(neo_lspc_t *l,uint16_t d0) {
    if (d0==0) {l->speed=0;l->disabled=true;}
    else {l->speed=(uint8_t)(d0-1u);l->disabled=false;}
}
/* One frame (the timer at VSSTART). */
static inline void neo_lspc_frame(neo_lspc_t *l) {
    if (l->frame_counter==0) {l->frame_counter=l->speed;l->counter++;}
    else l->frame_counter--;
}
/* The phase the map cells show (scene_cell_phase). */
static inline unsigned neo_lspc_phase(const neo_lspc_t *l) {return l->disabled?0u:(unsigned)(l->counter&7u);}
#endif
