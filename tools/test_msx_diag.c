/* Host test of the diagnostic ring and core (src/msx_diag_ring.h,
 * src/msx_diag_core.c, built with -DMSX_DIAG=1): order and wrap of the
 * fixed-size ring, pool peaks and refusals with one ring entry per report
 * window, explicit stops recorded once per change and dumped once per kind. */
#include <stdio.h>
#include <stdlib.h>
#include "../src/msx_diag_ring.h"
#include "../src/msx_diag_core.h"
static int failures;
#define CHECK(c) do { if (!(c)) {printf("FAIL %s:%d %s\n",__FILE__,__LINE__,#c);failures++;} } while (0)
static void test_ring(void) {
    static msx_diag_ring_t r;
    CHECK(msx_diag_ring_count(&r)==0 && msx_diag_ring_lost(&r)==0);
    for (unsigned i=0;i<5;i++) msx_diag_ring_push(&r,(msx_diag_entry_t){i,1,(uint8_t)i,(uint16_t)(100+i),0x1000u+i});
    CHECK(msx_diag_ring_count(&r)==5);
    for (unsigned i=0;i<5;i++) CHECK(msx_diag_ring_at(&r,i)->frame==i && msx_diag_ring_at(&r,i)->c==0x1000u+i);
    /* Fill past the size: the newest MSX_DIAG_RING stay, oldest first. */
    for (unsigned i=5;i<3u*MSX_DIAG_RING+7u;i++) msx_diag_ring_push(&r,(msx_diag_entry_t){i,2,0,0,i});
    uint32_t total=3u*MSX_DIAG_RING+7u;
    CHECK(r.total==total);
    CHECK(msx_diag_ring_count(&r)==MSX_DIAG_RING);
    CHECK(msx_diag_ring_lost(&r)==total-MSX_DIAG_RING);
    for (unsigned i=0;i<MSX_DIAG_RING;i++) CHECK(msx_diag_ring_at(&r,i)->frame==total-MSX_DIAG_RING+i);
    CHECK(msx_diag_ring_at(&r,MSX_DIAG_RING-1)->c==total-1u);
    /* Exactly full. */
    static msx_diag_ring_t f;
    for (unsigned i=0;i<MSX_DIAG_RING;i++) msx_diag_ring_push(&f,(msx_diag_entry_t){i,1,0,0,0});
    CHECK(msx_diag_ring_count(&f)==MSX_DIAG_RING && msx_diag_ring_lost(&f)==0 && msx_diag_ring_at(&f,0)->frame==0);
}
static void test_core(void) {
    msx_diag_set_frame(10);
    msx_diag_event(MSX_EV_STAGE,1,2,0x2d70fcu);
    CHECK(msx_diag.ring.total==1 && msx_diag_ring_at(&msx_diag.ring,0)->frame==10 && msx_diag_ring_at(&msx_diag.ring,0)->type==MSX_EV_STAGE);
    /* Peaks: the highest sample and its frame. */
    msx_diag_pool_sample(MSX_POOL_GACTOR,40);msx_diag_set_frame(11);msx_diag_pool_sample(MSX_POOL_GACTOR,57);msx_diag_pool_sample(MSX_POOL_GACTOR,12);
    CHECK(msx_diag.pool[MSX_POOL_GACTOR].used==12 && msx_diag.pool[MSX_POOL_GACTOR].peak==57 && msx_diag.pool[MSX_POOL_GACTOR].peak_frame==11);
    /* Refusals: all counted, one ring entry per window. */
    uint32_t before=msx_diag.ring.total;
    for (unsigned i=0;i<50;i++) msx_diag_pool_fail(MSX_POOL_ORDER);
    CHECK(msx_diag.pool[MSX_POOL_ORDER].fails==50 && msx_diag.ring.total==before+1);
    CHECK(msx_diag_ring_at(&msx_diag.ring,msx_diag_ring_count(&msx_diag.ring)-1)->type==MSX_EV_POOL);
    msx_diag.pool[MSX_POOL_ORDER].fails_window=0;   /* next window */
    msx_diag_pool_fail(MSX_POOL_ORDER);
    CHECK(msx_diag.ring.total==before+2 && msx_diag.pool[MSX_POOL_ORDER].fails==51);
    /* Stops: recorded on change only, the first of a kind dumps. */
    before=msx_diag.ring.total;unsigned dumps=msx_diag.dumps;
    msx_diag_stop(MSX_STOP_WORLD,0,0);
    CHECK(msx_diag.ring.total==before && msx_diag.dumps==dumps);
    msx_diag_stop(MSX_STOP_WORLD,1,0x0afe12u);
    msx_diag_stop(MSX_STOP_WORLD,1,0x0afe12u);
    CHECK(msx_diag.ring.total==before+1 && msx_diag.dumps==dumps+1);
    msx_diag_stop(MSX_STOP_WORLD,2,0x0afe12u);
    CHECK(msx_diag.ring.total==before+2 && msx_diag.dumps==dumps+1);
    msx_diag_stop(MSX_STOP_EXTERNAL,3,0x12345u);   /* routine in the reference: no dump */
    CHECK(msx_diag.dumps==dumps+1 && msx_diag.stop[MSX_STOP_EXTERNAL].routine==0x12345u);
    /* Dumps are capped (a stop loop cannot flood the log). */
    for (unsigned i=0;i<40;i++) msx_diag_dump("test");
    CHECK(msx_diag.dumps==16);
}
int main(void) {
    test_ring();
    test_core();
    if (failures) {printf("FAIL: msx_diag (%d)\n",failures);return 1;}
    printf("PASS: msx_diag ring (%u entries, wrap and order), pool peaks/refusals, stops and dump limit\n",MSX_DIAG_RING);
    return 0;
}
