/* Closed-loop check of the native region world (src/neo_world.c) against
 * four MAME captures (tools/extract_soldier_world.py). The world runs on its
 * own state for the whole capture: script, runners, soldiers, thrown objects
 * and death splashes are created, ordered and updated natively. Only the
 * inputs the original reads from outside the world are injected: scroll,
 * epoch, video counter, the player actor, its pistol bullets (ROM 01B882,
 * modelled as in test_neo_soldier_ai.c) and the ROM 0029C6 calls of actors
 * the port does not have (player, sparks, the 099DCC/099E06 entities), each
 * at its original list position. */
#define MSX_SOLDIER_PROGRAMS 1
#include <stdio.h>
#include <string.h>
#include "../src/neo_world.h"
#include "soldier_ai_tables.h"
#include "soldier_world_fixture.h"
#define COUNT(a) (sizeof(a)/sizeof((a)[0]))
typedef struct {
    neo_world_t *w;const world_tick_t *t;
    unsigned executed;        /* world actors run so far this update */
    unsigned log_at;          /* next log entry to look at */
    uint32_t map[6][NEO_WORLD_GACTORS]; /* native slot -> original node */
    unsigned previous_pos;int previous_soldier,previous_kind; /* for the bullet passes */
    /* Constructed this update: 003F24 leaves +24/+26 and +5B..+5D as the
     * previous occupant left them; the soldier only reads them after writing
     * them (animation start, hit). */
    bool fresh[NEO_WORLD_SOLDIERS];
    bool hit[NEO_WORLD_SOLDIERS];   /* +5B..+5D are written by the first hit */
    unsigned injected,bad_order,bullet_hits,external_attacks;
    char why[256];
} ctx_t;
/* Position of a world actor in the original list; -1 for outside actors. A
 * death splash (040F88/040FD4) never calls the RNG, so calls logged under a
 * node reused for one this update belong to its previous occupant (a bullet
 * spark) and are injected. */
static int position_of(const world_tick_t *t,uint32_t node) {
    for (unsigned i=0;i<t->order_count;i++)
        if (t->order[i].node==node) return t->order[i].kind==NEO_WORLD_SPLASH?-1:t->order[i].position;
    return -1;
}
static void bullet(ctx_t *c,const world_bullet_t *b,int kind,unsigned index) {
    neo_world_t *w=c->w;
    neo_contact_actor_t body={b->x,b->y,b->vx,b->vy,0,b->px,b->py,b->cx,b->cy,b->parent,b->kind_angle,0,0,b->flags15,b->flags5f,0,b->mark,b->correction};
    neo_contact_request_t q=neo_contact_prepare_air(&body);
    neo_kreq_t kq={q.x,q.y,q.dx,q.dy,(uint8_t)(q.parent_tag==0x10c72eu?1:q.parent_tag==0x10e968u?2:0),0,q.mode8,0,0,0,0};
    neo_kres_t k;if (!neo_kernel_sweep(&w->kernel,&kq,&k)) return;
    neo_contact_result_t res={k.x,k.y,k.vx,k.vy,k.parent==1?0x10c72eu:k.parent==2?0x10e968u:0,k.kind,k.flags};
    neo_contact_commit(&body,&res,res.parent_tag==0x10c72eu?&w->terrain:&w->root,w->epoch);
    if (kind==NEO_WORLD_GACTOR) {
        if (neo_world_hit_actor(w,index,&body,b->f5e,soldier_pistol_attacks[b->f6f&15u])) c->bullet_hits++;
        return;
    }
    neo_soldier_t *s=&w->soldiers[index];
    if (!s->active || !w->constructed[index]) return;
    if (neo_soldier_receive(s,&body,b->f5e,soldier_pistol_attacks[b->f6f&15u],w->grid)) {c->bullet_hits++;c->hit[index]=true;}
}
/* Bullets positioned after the actor that just ran hit it after its update. */
static void bullets_after(ctx_t *c) {
    if (c->previous_soldier<0) return;
    for (unsigned i=0;i<c->t->bullet_count;i++)
        if (c->t->bullets[i].position>c->previous_pos) bullet(c,&c->t->bullets[i],c->previous_kind,(unsigned)c->previous_soldier);
    c->previous_soldier=-1;
}
static void trace(void *context,uint8_t kind,uint8_t index) {
    ctx_t *c=context;const world_tick_t *t=c->t;
    bullets_after(c);
    /* A thrown object created this update is inserted after the script actor
     * (03BB8E -> 00411C), behind the traversal: it first runs next update. */
    while (c->executed<t->order_count && t->order[c->executed].created && t->order[c->executed].kind==NEO_WORLD_THROW) c->executed++;
    if (c->executed>=t->order_count) {c->bad_order++;return;}
    const world_actor_t *a=&t->order[c->executed++];
    if (a->kind!=kind) {c->bad_order++;snprintf(c->why,sizeof(c->why),"actor %u kind %u/%u node %06X",c->executed-1,kind,a->kind,(unsigned)a->node);}
    c->map[kind][index]=a->node;
    if (kind==NEO_WORLD_SOLDIER && !c->w->constructed[index]) {c->fresh[index]=true;c->hit[index]=false;}
    else if (kind==NEO_WORLD_SOLDIER) c->fresh[index]=false;
    /* Inject the calls of outside actors that precede this one. */
    while (c->log_at<t->log_count) {
        uint32_t node=t->log[c->log_at].node;int pos=position_of(t,node);
        if (pos>=0 && pos>=(int)a->position) break;
        if (pos<0) {neo_rng_next(&c->w->rng);c->injected++;}
        c->log_at++;
    }
    if (kind==NEO_WORLD_SOLDIER || kind==NEO_WORLD_GACTOR) {
        if (kind==NEO_WORLD_GACTOR || c->w->constructed[index])
            for (unsigned i=0;i<t->bullet_count;i++) if (t->bullets[i].position<a->position) bullet(c,&t->bullets[i],kind,index);
        c->previous_soldier=(int)index;c->previous_kind=kind;c->previous_pos=a->position;
    }
}
static const char *compare(const neo_soldier_t *s,const world_snapshot_t *t,bool fresh,bool hit) {
    static char buf[256];buf[0]=0;
    #define CHK(name,a,b) if ((uint32_t)(a)!=(uint32_t)(b)) {snprintf(buf+strlen(buf),sizeof(buf)-strlen(buf)," %s=%x/%x",name,(unsigned)(a),(unsigned)(b));}
    CHK("routine",s->routine,t->routine);CHK("x",s->b.x,t->x);CHK("y",s->b.y,t->y);CHK("vx",s->b.vx,t->vx);CHK("vy",s->b.vy,t->vy);
    CHK("s62",s->b.scalar62,t->scalar62);CHK("px",s->b.projected_x,t->px);CHK("py",s->b.projected_y,t->py);
    CHK("cx",s->b.cached_x,t->cx);CHK("cy",s->b.cached_y,t->cy);CHK("parent",s->b.parent_tag,t->parent);
    CHK("kind",s->b.kind_angle,t->kind_angle);CHK("f66",s->b.field66,t->field66);CHK("1e",s->b.word1e,t->word1e);
    CHK("f15",s->b.flags15,t->flags15);CHK("f5f",s->b.flags5f,t->flags5f);CHK("5a",s->b.timer5a,t->timer5a);
    if (!fresh) {CHK("timer",s->anim.timer,t->timer);CHK("speed",s->anim.speed,t->speed);}
    CHK("flash",s->anim.flash,t->flash);
    if (hit) {CHK("f5b",s->f5b,t->f5b);CHK("f5c",s->f5c,t->f5c);CHK("f5d",s->f5d,t->f5d);}
    CHK("f5e",s->f5e,t->f5e);CHK("health",s->health,t->health);
    CHK("68",s->variant68,t->variant68);CHK("69",s->group69,t->group69);
    for (unsigned i=0;i<38;i++) if (s->locals[i]!=t->locals[i]) {snprintf(buf+strlen(buf),sizeof(buf)-strlen(buf)," +%02X=%02x/%02x",0x6e + i,s->locals[i],t->locals[i]);break;}
    #undef CHK
    return buf;
}
/* Generic actor record against the original bytes. Not compared: list links
 * and creator (+04..+0F), palette handles (+14, +16, +19..+1C: the native
 * allocator numbers its own slots), the animation cursor (+20). Block
 * pointers (+38) are compared through the node map. */
static uint32_t node_of_tag(const ctx_t *c,uint32_t tag) {
    if ((tag&0xff000000u)==NEO_GA_TAG_BASE) return c->map[NEO_WORLD_GACTOR][tag&0xffu]+0x28u;
    return tag;
}
/* Fields ROM 004126 leaves as the previous occupant had them: while the port
 * has not written them since the node was taken they hold the original's
 * (injected) values. Registry slot numbers (+58/+59) count the player and
 * the outside actors and are not compared. */
static bool stale_offset(unsigned o) {
    return o==0x14 || o==0x16 || (o>=0x18 && o<0x1d) || (o>=0x20 && o<0x28) || (o>=0x5b && o<0x5e) ||
        (o>=0x64 && o<0x68) || o==0x6c || o==0x6d || o>=0xb0;
}
static void inject_stale(neo_gactor_t *a,const uint8_t *m) {
    for (unsigned o=0;o<NEO_GA_BYTES;o++) if (stale_offset(o) && !((a->written[o>>3]>>(o&7u))&1u)) a->m[o]=m[o];
}
static const char *compare_gactor(const ctx_t *c,const neo_gactor_t *a,const uint8_t *m) {
    static char buf[256];buf[0]=0;
    for (unsigned o=0;o<NEO_GA_BYTES;o++) {
        if ((o>=4 && o<0x10) || o==0x14 || o==0x16 || (o>=0x19 && o<0x1d) || (o>=0x20 && o<0x24) || o==0x58 || o==0x59) continue;
        if (o>=0x38 && o<0x3c) {
            uint32_t want=(uint32_t)m[0x38]<<24|(uint32_t)m[0x39]<<16|(uint32_t)m[0x3a]<<8|m[0x3b];
            uint32_t got=node_of_tag(c,ga_l(a,0x38));
            if (want!=got) {snprintf(buf,sizeof(buf)," +38=%06x/%06x",(unsigned)got,(unsigned)want);return buf;}
            o=0x3b;continue;
        }
        if (a->m[o]!=m[o]) {
            snprintf(buf+strlen(buf),sizeof(buf)-strlen(buf)," +%02X=%02x/%02x",o,a->m[o],m[o]);
            if (strlen(buf)>200) break;
        }
    }
    return buf;
}
static void dump(const neo_world_t *w) {
    for (unsigned i=0;i<NEO_WORLD_GACTORS;i++) if (w->gactors[i].active) {
        const neo_gactor_t *a=&w->gactors[i];
        printf("  gactor %u: %05X x=%08x y=%08x px=%08x py=%08x parent=%08x 6a=%02x\n",i,(unsigned)ga_l(a,0),(unsigned)ga_l(a,0x28),(unsigned)ga_l(a,0x2c),
            (unsigned)ga_l(a,0x3c),(unsigned)ga_l(a,0x40),(unsigned)ga_l(a,0x38),ga_b(a,0x6a));
    }
    for (unsigned i=0;i<NEO_KDYN_COUNT;i++) if (w->kernel.dyn[i].map) {
        const neo_kdyn_t *k=&w->kernel.dyn[i];
        printf("  dyn %u: pos %08x %08x bbox %08x %08x %08x %08x bins",i,(unsigned)k->pos.x,(unsigned)k->pos.y,(unsigned)k->bbox[0],(unsigned)k->bbox[1],(unsigned)k->bbox[2],(unsigned)k->bbox[3]);
        for (unsigned b=0;b<18;b++) printf(" %02x",w->kernel.bins_x[b]);
        printf("\n");
    }
}
int main(void) {
    unsigned injected_targets=0,ticks=0,states=0,gstates=0,injected=0,native_calls=0,throws=0,hits=0,draws=0,external_attacks=0,external_splats=0;
    for (unsigned ci=0;ci<COUNT(world_captures);ci++) {
        const world_capture_t *cap=&world_captures[ci];
        static neo_world_t w;neo_world_init(&w,cap->scroll_x);
        memcpy(w.rng.words,cap->rng_words,sizeof(w.rng.words));w.rng.cursor=cap->rng_cursor;
        w.script.pc=(uint16_t)(cap->pc-MISSION1_SCRIPT_BASE);w.epoch=cap->epoch;
        memcpy(w.grid,cap->grid,sizeof(w.grid));w.grid_cursor=cap->grid_cursor;
        ctx_t c;memset(&c,0,sizeof(c));c.w=&w;w.trace=trace;w.trace_context=&c;
        for (unsigned ti=0;ti<cap->tick_count;ti++) {
            const world_tick_t *t=&cap->ticks[ti];
            c.t=t;c.executed=0;c.log_at=0;c.previous_soldier=-1;c.why[0]=0;
            w.video_phase=t->video;
            neo_world_player_t p={t->player_x,t->player_y,t->player_flags15,t->player_alive,t->player_dead,t->follow,t->follow,!(t->active&1u)};
            unsigned before_calls=c.injected;
            neo_world_tick(&w,t->scroll_x,&p);
            bullets_after(&c);
            while (c.executed<t->order_count && t->order[c.executed].created && t->order[c.executed].kind==NEO_WORLD_THROW) c.executed++;
            /* Map the objects thrown this update to their nodes, in list order;
             * a slot freed (and possibly reused) this update loses its node. */
            for (unsigned i=0;i<NEO_WORLD_THROWS;i++) {
                bool listed=false;
                for (unsigned k=0;k<t->order_count;k++) listed|=t->order[k].kind==NEO_WORLD_THROW && !t->order[k].created && t->order[k].node==c.map[NEO_WORLD_THROW][i];
                if (!w.throws[i].active || !listed) c.map[NEO_WORLD_THROW][i]=0;
            }
            for (unsigned i=0,k=0;i<w.order_count;i++) {
                if (w.order[i].kind!=NEO_WORLD_THROW || c.map[NEO_WORLD_THROW][w.order[i].index]) continue;
                for (;k<t->order_count && !(t->order[k].created && t->order[k].kind==NEO_WORLD_THROW);k++) {}
                if (k<t->order_count) c.map[NEO_WORLD_THROW][w.order[i].index]=t->order[k++].node;
            }
            /* Remaining calls by outside actors after the last world actor. */
            while (c.log_at<t->log_count) {
                if (position_of(t,t->log[c.log_at].node)<0) {neo_rng_next(&w.rng);c.injected++;}
                c.log_at++;
            }
            bool bad=c.bad_order||c.executed!=t->order_count||w.unsupported||w.overflow||w.rng.cursor!=t->rng_cursor||
                memcmp(w.rng.words,t->rng_words,sizeof(w.rng.words))||neo_script_address(&w.script,w.script.pc)!=t->pc||memcmp(w.script.groups,t->groups,9);
            if (bad) {
                printf("FAIL %s frame %u: order %u/%u (%s) unsupported %u (%05X) overflow %u rng %u/%u pc %06X/%06X epoch %u/%u\n",cap->name,(unsigned)t->frame,
                    c.executed,t->order_count,c.why,w.unsupported,(unsigned)w.unsupported_routine,w.overflow,w.rng.cursor,t->rng_cursor,
                    (unsigned)neo_script_address(&w.script,w.script.pc),(unsigned)t->pc,w.epoch,t->epoch);
                return 1;
            }
            /* Every original soldier state must have its native counterpart. */
            for (unsigned k=0;k<t->soldier_count;k++) {
                const world_soldier_t *o=&t->soldiers[k];int found=-1;
                for (unsigned i=0;i<NEO_WORLD_SOLDIERS;i++) if (w.soldiers[i].active && w.constructed[i] && c.map[NEO_WORLD_SOLDIER][i]==o->node) found=(int)i;
                if (found<0) {printf("FAIL %s frame %u: soldier %06X missing natively\n",cap->name,(unsigned)t->frame,(unsigned)o->node);return 1;}
                /* An attack the port does not have (the player's knife, kind 2)
                 * is injected as the original recorded it: damage fields and
                 * the class-1 kill splat at the attacker's position. */
                neo_soldier_t *ns=&w.soldiers[found];
                if (o->s.timer5a && o->s.timer5a!=6 && ns->b.timer5a!=o->s.timer5a) {
                    ns->b.timer5a=(uint8_t)o->s.timer5a;ns->f5b=(uint8_t)o->s.f5b;ns->f5c=(uint8_t)o->s.f5c;ns->f5d=(uint8_t)o->s.f5d;
                    ns->f5e=(uint8_t)o->s.f5e;ns->anim.flash|=6u;ns->health=(uint16_t)o->s.health;
                    if ((o->s.flags5f&0x80u) && !(ns->b.flags5f&0x80u)) {ns->b.flags5f|=0x80u;neo_grid_splat(w.grid,t->player_x,t->player_y);}
                    c.hit[found]=true;c.external_attacks++;
                }
                /* Continue update: the re-created player's position as the
                 * soldiers read it is not recorded; their control target
                 * (+84..+8B) is taken from the original. */
                if (t->respawn) {memcpy(&ns->locals[0x16],&o->s.locals[0x16],8);injected_targets++;}
                const char *d=compare(ns,&o->s,c.fresh[found],c.hit[found]);
                if (*d) {printf("FAIL %s frame %u: soldier %06X%s\n",cap->name,(unsigned)t->frame,(unsigned)o->node,d);dump(&w);return 1;}
                states++;
            }
            /* Kills of actors outside the world (the 0400AA/099DCC entities
             * shot by the player) splat the grid too (ROM 00505A): a
             * difference that is exactly one or two splats is injected. */
            for (unsigned n=0;n<2 && memcmp(w.grid,t->grid,sizeof(w.grid));n++) {
                bool found=false;
                for (unsigned cell=0;cell<128 && !found;cell++) {
                    uint16_t g[128];memcpy(g,w.grid,sizeof(g));
                    neo_grid_splat(g,(uint16_t)(0x20u+(cell>>3)*32u),(uint16_t)(0x100u+(cell&7u)*32u));
                    if (!memcmp(g,t->grid,sizeof(g)) || n==0) {
                        /* Accept the first splat that reduces the difference to
                         * zero now, or (first pass) to another single splat. */
                        bool exact=!memcmp(g,t->grid,sizeof(g));
                        if (!exact) {
                            bool second=false;
                            for (unsigned c2=0;c2<128 && !second;c2++) {
                                uint16_t h[128];memcpy(h,g,sizeof(h));
                                neo_grid_splat(h,(uint16_t)(0x20u+(c2>>3)*32u),(uint16_t)(0x100u+(c2&7u)*32u));
                                second=!memcmp(h,t->grid,sizeof(h));
                            }
                            if (!second) continue;
                        }
                        memcpy(w.grid,g,sizeof(g));found=true;external_splats++;
                    }
                }
                if (!found) break;
            }
            if (!bad && memcmp(w.grid,t->grid,sizeof(w.grid))) {
                printf("FAIL %s frame %u: delay grid",cap->name,(unsigned)t->frame);
                for (unsigned k=0;k<128;k++) if (w.grid[k]!=t->grid[k]) printf(" [%u]=%x/%x",k,w.grid[k],t->grid[k]);
                printf("\n");return 1;
            }
            for (unsigned k=0;k<t->throw_count;k++) {
                const world_throw_t *o=&t->throws[k];int found=-1;
                for (unsigned i=0;i<NEO_WORLD_THROWS;i++) if (w.throws[i].active && c.map[NEO_WORLD_THROW][i]==o->node) found=(int)i;
                if (found<0 || w.throws[found].body.x!=o->x || w.throws[found].body.y!=o->y) {
                    printf("FAIL %s frame %u: thrown object %06X %s\n",cap->name,(unsigned)t->frame,(unsigned)o->node,found<0?"missing":"position");return 1;
                }
                throws++;
            }
            for (unsigned k=0;k<t->gactor_count;k++) {
                const world_gactor_t *o=&t->gactors[k];int found=-1;
                for (unsigned i=0;i<NEO_WORLD_GACTORS;i++) if (w.gactors[i].active && c.map[NEO_WORLD_GACTOR][i]==o->node) found=(int)i;
                if (found<0) {printf("FAIL %s frame %u: actor %06X (%05X) missing natively\n",cap->name,(unsigned)t->frame,(unsigned)o->node,
                    (unsigned)((uint32_t)o->m[1]<<16|(uint32_t)o->m[2]<<8|o->m[3]));return 1;}
                inject_stale(&w.gactors[found],o->m);
                const char *d=compare_gactor(&c,&w.gactors[found],o->m);
                if (*d) {printf("FAIL %s frame %u: actor %06X (%05X)%s\n",cap->name,(unsigned)t->frame,(unsigned)o->node,(unsigned)ga_l(&w.gactors[found],0),d);return 1;}
                gstates++;
            }
            unsigned alive=neo_world_alive(&w);(void)alive;
            native_calls+=t->log_count-(c.injected-before_calls);
            ticks++;hits+=c.bullet_hits;c.bullet_hits=0;draws+=w.draw_count;
        }
        injected+=c.injected;external_attacks+=c.external_attacks;
    }
    printf("PASS: %u closed-loop world updates: %u soldier states, %u generic actor states, %u thrown-object positions, %u native RNG calls (%u injected from outside actors), %u bullet hits, %u injected knife hits, %u injected outside-kill splats, %u injected continue targets, %u draws\n",
        ticks,states,gstates,throws,native_calls,injected,hits,external_attacks,external_splats,injected_targets,draws);
    return 0;
}
