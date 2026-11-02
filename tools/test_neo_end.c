/* Closed-loop test of the mission-end actors (src/neo_end.c) against a MAME
 * family trace (tools/extract_end_fixture.py -> tools/compile_end_fixture.py,
 * END_HEADER / END_NAME).
 *
 * The root starts from its recorded bytes when the mission actor creates it;
 * every other node exists only when the native code creates it and is bound
 * to the recorded node of the same creation order, starting from that node's
 * first recorded bytes (allocator fields), with the native creator's writes
 * on top. Inputs per update are the RAM words the original read (epoch, scene
 * root, layer words, mission). After every update the routine and the bytes
 * +10..+AF must match, except the allocator's palette slot numbers (+14, +16,
 * +19..+1C: native handles), the animation pointer +20..+23 and the cached
 * position +44..+4B; the creator +0C is compared through the node mapping.
 * A deleting update must delete. */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#define MSX_SOLDIER_PROGRAMS 1
#include "../src/neo_end.h"
#include "../src/neo_soldier_ai.h"
#include "soldier_ai_tables.h"
typedef struct {
    uint32_t frame;int node;uint32_t routine;uint8_t pre[0xb0];int has_post;uint8_t post[0xb0];
    int epoch,front_a,front_b,mission;int root[8];int correction;
} end_update_t;
#include END_HEADER
#define CAT2(a,b) a##b
#define CAT(a,b) CAT2(a,b)
#define UPDATES CAT(END_NAME,_updates)
#define BIRTHS CAT(END_NAME,_births)
enum { NU=sizeof(UPDATES)/sizeof(UPDATES[0]), NB=sizeof(BIRTHS)/sizeof(BIRTHS[0]) };
static const neo_anim_program_t *program(uint32_t tag) {
    for (unsigned i=0;i<sizeof(soldier_programs)/sizeof(soldier_programs[0]);i++) if (soldier_programs[i].tag==tag) return &soldier_programs[i].program;
    return NULL;
}
static int native_of[NB+1];   /* fixture id -> pool index */
static int id_of[NEO_END_NODES];
static const uint8_t *first_pre(int id) {
    for (unsigned i=0;i<NU;i++) if (UPDATES[i].node==id) return UPDATES[i].pre;
    return NULL;
}
static void on_alloc(void *ctx,neo_gactor_t *a,unsigned spawn) {
    (void)ctx;
    int id=spawn==0?0:(spawn-1<NB?BIRTHS[spawn-1]:-1);
    assert(id>=0);
    const uint8_t *p=first_pre(id);assert(p);
    /* The allocator's fields (and stale bytes) of the recorded node; the
     * native routine label and creator tag stay. */
    uint8_t keep[0x10];memcpy(keep,a->m,0x10);
    memcpy(a->m,p,0xb0);memcpy(a->m,keep,0x10);
    native_of[id]=a->index;id_of[a->index]=id;
}
static bool skip(unsigned o) {
    return o==0x14 || o==0x16 || (o>=0x19 && o<=0x1c) || (o>=0x20 && o<=0x23) || (o>=0x44 && o<=0x4b);
}
static neo_end_t s;
static unsigned cur_lo,cur_hi,updates,bytes,deletes,failures;
static void on_updated(void *ctx,neo_gactor_t *a) {
    (void)ctx;
    if (failures) return;
    int id=id_of[a->index];const end_update_t *u=NULL;
    for (unsigned i=cur_lo;i<cur_hi;i++) if (UPDATES[i].node==id) {u=&UPDATES[i];break;}
    if (!u) {fprintf(stderr,"frame %u: node %d updated natively, not in MAME\n",(unsigned)UPDATES[cur_lo].frame,id);failures++;return;}
    uint32_t f=u->frame,routine=ga_l(a,0);
    if (!u->has_post) {
        if (routine!=0x3f1e) {fprintf(stderr,"frame %u node %d: should delete (routine %06x)\n",(unsigned)f,id,(unsigned)routine);failures++;}
        deletes++;updates++;return;
    }
    uint32_t want=(uint32_t)u->post[0]<<24|u->post[1]<<16|u->post[2]<<8|u->post[3];
    unsigned bad=0;
    if (routine!=want) {fprintf(stderr,"frame %u node %d: routine %06x, MAME %06x\n",(unsigned)f,id,(unsigned)routine,(unsigned)want);bad++;}
    uint32_t wc=(uint32_t)u->post[0xc]<<24|u->post[0xd]<<16|u->post[0xe]<<8|u->post[0xf];
    int cid=s.creator[a->index]<0?-1:id_of[(int)s.creator[a->index]];
    if ((cid<0)!=(wc==0x101364u)) {fprintf(stderr,"frame %u node %d: creator %d, MAME %06x\n",(unsigned)f,id,cid,(unsigned)wc);bad++;}
    for (unsigned o=0x10;o<0xb0;o++) {
        if (skip(o)) continue;
        bytes++;
        if (a->m[o]!=u->post[o]) {
            if (bad<8) fprintf(stderr,"frame %u node %d routine %06x: +%02X native %02x MAME %02x\n",(unsigned)f,id,(unsigned)u->routine,o,a->m[o],u->post[o]);
            bad++;
        }
    }
    if (bad) failures++;
    updates++;
}
int main(void) {
    neo_end_reset(&s);s.on_alloc=on_alloc;s.on_updated=on_updated;
    for (unsigned i=0;i<=NB;i++) native_of[i]=-1;
    neo_end_inputs_t in;memset(&in,0,sizeof(in));
    in.bounds=(neo_cull_bounds_t){false,0,0,0,0,0,256,320,512};in.program=program;
    /* MISSION n START: 060878 requests the voice 607D2[mission] (0120F8)
     * in the letters' first update, once (1131 + RAM 10C2F4; MAME
     * sec01_19391: 1132 at 19428, return 060888). */
    static neo_end_ram_t ram;unsigned voices=0;uint16_t voice=0;
    if (UPDATES[0].routine==0x607de) in.ram=&ram;
    unsigned draws=0,frames=0;bool started=false;
    for (unsigned i=0;i<NU;) {
        uint32_t f=UPDATES[i].frame;unsigned j=i;
        for (;j<NU && UPDATES[j].frame==f;j++) {
            const end_update_t *u=&UPDATES[j];
            if (u->epoch>=0) in.epoch=(uint8_t)u->epoch;
            if (u->front_a>=0 && u->front_b>=0) in.front_layer=(int16_t)u->front_a<(int16_t)u->front_b;
            if (u->mission>=0) in.mission=(uint16_t)u->mission;
            if (u->root[0]>=0) {
                in.root.tag=0x10e968u;
                in.root.x=(int32_t)((uint32_t)u->root[0]<<16|(uint32_t)u->root[1]);in.root.y=(int32_t)((uint32_t)u->root[2]<<16|(uint32_t)u->root[3]);
                in.root.previous_x=(int32_t)((uint32_t)u->root[4]<<16|(uint32_t)u->root[5]);in.root.previous_y=(int32_t)((uint32_t)u->root[6]<<16|(uint32_t)u->root[7]);
                if (u->correction>=0) in.root.correction=(uint8_t)u->correction;
            }
        }
        if (!started) {assert(UPDATES[i].node==0 && (UPDATES[i].routine==0x6090a || UPDATES[i].routine==0x607de));neo_end_start(&s,UPDATES[i].routine);started=true;}
        cur_lo=i;cur_hi=j;
        unsigned before=updates;
        neo_end_update(&s,&in);frames++;
        if (in.ram) {
            if (ram.sound_count) {assert(frames==1 && ram.sound_count==1);voice=ram.sounds[0];voices++;assert(voice==0x1131u+in.mission);}
            ram.sound_count=0;
        }
        draws+=s.draw_count;
        if (s.unsupported) {fprintf(stderr,"frame %u: explicit stop at %06x\n",(unsigned)f,(unsigned)s.unsupported_at);return 1;}
        if (failures) return 1;
        if (updates-before!=j-i) {fprintf(stderr,"frame %u: %u native updates, MAME %u\n",(unsigned)f,updates-before,j-i);return 1;}
        i=j;
    }
    assert(s.mission60==1);
    if (in.ram) {assert(voices==1);printf("MISSION START voice %04X requested once, in the first update\n",voice);}
    printf("%s letters (root %06x): %u frames, %u updates, %u bytes equal, %u deletes, %u draws; mission actor +60 set\n",UPDATES[0].routine==0x607de?"MISSION START":"MISSION COMPLETE",(unsigned)UPDATES[0].routine,frames,updates,bytes,deletes,draws);
    return 0;
}
