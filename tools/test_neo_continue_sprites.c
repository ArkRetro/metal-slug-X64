/* The mission actor's CONTINUE? sprites (src/neo_continue.c, program 018120
 * converted by tools/extract_soldier_programs.py) against the sprite-list
 * entries the original wrote in the go2 capture (tools/mame/
 * capture_gameover.lua MSX_GO_TRACE=1 -> tools/extract_continue_sprites.py
 * -> tools/compile_continue_sprites.py). The program starts at the mission
 * start and is stepped once per update that drew in MAME, with the counter
 * +7E and the front-layer bit of that update; every entry must match:
 * x, y, zoom word (+12/+13), attributes (palette slot, flags) and
 * descriptor, in order. */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#define MSX_SOLDIER_PROGRAMS 1
#include "../src/neo_continue.h"
#include "../src/neo_soldier_ai.h"
#include "soldier_ai_tables.h"
typedef struct {uint16_t x,y,zoom,attributes;uint32_t descriptor;} entry_t;
typedef struct {uint32_t frame;uint16_t counter;int front;unsigned count;entry_t e[12];} continue_update_t;
#include "continue_sprites_fixture.h"
static const neo_anim_program_t *program(uint32_t tag) {
    for (unsigned i=0;i<sizeof(soldier_programs)/sizeof(soldier_programs[0]);i++) if (soldier_programs[i].tag==tag) return &soldier_programs[i].program;
    return NULL;
}
int main(void) {
    static neo_continue_t c;neo_continue_start(&c,program);
    assert(!c.unsupported);
    unsigned n=sizeof(continue_updates)/sizeof(continue_updates[0]),entries=0,digits=0;uint16_t shown=0xffff;
    for (unsigned i=0;i<n;i++) {
        const continue_update_t *u=&continue_updates[i];
        neo_continue_draw(&c,u->counter,u->front!=0,program);
        if (c.unsupported) {fprintf(stderr,"frame %u: explicit stop at %06x\n",(unsigned)u->frame,(unsigned)c.unsupported_at);return 1;}
        if (c.draw_count!=u->count) {fprintf(stderr,"frame %u: %u draws, MAME %u entries\n",(unsigned)u->frame,c.draw_count,u->count);return 1;}
        for (unsigned k=0;k<u->count;k++) {
            const neo_end_draw_t *d=&c.draws[k];const entry_t *m=&u->e[k];
            uint16_t zoom=(uint16_t)(d->zoom_x<<8|d->zoom_y);
            if (d->x!=m->x || d->y!=m->y || zoom!=m->zoom || d->attributes!=m->attributes || d->descriptor!=m->descriptor) {
                fprintf(stderr,"frame %u entry %u: native %x,%x zoom %04x attr %04x %06x, MAME %x,%x zoom %04x attr %04x %06x\n",(unsigned)u->frame,k,
                    d->x,d->y,zoom,d->attributes,(unsigned)d->descriptor,m->x,m->y,m->zoom,m->attributes,(unsigned)m->descriptor);
                return 1;
            }
            entries++;
        }
        if ((u->counter>>8)!=shown) {shown=(uint16_t)(u->counter>>8);digits++;}
    }
    printf("PASS: CONTINUE? sprites 018120: %u updates, %u sprite-list entries equal to MAME (x, y, zoom, attributes, descriptor), %u digits shown\n",n,entries,digits);
    return 0;
}
