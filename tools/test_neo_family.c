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
    uint16_t ring[64][2]; int wfirst,wcount; uint8_t ext[16][2]; int32_t boxes[8]; int32_t kill; int32_t w4068;
    int marker_count; int32_t markers[8][4];   /* object list read (040200/0402E6): x, y, height, id */
    int ram_in_first,ram_in_count,ram_out_first,ram_out_count;  /* FAMILY_RAM inputs/outputs */
    int32_t view[4];   /* RAM 10E9D2..10E9D8 as read (0C8BEC), -1: not read */
    int32_t p1xl;      /* RAM 101502: player 1 +3E (low word of the X long), -1: not read */
    int32_t solid_busy; /* RAM 106756: used entries of the solid-body table as read (004E76), -1: not read */
} family_update_t;
#include FAMILY_HEADER
/* The Mission 3 mid-boss 0A4A94 (neo_m3mid.c): pieces of one long fight
 * like the Mission 2 boss (nodes alive before a piece, the reserve of
 * 0040BC), its layer object 10E8C6 and RAM 10E959 from the RAM image. */
#if defined(FAMILY_M2BOSS) || defined(FAMILY_M3MID)
#define FAMILY_BOSSLIKE 1
#endif
#if defined(FAMILY_M3MID) && !defined(FAMILY_RESUME)
#define FAMILY_RESUME 1
#endif
#ifdef FAMILY_M3MID
static uint8_t wram[0x10000];   /* tentative: the RAM image below */
#endif
#define STR2(a) #a
#define STR(a) STR2(a)
#define CAT2(a,b) a##b
#define CAT(a,b) CAT2(a,b)
#define NODES CAT(FAMILY_NAME,_nodes)
#define UPDATES CAT(FAMILY_NAME,_updates)
#define WRITES CAT(FAMILY_NAME,_writes)
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
#ifdef FAMILY_EXIT_GLOBALS
    /* The stage exit 0B6ED2 (018F92, 0CA340, 018F40). */
    if (ad>=0x1067e4 && ad<0x1067e6) return 0x22+(int)(ad-0x1067e4);
    if (ad>=0x10ea00 && ad<0x10ea04) return 0x60+(int)(ad-0x10ea00);
    if (ad>=0x10c300 && ad<0x10c308) return 0x70+(int)(ad-0x10c300);
    if (ad>=0x10c00a && ad<0x10c00c) return 0x80+(int)(ad-0x10c00a);
#endif
    return -1;
}
static void put(uint32_t ad,uint8_t v) {int s=slot(ad);if (s>=0) ram[s]=v;}
static void camera_mode(void *ctx,uint8_t m) {(void)ctx;put(0x10e95a,m);}
static void camera_rise(void *ctx,int32_t vy) {(void)ctx;for (unsigned i=0;i<4;i++) put(0x10e960+i,(uint8_t)((uint32_t)vy>>(24-8*i)));}
static void shake(void *ctx) {(void)ctx;put(0x10e9c8,0);put(0x10e9c9,8);
#if defined(FAMILY_M3MID)
    wram[0xe9c8]=0;wram[0xe9c9]=8;
#endif
}
#if defined(FAMILY_M3MID)
/* 0C8022: RAM 10E9C8 as read; bits 0..1 set, or (negative) cleared and
 * 10E9CC = (10E9C8 long + 0x8000) & FFFF0000. */
static void shake_pulse(void *ctx) {
    (void)ctx;uint8_t *r=&wram[0xe9c8];
    if (!(r[0]&0x80u)) {r[1]|=3u;put(0x10e9c8u,r[0]);put(0x10e9c9u,r[1]);return;}
    r[1]&=0xfcu;
    uint32_t v=((uint32_t)r[0]<<24|(uint32_t)r[1]<<16|(uint32_t)r[2]<<8|r[3])+0x8000u;
    r[4]=(uint8_t)(v>>24);r[5]=(uint8_t)(v>>16);r[6]=0;r[7]=0;
    put(0x10e9c8u,r[0]);put(0x10e9c9u,r[1]);for (unsigned i=4;i<8;i++) put(0x10e9c8u+i,r[i]);
}
#endif
static int front_value;
static bool front_layer(void *ctx) {(void)ctx;return front_value>0;}
static bool swapped;
static bool map_swap(void *ctx) {(void)ctx;swapped=true;return true;}
static int index_of(const neo_gactor_t *a) {return (int)(a-actors);}
#ifdef FAMILY_TRAIN
static neo_kobj_t train_now;
#endif
static bool parent(void *ctx,uint32_t tag,neo_contact_parent_t *out) {
    (void)ctx;
    if (tag==0x10e968u) {*out=root;return true;}
#ifdef FAMILY_TRAIN
    /* Mission 3: the train 10D7B6 as read (+14..+20, +24 correction 0). */
    if (tag==0x10d7b6u) {*out=(neo_contact_parent_t){tag,train_now.x,train_now.y,train_now.prev_x,train_now.prev_y,0};return true;}
#endif
#if defined(FAMILY_M3MID) && defined(FAMILY_RAM)
    /* Mission 3: the mid-boss moves the layer object 10E8C6 (as read). */
    if (tag==0x10e8c6u) {
        #define L32(p) ((uint32_t)((uint32_t)(p)[0]<<24|(p)[1]<<16|(p)[2]<<8|(p)[3]))
        const uint8_t *b=&wram[0xe8c6];
        *out=(neo_contact_parent_t){tag,(int32_t)L32(b+0x14),(int32_t)L32(b+0x18),(int32_t)L32(b+0x1c),(int32_t)L32(b+0x20),b[0x25]};
        #undef L32
        return true;
    }
#endif
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
#ifdef FAMILY_FRESH_PROGRAM
static bool updated[NN],first_update[NN];
#endif
static neo_gactor_t actors[NN+1];
static uint32_t native_tag(uint32_t recorded) {
    /* A reused node address: the instance alive now first. */
    for (unsigned i=0;i<NN;i++) if (node_address[i] && bound[i] && actors[i].active && (node_address[i]==recorded || node_address[i]+0x28u==recorded)) return NEO_GA_TAG(i);
    /* A dead address: its latest instance (the gunship's exhaust 01D908 keeps
     * the block of its creator deleted in the same frame). */
    for (unsigned i=NN;i-->0;) if (node_address[i] && (node_address[i]==recorded || node_address[i]+0x28u==recorded)) return NEO_GA_TAG(i);
    for (unsigned i=0;i<NN;i++) if (node_address[i] && recorded>node_address[i] && recorded<node_address[i]+0xc0u) return NEO_GA_TAG(i)|((recorded-node_address[i])<<16);
    return recorded;
}
#if defined(FAMILY_M2BOSS) || defined(FAMILY_RESUME)
/* A node alive before the piece: its native program and cursor from the
 * recorded program pointer +20 (the ROM address of its next command). */
static void resume_program(neo_gactor_t *a) {
    uint32_t at=ga_l(a,0x20);
    if (!at) return;
    for (unsigned i=0;i<sizeof(soldier_programs)/sizeof(soldier_programs[0]);i++)
        for (unsigned k=0;k<soldier_programs[i].program.count;k++) if ((uint32_t)soldier_programs[i].address[k]==at) {
            a->program=&soldier_programs[i].program;a->program_tag=soldier_programs[i].tag;a->cursor=(uint16_t)k;return;
        }
    fprintf(stderr,"no native program holds the command at %06x\n",(unsigned)at);
}
#endif
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
/* Index of the update being replayed (UPDATES[current_update]). */
static unsigned current_update;
/* Frames after its creator's update in which a recorded child may first
 * appear (compile_family_fixture.py --spawn-window; default 8). With a
 * full node pool the original makes no child: a later recorded node is
 * another update's (the R family: 2). */
#ifndef FAMILY_SPAWN_WINDOW
#define FAMILY_SPAWN_WINDOW 8u
#endif
#define SPAWN_WINDOW FAMILY_SPAWN_WINDOW
/* 009EA8's D0.w: the slot the original's allocator gave, as recorded. */
static uint8_t palette_slot(void *ctx,neo_gactor_t *a,unsigned n) {
    (void)ctx;(void)a;const family_update_t *u=&UPDATES[current_update];
    return u->has_post && n<4u?u->post[0x19+n]:0;
}
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
#if defined(FAMILY_BOSSLIKE) || defined(FAMILY_SPLIT_CHILDREN)
    /* The frame mark can fall inside the list walk after the creator: the
     * children of one update (inserted after it, newest first) then split
     * over two frames; the first made is the last of them in the list. */
    if (pick>=0) {
        uint32_t minf=NODES[pick].frame;
        for (unsigned i=0;i<NN;i++) {
            if (bound[i] || NODES[i].creator!=c || NODES[i].routine!=routine || NODES[i].frame>minf+1u) continue;
            /* A child first run after the creator's next update is that
             * update's (a creator making one child per update, frames
             * apart: the flame emitter 04B79C of m3boss1). */
            bool later=false;
            for (unsigned k=current_update+1u;k<NU;k++) {
                if (UPDATES[k].node==(int)i) break;
                if (UPDATES[k].node==c) {later=true;break;}
            }
            if (later) continue;
            if ((int)i>pick) pick=(int)i;
        }
    }
#endif
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
#if defined(FAMILY_BOSSLIKE) || defined(FAMILY_RESERVE)
/* 0040BC with its reserve: the free-node count RAM 1063D8 as the update
 * first read it, one less per node made. */
static uint16_t free_nodes;
#ifdef FAMILY_LEAVE
static uint32_t leave_entry;
/* JMP 0B0626 / 0B0632: the node becomes a soldier (the world moves it). */
static bool leave_soldier(void *ctx,neo_gactor_t *a,uint32_t entry) {(void)ctx;(void)a;leave_entry=entry;return true;}
#endif
static neo_gactor_t *spawn_reserve(void *ctx,neo_gactor_t *creator,uint32_t routine,uint16_t reserve) {
    if (reserve>=free_nodes) return NULL;
    free_nodes--;return spawn(ctx,creator,routine,false);
}
#endif
static neo_gactor_t *actor_of(void *ctx,uint32_t t) {
    (void)ctx;
    if ((t&0xff000000u)==NEO_GA_TAG_BASE && (t&0xffffu)<NN && actors[t&0xffffu].active) return &actors[t&0xffffu];
    return NULL;
}
static neo_gactor_t *creator_of(void *ctx,neo_gactor_t *a) {
    (void)ctx;uint32_t t=ga_l(a,0x0c);
    if ((t&0xff000000u)==NEO_GA_TAG_BASE && (t&0xffffu)<NN) return &actors[t&0xffffu];
#ifdef FAMILY_RAM_CREATORS
    /* A creator outside the family (the player 1014C4, the gate 03889C):
     * its node from the RAM image, as the update read it. */
    if (t>=0x1010a4u && t<0x1010a4u+0xb0u*100u && !((t-0x1010a4u)%0xb0u)) {
        static neo_gactor_t view[100];neo_gactor_t *v=&view[(t-0x1010a4u)/0xb0u];
        memcpy(v->m,&wram[t&0xffffu],0xb0);v->active=true;v->index=-1;
        return v;
    }
#endif
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
static bool creator_alive(void *ctx,neo_gactor_t *a) {(void)ctx;neo_gactor_t *c=creator_of(NULL,a);return !c || (c->active && (c->index>=0 || ga_l(c,0)!=0x3f1eu));}   /* 00409A on a RAM view */
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
    /* 0140D2: from slot 7 down, the first with no map (+26) and no
     * countdown (+2A); a slot of an actor outside the family is busy as the
     * original read it (the rope soldiers 082BCE beside the chests). */
    for (int k=NEO_KDYN_COUNT-1;k>=0;k--) {
        const uint8_t *b=&dynram[k*64];
        bool outside=dyn_seen && ((b[0x26]|b[0x27]|b[0x28]|b[0x29]|b[0x2a]|b[0x2b])!=0);
        if (!dyn_used[k] && !dyn_countdown[k] && !outside) {dyn_used[k]=true;ga_sb(a,0x6a,(uint8_t)k);return;}
    }
}
static void dyn_release(void *ctx,neo_gactor_t *a) {(void)ctx;if (ga_b(a,0x6a)<NEO_KDYN_COUNT) {dyn_used[ga_b(a,0x6a)]=false;dyn_countdown[ga_b(a,0x6a)]=2;}ga_sb(a,0x6a,0xff);}
/* 004E76: solid-body slot +6B (the only solid body of the trace: slot 0). */
/* 004E76: +6B = the first free entry of the table 106756 (as the original
 * read it this update; entry 0 when the update read none), FF to remove. */
static int32_t solid_busy=-1;
static void solid(void *ctx,neo_gactor_t *a,uint32_t desc) {
    (void)ctx;
    if (!desc) {ga_sb(a,0x6b,0xff);return;}
    if (ga_b(a,0x6b)!=0xffu) return;
    unsigned i=0;
    if (solid_busy>=0) while (i<16 && (solid_busy>>i&1)) i++;
    if (i<16) ga_sb(a,0x6b,(uint8_t)i);
}
static unsigned stamps;
#if defined(FAMILY_RING) && defined(FAMILY_RAM)
static void stamp_ring(neo_gactor_t *a,uint32_t t);
#endif
static void stamp(void *ctx,neo_gactor_t *a,uint32_t t) {
    (void)ctx;(void)a;(void)t;stamps++;
#if defined(FAMILY_RING) && defined(FAMILY_RAM)
    stamp_ring(a,t);
#endif
}
#if defined(FAMILY_RING) && defined(FAMILY_RAM)
/* 0C88B0 into the ring +88 of the actor's layer (+38 = 10C72E or 10D7B6,
 * else nothing): template dx,dy (cells), w, h, material +4, mask +6, the
 * w*h shapes (columns of h from the bottom row up) at +10; window +14/+18
 * of the layer; an existing shape s survives unless mask bit s is set;
 * bits 5..7 of the old cell stay. */
static void stamp_ring(neo_gactor_t *a,uint32_t t) {
    uint32_t parent=ga_l(a,0x38);
    if (parent!=0x10c72eu && parent!=0x10d7b6u) return;
    const uint8_t *tp=ga_rom(t);assert(tp);
    unsigned L=parent&0xffffu;
    int16_t d0=(int16_t)ga_w(a,0x28),d1=(int16_t)ga_w(a,0x2c);
    int16_t d2=(int16_t)(0xffc0-(wram[L+0x14]<<8|wram[L+0x15])),d3=(int16_t)(0x80-(wram[L+0x18]<<8|wram[L+0x19]));
    d0=(int16_t)(d0>>3);d1=(int16_t)(d1>>3);d2=(int16_t)(d2>>3);d3=(int16_t)(d3>>3);
    d0=(int16_t)(d0+(int8_t)tp[0]);d1=(int16_t)(d1+(int8_t)tp[1]);
    unsigned d4=tp[2],d5=tp[3];
    uint32_t mask=(uint32_t)tp[6]<<24|(uint32_t)tp[7]<<16|(uint32_t)tp[8]<<8|tp[9];
    d1=(int16_t)-(d1+(int16_t)d5);d3=(int16_t)-(d3+0x3f);
    uint8_t *ring=&wram[(L+0x88u)&0xffffu];
    const uint8_t *a1=tp+10;
    for (;d4;d4--,a1++,d0++) {
        if ((uint16_t)(d0-d2)>=0x40u) continue;
        const uint8_t *a2=a1;int16_t r=d1;
        for (unsigned k=d5;k;k--,a2+=tp[2],r++) {
            if ((uint16_t)(r-d3)>=0x40u) continue;
            uint8_t *cell=&ring[((unsigned)(d0&0x3f)<<6)+(unsigned)(r&0x3f)];
            unsigned shape=*cell&15u;
            if (shape && !((mask>>shape)&1u)) continue;
            uint8_t v=*a2;assert(v<0x10);
            if (tp[4]) v|=0x10;
            *cell=(uint8_t)(v+(*cell&0xe0u));
        }
    }
}
/* 0C70FC (collision part, 0C72EC): the sub-map's cells into the ring +88
 * of the --ring layer, clipped to its 32x32-block window (+36/+38 as read). */
static unsigned patches;
static void map_patch(void *ctx,uint32_t layer,uint32_t submap,const int16_t d[6]) {
    (void)ctx;patches++;
    if (layer!=FAMILY_RING) return;
    const uint8_t *s=ga_rom(submap);assert(s);
    int d0=d[0],d1=d[1],d2=d[2],d3=d[3],d4=d[4],d5=d[5],d6,d7;
    uint32_t L=layer&0xffffu;
    int16_t w36=(int16_t)(wram[L+0x36]<<8|wram[L+0x37]),w38=(int16_t)(wram[L+0x38]<<8|wram[L+0x39]);
    d6=w36-d4;if (d6>=0) {d4+=d6;d0+=d6;d2-=d6;if (d2<=0) return;d6=0;}
    d6+=d2-0x20;if (d6>=0) {d2-=d6;if (d2<=0) return;}
    d6=w38-d5;if (d6>=0) {d5+=d6;d1-=d6;d3-=d6;if (d3<=0) return;d6=0;}
    d6+=d3-0x20;if (d6>=0) {d3-=d6;if (d3<=0) return;}
    int w=(int16_t)(s[8]<<8|s[9]),h=(int16_t)(s[10]<<8|s[11]);
    d0=(int16_t)(d0<<4)>>4;d1=(int16_t)(d1<<4)>>4;
    if (d0>=w || d1>=h) return;
    if (d0<0) {d2+=d0;if (d2<=0) return;d4-=d0;d0=0;}
    if (d1<0) {d3+=d1;if (d3<=0) return;d5+=d1;d1=0;}
    d7=d0+d2-w;if (d7>=0) {d2-=d7;if (d2<=0) return;}
    d7=d1+d3-h;if (d7>=0) {d3-=d7;if (d3<=0) return;}
    d4&=0x1f;d5=(~d5)&0x1f;
    uint32_t c=(uint32_t)s[4]<<24|(uint32_t)s[5]<<16|(uint32_t)s[6]<<8|s[7];
    if (!c) return;
    assert(c!=0xffffffffu);
    const uint8_t *src=ga_rom(c+(uint32_t)((d0*h+d1)*4));assert(src);
    uint8_t *ring=&wram[(L+0x88u)&0xffffu];
    unsigned col=(unsigned)(d4<<7)&0xf80u;
    for (int i=0;i<d2;i++) {
        unsigned r=(unsigned)d5*2u;
        for (int j=0;j<d3;j++) {
            unsigned o=col+(r&0x3eu);
            ring[o]=*src++;ring[o+0x40]=*src++;ring[o+1]=*src++;ring[o+0x41]=*src++;r+=2;
        }
        src+=(h-d3)*4;col=(col+0x80u)&0xf80u;
    }
}
#endif
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
    e.spawn=spawn;e.palette_slot=palette_slot;e.attack=attack;e.dyn_set=dyn_set;e.dyn_release=dyn_release;e.stamp=stamp;e.solid=solid;e.creator=creator_of;e.actor=actor_of;e.creator_alive=creator_alive;
    e.bounds=(neo_cull_bounds_t){false,0,0,0,0,0,256,320,512};
    e.view[0]=0;e.view[1]=319;e.view[2]=256;e.view[3]=511;
    e.players.mode88=2;e.players.rank=3;e.players.active_mask=1;e.players.targets_valid=true;
#ifndef FAMILY_TERRAIN
#define FAMILY_TERRAIN 0x10d7b6u   /* the families past camera 1023 */
#endif
    e.flags=&flags;e.terrain_tag=FAMILY_TERRAIN;e.node_word=node_word;kernel.b400c=0;   /* RAM 10C00C: 0 in the Mission 1 dumps */
    unsigned ram_bytes=0;(void)ram_bytes;
    e.kill_children=kill_children;e.has_children=has_children;e.camera_mode=camera_mode;e.camera_rise=camera_rise;e.shake=shake;e.map_swap=map_swap;e.front_layer=front_layer;
#if defined(FAMILY_RING) && defined(FAMILY_RAM)
    e.map_patch=map_patch;
#endif
#ifdef FAMILY_MARKER_SEQ
    static uint8_t marker_seq=FAMILY_MARKER_SEQ;e.marker_seq=&marker_seq;
#endif
#ifdef FAMILY_GROUP_MAP
    /* --group-map 0: RAM 10C00E = 0 (Mission 3), 012970 keeps the group. */
    if (FAMILY_GROUP_MAP) {kernel.group_map=ga_rom(FAMILY_GROUP_MAP);assert(kernel.group_map);} else kernel.group_map=NULL;
#endif
#ifdef FAMILY_GRID_COLUMNS
    /* A stage grid of another mission (collision.json, --grid). */
    {static uint8_t dynamic[FAMILY_GRID_COLUMNS*FAMILY_GRID_ROWS];neo_collision_set_grid(family_grid,dynamic,NULL,FAMILY_GRID_COLUMNS,FAMILY_GRID_ROWS);}
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
#if defined(FAMILY_M2BOSS) || defined(FAMILY_RESUME)
    ga_sl(&actors[0],0x0c,native_tag(ga_l(&actors[0],0x0c)));resume_program(&actors[0]);
#if defined(FAMILY_M3MID) && defined(FAMILY_NODE_FIELDS)
    {static const unsigned nf[]=FAMILY_NODE_FIELDS;for (unsigned i=0;i<sizeof(nf)/sizeof(nf[0]);i++) ga_sl(&actors[0],nf[i],native_tag(ga_l(&actors[0],nf[i])));}
#endif
#endif
#ifdef FAMILY_EXIT_GLOBALS
    ram[0x73]=expect[0x73]=0xff;   /* RAM 10C303 until 018F40 */
#endif
    unsigned sgun_born=0,sgun_left=0;
    unsigned compared=0,deleted=0,left=0;bool stopped=false;uint32_t kill_frame=0,kill_at=0;uint8_t epoch=1;int32_t p1x=0,p1y=0,p1xl=0;
    for (unsigned k=0;k<NU;k++) {
        const family_update_t *u=&UPDATES[k];
#ifdef FAMILY_FRESH_PROGRAM
        first_update[u->node]=!updated[u->node];updated[u->node]=true;
#endif
        neo_gactor_t *a=&actors[u->node];current_frame=u->frame;current_update=k;
        {static uint32_t last_frame;
         if (k && u->frame!=last_frame) for (unsigned i=0;i<NEO_KDYN_COUNT;i++) if (dyn_countdown[i]) dyn_countdown[i]--;
         last_frame=u->frame;}
        /* A node created by an actor outside the family (another family's
         * test) starts from its recorded bytes at its first update. */
#if defined(FAMILY_M2BOSS) || defined(FAMILY_RESUME)
        /* A piece of a long fight: a node made before its creator's first
         * recorded update (the trace starts in the middle of that update)
         * starts from its recorded bytes too. */
        static bool ran[NN];
        bool early=!bound[u->node] && NODES[u->node].creator>=0 && (!ran[NODES[u->node].creator] || u->frame<=UPDATES[0].frame+1u);
        ran[u->node]=true;

#else
        bool early=false;
#endif
        if (!bound[u->node] && (NODES[u->node].creator==-1 || early)) {
            bound[u->node]=true;memset(a,0,sizeof(*a));a->active=true;a->index=(int16_t)u->node;
            memcpy(a->m,NODES[u->node].pre,0xb0);ga_sl(a,0x38,native_tag(ga_l(a,0x38)));injected++;
#if defined(FAMILY_M2BOSS) || defined(FAMILY_RESUME)
            /* A node already alive when the piece starts: its creator (+0C)
             * is a node of the fixture (the boss's parts). */
            ga_sl(a,0x0c,native_tag(ga_l(a,0x0c)));resume_program(a);
#if defined(FAMILY_RESUME) && defined(FAMILY_NODE_FIELDS)
            {static const unsigned nf[]=FAMILY_NODE_FIELDS;   /* node pointers (the turrets' anchors) */
             for (unsigned i=0;i<sizeof(nf)/sizeof(nf[0]);i++) ga_sl(a,nf[i],native_tag(ga_l(a,nf[i])));}
#endif
#endif
        }
#if defined(FAMILY_M2BOSS) || defined(FAMILY_RESUME)
        /* Its creator alive before the piece and not yet run: from its
         * recorded bytes (the node reads its creator's fields). */
        if (bound[u->node]) {uint32_t t=ga_l(a,0x0c);unsigned c=t&0xffffu;
         if ((t&0xff000000u)==NEO_GA_TAG_BASE && c<NN && !bound[c] && (NODES[c].creator==-1
#ifdef FAMILY_M3MID
             || u->frame<=UPDATES[0].frame+1u   /* alive at the start, first run later in the list */
#endif
             )) {
            neo_gactor_t *p=&actors[c];bound[c]=true;memset(p,0,sizeof(*p));p->active=true;p->index=(int16_t)c;
            memcpy(p->m,NODES[c].pre,0xb0);ga_sl(p,0x38,native_tag(ga_l(p,0x38)));ga_sl(p,0x0c,native_tag(ga_l(p,0x0c)));resume_program(p);injected++;
#if defined(FAMILY_M3MID) && defined(FAMILY_NODE_FIELDS)
            {static const unsigned nf[]=FAMILY_NODE_FIELDS;for (unsigned i=0;i<sizeof(nf)/sizeof(nf[0]);i++) ga_sl(p,nf[i],native_tag(ga_l(p,nf[i])));}
#endif
         }}
#endif
#if defined(FAMILY_M3MID) && defined(FAMILY_NODE_FIELDS)
        /* The node a node field names (the body in +6E), alive at the start
         * and not yet run: from its recorded bytes. */
        if (bound[u->node] && u->frame<=UPDATES[0].frame+1u) {
            static const unsigned nf[]=FAMILY_NODE_FIELDS;
            for (unsigned i=0;i<sizeof(nf)/sizeof(nf[0]);i++) {
                uint32_t t=ga_l(a,nf[i]);unsigned c=t&0xffffu;
                if ((t&0xff000000u)!=NEO_GA_TAG_BASE || c>=NN || bound[c]) continue;
                neo_gactor_t *p=&actors[c];bound[c]=true;memset(p,0,sizeof(*p));p->active=true;p->index=(int16_t)c;
                memcpy(p->m,NODES[c].pre,0xb0);ga_sl(p,0x38,native_tag(ga_l(p,0x38)));ga_sl(p,0x0c,native_tag(ga_l(p,0x0c)));resume_program(p);injected++;
                for (unsigned j=0;j<sizeof(nf)/sizeof(nf[0]);j++) ga_sl(p,nf[j],native_tag(ga_l(p,nf[j])));
            }
        }
#endif
        if (!bound[u->node] || !a->active) {fprintf(stderr,"frame %u node %d (%05x) not created natively\n",u->frame,u->node,u->routine);return 1;}
        if (ga_l(a,0)!=u->routine) {fprintf(stderr,"frame %u node %d routine %05x expected %05x\n",u->frame,u->node,ga_l(a,0),u->routine);return 1;}
        if (u->root_mask&1) root.x=u->root[0];
        if (u->root_mask&2) root.y=u->root[1];
        if (u->root_mask&4) root.previous_x=u->root[2];
        if (u->root_mask&8) root.previous_y=u->root[3];
        root.tag=0x10e968u;if (u->corr>=0) root.correction=(uint8_t)u->corr;
        if (u->p1x>=0) p1x=u->p1x;
        if (u->p1xl>=0) p1xl=u->p1xl;
        solid_busy=u->solid_busy;
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
#ifdef FAMILY_M3MID
        /* A node killed by a player's shot between its updates (+5F bit 7,
         * 004892): the nodes running before its next update in the same
         * logic update read it already (the bombs of a dying body). */
        for (unsigned q=k+1;q<NU && UPDATES[q].frame==u->frame;q++) {
            const family_update_t *v=&UPDATES[q];
            if (v->node==u->node || !actors[v->node].active) continue;
            for (unsigned x=0;x<16 && v->ext[x][0];x++) if (v->ext[x][0]==0x5f && (v->ext[x][1]&0x80u) && !(actors[v->node].m[0x5f]&0x80u)) actors[v->node].m[0x5f]=v->ext[x][1];
        }
#endif
        /* Bytes other actors wrote into the node since its last update. */
        for (unsigned x=0;x<16 && u->ext[x][0];x++) a->m[u->ext[x][0]]=u->ext[x][1];
        /* B25: the blasts 0644C4/0646B0 (sgun2/sgun7) are born with the
         * player's +15 & 3 (0C4434: 01AC94); count the ones facing left. */
        if (u->routine==0x644c4u || u->routine==0x646b0u) {sgun_born++;if (a->m[0x15]&1u) sgun_left++;}
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
#ifdef FAMILY_TRAIN
        /* Mission 3: the train 10D7B6 as read (+14..+20) and its RAM ring. */
        {static neo_kobj_t train;const int32_t *tr=CAT(FAMILY_NAME,_train)[k];
         if (tr[0]&1) train.x=tr[1];
         if (tr[0]&2) train.y=tr[2];
         if (tr[0]&4) train.prev_x=tr[3];
         if (tr[0]&8) train.prev_y=tr[4];
         kernel.train_on=true;kernel.train=train;train_now=train;kernel.train_ring=&wram[0xd83e];}
#endif
        e.epoch=epoch;e.players.p1x=(uint16_t)p1x;e.players.p1y=(uint16_t)p1y;e.player_low[0]=(uint16_t)p1xl;
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
        /* RAM 10C590: 08BFD6 counts the game's updates (not the nodes'). */
        static uint32_t last_update_frame;
        unsigned taken=pickup.taken;if (u->frame!=last_update_frame) {pickup.frame++;last_update_frame=u->frame;}
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
        ram_to_env(&weapons);e.vehicle=&venv;e.video=w16(0x10000e);e.ref69b8=w16(0x10e9b8);
        e.player_flags15[0]=wram[0x14d9];e.player_flags15[1]=wram[0x1589];   /* the players' +15 as read */
        /* RAM 10C33C (01AEB0: the players present) when the family watches it. */
        for (unsigned g=0;g<sizeof(ram_ranges)/sizeof(ram_ranges[0]);g++) if (ram_ranges[g][0]<=0x10c33cu && 0x10c33du<ram_ranges[g][1]) e.players.active_mask=wram[0xc33c]&3u;   /* btst.l d0 on the byte 10C33C */
        e.laser=&wram[0xc340];   /* RAM 10C340..10C34F: the beam blocks (neo_pweapon.c) */
#ifdef FAMILY_RING
        kernel.ring=&wram[(FAMILY_RING+0x88u)&0xffffu];
#endif
        pickup.mission=w16(0x10c2f6);   /* RAM 10C2F6 as read (0 when not recorded) */
#ifdef FAMILY_M2BOSS
        /* The boss 026254 (neo_m2boss.c): RAM 10E99A, 10E9BA, 1013D6/1013DA,
         * 10C354, 10008A + 10F009 as read; the writes are compared. */
        static neo_m2boss_env_t m2;e.m2boss=&m2;e.spawn_reserve=spawn_reserve;free_nodes=w16(0x1063d8);
        flags.mission_end=wram[0xc300];flags.end_kind=wram[0xc304];   /* RAM 10C300/10C304 as read (019424) */
        {int s0=slot(0x10c300),s4=slot(0x10c304);   /* a piece after 018F92: the watched globals start as read */
         if (s0>=0) ram[s0]=expect[s0]=flags.mission_end;
         if (s4>=0) ram[s4]=expect[s4]=flags.end_kind;}
        m2.camera_dy=(int32_t)w32(0x10e99a);m2.point69ba=w16(0x10e9ba);m2.speed[0]=(int32_t)w32(0x1013d6);m2.speed[1]=(int32_t)w32(0x1013da);
        m2.tower_mode=(int16_t)w16(0x10c354);m2.frame_step=(uint8_t)(wram[0x008a]+wram[0xf009]);
#endif
#ifdef FAMILY_M3MID
        static neo_m3mid_env_t m3;m3.layer=&wram[0xe8c6];m3.flags959=wram[0xe959];e.m3mid=&m3;
        e.spawn_reserve=spawn_reserve;free_nodes=w16(0x1063d8);e.shake_pulse=shake_pulse;
#endif
#ifdef FAMILY_RESERVE
        e.spawn_reserve=spawn_reserve;free_nodes=w16(0x1063d8);
#endif
#ifdef FAMILY_LEAVE
        e.become_soldier=leave_soldier;
#endif
        e.camera_low=w16(0x10e9a0);e.camera_stop=w32(0x10e9bc);e.arrow=&wram[0xc44a];
        /* RAM 10C358 (03A892 / 03A740 / 03A75A) when the family watches it. */
        bool watch358=false;
        for (unsigned g=0;g<sizeof(ram_ranges)/sizeof(ram_ranges[0]);g++) if (ram_ranges[g][0]<=0x10c358u && 0x10c358u<ram_ranges[g][1]) watch358=true;
        if (watch358) flags.signal358=wram[0xc358];
        /* RAM 10C700 (0BC542/0BC548, the Mission 3 wagon 038D26) and 10C6BD
         * (the platform 0B9286 / 0B9400) when the family watches them. */
        /* The delay grid RAM 1067E6 (16 x 8 words, 005106 / 00505A) when
         * the family watches it. */
        static uint16_t dgrid[128];bool watchgrid=false;
        for (unsigned g=0;g<sizeof(ram_ranges)/sizeof(ram_ranges[0]);g++) if (ram_ranges[g][0]<=0x1067e6u && 0x1068e6u<=ram_ranges[g][1]) watchgrid=true;
        if (watchgrid) {for (unsigned q=0;q<128;q++) dgrid[q]=w16(0x1067e6+2*q);e.grid=dgrid;}
        bool watch700=false,watch6bd=false,watchm4=false;   /* RAM 10C6C0..10C6FF: the Mission 4 props (neo_m4props.c) */
        for (unsigned g=0;g<sizeof(ram_ranges)/sizeof(ram_ranges[0]);g++) {
            if (ram_ranges[g][0]<=0x10c700u && 0x10c701u<ram_ranges[g][1]) watch700=true;
            if (ram_ranges[g][0]<=0x10c6bdu && 0x10c6bdu<ram_ranges[g][1]) watch6bd=true;
            if (ram_ranges[g][0]<=0x10c6c0u && 0x10c6ffu<ram_ranges[g][1]) watchm4=true;
        }
        if (watch700) flags.limit700=w16(0x10c700);
        if (watch6bd) flags.platform6bd=wram[0xc6bd];
        if (watchm4) memcpy(flags.m4,&wram[0xc6c0],sizeof(flags.m4));
        flags.camera_restart=w16(0x10e9d0);
#endif
#ifdef FAMILY_ATTACKS
        answer_first=ATTACK_SPANS[k][0];answer_count=ATTACK_SPANS[k][1];answer_used=0;
#endif
#ifdef FAMILY_PREVIOUS
        previous_node=PREVIOUS[k];
#endif
        ga_result_t r=neo_region_tick(a,&e);
#ifdef FAMILY_LEAVE
        {static const uint32_t lv[][2]=FAMILY_LEAVE;bool leave=false;
         for (unsigned i=0;i<sizeof(lv)/sizeof(lv[0]);i++) if (lv[i][0]==u->frame && (int)lv[i][1]==u->node) leave=true;
         if (leave) {
            /* JMP 0B0626 / 0B0632 in this update: the node becomes a soldier
             * (the world moves the record; the soldier's code is another
             * test's). The native code keeps the node; what the soldier's
             * entry wrote, its later updates and its children are not
             * compared (compile_family_fixture.py --soldier). */
            if (!(r==GA_MORE && !e.unsupported && leave_entry)) {fprintf(stderr,"frame %u node %d %05x: expected the soldier conversion (result %d, entry %05x, stop %05x)\n",u->frame,u->node,u->routine,r,(unsigned)leave_entry,(unsigned)e.unsupported_at);return 1;}
            leave_entry=0;a->active=false;left++;continue;
         }}
        if (leave_entry) {fprintf(stderr,"frame %u node %d %05x: unexpected soldier conversion %05x\n",u->frame,u->node,u->routine,(unsigned)leave_entry);return 1;}
#endif
#ifdef FAMILY_ATTACKS
        if (answer_used!=answer_count) {fprintf(stderr,"frame %u node %d %05x: %u calls of 0049E8, original %u (result %d, stop %05x)\n",u->frame,u->node,u->routine,answer_used,answer_count,r,(unsigned)e.unsupported_at);return 1;}
#endif
#ifdef FAMILY_RAM
        env_to_ram(&weapons);
        if (watch358) wram[0xc358]=flags.signal358;
        if (watch700) {wram[0xc700]=(uint8_t)(flags.limit700>>8);wram[0xc701]=(uint8_t)flags.limit700;}
        if (watchgrid) for (unsigned q=0;q<128;q++) {wram[(0x67e6+2*q)&0xffff]=(uint8_t)(dgrid[q]>>8);wram[(0x67e7+2*q)&0xffff]=(uint8_t)dgrid[q];}
        if (watch6bd) wram[0xc6bd]=flags.platform6bd;
        if (watchm4) memcpy(&wram[0xc6c0],flags.m4,sizeof(flags.m4));
#ifdef FAMILY_M3MID
        wram[0xe959]=m3.flags959;
#endif
#ifdef FAMILY_M2BOSS
        wram[0xe9ba]=(uint8_t)(m2.point69ba>>8);wram[0xe9bb]=(uint8_t)m2.point69ba;wram[0xc354]=(uint8_t)((uint16_t)m2.tower_mode>>8);wram[0xc355]=(uint8_t)m2.tower_mode;
        for (unsigned i=0;i<8;i++) wram[0x13d6+i]=(uint8_t)((uint32_t)m2.speed[i>>2]>>(24-8*(i&3)));
        wram[0xc300]=flags.mission_end;wram[0xc304]=flags.end_kind;   /* 018F92 */
        if (flags.stage_done) {wram[0xc303]=0;wram[0xc306]=wram[0xc307]=0;}   /* 018F40 (0AAA88: no vehicle) */
#endif
        for (unsigned g=0;g<sizeof(ram_ranges)/sizeof(ram_ranges[0]);g++) for (uint32_t ad=ram_ranges[g][0];ad<ram_ranges[g][1];ad++) {
            /* The player nodes' list links +04..+0B change when a family
             * node next to them is inserted or deleted (0040BC/004188, the
             * node pool is not modelled). */
            if ((ad>=0x1014c8u && ad<0x1014d0u) || (ad>=0x101578u && ad<0x101580u)) continue;
#ifdef FAMILY_RAM_CREATORS
            /* The node pool is RAM only for the creators' reads: what the
             * list management writes there (links, a placeholder creator
             * freed with its last child, 004188) is not modelled. */
            if (ad>=0x1010a4u && ad<0x1010a4u+0xb0u*100u) continue;
#endif
            /* 0049E8's damage to a player (+5A..+5D of 1014C4 / 101574): the
             * attack() stub answers the recorded hit without writing it. */
            if ((ad>=0x10151eu && ad<0x101522u) || (ad>=0x1015ceu && ad<0x1015d2u)) continue;
#ifdef FAMILY_BOSSLIKE
            /* The vertical interrupt's frame counters (0036EE..00381E) may
             * run inside an update. */
            if (ad>=0x10f008u && ad<0x10f00au) continue;
#endif
#if defined(FAMILY_BOSSLIKE) || defined(FAMILY_RESERVE)
            if (ad>=0x1063d8u && ad<0x1063dau) continue;   /* 0040BC's count (free_nodes) */
#endif
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
        for (unsigned h=0;h<2;h++) {static const uint32_t hv[4]={0,0x00054b18u,0x000c0538u,0x000ca3ecu};uint32_t v=hv[flags.player_handlers&3u];for (unsigned i=0;i<4;i++) put(0x10e9f8+4*h+i,(uint8_t)(v>>(24-8*i)));}
#ifdef FAMILY_EXIT_GLOBALS
        if (flags.flash_request) put(0x1067e5,flags.flash_request);
        for (unsigned p=0;p<2;p++) {put(0x10ea00+2*p,(uint8_t)(flags.walk_to[p]>>8));put(0x10ea01+2*p,(uint8_t)flags.walk_to[p]);}
        put(0x10c300,flags.mission_end);put(0x10c304,flags.end_kind);put(0x10c00b,flags.scroll_lock);
        if (flags.wrote400a) {put(0x10c00a,flags.ram400a);flags.wrote400a=0;}
        if (flags.stage_done) put(0x10c303,0);
#endif
        put(0x109944,flags.flash);
        for (int w=0;w<u->wcount;w++) {int s=slot(WRITES[u->wfirst+w][0]);if (s>=0) expect[s]=(uint8_t)WRITES[u->wfirst+w][1];}
        /* 109945 is the palette allocator's (not a family global). */
        ram[0x31]=expect[0x31];
        if (memcmp(ram,expect,sizeof(ram))) {
            for (unsigned s=0;s<sizeof(ram);s++) if (ram[s]!=expect[s]) fprintf(stderr,"frame %u %05x global slot %02x = %02x expected %02x\n",u->frame,u->routine,s,ram[s],expect[s]);
            return 1;
        }
        if (!u->has_post) {
            if (r!=GA_DELETE && r!=GA_DELETE_TREE) {fprintf(stderr,"frame %u node %d %05x: original deletes, native result %d\n",u->frame,u->node,u->routine,r);return 1;}
#ifdef FAMILY_M3MID
            if (r==GA_DELETE_TREE) kill_children(NULL,a);   /* 0041AC: 004192 first */
#endif
            /* 004126 -> 0164D0: a deleted node releases its dynamic object. */
            if (ga_b(a,0x6a)!=0xffu) dyn_release(NULL,a);
            a->active=false;deleted++;continue;
        }
        if (r!=GA_MORE) {fprintf(stderr,"frame %u node %d %05x: native result %d, original keeps it\n",u->frame,u->node,u->routine,r);return 1;}
        if (ga_l(a,0)!=(uint32_t)((uint32_t)u->post[0]<<24|u->post[1]<<16|u->post[2]<<8|u->post[3])) {
            fprintf(stderr,"frame %u node %d %05x -> %05x expected %02x%02x%02x%02x\n",u->frame,u->node,u->routine,ga_l(a,0),u->post[0],u->post[1],u->post[2],u->post[3]);return 1;
        }
        uint32_t rec38=(uint32_t)((uint32_t)u->post[0x38]<<24|u->post[0x39]<<16|u->post[0x3a]<<8|u->post[0x3b]),t38=native_tag(rec38);
        /* The native block tag names a node whose recorded address is the
         * recorded block (a creator deleted and its address reused by a
         * node alive now: the original keeps the address, 01D908). */
        {uint32_t o38=ga_l(a,0x38);unsigned i38=o38&0xffffu;
         if ((o38&0xff000000u)==NEO_GA_TAG_BASE && i38<NN && node_address[i38] && (node_address[i38]==rec38 || node_address[i38]+0x28u==rec38)) t38=o38;}
        for (unsigned o=0x10;o<0xb0;o++) {
            if (skip(o)) continue;
#ifdef FAMILY_FRESH_PROGRAM
            /* The constructor steps the program (00427A) before it starts
             * one: the original steps the previous occupant's program left
             * in +20 for that update (097FCC: 098114 -> 09861E), the world
             * allocates a node without one (ga_alloc), so +24..+27 are not
             * compared in a node's first update while it has no program. */
            if (!a->program && o>=0x24 && o<0x28 && first_update[u->node]) {compared++;continue;}
#endif
            uint8_t want=(o>=0x38 && o<0x3c)?(uint8_t)(t38>>(24-8*(o-0x38))):u->post[o];
#ifdef FAMILY_NODE_FIELDS
            {static const unsigned nf[]=FAMILY_NODE_FIELDS;
             for (unsigned k=0;k<sizeof(nf)/sizeof(nf[0]);k++) if (o>=nf[k] && o<nf[k]+4u) {
                 uint32_t v=native_tag((uint32_t)u->post[nf[k]]<<24|(uint32_t)u->post[nf[k]+1]<<16|(uint32_t)u->post[nf[k]+2]<<8|u->post[nf[k]+3]);
                 want=(uint8_t)(v>>(24-8*(o-nf[k])));
             }}
#endif
#ifdef FAMILY_NODE_FIELDS
            /* A node pointer to a child outside the family (the scratch
             * node: the soldier 0AF17E of the Mission 4 flyer's rider
             * 0842A8, +7A): the original holds an address the fixture
             * does not record. */
            {static const unsigned nf[]=FAMILY_NODE_FIELDS;bool outside=false;
             for (unsigned k=0;k<sizeof(nf)/sizeof(nf[0]);k++) if (o>=nf[k] && o<nf[k]+4u && ga_l(a,nf[k])==NEO_GA_TAG(NN)) outside=true;
             if (outside) {compared++;continue;}}
#endif
#ifdef FAMILY_NODE_FIELDS
            /* A node pointer partly overwritten by byte fields (the shield
             * soldier's +72 under 04097A's +72/+74 bytes): its remaining
             * bytes are the node's address in the original, its tag here. */
            {static const unsigned nf[]=FAMILY_NODE_FIELDS;bool stale=false;
             for (unsigned k=0;k<sizeof(nf)/sizeof(nf[0]) && !stale;k++) if (o>=nf[k] && o<nf[k]+4u && a->m[o]!=want)
                 for (unsigned i=0;i<NN && !stale;i++) {unsigned sh=24u-8u*(o-nf[k]);
                     stale=node_address[i] && ((NEO_GA_TAG(i)>>sh)&0xffu)==a->m[o] && ((node_address[i]>>sh)&0xffu)==want;}
             if (stale) {compared++;continue;}}
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
    if (sgun_born) {
        printf("blasts 0644C4/0646B0: %u born, %u facing left (+15 bit 0)\n",sgun_born,sgun_left);
        if (!sgun_left || sgun_left==sgun_born) {fprintf(stderr,"the blast family must hold shots facing both ways\n");return 1;}
    }
    printf("PASS: family %s%s: %u nodes, %u closed-loop updates (frames %u..%u), %u bytes, %u deletions, %u draws, %u player contacts, %u collision stamps, %u children outside the family, %u deleted by 004192, %u created outside, %u pickups, %u soldiers%s\n",
        STR(FAMILY_NAME),stopped?" (to its explicit stop)":"",(unsigned)NN,(unsigned)NU,UPDATES[0].frame,UPDATES[NU-1].frame,compared,deleted,draws,contacts,stamps,externals,killed_children,injected,pickups_checked,left,swapped?", map swap requested":"");
    return 0;
}
