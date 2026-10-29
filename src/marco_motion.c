#include "marco_motion.h"
/* Explicit 68000 long wrapping and arithmetic shift, independent of signed
 * overflow or implementation-defined conversions on the host. */
static int32_t signed32(uint32_t v) {return v<=INT32_MAX?(int32_t)v:(int32_t)((int64_t)v-INT64_C(4294967296));}
static int32_t add32(int32_t a,int32_t b) {return signed32((uint32_t)a+(uint32_t)b);}
static int32_t sub32(int32_t a,int32_t b) {return signed32((uint32_t)a-(uint32_t)b);}
static int32_t asr32(int32_t v,unsigned n) {
 uint32_t bits=(uint32_t)v;return signed32((bits>>n)|(v<0?(UINT32_MAX<<(32-n)):0));
}
static uint32_t input(marco_motion_t *s) {
 marco_upper_t *u=&s->player.upper;return marco_input_filter(&u->input,u->packed,u->gate);
}
bool marco_motion_update(marco_motion_t *s,marco_animation_t *a,const marco_motion_services_t *v) {
 if(!s || !a || !v || s->player.failed || s->player.upper.failed || a->failed) return false;
    s->tail_call=false;
 marco_crouch_t *p=&s->player;marco_upper_t *u=&p->upper;
 uint32_t buttons;bool done=false;uint16_t timer;int32_t target;unsigned shift;
#define SELECT(t) do {if(!v->select || !v->select(v->context,t,s,a)) goto fail;} while(0)
#define MOVE(k) do {if(!v->move || !v->move(v->context,k,s,a)) goto fail;} while(0)
#define ANIMATE() do {if(!v->animate || !v->animate(v->context,s,a,&done)) goto fail;} while(0)
#define REQUEST(m) do {s->mode=m;s->tail_call=true;return true;} while(0)
#define GROUNDED() ((u->input.attributes&64u)!=0)
 switch(s->mode) {
 case MMR_RUN_JUMP:goto run_jump;case MMR_JUMP:goto jump;case MMR_AIR:goto air;
 case MMR_FALL_ACTION:goto fall_action;case MMR_FALL:goto fall;
 case MMR_TURN_START:goto turn_start;case MMR_TURN:goto turn;
 case MMR_TURN_ACTION_START:goto turn_action_start;case MMR_TURN_ACTION:goto turn_action;
 case MMR_RUN_START:goto run_start;case MMR_RUN:goto run;default:goto fail;
 }
run_jump: // C2642: running jump preserves horizontal velocity.
 SELECT(MMT_2642);u->desired90=3;goto launch;
jump: // C2654
 p->vx=0;SELECT(MMT_2658);a->field50_tag=0x2d0ac0;u->desired90=2;
launch: // C266E: source variant 13 adds to the current vertical velocity.
 p->vy=p->field6c==13?add32(p->vy,0xa9000):0xa9000;s->mode=MMR_AIR;
air: // C2692: preserve both input filter calls and long-width arithmetic.
 (void)input(s);p->vy=add32(p->vy,asr32(sub32(-0x160000,p->vy),5));
 buttons=input(s);target=0;shift=5;
 if(buttons&4u) {target=-0x40000;shift=2;}
 if(buttons&8u) {target=0x40000;shift=2;}
 p->vx=add32(p->vx,asr32(sub32(target,p->vx),shift));MOVE(MMM_AIR);
 /* CMP.W #-1 reads the HIGH word of the big-endian velocity, not its sign alone. */
 a->fields.field78=(p->vy>=-65536)?255:0;
 ANIMATE();if(GROUNDED()) s->mode=MMR_LAND_REQUEST;return true;
fall_action: // C2706
 p->vy=-0x100000;MOVE(MMM_FALL_SUPPORT);p->vy=0;
fall: // C271C
 a->field50_tag=0x2d0aa8;s->field7a=255;SELECT(MMT_272A);
 u->desired90=2;a->fields.field78=0;s->mode=MMR_AIR;goto air;
turn_start: // C2746
 a->field50_tag=0x2d0aa8;
 if(u->action98==1) goto turn_action_start;
 u->input.attributes^=1;if(u->action98==2) goto run_start;
 SELECT(u->aim92 && !(u->aim92&128u)?MMT_2778:MMT_2770);
 marco_upper_reset(u);p->motion62=(u->input.attributes&1u)?-0x30000:0x30000;s->mode=MMR_TURN;
turn: // C27A8
 if(!GROUNDED()) goto fall;
 if(u->action98) goto run_start;
 buttons=input(s);if(buttons&32u) goto run_jump;if(buttons&2u) REQUEST(MMR_CROUCH_REQUEST);
 MOVE(MMM_GROUND);ANIMATE();if(done) s->mode=MMR_RUN_START;return true;
turn_action_start: // C27E8
 p->motion62=0;SELECT(MMT_27EC);s->mode=MMR_TURN_ACTION;
turn_action: // C27FA
 if(!GROUNDED()) goto fall;
 if(u->action98!=1) goto run_start;
 buttons=input(s);if(!(buttons&12u)) REQUEST(MMR_IDLE_FAST_REQUEST);
 if(!(u->input.flags9a&8u)) goto run_start;
 if(buttons&32u) goto jump;
 if(buttons&2u) REQUEST(MMR_CROUCH_REQUEST);
 p->motion62=(u->input.attributes&1u)?0x30000:-0x30000;
 MOVE(MMM_GROUND);ANIMATE();return true;
run_start: // C285C
 a->field50_tag=0x2d0aa8;a->fields.frame77=0;SELECT(MMT_2868);
 a->body.speed=0x1c1;u->desired90=1;p->field80=0;p->field81=0x3f;s->mode=MMR_RUN;
run: // C2888
 if(p->field6c==13) a->body.speed=0x237;
 if(s->field86!=255) ++s->field86; // ADDQ.B/SCS/OR saturates.
 timer=((uint16_t)p->field80<<8)|p->field81;
 timer=(timer>>1)|(timer&0x8000u);p->field80=(uint8_t)(timer>>8);p->field81=(uint8_t)timer;
 if(!GROUNDED()) goto fall;
 if(p->field6c==12) REQUEST(MMR_CROUCH_REQUEST);
 buttons=input(s);
 if(u->input.flags9a&8u) {if(u->action98!=2) goto turn_start;u->input.attributes^=1;}
 if(!(buttons&12u)) REQUEST(timer?MMR_IDLE_FAST_REQUEST:MMR_IDLE_REQUEST);
 if(buttons&32u) goto run_jump;
 if(buttons&2u) REQUEST(MMR_CROUCH_REQUEST);
 if(p->field9d && (u->input.flags9a&1u) && (buttons&16u)) REQUEST(MMR_MELEE_REQUEST);
 target=u->form93==2?0x28000:0x30000;p->motion62=(u->input.attributes&1u)?-target:target;
 MOVE(MMM_GROUND);ANIMATE();return true;
fail:
 p->failed=true;u->failed=true;a->failed=true;return false;
#undef SELECT
#undef MOVE
#undef ANIMATE
#undef REQUEST
#undef GROUNDED
}
