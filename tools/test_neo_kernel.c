#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "kernel_fixture.h"
#ifdef N64
#include <libdragon.h>
#endif
#include "region_rom.h"
#define COUNT(a) (sizeof(a)/sizeof((a)[0]))
static const uint8_t *rom_map(uint32_t address) {
    for (unsigned i=0;i<COUNT(region_blobs);i++)
        if (address>=region_blobs[i].address && address<region_blobs[i].address+region_blobs[i].length) return region_blobs[i].data+(address-region_blobs[i].address);
    return NULL;
}
/* Scene of a case: the terrain/root object and, for the dynamic cases, the
 * collision objects 6/7 with their bins as the original had them. */
static neo_kscene_t make_scene(neo_kobj_t obj,uint16_t w4018,uint16_t w4068,int dyn,bool *ok) {
    neo_kscene_t scene={.terrain=obj,.root=obj,.group_map=kernel_group_map,.w4018=w4018,.w4068=w4068};
    *ok=true;
    if (dyn<0) return scene;
    const kernel_dyn_t *d=&kernel_dyns[dyn];
    for (unsigned k=0;k<2;k++) {
        neo_kdyn_t *o=&scene.dyn[6+k];
        o->map=d->slot[k].map?rom_map(d->slot[k].map):NULL;
        if (d->slot[k].map && !o->map) *ok=false;
        o->pos=d->slot[k].pos;o->countdown=d->slot[k].countdown;
        for (unsigned i=0;i<4;i++) o->bbox[i]=d->slot[k].bbox[i];
        o->chain[0]=d->slot[k].parent;
    }
    for (unsigned i=0;i<18;i++) scene.bins_x[i]=d->bins_x[i];
    for (unsigned i=0;i<16;i++) scene.bins_y[i]=d->bins_y[i];
    scene.solid=d->solid;scene.self_dyn=d->self_dyn;scene.self_block=d->self_block;
    return scene;
}
int main(void) {
#ifdef N64
    debug_init_emulog();console_init();
#endif
    setvbuf(stdout,NULL,_IONBF,0);
    unsigned failures=0,contacts=0,dynamic=0;
    for (unsigned i=0;i<COUNT(kernel_cases);i++) {
        const kernel_case_t *c=&kernel_cases[i];
        bool ok;neo_kscene_t scene=make_scene(c->scene,c->w4018,c->w4068,c->dyn,&ok);
        neo_kres_t r;
        ok=ok && neo_kernel_sweep(&scene,&c->q,&r);dynamic+=c->dyn>=0;
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
        bool ok;neo_kscene_t scene=make_scene(c->scene,c->w4018,c->w4068,c->dyn,&ok);
        neo_kfloor_t r;dynamic+=c->dyn>=0;
        ok=ok && neo_kernel_floor(&scene,c->x,c->y,c->parent,c->depth,c->b4008,c->b400a,c->b400d,&r);
        if (!ok || r.x!=c->r.x || r.y!=c->r.y || r.parent!=c->r.parent || r.kind!=c->r.kind || r.d6!=c->r.d6) {
            printf("floor %u frame=%u got %08x %08x p=%u k=%08x d6=%08x want %08x %08x p=%u k=%08x d6=%08x\n",i,(unsigned)c->frame,
                (unsigned)r.x,(unsigned)r.y,r.parent,(unsigned)r.kind,(unsigned)r.d6,(unsigned)c->r.x,(unsigned)c->r.y,c->r.parent,(unsigned)c->r.kind,(unsigned)c->r.d6);
            floor_failures++;
        }
    }
    printf("kernel: %u/%u sweeps match (%u with contact), floors %u/%u\n",(unsigned)COUNT(kernel_cases)-failures,(unsigned)COUNT(kernel_cases),contacts,
        (unsigned)COUNT(floor_cases)-floor_failures,(unsigned)COUNT(floor_cases));
    assert(!failures && !floor_failures);
    printf("PASS: native collision kernel matches %u original sweeps and %u floor searches (%u with dynamic collision objects)\n",(unsigned)COUNT(kernel_cases),(unsigned)COUNT(floor_cases),dynamic);
#ifdef N64
    debugf("MSX KERNEL REGRESSION PASS sweeps=%u floors=%u\n",(unsigned)COUNT(kernel_cases),(unsigned)COUNT(floor_cases));
    for (;;) wait_ms(100);
#endif
}
