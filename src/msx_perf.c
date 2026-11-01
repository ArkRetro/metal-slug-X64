#include <libdragon.h>
#include "msx_perf.h"
#if MSX_PROFILE
static const char *const names[MSX_PERF_COUNT]={"sim","prep","begin","bgreq","bg","mesh","actors","hud","end","wait","asset","step","audio"};
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
#endif
/* Presentation timing per boot stage (docs/RENDIMIENTO.md, "Cinemáticas sin
 * parones"): msx_frames_present() at every frame handed to the display (also
 * from the RDP completion interrupt), msx_frames_stage() at each stage's
 * start logs the previous stage. Counted in vertical blanks: a frame handed
 * over shows from the next blank, so consecutive frames one blank apart are
 * seamless. gap_vb: blanks from the previous stage's last frame to this
 * stage's first (1: no pause between them); late: frames that came 2 or more
 * blanks after the previous one (the previous image shown again), long: 3 or
 * more; max_vb the longest. */
static volatile uint32_t frames_vi,frames_last,frames_max,frames_first_gap,frames_stage_last;
static volatile unsigned frames_count,frames_late,frames_long;
static const char *frames_label;
static void frames_on_vi(void) {frames_vi++;}
void msx_frames_present(void) {
    uint32_t now=frames_vi;
    if (!frames_count) frames_first_gap=frames_stage_last?now-frames_stage_last:0;
    else {
        uint32_t d=now-frames_last;
        if (d>frames_max) frames_max=d;
        if (d>=2) frames_late++;
        if (d>=3) frames_long++;
    }
    frames_last=now;frames_count++;
}
unsigned msx_frames_count(void) {return frames_count;}
void msx_frames_stage(const char *label) {
    static bool hooked;
    if (!hooked) {register_VI_handler(frames_on_vi);hooked=true;}
    disable_interrupts();
    unsigned n=frames_count,late=frames_late,lng=frames_long;uint32_t mx=frames_max,gap=frames_first_gap,last=frames_last;
    frames_count=frames_late=frames_long=0;frames_max=0;if (n) frames_stage_last=last;
    enable_interrupts();
    if (frames_label) debugf("MSX FRAMES %s presents=%u gap_vb=%lu max_vb=%lu late=%u long=%u\n",frames_label,n,(unsigned long)gap,(unsigned long)mx,late,lng);
    frames_label=label;
}
