#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include "../src/marco_campaign_pistol.h"

typedef struct {marco_campaign_shot_t shot;unsigned calls;bool capacity;} sink_t;
static bool spawn(void *context,const marco_campaign_shot_t *shot) {
    sink_t *sink=context;++sink->calls;sink->shot=*shot;return sink->capacity;
}
static int32_t signed32(uint32_t value) {
    return value<=INT32_MAX?(int32_t)value:(int32_t)((int64_t)value-INT64_C(4294967296));
}
static const marco_campaign_event_t event={.kind=NEO_ANIM_EVENT,.target=0xc4372,.site=0x24c70a};
static void original_shots(void) {
    FILE *file=fopen("data/original-pistol-trace.csv","r");assert(file);
    char header[256];assert(fgets(header,sizeof(header),file));
    unsigned frame,first,desired,aim,attributes,dx,dy,angle,checks=0;
    uint32_t x,y,pose,child_x,child_y,vx,vy;
    while(fscanf(file,"%u,%u,%"SCNu32",%"SCNu32",%u,%u,%u,%"SCNu32",%u,%u,%u,%"SCNu32",%"SCNu32",%"SCNu32",%"SCNu32,
        &frame,&first,&x,&y,&desired,&aim,&attributes,&pose,&dx,&dy,&angle,&child_x,&child_y,&vx,&vy)==15) {
        marco_campaign_t campaign={0};marco_campaign_pistol_t pistol;sink_t sink={.capacity=true};
        marco_campaign_pistol_init(&pistol);marco_crouch_t *body=&campaign.body.motion.player;
        body->x=signed32(x);body->y=signed32(y);body->upper.desired90=(uint8_t)desired;
        body->upper.aim92=(uint8_t)aim;body->upper.input.attributes=(uint16_t)attributes;
        campaign.animation.fields.weapon_dx=(uint16_t)(pose>>16);
        campaign.animation.fields.weapon_dy=(uint16_t)pose;
        assert(marco_campaign_pistol_event(&pistol,&campaign,&event,spawn,&sink));
        assert(sink.calls==1 && body->upper.combo95==1);
        assert((uint32_t)sink.shot.x==child_x && (uint32_t)sink.shot.y==child_y);
        assert((uint32_t)sink.shot.vx==vx && (uint32_t)sink.shot.vy==vy && sink.shot.angle==angle);
        assert(first-frame==2);++checks;
    }
    fclose(file);assert(checks==134);
}
static void native_allocation(void) {
    marco_campaign_t campaign={0};marco_campaign_pistol_t pistol;sink_t sink={.capacity=false};
    marco_campaign_pistol_init(&pistol);pistol.ammo.ammunition=4;pistol.ammo.cap=1;
    assert(marco_campaign_pistol_event(&pistol,&campaign,&event,spawn,&sink));
    assert(sink.calls==1 && pistol.ammo.ammunition==3 && pistol.ammo.active==0);
    assert(campaign.body.motion.player.upper.combo95==0);
    sink.capacity=true;campaign.body.motion.player.upper.combo95=255;
    assert(marco_campaign_pistol_event(&pistol,&campaign,&event,spawn,&sink));
    assert(sink.calls==2 && pistol.ammo.ammunition==2 && pistol.ammo.active==1);
    assert(campaign.body.motion.player.upper.combo95==0);
    assert(marco_campaign_pistol_event(&pistol,&campaign,&event,spawn,&sink));
    assert(sink.calls==2 && pistol.ammo.ammunition==1 && pistol.ammo.active==1);
    marco_campaign_pistol_release(&pistol,&campaign,1);assert(pistol.ammo.active==1);
    marco_campaign_pistol_release(&pistol,&campaign,0);
    assert(pistol.ammo.active==0 && pistol.ammo.ammunition==1 && campaign.common.active_projectiles==0);
    campaign.body.motion.player.upper.weapon_kind=1;
    assert(!marco_campaign_pistol_event(&pistol,&campaign,&event,spawn,&sink));
    assert(sink.calls==2 && pistol.ammo.ammunition==1);
    campaign.body.motion.player.upper.weapon_kind=0;campaign.body.motion.player.upper.form93=1;
    assert(!marco_campaign_pistol_event(&pistol,&campaign,&event,spawn,&sink));
    assert(sink.calls==2 && pistol.ammo.ammunition==1);
}
int main(void) {
    original_shots();native_allocation();
    puts("Marco campaign pistol: 134 original shots and real allocation/ammunition boundaries passed");return 0;
}
