#include "neo_vehicle.h"
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include "vehicle_anim.h"
_Static_assert(offsetof(neo_vehicle_t,x)%4==0 && offsetof(neo_vehicle_t,target_x)%4==0,"Aligned fixed-point fields");
enum { IDLE,IDLE_DAMAGED,DRIVE,DRIVE_DAMAGED,DESTROY };
static const neo_vehicle_anim_frame_t *sequence(unsigned id,unsigned *count) {
    switch (id) {
    case IDLE:*count=2;return vehicle_idle;
    case IDLE_DAMAGED:*count=2;return vehicle_idle_damaged;
    case DRIVE:*count=4;return vehicle_drive;
    case DRIVE_DAMAGED:*count=4;return vehicle_drive_damaged;
    default:*count=3;return vehicle_destroy;
    }
}
static unsigned animation(const neo_vehicle_t *v) {
    if (v->state>=NEO_VEHICLE_DYING) return DESTROY;
    return (v->state==NEO_VEHICLE_MOVING?DRIVE:IDLE)+(v->health<v->max_health/2);
}
static void set_animation(neo_vehicle_t *v) {
    unsigned count;v->animation=animation(v);v->frame=0;
    const neo_vehicle_anim_frame_t *s=sequence(v->animation,&count);
    v->pose=s[0].pose;v->remaining=s[0].ticks;
}
void neo_vehicle_init(neo_vehicle_t *v,int32_t x,int32_t y,int32_t target_x,
    int32_t speed,int facing,uint16_t health,uint8_t flags) {
    assert(v && speed>=0 && health>0);
    memset(v,0,sizeof(*v));v->x=x;v->y=y;v->target_x=target_x;
    v->facing=facing<0?-1:1;v->vx=v->facing*speed;
    v->health=v->max_health=health;v->flags=flags;v->active=true;
    v->state=speed?NEO_VEHICLE_MOVING:NEO_VEHICLE_STOPPED;set_animation(v);
}
void neo_vehicle_damage(neo_vehicle_t *v,unsigned damage) {
    if (!v->active || !v->health || !damage) return;
    v->health=damage>=v->health?0:(uint16_t)(v->health-damage);
    if (!v->health) {v->vx=0;v->state=NEO_VEHICLE_DYING;}
    if (animation(v)!=v->animation) set_animation(v);
}
unsigned neo_vehicle_step(neo_vehicle_t *v) {
    if (!v->active || v->state==NEO_VEHICLE_WRECK) return 0;
    if (v->state==NEO_VEHICLE_DYING && !v->death_started) {
        v->death_started=true;
        --v->remaining;
        return NEO_VEHICLE_SMOKE|((v->flags&1) && !(v->flags&32)?NEO_VEHICLE_STAMP:0);
    }
    if (v->state==NEO_VEHICLE_MOVING) {
        v->x+=v->vx;
        if ((v->facing>0 && v->x>=v->target_x) || (v->facing<0 && v->x<=v->target_x)) {
            v->state=NEO_VEHICLE_STOPPED;v->vx=0;set_animation(v);
        }
    }
    if (--v->remaining) return 0;
    unsigned count;const neo_vehicle_anim_frame_t *s=sequence(v->animation,&count);
    if (++v->frame==count) {
        if (v->animation==DESTROY) {v->frame=count-1;v->state=NEO_VEHICLE_WRECK;return 0;}
        v->frame=0;
    }
    v->pose=s[v->frame].pose;v->remaining=s[v->frame].ticks;
    return v->animation==DESTROY && v->frame==1?NEO_VEHICLE_EXPLOSION:0;
}
bool neo_vehicle_hit_segment(const neo_vehicle_t *v,int32_t previous_x,int32_t x,
    int32_t y,int32_t half_width,int32_t half_height) {
    if (!v->active || !v->health) return false;
    // Original body and cabin rectangles, decoded from the collision descriptors.
    static const int16_t boxes[2][4]={{-48,0,88,28},{-32,0,24,48}};
    int64_t lo=previous_x<x?previous_x:x,hi=previous_x>x?previous_x:x;
    // The arcade collision routine reads integer screen-coordinate words.
    // Keep fractional motion in the actor, but quantize the box anchor here.
    int64_t anchor_x=(int64_t)(v->x/65536)*65536,anchor_y=(int64_t)(v->y/65536)*65536;
    for (unsigned i=0;i<2;i++) {
        int left=boxes[i][0],width=boxes[i][2];
        if (v->facing>0) left=-left-width;
        int64_t bx=anchor_x+left*65536,by=anchor_y+boxes[i][1]*65536;
        if (hi+half_width>=bx && lo-half_width<=bx+width*65536 &&
            (int64_t)y+half_height>=by && (int64_t)y-half_height<=by+boxes[i][3]*65536) return true;
    }
    return false;
}
