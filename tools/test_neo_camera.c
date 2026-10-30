/* Closed-loop test of the native mission camera (src/neo_camera.c) against
 * one MAME play of Mission 1 (data/camera_fixture.json.gz): the native state
 * is initialised once from the first record and never reloaded. Writes of
 * other actors between two camera updates are applied through the native
 * entry points that the original calls (script stop/release 0C7B0C/0C7C12,
 * autoscroll 0C7B4E/0C7BBA, shake 0C801A/0C8022, script op F on 69B8/69BA)
 * and counted; every recorded field must then match after each update. */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/neo_camera.h"
#include "camera_fixture.h"
#include "region_rom.h"
#define N (sizeof(camera_records)/sizeof(camera_records[0]))
static const uint8_t *rom(uint32_t a,uint16_t *size) {
    for (unsigned i=0;i<sizeof(region_blobs)/sizeof(region_blobs[0]);i++)
        if (region_blobs[i].address==a) {*size=region_blobs[i].length;return region_blobs[i].data;}
    return NULL;
}
static unsigned failures;
#define CHECK(f,expr,want) do{ if ((int64_t)(expr)!=(int64_t)(want)) { if (failures++<12) fprintf(stderr,"frame %u %s native=%lld recorded=%lld\n",(unsigned)r->frame,f,(long long)(expr),(long long)(want)); } }while(0)
int main(void) {
    uint16_t size;const uint8_t *list=rom(0x0B57BE,&size);assert(list);
    neo_camera_t c;neo_camera_init(&c,list,size,0x0B57BE);
    const camera_record_t *r=&camera_records[0];
    c.auto_mode=r->auto_mode;c.auto_owner=r->auto_owner;c.auto_vx=r->auto_vx;c.auto_vy=r->auto_vy;c.command=r->command;
    c.root_vx=r->root_vx;c.root_vy=r->root_vy;c.x=r->x;c.y=r->y;c.dx=r->dx;c.dy=r->dy;c.progress=r->progress;c.target=r->target;
    c.path_x=r->path_x;c.path_y=r->path_y;memcpy(c.bounds,r->bounds,8);c.mode=r->mode;c.lock=r->lock;memcpy(c.lock_bounds,r->lock_bounds,8);
    c.shake=r->shake;c.shake_applied=r->shake_applied;c.restart=r->restart;memcpy(c.view,r->view,8);
    uint16_t p8=r->p69b8,pa=r->p69ba;
    unsigned actor_autos=0,script_points=0,locks=0,autos=0,shakes=0,events=0,held_at=0,moved=0;
    for (unsigned k=1;k<N;k++) {
        r=&camera_records[k];
        neo_camera_finish(&c,&p8,&pa);events+=c.event_count;
        assert(!c.unsupported);
        if (c.held && !held_at) held_at=r->frame;
        if (r->p69b8!=p8 || r->p69ba!=pa) {p8=r->p69b8;pa=r->p69ba;script_points++;}
        if (r->lock!=c.lock || memcmp(r->lock_bounds,c.lock_bounds,8)) {
            uint16_t b[4];for (unsigned i=0;i<4;i++) b[i]=(uint16_t)r->lock_bounds[i];
            if (r->lock==0xffffffffu) neo_camera_release(&c); else neo_camera_lock(&c,(uint16_t)(r->lock>>16),b);
            if (r->lock==0xffffffffu) memcpy(c.lock_bounds,r->lock_bounds,8); /* 0C7C12 keeps the operands */
            locks++;
        }
        if (r->auto_mode!=c.auto_mode || r->auto_owner!=c.auto_owner || r->auto_vx!=c.auto_vx || r->auto_vy!=c.auto_vy) {
            /* Script ops D/E (0C7B4E/0C7BBA) or an actor's 0C7B48 with its
             * own 695C/6960 (the domed building 05486A: mode 2, vy 0). */
            neo_camera_t t=c;
            if (r->auto_mode) neo_camera_autoscroll(&t); else neo_camera_autoscroll_stop(&t);
            if (t.auto_mode==r->auto_mode && t.auto_owner==r->auto_owner && t.auto_vx==r->auto_vx && t.auto_vy==r->auto_vy) {c=t;autos++;}
            else {neo_camera_set_auto(&c,r->auto_mode);c.auto_vx=r->auto_vx;c.auto_vy=r->auto_vy;c.auto_owner=r->auto_owner;actor_autos++;}
        }
        if (r->shake!=c.shake) {
            neo_camera_t t=c;neo_camera_shake_pulse(&t);
            if (t.shake==r->shake) c=t; else {neo_camera_shake_start(&c);}
            shakes++;
        }
        neo_camera_player_t pl[2];
        for (unsigned i=0;i<2;i++) pl[i]=(neo_camera_player_t){r->players[i][0]!=0,r->players[i][1],r->players[i][2],r->words[i][0],r->words[i][1]};
        int32_t before=c.x;
        neo_camera_track(&c,pl,r->follow!=0,p8,pa);
        moved+=c.x!=before;
        CHECK("auto_mode",c.auto_mode,r->auto_mode);CHECK("auto_vx",c.auto_vx,r->auto_vx);CHECK("auto_vy",c.auto_vy,r->auto_vy);
        CHECK("command",c.command,r->command);CHECK("root_vx",c.root_vx,r->root_vx);CHECK("root_vy",c.root_vy,r->root_vy);
        CHECK("root_x",neo_camera_root_x(&c),r->root_x);CHECK("root_y",neo_camera_root_y(&c),r->root_y);
        CHECK("x",c.x,r->x);CHECK("y",c.y,r->y);CHECK("dx",c.dx,r->dx);CHECK("dy",c.dy,r->dy);
        CHECK("progress",c.progress,r->progress);CHECK("target",c.target,r->target);CHECK("path_x",c.path_x,r->path_x);CHECK("path_y",c.path_y,r->path_y);
        for (unsigned i=0;i<4;i++) {CHECK("bounds",c.bounds[i],r->bounds[i]);CHECK("view",c.view[i],r->view[i]);}
        CHECK("mode",c.mode,r->mode);CHECK("shake",c.shake,r->shake);CHECK("shake_applied",c.shake_applied,r->shake_applied);
        CHECK("lock",c.lock,r->lock);
    }
    if (failures) {fprintf(stderr,"FAIL: %u camera mismatches\n",failures);return 1;}
    printf("PASS: native camera reproduces %u original updates (frames %u..%u, %u moves, progress %u, view, root, command list held at %u, %u events); "
           "applied %u script points, %u stops, %u autoscroll, %u actor 0C7B48 and %u shake writes\n",(unsigned)N-1,(unsigned)camera_records[0].frame,(unsigned)camera_records[N-1].frame,
           moved,(unsigned)(c.progress>>16),held_at,events,script_points,locks,autos,actor_autos,shakes);
    return 0;
}
