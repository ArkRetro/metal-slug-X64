/* Host regression of the native Mission 1 boss (src/neo_boss.c) against
 * fights recorded in MAME (data/boss_fight_*.json.gz, compiled by
 * tools/compile_boss_fixture.py into build/boss_fixture.h).
 *
 * The boss nodes run CLOSED LOOP: the native state is never reloaded from
 * the recording. Before each original update the native field image must
 * equal the recorded one (after the bullets' hits of the interval are
 * applied through neo_boss_receive); after it, the update's result, next
 * routine and field image must equal the recording. The engine services
 * (RNG, allocator, palettes, collision kernel, object platforms, hit
 * broadphase, attack queries, scene bounds and objects) answer with the
 * results the original got, and every request is checked against the
 * recorded sequence; draws (sprite list entries) are compared too.
 *
 * Fixture stream (little endian): u16 nodes, u32 updates; per node u32 ctor,
 * i16 creator, u32 address; per update u32 frame, u16 node, u32 routine,
 * u8 has_pre, i32 handoff, [pre 0xB0], post 0xB0, u16 hits {u16 node,
 * u16 kind, u16 damage, u32 class, u8 has_attacker, attacker 0xB0, u16 writes
 * {u8 offset, u8 value, u32 pc}}, u8 env {u32 address, u8 value},
 * u16 events {u8 type, payload} (see tools/compile_boss_fixture.py). */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/neo_boss.h"
#ifdef N64
#include <libdragon.h>
#include "boss_fight_names.h"
/* The cartridge streams the same fixture stream from DFS (rom:/boss-<name>.bin). */
static const char *const fight_names[]={BOSS_FIGHT_NAMES};
#else
#include "boss_fixture.h"
#endif
enum { E_RNG=1,E_ALLOC,E_PAL,E_SCENE,E_SWEEP,E_FLOOR,E_PLAT,E_PLATFREE,E_HURT,E_ATTACK,E_BOUNDS,E_DRAW,E_MISSION };
static const char *ev_name[]={"?","rng","alloc","pal","scene","sweep","floor","plat","platfree","hurt","attack","bounds","draw","mission"};
/* Fixture reader: a memory stream on the host, a buffered DFS file on N64.
 * Blocks are copied into a per-update arena (reset before each update). */
typedef struct { const uint8_t *p,*end; FILE *f; uint8_t buf[4096]; unsigned at,len; } rd_t;
static uint8_t arena[1u<<20];static unsigned arena_used;
static uint8_t u8(rd_t *r) {
    if (!r->f) {assert(r->p<r->end);return *r->p++;}
    if (r->at==r->len) {r->len=(unsigned)fread(r->buf,1,sizeof(r->buf),r->f);r->at=0;assert(r->len);}
    return r->buf[r->at++];
}
static uint16_t u16(rd_t *r) {uint16_t v=u8(r);return (uint16_t)(v|u8(r)<<8);}
static uint32_t u32(rd_t *r) {uint32_t v=u16(r);return v|(uint32_t)u16(r)<<16;}
static const uint8_t *blk(rd_t *r,unsigned n) {
    assert(arena_used+n<=sizeof(arena));
    uint8_t *q=arena+arena_used;arena_used+=n;
    for (unsigned i=0;i<n;i++) q[i]=u8(r);
    return q;
}
typedef struct {
    uint8_t type;
    uint32_t a[8];
    const uint8_t *img,*expect;
} event_t;
enum { MAXEV=512 };
static event_t evs[MAXEV];static unsigned nev,cur;
static char error[256];
static unsigned draws_ok,draws_bad,effects_ok,effects_bad;
static unsigned errors_by_type[16];
static void bad(const char *fmt,uint32_t a,uint32_t b) {
    if (!error[0]) snprintf(error,sizeof(error),fmt,(unsigned)a,(unsigned)b);
}
static event_t *pop(uint8_t type) {
    if (cur>=nev) {if (!error[0]) snprintf(error,sizeof(error),"unexpected %s request",ev_name[type]);errors_by_type[type]++;return NULL;}
    event_t *e=&evs[cur];
    if (e->type!=type) {
        char b[96];snprintf(b,sizeof(b),"event %s, expected %s",ev_name[type],ev_name[e->type]);
        if (!error[0]) snprintf(error,sizeof(error),"%s",b);
        errors_by_type[type]++;return NULL;
    }
    cur++;return e;
}
/* ---- node identity ---- */
static unsigned nnodes;static uint32_t node_addr[4096],node_ctor[4096];
static int16_t native_of[4096];static int fixture_of[NEO_BOSS_NODES];
static uint16_t history[NEO_BOSS_NODES][256];static unsigned history_count[NEO_BOSS_NODES];
static neo_boss_t boss;
static uint16_t s_rng(void *c) {(void)c;event_t *e=pop(E_RNG);return e?(uint16_t)e->a[0]:0;}
static int pending_child[16];static unsigned npending;
static const uint8_t *pending_expect[16];
static bool s_alloc(void *c,uint16_t reserve,uint8_t image[NEO_BOSS_FIELDS]) {
    (void)c;event_t *e=pop(E_ALLOC);if (!e) return false;
    bool ok=reserve<e->a[0];
    int16_t child=(int16_t)e->a[1];
    if (ok!=(child!=-2)) {bad("alloc outcome reserve %u free %u",reserve,e->a[0]);return false;}
    if (!ok) return false;
    if (e->img) memcpy(image,e->img,NEO_BOSS_FIELDS); else memset(image,0,NEO_BOSS_FIELDS);
    if (npending<16) {pending_child[npending]=child;pending_expect[npending]=e->expect;npending++;}
    return true;
}
static bool s_pal(void *c,uint16_t res,uint8_t *slot) {(void)c;(void)res;event_t *e=pop(E_PAL);if (!e) return false;*slot=(uint8_t)e->a[0];return true;}
static bool s_scene(void *c,uint32_t tag,neo_contact_parent_t *o) {
    (void)c;event_t *e=pop(E_SCENE);if (!e) return false;
    if (e->a[0]!=tag) {bad("scene tag %x expected %x",tag,e->a[0]);return false;}
    o->tag=tag;o->x=(int32_t)e->a[1];o->y=(int32_t)e->a[2];o->previous_x=(int32_t)e->a[3];o->previous_y=(int32_t)e->a[4];o->correction=(uint8_t)e->a[5];
    return true;
}
static bool s_sweep(void *c,const neo_contact_request_t *q,neo_boss_sweep_t *o) {
    (void)c;(void)q;event_t *e=pop(E_SWEEP);if (!e) return false;
    *o=(neo_boss_sweep_t){(int32_t)e->a[0],(int32_t)e->a[1],(int32_t)e->a[2],(int32_t)e->a[3],e->a[4],(uint16_t)e->a[5],(uint16_t)e->a[6]};
    return true;
}
static bool s_floor(void *c,int32_t x,int32_t y,uint32_t parent,neo_boss_sweep_t *o) {
    (void)c;(void)x;(void)y;(void)parent;event_t *e=pop(E_FLOOR);if (!e) return false;
    *o=(neo_boss_sweep_t){(int32_t)e->a[0],(int32_t)e->a[1],0,0,e->a[2],0,(uint16_t)e->a[3]};
    return true;
}
static bool s_plat(void *c,int16_t node,uint8_t slot,uint32_t table,const int32_t pos[4],uint32_t parent,uint8_t *out) {
    (void)c;(void)node;(void)table;(void)parent;event_t *e=pop(E_PLAT);if (!e) return false;
    *out=e->a[0]!=0xff?(uint8_t)e->a[0]:slot;
    if (e->a[1]>=2 && ((uint32_t)pos[0]!=e->a[2] || (uint32_t)pos[1]!=e->a[3])) bad("platform position %x expected %x",(uint32_t)pos[0],e->a[2]);
    return true;
}
static void s_platfree(void *c,int16_t node,uint8_t slot) {(void)c;if (getenv("BOSS_LIST")) printf("platfree native %d fixture %d slot %02x routine %06x\n",node,fixture_of[node],slot,(unsigned)boss.nodes[node].routine);pop(E_PLATFREE);}
static bool s_hurt(void *c,int16_t node,uint8_t *off,uint8_t *idx,bool *kill) {
    (void)c;(void)node;event_t *e=pop(E_HURT);if (!e) return false;
    *off=(uint8_t)e->a[0];*idx=(uint8_t)e->a[1];*kill=e->a[2]!=0;return true;
}
static bool s_attack(void *c,int16_t node,uint32_t rows,uint16_t *r) {(void)c;(void)node;(void)rows;event_t *e=pop(E_ATTACK);if (!e) return false;*r=(uint16_t)e->a[0];return true;}
static bool s_bounds(void *c,neo_cull_bounds_t *b) {
    (void)c;event_t *e=pop(E_BOUNDS);if (!e) return false;
    memset(b,0,sizeof(*b));b->has_parent=e->a[1]!=0;b->x=(int32_t)e->a[2];b->y=(int32_t)e->a[3];
    b->left=(uint16_t)e->a[4];b->top=(uint16_t)e->a[5];b->right=(uint16_t)e->a[6];b->bottom=(uint16_t)e->a[7];
    return true;
}
static void s_draw(void *c,int16_t node,const neo_anim_draw_t *d) {
    (void)c;(void)node;event_t *e=pop(E_DRAW);
    if (!e) {draws_bad++;return;}
    uint16_t attr=(uint16_t)((d->attributes&0xff00u)|(d->attributes&7u));
    if (e->a[0]!=d->descriptor || e->a[1]!=d->x || e->a[2]!=d->y || e->a[3]!=attr || e->a[4]!=d->z) {
        draws_bad++;
        if (!error[0]) snprintf(error,sizeof(error),"draw %06x %u,%u attr %04x z %04x expected %06x %u,%u attr %04x z %04x",(unsigned)d->descriptor,d->x,d->y,attr,d->z,
            (unsigned)e->a[0],(unsigned)e->a[1],(unsigned)e->a[2],(unsigned)e->a[3],(unsigned)e->a[4]);
    } else draws_ok++;
}
static uint32_t last_effect_label;static int16_t current_node=-1;static int last_effect_kind=-1;
static bool s_effect(void *c,const neo_boss_effect_t *f) {
    (void)c;
    if (f->kind==NEO_BOSS_EFFECT_SPAWN) {
        /* the last allocation is this child: compare its seed */
        const uint8_t *x=npending?pending_expect[npending-1]:NULL;
        if (x) {
            bool ok=true;
            for (unsigned o=0x10;o<NEO_BOSS_FIELDS;o++) if (f->image[o]!=x[o]) {ok=false;if (!error[0]) snprintf(error,sizeof(error),"effect %06x seed +%02X=%02x expected %02x",(unsigned)f->label,o,f->image[o],x[o]);break;}
            if (ok) effects_ok++; else effects_bad++;
        }
    }
    if (f->kind==NEO_BOSS_EFFECT_CONVERT && f->node==current_node) {last_effect_label=f->label;last_effect_kind=f->kind;}
    return true;
}
static void s_mission(void *c,int16_t node,uint8_t step) {(void)c;(void)node;event_t *e=pop(E_MISSION);if (e && e->a[0]!=step) bad("mission step %u expected %u",step,e->a[0]);}
/* ---- image comparison ---- */
static uint32_t anim_address(const neo_boss_node_t *n) {return neo_boss_anim_address(n);}
static uint32_t rd_l(const uint8_t *p,unsigned o) {return (uint32_t)p[o]<<24|(uint32_t)p[o+1]<<16|(uint32_t)p[o+2]<<8|p[o+3];}
/* Compare the native node with the recorded image; returns the first offset that differs or -1. */
static int compare(const neo_boss_node_t *n,const uint8_t *img,char *why,size_t len) {
    uint8_t mine[NEO_BOSS_FIELDS];memcpy(mine,n->f,NEO_BOSS_FIELDS);
    /* pointer-valued fields: node tags -> original addresses */
    static const unsigned ptr_off[]={0x38,0x8e};
    for (unsigned k=0;k<2;k++) {
        unsigned o=ptr_off[k];uint32_t v=rd_l(mine,o);
        if (NEO_BOSS_IS_TAG(v)) {
            unsigned ix=NEO_BOSS_TAG_INDEX(v);int f=fixture_of[ix];
            uint32_t a=f>=0?node_addr[f]+(o==0x38?0x28u:0u):0;
            /* A tag kept after its node was freed (the original keeps the
             * stale address, e.g. +8E of the turret after its gun left):
             * any earlier occupant of that native slot may be the one. */
            for (unsigned h=0;h<history_count[ix] && a!=rd_l(img,o);h++)
                if (node_addr[history[ix][h]]+(o==0x38?0x28u:0u)==rd_l(img,o)) a=rd_l(img,o);
            mine[o]=(uint8_t)(a>>24);mine[o+1]=(uint8_t)(a>>16);mine[o+2]=(uint8_t)(a>>8);mine[o+3]=(uint8_t)a;
        }
    }
    uint32_t aa=anim_address(n);
    if (aa!=rd_l(img,0x20)) {snprintf(why,len,"+20=%06x/%06x",(unsigned)aa,(unsigned)rd_l(img,0x20));return 0x20;}
    for (unsigned o=0x10;o<NEO_BOSS_FIELDS;o++) {
        if (o>=0x20 && o<0x24) continue;
        if (mine[o]!=img[o]) {snprintf(why,len,"+%02X=%02x/%02x",o,mine[o],img[o]);return (int)o;}
    }
    return -1;
}
static void load_env(neo_boss_env_t *e,uint32_t a,uint8_t v) {
    switch (a) {
    case 0x10c606:e->facing606=v;break;
    case 0x10c33c:e->alive_mask=v;break;
    case 0x10c33a:e->epoch=v;break;
    case 0x10c300:e->mission_over=v!=0;break;
    case 0x10fd8b:e->rank=v;break;
    case 0x100088:e->players88=v;break;
    case 0x10000e:e->frame=(uint16_t)((e->frame&0xff)|v<<8);break;
    case 0x10000f:e->frame=(uint16_t)((e->frame&0xff00)|v);break;
    case 0x101500:e->p1x=(int16_t)((e->p1x&0xff)|v<<8);break;
    case 0x101501:e->p1x=(int16_t)((e->p1x&0xff00)|v);break;
    case 0x101504:e->p1y=(int16_t)((e->p1y&0xff)|v<<8);break;
    case 0x101505:e->p1y=(int16_t)((e->p1y&0xff00)|v);break;
    case 0x1015b0:e->p2x=(int16_t)((e->p2x&0xff)|v<<8);break;
    case 0x1015b1:e->p2x=(int16_t)((e->p2x&0xff00)|v);break;
    case 0x1015b4:e->p2y=(int16_t)((e->p2y&0xff)|v<<8);break;
    case 0x1015b5:e->p2y=(int16_t)((e->p2y&0xff00)|v);break;
    default:break;
    }
    /* player node routine longs (0040AC): live unless 0x3F1E */
    static uint8_t r1[4]={0,0,0x3f,0x1e},r2[4]={0,0,0x3f,0x1e};
    if (a>=0x1014c4 && a<0x1014c8) {r1[a-0x1014c4]=v;e->p1_node=!(r1[0]==0&&r1[1]==0&&r1[2]==0x3f&&r1[3]==0x1e);}
    if (a>=0x101574 && a<0x101578) {r2[a-0x101574]=v;e->p2_node=!(r2[0]==0&&r2[1]==0&&r2[2]==0x3f&&r2[3]==0x1e);}
}
static int32_t rl_s(const uint8_t *p,unsigned o) {return (int32_t)rd_l(p,o);}
int main(void) {
    setvbuf(stdout,NULL,_IONBF,0);
#ifdef N64
    debug_init_emulog();debug_init_isviewer();dfs_init(DFS_DEFAULT_LOCATION);
    unsigned fights=sizeof(fight_names)/sizeof(fight_names[0]);
#else
    unsigned fights=sizeof(boss_fights)/sizeof(boss_fights[0]);
#endif
    unsigned total_fail=0,total_pass=0,total_draws=0;
    for (unsigned fi=0;fi<fights;fi++) {
        static rd_t r;memset(&r,0,sizeof(r));
#ifdef N64
        char path[64];snprintf(path,sizeof(path),"rom:/boss-%s.bin",fight_names[fi]);
        r.f=fopen(path,"rb");assert(r.f);
        struct { const char *name; } FF={fight_names[fi]},*F=&FF;
#else
        const boss_fight_t *F=&boss_fights[fi];
        r.p=F->bytes;r.end=F->bytes+F->size;
#endif
        nnodes=u16(&r);uint32_t nup=u32(&r);
        assert(nnodes<=4096);
        for (unsigned i=0;i<nnodes;i++) {node_ctor[i]=u32(&r);(void)u16(&r);node_addr[i]=u32(&r);native_of[i]=-1;}
        arena_used=0;
        for (unsigned i=0;i<NEO_BOSS_NODES;i++) {fixture_of[i]=-1;history_count[i]=0;}
        neo_boss_services_t sv={NULL,s_rng,s_alloc,s_pal,NULL,NULL,s_scene,s_sweep,s_floor,s_plat,s_platfree,s_hurt,s_attack,s_bounds,s_draw,s_effect,s_mission};
        neo_boss_init(&boss,&sv);draws_ok=draws_bad=effects_ok=effects_bad=0;
        boss.env.p1_node=true;
        unsigned pass=0,fail=0,pre_fail=0,unsupported=0,hits=0,hits_unknown=0,handoffs=0,deletes=0;
        uint32_t covered[128];unsigned ncovered=0;
        static unsigned fail_rt[0x2000];memset(fail_rt,0,sizeof(fail_rt));
        bool stop=false;
        for (uint32_t k=0;k<nup && !stop;k++) {
            arena_used=0;
            uint32_t frame=u32(&r);uint16_t id=u16(&r);uint32_t routine=u32(&r);uint8_t has_pre=u8(&r);int32_t handoff=(int32_t)u32(&r);
            const uint8_t *pre=has_pre?blk(&r,NEO_BOSS_FIELDS):NULL,*post=blk(&r,NEO_BOSS_FIELDS);
            /* hits applied by other actors since the previous boss update */
            unsigned nh=u16(&r);
            for (unsigned h=0;h<nh;h++) {
                uint16_t hid=u16(&r),kind=u16(&r),damage=u16(&r);uint32_t klass=u32(&r);uint8_t has_att=u8(&r);
                const uint8_t *att=blk(&r,NEO_BOSS_FIELDS);
                unsigned nw=u16(&r);int16_t ix=0,iy=0;
                for (unsigned w=0;w<nw;w++) {uint8_t o=u8(&r),v=u8(&r);uint32_t pc=u32(&r);
                    if (pc==0x490c) ix=(int16_t)((int8_t)v*2);
                    if (pc==0x4916) {iy=(int16_t)((int8_t)v*2);}
                    (void)o;}
                hits++;
                if (kind==0xffff || !has_att || native_of[hid]<0) {hits_unknown++;continue;}
                neo_boss_hit_t hit={rl_s(att,0x3c),rl_s(att,0x40),rl_s(att,0x44),rl_s(att,0x48),(uint8_t)kind,(uint8_t)klass,att[0x5e],damage,ix,iy,false,0};
                neo_boss_receive(&boss,native_of[hid],&hit);
            }
            unsigned ne=u8(&r);
            for (unsigned i=0;i<ne;i++) {uint32_t a=u32(&r);uint8_t v=u8(&r);load_env(&boss.env,a,v);}
            nev=u16(&r);cur=0;assert(nev<=MAXEV);
            for (unsigned i=0;i<nev;i++) {
                event_t *e=&evs[i];memset(e,0,sizeof(*e));e->type=u8(&r);
                switch (e->type) {
                case E_RNG:e->a[0]=u16(&r);break;
                case E_ALLOC: {e->a[0]=u16(&r);e->a[1]=(uint32_t)(int32_t)(int16_t)u16(&r);uint16_t f=u16(&r);
                    if (f&1) e->img=blk(&r,NEO_BOSS_FIELDS);
                    if (f&2) {e->expect=blk(&r,NEO_BOSS_FIELDS);}
                    break;}
                case E_PAL:e->a[0]=u8(&r);break;
                case E_SCENE:for (unsigned j=0;j<5;j++) e->a[j]=u32(&r);e->a[5]=u8(&r);break;
                case E_SWEEP:for (unsigned j=0;j<5;j++) e->a[j]=u32(&r);e->a[5]=u16(&r);e->a[6]=u16(&r);break;
                case E_FLOOR:e->a[0]=u32(&r);e->a[1]=u32(&r);e->a[2]=u32(&r);e->a[3]=u16(&r);break;
                case E_PLAT: {e->a[0]=u8(&r);unsigned n=u8(&r);e->a[1]=n;for (unsigned j=0;j<n;j++) {uint32_t v=u32(&r);if (j<2) e->a[2+j]=v;}break;}
                case E_PLATFREE:break;
                case E_HURT:e->a[0]=u8(&r);e->a[1]=u8(&r);e->a[2]=u8(&r);break;
                case E_ATTACK:e->a[0]=u16(&r);break;
                case E_BOUNDS:for (unsigned j=0;j<4;j++) e->a[j]=u32(&r);for (unsigned j=0;j<4;j++) e->a[4+j]=u16(&r);break;
                case E_DRAW:e->a[0]=u32(&r);for (unsigned j=1;j<5;j++) e->a[j]=u16(&r);break;
                case E_MISSION:e->a[0]=u8(&r);break;
                default:assert(0);
                }
            }
            error[0]=0;npending=0;last_effect_kind=-1;last_effect_label=0;
            int16_t ni=native_of[id];
            if (ni<0 && routine==0xd5976 && node_ctor[id]==0xd5976) {
                ni=neo_boss_spawn(&boss,pre);native_of[id]=ni;fixture_of[ni]=id;
            }
            char why[300]="";bool ok=true;
            if (ni<0 || !boss.nodes[ni].routine) {snprintf(why,sizeof(why),"node %u not alive natively",id);ok=false;}
            else if (boss.nodes[ni].routine!=routine) {snprintf(why,sizeof(why),"routine %06x/%06x",(unsigned)boss.nodes[ni].routine,(unsigned)routine);ok=false;}
            else if (pre && compare(&boss.nodes[ni],pre,why,sizeof(why))>=0) {ok=false;pre_fail++;memmove(why+4,why,strlen(why)+1);memcpy(why,"pre ",4);}
            neo_boss_result_t res=NEO_BOSS_MORE;
            if (ok) {
                current_node=ni;res=neo_boss_step(&boss,ni);
                /* bind the new boss nodes to the fixture's children, in allocation order */
                for (unsigned b=0;b<boss.born_count && b<npending;b++)
                    if (pending_child[b]>=0) {
                        int16_t q=boss.born[b];
                        if (fixture_of[q]>=0 && history_count[q]<256) history[q][history_count[q]++]=(uint16_t)fixture_of[q];
                        native_of[pending_child[b]]=q;fixture_of[q]=pending_child[b];
                    }
                if (res==NEO_BOSS_HANDED_OFF) cur=nev;   /* the rest ran in generic code */
                if (res==NEO_BOSS_UNSUPPORTED && getenv("BOSS_POOL")) {
                    for (int q=0;q<NEO_BOSS_NODES;q++) printf(" %d:%06x/%d",q,(unsigned)boss.nodes[q].routine,boss.nodes[q].depth);
                    printf("\n");
                }
                if (res==NEO_BOSS_UNSUPPORTED) {unsupported++;ok=false;snprintf(why,sizeof(why),"UNSUPPORTED %06x (%s)",(unsigned)boss.unsupported_label,error);}
                else if (error[0]) {ok=false;snprintf(why,sizeof(why),"%s",error);}
                else if (cur!=nev) {ok=false;snprintf(why,sizeof(why),"%u events left, next %s",nev-cur,ev_name[evs[cur].type]);}
                else {
                    uint32_t next=rd_l(post,0);
                    if (handoff) {
                        if (res!=NEO_BOSS_HANDED_OFF || (int32_t)last_effect_label!=handoff) {ok=false;snprintf(why,sizeof(why),"hand-off %06x expected %06x",(unsigned)last_effect_label,(unsigned)handoff);}
                        else handoffs++;
                    } else if (next==0x3f1eu) {
                        if (res!=NEO_BOSS_DELETED) {ok=false;snprintf(why,sizeof(why),"not deleted (%d)",res);} else deletes++;
                    } else if (res!=NEO_BOSS_MORE) {ok=false;snprintf(why,sizeof(why),"result %d",res);}
                    else if (boss.nodes[ni].routine!=next) {ok=false;snprintf(why,sizeof(why),"next %06x/%06x",(unsigned)boss.nodes[ni].routine,(unsigned)next);}
                    else if (compare(&boss.nodes[ni],post,why,sizeof(why))>=0) ok=false;
                }
            }
            if (ok) {
                pass++;
                unsigned j;for (j=0;j<ncovered && covered[j]!=routine;j++);
                if (j==ncovered && ncovered<128) covered[ncovered++]=routine;
            } else {
                fail++;fail_rt[routine&0x1fff]++;
                if (getenv("BOSS_LIST") && fail<=2) {
                    for (int16_t q=boss.first;q>=0;q=boss.nodes[q].next) printf(" [%d %06x d%u 6a=%02x f%d]",q,(unsigned)boss.nodes[q].routine,boss.nodes[q].depth,boss.nodes[q].f[0x6a],fixture_of[q]);
                    printf("\n");
                }
                if (fail<=30) printf("  %s #%u f=%u node %u rt %06x: %s\n",F->name,(unsigned)k,(unsigned)frame,id,(unsigned)routine,why);
                if (getenv("BOSS_STOP")) stop=true;
            }
        }
        printf("fight %s: %u updates pass, %u fail (%u before the update, %u unsupported); hits %u (%u without attack row)\n",
            F->name,pass,fail,pre_fail,unsupported,hits,hits_unknown);
        printf("  draws %u ok %u bad; effect seeds %u ok %u bad; hand-offs %u; deletes %u; routines reproduced %u:",draws_ok,draws_bad,effects_ok,effects_bad,handoffs,deletes,ncovered);
        for (unsigned j=0;j<ncovered;j++) printf(" %05x",(unsigned)covered[j]);
        printf("\n");
        for (unsigned j=0;j<0x2000;j++) if (fail_rt[j]) printf("  routine ..%04x: %u failures\n",j,fail_rt[j]);
        total_fail+=fail;total_pass+=pass;total_draws+=draws_ok;
#ifdef N64
        fclose(r.f);
#endif
    }
#ifdef N64
    if (!total_fail) debugf("MSX BOSS REGRESSION PASS fights=%u updates=%u draws=%u\n",fights,total_pass,total_draws);
    else debugf("MSX BOSS REGRESSION FAIL %u\n",total_fail);
    for (;;) wait_ms(100);
#endif
    if (total_fail) {printf("FAIL\n");return 1;}
    printf("PASS: native boss reproduces the recorded fights (%u updates)\n",total_pass);
    return 0;
}
