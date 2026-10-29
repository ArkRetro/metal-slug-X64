#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "kernel_fixture.h"
#ifdef N64
#include <libdragon.h>
#endif
#define COUNT(a) (sizeof(a)/sizeof((a)[0]))
int main(void) {
#ifdef N64
    debug_init_emulog();console_init();
#endif
    setvbuf(stdout,NULL,_IONBF,0);
    unsigned failures=0,contacts=0;
    for (unsigned i=0;i<COUNT(kernel_cases);i++) {
        const kernel_case_t *c=&kernel_cases[i];
        neo_kscene_t scene={c->scene,c->scene,kernel_group_map,c->w4018,c->w4068};
        neo_kres_t r;
        bool ok=neo_kernel_sweep(&scene,&c->q,&r);
        bool same=ok && r.x==c->r.x && r.y==c->r.y && r.vx==c->r.vx && r.vy==c->r.vy && r.parent==c->r.parent &&
            r.flags==c->r.flags && r.kind==c->r.kind && r.w4068==c->r.w4068;
        contacts+=(c->r.flags&1u)!=0;
        if (!same) {
            if (failures<12) printf("sweep %u frame=%u actor=%06x caller=%06x ok=%d got x=%08x y=%08x vx=%08x vy=%08x p=%u f=%04x k=%08x a=%04x\n"
                "   want x=%08x y=%08x vx=%08x vy=%08x p=%u f=%04x k=%08x a=%04x  in x=%08x y=%08x dx=%08x dy=%08x m=%08x 8=%x 9=%x\n",
                i,(unsigned)c->frame,(unsigned)c->actor,(unsigned)c->caller,ok,(unsigned)r.x,(unsigned)r.y,(unsigned)r.vx,(unsigned)r.vy,r.parent,r.flags,(unsigned)r.kind,r.w4068,
                (unsigned)c->r.x,(unsigned)c->r.y,(unsigned)c->r.vx,(unsigned)c->r.vy,c->r.parent,c->r.flags,(unsigned)c->r.kind,c->r.w4068,
                (unsigned)c->q.x,(unsigned)c->q.y,(unsigned)c->q.dx,(unsigned)c->q.dy,(unsigned)c->q.mode4004,c->q.b4008,c->q.b4009);
            failures++;
        }
    }
    unsigned floor_failures=0;
    for (unsigned i=0;i<COUNT(floor_cases);i++) {
        const floor_case_t *c=&floor_cases[i];
        neo_kscene_t scene={c->scene,c->scene,kernel_group_map,c->w4018,c->w4068};
        neo_kfloor_t r;
        bool ok=neo_kernel_floor(&scene,c->x,c->y,c->parent,c->depth,c->b4008,c->b400a,c->b400d,&r);
        if (!ok || r.x!=c->r.x || r.y!=c->r.y || r.parent!=c->r.parent || r.kind!=c->r.kind || r.d6!=c->r.d6) {
            printf("floor %u frame=%u got %08x %08x p=%u k=%08x d6=%08x want %08x %08x p=%u k=%08x d6=%08x\n",i,(unsigned)c->frame,
                (unsigned)r.x,(unsigned)r.y,r.parent,(unsigned)r.kind,(unsigned)r.d6,(unsigned)c->r.x,(unsigned)c->r.y,c->r.parent,(unsigned)c->r.kind,(unsigned)c->r.d6);
            floor_failures++;
        }
    }
    printf("kernel: %u/%u sweeps match (%u with contact), floors %u/%u\n",(unsigned)COUNT(kernel_cases)-failures,(unsigned)COUNT(kernel_cases),contacts,
        (unsigned)COUNT(floor_cases)-floor_failures,(unsigned)COUNT(floor_cases));
    assert(!failures && !floor_failures);
    printf("PASS: native collision kernel matches %u original sweeps and %u floor searches\n",(unsigned)COUNT(kernel_cases),(unsigned)COUNT(floor_cases));
#ifdef N64
    debugf("MSX KERNEL REGRESSION PASS sweeps=%u floors=%u\n",(unsigned)COUNT(kernel_cases),(unsigned)COUNT(floor_cases));
    for (;;) wait_ms(100);
#endif
}
