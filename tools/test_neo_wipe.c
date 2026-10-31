/* Stage transition actors 3AED8/3AEF0 (src/neo_wipe.c) against the family
 * trace of the Mission 2 stage 1 -> 2 transition (data/wipe_fixture.json.gz):
 * per update the routine, +72, every fix-map write and the creator's +60. */
#include <stdio.h>
#include <string.h>
#include "../src/neo_wipe.h"
#include "wipe_fixture.h"
static int fails;
#define CHECK(c,...) do{if(!(c)){fails++;printf("FAIL: ");printf(__VA_ARGS__);printf("\n");}}while(0)
static unsigned zi,zend,zbad;
static void wr(void *ctx,uint16_t vram,uint16_t tile) {(void)ctx;if (zi>=zend || wipe_z[zi][0]!=vram || wipe_z[zi][1]!=tile) zbad++;zi++;}
int main(void) {
    neo_wipe_t w;unsigned n=sizeof wipe_updates/sizeof wipe_updates[0],writes=0,actors=0,waits[2]={0,0};
    for (unsigned i=0;i<n;i++) {
        const wipe_update_t *u=&wipe_updates[i];
        if (u->routine==0x3AED8 || u->routine==0x3AEF0) {neo_wipe_create(&w,u->routine==0x3AEF0);actors++;}
        CHECK(w.routine==u->routine && w.cursor==(uint16_t)u->pre,"frame %u: routine %X/%X +72 %X/%X",u->frame,w.routine,u->routine,w.cursor,u->pre);
        zi=u->z0;zend=u->z0+u->zn;zbad=0;
        bool alive=neo_wipe_update(&w,wr,0);
        CHECK(!zbad && zi==zend,"frame %u: fix writes %u bad, %u of %u",u->frame,zbad,zi-u->z0,u->zn);
        writes+=u->zn;
        if (u->post<0) {CHECK(!alive && w.signal && u->signal==1,"frame %u: end",u->frame);}
        else {CHECK(alive && !w.signal && w.routine==u->post_routine && w.cursor==(uint16_t)u->post,"frame %u: after %X +72 %X/%X",u->frame,w.routine,w.cursor,u->post);waits[actors-1]++;}
    }
    printf("%s: wipes %u actors, %u updates, %u fix-map writes (waits %u/%u updates before +60)\n",fails?"FAIL":"PASS",actors,n,writes,waits[0],waits[1]);
    return fails!=0;
}
