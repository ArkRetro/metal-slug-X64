#ifndef MSX_NEO_CONTROL_H
#define MSX_NEO_CONTROL_H
#include <stdbool.h>
#include <stdint.h>
#include "neo_rng.h"
/* Neutral names identify original fields, not an emulated actor layout. */
typedef struct {
    int32_t target84,velocity88;
    uint16_t phase8c,timer70,counter74,delay92,target8e,target90,selector6e;
    uint8_t flags15,flags5f,flags7c,selected80,variant68;
} neo_control_t;
typedef struct {
    uint16_t x,y,origin_x,origin_y,p1x,p1y,p2x,p2y;
    uint8_t active_mask,mode88,rank;
    bool targets_valid;
    const uint16_t *delay_grid; // Native 16x8 lookup consumed by original 00510C.
} neo_control_frame_t;
/* ROM AE9C0..AEB3E, with player targets supplied through the native scene API. */
void neo_control_target(neo_control_t *state,const neo_control_frame_t *frame);
/* ROM AEB5E..AED1C, includes original probability table and RNG flag decisions.
 * false denotes an unimplemented table/profile, not an invented fallback. */
bool neo_control_prefix(neo_control_t *state,const neo_control_frame_t *frame,neo_rng_t *rng);
/* ROM AED1C..AEDA6. Probe kernel itself remains outside this module. */
void neo_control_finish(neo_control_t *state,uint32_t probe1_flags,uint32_t probe2_flags);
uint16_t neo_control_probability(const neo_control_t *state,const neo_control_frame_t *frame);
#endif
