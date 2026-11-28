/* The player's ground step 014594 on the Mission 6 bridge 10D7B6 against
 * MAME 0.264 (data/m6walk_fixture.json.gz, tools/extract_m6walk_fixture.py:
 * captures-m6/fam/P). The bridge's ring is rebuilt as test_m6_ring does
 * (data/m6ring_fixture.json.gz); 10C00E is the stage's group map 0BC67C
 * (group 4 -> 9). Each update: +62 = +-0x30000 (0C2918),
 * neo_contact_prepare_ground (002E98 by +6D, the angle of the previous
 * update's D7), the player's 013968 (neo_kernel_player_sweep, 10C009 = FF)
 * from MAME's previous +28/+2C; the result must be MAME's next +28/+2C with
 * +38 the root (013BC0: group 9) and D6 bit 0 (on the floor). The first
 * update of a stretch takes its angle from the floor search 013DB2 at the
 * start (the update before it is not in the stretch). Root at the camera
 * (1888, 80) of the stage's camera list (data/m6s1_camera_fixture.json.gz). */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "neo_m6bridge.h"
#include "neo_kernel.h"
#include "neo_contact.h"
#include "region_rom.h"
#include "m6ring_fixture.h"
#include "m6walk_fixture.h"
#define COUNT(a) (sizeof(a)/sizeof((a)[0]))
static const uint8_t *blob(uint32_t a,unsigned n) {
    for (unsigned i=0;i<COUNT(region_blobs);i++) if (region_blobs[i].address==a && region_blobs[i].length>=n) return region_blobs[i].data;
    return NULL;
}
static neo_m6ring_t ring;static neo_m6map_t map;
static unsigned fi;static bool on;static int16_t p32,p34;static int32_t lx,ly,plx,ply;static uint32_t at;
/* The bridge header as the snapshot labels it (the tap's frame + 1). */
static void upto(uint32_t target) {
    for (uint32_t f=at;f<=target;f++) {
        if (f==m6ring_clear) neo_m6ring_clear(&ring);
        while (fi+1<COUNT(m6ring_frames) && m6ring_frames[fi+1].frame<=f) fi++;
        const m6ring_frame_t *h=&m6ring_frames[fi];
        if (h->map==0x0BC6E6u) {
            bool rows=(h->b82&4u)!=0;
            if (!on) {neo_m6ring_init(&ring,&map,h->x,h->y,h->o32,h->o34,NULL,NULL);on=true;}
            else {
                neo_m6ring_pass(&ring,&map,h->x,h->y,p32,p34,rows,NULL,NULL);
                if (h->o32!=p32 || h->o34!=p34) neo_m6ring_pass(&ring,&map,h->x,h->y,h->o32,h->o34,rows,NULL,NULL);
                neo_m6ring_mark(&ring,h->x,h->y,rows);
            }
            p32=h->o32;p34=h->o34;
        } else on=false;
        plx=lx;ply=ly;lx=h->x;ly=h->y;
    }
    at=target+1;
}
int main(void) {
    const uint8_t *d=blob(0x0BC6E6u,14),*c=blob(0x45FC2Au,1200);
    if (!d || !c) {printf("FAIL: region_rom.h lacks 0BC6E6 / 45FC2A\n");return 1;}
    map=(neo_m6map_t){c,(uint16_t)(d[8]<<8|d[9]),(uint16_t)(d[10]<<8|d[11])};
    neo_m6ring_clear(&ring);at=m6ring_frames[0].frame;
    static neo_kscene_t k;
    k.group_map=m6walk_group_map;
    k.root=(neo_kobj_t){m6walk_root[0],m6walk_root[1],m6walk_root[0],m6walk_root[1]};k.terrain=k.root;   /* no terrain cell under the bridge */
    k.train_on=true;k.bridge_ring=&ring;k.bridge_map=&map;
    unsigned steps=0,bad=0;
    for (unsigned s=0;s<COUNT(m6walk_stretches);s++) {
        const m6walk_stretch_t *t=&m6walk_stretches[s];
        const m6walk_sample_t *p=&m6walk_samples[t->first];
        upto(p->frame+1);k.train=(neo_kobj_t){lx,ly,plx,ply};
        neo_kfloor_t fl;
        if (!neo_kernel_floor(&k,p->x,p->y,NEO_KOBJ_TERRAIN,-16*65536,0xff,0,0,&fl) || !fl.found) {printf("FAIL: no floor at the start of stretch %u\n",s);return 1;}
        uint16_t angle=(uint16_t)(fl.kind>>8);int32_t vx=0,vy=0;
        for (unsigned i=1;i<t->count;i++) {
            const m6walk_sample_t *a=&p[i-1],*b=&p[i];
            upto(b->frame+1);k.train=(neo_kobj_t){lx,ly,plx,ply};
            neo_contact_actor_t ac={0};ac.x=a->x;ac.y=a->y;ac.vx=vx;ac.vy=vy;ac.scalar62=t->speed;ac.kind_angle=angle;ac.flags15=0x40;
            neo_contact_request_t q=neo_contact_prepare_ground(&ac);
            neo_kreq_t r={a->x,a->y,q.dx,q.dy,NEO_KOBJ_TERRAIN,q.mode,q.mode8,0xff,0,0,0xff};neo_kres_t o;
            steps++;
            if (!neo_kernel_player_sweep(&k,&r,&o) || o.x!=b->x || o.y!=b->y || o.parent!=NEO_KOBJ_ROOT || !(o.flags&1u)) {
                if (bad++<5) printf("FAIL frame %u: %08x,%08x parent %u flags %04x (MAME %08x,%08x root, floor)\n",(unsigned)b->frame,(unsigned)o.x,(unsigned)o.y,o.parent,o.flags,(unsigned)b->x,(unsigned)b->y);
            }
            angle=(uint16_t)(o.kind>>8);vx=o.vx;vy=o.vy;
        }
    }
    if (bad) {printf("FAIL: player on the bridge, %u of %u ground steps differ\n",bad,steps);return 1;}
    printf("PASS: player 1 walking on the bridge 10D7B6 (014594, group map 0BC67C): %u ground steps equal to MAME (+28/+2C, +38 = root, floor)\n",steps);
    return 0;
}
