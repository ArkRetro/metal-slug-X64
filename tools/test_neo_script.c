/* Closed-loop check of the native level-script interpreter against the
 * original updates recorded in four MAME captures (see
 * tools/extract_soldier_script.py). The interpreter runs on its own state for
 * the whole capture; only the inputs the original reads are injected. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MSX_SOLDIER_PROGRAMS 1
#include "../src/neo_script.h"
#include "../src/neo_soldier_ai.h"
#include "soldier_ai_tables.h"
#include "soldier_script_fixture.h"
typedef struct {
    const script_tick_t *t; unsigned rng_used;
    script_spawn_t spawns[8]; neo_script_spawn_t native[8]; unsigned spawn_count;
    int order[NEO_SCRIPT_RUNNERS]; unsigned order_count;
    neo_script_t *s; bool fail; int32_t lock;
} ctx_t;
static uint16_t rng(void *p) {
    ctx_t *c=p;
    if (c->rng_used>=c->t->rng_count) {printf("FAIL frame %u: extra RNG call\n",(unsigned)c->t->frame);c->fail=true;return 0;}
    return c->t->rng[c->rng_used++];
}
static void spawn(void *p,const neo_script_spawn_t *sp) {
    ctx_t *c=p;
    if (c->spawn_count==8) {c->fail=true;return;}
    c->native[c->spawn_count]=*sp;
    script_spawn_t *o=&c->spawns[c->spawn_count++];
    o->creator_record=sp->creator<0?0:neo_script_address(c->s,c->s->runners[sp->creator].record);
    o->group69=sp->group69;o->variant68=sp->variant68;o->construct=NULL;o->record=sp->record;
}
static void lock(void *p,uint16_t x,const uint16_t bounds[4]) {ctx_t *c=p;(void)bounds;c->lock=(int32_t)x;}
static void release(void *p) {ctx_t *c=p;c->lock=-1;}
static void runner(void *p,int creator,unsigned index) {
    ctx_t *c=p;unsigned at=0;
    if (creator>=0) for (unsigned i=0;i<c->order_count;i++) if (c->order[i]==creator) at=i+1;
    memmove(&c->order[at+1],&c->order[at],(c->order_count-at)*sizeof(int));
    c->order[at]=(int)index;c->order_count++;
}
static int cmp_spawn(const void *a,const void *b) {
    const script_spawn_t *x=a,*y=b;
    if (x->creator_record!=y->creator_record) return x->creator_record<y->creator_record?-1:1;
    if (x->record!=y->record) return x->record<y->record?-1:1;
    if (x->group69!=y->group69) return x->group69-y->group69;
    return x->variant68-y->variant68;
}
static int32_t s32(uint32_t v) {return v<=0x7fffffffu?(int32_t)v:(int32_t)((int64_t)v-0x100000000LL);}
static const neo_anim_program_t *program(uint32_t tag) {
    for (unsigned i=0;i<sizeof(soldier_programs)/sizeof(soldier_programs[0]);i++) if (soldier_programs[i].tag==tag) return &soldier_programs[i].program;
    return NULL;
}
/* ROM 0AF17E constructor run on the spawn the native interpreter produced,
 * compared with the fields the original left before the next update. */
static const char *construct(const neo_script_spawn_t *sp,const script_construct_t *k) {
    static char buf[256];buf[0]=0;
    neo_rng_t rng;memcpy(rng.words,k->rng_words,sizeof(rng.words));rng.cursor=k->rng_cursor;neo_rng_t rng0=rng;
    neo_kscene_t kernel={.terrain={k->terrain[0],k->terrain[1],k->terrain[2],k->terrain[3]},.root={k->terrain[0],k->terrain[1],k->terrain[2],k->terrain[3]},.group_map=soldier_group_map};
    uint16_t grid[128];memcpy(grid,k->grid,sizeof(grid));
    neo_soldier_env_t env={0};
    env.kernel=&kernel;env.terrain=(neo_contact_parent_t){0x10c72eu,k->terrain[0],k->terrain[1],k->terrain[2],k->terrain[3],0};
    env.root=env.terrain;env.root.tag=0x10e968u;env.epoch=k->epoch;env.camera_word=k->camera;
    env.players=(neo_control_frame_t){0,0,0,0,k->p1x,k->p1y,k->p2x,k->p2y,k->mask,k->mode88,k->rank,k->mask!=0,NULL};
    env.grid=grid;env.rng=&rng;env.program=program;
    neo_soldier_spawn_t in={sp->x,sp->y,{0},sp->variant68,sp->group69,0};
    for (unsigned i=0;i<6 && i<sp->word_count;i++) in.words[i]=sp->words[i];
    neo_soldier_t s;memset(&s,0,sizeof(s));
    if (!neo_soldier_construct(&s,&in,&env)) return "unsupported constructor";
    const ctor_snapshot_t *t=&k->after;
    #define CHK(name,a,b) if ((uint32_t)(a)!=(uint32_t)(b)) {snprintf(buf+strlen(buf),sizeof(buf)-strlen(buf)," %s=%x/%x",name,(unsigned)(a),(unsigned)(b));}
    CHK("routine",s.routine,t->routine);CHK("x",s.b.x,t->x);CHK("y",s.b.y,t->y);CHK("vx",s.b.vx,t->vx);CHK("vy",s.b.vy,t->vy);
    CHK("px",s.b.projected_x,t->px);CHK("py",s.b.projected_y,t->py);CHK("cx",s.b.cached_x,t->cx);CHK("cy",s.b.cached_y,t->cy);
    CHK("parent",s.b.parent_tag,t->parent);CHK("kind",s.b.kind_angle,t->kind_angle);CHK("f66",s.b.field66,t->field66);
    CHK("1e",s.b.word1e,t->word1e);CHK("f15",s.b.flags15,t->flags15);CHK("f5f",s.b.flags5f,t->flags5f);
    CHK("mark",s.b.mark,t->mark);CHK("corr",s.b.correction,t->correction);CHK("flash",s.anim.flash,t->flash);
    CHK("health",s.health,t->health);CHK("68",s.variant68,t->variant68);CHK("69",s.group69,t->group69);
    CHK("s62",s.b.scalar62,s32(t->scalar62));
    for (unsigned i=0;i<38;i++) if (s.locals[i]!=t->locals[i]) {snprintf(buf+strlen(buf),sizeof(buf)-strlen(buf)," +%02X=%02x/%02x",0x6e + i,s.locals[i],t->locals[i]);break;}
    #undef CHK
    neo_rng_t want=rng0;for (unsigned n=0;n<k->rng_calls;n++) neo_rng_next(&want);
    if (want.cursor!=rng.cursor || memcmp(want.words,rng.words,sizeof(rng.words))) strcat(buf," RNG");
    return buf;
}
int main(void) {
    unsigned constructed=0;
    unsigned ticks=0,spawns=0,draws=0,runs=0;
    for (unsigned ci=0;ci<sizeof(script_captures)/sizeof(script_captures[0]);ci++) {
        const script_capture_t *cap=&script_captures[ci];
        neo_script_t s;neo_script_init(&s,mission1_script,sizeof(mission1_script),MISSION1_SCRIPT_BASE);
        s.pc=(uint16_t)(cap->pc-MISSION1_SCRIPT_BASE);s.timer72=cap->timer72;
        s.w4330=cap->w4330;s.w4332=cap->w4332;s.w69b8=cap->w69b8;s.w69ba=cap->w69ba;
        ctx_t c;memset(&c,0,sizeof(c));c.s=&s;c.lock=cap->lock;
        for (unsigned i=0;i<cap->runner_count;i++) {
            s.runners[i]=(neo_script_runner_t){true,(uint16_t)(cap->runners[i].record-MISSION1_SCRIPT_BASE),cap->runners[i].timer72,cap->runners[i].count76,false};
            c.order[c.order_count++]=(int)i;
        }
        for (unsigned ti=0;ti<cap->tick_count;ti++) {
            const script_tick_t *t=&cap->ticks[ti];
            c.t=t;c.rng_used=0;c.spawn_count=0;
            memcpy(s.groups,t->groups,sizeof(s.groups));
            neo_script_env_t e={t->camera,t->scroll_x,t->scroll_y,
                {{t->players[0].alive,t->players[0].fx,t->players[0].fy,t->players[0].px,t->players[0].py},
                 {t->players[1].alive,t->players[1].fx,t->players[1].fy,t->players[1].px,t->players[1].py}},
                rng,spawn,runner,lock,release,NULL,&c,NULL,false};
            neo_script_tick(&s,&e);
            for (unsigned i=0;i<c.order_count;) {
                int r=c.order[i];
                if (!neo_script_tick_runner(&s,(unsigned)r,&e)) {
                    memmove(&c.order[i],&c.order[i+1],(c.order_count-i-1)*sizeof(int));c.order_count--;
                } else i++;
                runs++;
            }
            bool bad=c.fail||s.halted||c.rng_used!=t->rng_count||neo_script_address(&s,s.pc)!=t->pc||s.timer72!=t->timer72||
                memcmp(s.groups,t->groups_after,9)||c.order_count!=t->runner_count||c.spawn_count!=t->spawn_count||c.lock!=t->lock;
            for (unsigned i=0;!bad && i<c.order_count;i++) {
                const neo_script_runner_t *r=&s.runners[c.order[i]];
                bad=neo_script_address(&s,r->record)!=t->runners[i].record||r->timer72!=t->runners[i].timer72||r->count76!=t->runners[i].count76;
            }
            if (!bad) {
                script_spawn_t want[6];memcpy(want,t->spawns,sizeof(want));
                bool exact=false;for (unsigned i=0;i<t->spawn_count;i++) exact|=t->spawns[i].record!=0;
                for (unsigned i=0;i<c.spawn_count;i++) {
                    if (!exact) c.spawns[i].record=0;
                    else if (c.native[i].constructor!=0xaf17eu) c.spawns[i].group69=c.spawns[i].variant68=0;
                }
                qsort(want,t->spawn_count,sizeof(want[0]),cmp_spawn);qsort(c.spawns,c.spawn_count,sizeof(c.spawns[0]),cmp_spawn);
                for (unsigned i=0;i<c.spawn_count;i++) bad|=cmp_spawn(&want[i],&c.spawns[i])!=0;
                /* Soldier constructions, matched by creator/group/variant. */
                for (unsigned i=0;!bad && i<c.spawn_count;i++) {
                    if (c.native[i].constructor!=0xaf17eu) continue;
                    const neo_script_spawn_t *sp=&c.native[i];
                    uint32_t rec=sp->creator<0?0:neo_script_address(&s,s.runners[sp->creator].record);
                    const script_construct_t *k=NULL;
                    for (unsigned j=0;j<t->spawn_count && !k;j++)
                        if (t->spawns[j].construct && t->spawns[j].creator_record==rec && t->spawns[j].group69==sp->group69 && t->spawns[j].variant68==sp->variant68 &&
                            (!t->spawns[j].record || t->spawns[j].record==sp->record)) k=t->spawns[j].construct;
                    if (!k) {printf("FAIL %s frame %u: soldier spawn without constructor evidence\n",cap->name,(unsigned)t->frame);return 1;}
                    const char *d=construct(sp,k);
                    if (*d) {printf("FAIL %s frame %u: constructor%s\n",cap->name,(unsigned)t->frame,d);return 1;}
                    constructed++;
                }
            }
            if (bad) {
                printf("FAIL %s frame %u: lock %d/%d pc %06X/%06X timer %u/%u runners %u/%u spawns %u/%u rng %u/%u halted %d at %06X\n",cap->name,(unsigned)t->frame,(int)c.lock,(int)t->lock,
                    (unsigned)neo_script_address(&s,s.pc),(unsigned)t->pc,s.timer72,t->timer72,c.order_count,t->runner_count,
                    c.spawn_count,t->spawn_count,c.rng_used,t->rng_count,s.halted,(unsigned)s.halt_address);
                for (unsigned i=0;i<c.order_count;i++) printf("  native runner %06X t%u c%u\n",(unsigned)neo_script_address(&s,s.runners[c.order[i]].record),s.runners[c.order[i]].timer72,s.runners[c.order[i]].count76);
                for (unsigned i=0;i<t->runner_count;i++) printf("  orig runner %06X t%u c%u\n",(unsigned)t->runners[i].record,t->runners[i].timer72,t->runners[i].count76);
                return 1;
            }
            ticks++;spawns+=c.spawn_count;draws+=c.rng_used;
        }
    }
    printf("PASS: %u original script updates (%u runner updates) reproduced natively: position, timers, runners, group counters, %u spawns and %u RNG draws\n",
        ticks,runs,spawns,draws);
    printf("PASS: %u soldier constructions (ROM 0AF17E) from native spawns reproduced (fields, +6E..+93, RNG)\n",constructed);
    return 0;
}
