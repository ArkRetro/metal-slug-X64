/* Closed-loop test of the prisoner tally 0308C0 (src/neo_tally.c, the text
 * engine src/neo_text.c) against a MAME family trace with the VRAM write
 * trace (tools/extract_tally_fixture.py -> tools/compile_tally_fixture.py,
 * TALLY_HEADER / TALLY_NAME).
 *
 * The root starts from its recorded bytes when the mission actor creates it;
 * every other node exists only when the native code creates it and is bound
 * to the recorded life of the same creation order (its first recorded bytes
 * are the allocator's stale bytes), with the native creator's writes on top.
 * The updates run by list pass (a pass of the original can span a frame
 * end). Inputs per update are the RAM the original read (region, players, new
 * presses, video counter, epoch, scene root) and its random words in order;
 * the rescued counts, special prisoners and the score start from their first
 * recorded reads and then live in the native state. After every update:
 * the routine and bytes +10..+AF (except the palette slot numbers +14, +16,
 * +19..+1C, the animation pointer +20..+23 and the cached position
 * +44..+4B; the creator +0C and the holder pointer +6E of a counter are
 * compared through the life mapping), every VRAM word written (address and
 * data, in order), the random words consumed, the score and rescued words
 * and the fade targets it wrote. */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#define MSX_SOLDIER_PROGRAMS 1
#include "../src/neo_end.h"
#include "../src/neo_soldier_ai.h"
#include "soldier_ai_tables.h"
typedef struct {
    uint32_t frame,pass;int node;uint32_t routine;uint8_t pre[0xb0];int has_post;uint8_t post[0xb0];
    /* region, video, players, pressed P1/P2, rescued, special 10C5FE/10C5FF,
     * medals, epoch, root correction 10E98C */
    int reads[12];int root[8];int score[4];
    unsigned rng_first,rng_count,writes_first,writes_count,vram_first,vram_count;
} tally_update_t;
#include TALLY_HEADER
#define CAT2(a,b) a##b
#define CAT(a,b) CAT2(a,b)
#define UPDATES CAT(TALLY_NAME,_updates)
#define BIRTHS CAT(TALLY_NAME,_births)
#define CREATORS CAT(TALLY_NAME,_creators)
#define RNG CAT(TALLY_NAME,_rng)
#define WRITES CAT(TALLY_NAME,_writes)
#define VRAM CAT(TALLY_NAME,_vram)
enum { NU=sizeof(UPDATES)/sizeof(UPDATES[0]), NB=sizeof(BIRTHS)/sizeof(BIRTHS[0]) };
static const neo_anim_program_t *program(uint32_t tag) {
    for (unsigned i=0;i<sizeof(soldier_programs)/sizeof(soldier_programs[0]);i++) if (soldier_programs[i].tag==tag) return &soldier_programs[i].program;
    return NULL;
}
static int id_of[NEO_END_NODES];
static uint32_t mame_address[NB+2];   /* life -> MAME node address (from the recorded creator/holder words) */
static const uint8_t *first_pre(int id) {
    for (unsigned i=0;i<NU;i++) if (UPDATES[i].node==id) return UPDATES[i].pre;
    return NULL;
}
static void on_alloc(void *ctx,neo_gactor_t *a,unsigned spawn) {
    (void)ctx;
    int id=spawn==0?0:((int)spawn-1<(int)NB?BIRTHS[spawn-1]:-1);
    assert(id>=0);
    const uint8_t *p=first_pre(id);assert(p);
    uint8_t keep[0x10];memcpy(keep,a->m,0x10);
    memcpy(a->m,p,0xb0);memcpy(a->m,keep,0x10);
    id_of[a->index]=id;
}
static bool skip(unsigned o) {
    return o==0x14 || o==0x16 || (o>=0x19 && o<=0x1c) || (o>=0x20 && o<=0x23) || (o>=0x44 && o<=0x4b);
}
/* The root never starts a program: its 00427A at 030A5E runs the stale
 * program pointer the allocator left (+20, the previous occupant's), whose
 * timer/flash bytes are not reproduced (the native allocator clears). */
static bool stale_animation(int id,unsigned o) {return id==0 && (o==0x17 || o==0x18 || (o>=0x24 && o<=0x27));}
static uint32_t be32(const uint8_t *p) {return (uint32_t)p[0]<<24|p[1]<<16|p[2]<<8|p[3];}
static neo_end_t s;static neo_end_ram_t ram;static neo_hud_t hud;static neo_fade_t fade;
static unsigned cur_lo,cur_hi,updates,bytes,deletes,failures,vram_words,rng_used,rng_at,rng_end;
static uint16_t written[4096][2];static unsigned nwritten;
static void on_write(void *ctx,uint16_t address,uint16_t word) {
    (void)ctx;
    if (nwritten<4096) {written[nwritten][0]=address;written[nwritten][1]=word;}
    nwritten++;
}
static uint16_t rand_word(void *ctx) {
    (void)ctx;
    if (rng_at>=rng_end) {fprintf(stderr,"native random word %u not in MAME\n",rng_at);failures++;return 0;}
    rng_used++;return RNG[rng_at++];
}
static void on_updated(void *ctx,neo_gactor_t *a) {
    (void)ctx;
    unsigned nw=nwritten;nwritten=0;
    if (failures) return;
    int id=id_of[a->index];const tally_update_t *u=NULL;
    for (unsigned i=cur_lo;i<cur_hi;i++) if (UPDATES[i].node==id) {u=&UPDATES[i];break;}
    if (!u) {fprintf(stderr,"frame %u: node %d (%06x) updated natively, not in MAME\n",(unsigned)UPDATES[cur_lo].frame,id,(unsigned)ga_l(a,0));failures++;return;}
    uint32_t f=u->frame,routine=ga_l(a,0);unsigned bad=0;
    /* VRAM words, in order. */
    if (nw!=u->vram_count) {fprintf(stderr,"frame %u node %d routine %06x: %u VRAM words, MAME %u\n",(unsigned)f,id,(unsigned)u->routine,nw,u->vram_count);bad++;}
    for (unsigned i=0;i<nw && i<u->vram_count && i<4096;i++) {
        const uint16_t *w=VRAM[u->vram_first+i];
        if (written[i][0]!=w[0] || written[i][1]!=w[1]) {
            if (bad<8) fprintf(stderr,"frame %u node %d routine %06x: VRAM word %u %04x=%04x, MAME %04x=%04x\n",(unsigned)f,id,(unsigned)u->routine,i,written[i][0],written[i][1],w[0],w[1]);
            bad++;
        } else vram_words++;
    }
    /* RAM the native state keeps: the last write of each byte. */
    for (unsigned i=0;i<u->writes_count;i++) {
        const uint32_t *w=WRITES[u->writes_first+i];
        uint32_t ad=w[0],m=w[1],d=w[2];
        for (unsigned b=0;b<2;b++) {
            if (!(m&(b?0xffu:0xff00u))) continue;
            uint32_t at=ad+b;uint8_t v=(uint8_t)(b?d:d>>8);
            /* later writes of the same byte win */
            bool last=true;
            for (unsigned j=i+1;j<u->writes_count;j++) {
                const uint32_t *x=WRITES[u->writes_first+j];
                if ((x[0]==ad && (x[1]&(b?0xffu:0xff00u)))) last=false;
            }
            if (!last) continue;
            int native=-1;
            if (at>=0x10c46a && at<0x10c472) native=ram.score[0][at-0x10c46a];
            else if (at>=0x10c472 && at<0x10c476) native=ram.score[1][at-0x10c472];
            else if (at==0x10c5fc || at==0x10c5fd) native=(uint8_t)(ram.rescued[0]>>(at&1?0:8));
            else if (at==0x10c600 || at==0x10c601) native=(uint8_t)(ram.rescued[1]>>(at&1?0:8));
            else if (at>=0x109920 && at<0x109926) native=(uint8_t)((uint16_t)fade.target[(at-0x109920)/2]>>(at&1?0:8));
            else if (at==0x10992c) native=fade.count;
            else continue;
            if (native!=v) {fprintf(stderr,"frame %u node %d: RAM %06x native %02x MAME %02x\n",(unsigned)f,id,(unsigned)at,native,v);bad++;}
        }
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
    if (cid<0?wc!=0x101364u:wc!=mame_address[cid]) {fprintf(stderr,"frame %u node %d: creator %d, MAME %06x\n",(unsigned)f,id,cid,(unsigned)wc);bad++;}
    /* A deleting update that returned through 00424A (004188 -> 004126:
     * +1D = 0, palettes freed) keeps its V record. */
    bool deleted=want==0x3f1e;
    if (deleted) deletes++;
    bool counter=u->routine>=0x30b0a && u->routine<=0x30f9a && u->routine!=0x30b08;
    for (unsigned o=0x10;o<0xb0;o++) {
        if (skip(o) || (deleted && o==0x1d) || stale_animation(id,o)) continue;
        if (counter && o>=0x6e && o<0x72) continue;   /* holder pointer: below */
        bytes++;
        if (a->m[o]!=u->post[o]) {
            if (bad<8) fprintf(stderr,"frame %u node %d routine %06x: +%02X native %02x MAME %02x\n",(unsigned)f,id,(unsigned)u->routine,o,a->m[o],u->post[o]);
            bad++;
        }
    }
    if (counter && ga_l(a,0x6e)) {
        uint32_t tag=ga_l(a,0x6e);int hid=(tag&0xff000000u)==NEO_GA_TAG_BASE?id_of[tag&0xffffu]:-1;
        if (hid<0 || mame_address[hid]!=be32(u->post+0x6e)) {fprintf(stderr,"frame %u node %d: holder %d, MAME %06x\n",(unsigned)f,id,hid,(unsigned)be32(u->post+0x6e));bad++;}
    }
    if (bad) failures++;
    updates++;
}
int main(void) {
    neo_end_reset(&s);s.on_alloc=on_alloc;s.on_updated=on_updated;
    /* Life -> MAME node address: a node's creator word (+0C) names its creator's address. */
    for (unsigned i=0;i<NB;i++) {
        const uint8_t *p=first_pre(BIRTHS[i]);
        if (p) mame_address[CREATORS[i]]=be32(p+0xc);
    }
    /* The holder (030B08) address: the counter's +6E after its constructor. */
    for (unsigned i=0;i<NU;i++) {
        const tally_update_t *u=&UPDATES[i];
        if (u->routine==0x30b0a && u->has_post) {
            uint32_t h=be32(u->post+0x6e);
            for (unsigned k=0;k<NB;k++) {
                bool holder=false;
                for (unsigned j=0;j<NU;j++) if (UPDATES[j].node==BIRTHS[k]) {holder=UPDATES[j].routine==0x30b08;break;}
                if (holder && CREATORS[k]==u->node) mame_address[BIRTHS[k]]=h;
            }
        }
    }
    neo_hud_init(&hud,0);
    memset(&ram,0,sizeof(ram));ram.hud=&hud;ram.on_write=on_write;ram.fade=&fade;ram.rand=rand_word;
    /* First recorded reads: rescued counts, special prisoners, scores. */
    bool have_rescued=false,have_score=false;
    for (unsigned i=0;i<NU;i++) {
        const tally_update_t *u=&UPDATES[i];
        if (!have_rescued && u->reads[5]>=0) {ram.rescued[0]=(uint16_t)u->reads[5];have_rescued=true;}
        if (u->reads[6]>=0) ram.special[0][0]=(uint8_t)u->reads[6];
        if (u->reads[7]>=0) ram.special[0][1]=(uint8_t)u->reads[7];
        if (!have_score && u->score[0]>=0) {for (unsigned b=0;b<4;b++) ram.score[0][b]=(uint8_t)u->score[b];have_score=true;}
        if (u->reads[6]>=0) break;
    }
    if (!have_score) for (unsigned i=0;i<NU;i++) if (UPDATES[i].score[0]>=0) {for (unsigned b=0;b<4;b++) ram.score[0][b]=(uint8_t)UPDATES[i].score[b];break;}
    neo_end_inputs_t in;memset(&in,0,sizeof(in));
    in.bounds=(neo_cull_bounds_t){false,0,0,0,0,0,256,320,512};in.program=program;in.ram=&ram;
    unsigned draws=0,frames=0;bool started=false;
    for (unsigned i=0;i<NU;) {
        uint32_t f=UPDATES[i].frame,pass=UPDATES[i].pass;unsigned j=i;
        rng_at=UPDATES[i].rng_first;rng_end=rng_at;
        for (;j<NU && UPDATES[j].pass==pass;j++) {
            const tally_update_t *u=&UPDATES[j];
            if (u->reads[0]>=0) ram.region=(uint8_t)u->reads[0];
            if (u->reads[1]>=0) ram.video=(uint16_t)u->reads[1];
            if (u->reads[2]>=0) ram.players=(uint8_t)u->reads[2];
            if (u->reads[3]>=0) ram.pressed[0]=(uint8_t)u->reads[3];
            if (u->reads[4]>=0) ram.pressed[1]=(uint8_t)u->reads[4];
            if (u->reads[8]>=0) ram.medal[0]=(uint8_t)u->reads[8];
            if (u->reads[9]>=0) ram.medal[1]=(uint8_t)u->reads[9];
            if (u->reads[10]>=0) in.epoch=(uint8_t)u->reads[10];
            if (u->reads[5]>=0 && u->reads[5]!=ram.rescued[0]) {fprintf(stderr,"frame %u: MAME reads rescued %d, native %u\n",(unsigned)f,u->reads[5],ram.rescued[0]);return 1;}
            if (u->root[0]>=0) {
                in.root.tag=0x10e968u;
                in.root.x=(int32_t)((uint32_t)u->root[0]<<16|(uint32_t)u->root[1]);in.root.y=(int32_t)((uint32_t)u->root[2]<<16|(uint32_t)u->root[3]);
                in.root.previous_x=(int32_t)((uint32_t)u->root[4]<<16|(uint32_t)u->root[5]);in.root.previous_y=(int32_t)((uint32_t)u->root[6]<<16|(uint32_t)u->root[7]);
                if (u->reads[11]>=0) in.root.correction=(uint8_t)u->reads[11];
            }
            rng_end=u->rng_first+u->rng_count;
        }
        if (!started) {assert(UPDATES[i].node==0 && UPDATES[i].routine==0x308c0);neo_end_start(&s,0x308c0);started=true;}
        neo_fade_step(&fade);                     /* list head 003918 -> 00698C */
        cur_lo=i;cur_hi=j;nwritten=0;
        unsigned before=updates;
        neo_end_update(&s,&in);frames++;
        draws+=s.draw_count;
        if (s.unsupported) {fprintf(stderr,"frame %u: explicit stop at %06x\n",(unsigned)f,(unsigned)s.unsupported_at);return 1;}
        if (failures) return 1;
        if (updates-before!=j-i) {fprintf(stderr,"frame %u: %u native updates, MAME %u\n",(unsigned)f,updates-before,j-i);return 1;}
        if (rng_at!=rng_end) {fprintf(stderr,"frame %u: %u random words used, MAME %u\n",(unsigned)f,rng_at-UPDATES[i].rng_first,rng_end-UPDATES[i].rng_first);return 1;}
        i=j;
    }
    assert(s.mission60==1);
    printf("prisoner tally 0308C0 (%s): %u passes, %u updates, %u bytes equal, %u VRAM words equal, %u random words, %u deletes, %u draws, %u sounds; score %02x%02x%02x%02x; mission actor +60 set\n",
        TALLY_HEADER,frames,updates,bytes,vram_words,rng_used,deletes,draws,ram.sound_count+ram.sounds_dropped,ram.score[0][0],ram.score[0][1],ram.score[0][2],ram.score[0][3]);
    return 0;
}
