#include "marco_campaign_pistol.h"
#include <stddef.h>

static int16_t signed16(uint16_t value) {
    return (int16_t)((int)(value&32767u)-(value&32768u?32768:0));
}
static int32_t signed32(uint32_t value) {
    return value<=INT32_MAX?(int32_t)value:(int32_t)((int64_t)value-INT64_C(4294967296));
}
static int32_t offset(int32_t position,uint16_t delta) {
    return signed32((uint32_t)position+(uint32_t)((int32_t)signed16(delta)*65536));
}
static void publish(const marco_campaign_pistol_t *p,marco_campaign_t *c) {
    c->common.ammunition=p->ammo.ammunition;
    c->common.active_projectiles=p->ammo.active;
}
void marco_campaign_pistol_init(marco_campaign_pistol_t *p) {
    if(p) *p=(marco_campaign_pistol_t){.ammo={.ammunition=UINT16_MAX,.cap=UINT8_MAX,.kind=0}};
}
bool marco_campaign_pistol_event(marco_campaign_pistol_t *p,marco_campaign_t *c,
    const marco_campaign_event_t *event,marco_campaign_pistol_spawn_t spawn,void *context) {
    if(!p || !c || !event || !spawn || event->kind!=NEO_ANIM_EVENT || event->target!=0x0c4372)
        return false;
    marco_crouch_t *body=&c->body.motion.player;marco_upper_t *upper=&body->upper;
    if(upper->weapon_kind || p->ammo.kind || upper->form93) return false;

    /* C4244..C42A8. Normal form words are read from C4204/C4214/C4224;
     * desired90=FF instead selects crouch's C4234 pair and ignores +72/+74.
     * All offset additions/reflections retain the original word wrapping. */
    uint16_t dx,dy;uint8_t angle;
    if(upper->desired90==UINT8_MAX) {dx=16;dy=16;angle=0;}
    else {
        int aim=upper->aim92<128u?upper->aim92:(int)upper->aim92-256;
        if(aim<0) {dx=0;dy=(uint16_t)-16;angle=192;}
        else if(aim>0) {dx=0;dy=40;angle=64;}
        else {dx=16;dy=10;angle=0;}
        dx=(uint16_t)(dx+c->animation.fields.weapon_dx);
        dy=(uint16_t)(dy+c->animation.fields.weapon_dy);
    }
    if(upper->input.attributes&1u) {dx=(uint16_t)(0u-dx);angle=(uint8_t)(128u-angle);}
    marco_campaign_shot_t shot={.x=offset(body->x,dx),.y=offset(body->y,dy),.angle=angle,.kind=0};
    /* 01B794 sets scalar 00100000; 01B7D8 calls 002E7C. Cardinal sine
     * words at 0029FC/002A7C are 0,+256,0,-256, giving exactly 16 px. */
    switch(angle) {
    case 0:shot.vx=0x100000;break;
    case 64:shot.vy=0x100000;break;
    case 128:shot.vx=-0x100000;break;
    case 192:shot.vy=-0x100000;break;
    default:return false;
    }
    /* C4384 reserves before C4422 allocates. C4526 releases the reservation
     * on allocation failure; C4430 increments combo only after real success. */
    if(!marco_ammo_reserve(&p->ammo,0,p->unlimited,p->special)) {publish(p,c);return true;}
    bool allocated=spawn(context,&shot);
    if(!allocated) marco_ammo_release(&p->ammo,0);
    marco_upper_projectile_created(upper,allocated);
    publish(p,c);return true;
}
void marco_campaign_pistol_release(marco_campaign_pistol_t *p,marco_campaign_t *c,uint8_t kind) {
    if(!p) return;
    marco_ammo_release(&p->ammo,kind);
    if(c) publish(p,c);
}
