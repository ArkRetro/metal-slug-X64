/* Closed-loop test of the ending actors (src/neo_ending.c, in the end list of
 * src/neo_end.c) against a MAME family trace of the TAS with the VRAM write
 * trace (tools/extract_ending_fixture.py -> tools/compile_ending_fixture.py,
 * ENDING_HEADER / ENDING_NAME).
 *
 * As tools/test_neo_tally.c: the root starts from its recorded bytes when
 * the mission actor creates it; every other node exists only when the native
 * code creates it and is bound to the recorded life of the same creation
 * order. The RAM of the original outside the nodes is a 64 KB image: before
 * each list pass the bytes its updates first read are set from the record;
 * after each node's update the bytes it wrote (last value) must be the
 * record's, and no other. Compared after every update: the routine, the
 * bytes +10..+AF (except the palette slot numbers +14, +16, +19..+1C, the
 * animation pointer +20..+23 and the cached position +44..+4B; the creator
 * +0C through the life mapping), every VRAM word written (address and data,
 * in order), the random words consumed and the RAM written.
 *
 * Not compared, counted: the root's first update 056FCC (palette slots,
 * layers and the sprite clear of the environment, ram->setup: its RAM writes
 * and VRAM words), and the dead address register A2 that 03D292 stores at
 * +E of a text state (10C392..10C395, 10C45E..10C461: taken from the
 * record). */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MSX_SOLDIER_PROGRAMS 1
#include "../src/neo_end.h"
#include "../src/neo_soldier_ai.h"
#include "soldier_ai_tables.h"
typedef struct {
    uint32_t frame,pass;int node;uint32_t routine;uint8_t pre[0xb0];int has_post;uint8_t post[0xb0];
    unsigned reads_first,reads_count,writes_first,writes_count,vram_first,vram_count,rng_first,rng_count;
} ending_update_t;
#include ENDING_HEADER
#define CAT2(a,b) a##b
#define CAT(a,b) CAT2(a,b)
#define UPDATES CAT(ENDING_NAME,_updates)
#define BIRTHS CAT(ENDING_NAME,_births)
#define CREATORS CAT(ENDING_NAME,_creators)
#define READS CAT(ENDING_NAME,_reads)
#define WRITES CAT(ENDING_NAME,_writes)
#define VRAM CAT(ENDING_NAME,_vram)
#define RNG CAT(ENDING_NAME,_rng)
enum { NU=sizeof(UPDATES)/sizeof(UPDATES[0]) };
static const int NB=ENDING_NBIRTHS;
static const neo_anim_program_t *program(uint32_t tag) {
    for (unsigned i=0;i<sizeof(soldier_programs)/sizeof(soldier_programs[0]);i++) if (soldier_programs[i].tag==tag) return &soldier_programs[i].program;
    return NULL;
}
static neo_end_t s;
static int id_of[NEO_END_NODES];
static uint32_t mame_address[ENDING_NLIVES+1];
/* Lives: a root (created by the mission actor or the level script: started from its record) or a birth, bound to the
 * next unused birth of the same creator. */
static bool born[ENDING_NLIVES+1],started_life[ENDING_NLIVES+1],birth_used[ENDING_NBIRTHS+1];
static int pending_root=-1;
static bool is_setup(uint32_t routine) {return routine==0x570a0u || routine==0x57e00u || routine==0xa7ffeu || routine==0xa8096u;}
static const uint8_t *first_pre(int id) {
    for (unsigned i=0;i<NU;i++) if (UPDATES[i].node==id) return UPDATES[i].pre;
    return NULL;
}
static void on_alloc(void *ctx,neo_gactor_t *a,unsigned spawn) {
    (void)ctx;(void)spawn;
    int id=-1;
    if (s.creator[a->index]<0) id=pending_root;
    else {
        int c=id_of[(int)s.creator[a->index]];
        for (int i=0;i<NB;i++) if (!birth_used[i] && CREATORS[i]==c) {birth_used[i]=true;id=BIRTHS[i];break;}
    }
    if (id<0) {fprintf(stderr,"node %06x created natively, not in MAME\n",(unsigned)ga_l(a,0));exit(1);}
    const uint8_t *p=first_pre(id);assert(p);
    uint8_t keep[0x10];memcpy(keep,a->m,0x10);
    memcpy(a->m,p,0xb0);memcpy(a->m,keep,0x10);
    id_of[a->index]=id;
}
static bool skip(unsigned o) {
    return o==0x14 || o==0x16 || (o>=0x19 && o<=0x1c) || (o>=0x20 && o<=0x23) || (o>=0x44 && o<=0x4b);
}
static bool dead_register(uint32_t ad) {return (ad>=0x10c392 && ad<0x10c396) || (ad>=0x10c45e && ad<0x10c462);}
static uint32_t be32(const uint8_t *p) {return (uint32_t)p[0]<<24|p[1]<<16|p[2]<<8|p[3];}
static neo_end_ram_t ram;static neo_hud_t hud;static neo_fade_t fade;
static uint8_t img[0x10000];
static unsigned edge_updates,cur_lo,cur_hi,updates,bytes,deletes,failures,vram_words,rng_used,rng_at,rng_end,ram_bytes,dead_bytes,setup_words,skipped_writes,setup_rng,setup_fix;
static uint16_t written[8192][2];static unsigned nwritten;
static uint32_t wlog[4096];static unsigned nwlog;
static void on_write(void *ctx,uint16_t address,uint16_t word) {
    (void)ctx;
    if (nwritten<8192) {written[nwritten][0]=address;written[nwritten][1]=word;}
    nwritten++;
}
static uint8_t rd(void *ctx,uint32_t ad) {(void)ctx;return img[ad&0xffffu];}
static bool skipped(uint32_t ad) {
    static const uint32_t sk[][2]=ENDING_SKIP;
    for (unsigned i=0;i<sizeof(sk)/sizeof(sk[0]);i++) if (ad>=sk[i][0] && ad<sk[i][1]) return true;
    return false;
}
/* The work RAM 100000..10FFFF is mirrored up to 1FFFFF (057D18 with d1 = 7FFF writes 14F008 = 10F008). */
static void wr(void *ctx,uint32_t ad,uint8_t v) {
    (void)ctx;ad=0x100000u|(ad&0xffffu);img[ad&0xffffu]=v;
    if (skipped(ad)) {skipped_writes++;return;}
    for (unsigned i=0;i<nwlog && i<4096;i++) if (wlog[i]==ad) return;
    if (nwlog<4096) wlog[nwlog]=ad;
    nwlog++;
}
/* 056FCC: the environment's writes (palette slots, layers, 10FDxx) are the record's. */
static neo_end_inputs_t in;
static void root_from_ram(void) {
    in.root.tag=0x10e968u;
    in.root.x=(int32_t)be32(&img[0xe97c]);in.root.y=(int32_t)be32(&img[0xe980]);
    in.root.previous_x=(int32_t)be32(&img[0xe984]);in.root.previous_y=(int32_t)be32(&img[0xe988]);in.root.correction=img[0xe98d];   /* +25 (01AC00) */
}
static void setup(void *ctx) {
    (void)ctx;
    for (unsigned i=cur_lo;i<cur_hi;i++) {
        const ending_update_t *u=&UPDATES[i];
        if (born[u->node] || !is_setup(u->routine)) continue;
        for (unsigned k=0;k<u->writes_count;k++) img[WRITES[u->writes_first+k][0]&0xffffu]=(uint8_t)WRITES[u->writes_first+k][1];
        /* Its random words (0029C6 in the environment's reset) are the environment's too. */
        if (rng_at==u->rng_first) {rng_at+=u->rng_count;setup_rng+=u->rng_count;}
    }
    root_from_ram();
}
static unsigned rng_n;
static uint16_t rand_word(void *ctx) {
    (void)ctx;
    if (rng_at>=rng_end) {fprintf(stderr,"native random word %u not in MAME\n",rng_at);failures++;return 0;}
    rng_used++;rng_n=(RNG[rng_at]>>16)&1u;return (uint16_t)RNG[rng_at++];
}
static bool rand_negative(void *ctx) {(void)ctx;return rng_n!=0;}
/* Before a node's update: the bytes it first reads, as recorded (the vertical blank inside a pass changes the epoch
 * 10C33A for the nodes after it). */
static void on_update(void *ctx,neo_gactor_t *a) {
    (void)ctx;
    int id=id_of[a->index];
    for (unsigned i=cur_lo;i<cur_hi;i++) if (UPDATES[i].node==id) {
        const ending_update_t *u=&UPDATES[i];
        for (unsigned k=0;k<u->reads_count;k++) img[READS[u->reads_first+k][0]&0xffffu]=(uint8_t)READS[u->reads_first+k][1];
        if (u->reads_count) in.epoch=img[0xc33a];
        root_from_ram();
        break;
    }
}
static void on_updated(void *ctx,neo_gactor_t *a) {
    (void)ctx;
    unsigned nw=nwritten,nl=nwlog;nwritten=0;nwlog=0;
    if (failures) return;
    int id=id_of[a->index];const ending_update_t *u=NULL;
    for (unsigned i=cur_lo;i<cur_hi;i++) if (UPDATES[i].node==id) {u=&UPDATES[i];break;}
    /* The last pass of a frame window can be cut by the window's end: its missing updates are not compared. */
    if (!u && cur_hi==NU) {edge_updates++;return;}
    if (!u) {fprintf(stderr,"frame %u: node %d (%06x) updated natively, not in MAME\n",(unsigned)UPDATES[cur_lo].frame,id,(unsigned)ga_l(a,0));failures++;return;}
    uint32_t f=u->frame,routine=ga_l(a,0);unsigned bad=0;
    bool setup=is_setup(u->routine) && !born[id];
    if (setup) {
        /* An environment reset: its sprite and layer words are the environment's; its fix-map words (7000..74FF: the
         * title's texts of 0A863C) are the node's, compared in order. */
        unsigned k=0;
        for (unsigned i=0;i<u->vram_count;i++) {
            const uint16_t *w=VRAM[u->vram_first+i];
            if (w[0]<0x7000u || w[0]>=0x7500u) {setup_words++;continue;}
            if (k>=nw || k>=8192 || written[k][0]!=w[0] || written[k][1]!=w[1]) {
                if (bad<8) fprintf(stderr,"frame %u node %d routine %06x: fix word %u of the reset: MAME %04x=%04x, native %04x=%04x\n",(unsigned)f,id,(unsigned)u->routine,k,w[0],w[1],k<nw?written[k][0]:0,k<nw?written[k][1]:0);
                bad++;
            } else {vram_words++;setup_fix++;}
            k++;
        }
        if (k!=nw) {fprintf(stderr,"frame %u node %d routine %06x: %u fix words in the reset, MAME %u\n",(unsigned)f,id,(unsigned)u->routine,nw,k);bad++;}
    } else {
        if (nw!=u->vram_count) {fprintf(stderr,"frame %u node %d routine %06x: %u VRAM words, MAME %u\n",(unsigned)f,id,(unsigned)u->routine,nw,u->vram_count);bad++;}
        for (unsigned i=0;i<nw && i<u->vram_count && i<8192;i++) {
            const uint16_t *w=VRAM[u->vram_first+i];
            if (written[i][0]!=w[0] || written[i][1]!=w[1]) {
                if (bad<8) fprintf(stderr,"frame %u node %d routine %06x: VRAM word %u %04x=%04x, MAME %04x=%04x\n",(unsigned)f,id,(unsigned)u->routine,i,written[i][0],written[i][1],w[0],w[1]);
                bad++;
            } else vram_words++;
        }
    }
    /* RAM: the record's last values, and nothing else written. */
    for (unsigned i=0;i<u->writes_count;i++) {
        uint32_t ad=WRITES[u->writes_first+i][0];uint8_t v=(uint8_t)WRITES[u->writes_first+i][1];
        if (setup) {img[ad&0xffffu]=v;continue;}
        if (dead_register(ad)) {img[ad&0xffffu]=v;dead_bytes++;continue;}
        bool mine=false;for (unsigned k=0;k<nl && k<4096;k++) if (wlog[k]==ad) mine=true;
        if (!mine) {if (bad<8) fprintf(stderr,"frame %u node %d routine %06x: MAME writes RAM %06x = %02x, native does not\n",(unsigned)f,id,(unsigned)u->routine,(unsigned)ad,v);bad++;continue;}
        if (img[ad&0xffffu]!=v) {if (bad<8) fprintf(stderr,"frame %u node %d routine %06x: RAM %06x native %02x MAME %02x\n",(unsigned)f,id,(unsigned)u->routine,(unsigned)ad,img[ad&0xffffu],v);bad++;}
        else ram_bytes++;
    }
    if (!setup) for (unsigned k=0;k<nl && k<4096;k++) {
        bool rec=false;for (unsigned i=0;i<u->writes_count;i++) if (WRITES[u->writes_first+i][0]==wlog[k]) rec=true;
        if (!rec) {if (bad<8) fprintf(stderr,"frame %u node %d routine %06x: native writes RAM %06x, MAME does not\n",(unsigned)f,id,(unsigned)u->routine,(unsigned)wlog[k]);bad++;}
    }
    if (!u->has_post) {
        if (routine!=0x3f1e) {fprintf(stderr,"frame %u node %d: should delete (routine %06x)\n",(unsigned)f,id,(unsigned)routine);bad++;}
        if (bad) failures++;
        deletes++;updates++;return;
    }
    uint32_t want=be32(u->post);
    if (routine!=want) {fprintf(stderr,"frame %u node %d: routine %06x, MAME %06x\n",(unsigned)f,id,(unsigned)routine,(unsigned)want);bad++;}
    int cid=s.creator[a->index]<0?-1:id_of[(int)s.creator[a->index]];
    uint32_t wc=be32(u->post+0xc);
    if (cid<0?wc!=be32(first_pre(id)+0xc):wc!=mame_address[cid]) {fprintf(stderr,"frame %u node %d: creator %d, MAME %06x\n",(unsigned)f,id,cid,(unsigned)wc);bad++;}
    bool deleted=want==0x3f1e;
    if (deleted) deletes++;
    for (unsigned o=0x10;o<0xb0;o++) {
        if (skip(o) || (deleted && o==0x1d)) continue;
        bytes++;
        if (a->m[o]!=u->post[o]) {
            if (bad<8) fprintf(stderr,"frame %u node %d routine %06x: +%02X native %02x MAME %02x\n",(unsigned)f,id,(unsigned)u->routine,o,a->m[o],u->post[o]);
            bad++;
        }
    }
    if (bad) failures++;
    updates++;
}
int main(void) {
    neo_end_reset(&s);s.on_alloc=on_alloc;s.on_updated=on_updated;s.on_update=on_update;
    for (int i=0;i<NB;i++) {
        const uint8_t *p=first_pre(BIRTHS[i]);
        if (p) mame_address[CREATORS[i]]=be32(p+0xc);
    }
    for (int i=0;i<NB;i++) born[BIRTHS[i]]=true;
    neo_hud_init(&hud,0);
    memset(&ram,0,sizeof(ram));ram.hud=&hud;ram.on_write=on_write;ram.fade=&fade;ram.rand=rand_word;ram.rand_negative=rand_negative;ram.rd=rd;ram.wr=wr;ram.setup=setup;
    memset(&in,0,sizeof(in));
    in.bounds=(neo_cull_bounds_t){false,0,0,0,0,0,256,320,512};in.program=program;in.ram=&ram;
    unsigned draws=0,frames=0,reads=0;bool started=false;
    for (unsigned i=0;i<NU;) {
        uint32_t f=UPDATES[i].frame,pass=UPDATES[i].pass;unsigned j=i;
        rng_at=UPDATES[i].rng_first;rng_end=rng_at;
        for (;j<NU && UPDATES[j].pass==pass;j++) {
            const ending_update_t *u=&UPDATES[j];
            for (unsigned k=0;k<u->reads_count;k++) {img[READS[u->reads_first+k][0]&0xffffu]=(uint8_t)READS[u->reads_first+k][1];reads++;}
            if (u->reads_count) {uint8_t e=img[0xc33a];in.epoch=e;}
            /* The scene root 10E968 as 01ACF2 / 01ABC8 read it (10E97C.. x, y, previous x, y; 10E98C). */
            root_from_ram();
            rng_end=u->rng_first+u->rng_count;
        }
        /* The roots first updated in this pass, the last of the pass first: 0040BC puts a node right after its
         * creator, so the newest is the first of the list. */
        for (unsigned k=j;k-->i;) {
            const ending_update_t *u=&UPDATES[k];
            if (born[u->node] || started_life[u->node]) continue;
            started_life[u->node]=true;pending_root=u->node;
            if (!neo_end_start(&s,u->routine)) {fprintf(stderr,"frame %u: no node for the root %06x\n",(unsigned)f,(unsigned)u->routine);return 1;}
            started=true;
        }
        assert(started);
        neo_fade_step(&fade);
        cur_lo=i;cur_hi=j;nwritten=0;nwlog=0;
        unsigned before=updates;
        neo_end_update(&s,&in);frames++;
        draws+=s.draw_count;
        if (s.unsupported) {fprintf(stderr,"frame %u: explicit stop at %06x\n",(unsigned)f,(unsigned)s.unsupported_at);return 1;}
        if (failures) return 1;
        if (updates-before!=j-i && j!=NU) {fprintf(stderr,"frame %u: %u native updates, MAME %u\n",(unsigned)f,updates-before,j-i);return 1;}
        if (rng_at!=rng_end) {fprintf(stderr,"frame %u: %u random words used, MAME %u\n",(unsigned)f,rng_at-UPDATES[i].rng_first,rng_end-UPDATES[i].rng_first);return 1;}
        i=j;
    }
    printf("PASS: ending %s (root %06x): %u passes, %u updates, %u node bytes, %u VRAM words (%u of them fix words of a reset), %u RAM bytes equal, %u RAM bytes read, %u random words, %u deletes, %u draws, %u sounds, %u audio commands, %u slot levels; not compared: %u VRAM words and %u random words of the reset (056FCC / 057F34), %u dead A2 bytes, %u writes in the skipped RAM, %u updates past the window's end; creator +60 %s\n",
        ENDING_HEADER,(unsigned)ENDING_ROOT,frames,updates,bytes,vram_words,setup_fix,ram_bytes,reads,rng_used,deletes,draws,ram.sound_count+ram.sounds_dropped,ram.audio_commands,ram.slot_levels,setup_words,setup_rng,dead_bytes,skipped_writes,edge_updates,s.mission60?"set":"clear");
    return 0;
}
