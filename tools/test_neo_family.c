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
#include "../src/neo_collision.h"
#include "../src/neo_soldier_ai.h"
#include "soldier_ai_tables.h"
typedef struct { int creator; uint32_t routine,frame; uint8_t pre[0xb0]; } family_node_t;
typedef struct {
    uint32_t frame; int node; uint32_t routine; int has_post; uint8_t post[0xb0];
    int root_mask; int32_t root[4]; int corr;
    int tmask; int32_t terrain[4]; int tcorr;
    int32_t p1x,p1y,signal,group,front,epoch,cursor,progress; int16_t groups[9]; uint16_t dyn[48][3];
    uint16_t ring[64][2]; uint32_t writes[12][2]; uint8_t ext[16][2]; int32_t boxes[8]; int32_t kill; int32_t w4068;
    int marker_count; int32_t markers[8][4];   /* object list read (040200/0402E6): x, y, height, id */
    int ram_in_first,ram_in_count,ram_out_first,ram_out_count;  /* FAMILY_RAM inputs/outputs */
    int32_t view[4];   /* RAM 10E9D2..10E9D8 as read (0C8BEC), -1: not read */
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
static uint8_t dynram[0x224];             /* RAM 10C06E..10C291 as read */
static bool terrain_seen;
static neo_ga_flags_t flags;
static uint8_t ram[0x200];                   /* globals, see slot() */
/* RAM 10C06E..10C291 as read by the original (dynamic collision objects of
 * actors outside the family, e.g. the wagon under its swordsmen). */
static bool dyn_seen;
static void dyn_inputs(neo_kscene_t *k,const family_update_t *u) {
    for (unsigned i=0;i<48 && u->dyn[i][0]!=0xffffu;i++) {
        unsigned o=u->dyn[i][0];uint16_t m=u->dyn[i][1],v=u->dyn[i][2];
        if (m&0xff00u) dynram[o]=(uint8_t)(v>>8);
        if (m&0x00ffu) dynram[o+1]=(uint8_t)v;
        dyn_seen=true;
    }
    if (!dyn_seen) return;
    /* Native objects of the family's own dyn_set keep their slots (none of
     * the recorded families mixes both). */
    for (unsigned i=0;i<NEO_KDYN_COUNT;i++) {
        const uint8_t *b=&dynram[i*64];
        #define L32(p) ((uint32_t)((uint32_t)(p)[0]<<24|(p)[1]<<16|(p)[2]<<8|(p)[3]))
        uint32_t map=L32(b+0x26);
        neo_kdyn_t *d=&k->dyn[i];
        d->map=map?ga_rom(map):NULL;
        d->pos=(neo_kobj_t){(int32_t)L32(b+0x14),(int32_t)L32(b+0x18),(int32_t)L32(b+0x1c),(int32_t)L32(b+0x20)};
        for (unsigned j=0;j<4;j++) d->bbox[j]=(int32_t)L32(b+0x2c+4*j);
        d->countdown=(uint16_t)(b[0x2a]<<8|b[0x2b]);
        d->chain[0]=d->chain[1]=d->chain[2]=0;
        #undef L32
    }
    memcpy(k->bins_x,&dynram[0x200],18);memcpy(k->bins_y,&dynram[0x212],16);k->solid=dynram[0x222];
}
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
    /* Dynamic collision objects as read (dyn_inputs). */
    if (tag>=0x10c06eu && tag<0x10c06eu+64u*NEO_KDYN_COUNT && !((tag-0x10c06eu)&63u)) {
        const uint8_t *b=&dynram[tag-0x10c06eu];
        #define L32(p) ((uint32_t)((uint32_t)(p)[0]<<24|(p)[1]<<16|(p)[2]<<8|(p)[3]))
        *out=(neo_contact_parent_t){tag,(int32_t)L32(b+0x14),(int32_t)L32(b+0x18),(int32_t)L32(b+0x1c),(int32_t)L32(b+0x20),b[0x25]};
        #undef L32
        return true;
    }
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
/* Recorded node address -> index (the +0C/+38 of the recorded bytes). An
 * address inside a node other than its block (+28) keeps the offset in bits
 * 16..23 (a turret's +6E into its vehicle). */
static uint32_t node_address[NN];
static uint32_t native_tag(uint32_t recorded) {
    for (unsigned i=0;i<NN;i++) if (node_address[i] && (node_address[i]==recorded || node_address[i]+0x28u==recorded)) return NEO_GA_TAG(i);
    for (unsigned i=0;i<NN;i++) if (node_address[i] && recorded>node_address[i] && recorded<node_address[i]+0xc0u) return NEO_GA_TAG(i)|((recorded-node_address[i])<<16);
    return recorded;
}
static uint16_t node_word(void *ctx,const neo_gactor_t *a) {(void)ctx;return a->index>=0 && a->index<(int)NN?(uint16_t)node_address[a->index]:0;}
#ifdef FAMILY_RAM
/* RAM the family reads and writes outside its nodes (vehicle slots, input
 * words, weapon block, player nodes, RAM 10000E): an image of 100000..10FFFF
 * with the bytes each update read; the native writes must be the
 * original's. */
#define RAMIN CAT(FAMILY_NAME,_ram_in)
#define RAMOUT CAT(FAMILY_NAME,_ram_out)
static uint8_t wram[0x10000],wexpect[0x10000];
static const uint32_t ram_ranges[][2]=FAMILY_RAM;
static neo_vslots_t vslots;static neo_vehicle_env_t venv;
static uint16_t w16(uint32_t ad) {return (uint16_t)(wram[ad&0xffffu]<<8|wram[(ad+1)&0xffffu]);}
static uint32_t w32(uint32_t ad) {return (uint32_t)w16(ad)<<16|w16(ad+2);}
static void ram_to_env(marco_weapons_t *weapons) {
    memcpy(vslots.slot[0].b,&wram[0xc608],0x24);memcpy(vslots.slot[1].b,&wram[0xc62c],0x24);
    vslots.record[0]=w32(0x10c2e8);vslots.record[1]=w32(0x10c2ec);
    for (unsigned p=0;p<2;p++) {
        uint32_t n=p?0x101574u:0x1014c4u;
        venv.player[p]=&wram[n&0xffffu];venv.exists[p]=w32(n)!=0x3f1eu;
        venv.input[p][0]=w16(0x10e9ecu+6u*p);venv.input[p][1]=w16(0x10e9eeu+6u*p);venv.repeat[p]=wram[0xf084+p];
    }
    venv.halted=wram[0xc301];venv.system=wram[0xc300];venv.dip10f000=wram[0xf000];venv.dip10f001=wram[0xf001];venv.solids=0;venv.slots=&vslots;
    /* The solid-body table 106756 (zero unless the fixture records it with
     * the bodies' nodes: --ram 106756-1067d6 and the node ranges). */
    for (unsigned i=0;i<16;i++) {
        uint32_t n=w32(0x106756u+8u*i);
        venv.solid_node[i]=n?&wram[n&0xffffu]:NULL;venv.solid_desc[i]=w32(0x10675au+8u*i);
        if (n) venv.solids++;
    }
    memcpy(weapons->b,&wram[0xc718],MARCO_WEAPON_BLOCK);
}
static void env_to_ram(const marco_weapons_t *weapons) {
    memcpy(&wram[0xc608],vslots.slot[0].b,0x24);memcpy(&wram[0xc62c],vslots.slot[1].b,0x24);
    for (unsigned r=0;r<2;r++) for (unsigned i=0;i<4;i++) wram[0xc2e8+4*r+i]=(uint8_t)(vslots.record[r]>>(24-8*i));
    memcpy(&wram[0xc718],weapons->b,MARCO_WEAPON_BLOCK);
}
#endif
/* Frame of the update being replayed: a child the original made shows up
 * within a few frames (0040BC: the same frame; 00411C: the next one). A
 * native spawn with no recorded node that soon is a child the original
 * could not make (node pool exhausted: the shell 047DE0 of the bomb at
 * heli2 frame 11363), counted as external. */
static uint32_t current_frame;
/* Frames after its creator's update in which a recorded child may first
 * appear (compile_family_fixture.py --spawn-window; default 8). With a
 * full node pool the original makes no child: a later recorded node is
 * another update's (the R family: 2). */
#ifndef FAMILY_SPAWN_WINDOW
#define FAMILY_SPAWN_WINDOW 8u
#endif
#define SPAWN_WINDOW FAMILY_SPAWN_WINDOW
static neo_gactor_t *spawn(void *ctx,neo_gactor_t *creator,uint32_t routine,bool after_script) {
    (void)ctx;
    int c=creator?index_of(creator):after_script?-2:-1;
    /* 0040BC inserts right after the creator: children created in one
     * update appear in the list in reverse order of creation. */
    int pick=-1;
    /* c = -2: after the script actor (00411C), the recorded creator is the
     * script actor (compile_family_fixture.py --script-actor). */
    if ((c>=0 && c<(int)NN) || c==-2) for (unsigned i=0;i<NN;i++) {
        if (bound[i] || NODES[i].creator!=c || NODES[i].routine!=routine || NODES[i].frame>current_frame+SPAWN_WINDOW) continue;   /* 0040BC: this update or the next frame (a later node is another update's child: the pool was full) */
        if (pick<0 || NODES[i].frame<NODES[pick].frame) pick=(int)i;
        else if (NODES[i].frame==NODES[pick].frame) pick=(int)i;
    }
    if (pick>=0) {
        unsigned i=(unsigned)pick;bound[i]=true;neo_gactor_t *a=&actors[i];memset(a,0,sizeof(*a));a->active=true;a->index=(int16_t)i;
        memcpy(a->m,NODES[i].pre,0xb0);
        if (c>=0) ga_sl(a,0x0c,NEO_GA_TAG((unsigned)c));
        ga_sl(a,0x38,native_tag(ga_l(a,0x38)));
        return a;
    }
    externals++;
    neo_gactor_t *s=&actors[NN];memset(s,0,sizeof(*s));s->active=true;s->index=(int16_t)NN;ga_sl(s,0,routine);
    if (creator) ga_sl(s,0x0c,NEO_GA_TAG((unsigned)c));
    return s;
}
static neo_gactor_t *actor_of(void *ctx,uint32_t t) {
    (void)ctx;
    if ((t&0xff000000u)==NEO_GA_TAG_BASE && (t&0xffffu)<NN && actors[t&0xffffu].active) return &actors[t&0xffffu];
    return NULL;
}
static neo_gactor_t *creator_of(void *ctx,neo_gactor_t *a) {
    (void)ctx;uint32_t t=ga_l(a,0x0c);
    if ((t&0xff000000u)==NEO_GA_TAG_BASE && (t&0xffffu)<NN) return &actors[t&0xffffu];
    return NULL;
}
/* 004192: the nodes deeper than `a` (its descendants) are deleted. */
static unsigned killed_children,injected;
static void kill_children(void *ctx,neo_gactor_t *a) {
    (void)ctx;
    for (unsigned i=0;i<NN;i++) {
        if (!actors[i].active || &actors[i]==a) continue;
        unsigned depth=0;
        for (neo_gactor_t *p=creator_of(NULL,&actors[i]);p && depth<NN;p=creator_of(NULL,p),depth++)
            if (p==a) {actors[i].active=false;killed_children++;break;}
    }
}
/* 00408C: the next node is deeper (the nodes after `a` are its descendants
 * in the list: any live descendant). */
static bool has_children(void *ctx,neo_gactor_t *a) {
    (void)ctx;
    for (unsigned i=0;i<NN;i++) {
        if (!actors[i].active || &actors[i]==a) continue;
        unsigned depth=0;
        for (neo_gactor_t *p=creator_of(NULL,&actors[i]);p && depth<NN;p=creator_of(NULL,p),depth++) if (p==a) return true;
    }
    return false;
}
static bool creator_alive(void *ctx,neo_gactor_t *a) {(void)ctx;neo_gactor_t *c=creator_of(NULL,a);return !c || c->active;}
/* 0049E8 against the player (class 0): the player's hurtbox is not in the
 * trace; a contact is taken from the update's outcome (the original deletes
 * the attacker in that update). */
static bool contact;
static unsigned contacts;
#ifdef FAMILY_ATTACKS
/* The recorded 0049E8 answers of the update, in call order: D0 and the
 * A0/A1 targets as 01B162 read them (compile_family_fixture.py --attacks). */
#define ATTACKS CAT(FAMILY_NAME,_attacks)
#define ATTACK_SPANS CAT(FAMILY_NAME,_attack_spans)
static neo_genv_t *answer_env;
static unsigned answer_first,answer_count,answer_used,answers;
static uint8_t attack(void *ctx,neo_gactor_t *a,const int16_t row[7]) {
    (void)ctx;(void)a;(void)row;
    if (answer_used>=answer_count) {answer_used++;return 0;}
    const int32_t *r=ATTACKS[answer_first+answer_used++];answers++;
    for (unsigned i=0;i<2;i++) {
        const int32_t *t=r+1+10*i;neo_ga_target_t *o=&answer_env->target[i];
        *o=(neo_ga_target_t){t[0]!=0,(uint16_t)t[1],(uint16_t)t[2],t[3]!=0,{(int16_t)t[4],(int16_t)t[5],(int16_t)t[6],(int16_t)t[7]},(uint16_t)t[8],(uint16_t)t[9]};
    }
    if (r[0]) contacts++;
    return (uint8_t)r[0];
}
#else
static uint8_t attack(void *ctx,neo_gactor_t *a,const int16_t row[7]) {(void)ctx;(void)a;(void)row;if (contact) contacts++;return contact?1:0;}
#endif
#ifdef FAMILY_PREVIOUS
/* +04 of the recorded node: the node before it in the list. */
#define PREVIOUS CAT(FAMILY_NAME,_previous)
static int previous_node;
static neo_gactor_t *previous(void *ctx,neo_gactor_t *a) {(void)ctx;(void)a;return previous_node>=0?&actors[previous_node]:NULL;}
#endif
/* 0140C0/013F1E: slot of the dynamic collision object (+6A), from 7 down;
 * no other actor of the trace holds one. */
/* A released slot stays busy for two frames (013F1E countdown +2A,
 * decremented at the end of the camera update 01406A). */
static bool dyn_used[NEO_KDYN_COUNT];
static uint8_t dyn_countdown[NEO_KDYN_COUNT];
static void dyn_set(void *ctx,neo_gactor_t *a,uint32_t map,int16_t dx,int16_t dy) {
    (void)ctx;(void)map;(void)dx;(void)dy;
    if (ga_b(a,0x6a)!=0xffu) return;
    for (int k=NEO_KDYN_COUNT-1;k>=0;k--) if (!dyn_used[k] && !dyn_countdown[k]) {dyn_used[k]=true;ga_sb(a,0x6a,(uint8_t)k);return;}
}
static void dyn_release(void *ctx,neo_gactor_t *a) {(void)ctx;if (ga_b(a,0x6a)<NEO_KDYN_COUNT) {dyn_used[ga_b(a,0x6a)]=false;dyn_countdown[ga_b(a,0x6a)]=2;}ga_sb(a,0x6a,0xff);}
/* 004E76: solid-body slot +6B (the only solid body of the trace: slot 0). */
static void solid(void *ctx,neo_gactor_t *a,uint32_t desc) {(void)ctx;ga_sb(a,0x6b,desc?0:0xff);}
static unsigned stamps;
static void stamp(void *ctx,neo_gactor_t *a,uint32_t t) {(void)ctx;(void)a;(void)t;stamps++;}
static void emit(void *ctx,const neo_anim_draw_t *d) {(void)ctx;(void)d;draws++;}
static bool skip(unsigned o) {return o==0x14 || o==0x16 || o==0x58 || o==0x59 || (o>=0x19 && o<=0x1c) || (o>=0x20 && o<=0x23) || (o>=0x44 && o<=0x4b);}
int main(void) {
    /* Node addresses are not in the fixture: a child's recorded +0C names
     * its creator. */
    for (unsigned i=0;i<NN;i++) if (NODES[i].creator>=0)
        node_address[NODES[i].creator]=(uint32_t)((uint32_t)NODES[i].pre[0x0c]<<24|NODES[i].pre[0x0d]<<16|NODES[i].pre[0x0e]<<8|NODES[i].pre[0x0f]);
#ifdef FAMILY_ADDRESSES
    {static const uint32_t ad[]=FAMILY_ADDRESSES;for (unsigned i=0;i<NN && i<sizeof(ad)/sizeof(ad[0]);i++) if (!node_address[i]) node_address[i]=ad[i];}
#endif
    neo_rng_t rng;memset(&rng,0,sizeof(rng));
    static neo_kscene_t kernel;kernel.group_map=soldier_group_map;
    uint8_t groups[9]={0},flag606=0;
    neo_genv_t e;memset(&e,0,sizeof(e));
    e.kernel=&kernel;e.rng=&rng;e.parent=parent;e.program=program;e.emit=emit;e.groups=groups;e.flag606=&flag606;
#ifdef FAMILY_ATTACKS
    answer_env=&e;
#endif
#ifdef FAMILY_PREVIOUS
    e.previous=previous;
#endif
    e.spawn=spawn;e.attack=attack;e.dyn_set=dyn_set;e.dyn_release=dyn_release;e.stamp=stamp;e.solid=solid;e.creator=creator_of;e.actor=actor_of;e.creator_alive=creator_alive;
    e.bounds=(neo_cull_bounds_t){false,0,0,0,0,0,256,320,512};
    e.view[0]=0;e.view[1]=319;e.view[2]=256;e.view[3]=511;
    e.players.mode88=2;e.players.rank=3;e.players.active_mask=1;e.players.targets_valid=true;
#ifndef FAMILY_TERRAIN
#define FAMILY_TERRAIN 0x10d7b6u   /* the families past camera 1023 */
#endif
    e.flags=&flags;e.terrain_tag=FAMILY_TERRAIN;e.node_word=node_word;kernel.b400c=0;   /* RAM 10C00C: 0 in the Mission 1 dumps */
    unsigned ram_bytes=0;(void)ram_bytes;
    e.kill_children=kill_children;e.has_children=has_children;e.camera_mode=camera_mode;e.camera_rise=camera_rise;e.shake=shake;e.map_swap=map_swap;e.front_layer=front_layer;
#ifdef FAMILY_MARKER_SEQ
    static uint8_t marker_seq=FAMILY_MARKER_SEQ;e.marker_seq=&marker_seq;
#endif
#ifdef FAMILY_STAMPS
    /* Collision stamps of other actors made before the family's first
     * update (seen in the RAM dumps of the capture). */
    {static const uint32_t st[][3]=FAMILY_STAMPS;
     for (unsigned i=0;i<sizeof(st)/sizeof(st[0]);i++) {const uint8_t *t=ga_rom(st[i][2]);assert(t);neo_collision_stamp((int32_t)st[i][0],(int32_t)st[i][1],t);}}
#endif
    /* Item pickups (neo_item.c): player 1 from a new life (0C0E20/0C0E6E). */
    static marco_weapons_t weapons;static marco_weapon_player_t player1;static neo_pickup_env_t pickup;
    marco_weapons_new_game(&weapons);player1.exists=true;marco_weapons_life(&weapons,0,&player1);
    pickup.weapons=&weapons;pickup.player[0]=&player1;pickup.mission=0;e.pickup=&pickup;
    unsigned pickups_checked=0;
    bound[0]=true;memcpy(actors[0].m,NODES[0].pre,0xb0);actors[0].active=true;actors[0].index=0;
    ga_sl(&actors[0],0x38,native_tag(ga_l(&actors[0],0x38)));
    unsigned compared=0,deleted=0;bool stopped=false;uint32_t kill_frame=0,kill_at=0;uint8_t epoch=1;int32_t p1x=0,p1y=0;
    for (unsigned k=0;k<NU;k++) {
        const family_update_t *u=&UPDATES[k];
        neo_gactor_t *a=&actors[u->node];current_frame=u->frame;
        {static uint32_t last_frame;
         if (k && u->frame!=last_frame) for (unsigned i=0;i<NEO_KDYN_COUNT;i++) if (dyn_countdown[i]) dyn_countdown[i]--;
         last_frame=u->frame;}
        /* A node created by an actor outside the family (another family's
         * test) starts from its recorded bytes at its first update. */
        if (!bound[u->node] && NODES[u->node].creator==-1) {
            bound[u->node]=true;memset(a,0,sizeof(*a));a->active=true;a->index=(int16_t)u->node;
            memcpy(a->m,NODES[u->node].pre,0xb0);ga_sl(a,0x38,native_tag(ga_l(a,0x38)));injected++;
        }
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
        for (unsigned v=0;v<4;v++) if (u->view[v]>=0) e.view[v]=(uint16_t)u->view[v];
        for (unsigned g=0;g<9;g++) if (u->groups[g]>=0) groups[g]=(uint8_t)u->groups[g];
        for (unsigned b=0;b<8;b++) if (u->boxes[b]>=0) e.pickup_box[b]=(uint16_t)u->boxes[b];
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
        dyn_inputs(&kernel,u);
        if (u->w4068>=0) kernel.w4068=(uint16_t)u->w4068;     /* RAM 10C068 as read */
        /* Object list as read (markers of other families' actors). */
        static neo_marker_t marks[8];
        for (int m=0;m<u->marker_count;m++) marks[m]=(neo_marker_t){u->markers[m][0],u->markers[m][1],(uint8_t)u->markers[m][2],(uint8_t)u->markers[m][3]};
        e.markers=marks;e.marker_count=(unsigned)u->marker_count;
        kernel.terrain=terrain_seen?(neo_kobj_t){terrain.x,terrain.y,terrain.previous_x,terrain.previous_y}:kernel.root;
        e.epoch=epoch;e.players.p1x=(uint16_t)p1x;e.players.p1y=(uint16_t)p1y;
        /* RAM 1067E2 holds 1067E3 for the logic update after the request. */
        if (kill_frame && !kill_at && u->frame>kill_frame) kill_at=u->frame;
        e.kill_all=kill_at && u->frame==kill_at;
        /* RAM 1067E2 as read (set by another family, e.g. the shop). */
        if (u->kill>0) e.kill_all=true;
        uint8_t kill_before=flags.kill_request;
        contact=!u->has_post;
#ifdef FAMILY_HIT_UPDATES
        /* Contacts are the listed updates only (a deletion is the view). */
        {static const int hits[][2]=FAMILY_HIT_UPDATES;contact=false;
         for (unsigned h=0;h<sizeof(hits)/sizeof(hits[0]);h++) if ((uint32_t)hits[h][0]==u->frame && hits[h][1]==u->node) contact=true;}
#endif
#ifdef FAMILY_HIT_CHILD
        /* A bullet deleted without an impact (its first update in this or
         * the next logic update) left the view: no contact. */
        if (contact) {
            contact=false;
            for (unsigned i=0;i<NN;i++) if (NODES[i].creator==u->node && NODES[i].routine==FAMILY_HIT_CHILD &&
                NODES[i].frame>=u->frame && NODES[i].frame<=u->frame+3u) contact=true;
        }
#endif
        unsigned taken=pickup.taken;pickup.frame++;
#ifdef FAMILY_LABEL_COUNTS
        /* RAM 10C5B2/10C5B4 (score labels per player, 08D330) as read. */
        {const int32_t (*lc)[3]=CAT(FAMILY_NAME,_label_counts);
         for (unsigned i=0;i<sizeof(CAT(FAMILY_NAME,_label_counts))/sizeof(lc[0]);i++) if (lc[i][0]==(int32_t)k) {
             if (lc[i][1]>=0) pickup.label_count[0]=(uint16_t)lc[i][1];
             if (lc[i][2]>=0) pickup.label_count[1]=(uint16_t)lc[i][2];
         }}
#endif
#ifdef FAMILY_RAM
        for (int i=0;i<u->ram_in_count;i++) wram[RAMIN[u->ram_in_first+i][0]&0xffffu]=(uint8_t)RAMIN[u->ram_in_first+i][1];
        memcpy(wexpect,wram,sizeof(wram));
        for (int i=0;i<u->ram_out_count;i++) wexpect[RAMOUT[u->ram_out_first+i][0]&0xffffu]=(uint8_t)RAMOUT[u->ram_out_first+i][1];
        ram_to_env(&weapons);e.vehicle=&venv;e.video=w16(0x10000e);
#endif
#ifdef FAMILY_ATTACKS
        answer_first=ATTACK_SPANS[k][0];answer_count=ATTACK_SPANS[k][1];answer_used=0;
#endif
#ifdef FAMILY_PREVIOUS
        previous_node=PREVIOUS[k];
#endif
        ga_result_t r=neo_region_tick(a,&e);
#ifdef FAMILY_ATTACKS
        if (answer_used!=answer_count) {fprintf(stderr,"frame %u node %d %05x: %u calls of 0049E8, original %u\n",u->frame,u->node,u->routine,answer_used,answer_count);return 1;}
#endif
#ifdef FAMILY_RAM
        env_to_ram(&weapons);
        for (unsigned g=0;g<sizeof(ram_ranges)/sizeof(ram_ranges[0]);g++) for (uint32_t ad=ram_ranges[g][0];ad<ram_ranges[g][1];ad++) {
            /* The player nodes' list links +04..+0B change when a family
             * node next to them is inserted or deleted (0040BC/004188, the
             * node pool is not modelled). */
            if ((ad>=0x1014c8u && ad<0x1014d0u) || (ad>=0x101578u && ad<0x101580u)) continue;
            /* A recorded RAM range over a node the family holds now (a
             * solid body's node reused by a family node): the node test
             * compares it. */
            {bool own=false;for (unsigned k=0;k<NN && !own;k++) own=actors[k].active && node_address[k] && ad>=node_address[k] && ad<node_address[k]+0xc0u;if (own) continue;}
            if (wram[ad&0xffffu]!=wexpect[ad&0xffffu]) {fprintf(stderr,"frame %u node %d %05x: RAM %06x = %02x, original %02x\n",u->frame,u->node,u->routine,(unsigned)ad,wram[ad&0xffffu],wexpect[ad&0xffffu]);return 1;}
            ram_bytes++;
        }
        memcpy(wram,wexpect,sizeof(wram));
#endif
        if (pickup.taken!=taken) {
#ifdef FAMILY_PICKUPS
            static const uint32_t pk[][3]=FAMILY_PICKUPS;bool ok=false;
            for (unsigned i=0;i<sizeof(pk)/sizeof(pk[0]);i++)
                if (pk[i][0]==u->frame) ok=marco_weapons_id(&weapons,0)==pk[i][1] && marco_weapons_rounds(&weapons,0)==pk[i][2];
            if (!ok) {fprintf(stderr,"frame %u: pickup left weapon %x rounds %x\n",u->frame,marco_weapons_id(&weapons,0),marco_weapons_rounds(&weapons,0));return 1;}
            pickups_checked++;
#else
            fprintf(stderr,"frame %u: unexpected pickup\n",u->frame);return 1;
#endif
        }
#ifdef FAMILY_STOP
        {static const uint32_t stop[2]=FAMILY_STOP;
         if (u->frame==stop[0]) {
            /* An untranslated service the original reached in this update:
             * the native code must stop exactly there. */
            if (!(r==GA_UNSUPPORTED || e.unsupported) || e.unsupported_at!=stop[1]) {fprintf(stderr,"frame %u: expected the explicit stop at %05x (result %d at %05x)\n",u->frame,stop[1],r,(unsigned)e.unsupported_at);return 1;}
            stopped=true;break;
         }}
#endif
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
        if (ga_l(a,0)!=(uint32_t)((uint32_t)u->post[0]<<24|u->post[1]<<16|u->post[2]<<8|u->post[3])) {
            fprintf(stderr,"frame %u node %d %05x -> %05x expected %02x%02x%02x%02x\n",u->frame,u->node,u->routine,ga_l(a,0),u->post[0],u->post[1],u->post[2],u->post[3]);return 1;
        }
        uint32_t t38=native_tag((uint32_t)((uint32_t)u->post[0x38]<<24|u->post[0x39]<<16|u->post[0x3a]<<8|u->post[0x3b]));
        for (unsigned o=0x10;o<0xb0;o++) {
            if (skip(o)) continue;
            uint8_t want=(o>=0x38 && o<0x3c)?(uint8_t)(t38>>(24-8*(o-0x38))):u->post[o];
#ifdef FAMILY_NODE_FIELDS
            {static const unsigned nf[]=FAMILY_NODE_FIELDS;
             for (unsigned k=0;k<sizeof(nf)/sizeof(nf[0]);k++) if (o>=nf[k] && o<nf[k]+4u) {
                 uint32_t v=native_tag((uint32_t)u->post[nf[k]]<<24|(uint32_t)u->post[nf[k]+1]<<16|(uint32_t)u->post[nf[k]+2]<<8|u->post[nf[k]+3]);
                 want=(uint8_t)(v>>(24-8*(o-nf[k])));
             }}
#endif
            if (a->m[o]!=want) {
                fprintf(stderr,"frame %u node %d routine %05x +%02X=%02x expected %02x\n",u->frame,u->node,u->routine,o,a->m[o],want);
                fprintf(stderr,"  got     ");for (unsigned q=0x10;q<0xb0;q++) fprintf(stderr,"%02x%s",a->m[q],(q&15)==15?"\n          ":"");
                fprintf(stderr,"\n  wanted  ");for (unsigned q=0x10;q<0xb0;q++) fprintf(stderr,"%02x%s",u->post[q],(q&15)==15?"\n          ":"");
                fprintf(stderr,"\n");return 1;
            }
            compared++;
        }
    }
    for (unsigned i=0;i<NN;i++) if (!bound[i]) {fprintf(stderr,"node %u (%05x) never created\n",i,NODES[i].routine);return 1;}
#ifdef FAMILY_STOP
    if (!stopped) {fprintf(stderr,"explicit stop never reached\n");return 1;}
#endif
#ifdef FAMILY_PICKUPS
    {static const uint32_t pk[][3]=FAMILY_PICKUPS;
     if (pickups_checked!=sizeof(pk)/sizeof(pk[0])) {fprintf(stderr,"%u pickups, expected %u\n",pickups_checked,(unsigned)(sizeof(pk)/sizeof(pk[0])));return 1;}}
#endif
#ifdef FAMILY_RAM
    printf("RAM outside the nodes (slots, input words, weapon block, player nodes): %u bytes compared\n",ram_bytes);
#endif
    printf("PASS: family %s%s: %u nodes, %u closed-loop updates (frames %u..%u), %u bytes, %u deletions, %u draws, %u player contacts, %u collision stamps, %u children outside the family, %u deleted by 004192, %u created outside, %u pickups%s\n",
        STR(FAMILY_NAME),stopped?" (to its explicit stop)":"",(unsigned)NN,(unsigned)NU,UPDATES[0].frame,UPDATES[NU-1].frame,compared,deleted,draws,contacts,stamps,externals,killed_children,injected,pickups_checked,swapped?", map swap requested":"");
    return 0;
}
