#include "marco_common.h"
bool marco_common_begin(marco_common_t *s,marco_body_t *b,marco_animation_t *a,
 const marco_common_services_t *v,const marco_upper_services_t *upper) {
 if(!s || !b || !a || !v || !upper) return false;
 marco_crouch_t *p=&b->motion.player;marco_upper_t *u=&p->upper;
 if(p->failed || u->failed || a->failed) return false;
 if(!v->prepare || !v->prepare(v->context,s,b,a)) goto fail;
 a->fields.variant79=u->desired90==3?1:0; // C32FA..C3304
 if(!marco_upper_update(u,a,upper)) goto fail; // C3308..C330C
 if(s->field5a==15) s->field9e=255;
 p->field9d=((u->form93==0 || u->form93==1) && (p->field6c==7 || p->field6c==9))?255:0;
 return true;
fail:
 p->failed=true;u->failed=true;a->failed=true;return false;
}
bool marco_common_palette(marco_common_t *s,marco_body_t *b,marco_animation_t *a,const marco_common_services_t *v) {
 if(!s || !b || !a || !v) return false;
 marco_crouch_t *p=&b->motion.player;
 if(p->failed || p->upper.failed || a->failed) return false;
 if(!s->field99) return true;
 --s->field99;uint16_t descriptor=0x390;
 if(s->field99!=3 && s->field99!=5) {
  marco_common_palette_t key=s->field99==1?MCP_35FC:s->alternate_palette?MCP_3614:MCP_361C;
  if(!v->palette_lookup || !v->palette_lookup(v->context,key,p->upper.character,&descriptor)) goto fail;
 }
 if(!v->palette_write || !v->palette_write(v->context,s->field19,descriptor)) goto fail;
 return true;
fail:
 p->failed=true;p->upper.failed=true;a->failed=true;return false;
}
bool marco_common_tail(marco_common_t *s,marco_body_t *b,marco_animation_t *a,const marco_common_services_t *v) {
 if(!s || !b || !a || !v) return false;
 marco_crouch_t *p=&b->motion.player;marco_upper_t *u=&p->upper;
 if(p->failed || u->failed || a->failed) return false;
 uint16_t value=0;bool carry=false;
#define QUERY(k) do {if(!v->query || !v->query(v->context,k,b,&value,&carry)) goto fail;} while(0)
 s->destination=MCC_KEEP;
 u->input.flags5f&=(uint8_t)~64u; // C3634
 if(s->timer84) {
  u->input.flags5f|=64;
  u->input.attributes=(uint16_t)((u->input.attributes&255u)|((uint16_t)s->field19<<8));
  --s->timer84;
  if(!s->timer84) u->input.flags9a&=(uint8_t)~4u;
  else if((u->input.flags9a&4u) && s->timer84>=16 && (s->timer84&1u))
   u->input.attributes=(uint16_t)((u->input.attributes&255u)|((uint16_t)s->field1a<<8));
 }
 if(s->global_invincible) u->input.flags5f|=64;
 QUERY(MCQ_PROTECTION_CA2F8);if(carry) u->input.flags5f|=64;
 if(u->input.flags5f&64u) {s->field5a=0;u->input.flags5f&=(uint8_t)~128u;}
 QUERY(MCQ_CONTACT_4C24);
 if((uint8_t)value==16) {
  if(u->form93==3) s->destination=MCC_DEATH_2956;
  else {
   s->field5a=0;u->input.flags5f=(uint8_t)((u->input.flags5f&~128u)|64u);s->field94=3;
   if(!v->reset_weapon || !v->reset_weapon(v->context,b)) goto fail;
  }
 } else if(u->input.flags5f&128u) s->destination=MCC_DEATH_2956;
 if(s->projected_y<s->death_line) {
  u->input.flags5f|=128;s->destination=MCC_FALL_DEATH_2D6A;
 }
 if(s->field9e) {
  u->input.flags5f|=128;s->field5a=6;s->destination=MCC_DEATH_2956;
 }
 u->input.flags9a&=(uint8_t)~1u;
 QUERY(MCQ_PROXIMITY_49E8);
 if((uint8_t)value==4 || (uint8_t)value==5) u->input.flags9a|=1;
 u->input.mask9c=255;return true;
fail:
 p->failed=true;u->failed=true;a->failed=true;return false;
#undef QUERY
}
