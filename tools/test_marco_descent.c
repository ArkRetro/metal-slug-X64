/* The parachute descent (src/marco_descent.c: player 0C162A/0C16D8/0C170C;
 * src/neo_landing.c: canopy 0C4A58/0C4AAE/0C4B20) against the MAME family
 * traces ch0 (no input), chA (A high, C, A low: cut) and chB (B: cut),
 * tools/extract_descent_fixture.py. The player's fields of the descent
 * (+00, +30, +34, +50, +5F, +61, +78, +90, +9A) run in closed loop
 * from the first update; the rest of the node (the sweep 0143EE, the common
 * processing, which also takes the press accumulator +9B) is taken from
 * MAME before each update. On a landing the stance (0C1F4A/0C1EBE) owns the
 * routine, the velocities and +90/+9A. The canopy runs in
 * closed loop from its constructor with the player and root words it read. */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#define MSX_SOLDIER_PROGRAMS 1
#include "../src/marco_descent.h"
#include "../src/marco_controller.h"
#include "../src/neo_landing.h"
#include "../src/neo_soldier_ai.h"
#include "soldier_ai_tables.h"
typedef struct {int capture;uint32_t frame;int canopy;uint32_t routine;uint8_t pre[0xb0];int has_post;uint8_t post[0xb0];
    int random,packed_hi,packed_lo,gate[3],ram[44];} descent_update_t;
#include "descent_fixture.h"
enum { NU=sizeof(descent_updates)/sizeof(descent_updates[0]),NP=sizeof(descent_pfields)/sizeof(descent_pfields[0]),NR=sizeof(descent_rfields)/sizeof(descent_rfields[0]) };
static const neo_anim_program_t *program(uint32_t tag) {
    for (unsigned i=0;i<sizeof(soldier_programs)/sizeof(soldier_programs[0]);i++) if (soldier_programs[i].tag==tag) return &soldier_programs[i].program;
    return NULL;
}
static const unsigned owned[]={0,1,2,3,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x50,0x51,0x52,0x53,0x5f,0x61,0x78,0x90,0x9a};
static bool is_owned(unsigned o) {for (unsigned i=0;i<sizeof(owned)/sizeof(owned[0]);i++) if (owned[i]==o) return true;return false;}
static unsigned failures,bytes,updates,cuts,landings;
static void compare(const descent_update_t *u,const uint8_t *m,const char *what,bool skip_velocity) {
    for (unsigned i=0;i<sizeof(owned)/sizeof(owned[0]);i++) {
        unsigned o=owned[i];
        if (skip_velocity && ((o>=0x30 && o<0x38) || o==0x90 || o==0x9a)) continue;
        if (o<4 && ((uint32_t)u->post[0]<<24|u->post[1]<<16|u->post[2]<<8|u->post[3])!=0xc170cu) continue;   /* 0C1F4A/0C1EBE: the stance's */
        bytes++;
        if (m[o]!=u->post[o]) {if (failures++<10) fprintf(stderr,"%s frame %u %s routine %06x: +%02X native %02x MAME %02x\n",descent_captures[u->capture],(unsigned)u->frame,what,(unsigned)u->routine,o,m[o],u->post[o]);}
    }
}
static void player(void) {
    uint8_t m[0xb0];int capture=-1;
    for (unsigned i=0;i<NU;i++) {
        const descent_update_t *u=&descent_updates[i];
        if (u->canopy) continue;
        if (u->capture!=capture) {capture=u->capture;memcpy(m,u->pre,0xb0);}
        for (unsigned o=0;o<0xb0;o++) if (!is_owned(o)) m[o]=u->pre[o];
        /* 0C0FEA: the input of the selected buffer through the filter. */
        marco_input_state_t s={(uint16_t)(m[0x14]<<8|m[0x15]),m[0x5f],m[0x9a],m[0x9b],m[0x9c]};
        marco_input_gate_t gate={(uint8_t)u->gate[0],(uint8_t)u->gate[1],(uint8_t)u->gate[2]};
        if (u->routine==MARCO_DESCENT_HANG) {
            uint32_t d0=marco_input_filter(&s,(uint32_t)u->packed_hi<<16|(uint32_t)u->packed_lo,gate);
            m[0x5f]=s.flags5f;m[0x9a]=s.flags9a;m[0x9b]=s.accumulator9b;
            if (marco_descent_hang(m,(uint16_t)u->random,d0)==MARCO_DESCENT_CUT) {cuts++;compare(u,m,"cut",u->post[0x15]&0x40u);}
            else {
                m[0x15]=u->post[0x15];
                bool landed=marco_descent_hang_after(m);landings+=landed;
                if (landed!=((u->post[0x15]&0x40u)!=0)) {fprintf(stderr,"frame %u: landing\n",(unsigned)u->frame);failures++;}
                compare(u,m,"hang",landed);
            }
        } else {
            marco_descent_fall(m);m[0x15]=u->post[0x15];
            bool down=marco_descent_fall_after(m);
            compare(u,m,"fall",down || (u->post[0x34]&0x80u)==0);   /* 0C109C's sweep keeps or zeroes +34 */
            landings+=down;
        }
        updates++;
    }
}
static neo_landing_t L;static const descent_update_t *expect;static unsigned cbytes,cupdates,cdeletes,cdraws;
static bool skip(unsigned o) {return o==0x14 || o==0x16 || (o>=0x19 && o<=0x1c) || (o>=0x20 && o<=0x23) || (o>=0x44 && o<=0x4b);}
static void on_updated(void *ctx,neo_gactor_t *a) {
    (void)ctx;
    if (a!=&L.canopy) return;
    const descent_update_t *u=expect;expect=NULL;
    if (!u) {fprintf(stderr,"canopy updated natively, not in MAME\n");failures++;return;}
    cupdates++;uint32_t r=ga_l(a,0);
    if (!u->has_post) {if (r!=0x3f1e) {fprintf(stderr,"frame %u: should delete\n",(unsigned)u->frame);failures++;} cdeletes++;return;}
    uint32_t want=(uint32_t)u->post[0]<<24|u->post[1]<<16|u->post[2]<<8|u->post[3];
    if (r!=want) {fprintf(stderr,"%s frame %u: canopy routine %06x MAME %06x\n",descent_captures[u->capture],(unsigned)u->frame,(unsigned)r,(unsigned)want);failures++;}
    if (want==0x3f1e) cdeletes++;
    for (unsigned o=0x10;o<0xb0;o++) {
        if (skip(o) || (want==0x3f1e && o==0x1d)) continue;
        cbytes++;
        if (a->m[o]!=u->post[o] && failures++<10) fprintf(stderr,"%s frame %u canopy routine %06x: +%02X native %02x MAME %02x\n",descent_captures[u->capture],(unsigned)u->frame,(unsigned)u->routine,o,a->m[o],u->post[o]);
    }
}
static int pfield(const descent_update_t *u,unsigned off) {
    for (unsigned i=0;i<NP;i++) if (descent_pfields[i]==off) return u->ram[i];
    return -1;
}
static int rfield(const descent_update_t *u,unsigned off) {
    for (unsigned i=0;i<NR;i++) if (descent_rfields[i]==off) return u->ram[NP+i];
    return -1;
}
static uint32_t lng(int (*f)(const descent_update_t *,unsigned),const descent_update_t *u,unsigned off,uint32_t keep) {
    for (unsigned i=0;i<4;i++) {int b=f(u,off+i);if (b>=0) keep=(keep&~(0xffu<<(24-8*i)))|(uint32_t)b<<(24-8*i);}
    return keep;
}
static void canopy(void) {
    neo_landing_input_t in;int capture=-1;
    for (unsigned i=0;i<NU;i++) {
        const descent_update_t *u=&descent_updates[i];
        if (!u->canopy) continue;
        if (u->capture!=capture) {
            capture=u->capture;memset(&in,0,sizeof(in));
            neo_landing_descent(&L);L.on_updated=on_updated;
            uint8_t keep[0x20];memcpy(keep,L.canopy.m,0x20);memcpy(L.canopy.m,u->pre,0xb0);
            L.canopy.m[0x14]=keep[0x14];memcpy(&L.canopy.m[0x19],&keep[0x19],4);L.canopy.m[0x1d]=keep[0x1d];
        }
        int v;
        if ((v=pfield(u,0x15))>=0) in.flags15=(uint8_t)v;
        if ((v=pfield(u,0x5f))>=0) in.flags5f=(uint8_t)v;
        if ((v=pfield(u,0x61))>=0) in.state61=(uint8_t)v;
        if ((v=pfield(u,0x1e))>=0 && pfield(u,0x1f)>=0) in.z=(uint16_t)(v<<8|pfield(u,0x1f));
        if ((v=pfield(u,0x4d))>=0) in.correction=(uint8_t)v;
        if ((v=rfield(u,0x25))>=0) in.root_correction=(uint8_t)v;
        if (u->ram[NP+NR]>=0) in.epoch=(uint8_t)u->ram[NP+NR];
        in.x=(int32_t)lng(pfield,u,0x3c,(uint32_t)in.x);in.y=(int32_t)lng(pfield,u,0x40,(uint32_t)in.y);
        in.previous_x=(int32_t)lng(pfield,u,0x44,(uint32_t)in.previous_x);in.previous_y=(int32_t)lng(pfield,u,0x48,(uint32_t)in.previous_y);
        in.root_x=(int32_t)lng(rfield,u,0x14,(uint32_t)in.root_x);in.root_y=(int32_t)lng(rfield,u,0x18,(uint32_t)in.root_y);
        in.root_previous_x=(int32_t)lng(rfield,u,0x1c,(uint32_t)in.root_previous_x);in.root_previous_y=(int32_t)lng(rfield,u,0x20,(uint32_t)in.root_previous_y);
        in.odd_phase=(u->frame>>1)&1u;
        expect=u;unsigned before=cupdates;
        neo_landing_update(&L,&in,program);cdraws+=L.draw_count;
        if (L.unsupported) {fprintf(stderr,"%s frame %u: explicit stop at %06x\n",descent_captures[u->capture],(unsigned)u->frame,(unsigned)L.unsupported_at);failures++;return;}
        if (cupdates==before) {fprintf(stderr,"frame %u: no native canopy update\n",(unsigned)u->frame);failures++;return;}
        if (failures) return;
    }
}
int main(void) {
    player();
    if (failures) return 1;
    canopy();
    if (failures) return 1;
    printf("PASS: parachute descent canopy (0C4A58/0C4AAE/0C4B20/0C4B62): %u updates, %u bytes equal, %u deletes, %u draws\n",cupdates,cbytes,cdeletes,cdraws);
    printf("PASS: parachute descent player (0C162A/0C16D8/0C170C): %u updates, %u bytes equal, %u cuts, %u landings\n",updates,bytes,cuts,landings);
    return 0;
}
