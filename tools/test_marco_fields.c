#include <assert.h>
#include <stdio.h>
#include "../src/neo_animation.h"
#include "marco_fields.h"
#include "marco_selectors.h"
#ifdef __mips__
#include <libdragon.h>
#endif
/* Native C members, not a byte-addressable imitation of the original RAM. */
typedef struct {uint32_t field72;uint8_t field76,field77,field78;} player_fields_t;
static bool access_field(void *context,unsigned field,unsigned bytes,bool write,uint32_t *value) {
    player_fields_t *p=context;
    if (field==0x72 && bytes==4) {
        if (write) p->field72=*value;else *value=p->field72;return true;
    }
    if ((field==0x72 || field==0x74) && bytes==2) {
        unsigned shift=field==0x72?16:0;
        if (write) p->field72=(p->field72&~(65535u<<shift))|((*value&65535u)<<shift);
        else *value=(p->field72>>shift)&65535u;
        return true;
    }
    uint8_t *member=field==0x76?&p->field76:field==0x77?&p->field77:field==0x78?&p->field78:NULL;
    if (!member || bytes!=1) return false;
    if (write) *member=(uint8_t)*value;else *value=*member;return true;
}
typedef struct {unsigned field,value,count;neo_anim_draw_t draw;} selector_t;
static bool selector_field(void *context,unsigned field,unsigned bytes,bool write,uint32_t *value) {
    selector_t *s=context;if (write || bytes!=1 || field!=s->field) return false;
    *value=s->value;return true;
}
static void selector_draw(void *context,const neo_anim_draw_t *draw) {
    selector_t *s=context;s->count++;s->draw=*draw;
}
static unsigned test_selectors(void) {
    unsigned count=sizeof(marco_select_cases)/sizeof(*marco_select_cases);
    for (unsigned i=0;i<count;i++) {
        selector_t output={.field=marco_select_cases[i].field,.value=marco_select_cases[i].value};
        neo_anim_state_t state={.program=&marco_select_cases[i].program,.timer=marco_select_cases[i].timer,.flash=marco_select_cases[i].flash};
        neo_anim_render_t render=marco_select_cases[i].render;
        assert(neo_anim_step_fields(&state,&render,selector_draw,&output,32,NULL,NULL,selector_field,&output)==NEO_ANIM_COMPLETE);
        assert(output.count==marco_select_cases[i].count);
        if (output.count) {
            const neo_anim_draw_t *expected=&marco_select_cases[i].draw;
            assert(output.draw.descriptor==expected->descriptor && output.draw.x==expected->x && output.draw.y==expected->y && output.draw.attributes==expected->attributes && output.draw.z==expected->z);
        }
    }
    return count;
}
int main(void) {
#ifdef __mips__
    debug_init_emulog();console_init();
#endif
    unsigned count=sizeof(marco_field_cases)/sizeof(*marco_field_cases);
    for (unsigned i=0;i<count;i++) {
        const uint32_t *before=marco_field_cases[i].before,*after=marco_field_cases[i].after;
        player_fields_t fields={before[0],before[1],before[2],before[3]};
        neo_anim_command_t commands[5]={{0}};
        commands[0]=(neo_anim_command_t){.kind=marco_field_cases[i].kind,.asset=marco_field_cases[i].value,.target=marco_field_cases[i].field};
        commands[1]=(neo_anim_command_t){.kind=NEO_ANIM_FIELD50,.asset=7};commands[2].kind=NEO_ANIM_FINISH;
        commands[3]=(neo_anim_command_t){.kind=NEO_ANIM_FIELD50,.asset=9};commands[4].kind=NEO_ANIM_FINISH;
        if (commands[0].kind==NEO_ANIM_BRANCH_FIELD) {
            commands[0].asset=marco_field_cases[i].field;commands[0].target=3;commands[0].operand=(uint8_t)marco_field_cases[i].value;
        }
        neo_anim_program_t program={commands,5};neo_anim_state_t state={0};neo_anim_render_t render={0};neo_anim_start(&state,&program,256);
        assert(neo_anim_step_fields(&state,&render,NULL,NULL,16,NULL,NULL,access_field,&fields)==NEO_ANIM_COMPLETE);
        assert(fields.field72==after[0] && fields.field76==after[1] && fields.field77==after[2] && fields.field78==after[3]);
        assert(state.field50_tag==(marco_field_cases[i].taken?9u:7u));
    }
    /* Boundary checks supplement the captured cases; these are synthetic. */
    neo_anim_command_t dec[]={{.kind=NEO_ANIM_DECREMENT_BYTE,.target=0x76},{.kind=NEO_ANIM_FINISH}};
    neo_anim_program_t program={dec,2};neo_anim_render_t render={0};neo_anim_state_t state={0};player_fields_t fields={0};
    neo_anim_start(&state,&program,256);
    assert(neo_anim_step_fields(&state,&render,NULL,NULL,8,NULL,NULL,access_field,&fields)==NEO_ANIM_COMPLETE && fields.field76==255);
    neo_anim_start(&state,&program,256);
    assert(neo_anim_step_events(&state,&render,NULL,NULL,8,NULL,NULL)==NEO_ANIM_UNSUPPORTED);
    dec[0].target=0xffff;neo_anim_start(&state,&program,256);
    assert(neo_anim_step_fields(&state,&render,NULL,NULL,8,NULL,NULL,access_field,&fields)==NEO_ANIM_UNSUPPORTED);
    unsigned selectors=test_selectors();
    printf("PASS: %u original Marco field/branch operations and %u inline pose selections\n",count,selectors);
#ifdef __mips__
    debugf("MSX MARCO REGRESSION PASS fields=%u selectors=%u\n",count,selectors);for (;;) wait_ms(100);
#endif
}
