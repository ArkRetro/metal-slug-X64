/* Playable characters (docs/PERSONAJES.md; private data, make check-characters).
 *  1. The exporter and the loader against the built-in Marco: Marco's own
 *     pack (tools/export_character.py --character 0, build/chars/c0.bin)
 *     must draw exactly what the built-in catalog draws, update by update,
 *     through every form, weapon program, throw, cut, descent, victory,
 *     death and entrance (pseudo-random situations).
 *  2. Tarma, Eri and Fio pose by pose against their own MAME 0.264 captures
 *     (chosen at SOLDIER SELECT with the controller): the cases of Marco's
 *     fixture with each character's programs
 *     (data/character_fixture_cN.json, tools/extract_character_fixture.py). */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "marco_visual.h"
#include "marco_hgun.h"
#include "marco_death.h"
#include "msx_character.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverflow"
#include "marco_campaign.h"
#pragma GCC diagnostic pop
typedef struct {const char *name;int part,form,situation;unsigned program,first,count;} throw_case_t;
/* The same program in the built-in script (from m) and in the pack (from
 * p): command by command, jumps followed (EVENT ids are not compared: the
 * visual ignores them). Returns the commands compared, 0 on a difference. */
static int16_t twin[65536];
static unsigned same_program(const neo_anim_command_t *pc,unsigned pn,unsigned m,unsigned p) {
    unsigned n=0;
    for (;;) {
        if (m>=marco_script.count || p>=pn) return 0;
        if (twin[m]>=0) return twin[m]==(int16_t)p?n+1:0;
        twin[m]=(int16_t)p;
        const neo_anim_command_t *a=&marco_commands[m],*b=&pc[p];n++;
        if (a->kind!=b->kind || a->operand!=b->operand || a->flags!=b->flags || a->dx!=b->dx || a->dy!=b->dy) return 0;
        bool jump=a->kind==NEO_ANIM_JUMP || a->kind==NEO_ANIM_BRANCH_FIELD || a->kind==NEO_ANIM_SELECT_FIELD_POSE;
        if (a->kind!=NEO_ANIM_EVENT && a->asset!=b->asset) return 0;
        if (!jump && a->target!=b->target) return 0;
        if (a->kind==NEO_ANIM_FINISH || a->kind==NEO_ANIM_UNRECOVERED) return n;
        if (a->kind==NEO_ANIM_SELECT_FIELD_POSE) {
            for (unsigned i=0;i<a->operand;i++) {
                const neo_anim_command_t *x=&marco_commands[a->target+i],*y=&pc[b->target+i];
                if (x->kind!=y->kind || x->asset!=y->asset || x->flags!=y->flags || x->dx!=y->dx || x->dy!=y->dy) return 0;
            }
            n+=a->operand;m+=1+a->operand;p+=1+b->operand;continue;
        }
        if (jump) {
            unsigned k=same_program(pc,pn,a->target,b->target);if (!k) return 0;
            n+=k;if (a->kind==NEO_ANIM_JUMP) return n;
        }
        m++;p++;
    }
}
#include "throw_fixture_driver.h"
#include "character_fixture_c1.h"
#include "character_fixture_c2.h"
#include "character_fixture_c3.h"
static uint8_t *read_file(const char *path,uint32_t *size) {
    FILE *f=fopen(path,"rb");if (!f) return NULL;
    fseek(f,0,SEEK_END);long n=ftell(f);fseek(f,0,SEEK_SET);
    uint8_t *b=malloc((size_t)n);if (fread(b,1,(size_t)n,f)!=(size_t)n) {fclose(f);free(b);return NULL;}
    fclose(f);*size=(uint32_t)n;return b;
}
static uint32_t rng=0x1234567u;
static unsigned rnd(unsigned n) {rng=rng*1664525u+1013904223u;return (rng>>8)%n;}
enum { STEPS=60000,DEATH_RUNS=64,DEATH_STEPS=260 };
typedef struct {neo_anim_draw_t body[MARCO_VISUAL_DRAWS],weapon[MARCO_VISUAL_DRAWS];unsigned bn,wn;int br,wr;unsigned stops,failures,starts;} frame_t;
/* A long walk through the visual's inputs; the same sequence each call. */
static void visual_run(frame_t *out) {
    rng=0x1234567u;marco_visual_t v;marco_visual_reset(&v);
    marco_visual_input_t in={.grounded=true,.facing=1,.x=120,.y=350,.hgun_program=-1};
    for (unsigned k=0;k<STEPS;k++) {
        if (rnd(400)==0) {in.form=(uint8_t)rnd(4);in.transforming=in.form>=2 && rnd(2);in.cgun=in.form==1 && rnd(2);in.cgroup=(uint8_t)(1+rnd(4));}
        if (rnd(60)==0) in.transforming=false;
        if (rnd(40)==0) in.moving=rnd(2);
        if (rnd(50)==0) in.crouch=rnd(3)==0;
        if (rnd(50)==0) in.aim_up=rnd(3)==0;
        if (rnd(70)==0) in.facing=rnd(2)?1:-1;
        if (rnd(90)==0) {in.grounded=!in.grounded;in.running_jump=!in.grounded && in.moving;in.takeoff=!in.grounded;}
        else in.takeoff=false;
        in.aim_down=!in.grounded && rnd(4)==0;
        in.fired=rnd(9)==0;in.thrown=rnd(47)==0;in.knife=rnd(83)==0;in.cgun_fire=in.cgun && rnd(11)==0;
        in.hgun_start=false;
        if (in.form==1 && !in.cgun && rnd(25)==0) {in.hgun_program=(int)rnd(7)-1;in.hgun_start=in.hgun_program>=0;}
        if (in.form!=1 || in.cgun) in.hgun_program=-1;
        if (rnd(900)==0) in.hanging=!in.hanging;
        if (rnd(1500)==0) in.victory=!in.victory;
        marco_visual_step(&v,&in);
        frame_t *f=&out[k];memset(f,0,sizeof *f);
        f->bn=v.body_count;f->wn=v.weapon_count;f->br=v.body_role;f->wr=v.weapon_role;f->stops=v.stops;f->failures=v.failures;f->starts=v.starts;
        memcpy(f->body,v.body_draws,v.body_count*sizeof v.body_draws[0]);memcpy(f->weapon,v.weapon_draws,v.weapon_count*sizeof v.weapon_draws[0]);
    }
}
typedef struct {marco_life_phase_t phase;bool drawn,shown,burnt;neo_anim_draw_t draw;uint16_t speed;uint8_t f78,count80;int32_t vx,vy;int facing;bool translated;} death_frame_t;
static void death_run(death_frame_t *out) {
    static const uint8_t kinds[]={0,2,7,0x10,8,0x12,1,5};
    for (unsigned r=0;r<DEATH_RUNS;r++) {
        marco_death_t d;int32_t vx=0x12345,vy=-0x2345;int facing=1;death_frame_t *o=out+r*DEATH_STEPS;
        bool entrance=(r&7)==7,grounded=(r>>3)&1;unsigned land=20+r;
        bool translated=true;
        if (entrance) marco_entrance_start(&d);
        else translated=marco_death_start(&d,kinds[r&7],grounded,(r>>4)&1,&vx,&vy,&facing);
        for (unsigned k=0;k<DEATH_STEPS;k++) {
            bool ground=k>=land;
            if (entrance) marco_entrance_step(&d);
            else {marco_death_velocity(&d,ground,&vx,&vy);marco_death_after(&d,ground,&vx,100,100);}
            o[k]=(death_frame_t){d.phase,d.drawn,d.shown,d.burnt,d.draw,d.anim.speed,d.f78,d.count80,vx,vy,facing,translated};
        }
    }
}
int main(int argc,char **argv) {
    setvbuf(stdout,NULL,_IONBF,0);
    const char *dir=argc>1?argv[1]:"build/chars";char path[256];uint32_t size=0;
    /* 1. Marco's own pack against the built-in catalog. */
    {
        frame_t *a=calloc(STEPS,sizeof *a),*b=calloc(STEPS,sizeof *b);
        death_frame_t *da=calloc(DEATH_RUNS*DEATH_STEPS,sizeof *da),*db=calloc(DEATH_RUNS*DEATH_STEPS,sizeof *db);
        assert(a && b && da && db);
        msx_character_use(NULL);visual_run(a);death_run(da);
        snprintf(path,sizeof path,"%s/c0.bin",dir);uint8_t *blob=read_file(path,&size);
        if (!blob) {printf("FAIL: %s missing (tools/export_character.py --character 0)\n",path);return 1;}
        msx_character_pack_t pack;assert(msx_character_open(&pack,blob,size) && pack.layout.character==0);
        /* Every role: the pack's program is the built-in one. */
        unsigned compared=0;memset(twin,0xff,sizeof twin);
        assert(pack.layout.roles>=100);
        for (unsigned i=0;i<pack.layout.roles;i++) {
            unsigned k=same_program(pack.commands,pack.layout.commands,pack.roles[i][0],pack.roles[i][1]);
            if (!k) {printf("FAIL: role %u of Marco's pack is not the built-in program\n",pack.roles[i][0]);return 1;}
            compared+=k;
        }
        msx_character_use(&pack);visual_run(b);death_run(db);msx_character_use(NULL);
        unsigned drawn=0,roles=0,stops=a[STEPS-1].stops;
        for (unsigned k=0;k<STEPS;k++) {
            if (memcmp(&a[k],&b[k],sizeof a[k])) {printf("FAIL: Marco's pack differs from the built-in catalog at update %u (roles %d/%d vs %d/%d)\n",k,a[k].br,a[k].wr,b[k].br,b[k].wr);return 1;}
            drawn+=a[k].bn+a[k].wn;
        }
        {   /* the roles the walk started */
            static unsigned char seen[65536];
            for (unsigned k=0;k<STEPS;k++) {if (a[k].br>=0 && !seen[a[k].br]) {seen[a[k].br]=1;roles++;} if (a[k].wr>=0 && !seen[a[k].wr]) {seen[a[k].wr]=1;roles++;}}
        }
        if (getenv("CHARACTER_VERBOSE")) printf("failures %u drawn %u roles %u stops %u starts %u\n",a[STEPS-1].failures,drawn,roles,stops,a[STEPS-1].starts);
        assert(!a[STEPS-1].failures && drawn>STEPS && roles>=70);
        unsigned poses=0;
        for (unsigned k=0;k<DEATH_RUNS*DEATH_STEPS;k++) {
            /* padding-free comparison field by field */
            const death_frame_t *x=&da[k],*y=&db[k];
            if (x->phase!=y->phase || x->drawn!=y->drawn || x->shown!=y->shown || x->burnt!=y->burnt || x->speed!=y->speed || x->f78!=y->f78 ||
                x->count80!=y->count80 || x->vx!=y->vx || x->vy!=y->vy || x->facing!=y->facing || x->translated!=y->translated ||
                x->draw.descriptor!=y->draw.descriptor || x->draw.x!=y->draw.x || x->draw.y!=y->draw.y || x->draw.attributes!=y->draw.attributes) {
                printf("FAIL: Marco's pack death/entrance differs at run %u update %u (%06x vs %06x)\n",k/DEATH_STEPS,k%DEATH_STEPS,(unsigned)x->draw.descriptor,(unsigned)y->draw.descriptor);return 1;
            }
            poses+=x->shown;
        }
        assert(poses>DEATH_RUNS*20);
        printf("PASS: Marco's pack (export_character.py --character 0) equals the built-in catalog: %u roles command by command (%u commands); %u updates, %u draws, %u roles started, %u stops; %u death/entrance runs, %u poses\n",
            pack.layout.roles,compared,(unsigned)STEPS,drawn,roles,stops,(unsigned)DEATH_RUNS,poses);
        msx_character_close(&pack);free(blob);free(a);free(b);free(da);free(db);
    }
    /* 2. Each character against its own captures. */
    static const struct {const char *name;const throw_case_t *cases;unsigned count;const unsigned *descriptors;uint16_t voice;} who[4]={
        {"Marco",NULL,0,NULL,0x1123},
        {"Tarma",character1_cases,CHARACTER1_CASES,character1_descriptors,0x1087},
        {"Eri",character2_cases,CHARACTER2_CASES,character2_descriptors,0x1125},
        {"Fio",character3_cases,CHARACTER3_CASES,character3_descriptors,0x114e}};
    assert(msx_character_of_place(0)==0 && msx_character_of_place(1)==2 && msx_character_of_place(2)==1 && msx_character_of_place(3)==3);
    for (unsigned c=1;c<4;c++) {
        snprintf(path,sizeof path,"%s/c%u.bin",dir,c);uint8_t *blob=read_file(path,&size);
        if (!blob) {printf("FAIL: %s missing\n",path);return 1;}
        msx_character_pack_t pack;assert(msx_character_open(&pack,blob,size) && pack.layout.character==c);
        msx_character_use(&pack);assert(msx_character_current()==c && msx_character_death_voice(c)==who[c].voice);
        /* Every pose a role program can draw is in the pack's catalog. */
        const msx_character_layout_t *l=&pack.layout;unsigned absent=0;
        for (unsigned i=0;i<l->commands;i++) if (pack.commands[i].kind==NEO_ANIM_POSE) {
            uint32_t tag=(uint32_t)pack.commands[i].asset;unsigned lo=0,hi=l->poses;
            while (lo<hi) {unsigned mid=lo+(hi-lo)/2;if (msx_character_be32(blob+l->off_poses+mid*12u)<tag) lo=mid+1;else hi=mid;}
            if (lo==l->poses || msx_character_be32(blob+l->off_poses+lo*12u)!=tag) absent++;
        }
        assert(!absent);
        /* The long walk: no program fails to start; the built-in Marco's
         * roles all exist for the character. */
        frame_t *a=calloc(STEPS,sizeof *a);assert(a);visual_run(a);
        msx_character_use(NULL);frame_t *m=calloc(STEPS,sizeof *m);assert(m);visual_run(m);msx_character_use(&pack);
        assert(!a[STEPS-1].failures && a[STEPS-1].stops==m[STEPS-1].stops && a[STEPS-1].starts==m[STEPS-1].starts);
        unsigned differ=0;for (unsigned k=0;k<STEPS;k++) differ+=a[k].wn && m[k].wn && a[k].weapon[0].descriptor!=m[k].weapon[0].descriptor;
        assert(differ>STEPS/4);   /* its own upper body, not Marco's */
        free(a);free(m);
        unsigned ok=0,bad=0;
        if (who[c].count) throw_fixture_run(who[c].cases,who[c].count,who[c].descriptors,&ok,&bad);
        printf("%s: %u cases, %u steps equal, %u different\n",who[c].name,who[c].count,ok,bad);
        if (bad || !who[c].count) {printf("FAIL: %s against its MAME captures\n",who[c].name);return 1;}
        printf("PASS: %s (+6E = %u): %u commands, %u roles, %u poses; every role starts; fire, throws, cuts, descent, victory, fat and mummy pose by pose against its captures\n",
            who[c].name,c,l->commands,l->roles,l->poses);
        msx_character_use(NULL);msx_character_close(&pack);free(blob);
    }
    return 0;
}
