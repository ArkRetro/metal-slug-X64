#include "marco_campaign.h"
#include <string.h>
#include "marco_campaign_assets.h"

static bool stop(marco_campaign_t *c,marco_campaign_status_t status,uint32_t reason) {
 if(c->status==MARCO_CAMPAIGN_READY) {c->status=status;c->reason=reason;}
 return false;
}
static bool service(marco_campaign_t *c,bool result,uint32_t reason) {
 return result || stop(c,MARCO_CAMPAIGN_SERVICE,reason);
}
static bool table(marco_campaign_t *c,uint32_t tag,const marco_program_table_t **out) {
 return service(c,asset_table(tag,out),tag);
}
static bool select_program(marco_campaign_t *c,uint32_t tag,marco_part_t part,
 bool direct,marco_upper_t *u,marco_animation_t *a) {
 const marco_program_table_t *programs;
 if(!table(c,tag,&programs)) return false;
 return service(c,marco_program_select(a,part,programs,direct?0:u->form93,u->character),tag);
}
static bool available_program(marco_campaign_t *c,uint32_t tag,marco_upper_t *u,bool *ready) {
 const marco_program_table_t *programs;
 if(!table(c,tag,&programs)) return false;
 return service(c,marco_program_available(programs,u->form93,u->character,ready),tag);
}
static bool motion_select(void *ctx,marco_motion_table_t key,marco_motion_t *m,marco_animation_t *a) {
 marco_campaign_t *c=ctx;
 if(key>=MMT_COUNT) return stop(c,MARCO_CAMPAIGN_SERVICE,key);
 return select_program(c,marco_campaign_motion_tags[key],MARCO_BODY,false,&m->player.upper,a);
}
static bool crouch_select(void *ctx,marco_crouch_table_t key,marco_crouch_t *p,marco_animation_t *a) {
 marco_campaign_t *c=ctx;uint32_t tag;
 if(key==MCT_FIRE) {
  if(p->upper.weapon_kind>=16) return stop(c,MARCO_CAMPAIGN_SERVICE,key);
  tag=marco_campaign_crouch_fire_tags[p->upper.weapon_kind];
 } else {
  if(key>=MCT_FIRE) return stop(c,MARCO_CAMPAIGN_SERVICE,key);
  tag=marco_campaign_crouch_tags[key];
 }
 return select_program(c,tag,MARCO_BODY,false,&p->upper,a);
}
static bool stance_select(void *ctx,marco_stance_table_t key,marco_stance_t *s,
 marco_motion_t *m,marco_animation_t *a) {
 marco_campaign_t *c=ctx;(void)s;
 if(key>=MST_TABLE_COUNT) return stop(c,MARCO_CAMPAIGN_SERVICE,key);
 return select_program(c,marco_campaign_stance_tags[key],MARCO_BODY,false,&m->player.upper,a);
}
static bool upper_tag(marco_campaign_t *c,marco_upper_table_t key,marco_upper_t *u,uint32_t *tag) {
 if(key>=MUT_COUNT || u->weapon_kind>=16) return stop(c,MARCO_CAMPAIGN_SERVICE,key);
 *tag=key<MUT_FIRE_FLAT?marco_campaign_upper_tags[key]:
  marco_campaign_upper_fire_tags[key-MUT_FIRE_FLAT][u->weapon_kind];
 return true;
}
static bool upper_select(void *ctx,marco_upper_table_t key,bool direct,
 marco_upper_t *u,marco_animation_t *a) {
 marco_campaign_t *c=ctx;uint32_t tag;
 return upper_tag(c,key,u,&tag) && select_program(c,tag,MARCO_WEAPON,direct,u,a);
}
static bool upper_available(void *ctx,marco_upper_table_t key,marco_upper_t *u,bool *ready) {
 marco_campaign_t *c=ctx;uint32_t tag;
 return upper_tag(c,key,u,&tag) && available_program(c,tag,u,ready);
}
static bool stance_available(void *ctx,marco_stance_table_t key,marco_motion_t *m,bool *ready) {
 marco_campaign_t *c=ctx;
 if(key>=MST_TABLE_COUNT) return stop(c,MARCO_CAMPAIGN_SERVICE,key);
 return available_program(c,marco_campaign_stance_tags[key],&m->player.upper,ready);
}
static bool random16(void *ctx,uint16_t *value) {
 marco_campaign_t *c=ctx;const marco_campaign_services_t *v=c->services;
 return service(c,v->random16 && v->random16(v->context,value),0x0029c6);
}
static void emit(void *ctx,const neo_anim_draw_t *draw) {
 marco_campaign_t *c=ctx;
 if(c->draw_count>=MARCO_CAMPAIGN_DRAWS) {stop(c,MARCO_CAMPAIGN_DRAW_CAPACITY,draw->descriptor);return;}
 c->draws[c->draw_count++]=*draw;
}
static bool animation_event(void *ctx,neo_anim_kind_t kind,uint32_t asset,neo_anim_render_t *render) {
 marco_campaign_t *c=ctx;const marco_campaign_services_t *v=c->services;
 marco_campaign_event_t e={.kind=kind,.asset=asset};
 if(kind==NEO_ANIM_EVENT && !asset_event(asset,&e.target,&e.site))
  return stop(c,MARCO_CAMPAIGN_ANIMATION_EVENT,asset);
 if(kind==NEO_ANIM_EVENT) {
  e.length=marco_campaign_asset_events[asset].length;
  e.service=marco_campaign_asset_events[asset].service;
  memcpy(e.payload,marco_campaign_asset_events[asset].payload,e.length);
 }
 if(!v->event || !v->event(v->context,c,&e,render))
  return stop(c,MARCO_CAMPAIGN_ANIMATION_EVENT,e.target?e.target:asset);
 return c->status==MARCO_CAMPAIGN_READY;
}
static bool project(marco_campaign_t *c,marco_animation_view_t *view) {
 const marco_campaign_services_t *v=c->services;memset(view,0,sizeof(*view));
 return service(c,v->project && v->project(v->context,c,view),0x01abc8);
}
static bool crouch_animate(void *ctx,marco_crouch_t *p,marco_animation_t *a,bool *complete) {
 marco_campaign_t *c=ctx;marco_animation_view_t view;
 if(!project(c,&view)) return false;
 return service(c,marco_crouch_animation_step(p,a,&view,emit,c,256,
  animation_event,c,complete),0x00427a) && c->status==MARCO_CAMPAIGN_READY;
}
static bool motion_animate(void *ctx,marco_motion_t *m,marco_animation_t *a,bool *complete) {
 return crouch_animate(ctx,&m->player,a,complete);
}
static bool upper_animate(void *ctx,marco_upper_t *u,marco_animation_t *a,bool *complete) {
 marco_campaign_t *c=ctx;marco_animation_view_t view;
 if(!project(c,&view)) return false;
 return service(c,marco_upper_animation_step(u,a,&view,emit,c,256,
  animation_event,c,complete),0x0c3778) && c->status==MARCO_CAMPAIGN_READY;
}
static bool motion_move(void *ctx,marco_motion_service_t kind,marco_motion_t *m,marco_animation_t *a) {
 marco_campaign_t *c=ctx;const marco_motion_services_t *v=&c->services->motion;
 static const uint32_t source[]={0x014594,0x0c109c,0x0143ee};
 return service(c,v->move && v->move(v->context,kind,m,a),kind<=MMM_FALL_SUPPORT?source[kind]:0);
}
static bool crouch_move(void *ctx,marco_crouch_t *p,marco_animation_t *a) {
 marco_campaign_t *c=ctx;const marco_crouch_services_t *v=&c->services->crouch;
 return service(c,v->move && v->move(v->context,p,a),0x014594);
}
static bool land_effect(void *ctx,marco_stance_t *s,marco_motion_t *m,marco_animation_t *a) {
 marco_campaign_t *c=ctx;const marco_stance_services_t *v=&c->services->stance;
 return service(c,v->land_effect && v->land_effect(v->context,s,m,a),0x0c1ee0);
}
static bool idle_query(void *ctx,marco_idle_query_t key,int16_t arg,
 marco_motion_t *m,uint16_t *value,bool *carry) {
 marco_campaign_t *c=ctx;const marco_stance_services_t *v=&c->services->stance;
 static const uint32_t source[]={0x018f24,0x018ffe,0x014850,0x0191fc};
 return service(c,v->idle_query && v->idle_query(v->context,key,arg,m,value,carry),
  key<=MSQ_191FC?source[key]:0);
}
static bool latch_input(void *ctx,uint16_t mask) {
 /* CA202 consumes this only for an alternate scripted input provider. The
  * ordinary live-joypad CA268 path retains it without changing button lanes. */
 marco_campaign_t *c=ctx;c->input_latch|=mask;return true;
}
static bool common_prepare(void *ctx,marco_common_t *s,marco_body_t *b,marco_animation_t *a) {
 marco_campaign_t *c=ctx;const marco_common_services_t *v=&c->services->common;
 return service(c,v->prepare && v->prepare(v->context,s,b,a),0x019432);
}
static bool common_query(void *ctx,marco_common_query_t key,marco_body_t *b,uint16_t *value,bool *carry) {
 marco_campaign_t *c=ctx;const marco_common_services_t *v=&c->services->common;
 static const uint32_t source[]={0x0ca2f8,0x004c24,0x0049e8,0x0aa458,0x0c0bae};
 return service(c,v->query && v->query(v->context,key,b,value,carry),key<=MCQ_FORM_C0BAE?source[key]:0);
}
static bool palette_lookup(void *ctx,marco_common_palette_t key,uint16_t character,uint16_t *value) {
 marco_campaign_t *c=ctx;const marco_common_services_t *v=&c->services->common;
 return service(c,v->palette_lookup && v->palette_lookup(v->context,key,character,value),0x0c35d0);
}
static bool palette_write(void *ctx,uint8_t bank,uint16_t value) {
 marco_campaign_t *c=ctx;const marco_common_services_t *v=&c->services->common;
 return service(c,v->palette_write && v->palette_write(v->context,bank,value),0x009d1e);
}
static bool form_effect(void *ctx,marco_body_t *b) {
 marco_campaign_t *c=ctx;const marco_common_services_t *v=&c->services->common;
 return service(c,v->form_effect && v->form_effect(v->context,b),0x0c3598);
}
static bool reset_weapon(void *ctx,marco_body_t *b) {
 marco_campaign_t *c=ctx;const marco_common_services_t *v=&c->services->common;
 return service(c,v->reset_weapon && v->reset_weapon(v->context,b),0x0c0ee2);
}
void marco_campaign_init(marco_campaign_t *c,int32_t x,int32_t y,int facing) {
 memset(c,0,sizeof(*c));c->body.domain=MARCO_BODY_STANCE;c->body.stance.mode=MST_IDLE_START;
 marco_crouch_t *p=&c->body.motion.player;marco_upper_t *u=&p->upper;
 p->x=x;p->y=y;u->input.attributes=(uint16_t)(64u|(facing<0?1u:0u));
 u->input.mask9c=255;u->gate.limit=255;marco_upper_reset(u);
 c->common.ammunition=UINT16_MAX;c->common.field94=255;
 marco_animation_reset(&c->animation);
}
bool marco_campaign_update(marco_campaign_t *c,uint16_t held,uint16_t pressed,
 const marco_campaign_services_t *v) {
 if(!c || !v || c->status!=MARCO_CAMPAIGN_READY) return false;
 c->services=v;c->draw_count=0;
 marco_upper_t *u=&c->body.motion.player.upper;
 /* CA1B8/CA2C6: directions are held in both lanes, action trigger pulses
  * occupy the high word and held actions occupy the low word. C0FEA SWAPs
  * these lanes before the recovered body/upper controllers consume them. */
 uint16_t down=(uint16_t)(held|pressed);
 uint16_t triggers=(uint16_t)((down&15u)|(pressed&240u));
 u->packed=((uint32_t)triggers<<16)|down;
 marco_motion_services_t motion={.context=c,.select=motion_select,.move=motion_move,.animate=motion_animate};
 marco_crouch_services_t crouch={.context=c,.select=crouch_select,.move=crouch_move,.animate=crouch_animate};
 marco_stance_services_t stance={.context=c,.select=stance_select,.land_effect=land_effect,
  .idle_query=idle_query,.available=stance_available,.random16=random16,.latch_input=latch_input};
 marco_upper_services_t upper={.context=c,.compatibility=marco_campaign_compatibility,
  .compatibility_count=sizeof(marco_campaign_compatibility)/sizeof(*marco_campaign_compatibility),
  .combo_limits=marco_campaign_combo_limits,.combo_limit_count=sizeof(marco_campaign_combo_limits),
  .select=upper_select,.available=upper_available,.random16=random16,.animate=upper_animate,
  .latch_input=latch_input};
 marco_common_services_t common={.context=c,.query=common_query,.prepare=common_prepare,
  .palette_lookup=palette_lookup,.palette_write=palette_write,.form_effect=form_effect,.reset_weapon=reset_weapon};
 marco_body_result_t result=marco_body_update(&c->body,&c->animation,&motion,&crouch,&stance);
 if(result==MARCO_BODY_EXTERNAL) stop(c,MARCO_CAMPAIGN_BODY_DESTINATION,
  c->body.domain==MARCO_BODY_MOTION?0x0c215e:(uint32_t)c->body.stance.mode);
 else if(result!=MARCO_BODY_COMMON) stop(c,MARCO_CAMPAIGN_SERVICE,0);
 else if(!marco_common_update(&c->common,&c->body,&c->animation,&common,&upper))
  stop(c,MARCO_CAMPAIGN_SERVICE,0x0c32f4);
 else if(c->common.destination!=MCC_KEEP)
  stop(c,MARCO_CAMPAIGN_COMMON_DESTINATION,(uint32_t)c->common.destination);
 c->services=NULL;
 if(c->status!=MARCO_CAMPAIGN_READY) return false;
 ++c->ticks;return true;
}
const char *marco_campaign_status_name(marco_campaign_status_t status) {
 switch(status) {
 case MARCO_CAMPAIGN_READY:return "ready";
 case MARCO_CAMPAIGN_SERVICE:return "world/program service";
 case MARCO_CAMPAIGN_BODY_DESTINATION:return "body destination";
 case MARCO_CAMPAIGN_COMMON_DESTINATION:return "common destination";
 case MARCO_CAMPAIGN_ANIMATION_EVENT:return "animation effect";
 case MARCO_CAMPAIGN_DRAW_CAPACITY:return "draw capacity";
 default:return "invalid status";
 }
}
