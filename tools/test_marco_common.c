/* Synthetic boundary tests based on addressed C3634..C3776 disassembly.
 * No dynamic MAME coverage or completed campaign claim. */
#include <assert.h>
#include <stdio.h>
#include "../src/marco_common.h"
#ifdef __mips__
#include <libdragon.h>
#endif
static unsigned queries,resets;
static uint16_t contact,proximity;
static bool protection;
static bool query(void *ctx,marco_common_query_t k,marco_body_t *b,uint16_t *value,bool *carry) {
 (void)ctx;(void)b;assert((unsigned)k==queries++);*carry=false;*value=0;
 if(k==MCQ_PROTECTION_CA2F8) *carry=protection;
 if(k==MCQ_CONTACT_4C24) *value=contact;
 if(k==MCQ_PROXIMITY_49E8) *value=proximity;
 return true;
}
static bool reset(void *ctx,marco_body_t *b) {(void)ctx;(void)b;++resets;return true;}
static void run(marco_common_t *s,marco_body_t *b,marco_animation_t *a) {
 marco_common_services_t v={.query=query,.reset_weapon=reset};queries=0;
 assert(marco_common_tail(s,b,a,&v) && queries==3);
 assert(b->motion.player.upper.input.mask9c==255);
}
static unsigned stage;
static bool prepare(void *ctx,marco_common_t *s,marco_body_t *b,marco_animation_t *a) {
 (void)ctx;(void)a;assert(stage==0);stage=1;
 s->field5a=15;b->motion.player.upper.desired90=3;return true;
}
static bool select_upper(void *ctx,marco_upper_table_t table,bool direct,marco_upper_t *u,marco_animation_t *a) {
 (void)ctx;(void)u;assert(stage==1 && table==MUT_383C && !direct && a->fields.variant79==1);
 stage=2;return true;
}
static bool animate_upper(void *ctx,marco_upper_t *u,marco_animation_t *a,bool *done) {
 (void)ctx;(void)a;assert(stage==2);stage=3;u->form93=1;*done=false;return true;
}
static void begin_limits(void) {
 marco_body_t b={0};marco_common_t s={0};marco_animation_t a;marco_animation_reset(&a);
 b.motion.player.field6c=7;marco_upper_reset(&b.motion.player.upper);
 static const uint16_t compatible[4]={0xffff,0xffff,0xffff,0xffff};
 marco_upper_services_t upper={.compatibility=compatible,.compatibility_count=4,.select=select_upper,.animate=animate_upper};
 marco_common_services_t common={.prepare=prepare};stage=0;
 assert(marco_common_begin(&s,&b,&a,&common,&upper));
 assert(stage==3 && s.field9e==255 && b.motion.player.field9d==255);
 // Invalid/unimplemented world preparation must not reach the upper controller.
 common.prepare=NULL;stage=0;
 assert(!marco_common_begin(&s,&b,&a,&common,&upper) && !stage && b.motion.player.failed && a.failed);
}
static unsigned palette_writes,palette_reads;
static marco_common_palette_t palette_key;
static uint16_t palette_value;
static bool palette_lookup(void *ctx,marco_common_palette_t key,uint16_t character,uint16_t *out) {
 (void)ctx;assert(character==2);++palette_reads;palette_key=key;*out=0x456;return true;
}
static bool palette_write(void *ctx,uint8_t slot,uint16_t value) {
 (void)ctx;assert(slot==7);++palette_writes;palette_value=value;return true;
}
static void palette_limits(void) {
 marco_body_t b={0};marco_animation_t a;marco_animation_reset(&a);
 marco_common_t s={.field19=7,.field99=6};b.motion.player.upper.character=2;
 marco_common_services_t v={.palette_lookup=palette_lookup,.palette_write=palette_write};
 assert(marco_common_palette(&s,&b,&a,&v) && s.field99==5 && palette_value==0x390 && !palette_reads);
 s.field99=2;assert(marco_common_palette(&s,&b,&a,&v) && palette_key==MCP_35FC && palette_value==0x456);
 assert(marco_common_palette(&s,&b,&a,&v) && !s.field99 && palette_key==MCP_361C);
 unsigned before=palette_writes;assert(marco_common_palette(&s,&b,&a,&v) && palette_writes==before);
 s.alternate_palette=true;s.field99=1;
 assert(marco_common_palette(&s,&b,&a,&v) && palette_key==MCP_3614);
 s.field99=4;v.palette_write=NULL;
 assert(!marco_common_palette(&s,&b,&a,&v) && b.motion.player.failed && a.failed);
}
int main(void) {
#ifdef __mips__
 debug_init_emulog();console_init();console_set_render_mode(RENDER_MANUAL);
#endif
 begin_limits();palette_limits();
 marco_body_t b={0};marco_animation_t a;marco_animation_reset(&a);
 marco_common_t s={.timer84=18,.field19=0x32,.field1a=0x45,.field5a=7,.death_line=-100};
 marco_upper_t *u=&b.motion.player.upper;
 u->input.attributes=0xaa41;u->input.flags9a=4;u->input.flags5f=128;proximity=5;
 run(&s,&b,&a);assert(s.timer84==17 && u->input.attributes==0x4541 && s.field5a==0);
 assert(s.destination==MCC_KEEP && u->input.flags5f==64 && (u->input.flags9a&1));
 s.timer84=1;proximity=0;run(&s,&b,&a);
 assert(!s.timer84 && !(u->input.flags9a&5) && u->input.attributes==0x3241);
 protection=true;u->input.flags5f=128;run(&s,&b,&a);assert(s.destination==MCC_KEEP && u->input.flags5f==64);
 protection=false;contact=16;u->form93=0;run(&s,&b,&a);assert(s.field94==3 && resets==1 && s.destination==MCC_KEEP);
 u->form93=3;run(&s,&b,&a);assert(s.destination==MCC_DEATH_2956 && resets==1);
 s.projected_y=-101;run(&s,&b,&a);assert(s.destination==MCC_FALL_DEATH_2D6A);
 s.field9e=1;run(&s,&b,&a);assert(s.destination==MCC_DEATH_2956 && s.field5a==6);
 marco_common_services_t missing={0};assert(!marco_common_tail(&s,&b,&a,&missing) && b.motion.player.failed && a.failed);
 puts("PASS: common-tail timer/palette width, query order, immunity, form request and death precedence");
#ifdef __mips__
 debugf("MSX COMMON REGRESSION PASS\n");console_render();while(1) {}
#endif
 return 0;
}
