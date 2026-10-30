#ifndef MSX_NEO_MOTION_H
#define MSX_NEO_MOTION_H
#include <stdint.h>
/* Recovered player air update. Q16.16 world Y increases upwards.
 * Run once per TWO Neo Geo video ticks. Collision resolution is separate. */
enum { NEO_UNIT=65536, NEO_JUMP_VY=0xa9000 };
typedef struct { int32_t x,y,vx,vy; } neo_air_t;
static inline int32_t neo_asr(int64_t v,unsigned shift) {
    int64_t divisor=(int64_t)1<<shift;
    return (int32_t)(v>=0?v/divisor:-((-v+divisor-1)/divisor));
}
/* Acceleration (03BD3A), then the displacement of the air step (0143EE);
 * the solid-body push-out 004ED2 runs between them. */
static inline void neo_air_accel(neo_air_t *p,int direction) {
    p->vy+=neo_asr(-22*(int64_t)NEO_UNIT-p->vy,5);
    p->vx+=neo_asr(direction*4*(int64_t)NEO_UNIT-p->vx,direction?2:5);
}
static inline void neo_air_step(neo_air_t *p,int direction) {
    neo_air_accel(p,direction);
    p->x+=p->vx;p->y+=p->vy;
}
#endif
