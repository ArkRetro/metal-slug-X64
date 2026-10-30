#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/marco_environment.h"
#include "marco_environment_fixture.h"
#ifdef __mips__
#include <libdragon.h>
#endif
static struct {marco_environment_query_t kind;uint16_t value;bool carry;} queries[4];
static unsigned wanted,at;static char events[32];static unsigned used;
static bool allocated,ground_on_move;
static uint16_t palette_value;static uint8_t palette_slot;
static void event(char c) {assert(used+1<sizeof(events));events[used++]=c;events[used]=0;}
static bool query(void *ctx,marco_environment_query_t k,marco_environment_t *e,marco_body_t *b,uint16_t *v,bool *c) {
 (void)ctx;(void)e;(void)b;event('Q');assert(at<wanted && queries[at].kind==k);*v=queries[at].value;*c=queries[at++].carry;return true;
}
static bool world(void *ctx,marco_environment_world_t k,marco_environment_t *e,marco_body_t *b,marco_animation_t *a) {
 (void)ctx;(void)k;(void)e;(void)b;(void)a;event('W');return true;
}
static bool select_program(void *ctx,marco_environment_table_t t,marco_body_t *b,marco_animation_t *a) {
 (void)ctx;(void)t;(void)b;(void)a;event('S');return true;
}
static bool palette(void *ctx,uint8_t slot,uint16_t value) {(void)ctx;event('P');palette_slot=slot;palette_value=value;return true;}
static bool move(void *ctx,marco_environment_move_t k,marco_body_t *b,marco_animation_t *a) {
 (void)ctx;(void)k;(void)a;event('M');if(ground_on_move)b->motion.player.upper.input.attributes|=64;return true;
}
static bool animate(void *ctx,marco_body_t *b,marco_animation_t *a) {(void)ctx;(void)b;(void)a;event('A');return true;}
static bool spawn(void *ctx,marco_body_t *b,bool *ok) {(void)ctx;(void)b;event('E');*ok=allocated;return true;}
static const marco_environment_services_t services={.query=query,.world=world,.select=select_program,.palette=palette,.move=move,.animate=animate,.spawn=spawn};
static void clear(void) {used=at=wanted=0;events[0]=0;ground_on_move=allocated=false;}
static void q(marco_environment_query_t k,uint16_t value,bool carry) {queries[wanted].kind=k;queries[wanted].value=value;queries[wanted++].carry=carry;}
static int32_t signed32(uint32_t v) {return v<=INT32_MAX?(int32_t)v:(int32_t)((int64_t)v-INT64_C(4294967296));}
static uint32_t read80(const marco_environment_t *e,const marco_body_t *b) {return ((uint32_t)b->motion.player.field80<<24)|((uint32_t)b->motion.player.field81<<16)|e->field82;}
static void original_arithmetic(void) {
 for(unsigned i=0;i<sizeof(env_cases)/sizeof(*env_cases);i++) {
  clear();marco_environment_t e={.mode=MEN_LAUNCH_UPDATE,.field82=(uint16_t)env_cases[i][0]};
  marco_body_t b={0};marco_common_t s={0};marco_animation_t a={0};
  b.motion.player.field80=(uint8_t)(env_cases[i][0]>>24);b.motion.player.field81=(uint8_t)(env_cases[i][0]>>16);b.motion.player.vy=signed32(env_cases[i][1]);
  assert(marco_environment_update(&e,&s,&b,&a,&services)==MEN_COMMON);
  assert(read80(&e,&b)==env_cases[i][2] && (uint32_t)b.motion.player.vy==env_cases[i][3]);
  assert(!strcmp(events,"WMA") && s.timer84==6);
 }
}
static void branches(void) {
 marco_environment_t e={0};marco_body_t b={0};marco_common_t s={0};marco_animation_t a={0};
 marco_upper_t *u=&b.motion.player.upper;
 clear();assert(marco_environment_request(&e,MCC_ENTER_3078));assert(!marco_environment_request(&e,MCC_DEATH_2956));
 u->input.attributes=0x8101;u->aim92=7;a.field50_tag=0x1234;u->action98=4;
 q(MEQ_19424,0,false);q(MEQ_18F16,0,false);
 assert(marco_environment_update(&e,&s,&b,&a,&services)==MEN_COMMON);
 assert(!strcmp(events,"WQQ") && at==wanted && e.mode==MEN_BOUND && (u->input.flags9a&128));
 assert(u->input.attributes==0x8100 && !a.field50_tag && u->aim92==7 && !u->action98 && u->active91==255);
 // The carry on 19424 skips 18F16 and proceeds directly to the native launch.
 clear();q(MEQ_19424,0,true);q(MEQ_AAA88,4,false);e.palette_variant=true;e.palette1b=9;u->character=1;
 assert(marco_environment_update(&e,&s,&b,&a,&services)==MEN_COMMON);
 assert(!strcmp(events,"WQQPSWMA") && at==wanted && palette_slot==9 && palette_value==0x1d5);
 assert(read80(&e,&b)==0xd8000 && b.motion.player.vy==0xc1800 && e.mode==MEN_LAUNCH_UPDATE);
 // Completion state is scheduled; this tick still goes through common.
 clear();ground_on_move=true;
 assert(marco_environment_update(&e,&s,&b,&a,&services)==MEN_COMMON && e.mode==MEN_LAUNCH_FINISH);
 clear();assert(marco_environment_update(&e,&s,&b,&a,&services)==MEN_FALL_271C && !strcmp(events,"E"));
 clear();allocated=true;e.mode=MEN_LAUNCH_FINISH;
 assert(marco_environment_update(&e,&s,&b,&a,&services)==MEN_EXTERNAL_160A && s.timer84==90);
 // Exit code 0 must use D1.byte from 19424, not the old D0 exit code.
 clear();e.mode=MEN_EXIT;q(MEQ_AA5AC,0,false);q(MEQ_19424,0x1a5,false);
 assert(marco_environment_update(&e,&s,&b,&a,&services)==MEN_DEATH && s.field5a==0xa5 && (u->input.flags5f&128));
 clear();e.mode=MEN_EXIT;q(MEQ_AA5AC,2,false);b.motion.player.vx=99;
 assert(marco_environment_update(&e,&s,&b,&a,&services)==MEN_FALL_271C && b.motion.player.vy==0x20000 && !b.motion.player.vx);
 assert(marco_environment_route_tail(MEN_FALL_271C,&b));
 assert(b.domain==MARCO_BODY_MOTION && b.motion.mode==MMR_FALL && b.motion.player.vy==0x20000);
 assert(!marco_environment_route_tail(MEN_DEATH,&b));
 assert(marco_environment_route_tail(MEN_IDLE_FAST,&b) && b.domain==MARCO_BODY_STANCE && b.stance.mode==MST_IDLE_FAST);
 for(unsigned code=3;code<=5;code+=2) {
  clear();e.mode=MEN_EXIT;q(MEQ_AA5AC,(uint16_t)code,false);
  assert(marco_environment_update(&e,&s,&b,&a,&services)==MEN_IDLE_FAST && at==wanted);
  if(code==5)assert(u->input.attributes&1);
 }
 // Right takes priority over simultaneous left. Input-filter side effects survive.
 clear();e.mode=MEN_ESCAPE;u->packed=0x000c000c;u->input.mask9c=255;u->input.attributes=0;
 q(MEQ_19424,0,false);ground_on_move=true;
 assert(marco_environment_update(&e,&s,&b,&a,&services)==MEN_COMMON);
 assert(!strcmp(events,"SWQMA") && b.motion.player.vx==0x10000 && e.land_next_tick && (u->input.accumulator9b&12)==12);
 assert(s.timer84==0x5e); // Previous 0x5A OR 0x1E; never replace the timer.
 // Owner must dispatch the scheduled landing; missing services stop the actor.
 clear();assert(marco_environment_update(&e,&s,&b,&a,&services)==MEN_FAILED && a.failed);
 b=(marco_body_t){0};a=(marco_animation_t){0};e=(marco_environment_t){.mode=MEN_LAUNCH};
 marco_environment_services_t missing=services;missing.palette=NULL;
 assert(marco_environment_update(&e,&s,&b,&a,&missing)==MEN_FAILED && b.motion.player.failed && !used);
}
int main(void) {
#ifdef __mips__
 debug_init_emulog();console_init();console_set_render_mode(RENDER_MANUAL);
#endif
 original_arithmetic();branches();
 puts("PASS: 256 original M68000 arithmetic cases, environment branch/order/alias and missing-service boundaries");
#ifdef __mips__
 debugf("MSX ENVIRONMENT REGRESSION PASS\n");console_render();while(1) {}
#endif
 return 0;
}
