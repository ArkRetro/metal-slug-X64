/* Closed-loop test of a generic-actor family (src/neo_*.c through
 * neo_region_tick) against a MAME family trace compiled by
 * tools/compile_family_fixture.py (FAMILY_HEADER, FAMILY_NAME).
 *
 * The root node starts from its recorded bytes and is never reloaded. Every
 * other node exists only when the native code creates it: a native spawn is
 * bound to the next recorded node with the same creator and first routine,
 * and starts from that node's first recorded bytes (the allocator's fields
 * and what the creator wrote before it runs; the native creator's own writes
 * then go on top). Children outside the family (effects, debris, soldier
 * deaths) go to a scratch node and are counted.
 *
 * Inputs per update are the RAM words the original read (scene root; the
 * layer objects 10C72E/10D7B6/10E8C6 hold the same position in Mission 1,
 * checked in the RAM dumps), player 1, script flags, layer words, epoch, RNG cursor and ring).
 * After every update the routine and the node bytes +10..+AF must match,
 * except the allocator's palette slot numbers (+14, +16, +19..+1C: native
 * handles), the animation pointer +20..+23 (native program/cursor) and the
 * cached position +44..+4B; block tags +0C/+38 are compared through the node
 * mapping. A deleting update must delete. The watched globals (camera mode
 * and vy 10E95A..10E963, shake 10E9C8, kill-all 1067E3, flash 109944,
 * player lock 10E9E3 and handlers 10E9F8..10E9FF) must hold after every
 * update the bytes the original wrote. */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#define MSX_SOLDIER_PROGRAMS 1
#include "../src/neo_region.h"
#include "../src/neo_soldier_ai.h"
#include "soldier_ai_tables.h"
typedef struct { int creator; uint32_t routine,frame; uint8_t pre[0xb0]; } family_node_t;
typedef struct {
    uint32_t frame; int node; uint32_t routine; int has_post; uint8_t post[0xb0];
    int root_mask; int32_t root[4]; int corr;
    int tmask; int32_t terrain[4]; int tcorr;
    int32_t p1x,p1y,signal,group,front,epoch,cursor,progress; int16_t groups[9];
    uint16_t ring[64][2]; uint32_t writes[12][2]; uint8_t ext[16][2];
} family_update_t;
#include FAMILY_HEADER
#define STR2(a) #a
#define STR(a) STR2(a)
#define CAT2(a,b) a##b
#define CAT(a,b) CAT2(a,b)
#define NODES CAT(FAMILY_NAME,_nodes)
#define UPDATES CAT(FAMILY_NAME,_updates)
enum { NN=sizeof(NODES)/sizeof(NODES[0]), NU=sizeof(UPDATES)/sizeof(UPDATES[0]) };
static neo_gactor_t actors[NN+1];            /* [NN] = scratch */
static bool bound[NN];
static unsigned externals,draws;
static neo_contact_parent_t root,terrain;
static bool terrain_seen;
static neo_ga_flags_t flags;
static uint8_t ram[0x200];                   /* globals, see slot() */
static uint8_t expect[0x200];
static int slot(uint32_t ad) {
    if (ad>=0x10e95a && ad<0x10e964) return (int)(ad-0x10e95a);
    if (ad>=0x10e9c8 && ad<0x10e9d0) return 0x10+(int)(ad-0x10e9c8);
    if (ad>=0x1067e2 && ad<0x1067e4) return 0x20+(int)(ad-0x1067e2);
    if (ad>=0x109944 && ad<0x109946) return 0x30+(int)(ad-0x109944);
    if (ad>=0x10e9e2 && ad<0x10e9e4) return 0x40+(int)(ad-0x10e9e2);
    if (ad>=0x10e9f8 && ad<0x10ea00) return 0x50+(int)(ad-0x10e9f8);
    return -1;
}
static void put(uint32_t ad,uint8_t v) {int s=slot(ad);if (s>=0) ram[s]=v;}
static void camera_mode(void *ctx,uint8_t m) {(void)ctx;put(0x10e95a,m);}
static void camera_rise(void *ctx,int32_t vy) {(void)ctx;for (unsigned i=0;i<4;i++) put(0x10e960+i,(uint8_t)((uint32_t)vy>>(24-8*i)));}
static void shake(void *ctx) {(void)ctx;put(0x10e9c8,0);put(0x10e9c9,8);}
static int front_value;
static bool front_layer(void *ctx) {(void)ctx;return front_value>0;}
static bool swapped;
static bool map_swap(void *ctx) {(void)ctx;swapped=true;return true;}
static int index_of(const neo_gactor_t *a) {return (int)(a-actors);}
static bool parent(void *ctx,uint32_t tag,neo_contact_parent_t *out) {
    (void)ctx;
    if (tag==0x10e968u) {*out=root;return true;}
    /* Layer objects (the world keeps them with the terrain). */
    if (tag==0x10c72eu || tag==0x10d7b6u || tag==0x10e8c6u) {*out=terrain_seen?terrain:root;out->tag=tag;return true;}
    if ((tag&0xff000000u)==NEO_GA_TAG_BASE && (tag&0xffffu)<NN) {
        const neo_gactor_t *a=&actors[tag&0xffffu];
        *out=(neo_contact_parent_t){tag,(int32_t)ga_l(a,0x3c),(int32_t)ga_l(a,0x40),(int32_t)ga_l(a,0x44),(int32_t)ga_l(a,0x48),ga_b(a,0x4d)};
        return true;
    }
    return false;
}
static const neo_anim_program_t *program(uint32_t tag) {
    for (unsigned i=0;i<sizeof(soldier_programs)/sizeof(soldier_programs[0]);i++) if (soldier_programs[i].tag==tag) return &soldier_programs[i].program;
    return NULL;
}
/* Recorded node address -> index (the +0C/+38 of the recorded bytes). */
static uint32_t node_address[NN];
static uint32_t native_tag(uint32_t recorded) {
    for (unsigned i=0;i<NN;i++) if (node_address[i] && (node_address[i]==recorded || node_address[i]+0x28u==recorded)) return NEO_GA_TAG(i);
    return recorded;
}
static neo_gactor_t *spawn(void *ctx,neo_gactor_t *creator,uint32_t routine,bool after_script) {
    (void)ctx;(void)after_script;
    int c=creator?index_of(creator):-1;
    /* 0040BC inserts right after the creator: children created in one
     * update appear in the list in reverse order of creation. */
    int pick=-1;
    if (c>=0 && c<(int)NN) for (unsigned i=0;i<NN;i++) {
        if (bound[i] || NODES[i].creator!=c || NODES[i].routine!=routine) continue;
        if (pick<0 || NODES[i].frame<NODES[pick].frame) pick=(int)i;
        else if (NODES[i].frame==NODES[pick].frame) pick=(int)i;
    }
    if (pick>=0) {
        unsigned i=(unsigned)pick;bound[i]=true;neo_gactor_t *a=&actors[i];memset(a,0,sizeof(*a));a->active=true;a->index=(int16_t)i;
        memcpy(a->m,NODES[i].pre,0xb0);
        ga_sl(a,0x0c,NEO_GA_TAG((unsigned)c));ga_sl(a,0x38,native_tag(ga_l(a,0x38)));
        return a;
    }
    externals++;
    neo_gactor_t *s=&actors[NN];memset(s,0,sizeof(*s));s->active=true;s->index=(int16_t)NN;ga_sl(s,0,routine);
    if (creator) ga_sl(s,0x0c,NEO_GA_TAG((unsigned)c));
    return s;
}
static neo_gactor_t *creator_of(void *ctx,neo_gactor_t *a) {
    (void)ctx;uint32_t t=ga_l(a,0x0c);
    if ((t&0xff000000u)==NEO_GA_TAG_BASE && (t&0xffffu)<NN) return &actors[t&0xffffu];
    return NULL;
}
/* 004192: the nodes deeper than `a` (its descendants) are deleted. */
static unsigned killed_children;
static void kill_children(void *ctx,neo_gactor_t *a) {
    (void)ctx;
    for (unsigned i=0;i<NN;i++) {
        if (!actors[i].active || &actors[i]==a) continue;
        unsigned depth=0;
        for (neo_gactor_t *p=creator_of(NULL,&actors[i]);p && depth<NN;p=creator_of(NULL,p),depth++)
            if (p==a) {actors[i].active=false;killed_children++;break;}
    }
}
static bool creator_alive(void *ctx,neo_gactor_t *a) {(void)ctx;neo_gactor_t *c=creator_of(NULL,a);return !c || c->active;}
/* 0049E8 against the player (class 0): the player's hurtbox is not in the
 * trace; a contact is taken from the update's outcome (the original deletes
 * the attacker in that update). */
static bool contact;
static unsigned contacts;
static uint8_t attack(void *ctx,neo_gactor_t *a,const int16_t row[7]) {(void)ctx;(void)a;(void)row;if (contact) contacts++;return contact?1:0;}
/* 0140C0/013F1E: slot of the dynamic collision object (+6A), from 7 down;
 * no other actor of the trace holds one. */
static bool dyn_used[NEO_KDYN_COUNT];
static void dyn_set(void *ctx,neo_gactor_t *a,uint32_t map,int16_t dx,int16_t dy) {
    (void)ctx;(void)map;(void)dx;(void)dy;
    if (ga_b(a,0x6a)!=0xffu) return;
    for (int k=NEO_KDYN_COUNT-1;k>=0;k--) if (!dyn_used[k]) {dyn_used[k]=true;ga_sb(a,0x6a,(uint8_t)k);return;}
}
static void dyn_release(void *ctx,neo_gactor_t *a) {(void)ctx;if (ga_b(a,0x6a)<NEO_KDYN_COUNT) dyn_used[ga_b(a,0x6a)]=false;ga_sb(a,0x6a,0xff);}
static unsigned stamps;
static void stamp(void *ctx,neo_gactor_t *a,uint32_t t) {(void)ctx;(void)a;(void)t;stamps++;}
static void emit(void *ctx,const neo_anim_draw_t *d) {(void)ctx;(void)d;draws++;}
static bool skip(unsigned o) {return o==0x14 || o==0x16 || o==0x58 || o==0x59 || (o>=0x19 && o<=0x1c) || (o>=0x20 && o<=0x23) || (o>=0x44 && o<=0x4b);}
int main(void) {
    /* Node addresses are not in the fixture: a child's recorded +0C names
     * its creator. */
    for (unsigned i=0;i<NN;i++) if (NODES[i].creator>=0)
        node_address[NODES[i].creator]=(uint32_t)(NODES[i].pre[0x0c]<<24|NODES[i].pre[0x0d]<<16|NODES[i].pre[0x0e]<<8|NODES[i].pre[0x0f]);
    neo_rng_t rng;memset(&rng,0,sizeof(rng));
    static neo_kscene_t kernel;kernel.group_map=soldier_group_map;
    uint8_t groups[9]={0},flag606=0;
    neo_genv_t e;memset(&e,0,sizeof(e));
    e.kernel=&kernel;e.rng=&rng;e.parent=parent;e.program=program;e.emit=emit;e.groups=groups;e.flag606=&flag606;
    e.spawn=spawn;e.attack=attack;e.dyn_set=dyn_set;e.dyn_release=dyn_release;e.stamp=stamp;e.creator=creator_of;e.creator_alive=creator_alive;
    e.bounds=(neo_cull_bounds_t){false,0,0,0,0,0,256,320,512};
    e.view[0]=0;e.view[1]=319;e.view[2]=256;e.view[3]=511;
    e.players.mode88=2;e.players.rank=3;e.players.active_mask=1;e.players.targets_valid=true;
    e.flags=&flags;e.terrain_tag=0x10d7b6u;   /* both families lie past camera 1023 */
    e.kill_children=kill_children;e.camera_mode=camera_mode;e.camera_rise=camera_rise;e.shake=shake;e.map_swap=map_swap;e.front_layer=front_layer;
    bound[0]=true;memcpy(actors[0].m,NODES[0].pre,0xb0);actors[0].active=true;actors[0].index=0;
    ga_sl(&actors[0],0x38,native_tag(ga_l(&actors[0],0x38)));
    unsigned compared=0,deleted=0;uint32_t kill_frame=0,kill_at=0;uint8_t epoch=1;int32_t p1x=0,p1y=0;
    for (unsigned k=0;k<NU;k++) {
        const family_update_t *u=&UPDATES[k];
        neo_gactor_t *a=&actors[u->node];
        if (!bound[u->node] || !a->active) {fprintf(stderr,"frame %u node %d (%05x) not created natively\n",u->frame,u->node,u->routine);return 1;}
        if (ga_l(a,0)!=u->routine) {fprintf(stderr,"frame %u node %d routine %05x expected %05x\n",u->frame,u->node,ga_l(a,0),u->routine);return 1;}
        if (u->root_mask&1) root.x=u->root[0];
        if (u->root_mask&2) root.y=u->root[1];
        if (u->root_mask&4) root.previous_x=u->root[2];
        if (u->root_mask&8) root.previous_y=u->root[3];
        root.tag=0x10e968u;if (u->corr>=0) root.correction=(uint8_t)u->corr;
        if (u->p1x>=0) p1x=u->p1x;
        if (u->p1y>=0) p1y=u->p1y;
        if (u->epoch>=0) epoch=(uint8_t)u->epoch;
        if (u->progress>=0) e.camera=(uint16_t)u->progress;
        for (unsigned g=0;g<9;g++) if (u->groups[g]>=0) groups[g]=(uint8_t)u->groups[g];
        if (u->signal>=0) {flags.dome_signal=(uint8_t)u->signal;flags.dome_group=(uint8_t)u->group;}
        front_value=u->front;
        if (u->cursor>=0) rng.cursor=(uint8_t)u->cursor;
        for (unsigned r=0;r<64;r++) if (u->ring[r][0]<NEO_RNG_WORDS) rng.words[u->ring[r][0]]=u->ring[r][1];
        /* Bytes other actors wrote into the node since its last update. */
        for (unsigned x=0;x<16 && u->ext[x][0];x++) a->m[u->ext[x][0]]=u->ext[x][1];
        /* Static collision objects (the world keeps terrain and root
         * together). */
        kernel.root=(neo_kobj_t){root.x,root.y,root.previous_x,root.previous_y};
        if (u->tmask&1) terrain.x=u->terrain[0];
        if (u->tmask&2) terrain.y=u->terrain[1];
        if (u->tmask&4) terrain.previous_x=u->terrain[2];
        if (u->tmask&8) terrain.previous_y=u->terrain[3];
        if (u->tmask&16) terrain.correction=(uint8_t)u->tcorr;
        if (u->tmask) terrain_seen=true;
        kernel.terrain=terrain_seen?(neo_kobj_t){terrain.x,terrain.y,terrain.previous_x,terrain.previous_y}:kernel.root;
        e.epoch=epoch;e.players.p1x=(uint16_t)p1x;e.players.p1y=(uint16_t)p1y;
        /* RAM 1067E2 holds 1067E3 for the logic update after the request. */
        if (kill_frame && !kill_at && u->frame>kill_frame) kill_at=u->frame;
        e.kill_all=kill_at && u->frame==kill_at;
        uint8_t kill_before=flags.kill_request;
        contact=!u->has_post;
        ga_result_t r=neo_region_tick(a,&e);
        if (r==GA_UNSUPPORTED || e.unsupported) {fprintf(stderr,"frame %u node %d %05x unsupported %05x\n",u->frame,u->node,u->routine,(unsigned)e.unsupported_at);return 1;}
        /* Flags the family writes through neo_ga_flags_t. */
        if (flags.kill_request!=kill_before) {put(0x1067e3,flags.kill_request);if (!kill_frame) kill_frame=u->frame;}
        put(0x10e9e3,flags.player_lock);
        for (unsigned h=0;h<2;h++) {uint32_t v=flags.player_handlers?0x00054b18u:0;for (unsigned i=0;i<4;i++) put(0x10e9f8+4*h+i,(uint8_t)(v>>(24-8*i)));}
        put(0x109944,flags.flash);
        for (unsigned w=0;w<12 && u->writes[w][0];w++) {int s=slot(u->writes[w][0]);if (s>=0) expect[s]=(uint8_t)u->writes[w][1];}
        /* 109945 is the palette allocator's (not a family global). */
        ram[0x31]=expect[0x31];
        if (memcmp(ram,expect,sizeof(ram))) {
            for (unsigned s=0;s<sizeof(ram);s++) if (ram[s]!=expect[s]) fprintf(stderr,"frame %u %05x global slot %02x = %02x expected %02x\n",u->frame,u->routine,s,ram[s],expect[s]);
            return 1;
        }
        if (!u->has_post) {
            if (r!=GA_DELETE && r!=GA_DELETE_TREE) {fprintf(stderr,"frame %u node %d %05x: original deletes, native result %d\n",u->frame,u->node,u->routine,r);return 1;}
            a->active=false;deleted++;continue;
        }
        if (r!=GA_MORE) {fprintf(stderr,"frame %u node %d %05x: native result %d, original keeps it\n",u->frame,u->node,u->routine,r);return 1;}
        if (ga_l(a,0)!=(uint32_t)(u->post[0]<<24|u->post[1]<<16|u->post[2]<<8|u->post[3])) {
            fprintf(stderr,"frame %u node %d %05x -> %05x expected %02x%02x%02x%02x\n",u->frame,u->node,u->routine,ga_l(a,0),u->post[0],u->post[1],u->post[2],u->post[3]);return 1;
        }
        uint32_t t38=native_tag((uint32_t)(u->post[0x38]<<24|u->post[0x39]<<16|u->post[0x3a]<<8|u->post[0x3b]));
        for (unsigned o=0x10;o<0xb0;o++) {
            if (skip(o)) continue;
            uint8_t want=(o>=0x38 && o<0x3c)?(uint8_t)(t38>>(24-8*(o-0x38))):u->post[o];
            if (a->m[o]!=want) {fprintf(stderr,"frame %u node %d routine %05x +%02X=%02x expected %02x\n",u->frame,u->node,u->routine,o,a->m[o],want);return 1;}
            compared++;
        }
    }
    for (unsigned i=0;i<NN;i++) if (!bound[i]) {fprintf(stderr,"node %u (%05x) never created\n",i,NODES[i].routine);return 1;}
    printf("PASS: family %s: %u nodes, %u closed-loop updates (frames %u..%u), %u bytes, %u deletions, %u draws, %u player contacts, %u collision stamps, %u children outside the family, %u deleted by 004192%s\n",
        STR(FAMILY_NAME),(unsigned)NN,(unsigned)NU,UPDATES[0].frame,UPDATES[NU-1].frame,compared,deleted,draws,contacts,stamps,externals,killed_children,swapped?", map swap requested":"");
    return 0;
}
