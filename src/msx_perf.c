#include <libdragon.h>
#include "msx_perf.h"
static const char *const names[MSX_PERF_COUNT]={"sim","prep","begin","bgreq","bg","mesh","actors","hud","end","wait","asset","step"};
static uint32_t last,frame_ticks[MSX_PERF_COUNT];
static uint64_t sum[MSX_PERF_COUNT],sum_build,sum_total;
static uint32_t peak[MSX_PERF_COUNT],peak_build,peak_total;
static unsigned frames,steps_now,steps_max,steps_sum,multi,draws_max,soldiers_max;
static bool started;
void msx_perf_frame_start(void) {
    uint32_t now=TICKS_READ();
    if (started) frame_ticks[MSX_PERF_OUTSIDE]+=now-last;
    last=now;
}
void msx_perf_mark(unsigned phase) {uint32_t now=TICKS_READ();frame_ticks[phase]+=now-last;last=now;}
void msx_perf_add(unsigned bucket,uint32_t ticks) {
    if (bucket==MSX_PERF_STEP) {if (ticks>frame_ticks[bucket]) frame_ticks[bucket]=ticks;}
    else frame_ticks[bucket]+=ticks;
}
void msx_perf_steps(unsigned steps) {steps_now=steps;}
void msx_perf_counts(unsigned draws,unsigned soldiers) {
    if (draws>draws_max) draws_max=draws;
    if (soldiers>soldiers_max) soldiers_max=soldiers;
}
void msx_perf_frame_end(void) {
    uint32_t build=0,total=0;
    for (unsigned i=0;i<MSX_PERF_COUNT;i++) {
        if (i<MSX_PERF_OUTSIDE) total+=frame_ticks[i];
        if (i>=MSX_PERF_BEGIN && i<=MSX_PERF_END) build+=frame_ticks[i];
        // The first frame has no previous end: its outside time is undefined.
        if (started || i!=MSX_PERF_OUTSIDE) {
            sum[i]+=frame_ticks[i];if (frame_ticks[i]>peak[i]) peak[i]=frame_ticks[i];
        }
        frame_ticks[i]=0;
    }
    sum_build+=build;sum_total+=total;
    if (build>peak_build) peak_build=build;
    if (total>peak_total) peak_total=total;
    steps_sum+=steps_now;if (steps_now>steps_max) steps_max=steps_now;
    if (steps_now>1) multi++;
    started=true;
    if (++frames<300) return;
    char line[512];int n=snprintf(line,sizeof(line),"MSX PERF: frames=%u steps=%u/%u multi=%u draws<=%u soldiers<=%u cpu=%lu/%lu build=%lu/%lu",
        frames,steps_sum,steps_max,multi,draws_max,soldiers_max,
        (unsigned long)TICKS_TO_US(sum_total/frames),(unsigned long)TICKS_TO_US(peak_total),
        (unsigned long)TICKS_TO_US(sum_build/frames),(unsigned long)TICKS_TO_US(peak_build));
    for (unsigned i=0;i<MSX_PERF_COUNT && n<(int)sizeof(line);i++)
        n+=snprintf(line+n,sizeof(line)-n," %s=%lu/%lu",names[i],(unsigned long)TICKS_TO_US(sum[i]/frames),(unsigned long)TICKS_TO_US(peak[i]));
    debugf("%s us(mean/max)\n",line);
    for (unsigned i=0;i<MSX_PERF_COUNT;i++) sum[i]=peak[i]=0;
    sum_build=sum_total=0;peak_build=peak_total=0;frames=steps_sum=steps_max=multi=draws_max=soldiers_max=0;
}
