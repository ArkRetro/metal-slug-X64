/* The parachute landing actors (src/neo_landing.c: canopy 0C4A58 collapse
 * 0C4B62, player marker 0C5286) against the MAME family trace land2
 * (tools/extract_landing_fixture.py). Each node starts from its recorded
 * bytes at the landing (the canopy still in 0C4AAE, the marker before its
 * constructor); per update the player fields it read are the input; after
 * every update the routine and bytes +10..+AF must match (except the
 * palette slot numbers +14/+16/+19..+1C, the animation pointer +20..+23 and
 * the cached position +44..+4B; +1D after a deletion through 004126). */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#define MSX_SOLDIER_PROGRAMS 1
#include "../src/neo_landing.h"
#include "../src/neo_soldier_ai.h"
#include "soldier_ai_tables.h"
typedef struct {uint32_t frame;int kind;uint32_t routine;uint8_t pre[0xb0];int has_post;uint8_t post[0xb0];int ram[26];} landing_update_t;
#include "landing_fixture.h"
static const neo_anim_program_t *program(uint32_t tag) {
    for (unsigned i=0;i<sizeof(soldier_programs)/sizeof(soldier_programs[0]);i++) if (soldier_programs[i].tag==tag) return &soldier_programs[i].program;
    return NULL;
}
enum { NU=sizeof(landing_updates)/sizeof(landing_updates[0]) };
static neo_landing_t s;static const landing_update_t *expect[2];static unsigned bytes,updates,deletes,failures;
static bool skip(unsigned o) {return o==0x14 || o==0x16 || (o>=0x19 && o<=0x1c) || (o>=0x20 && o<=0x23) || (o>=0x44 && o<=0x4b);}
static void on_updated(void *ctx,neo_gactor_t *a) {
    (void)ctx;int k=a==&s.canopy;const landing_update_t *u=expect[k];
    if (!u) {fprintf(stderr,"%s updated natively, not in MAME\n",k?"canopy":"marker");failures++;return;}
    expect[k]=NULL;updates++;
    uint32_t r=ga_l(a,0);
    if (!u->has_post) {if (r!=0x3f1e) {fprintf(stderr,"frame %u: should delete\n",(unsigned)u->frame);failures++;} deletes++;return;}
    uint32_t want=(uint32_t)u->post[0]<<24|u->post[1]<<16|u->post[2]<<8|u->post[3];
    if (r!=want) {fprintf(stderr,"frame %u %s: routine %06x MAME %06x\n",(unsigned)u->frame,k?"canopy":"marker",(unsigned)r,(unsigned)want);failures++;}
    if (want==0x3f1e) deletes++;
    for (unsigned o=0x10;o<0xb0;o++) {
        if (skip(o) || (want==0x3f1e && o==0x1d)) continue;
        bytes++;
        if (a->m[o]!=u->post[o]) {if (failures<10) fprintf(stderr,"frame %u %s routine %06x: +%02X native %02x MAME %02x\n",(unsigned)u->frame,k?"canopy":"marker",(unsigned)u->routine,o,a->m[o],u->post[o]);failures++;}
    }
}
static int field(const landing_update_t *u,unsigned off) {
    for (unsigned i=0;i<sizeof(landing_fields)/sizeof(landing_fields[0]);i++) if (landing_fields[i]==off) return u->ram[i];
    return -1;
}
/* The bytes of a long the update read (the marker reads only the word). */
static uint32_t lng(const landing_update_t *u,unsigned off,uint32_t keep) {
    for (unsigned i=0;i<4;i++) {int b=field(u,off+i);if (b>=0) keep=(keep&~(0xffu<<(24-8*i)))|(uint32_t)b<<(24-8*i);}
    return keep;
}
int main(void) {
    neo_landing_input_t in;memset(&in,0,sizeof(in));
    const landing_update_t *first[2]={NULL,NULL};
    for (unsigned i=0;i<NU;i++) if (!first[landing_updates[i].kind]) first[landing_updates[i].kind]=&landing_updates[i];
    assert(first[0] && first[1]);
    neo_landing_start(&s,&in,program);
    for (unsigned k=0;k<2;k++) {
        neo_gactor_t *a=k?&s.canopy:&s.marker;
        uint8_t keep[0x20];memcpy(keep,a->m,0x20);
        memcpy(a->m,first[k]->pre,0xb0);
        a->m[0x14]=keep[0x14];memcpy(&a->m[0x19],&keep[0x19],4);a->m[0x1d]=keep[0x1d];
    }
    s.on_updated=on_updated;
    unsigned draws=0,passes=0;
    for (unsigned i=0;i<NU;) {
        uint32_t f=landing_updates[i].frame;unsigned j=i;expect[0]=expect[1]=NULL;
        for (;j<NU && landing_updates[j].frame==f;j++) {
            const landing_update_t *u=&landing_updates[j];expect[u->kind]=u;
            int v;
            if ((v=field(u,0x15))>=0) in.flags15=(uint8_t)v;
            if ((v=field(u,0x1e))>=0 && field(u,0x1f)>=0) in.z=(uint16_t)(v<<8|field(u,0x1f));
            if ((v=field(u,0x70))>=0 && field(u,0x71)>=0) in.player=(uint16_t)(v<<8|field(u,0x71));
            if ((v=field(u,0x4d))>=0) in.correction=(uint8_t)v;
            if (u->ram[23]>=0) in.epoch=(uint8_t)u->ram[23];
            in.x=(int32_t)lng(u,0x3c,(uint32_t)in.x);in.y=(int32_t)lng(u,0x40,(uint32_t)in.y);
            in.previous_x=(int32_t)lng(u,0x44,(uint32_t)in.previous_x);in.previous_y=(int32_t)lng(u,0x48,(uint32_t)in.previous_y);
        }
        unsigned before=updates;
        neo_landing_update(&s,&in,program);passes++;draws+=s.draw_count;
        if (s.unsupported) {fprintf(stderr,"frame %u: explicit stop at %06x\n",(unsigned)f,(unsigned)s.unsupported_at);return 1;}
        if (failures) return 1;
        if (updates-before!=j-i) {fprintf(stderr,"frame %u: %u native updates, MAME %u\n",(unsigned)f,updates-before,j-i);return 1;}
        i=j;
    }
    printf("PASS: parachute landing (canopy 0C4B62, marker 0C5286): %u passes, %u updates, %u bytes equal, %u deletes, %u draws\n",passes,updates,bytes,deletes,draws);
    return 0;
}
