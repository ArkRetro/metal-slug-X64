#include <assert.h>
#include <stdio.h>
#include <string.h>
#define MSX_SOLDIER_PROGRAMS 1
#include "../src/neo_soldier_ai.h"
#include "soldier_ai_tables.h"
#include "soldier_tick_fixture.h"
#define COUNT(a) (sizeof(a)/sizeof((a)[0]))
static int32_t s32(uint32_t v) {return v<=0x7fffffffu?(int32_t)v:(int32_t)((int64_t)v-0x100000000LL);}
static const neo_anim_program_t *program(uint32_t tag) {
    for (unsigned i=0;i<COUNT(soldier_programs);i++) if (soldier_programs[i].tag==tag) return &soldier_programs[i].program;
    return NULL;
}
static bool locate(uint32_t address,const neo_anim_program_t **prog,uint16_t *cursor,const int32_t **addr) {
    for (unsigned i=0;i<COUNT(soldier_programs);i++)
        for (unsigned j=0;j<soldier_programs[i].program.count;j++)
            if ((uint32_t)soldier_programs[i].address[j]==address) {*prog=&soldier_programs[i].program;*cursor=(uint16_t)j;*addr=soldier_programs[i].address;return true;}
    return false;
}
static bool load(neo_soldier_t *s,const tick_snapshot_t *t) {
    memset(s,0,sizeof(*s));s->active=true;s->routine=t->routine;
    neo_contact_actor_t *b=&s->b;
    b->x=s32(t->x);b->y=s32(t->y);b->vx=s32(t->vx);b->vy=s32(t->vy);b->scalar62=s32(t->scalar62);
    b->projected_x=s32(t->px);b->projected_y=s32(t->py);b->cached_x=s32(t->cx);b->cached_y=s32(t->cy);
    b->parent_tag=t->parent;b->kind_angle=(uint16_t)t->kind_angle;b->field66=(uint16_t)t->field66;b->word1e=(uint16_t)t->word1e;
    b->flags15=(uint8_t)t->flags15;b->flags5f=(uint8_t)t->flags5f;b->timer5a=(uint8_t)t->timer5a;b->mark=(uint8_t)t->mark;b->correction=(uint8_t)t->correction;
    s->anim.timer=(uint16_t)t->timer;s->anim.speed=(uint16_t)t->speed;s->anim.flash=(uint8_t)t->flash;s->anim.field18=(uint8_t)t->field18;
    s->anim.field50_tag=t->field50;
    /* A freshly constructed node still holds the previous occupant's script
     * pointer; entry routines start their own program before stepping it. */
    if (t->anim) {const int32_t *a;if (!locate(t->anim,&s->anim.program,&s->anim.cursor,&a) && t->routine!=0xaf65au && t->routine!=0xaf380u) return false;}
    s->render.attributes=(uint16_t)(0x7100u|b->flags15);
    s->health=(uint16_t)t->health;s->f5b=(uint8_t)t->f5b;s->f5c=(uint8_t)t->f5c;s->f5d=(uint8_t)t->f5d;s->f5e=(uint8_t)t->f5e;s->variant68=(uint8_t)t->variant68;s->group69=(uint8_t)t->group69;
    memcpy(s->locals,t->locals,sizeof(s->locals));
    return true;
}
static const char *diff(const neo_soldier_t *s,const tick_snapshot_t *t) {
    static char buf[256];buf[0]=0;
    #define CHK(name,a,b) if ((uint32_t)(a)!=(uint32_t)(b)) {snprintf(buf+strlen(buf),sizeof(buf)-strlen(buf)," %s=%x/%x",name,(unsigned)(a),(unsigned)(b));}
    CHK("routine",s->routine,t->routine);CHK("x",s->b.x,t->x);CHK("y",s->b.y,t->y);CHK("vx",s->b.vx,t->vx);CHK("vy",s->b.vy,t->vy);
    CHK("s62",s->b.scalar62,t->scalar62);CHK("px",s->b.projected_x,t->px);CHK("py",s->b.projected_y,t->py);
    CHK("cx",s->b.cached_x,t->cx);CHK("cy",s->b.cached_y,t->cy);CHK("parent",s->b.parent_tag,t->parent);
    CHK("kind",s->b.kind_angle,t->kind_angle);CHK("f66",s->b.field66,t->field66);CHK("1e",s->b.word1e,t->word1e);
    CHK("f15",s->b.flags15,t->flags15);CHK("f5f",s->b.flags5f,t->flags5f);CHK("5a",s->b.timer5a,t->timer5a);
    CHK("mark",s->b.mark,t->mark);CHK("corr",s->b.correction,t->correction);
    CHK("timer",s->anim.timer,t->timer);CHK("speed",s->anim.speed,t->speed);CHK("flash",s->anim.flash,t->flash);
    CHK("f18",s->anim.field18,t->field18);CHK("f50",s->anim.field50_tag,t->field50);
    CHK("f5b",s->f5b,t->f5b);CHK("f5c",s->f5c,t->f5c);CHK("f5d",s->f5d,t->f5d);CHK("f5e",s->f5e,t->f5e);CHK("health",s->health,t->health);CHK("68",s->variant68,t->variant68);CHK("69",s->group69,t->group69);
    if (t->anim) {
        const neo_anim_program_t *p;uint16_t cur;const int32_t *a;
        if (!locate(t->anim,&p,&cur,&a)) {CHK("anim?",0,t->anim)}
        else {
            const int32_t *mine=NULL;
            for (unsigned i=0;i<COUNT(soldier_programs);i++) if (&soldier_programs[i].program==s->anim.program) mine=soldier_programs[i].address;
            uint32_t at=mine && s->anim.cursor<s->anim.program->count?(uint32_t)mine[s->anim.cursor]:0;
            CHK("anim",at,t->anim);
        }
    }
    for (unsigned i=0;i<38;i++) if (s->locals[i]!=t->locals[i]) {snprintf(buf+strlen(buf),sizeof(buf)-strlen(buf)," +%02X=%02x/%02x",0x6e + i,s->locals[i],t->locals[i]);break;}
    #undef CHK
    return buf;
}
int main(void) {
    setvbuf(stdout,NULL,_IONBF,0);
    unsigned pass=0,fail=0,unsupported=0,bad_load=0,external=0,rng_total=0,ncovered=0;uint32_t covered[64];
    unsigned routine_fail[64]={0};uint32_t routine_id[64]={0};unsigned nroutines=0;
    for (unsigned i=0;i<COUNT(tick_cases);i++) {
        const tick_case_t *c=&tick_cases[i];
        if (c->external_attack) {external++;continue;}
        neo_soldier_t s;if (!load(&s,&c->before)) {bad_load++;continue;}
        neo_soldier_result_t result=NEO_SOLDIER_MORE;
        neo_rng_t rng;memcpy(rng.words,c->rng_words,sizeof(rng.words));rng.cursor=c->rng_cursor;neo_rng_t rng0=rng;
        neo_kscene_t kernel={{c->terrain[0],c->terrain[1],c->terrain[2],c->terrain[3]},{c->terrain[0],c->terrain[1],c->terrain[2],c->terrain[3]},
            soldier_group_map,0,0};
        uint16_t grid[128];memcpy(grid,c->grid,sizeof(grid));
        neo_hurtbox_t player={c->hx,c->hy,c->hflags15,soldier_player_hurtbox.type,0,
            {soldier_player_hurtbox.rect[0],soldier_player_hurtbox.rect[1],soldier_player_hurtbox.rect[2],soldier_player_hurtbox.rect[3]},c->hdead};
        neo_soldier_env_t env={0};
        env.kernel=&kernel;
        env.terrain=(neo_contact_parent_t){0x10c72eu,c->terrain[0],c->terrain[1],c->terrain[2],c->terrain[3],0};
        env.root=env.terrain;env.root.tag=0x10e968u;
        env.epoch=c->epoch;env.camera_word=c->camera;
        env.players=(neo_control_frame_t){0,0,0,0,c->p1x,c->p1y,c->p2x,c->p2y,c->mask,c->mode88,c->rank,c->mask!=0,NULL};
        env.grid=grid;env.rng=&rng;env.targets=c->hlive?&player:NULL;env.target_count=c->hlive?1:0;
        env.bounds=(neo_cull_bounds_t){false,0,0,0,0,c->bounds[0],c->bounds[1],c->bounds[2],c->bounds[3]};
        env.program=program;
        uint32_t routine=c->before.routine;
        /* Player bullets (01B882): air step, then 0049E8 against this soldier. */
        bool consumed[16]={0};
        for (unsigned pass=0;pass<2;pass++) {
            neo_soldier_result_t r=NEO_SOLDIER_MORE;
            if (pass==1) {r=neo_soldier_tick(&s,&env);result=r;}
            for (unsigned b=0;b<c->bullet_count;b++) {
                const tick_bullet_t *t=&c->bullets[b];
                if ((t->after_soldier!=0)!=(pass==1) || consumed[b] || r!=NEO_SOLDIER_MORE) continue;
                neo_contact_actor_t body={t->x,t->y,t->vx,t->vy,0,t->px,t->py,t->cx,t->cy,t->parent,t->kind_angle,0,0,t->flags15,t->flags5f,0,t->mark,t->correction};
                neo_contact_request_t q=neo_contact_prepare_air(&body);
                neo_kreq_t kq={q.x,q.y,q.dx,q.dy,(uint8_t)(q.parent_tag==0x10c72eu?1:q.parent_tag==0x10e968u?2:0),0,q.mode8,0,0,0,0};
                neo_kres_t k;if (!neo_kernel_sweep(&kernel,&kq,&k)) continue;
                neo_contact_result_t res={k.x,k.y,k.vx,k.vy,k.parent==1?0x10c72eu:k.parent==2?0x10e968u:0,k.kind,k.flags};
                neo_contact_commit(&body,&res,res.parent_tag==0x10c72eu?&env.terrain:&env.root,env.epoch);
                if (neo_soldier_receive(&s,&body,t->f5e,soldier_pistol_attacks[t->f6f&15u],grid)) consumed[b]=true;
            }
        }
        neo_soldier_result_t r=result;
        unsigned k;for (k=0;k<nroutines && routine_id[k]!=routine;k++);
        if (k==nroutines && nroutines<64) routine_id[nroutines++]=routine;
        const char *d="";
        bool ok;
        if (r==NEO_SOLDIER_UNSUPPORTED) {unsupported++;ok=false;d=" UNSUPPORTED";}
        else if (c->deleted) ok=r==NEO_SOLDIER_DELETED;
        else {d=diff(&s,&c->after);ok=r==NEO_SOLDIER_MORE && !*d;}
        /* The soldier must consume exactly the RNG calls the log attributes to it. */
        neo_rng_t expect=rng0;for (unsigned n=0;n<c->rng_calls;n++) neo_rng_next(&expect);
        if (ok && (expect.cursor!=rng.cursor || memcmp(expect.words,rng.words,sizeof(rng.words)))) {ok=false;d=" RNG";}
        if (ok) {pass++;rng_total+=c->rng_calls;
            unsigned j;for (j=0;j<ncovered && covered[j]!=routine;j++);
            if (j==ncovered && ncovered<64) covered[ncovered++]=routine;}
        else {fail++;if (k<64) routine_fail[k]++;
            if (fail<=40) printf("case %u %s f=%u node=%06x rt=%06x ->%s r=%d del=%d\n",i,c->capture,(unsigned)c->frame,(unsigned)c->node,(unsigned)routine,d,r,c->deleted);}
    }
    printf("ticks: %u pass, %u fail (%u unsupported), %u unloadable, %u with non-pistol attacks of %u\n",pass,fail,unsupported,bad_load,external,(unsigned)COUNT(tick_cases));
    printf("RNG calls reproduced: %u; routines entered: %u:",rng_total,ncovered);
    for (unsigned j=0;j<ncovered;j++) printf(" %05x",(unsigned)covered[j]);
    printf("\n");
    for (unsigned k=0;k<nroutines;k++) if (routine_fail[k]) printf("  routine %06x: %u failures\n",(unsigned)routine_id[k],routine_fail[k]);
    assert(!fail && !bad_load && pass+external==COUNT(tick_cases) && external<=2);
    printf("PASS: %u original soldier updates reproduced natively (state, animation cursor and RNG)\n",pass);
    return 0;
}
