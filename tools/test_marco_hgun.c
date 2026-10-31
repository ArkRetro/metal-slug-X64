/* H upper body (src/marco_hgun.c).
 * 1. The states with scripted inputs: the bullets of every update follow the
 *    callback updates of the programs (0C4372 at 0/2/4/6 of a burst, 0C453A
 *    at 0/2/4/6 of a sweep, END at 8/6/12/16) and +92/+95.
 * 2. hfam (data/hgun_fixture.json, tools/extract_hgun_fixture.py): every
 *    diagonal H bullet the capture logged (angle not a fan angle of 0/40/80)
 *    is, unmirrored with the facing of its frame (+15 bit 0: 0x80 - angle), a
 *    step of a sweep program of marco_hgun.c, in program order within its
 *    sweep (the capture has no 10000E column, so the frames of the updates
 *    are not checked against it). */
#include <stdio.h>
#include <string.h>
#include "../src/marco_hgun.h"
#include "hgun_fixture.h"

static bool reserve(void *c) {(void)c;return true;}
static unsigned bad;
#define EXPECT(cond,...) do { if (!(cond)) {bad++;fprintf(stderr,__VA_ARGS__);fputc('\n',stderr);} } while (0)

/* Steps the model n updates and appends every shot angle (0xFFFF-terminated per update). */
static unsigned run(marco_hgun_t *h,marco_hgun_input_t in,unsigned updates,uint8_t *angles,unsigned *count) {
    unsigned shots=0;
    for (unsigned u=0;u<updates;u++) {
        marco_hgun_shot_t s[4];unsigned n=marco_hgun_step(h,&in,reserve,NULL,s,4);
        for (unsigned k=0;k<n;k++) angles[(*count)++]=s[k].angle;
        shots+=n;
    }
    return shots;
}
static void scripted(void) {
    marco_hgun_t h;marco_hgun_reset(&h);uint8_t a[64];unsigned n=0;
    marco_hgun_input_t fwd={false,false,false},up={true,false,false},upl={true,false,true};
    /* Press forward: the press is seen, the burst starts next update. */
    marco_hgun_press(&h);
    EXPECT(run(&h,fwd,1,a,&n)==0 && h.state==MARCO_HGUN_ENTER,"idle press");
    /* Burst forward: bullets at updates 0,2,4,6 with the fan 0,+2,-2,0 (+95 1..4). */
    unsigned shots[13];for (unsigned u=0;u<13;u++) shots[u]=run(&h,fwd,1,a,&n);
    EXPECT(shots[0]==1&&shots[1]==0&&shots[2]==1&&shots[4]==1&&shots[6]==1&&shots[7]==0,"burst callback updates");
    EXPECT(n==4 && a[0]==0x00 && a[1]==0x02 && a[2]==0xfe && a[3]==0x00,"burst fan %02X %02X %02X %02X",a[0],a[1],a[2],a[3]);
    EXPECT(h.state==MARCO_HGUN_IDLE && h.combo95==4,"burst end (END at 12) state %u +95 %u",h.state,h.combo95);
    /* A burst, then up before +76: the forward->up sweep 08/18/28/38 at once. */
    marco_hgun_reset(&h);n=0;marco_hgun_press(&h);run(&h,fwd,2,a,&n);
    run(&h,up,9,a,&n);
    EXPECT(n==5 && a[1]==0x08 && a[2]==0x18 && a[3]==0x28 && a[4]==0x38 && h.aim92==1,"sweep up: %u shots %02X %02X %02X %02X +92 %d",n,a[1],a[2],a[3],a[4],h.aim92);
    EXPECT(h.state==MARCO_HGUN_IDLE,"sweep end without a press: idle");
    /* Facing left the sweep is mirrored (0x80 - angle); a press during it
     * chains the burst up in the update of the END (two updates after 0x48). */
    marco_hgun_reset(&h);n=0;marco_hgun_press(&h);run(&h,upl,2,a,&n);
    unsigned u0=0;run(&h,upl,3,a,&n);marco_hgun_press(&h);run(&h,upl,6,a,&n);
    EXPECT(n-u0>=5 && a[u0]==0x78 && a[u0+1]==0x68 && a[u0+2]==0x58 && a[u0+3]==0x48 && ((a[u0+4]+2)&0xff)>=0x3e && ((a[u0+4]+2)&0xff)<=0x44,"left sweep then burst up: %02X %02X %02X %02X %02X",a[u0],a[u0+1],a[u0+2],a[u0+3],a[u0+4]);
    /* Releasing up after the sweep: up->forward 30/20/10 on the next press. */
    marco_hgun_reset(&h);h.aim92=1;n=0;marco_hgun_press(&h);run(&h,fwd,8,a,&n);
    EXPECT(n==3 && a[0]==0x30 && a[1]==0x20 && a[2]==0x10 && h.aim92==0,"sweep up->forward %u %02X %02X %02X",n,a[0],a[1],a[2]);
}
/* 3. The capture air (data/hgun_replay_fixture.json): its input words fed
 * update by update (the fire press as the next update's word shows it: the
 * words are sampled a frame after the player read them; up = +2 bit 0, down
 * = bit 1 in the air, facing = +15 bit 0) give every H bullet of the
 * original in the same update with the same angle: the downward bursts and
 * sweeps in the air, the forward<->up sweeps walking and the down->forward
 * sweep on landing. Left out, and resynchronised once the original is quiet
 * for 8 updates (+92, +95 from the capture): the windows where the
 * original's upper body takes states the port's player has not, the knife
 * 0C3B46 (+9A bit 0 from 0C3744: an enemy in reach, a press slashes) and the
 * landing into the crouch with down held (0C1EBE -> 0C2236). */
static void replay(void) {
    marco_hgun_t h;marco_hgun_reset(&h);h.aim92=(int8_t)hgun_updates[0].f92;h.combo95=(uint8_t)hgun_updates[0].f95;
    unsigned compared=0,skipped=0,bullets=0,diagonal=0,hold=0,quiet=0;bool skip=false;
    for (unsigned i=0;i<HGUN_UPDATES;i++) {
        const hgun_update_t *u=&hgun_updates[i];
        if (u->f15&0x100u) hold=12;
        if ((u->f15&0x40u) && (u->held&2u) && h.aim92<0) hold=12;
        if (hold || skip) {
            if (hold) hold--;
            quiet=u->births?0:quiet+1;skipped++;
            if (!hold && quiet>=8) {skip=false;marco_hgun_reset(&h);h.aim92=(int8_t)u->f92;h.combo95=(uint8_t)u->f95;}
            else skip=true;
            continue;
        }
        bool next=i+1<HGUN_UPDATES && (hgun_updates[i+1].news&0x10u);
        if ((h.state==MARCO_HGUN_IDLE && next) || (h.state!=MARCO_HGUN_IDLE && (u->news&0x10u))) marco_hgun_press(&h);
        marco_hgun_input_t in={(u->held&1u)!=0,(u->held&2u) && !(u->f15&0x40u),(u->f15&1u)!=0};
        marco_hgun_shot_t s[4];unsigned n=marco_hgun_step(&h,&in,reserve,NULL,s,4);
        bool same=n==u->births;
        for (unsigned k=0;same && k<n;k++) same=s[k].angle==u->angle[k];
        EXPECT(same,"air %u: %u bullets (%02X), original %u (%02X)",u->frame,n,n?s[0].angle:0,u->births,u->births?u->angle[0]:0);
        for (unsigned k=0;k<n;k++) diagonal+=s[k].sweep;
        bullets+=n;compared++;
    }
    printf("air replay: %u updates compared (%u bullets, %u sweep steps), %u left out (knife, crouch landing)\n",compared,bullets,diagonal,skipped);
    EXPECT(compared>100 && diagonal>0,"air replay too short");
}
int main(void) {
    scripted();
    replay();
    static const uint8_t steps[2][4]={{0x08,0x18,0x28,0x38},{0x30,0x20,0x10,0}};
    unsigned diagonal=0,sweeps=0;int seq=-1,at=0;unsigned last=0;
    for (unsigned i=0;i<HGUN_ROWS;i++) {
        const hgun_row_t *r=&hgun_rows[i];
        for (unsigned b=0;b<r->births;b++) {
            uint8_t g=r->angle[b];
            if (((g+2)&0x3f)<=4) continue;  /* fan angles of 0C4372 */
            uint8_t raw=(r->f15&1)?(uint8_t)(0x80-g):g;
            /* A new sweep after a pause of more than one sweep step. */
            int s=-1,k=-1;
            for (int q=0;q<2;q++) for (int j=0;j<4;j++) if (steps[q][j]==raw && steps[q][j]) {s=q;k=j;}
            if (s<0) {bad++;fprintf(stderr,"%u: logged angle %02X (facing %s) is no sweep step\n",r->frame,g,(r->f15&1)?"left":"right");continue;}
            bool fresh=seq!=s || r->frame-last>12;
            if (fresh) {sweeps++;seq=s;at=k;}
            else {
                /* A logged step follows the previous one (unlogged steps are skipped). */
                if (k<=at) {bad++;fprintf(stderr,"%u: angle %02X out of order\n",r->frame,g);}
                at=k;
            }
            last=r->frame;diagonal++;
        }
    }
    printf("H aim: %u logged diagonal bullets in %u sweeps are steps of 0C453A programs (facing-mirrored); scripted states checked; %u mismatches\n",diagonal,sweeps,bad);
    if (bad) return 1;
    printf("PASS: H fire states and diagonal sweeps (0C3E9C..0C4006, 0C453A)\n");
    return 0;
}
