/* The credits' stage 0x2C in the native world (src/neo_world.c with the ending's nodes of src/neo_ending.c dispatched
 * from its list) against the TAS (captures-m6/fam/T1: tools/extract_credits_world.py, docs/MISION6.md "Los créditos
 * en el mundo").
 *
 * The world starts the stage as the game does (neo_world_init_stage with the script's section 22, ROM 2E57D4, and the
 * camera list 025F3E) with the random ring of the update before it (RAM 100016.., the capture's dump): the script's
 * two random choices create the two figures 0A8C62, their sub-scripts 019E72 (records of the table 00087A) create the
 * pages, timers, music, pictures and the audio command, all natively. Per list pass, every update of the script's
 * descendants must be the capture's, in list order: the same routine, and the bytes +10..+AF before and after it
 * (a 32-bit FNV-1a of them, without the palette slot numbers +14, +16, +19..+1C, the animation pointer +20..+23 and
 * the cached position +44..+4B, as tools/test_neo_ending.c). The sub-script nodes 019E72 / 019E82 keep their state in
 * the native interpreter: their place in the list and their births are compared, not their bytes. Injected: the
 * buttons held (RAM 10C35C as the music actor 0A4408 read it). */
#define MSX_SOLDIER_PROGRAMS 1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../src/neo_world.h"
#include "../src/neo_end.h"
#include "soldier_ai_tables.h"
#include "credits_world_fixture.h"
static neo_world_t w;static neo_end_ram_t ram;static neo_hud_t hud;static neo_fade_t fade;
static uint8_t mem_c604,mem_held;static unsigned stray;
static uint8_t rd(void *c,uint32_t ad) {
    (void)c;
    switch (ad) {
    case 0x10c604: return mem_c604;
    case 0x10c35c: return mem_held;
    case 0x10fdb6: return 3;        /* player 1 in play */
    case 0x10fdb7: case 0x10c362: case 0x100002: return 0;
    default: stray++;return 0;
    }
}
static void wr(void *c,uint32_t ad,uint8_t v) {(void)c;if (ad==0x10c604) mem_c604=v; else stray++;}
static uint16_t rnd(void *c) {(void)c;return neo_rng_next(&w.rng);}
static bool skip(unsigned o) {return o==0x14 || o==0x16 || (o>=0x19 && o<=0x1c) || (o>=0x20 && o<=0x23) || (o>=0x44 && o<=0x4b);}
static uint32_t fnv(const neo_gactor_t *a) {
    uint32_t h=0x811c9dc5u;
    for (unsigned o=0x10;o<0xb0;o++) if (!skip(o)) h=(h^a->m[o])*0x01000193u;
    return h;
}
static bool sub(uint32_t r) {return r==0x19e72u || r==0x19e82u;}
static unsigned pass,at,failures,compared,births,adopted,born_bytes;
static int previous=-1;static unsigned previous_at;
static int life_of[NEO_WORLD_GACTORS];static bool bound[NEO_WORLD_GACTORS];static bool life_used[CREDITS_LIVES+1];
static void check_post(void) {
    if (previous<0) return;
    const neo_gactor_t *a=&w.gactors[previous];unsigned k=previous_at;previous=-1;
    if (!credits_updates[k].has_post) {
        if (a->active && !a->placeholder && ga_l(a,0)!=0x3f1e) {fprintf(stderr,"pass %u (frame %u) update %u: life %d should be deleted (routine %06x)\n",pass,(unsigned)credits_passes[pass].frame,k,credits_updates[k].life,(unsigned)ga_l(a,0));failures++;}
        return;
    }
    if (sub(credits_updates[k].routine)) return;
    if (fnv(a)!=credits_updates[k].post) {fprintf(stderr,"pass %u (frame %u): life %d routine %06x: bytes after the update differ\n",pass,(unsigned)credits_passes[pass].frame,credits_updates[k].life,(unsigned)credits_updates[k].routine);failures++;}
    else compared++;
}
static void trace(void *ctx,uint8_t kind,uint8_t index) {
    (void)ctx;
    check_post();
    if (kind!=NEO_WORLD_GACTOR || failures) return;
    const neo_gactor_t *a=&w.gactors[index];
    if (a->placeholder || ga_l(a,0)==0x3f1e) return;
    unsigned end=credits_passes[pass].first+credits_passes[pass].count;
    if (at>=end) {fprintf(stderr,"pass %u (frame %u): node %06x updated natively, not in MAME\n",pass,(unsigned)credits_passes[pass].frame,(unsigned)ga_l(a,0));failures++;return;}
    unsigned k=at++;int life=credits_updates[k].life;
    if (!bound[index] || life_of[index]!=life) {
        /* A birth: the life must be new, and its creator's life the record's. */
        if (life_used[life]) {fprintf(stderr,"pass %u (frame %u): native node %u (%06x) at the place of life %d, already bound\n",pass,(unsigned)credits_passes[pass].frame,index,(unsigned)ga_l(a,0),life);failures++;return;}
        life_used[life]=true;bound[index]=true;life_of[index]=life;births++;
        /* The bytes the creator wrote must be the record's; the others (004126 clears some and leaves the previous
         * occupant's in the rest) are taken from it. */
        neo_gactor_t *n=&w.gactors[index];
        for (unsigned o=0x10;o<0xb0;o++) {
            if (skip(o)) continue;
            if (!(n->written[o>>3]&(1u<<(o&7u)))) {n->m[o]=credits_first[life][o];adopted++;continue;}
            if (n->m[o]!=credits_first[life][o] && !sub(ga_l(n,0))) {fprintf(stderr,"pass %u (frame %u): life %d (%06x) born with +%02X = %02x, MAME %02x\n",pass,(unsigned)credits_passes[pass].frame,life,(unsigned)ga_l(n,0),o,n->m[o],credits_first[life][o]);failures++;}
            else born_bytes++;
        }
        uint32_t c=ga_l(a,0x0c);int cl=-1;
        if ((c&0xff000000u)==NEO_GA_TAG_BASE && (c&0xffffu)<NEO_WORLD_GACTORS && bound[c&0xffffu]) cl=life_of[c&0xffffu];
        if (cl!=credits_creators[life]) {fprintf(stderr,"pass %u (frame %u): life %d (%06x) created by life %d, MAME %d\n",pass,(unsigned)credits_passes[pass].frame,life,(unsigned)ga_l(a,0),cl,credits_creators[life]);failures++;return;}
    }
    if (ga_l(a,0)!=credits_updates[k].routine) {fprintf(stderr,"pass %u (frame %u): life %d routine %06x, MAME %06x\n",pass,(unsigned)credits_passes[pass].frame,life,(unsigned)ga_l(a,0),(unsigned)credits_updates[k].routine);failures++;return;}
    if (getenv("CREDITS_DUMP") && pass>=(unsigned)atoi(getenv("CREDITS_DUMP"))) {fprintf(stderr,"D %u %d %06x ",pass,life,(unsigned)ga_l(a,0));for (unsigned o=0;o<0xb0;o++) fprintf(stderr,"%02x",a->m[o]);fprintf(stderr,"\n");}
    if (!sub(credits_updates[k].routine) && fnv(a)!=credits_updates[k].pre) {fprintf(stderr,"pass %u (frame %u): life %d routine %06x: bytes before the update differ\n",pass,(unsigned)credits_passes[pass].frame,life,(unsigned)credits_updates[k].routine);failures++;return;}
    previous=index;previous_at=k;
}
int main(void) {
    const uint8_t *script=ga_rom(0x2e57d4),*camera=ga_rom(0x25f3e);
    if (!script || !camera) {fprintf(stderr,"no ROM blob for the credits' script or camera list\n");return 1;}
    neo_world_stage_t st={script,0x2e598c-0x2e57d4,0x2e57d4,camera,0x25fa0-0x25f3e,0x25f3e};
    neo_world_init(&w,0);
    neo_world_init_stage(&w,0,&st);
    memcpy(w.rng.words,credits_ring,sizeof(w.rng.words));w.rng.cursor=CREDITS_CURSOR;
    neo_rng_next(&w.rng);   /* 0180AA -> 08BFC6: the stage start's word (RAM 10C5EA), as msx_game_begin_stage */
    neo_hud_init(&hud,0);memset(&ram,0,sizeof(ram));
    ram.hud=&hud;ram.fade=&fade;ram.rand=rnd;ram.rd=rd;ram.wr=wr;
    w.trace=trace;w.end_ram=&ram;
    neo_world_player_t p;memset(&p,0,sizeof(p));p.inactive=true;p.unregistered=true;
    unsigned draws=0,mixed=0,sounds=0;
    for (pass=0;pass<CREDITS_PASSES;pass++) {
        at=credits_passes[pass].first;mem_held=credits_passes[pass].held;
        neo_world_camera(&w,&p);
        w.video_phase=(uint16_t)pass;w.scroll_y=neo_camera_root_y(&w.camera);
        neo_world_tick(&w,neo_camera_root_x(&w.camera),&p);
        check_post();
        if (w.unsupported) {fprintf(stderr,"pass %u (frame %u): explicit stop at %06x\n",pass,(unsigned)credits_passes[pass].frame,(unsigned)w.unsupported_routine);return 1;}
        if (w.script.halted) {fprintf(stderr,"pass %u: the script halted at %06x\n",pass,(unsigned)w.script.halt_address);return 1;}
        if (failures) return 1;
        if (at!=credits_passes[pass].first+credits_passes[pass].count) {fprintf(stderr,"pass %u (frame %u): %u native updates, MAME %u\n",pass,(unsigned)credits_passes[pass].frame,at-credits_passes[pass].first,credits_passes[pass].count);return 1;}
        draws+=w.draw_count;
        for (unsigned k=0;k<w.draw_count;k++) if (w.draws[k].mix_weight) mixed++;
        sounds+=ram.sound_count;ram.sound_count=0;w.sound_count=0;
    }
    if (!w.script.finished) {fprintf(stderr,"the script did not end (op 3) in the last pass\n");return 1;}
    printf("PASS: credits in the world: %u passes, %u updates equal (bytes before and after), %u births bound to MAME's lives (%u bytes written at birth equal, %u unwritten taken from the record), %u draws (%u with a run-time palette mix), %u sounds, %u audio commands, script ended in the last pass; mix overflow %u, missing %u, stray RAM %u\n",
        pass,compared,births,born_bytes,adopted,draws,mixed,sounds,ram.audio_commands,w.mix_overflow,w.mix_missing,stray);
    return w.mix_overflow || w.mix_missing || stray;
}
