/* The mummy transformation's actors (src/neo_landing.c: the smoke 0C4DD8
 * that 0C17A0 creates after the player, then the marker 0C5286 of 0C182A)
 * against the MAME family trace mz4 (tools/capture_fullgame.lua with
 * MSX_FULL_GIVE_MUMMY, tools/extract_landing_fixture.py). The smoke starts
 * from its recorded bytes before its constructor (0040BC's node), the marker
 * when the smoke has set the player's +60 (smoke_done) and the
 * transformation asked for it (neo_landing_marker). Per update the player
 * fields it read are the input; after every update the routine and bytes
 * +10..+AF must match (except the palette slot numbers +14/+16/+19..+1C, the
 * animation pointer +20..+23 and the cached position +44..+4B; +1D after a
 * deletion through 004126). The smoke must end in its 17th update with
 * smoke_done set (0C17F0 waits for it: 19 updates of transformation). */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#define MSX_SOLDIER_PROGRAMS 1
#include "../src/neo_landing.h"
#include "../src/neo_soldier_ai.h"
#include "soldier_ai_tables.h"
typedef struct {uint32_t frame;int kind;uint32_t routine;uint8_t pre[0xb0];int has_post;uint8_t post[0xb0];int ram[26];} landing_update_t;
#include "smoke_fixture.h"
static const neo_anim_program_t *program(uint32_t tag) {
    for (unsigned i=0;i<sizeof(soldier_programs)/sizeof(soldier_programs[0]);i++) if (soldier_programs[i].tag==tag) return &soldier_programs[i].program;
    return NULL;
}
enum { NU=sizeof(landing_updates)/sizeof(landing_updates[0]), MARKER=0, SMOKE=2 };
static neo_landing_t s;static const landing_update_t *expect[3];static unsigned bytes,updates,deletes,failures;
static const char *name(int k) {return k==SMOKE?"smoke":"marker";}
static bool skip(unsigned o) {return o==0x14 || o==0x16 || (o>=0x19 && o<=0x1c) || (o>=0x20 && o<=0x23) || (o>=0x44 && o<=0x4b);}
static void on_updated(void *ctx,neo_gactor_t *a) {
    (void)ctx;int k=a==&s.smoke?SMOKE:a==&s.marker?MARKER:1;const landing_update_t *u=k==1?NULL:expect[k];
    if (!u) {fprintf(stderr,"%s updated natively, not in MAME\n",k==1?"canopy":name(k));failures++;return;}
    expect[k]=NULL;updates++;
    uint32_t r=ga_l(a,0);
    if (!u->has_post) {if (r!=0x3f1e) {fprintf(stderr,"frame %u: should delete\n",(unsigned)u->frame);failures++;} deletes++;return;}
    uint32_t want=(uint32_t)u->post[0]<<24|u->post[1]<<16|u->post[2]<<8|u->post[3];
    if (r!=want) {fprintf(stderr,"frame %u %s: routine %06x MAME %06x\n",(unsigned)u->frame,name(k),(unsigned)r,(unsigned)want);failures++;}
    if (want==0x3f1e) deletes++;
    for (unsigned o=0x10;o<0xb0;o++) {
        if (skip(o) || (want==0x3f1e && o==0x1d)) continue;
        bytes++;
        if (a->m[o]!=u->post[o]) {if (failures<10) fprintf(stderr,"frame %u %s routine %06x: +%02X native %02x MAME %02x\n",(unsigned)u->frame,name(k),(unsigned)u->routine,o,a->m[o],u->post[o]);failures++;}
    }
}
static int field(const landing_update_t *u,unsigned off) {
    for (unsigned i=0;i<sizeof(landing_fields)/sizeof(landing_fields[0]);i++) if (landing_fields[i]==off) return u->ram[i];
    return -1;
}
static uint32_t lng(const landing_update_t *u,unsigned off,uint32_t keep) {
    for (unsigned i=0;i<4;i++) {int b=field(u,off+i);if (b>=0) keep=(keep&~(0xffu<<(24-8*i)))|(uint32_t)b<<(24-8*i);}
    return keep;
}
/* A node created this pass: the recorded bytes before its constructor. */
static void seed(neo_gactor_t *a,const landing_update_t *u) {
    uint8_t keep[0x20];memcpy(keep,a->m,0x20);
    memcpy(a->m,u->pre,0xb0);
    a->m[0x14]=keep[0x14];memcpy(&a->m[0x19],&keep[0x19],4);a->m[0x1d]=keep[0x1d];
}
int main(void) {
    neo_landing_input_t in;memset(&in,0,sizeof(in));
    memset(&s,0,sizeof(s));s.on_updated=on_updated;
    unsigned draws=0,passes=0,smoke_updates=0,done_at=0,smoke_draws=0;
    for (unsigned i=0;i<NU;) {
        uint32_t f=landing_updates[i].frame;unsigned j=i;expect[0]=expect[1]=expect[2]=NULL;
        for (;j<NU && landing_updates[j].frame==f;j++) {
            const landing_update_t *u=&landing_updates[j];expect[u->kind]=u;
            int v;
            if ((v=field(u,0x15))>=0) in.flags15=(uint8_t)v;
            if ((v=field(u,0x1e))>=0 && field(u,0x1f)>=0) in.z=(uint16_t)(v<<8|field(u,0x1f));
            if ((v=field(u,0x70))>=0 && field(u,0x71)>=0) in.player=(uint16_t)(v<<8|field(u,0x71));
            if ((v=field(u,0x4d))>=0) in.correction=(uint8_t)v;
            if (u->ram[23]>=0) in.epoch=(uint8_t)u->ram[23];
            if (u->ram[24]>=0 && u->ram[25]>=0) in.front_layer=(int16_t)u->ram[24]<(int16_t)u->ram[25];   /* 012022 */
            in.x=(int32_t)lng(u,0x3c,(uint32_t)in.x);in.y=(int32_t)lng(u,0x40,(uint32_t)in.y);
            in.previous_x=(int32_t)lng(u,0x44,(uint32_t)in.previous_x);in.previous_y=(int32_t)lng(u,0x48,(uint32_t)in.previous_y);
            if (u->routine==0xc4dd8) {neo_landing_smoke(&s);seed(&s.smoke,u);}
            if (u->routine==0xc5286) {
                if (!s.smoke_done) {fprintf(stderr,"frame %u: marker before the smoke's end\n",(unsigned)f);return 1;}
                s.smoke_done=false;neo_landing_marker(&s);seed(&s.marker,u);
            }
        }
        unsigned before=updates;bool smoke=s.smoke.active;
        neo_landing_update(&s,&in,program);passes++;draws+=s.draw_count;
        if (smoke) {smoke_updates++;smoke_draws+=s.marker_draws;if (s.smoke_done && !done_at) done_at=smoke_updates;}
        if (s.unsupported) {fprintf(stderr,"frame %u: explicit stop at %06x\n",(unsigned)f,(unsigned)s.unsupported_at);return 1;}
        if (failures) return 1;
        if (updates-before!=j-i) {fprintf(stderr,"frame %u: %u native updates, MAME %u\n",(unsigned)f,updates-before,j-i);return 1;}
        i=j;
    }
    if (done_at!=17) {fprintf(stderr,"smoke_done after %u updates (0C17F0: 17)\n",done_at);return 1;}
    if (!smoke_draws) {fprintf(stderr,"the smoke drew nothing\n");return 1;}
    printf("PASS: mummy transformation (smoke 0C4DD8, marker 0C5286): %u passes, %u updates, %u bytes equal, %u deletes, smoke done in update %u, %u draws (%u over the player during the smoke)\n",
        passes,updates,bytes,deletes,done_at,draws,smoke_draws);
    return 0;
}
