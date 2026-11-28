/* Mission 6 camera and layer objects, closed loop (docs/MISION6.md), one
 * build per stage (-DM6_STAGE=0..2 with that stage's fixture as
 * m6_camera_fixture.h): the native camera (src/neo_camera.c) walks the
 * stage's own command list (0BCA00 the coast and the bridge, 0BDCE2 the factory,
 * 0BE99E the last arena; the pack's blob) with the inputs of one MAME play
 * (data/m6s<k>_camera_fixture.json.gz: tools/capture_m6.sh, MSX_FULL_CAMERA=1
 * from the section states of run4, every camera update of the stage loop
 * 0186A0), as tools/test_m4_camera.c does for Mission 4; and the layer
 * objects 10C72E / 10D7B6 / 10E83E / 10E8C6 (src/neo_train.c neo_layers_*)
 * must hold MAME's +0, +4, +26, +28, +2A, +2B after every update. Writes of
 * other actors (script points, stops, autoscroll, shakes, layer flags) are
 * applied as recorded and counted. Private input: romfs/mission6.pack. */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/neo_camera.h"
#include "../src/neo_train.h"
#include "m6_camera_fixture.h"
#ifndef M6_SOURCE
#define M6_SOURCE "run4 replay"   /* -DM6_SOURCE="\"TAS\"" with data/m6tas<k>_camera_fixture.json.gz (tools/capture_m6_tas.sh: no memory writes) */
#endif
#ifndef M6_STAGE
#error "M6_STAGE"
#endif
static const uint32_t m6_descriptor[3]={0x016E5Eu,0x016E6Eu,0x016E7Eu},m6_list[3]={0x0BCA00u,0x0BDCE2u,0x0BE99Eu};
#define N (sizeof(camera_records)/sizeof(camera_records[0]))
static unsigned rd16(const uint8_t *p) {return (unsigned)p[0]<<8|p[1];}
static uint32_t rd32(const uint8_t *p) {return (uint32_t)p[0]<<24|(uint32_t)p[1]<<16|(uint32_t)p[2]<<8|p[3];}
static unsigned failures;
#define CHECK(f,expr,want) do{ if ((int64_t)(expr)!=(int64_t)(want)) { if (failures++<12) fprintf(stderr,"frame %u %s native=%lld recorded=%lld\n",(unsigned)r->frame,f,(long long)(expr),(long long)(want)); } }while(0)
int main(int argc,char **argv) {
    FILE *f=fopen(argc>1?argv[1]:"romfs/mission6.pack","rb");
    if (!f) {printf("SKIP: Mission 6 camera (no pack)\n");return 0;}
    fseek(f,0,SEEK_END);long n=ftell(f);fseek(f,0,SEEK_SET);
    uint8_t *pk=malloc((size_t)n);assert(pk && fread(pk,1,(size_t)n,f)==(size_t)n);fclose(f);
    assert(!memcmp(pk,"MSXM",4) && rd16(pk+4)>=4 && rd16(pk+6)==6 && rd16(pk+8)==3);
    const uint8_t *st=pk+48+128*M6_STAGE;assert(rd32(st)==m6_descriptor[M6_STAGE]);
    uint32_t lrom=rd32(st+44),off=rd32(st+48),len=rd32(st+52);assert(lrom==m6_list[M6_STAGE] && off+len<=(uint32_t)n);
    const uint8_t *list=pk+off;uint16_t size=(uint16_t)len;
    neo_camera_t c;neo_camera_init(&c,list,size,m6_list[M6_STAGE]);
    neo_layer_t L[NEO_LAYERS];memset(L,0,sizeof(L));unsigned layer_ops=0,flag_writes=0,speed_writes=0,owned=0,layer_checks=0;
    const camera_record_t *r=&camera_records[0];
    c.auto_mode=r->auto_mode;c.auto_owner=r->auto_owner;c.auto_vx=r->auto_vx;c.auto_vy=r->auto_vy;c.command=r->command;
    c.root_vx=r->root_vx;c.root_vy=r->root_vy;c.x=r->x;c.y=r->y;c.dx=r->dx;c.dy=r->dy;c.progress=r->progress;c.target=r->target;
    c.path_x=r->path_x;c.path_y=r->path_y;memcpy(c.bounds,r->bounds,8);c.mode=r->mode;c.lock=r->lock;memcpy(c.lock_bounds,r->lock_bounds,8);
    c.shake=r->shake;c.shake_applied=r->shake_applied;c.restart=r->restart;memcpy(c.view,r->view,8);
    uint16_t p8=r->p69b8,pa=r->p69ba;
    for (unsigned i=0;i<NEO_LAYERS;i++) L[i]=(neo_layer_t){.x=r->layers[i].x,.y=r->layers[i].y,.fx=r->layers[i].fx,.fy=r->layers[i].fy,.frac_x=r->layers[i].frac_x,.frac_y=r->layers[i].frac_y};
    for (unsigned i=0;i<4;i++) c.layer_flags[i]=(uint8_t)(r->layer_flags>>(24-8*i));
    c.speed_x=r->speed[0];c.speed_y=r->speed[1];
    unsigned arrivals=0,actor_autos=0,script_points=0,locks=0,autos=0,shakes=0,events=0,held_at=0,moved=0;
    for (unsigned k=1;k<N;k++) {
        r=&camera_records[k];
        /* 0C81C0: the layers step with the velocity of the camera update,
         * before the list's commands of that update. */
        neo_layers_step(L,c.root_vx,c.root_vy,c.layer_flags);
        {   /* Op 4 (0C84BE) of the previous update's command pass reads the players as that update saw them. */
            const camera_record_t *q=&camera_records[k-1];
            for (unsigned i=0;i<2;i++) {c.op4_players[i].alive=q->players[i][0]!=0;c.op4_players[i].px=q->words[i][0];c.op4_players[i].py=q->words[i][1];
                                        c.op4_players[i].rect_ok=!(q->pflags[i][0]&0x80u) && (q->pflags[i][1]&0x40u);}
        }
        neo_camera_finish(&c,&p8,&pa);events+=c.event_count;
        for (unsigned i=0;i<c.event_count;i++) {
            const neo_camera_event_t *e=&c.events[i];
            if (e->kind==0x44u || e->kind==0x1cu || e->kind==0x18u) layer_ops++;
            neo_layers_event(L,c.list,e->kind,e->address,e->at,neo_camera_root_x(&c),neo_camera_root_y(&c));
        }
        for (unsigned i=0;i<4;i++) {uint8_t v=(uint8_t)(r->layer_flags>>(24-8*i));if (v!=c.layer_flags[i]) {c.layer_flags[i]=v;flag_writes++;}}
        if (r->speed[0]!=c.speed_x || r->speed[1]!=c.speed_y) {c.speed_x=r->speed[0];c.speed_y=r->speed[1];speed_writes++;}
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
            else {
                /* 04445A (an actor of the last stage, phase 2: the camera glides to its point +86 / +88 at +8A / +8C << 8):
                 * on the update that would pass the point it stores the point itself in 698E / 6992 and clears 695C /
                 * 6960. The stored word is the actor's: taken as recorded and counted. */
                if ((c.auto_mode&1u) && c.auto_vx && !r->auto_vx && r->auto_mode==c.auto_mode) {neo_camera_place_x(&c,r->x);arrivals++;}
                if ((c.auto_mode&2u) && c.auto_vy && !r->auto_vy && r->auto_mode==c.auto_mode) {neo_camera_place_y(&c,r->y);arrivals++;}
                neo_camera_set_auto(&c,r->auto_mode);c.auto_vx=r->auto_vx;c.auto_vy=r->auto_vy;c.auto_owner=r->auto_owner;actor_autos++;
            }
        }
        if (r->shake!=c.shake) {
            neo_camera_t t=c;neo_camera_shake_pulse(&t);
            if (t.shake==r->shake) c=t; else {neo_camera_shake_start(&c);}
            shakes++;
        }
        neo_camera_player_t pl[2];
        for (unsigned i=0;i<2;i++) pl[i]=(neo_camera_player_t){r->players[i][0]!=0,r->players[i][1],r->players[i][2],r->words[i][0],r->words[i][1],!(r->pflags[i][0]&0x80u) && (r->pflags[i][1]&0x40u)};
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
        for (unsigned i=0;i<NEO_LAYERS;i++) {
            /* A layer with its flag byte set, or whose map an actor loaded, is moved by its owner. */
            if (c.layer_flags[i]) {L[i]=(neo_layer_t){.x=r->layers[i].x,.y=r->layers[i].y,.fx=r->layers[i].fx,.fy=r->layers[i].fy,.frac_x=r->layers[i].frac_x,.frac_y=r->layers[i].frac_y};owned++;continue;}
            CHECK("layer x",L[i].x,r->layers[i].x);CHECK("layer y",L[i].y,r->layers[i].y);CHECK("layer fx",L[i].fx,r->layers[i].fx);CHECK("layer fy",L[i].fy,r->layers[i].fy);
            CHECK("layer frac x",L[i].frac_x,r->layers[i].frac_x);CHECK("layer frac y",L[i].frac_y,r->layers[i].frac_y);layer_checks++;
        }
    }
    if (failures) {fprintf(stderr,"FAIL: %u camera mismatches\n",failures);return 1;}
    printf("PASS: Mission 6 stage %u layer objects (" M6_SOURCE "): %u layer states equal (x, y, factors, remainders), %u layer ops of the list, %u states of layers moved by their owner, %u flag and %u speed writes applied\n",M6_STAGE+1u,layer_checks,layer_ops,owned,flag_writes,speed_writes);
    printf("PASS: Mission 6 stage %u camera list %06lX (" M6_SOURCE "): native camera reproduces %u original updates (frames %u..%u, %u moves, progress %u, view, root, command list held at %u, %u events); "
           "applied %u script points, %u stops, %u autoscroll, %u actor 0C7B48 (%u arrivals of 04445A) and %u shake writes\n",M6_STAGE+1u,(unsigned long)m6_list[M6_STAGE],(unsigned)N-1,(unsigned)camera_records[0].frame,(unsigned)camera_records[N-1].frame,
           moved,(unsigned)(c.progress>>16),held_at,events,script_points,locks,autos,actor_autos,arrivals,shakes);
    return 0;
}
