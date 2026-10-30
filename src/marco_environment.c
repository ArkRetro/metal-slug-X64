#include "marco_environment.h"
static int32_t signed32(uint32_t v) {return v<=INT32_MAX?(int32_t)v:(int32_t)((int64_t)v-INT64_C(4294967296));}
static int32_t add32(int32_t a,int32_t b) {return signed32((uint32_t)a+(uint32_t)b);}
static int32_t sub32(int32_t a,int32_t b) {return signed32((uint32_t)a-(uint32_t)b);}
static int32_t asr32(int32_t v,unsigned n) {uint32_t b=(uint32_t)v;return signed32((b>>n)|(v<0?(UINT32_MAX<<(32-n)):0));}
/* +80 is also accessed by the crouch/run controllers as bytes +80/+81.
 * Assemble it explicitly; do not overlay a packed M68000 struct on host/MIPS. */
static int32_t long80(const marco_environment_t *e,const marco_crouch_t *p) {
 return signed32(((uint32_t)p->field80<<24)|((uint32_t)p->field81<<16)|e->field82);
}
static void write80(marco_environment_t *e,marco_crouch_t *p,int32_t v) {
 p->field80=(uint8_t)((uint32_t)v>>24);p->field81=(uint8_t)((uint32_t)v>>16);e->field82=(uint16_t)v;
}
bool marco_environment_request(marco_environment_t *e,marco_common_destination_t request) {
 if(!e) return false;
 if(request==MCC_ENTER_3078) e->mode=MEN_ENTER;
 else if(request==MCC_EXIT_30F0) e->mode=MEN_EXIT;
 else return false;
 e->land_next_tick=false;return true;
}
bool marco_environment_route_tail(marco_environment_result_t result,marco_body_t *b) {
 if(!b) return false;
 if(result==MEN_FALL_271C) {b->domain=MARCO_BODY_MOTION;b->motion.mode=MMR_FALL;return true;}
 if(result==MEN_IDLE_FAST) {b->domain=MARCO_BODY_STANCE;b->stance.mode=MST_IDLE_FAST;return true;}
 return false;
}
marco_environment_result_t marco_environment_update(marco_environment_t *e,marco_common_t *s,
 marco_body_t *b,marco_animation_t *a,const marco_environment_services_t *v) {
 if(!e || !s || !b || !a || !v) return MEN_FAILED;
 marco_motion_t *m=&b->motion;marco_crouch_t *p=&m->player;marco_upper_t *u=&p->upper;
 if(p->failed || u->failed || a->failed || e->land_next_tick) goto fail;
 uint16_t value=0;bool carry=false,allocated=false;int32_t target,accumulator;unsigned shift;
#define QUERY(k) do {if(!v->query || !v->query(v->context,k,e,b,&value,&carry)) goto fail;} while(0)
#define WORLD(k) do {if(!v->world || !v->world(v->context,k,e,b,a)) goto fail;} while(0)
#define SELECT(k) do {if(!v->select || !v->select(v->context,k,b,a)) goto fail;} while(0)
#define MOVE(k) do {if(!v->move || !v->move(v->context,k,b,a)) goto fail;} while(0)
#define ANIMATE() do {if(!v->animate || !v->animate(v->context,b,a)) goto fail;} while(0)
 switch(e->mode) {
 case MEN_ENTER: // C3078 falls through into C309C in this same update.
  b->stance.field7b=m->field7a=255;u->input.attributes&=(uint16_t)~1u;
  a->field50_tag=0;marco_upper_reset(u);e->mode=MEN_BOUND;
  /* fall through */
 case MEN_BOUND:
  WORLD(MEW_AA550);QUERY(MEQ_19424);
  if(!carry) {QUERY(MEQ_18F16);if(!carry) {u->input.flags9a|=128;return MEN_COMMON;}}
  u->input.flags9a&=(uint8_t)~128u;QUERY(MEQ_AAA88);
  if((uint8_t)value==4) goto launch;
  if(!(u->input.attributes&64u)) {u->input.flags9a|=128;return MEN_COMMON;}
  goto escape;
 case MEN_EXIT:
  u->input.flags9a&=(uint8_t)~128u;QUERY(MEQ_AA5AC);
  switch((uint8_t)value) {
  case 0:QUERY(MEQ_19424);if(carry) goto escape;
   s->field5a=(uint8_t)value;u->input.flags5f|=128;return MEN_DEATH;
  case 2:goto fall;
  case 3:return MEN_IDLE_FAST;
  case 5:u->input.attributes|=1;return MEN_IDLE_FAST;
  case 4:goto launch;
  default:goto escape;
  }
 case MEN_LAUNCH:goto launch;
 case MEN_LAUNCH_UPDATE:goto launch_update;
 case MEN_LAUNCH_FINISH:
  if(!v->spawn || !v->spawn(v->context,b,&allocated)) goto fail;
  if(!allocated) return MEN_FALL_271C;
  s->timer84=0x5a;return MEN_EXTERNAL_160A;
 case MEN_ESCAPE:goto escape;
 case MEN_ESCAPE_UPDATE:goto escape_update;
 case MEN_FALL:goto fall;
 default:goto fail;
 }
launch: // C315A..C31B2. ROM C3152 is a four-word character palette table.
 b->stance.field66=b->stance.field1e=0x87f4;
 value=0x0d;
 if(e->palette_variant) {
  static const uint16_t palettes[4]={0x1d4,0x1d5,0x1d4,0x1d6};
  if(u->character>=4) goto fail;
  value=palettes[u->character];
 }
 if(!v->palette || !v->palette(v->context,e->palette1b,value)) goto fail;
 SELECT(MET_3192);p->vx=0;p->vy=0xc0000;write80(e,p,0x100000);e->mode=MEN_LAUNCH_UPDATE;
launch_update:
 WORLD(MEW_AA4C8);
 accumulator=long80(e,p);accumulator=add32(accumulator,asr32(sub32(-0x180000,accumulator),4));write80(e,p,accumulator);
 p->vy=add32(p->vy,asr32(sub32(accumulator,p->vy),4));
 MOVE(MEM_SUPPORT_143EE);ANIMATE();
 if(p->vy<=-0x40000 || (u->input.attributes&64u)) e->mode=MEN_LAUNCH_FINISH;
 s->timer84|=6;return MEN_COMMON;
escape: // C3232 enters C3250 immediately, not on the following tick.
 SELECT(MET_3232);marco_upper_reset(u);p->vx=0;p->vy=0xb7000;e->mode=MEN_ESCAPE_UPDATE;
escape_update:
 WORLD(MEW_AA4C8);s->timer84|=0x1e;
 (void)marco_input_filter(&u->input,u->packed,u->gate); // C325C is observable even when carry later skips horizontal input.
 p->vy=add32(p->vy,asr32(sub32(-0x160000,p->vy),5));QUERY(MEQ_19424);
 if(!carry) {
  uint32_t buttons=marco_input_filter(&u->input,u->packed,u->gate);target=0;shift=5;
  if(buttons&4u) {target=-0x40000;shift=2;}
  if(buttons&8u) {target=0x40000;shift=2;} // Right wins when both bits are set.
  p->vx=add32(p->vx,asr32(sub32(target,p->vx),shift));
 }
 MOVE(MEM_AIR_C109C);ANIMATE();
 if(u->input.attributes&64u) e->land_next_tick=true;
 return MEN_COMMON;
fall: // C32D0 tail-calls C271C; its animation start is owned by motion.
 p->vx=0;p->vy=0x20000;return MEN_FALL_271C;
fail:
 p->failed=true;u->failed=true;a->failed=true;return MEN_FAILED;
#undef QUERY
#undef WORLD
#undef SELECT
#undef MOVE
#undef ANIMATE
}
