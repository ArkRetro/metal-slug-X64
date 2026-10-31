/* The mission actor's CONTINUE? sprites, manual translation of the program
 * 018120 callbacks 012022 and 0180F6 (labels are provenance only); the
 * program itself is converted by tools/extract_soldier_programs.py:
 *   018120  012022 (+15 bit 2: front layer), 0180F6 (zoom +12 from the
 *           counter's low byte), +14 = FE, +3C/+40 = A0/170, the digit pose
 *           selected by +7E (the counter's high byte, 1D913A..1D9120),
 *           +12 = FF, +14 = 04, the nine letters of CONTINUE?, WAIT 1, back
 *           to 018120.
 * The palette slots it sets directly are the resources the allocator table
 * holds for them in every snapshot of the go2 capture: slot 04 = 0x63,
 * slot FE = 0x25F. */
#include "neo_continue.h"
#include <string.h>
static const uint8_t *g_rom(uint32_t address) {return ga_rom(address);}
typedef struct {neo_continue_t *c;bool front;} ctx_t;
static void emit(void *context,const neo_anim_draw_t *d) {
    ctx_t *t=context;neo_continue_t *c=t->c;
    if (c->draw_count==NEO_CONTINUE_DRAWS) {c->unsupported++;return;}
    uint8_t slot=(uint8_t)(d->attributes>>8);uint16_t resource=0;
    if (slot==0x04) resource=0x63; else if (slot==0xfe) resource=0x25f;
    else {if (!c->unsupported++) c->unsupported_at=0x18120;return;}
    const neo_gactor_t *a=&c->node;
    c->draws[c->draw_count++]=(neo_end_draw_t){d->descriptor,d->x,d->y,d->attributes,resource,ga_b(a,0x12),ga_b(a,0x13)};
}
static bool front(void *context) {return ((ctx_t *)context)->front;}
/* 0180F6: +12 = sine(2 * low byte of +7E, the doubling saturated at 0x80) - 1, at least 0. */
static void zoom(neo_gactor_t *a) {
    uint8_t lo=(uint8_t)ga_w(a,0x7e);int8_t s=(int8_t)lo;uint8_t d1;
    int16_t twice=(int16_t)(s*2);
    if (twice>127 || twice<-128) d1=0x80; else d1=(uint8_t)twice;
    const uint8_t *t=g_rom(0x29fcu+(d1&0xfeu));
    uint16_t w=t?(uint16_t)(t[0]<<8|t[1]):0;
    w=w?(uint16_t)(w-1):0;
    ga_sb(a,0x12,(uint8_t)w);
}
static bool call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    if (routine==0x12022) {
        if (e->front_layer(e->ctx)) ga_sb(a,0x15,(uint8_t)(ga_b(a,0x15)|4u)); else ga_sb(a,0x15,(uint8_t)(ga_b(a,0x15)&0xfbu));
        return true;
    }
    if (routine==0x180f6) {zoom(a);return true;}
    return false;
}
static void env(neo_genv_t *e,ctx_t *t,neo_ga_flags_t *flags,const neo_anim_program_t *(*program)(uint32_t)) {
    memset(e,0,sizeof(*e));memset(flags,0,sizeof(*flags));
    e->ctx=t;e->emit=emit;e->emit_ctx=t;e->program=program;e->front_layer=front;e->flags=flags;
}
void neo_continue_start(neo_continue_t *c,const neo_anim_program_t *(*program)(uint32_t tag)) {
    memset(c,0,sizeof(*c));
    neo_gactor_t *a=&c->node;a->active=true;
    ga_sb(a,0x12,0xff);ga_sb(a,0x13,0xff);ga_sw(a,0x1e,0xffff);
    ctx_t t={c,false};neo_genv_t e;neo_ga_flags_t flags;env(&e,&t,&flags,program);
    if (!ga_anim_start(a,&e,0x18120,0x100)) {c->unsupported++;c->unsupported_at=0x18120;}
}
void neo_continue_draw(neo_continue_t *c,uint16_t counter,bool front_layer,const neo_anim_program_t *(*program)(uint32_t tag)) {
    neo_gactor_t *a=&c->node;
    c->draw_count=0;
    if (c->unsupported) return;
    ga_sw(a,0x7e,counter);
    ctx_t t={c,front_layer};neo_genv_t e;neo_ga_flags_t flags;env(&e,&t,&flags,program);
    if (ga_anim_step(a,&e,call)<0) {c->unsupported++;c->unsupported_at=e.unsupported_at?e.unsupported_at:0x427a;}
    c->steps++;
}
