/* Mission 2 stage 2: the 45-degree stairs (cells of shape 5, x 976..1031,
 * y 168..224) and the quarter slopes after them, against MAME captures of
 * jumps on them while the camera rises (data/m2_stairs_fixture.json.gz,
 * tools/capture_m2_stairs.lua; docs/MISION2.md, "Fase 2i").
 *  1. Every captured call 0143EE -> 013968 of player 1 is reproduced bit
 *     for bit by the native kernel with the port's grid and ring, the
 *     screen box of 0128E8 (ROM 016DBE, neo_kernel_player_sweep) and the
 *     captured scene positions. Without that box (the world's, 0 x 0: the
 *     port's wall_stop before the fix) no wall stop of the pilot's run at
 *     the wall x 944 is seen: the player walked into it and fell.
 *  2. The port's plane test (neo_land_step) misses some of the arcade's
 *     landings on these slopes: the falls of the real-hardware report.
 *     neo_kernel_air_landing (the fix, gameplay.c's air step) lands them
 *     exactly where the arcade did, and nowhere else.
 *  3. Closed loop: the captured input plans played through the port's floor
 *     model (gameplay.c's ground and air steps) fall through the stairs
 *     without the fix and follow the arcade's positions with it. */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/neo_collision.h"
#include "../src/neo_kernel.h"
#include "../src/neo_motion.h"
#include "m2_stairs_fixture.h"
static uint8_t grid[STAIRS_COLUMNS*STAIRS_ROWS],grid_dynamic[STAIRS_COLUMNS*STAIRS_ROWS],ring[4096];
static neo_kscene_t scene_of(const int32_t *terrain,const int32_t *root) {
    neo_kscene_t k;memset(&k,0,sizeof(k));
    k.terrain=(neo_kobj_t){terrain[0],terrain[1],terrain[2],terrain[3]};
    k.root=(neo_kobj_t){root[0],root[1],root[2],root[3]};
    k.group_map=stairs_group_map;k.ring=neo_collision_ring(k.terrain.x,k.terrain.y);
    return k;
}
static uint8_t object_of(uint32_t a) {return a==0x10e968u?NEO_KOBJ_ROOT:a==0x10c72eu?NEO_KOBJ_TERRAIN:NEO_KOBJ_NONE;}
/* gameplay.c's player step on the stairs (no wall, solid body or screen
 * edge there): the jump on the press edge, the ground step and the air
 * step with the plane test, and with `fix` the sweep's landing. */
typedef struct { neo_air_t b; bool grounded,jump_was; unsigned kernel_landings; } port_t;
static void port_step(port_t *p,const char *input,neo_kscene_t *k,bool fix) {
    int direction=strchr(input,'R')?1:strchr(input,'L')?-1:0;bool jump=strchr(input,'B')!=NULL;
    bool pending=jump && !p->jump_was;p->jump_was=jump;
    if (pending && p->grounded) {p->grounded=false;p->b.vy=NEO_JUMP_VY;}
    if (p->grounded) {p->grounded=neo_walk_step_shift(&p->b,direction,0);return;}
    int32_t previous=p->b.y;
    neo_air_accel(&p->b,direction);
    const neo_air_t start=p->b;
    p->b.x+=p->b.vx;p->b.y+=p->b.vy;
    p->grounded=neo_land_step(&p->b,previous);
    neo_kland_t land;
    if (!p->grounded && fix && neo_kernel_air_landing(k,start.x,start.y,start.vx,start.vy,&land)) {
        p->b=(neo_air_t){land.x,land.y,land.vx,land.vy};p->grounded=true;p->kernel_landings++;
    }
}
int main(void) {
    for (int c=STAIRS_C0,i=0;c<=STAIRS_C1;c++) for (int r=STAIRS_R0;r<=STAIRS_R1;r++,i++) grid[c*STAIRS_ROWS+r]=stairs_window[i];
    neo_collision_set_grid(grid,grid_dynamic,NULL,STAIRS_COLUMNS,STAIRS_ROWS);neo_collision_set_ring(ring);
    /* 1. The sweeps. */
    unsigned n=sizeof(stairs_sweeps)/sizeof(stairs_sweeps[0]),walls=0,walls_lost=0,air=0,landings=0,misses=0,fixed=0;
    for (unsigned i=0;i<n;i++) {
        const stairs_sweep_t *s=&stairs_sweeps[i];
        neo_kscene_t k=scene_of(s->terrain,s->root);
        k.b400c=(uint8_t)s->in[10];k.w4068=(uint16_t)s->in[12];k.w4018=(uint16_t)s->in[13];
        neo_kreq_t q={(int32_t)s->in[0],(int32_t)s->in[1],(int32_t)s->in[2],(int32_t)s->in[3],object_of(s->in[4]),s->in[5],
            (uint8_t)s->in[6],(uint8_t)s->in[7],(uint8_t)s->in[8],(uint8_t)s->in[9],(uint8_t)s->in[11]};
        assert(q.parent!=NEO_KOBJ_NONE && object_of(s->out[4])!=NEO_KOBJ_NONE);
        neo_kres_t r;
        /* Before the fix wall_stop swept with the world's box (0 x 0 here). */
        neo_kscene_t old_scene=k;neo_kres_t old;
        assert(neo_kernel_sweep(&old_scene,&q,&old));
        if (s->out[6]&4u) {walls++;walls_lost+=!(old.flags&4u);}
        assert(neo_kernel_player_sweep(&k,&q,&r));
        assert(k.config[0]==0 && k.config[2]==0);   /* the world's box is put back */
        if ((uint32_t)r.x!=s->out[0] || (uint32_t)r.y!=s->out[1] || (uint32_t)r.vx!=s->out[2] || (uint32_t)r.vy!=s->out[3] ||
            r.parent!=object_of(s->out[4]) || r.w4068!=(uint16_t)s->out[5] || r.flags!=(uint16_t)s->out[6] || r.kind!=s->out[7]) {
            printf("FAIL: sweep %u frame %u: %08X %08X %08X %08X flags %04X kind %08X, MAME %08X %08X %08X %08X flags %04X kind %08X\n",i,s->frame,
                (unsigned)r.x,(unsigned)r.y,(unsigned)r.vx,(unsigned)r.vy,r.flags,(unsigned)r.kind,s->out[0],s->out[1],s->out[2],s->out[3],s->out[6]&0xffffu,s->out[7]);
            return 1;
        }
        if (s->in[7]) continue;   /* 10C009 set: the ground step 014594 */
        air++;
        /* 2. The air step from the same state, relative to the terrain. */
        int32_t x=(int32_t)s->in[0],y=(int32_t)s->in[1],vx=(int32_t)s->in[2],vy=(int32_t)s->in[3];
        if (q.parent==NEO_KOBJ_ROOT) {x+=k.root.prev_x-k.terrain.prev_x;y+=k.root.prev_y-k.terrain.prev_y;}
        int32_t ox=(int32_t)s->out[0],oy=(int32_t)s->out[1];
        if (r.parent==NEO_KOBJ_ROOT) {ox+=k.root.x-k.terrain.x;oy+=k.root.y-k.terrain.y;}
        bool arcade=(s->out[6]&1u)!=0;landings+=arcade;
        neo_air_t plane={x+vx,y+vy,vx,vy};
        bool hit=neo_land_step(&plane,y);
        assert(!hit || arcade);            /* never a landing the arcade does not make */
        k=scene_of(s->terrain,s->root);k.w4068=(uint16_t)s->in[12];k.w4018=(uint16_t)s->in[13];
        neo_kland_t land;bool kernel=neo_kernel_air_landing(&k,x,y,vx,vy,&land);
        assert(kernel==arcade);
        if (kernel) assert(land.x==ox && land.y==oy && land.vx==(int32_t)s->out[2] && land.vy==(int32_t)s->out[3]);
        if (!kernel) assert(k.w4068==(uint16_t)s->in[12] && k.w4018==(uint16_t)s->in[13]);
        if (arcade && !hit) {
            misses++;fixed+=kernel;
            printf("plane test misses the arcade landing of frame %u: from (%.3f, %.3f) by (%.3f, %.3f), arcade and sweep at (%.3f, %.3f)\n",
                s->frame,x/65536.0,y/65536.0,vx/65536.0,vy/65536.0,ox/65536.0,oy/65536.0);
        }
    }
    assert(misses>0 && fixed==misses);
    /* The wall x 944 of the corridor y 80 (the pilot's run, frames 21500..21760). */
    printf("wall stops (D6 bit 2): %u in MAME, all reproduced with the player's box; %u lost with the world's 0 x 0 box\n",walls,walls_lost);
    assert(walls>0 && walls_lost==walls);
    /* 3. The runs. */
    unsigned runs=sizeof(stairs_runs)/sizeof(stairs_runs[0]),checked=0,falls=0,followed=0;
    for (unsigned i=0;i<runs;i++) {
        const stairs_run_t *run=&stairs_runs[i];
        bool fell[2]={false,false};unsigned same[2]={0,0},count[2]={0,0},kl=0;
        for (int fix=0;fix<2;fix++) {
            port_t p={{run->start[1],run->start[2],run->start[3],run->start[4]},(run->start[5]&0x40u)!=0,false,0};
            /* Synchronised on the first update after the walk to the stairs
             * (the walk itself is the same in every run). */
            bool on=false;
            for (unsigned u=0;u<run->count;u++) {
                const stairs_update_t *a=&run->updates[u];
                if (!on) {
                    if (a->frame<22388u) continue;
                    on=true;p.b=(neo_air_t){a->x,a->y,a->vx,a->vy};p.grounded=(a->flags15&0x40u)!=0;p.jump_was=strchr(a->input,'B')!=NULL;continue;
                }
                neo_kscene_t k=scene_of(a->terrain,a->root);
                port_step(&p,a->input,&k,fix!=0);
                count[fix]++;
                if (p.b.x==a->x && p.b.y==a->y) same[fix]++;
                if (p.b.x>976*NEO_UNIT && p.b.y<160*NEO_UNIT) fell[fix]=true;
            }
            if (fix) kl=p.kernel_landings;
        }
        /* The arcade never falls: its feet stay on or above the stairs. */
        for (unsigned u=0;u<run->count;u++) assert(!(run->updates[u].x>976*NEO_UNIT && run->updates[u].y<160*NEO_UNIT));
        printf("run %s (%s): %u updates; plane test only: %s, %u/%u positions as MAME; with the sweep's landing (%u): %s, %u/%u\n",
            run->name,run->plan,count[0],fell[0]?"FALLS THROUGH":"no fall",same[0],count[0],kl,fell[1]?"FALLS THROUGH":"no fall",same[1],count[1]);
        /* eB/eE/eG leave the model's scope: the jump without direction keeps
         * no speed in the arcade (vx = 0, eE), eG meets the wall x 944 (wall_stop). */
        if (!strcmp(run->name,"eA") || !strcmp(run->name,"eC") || !strcmp(run->name,"eD") || !strcmp(run->name,"eH")) {
            assert(fell[0] && !fell[1]);falls++;checked++;
            if (same[1]+8u>=count[1]) followed++;
        }
    }
    assert(checked==4 && falls==4 && followed==4);
    printf("PASS: %u player sweeps 0143EE/013968 bit-exact (%u wall stops, all lost without the box; %u in the air, %u landings); the plane test misses %u arcade landings on the stairs, "
        "all landed exactly by the sweep; 4 captured jump plans fall through without it and follow MAME with it\n",n,walls,air,landings,misses);
    return 0;
}
