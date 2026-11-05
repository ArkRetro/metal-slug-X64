#ifndef MSX_MARCO_CAMPAIGN_H
#define MSX_MARCO_CAMPAIGN_H
#include "marco_common.h"

enum {
 MARCO_BUTTON_UP=1,MARCO_BUTTON_DOWN=2,MARCO_BUTTON_LEFT=4,
 MARCO_BUTTON_RIGHT=8,MARCO_BUTTON_FIRE=16,MARCO_BUTTON_JUMP=32,
 MARCO_BUTTON_GRENADE=64,MARCO_CAMPAIGN_DRAWS=32
};
typedef enum {
 MARCO_CAMPAIGN_READY,MARCO_CAMPAIGN_SERVICE,MARCO_CAMPAIGN_BODY_DESTINATION,
 MARCO_CAMPAIGN_COMMON_DESTINATION,MARCO_CAMPAIGN_ANIMATION_EVENT,
 MARCO_CAMPAIGN_DRAW_CAPACITY
} marco_campaign_status_t;
typedef struct marco_campaign marco_campaign_t;
typedef struct {
 uint32_t target,site; /* Offline provenance tags, never callable addresses. */
 neo_anim_kind_t kind;
 uint32_t asset;
 uint8_t payload[16],length;
 bool service;
} marco_campaign_event_t;
typedef struct {
 void *context;
 /* The campaign supplies select/animate; the world supplies actual movement. */
 marco_motion_services_t motion;
 marco_crouch_services_t crouch;
 /* The campaign supplies select/available/random16; the world supplies queries. */
 marco_stance_services_t stance;
 marco_common_services_t common;
 bool (*random16)(void*,uint16_t*);
 /* Required projection/palette binding for each native body/weapon call. */
 bool (*project)(void*,marco_campaign_t*,marco_animation_view_t*);
 /* Every reached animation effect must have an explicit native owner. */
 bool (*event)(void*,marco_campaign_t*,const marco_campaign_event_t*,neo_anim_render_t*);
} marco_campaign_services_t;
struct marco_campaign {
 marco_body_t body;
 marco_common_t common;
 marco_animation_t animation;
 neo_anim_draw_t draws[MARCO_CAMPAIGN_DRAWS];
 unsigned draw_count,ticks;
 marco_campaign_status_t status;
 uint32_t reason;
 uint16_t input_latch;
 /* Services are borrowed only for the duration of update. */
 const marco_campaign_services_t *services;
};
/* Position uses the collision world's Q16.16, with Y increasing upwards.
 * Entry must already have a verified native support surface. */
void marco_campaign_init(marco_campaign_t*,int32_t x,int32_t y,int facing);
/* One recovered actor update. Campaign owner calls this on alternate video
 * ticks. The live joypad adapter supplies held logical buttons and action
 * trigger edges to the recovered CA1B8/CA2C6 two-lane contract; no fixture or
 * recorded actor state is consumed. Input latch masks 1/2 are retained for
 * the original alternate-provider path; ordinary joypad does not consume them. */
bool marco_campaign_update(marco_campaign_t*,uint16_t held,uint16_t pressed,
 const marco_campaign_services_t*);
const char *marco_campaign_status_name(marco_campaign_status_t);
#endif
