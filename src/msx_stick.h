#ifndef MSX_STICK_H
#define MSX_STICK_H
/* The N64 analog stick as the arcade's 8-way lever: outside a dead zone of
 * radius 25, an axis counts only when the stick lies within 67.5 degrees of
 * it (|c| * 12 >= |other| * 5, tan 22.5 = 0.414), so every direction owns a
 * 45-degree sector as on a gated lever. The former square gate (each axis
 * past 25 on its own) read a push right with 17 degrees of slack as
 * up-right: the H swept forward->up and back while walking (MiSTer report
 * "la H dispara mal en diagonal"). */
static inline void msx_stick_lever(int x,int y,int *h,int *v) {
    *h=*v=0;
    if (x*x+y*y<25*25) return;
    int ax=x<0?-x:x,ay=y<0?-y:y;
    if (ax*12>=ay*5) *h=x<0?-1:1;
    if (ay*12>=ax*5) *v=y<0?-1:1;
}
#endif
