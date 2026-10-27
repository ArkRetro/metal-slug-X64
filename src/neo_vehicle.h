#ifndef MSX_NEO_VEHICLE_H
#define MSX_NEO_VEHICLE_H
#include <stdbool.h>
#include <stdint.h>
/* All positions/velocities are signed Q16.16 world units. Y increases upwards.
 * Call step once per recovered arcade object tick, not once per rendered VI. */
enum { NEO_VEHICLE_MOVING,NEO_VEHICLE_STOPPED,NEO_VEHICLE_DYING,NEO_VEHICLE_WRECK };
enum { NEO_VEHICLE_STAMP=1,NEO_VEHICLE_SMOKE=2,NEO_VEHICLE_EXPLOSION=4 };
typedef struct { uint8_t pose,ticks; } neo_vehicle_anim_frame_t;
typedef struct {
    int32_t x,y,vx,target_x;
    uint16_t health,max_health;
    int8_t facing;
    uint8_t state,flags,animation,frame,remaining,pose;
    bool active,death_started;
} neo_vehicle_t;
void neo_vehicle_init(neo_vehicle_t *v,int32_t x,int32_t y,int32_t target_x,
    int32_t speed,int facing,uint16_t health,uint8_t flags);
unsigned neo_vehicle_step(neo_vehicle_t *v);
void neo_vehicle_damage(neo_vehicle_t *v,unsigned damage);
bool neo_vehicle_hit_segment(const neo_vehicle_t *v,int32_t previous_x,int32_t x,
    int32_t y,int32_t half_width,int32_t half_height);
#endif
