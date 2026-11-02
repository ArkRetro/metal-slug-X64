/* Diagnostic mode, portable part (msx_diag.h): event ring, pool peaks and
 * refusals, explicit stops, dumps. Compiled to nothing unless MSX_DIAG. */
#include "msx_diag.h"
#if MSX_DIAG
#include "msx_diag_ring.h"
#include "msx_diag_core.h"
#include <stdio.h>
#include <stdlib.h>
#ifdef N64
#include <libdragon.h>
#define DIAG_PRINT(...) debugf(__VA_ARGS__)
#else
#define DIAG_PRINT(...) fprintf(stderr,__VA_ARGS__)
#endif
msx_diag_state_t msx_diag;
static const char *const event_names[MSX_EV_COUNT]={"-","boot","stage","flow","reset","spawn","soldier","kill","death","weapon","stop","pool","slow","boss","mount","checkpoint","combo"};
const char *const msx_diag_pool_names[MSX_POOL_COUNT]={"order","soldier","throw","splash","gactor","boss","draw","box","reg","sound","shot","sub","solid","dyn","bullet"};
const uint16_t msx_diag_pool_capacity[MSX_POOL_COUNT]={160,12,8,8,96,96,160,48,64,16,8,4,16,8,16};
const char *const msx_diag_stop_names[MSX_STOP_COUNT]={"world","boss","death","weapon","grenade","mount","vehicle","item","hud","script","skipped","external","camera","marco","solid"};
void msx_diag_set_frame(uint32_t frame) {msx_diag.frame=frame;}
void msx_diag_event(msx_diag_event_t type,unsigned a,unsigned b,uint32_t c) {
    msx_diag_ring_push(&msx_diag.ring,(msx_diag_entry_t){msx_diag.frame,(uint8_t)type,(uint8_t)a,(uint16_t)b,c});
}
void msx_diag_pool_fail(msx_diag_pool_t pool) {
    /* One ring entry per pool and report window: a full pool refuses on
     * every update and would otherwise flush the ring. */
    if (!msx_diag.pool[pool].fails_window++) msx_diag_event(MSX_EV_POOL,pool,(unsigned)(msx_diag.pool[pool].fails+1u),0);
    msx_diag.pool[pool].fails++;
}
void msx_diag_pool_sample(msx_diag_pool_t pool,unsigned used) {
    msx_diag_pool_stat_t *p=&msx_diag.pool[pool];
    p->used=(uint16_t)used;
    if (used>p->peak) {p->peak=(uint16_t)used;p->peak_frame=msx_diag.frame;}
    if (used>p->peak_window) p->peak_window=(uint16_t)used;
}
void msx_diag_stop(msx_diag_stop_t kind,unsigned count,uint32_t routine) {
    msx_diag_stop_stat_t *s=&msx_diag.stop[kind];
    if (!count) routine=0;   /* nothing stopped yet: no routine to report */
    if (count==s->count && routine==s->routine) return;
    s->count=count;s->routine=routine;
    msx_diag_event(MSX_EV_STOP,kind,count,routine);
    /* The first stop of each kind dumps the ring (what led to it); unported
     * spawns and skipped records are routine in the reference: no dump. */
    if (!(msx_diag.dumped_stops&(1u<<kind)) && kind!=MSX_STOP_EXTERNAL && kind!=MSX_STOP_SKIPPED && kind!=MSX_STOP_CAMERA) {
        msx_diag.dumped_stops|=1u<<kind;
        char reason[48];snprintf(reason,sizeof(reason),"stop %s %06lx",msx_diag_stop_names[kind],(unsigned long)routine);
        msx_diag_dump(reason);
    }
}
static void dump_entry(unsigned i,const msx_diag_entry_t *e) {
    const char *name=e->type<MSX_EV_COUNT?event_names[e->type]:"?";
    if (e->type==MSX_EV_POOL) DIAG_PRINT("MSX DIAG ring %3u f=%lu pool %s fails=%u\n",i,(unsigned long)e->frame,e->a<MSX_POOL_COUNT?msx_diag_pool_names[e->a]:"?",e->b);
    else if (e->type==MSX_EV_STOP) DIAG_PRINT("MSX DIAG ring %3u f=%lu stop %s count=%u routine=%06lx\n",i,(unsigned long)e->frame,e->a<MSX_STOP_COUNT?msx_diag_stop_names[e->a]:"?",e->b,(unsigned long)e->c);
    else DIAG_PRINT("MSX DIAG ring %3u f=%lu %s a=%u b=%u c=%06lx\n",i,(unsigned long)e->frame,name,e->a,e->b,(unsigned long)e->c);
}
void msx_diag_dump_pools(void) {
    char line[400];int n=snprintf(line,sizeof(line),"MSX DIAG pools (used/peak/cap fails):");
    for (unsigned i=0;i<MSX_POOL_COUNT && n<(int)sizeof(line);i++) {
        const msx_diag_pool_stat_t *p=&msx_diag.pool[i];
        n+=snprintf(line+n,sizeof(line)-n," %s=%u/%u/%u",msx_diag_pool_names[i],p->used,p->peak,msx_diag_pool_capacity[i]);
        if (p->fails && n<(int)sizeof(line)) n+=snprintf(line+n,sizeof(line)-n,"!%lu",(unsigned long)p->fails);
    }
    DIAG_PRINT("%s\n",line);
}
void msx_diag_dump_stops(void) {
    char line[400];int n=snprintf(line,sizeof(line),"MSX DIAG stops:");
    for (unsigned i=0;i<MSX_STOP_COUNT && n<(int)sizeof(line);i++) if (msx_diag.stop[i].count)
        n+=snprintf(line+n,sizeof(line)-n," %s=%u@%06lx",msx_diag_stop_names[i],msx_diag.stop[i].count,(unsigned long)msx_diag.stop[i].routine);
    DIAG_PRINT("%s\n",line);
}
void msx_diag_dump(const char *reason) {
    /* Rate limit: at most 16 dumps per session (a stop loop cannot flood
     * the log); assertions and exceptions always dump. */
    if (msx_diag.dumps>=16 && !msx_diag.fatal) return;
    msx_diag.dumps++;
    const msx_diag_ring_t *r=&msx_diag.ring;unsigned count=msx_diag_ring_count(r);
    DIAG_PRINT("MSX DIAG dump (%s) frame=%lu events=%u lost=%lu\n",reason,(unsigned long)msx_diag.frame,count,(unsigned long)msx_diag_ring_lost(r));
    for (unsigned i=0;i<count;i++) dump_entry(i,msx_diag_ring_at(r,i));
    msx_diag_dump_pools();
    msx_diag_dump_stops();
    DIAG_PRINT("MSX DIAG dump end\n");
}
void msx_diag_check_failed(const char *file,int line,const char *expr) {
    msx_diag.fatal=true;
    char reason[96];snprintf(reason,sizeof(reason),"check %s:%d %s",file,line,expr);
    msx_diag_dump(reason);
#ifdef N64
    assertf(0,"MSX diag check: %s (%s:%d)",expr,file,line);
#endif
    abort();
}
#else
typedef int msx_diag_core_unused_t;
#endif
