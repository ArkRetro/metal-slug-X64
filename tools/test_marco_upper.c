#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/marco_upper.h"
#include "marco_upper_fixture.h"
#ifdef __mips__
#include <libdragon.h>
#endif
static unsigned case_index,next_op,end_op,total_ops;
static uint32_t source_clock;
static uint16_t latch_word;
#ifdef MARCO_UPPER_NATIVE_ANIMATION
#include "marco_upper_native.h"
static unsigned native_draws,native_events;
static const upper_op_t *native_operation;
static unsigned draw_index,event_index;
static marco_upper_t *native_owner;
static unsigned native_cursor(uint32_t tag) {
    unsigned lo=0,hi=sizeof(upper_source_commands)/sizeof(*upper_source_commands);
    while(lo<hi) {unsigned mid=lo+(hi-lo)/2;if(upper_source_commands[mid]<tag) lo=mid+1;else hi=mid;}
    assert(lo<sizeof(upper_source_commands)/sizeof(*upper_source_commands) && upper_source_commands[lo]==tag);return lo;
}
static int native_table(uint32_t tag) {
    for(unsigned i=0;i<sizeof(upper_native_tables)/sizeof(*upper_native_tables);i++) if(upper_native_tables[i].tag==tag) return (int)i;
    return -1;
}
#else
static const neo_anim_command_t stub_commands[]={{.kind=NEO_ANIM_FINISH}};
static const neo_anim_program_t stub_program={stub_commands,1};
#endif
static const upper_op_t *take(unsigned kind) {
    if(next_op>=end_op || upper_operations[next_op].kind!=kind) {
        printf("upper case %u op %u requested=%u actual=%u\n",case_index,next_op,kind,next_op<end_op?upper_operations[next_op].kind:99u);assert(false);
    }
    total_ops++;return &upper_operations[next_op++];
}
static uint32_t table_tag(marco_upper_table_t key,marco_upper_t *s) {
    if(key<MUT_FIRE_FLAT) return upper_table_tags[key];
    assert(key<MUT_COUNT && s->weapon_kind<16);
    return upper_fire_tables[key-MUT_FIRE_FLAT][s->weapon_kind];
}
static bool select_table(void *ctx,marco_upper_table_t key,bool direct,marco_upper_t *s,marco_animation_t *a) {
    (void)ctx;const upper_op_t *op=take(0);
    assert(table_tag(key,s)==op->table && direct==op->direct);
    /* Source clock is a provenance tag of the replayed animation service,
     * never placed in the native cursor or called as a CPU address. */
#ifdef MARCO_UPPER_NATIVE_ANIMATION
    int i=native_table(op->table);assert(i>=0);
    assert(marco_program_select(a,MARCO_WEAPON,&upper_native_tables[i].table,direct?0:s->form93,s->character));
    source_clock=a->weapon.program?upper_source_commands[a->weapon.cursor]:0;
    assert(source_clock==op->program);return true;
#else
    source_clock=op->program;
    return marco_animation_start(a,MARCO_WEAPON,source_clock?&stub_program:NULL,0,256);
#endif
}
static bool available(void *ctx,marco_upper_table_t key,marco_upper_t *s,bool *result) {
    (void)ctx;const upper_op_t *op=take(1);assert(table_tag(key,s)==op->table);
#ifdef MARCO_UPPER_NATIVE_ANIMATION
    int i=native_table(op->table);assert(i>=0);unsigned index=s->form93*4u+s->character;
    assert(index<upper_native_tables[i].table.count);*result=(upper_native_tables[i].present&(1u<<index))!=0;assert(*result==(op->value!=0));
#else
    *result=op->value!=0;
#endif
    return true;
}
static bool random16(void *ctx,uint16_t *result) {(void)ctx;*result=(uint16_t)take(2)->value;return true;}
static void anim_snapshot(marco_animation_t *a,uint32_t out[A_COUNT]) {
    uint32_t values[]={source_clock,a->weapon.timer,a->weapon.speed,a->flash,a->field18,a->field50_tag,
        ((uint32_t)a->fields.weapon_dx<<16)|a->fields.weapon_dy,a->fields.field76,a->fields.frame77,a->fields.field78,a->fields.variant79};
    memcpy(out,values,sizeof(values));
}
static void anim_seed(marco_animation_t *a,const uint32_t *v) {
    source_clock=v[A_CURSOR];
#ifdef MARCO_UPPER_NATIVE_ANIMATION
    a->weapon.program=source_clock?&marco_script:NULL;a->weapon.cursor=source_clock?(uint16_t)native_cursor(source_clock):0;
#else
    a->weapon.program=source_clock?&stub_program:NULL;
#endif
    a->weapon.timer=v[A_TIMER];a->weapon.speed=v[A_SPEED];a->flash=v[A_FLASH];a->field18=v[A_F18];a->field50_tag=v[A_FIELD50];
    a->fields=(marco_pose_fields_t){v[A_F72]>>16,v[A_F72]&65535,v[A_F76],v[A_F77],v[A_F78],v[A_F79]};
}
#ifdef MARCO_UPPER_NATIVE_ANIMATION
static void native_draw(void *ctx,const neo_anim_draw_t *d) {
    (void)ctx;assert(draw_index<native_operation->draw_count);
    const neo_anim_draw_t *e=&upper_draws[native_operation->draw_first+draw_index++];
    assert(d->descriptor==e->descriptor && d->x==e->x && d->y==e->y && d->attributes==e->attributes && d->z==e->z);native_draws++;
}
static bool native_event(void *ctx,neo_anim_kind_t kind,uint32_t asset,neo_anim_render_t *r) {
    (void)ctx;(void)r;assert(kind==NEO_ANIM_EVENT && event_index<native_operation->event_count);
    unsigned index=native_operation->event_first+event_index++;
    assert(asset<sizeof(upper_event_targets)/sizeof(*upper_event_targets));
    assert(upper_event_targets[asset]==upper_events[index].target && (!upper_events[index].site || upper_event_sites[asset]==upper_events[index].site));
    for(unsigned i=0;i<upper_events[index].spawn_count;i++) marco_upper_projectile_created(native_owner,(upper_events[index].spawn_mask&(1u<<i))!=0);
    if(upper_events[index].target==0xc4786) (void)marco_upper_consume_grenade(native_owner);
    native_events++;return true; // World service request observed; not a spawned child implementation.
}
#endif
static bool animate(void *ctx,marco_upper_t *s,marco_animation_t *a,bool *complete) {
    (void)ctx;const upper_op_t *op=take(3);uint32_t before[A_COUNT];anim_snapshot(a,before);
    if(memcmp(before,op->before,sizeof(before))) {
        printf("upper case %u: animation-service input differs\n",case_index);
        for(unsigned i=0;i<A_COUNT;i++) if(before[i]!=op->before[i]) printf(" anim field %u got=%u expected=%u\n",i,(unsigned)before[i],(unsigned)op->before[i]);
        assert(false);
    }
    /* These outcomes are world-service inputs. The counter/ammunition updates
     * are native code, not overwritten from expected controller after-fields. */
#ifdef MARCO_UPPER_NATIVE_ANIMATION
    const uint32_t *b=upper_cases[case_index].before;
    marco_animation_view_t view={.render={.x=b[U_X],.y=b[U_Y],.attributes=s->input.attributes,.z=b[U_Z],.correction=b[U_CORRECTION],.flash_palette=b[U_FLASH_PALETTE],
        .palettes={b[U_PALETTES]>>24,b[U_PALETTES]>>16,b[U_PALETTES]>>8,b[U_PALETTES]},.suppress=(s->input.flags5f&8u) && (b[U_PHASE]&1u)},.cached_x=b[U_CACHED_X],.cached_y=b[U_CACHED_Y]};
    native_operation=op;native_owner=s;draw_index=event_index=0;
    assert(marco_upper_animation_step(s,a,&view,native_draw,NULL,256,native_event,NULL,complete));
    assert(*complete==op->complete && draw_index==op->draw_count && event_index==op->event_count);
    source_clock=a->weapon.program?upper_source_commands[a->weapon.cursor]:0;s->input.attributes=view.render.attributes;
    uint32_t after[A_COUNT];anim_snapshot(a,after);assert(!memcmp(after,op->after,sizeof(after)));
#else
    for(unsigned i=0;i<op->spawn_count;i++) marco_upper_projectile_created(s,(op->spawn_mask&(1u<<i))!=0);
    for(unsigned i=0;i<op->grenades;i++) (void)marco_upper_consume_grenade(s);
    anim_seed(a,op->after);*complete=op->complete;
#endif
    return true;
}
static bool latch(void *ctx,uint16_t mask) {
    (void)ctx;const upper_op_t *op=take(4);assert(mask==op->mask && latch_word==op->word);latch_word|=mask;return true;
}
static void state_seed(marco_upper_t *s,marco_animation_t *a,const uint32_t *v) {
    memset(s,0,sizeof(*s));marco_animation_reset(a);
    s->mode=v[U_MODE];s->desired90=v[U_DESIRED];s->active91=v[U_ACTIVE];s->aim92=v[U_AIM];s->form93=v[U_FORM];s->combo95=v[U_COMBO];s->idle97=v[U_IDLE];s->action98=v[U_ACTION];
    s->input=(marco_input_state_t){v[U_ATTR],v[U_F5F],v[U_F9A],v[U_F9B],v[U_F9C]};
    s->packed=v[U_PACKED];s->gate=(marco_input_gate_t){v[U_GATE_MODE],v[U_GATE_CURRENT],v[U_GATE_LIMIT]};
    s->character=v[U_CHARACTER];s->weapon_kind=v[U_KIND];s->bombs=v[U_BOMBS];s->unlimited_bombs=v[U_UNLIMITED]!=0;latch_word=v[U_INPUT_LATCH];
    uint32_t av[]={v[U_CURSOR],v[U_TIMER],v[U_SPEED],v[U_FLASH],v[U_F18],v[U_FIELD50],v[U_F72],v[U_F76],v[U_F77],v[U_F78],v[U_F79]};anim_seed(a,av);
}
static void check(marco_upper_t *s,marco_animation_t *a,const uint32_t *e) {
    uint32_t actual[U_COUNT];memcpy(actual,upper_cases[case_index].before,sizeof(actual));
    // Video timestamps belong to the world, and may advance during the call.
    actual[U_FRAME]=e[U_FRAME];actual[U_PHASE]=e[U_PHASE];
    actual[U_MODE]=s->mode;actual[U_DESIRED]=s->desired90;actual[U_ACTIVE]=s->active91;actual[U_AIM]=s->aim92;actual[U_FORM]=s->form93;
    actual[U_COMBO]=s->combo95;actual[U_IDLE]=s->idle97;actual[U_ACTION]=s->action98;
    actual[U_ATTR]=s->input.attributes;actual[U_F5F]=s->input.flags5f;actual[U_F9A]=s->input.flags9a;actual[U_F9B]=s->input.accumulator9b;actual[U_F9C]=s->input.mask9c;
    actual[U_PACKED]=s->packed;actual[U_GATE_MODE]=s->gate.mode;actual[U_GATE_CURRENT]=s->gate.current;actual[U_GATE_LIMIT]=s->gate.limit;actual[U_UNLIMITED]=s->unlimited_bombs;
    actual[U_CHARACTER]=s->character;actual[U_KIND]=s->weapon_kind;actual[U_BOMBS]=s->bombs;actual[U_INPUT_LATCH]=latch_word;
    actual[U_CURSOR]=source_clock;actual[U_TIMER]=a->weapon.timer;actual[U_SPEED]=a->weapon.speed;actual[U_FLASH]=a->flash;actual[U_F18]=a->field18;actual[U_FIELD50]=a->field50_tag;
    actual[U_F72]=((uint32_t)a->fields.weapon_dx<<16)|a->fields.weapon_dy;actual[U_F76]=a->fields.field76;actual[U_F77]=a->fields.frame77;actual[U_F78]=a->fields.field78;actual[U_F79]=a->fields.variant79;
    for(unsigned i=0;i<U_COUNT;i++) if(actual[i]!=e[i]) {printf("upper case %u field %u got=%u expected=%u\n",case_index,i,(unsigned)actual[i],(unsigned)e[i]);assert(false);}
}
int main(void) {
#ifdef __mips__
    debug_init_emulog();console_init();
#endif
    marco_upper_services_t services={NULL,upper_compatibility,sizeof(upper_compatibility)/sizeof(*upper_compatibility),upper_combo_limits,sizeof(upper_combo_limits),select_table,available,random16,animate,latch};
    unsigned count=sizeof(upper_cases)/sizeof(*upper_cases),resets=0;
    for(case_index=0;case_index<count;case_index++) {
        marco_upper_t state;marco_animation_t actor;state_seed(&state,&actor,upper_cases[case_index].before);
        next_op=upper_cases[case_index].first;end_op=next_op+upper_cases[case_index].count;
        if(upper_cases[case_index].reset) {marco_upper_reset(&state);resets++;}
        else assert(marco_upper_update(&state,&actor,&services));
        assert(next_op==end_op);check(&state,&actor,upper_cases[case_index].after);
    }
    printf("PASS: upper calls=%u resets=%u services=%u\n",count,resets,total_ops);
#ifdef MARCO_UPPER_NATIVE_ANIMATION
    printf("PASS: upper real native animation draws=%u events=%u\n",native_draws,native_events);
#ifdef __mips__
    debugf("MSX UPPER NATIVE ANIMATION PASS draws=%u events=%u\n",native_draws,native_events);
#endif
#endif
#ifdef __mips__
    debugf("MSX UPPER REGRESSION PASS calls=%u resets=%u services=%u\n",count,resets,total_ops);for (;;) wait_ms(100);
#endif
}
