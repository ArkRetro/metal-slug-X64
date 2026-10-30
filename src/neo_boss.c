#include "neo_boss.h"
#include <stddef.h>
#include <string.h>
#include "boss_tables.h"
#include "neo_soldier_ai.h"   /* neo_angle (ROM 002EB4) */

/* Native translation of the Mission 1 boss family (ROM 0D5976..0D74D4) and
 * the shell path 02A260..02A3B2. Each routine is labelled with its ROM
 * entry and follows the original instruction order, including writes that
 * are overwritten later in the same update. Engine services the original
 * calls (0029C6, 0040BC, 009EA8, 0120F8, 0140C0, 0143E4, 0146E8, 004C24,
 * 0049E8, 019504, 01DA40, 01DC8E, 018F92/018F40) go through the explicit
 * service table; their callers' logic is native. Nothing here decodes or
 * executes 68000 instructions or dereferences an original address. */

static int32_t S32(uint32_t v) {return v<=0x7fffffffu?(int32_t)v:(int32_t)((int64_t)v-0x100000000LL);}
static int16_t S16(uint16_t v) {return (int16_t)(v<=0x7fffu?(int)v:(int)v-65536);}
static int8_t S8(uint8_t v) {return (int8_t)(v<=0x7fu?(int)v:(int)v-256);}
static int32_t ASRL(int32_t v,unsigned n) {return v>=0?v>>n:-(int32_t)(((uint32_t)-(int64_t)v+((1u<<n)-1u))>>n);}
static int16_t ASRW(int16_t v,unsigned n) {return (int16_t)ASRL(v,n);}

uint8_t neo_boss_b(const neo_boss_node_t *n,unsigned o) {return n->f[o];}
uint16_t neo_boss_w(const neo_boss_node_t *n,unsigned o) {return (uint16_t)(n->f[o]<<8|n->f[o+1]);}
uint32_t neo_boss_l(const neo_boss_node_t *n,unsigned o) {return (uint32_t)neo_boss_w(n,o)<<16|neo_boss_w(n,o+2);}
static void set_b(neo_boss_node_t *n,unsigned o,uint8_t v) {n->f[o]=v;}
static void set_w(neo_boss_node_t *n,unsigned o,uint16_t v) {n->f[o]=(uint8_t)(v>>8);n->f[o+1]=(uint8_t)v;}
static void set_l(neo_boss_node_t *n,unsigned o,uint32_t v) {set_w(n,o,(uint16_t)(v>>16));set_w(n,o+2,(uint16_t)v);}

typedef struct {
    neo_boss_t *w;
    neo_boss_node_t *n;
    int16_t i;
    bool unsupported,deleted,handed_off;
} ctx_t;
#define N (c->n)
#define B(o) neo_boss_b(N,o)
#define W(o) neo_boss_w(N,o)
#define L(o) neo_boss_l(N,o)
#define SB(o,v) set_b(N,o,(uint8_t)(v))
#define SW(o,v) set_w(N,o,(uint16_t)(v))
#define SL(o,v) set_l(N,o,(uint32_t)(v))
#define SV (&c->w->services)
#define ENV (&c->w->env)
/* An original path that this port has not recovered: stop the update. */
#define FAIL(label) do { c->w->unsupported_label=(label);c->unsupported=true;return; } while (0)
#define CHECK do { if (c->unsupported||c->deleted||c->handed_off) return; } while (0)
static void fail(ctx_t *c,uint32_t label) {if (!c->unsupported) c->w->unsupported_label=label;c->unsupported=true;}

/* ---- node pool / list -------------------------------------------------------- */
#define DEAD 0x3f1eu   /* ROM 003F1E: a deleted node still linked before deeper nodes */
static neo_boss_node_t *node_at(neo_boss_t *w,int16_t i) {return i>=0 && i<NEO_BOSS_NODES && w->nodes[i].routine?&w->nodes[i]:NULL;}
static neo_boss_node_t *tag_node(ctx_t *c,uint32_t tag) {
    if (!NEO_BOSS_IS_TAG(tag)) return NULL;
    return node_at(c->w,(int16_t)NEO_BOSS_TAG_INDEX(tag));
}
static neo_boss_node_t *creator(ctx_t *c) {return node_at(c->w,N->creator);}
int16_t neo_boss_next(const neo_boss_t *w,int16_t i) {return i<0?w->first:w->nodes[i].next;}
static int16_t free_slot(neo_boss_t *w) {
    for (int16_t i=0;i<NEO_BOSS_NODES;i++) if (!w->nodes[i].routine) return i;
    return -1;
}
/* ROM 0040BC: the new node is linked right after its creator. */
static int16_t link_after(neo_boss_t *w,int16_t creator_index,uint32_t routine,const uint8_t image[NEO_BOSS_FIELDS]) {
    int16_t i=free_slot(w);
    if (i<0) return -1;
    neo_boss_node_t *n=&w->nodes[i];
    memset(n,0,sizeof(*n));
    memcpy(n->f,image,NEO_BOSS_FIELDS);
    n->routine=routine;n->creator=creator_index;
    if (creator_index<0) {n->next=w->first;w->first=i;n->depth=n->f[0x11];}
    else {
        neo_boss_node_t *p=&w->nodes[creator_index];n->next=p->next;p->next=i;n->depth=(uint8_t)(p->depth+1);
        n->f[0x10]=0;n->f[0x11]=n->depth;   /* 0040F0..0040F6 */
        n->f[0x68]=p->f[0x68];      /* 004110 */
    }
    return i;
}
static int16_t previous(neo_boss_t *w,int16_t i) {
    if (w->first==i) return -1;
    for (int16_t p=w->first;p>=0;p=w->nodes[p].next) if (w->nodes[p].next==i) return p;
    return -1;
}
static void unlink_node(neo_boss_t *w,int16_t i) {
    int16_t p=previous(w,i);
    if (p<0) w->first=w->nodes[i].next; else w->nodes[p].next=w->nodes[i].next;
    w->nodes[i].routine=0;w->nodes[i].next=-1;
}
/* ROM 004126 after 003F90: mark dead; unlink unless deeper nodes follow,
 * then retry on dead predecessors that were waiting for this one. */
static void kill(neo_boss_t *w,int16_t i) {
    w->nodes[i].routine=DEAD;
    while (i>=0 && w->nodes[i].routine==DEAD) {
        int16_t k=w->nodes[i].next;
        if (k>=0 && w->nodes[k].depth>w->nodes[i].depth) return;
        int16_t p=previous(w,i);
        unlink_node(w,i);
        i=p;
    }
}
void neo_boss_init(neo_boss_t *w,const neo_boss_services_t *s) {
    memset(w,0,sizeof(*w));w->first=-1;
    if (s) w->services=*s;
}
int16_t neo_boss_spawn(neo_boss_t *w,const uint8_t image[NEO_BOSS_FIELDS]) {
    /* The script actor inserts the record's actor after itself (00411C); the
     * boss is the only boss node then, so it starts the boss list. */
    int16_t i=link_after(w,-1,0xd5976u,image);
    return i;
}

/* ---- animation (004256 / 00427A) ------------------------------------------------ */
static const neo_anim_program_t *program(uint32_t tag) {
    for (unsigned i=0;i<BOSS_PROGRAM_COUNT;i++) if (boss_programs[i].tag==tag) return &boss_programs[i].program;
    return NULL;
}
static void anim_start(ctx_t *c,uint32_t tag) {
    const neo_anim_program_t *p=program(tag);
    if (!p) {fail(c,tag);return;}
    /* 004256: +18=0, +20=program, +24=0, +26=0x100. */
    neo_anim_start(&N->anim,p,0x100);
    SB(0x18,0);SL(0x20,tag);SW(0x24,0);SW(0x26,0x100);
}
typedef struct { ctx_t *c; } anim_ctx_t;
static void emit(void *context,const neo_anim_draw_t *d) {
    ctx_t *c=context;
    neo_anim_draw_t draw=*d;
    if (SV->draw) SV->draw(SV->context,c->i,&draw);
}
static bool field_read(void *context,uint16_t field,uint8_t *value) {
    ctx_t *c=context;if (field<0x6e || field>=NEO_BOSS_FIELDS) return false;
    *value=B(field);return true;
}
static bool field_write(void *context,uint16_t field,unsigned size,uint32_t value) {
    ctx_t *c=context;
    if (field==0x96 && size==4) {SL(0x96,value);return true;}
    if (field<0x6e || field+size>NEO_BOSS_FIELDS) return false;
    if (size==1) SB(field,value); else if (size==2) SW(field,value); else SL(field,value);
    return true;
}
static void anim_sound(void *context,uint16_t id) {ctx_t *c=context;if (SV->sound) SV->sound(SV->context,id);}
static void d72e4(ctx_t *c);static void d7308(ctx_t *c);static void d7330(ctx_t *c);
static void d7354(ctx_t *c);static void d738e(ctx_t *c);static void d739c(ctx_t *c);
static void p2a680(ctx_t *c);static void p2a6a2(ctx_t *c);
static bool anim_call(void *context,uint32_t routine,uint16_t parameter,neo_anim_render_t *render) {
    ctx_t *c=context;(void)parameter;
    /* Callbacks run with the render words already latched; they only touch
     * fields the remaining commands do not reread from `render`, except the
     * palette byte of 02A6A2, mirrored below. */
    switch (routine) {
    case 0xd72e4:d72e4(c);break;
    case 0xd7308:d7308(c);break;
    case 0xd7330:d7330(c);break;
    case 0xd7354:d7354(c);break;
    case 0xd738e:d738e(c);break;
    case 0xd739c:d739c(c);break;
    case 0xd74c6:SW(0x66,0xe0ef);SW(0x1e,0xe0ef);render->z=0xe0ef;break;
    case 0x2a680:p2a680(c);break;
    case 0x2a6a2:p2a6a2(c);render->attributes=(uint16_t)((render->attributes&255u)|((unsigned)B(0x14)<<8));break;
    default:return false;
    }
    return !c->unsupported;
}
/* ROM 00427A. Returns true when the program signals completion (carry). */
static bool anim_step(ctx_t *c) {
    neo_anim_state_t *a=&N->anim;
    if (!L(0x20)) return true;       /* 004280: no program, carry set */
    a->flash=B(0x17);a->field18=B(0x18);a->timer=W(0x24);a->speed=W(0x26);a->field50_tag=L(0x50);
    neo_anim_render_t r;memset(&r,0,sizeof(r));
    r.x=W(0x3c);r.y=W(0x40);r.attributes=(uint16_t)(B(0x14)<<8|B(0x15));r.z=W(0x1e);
    r.correction=B(0x4d);r.flash_palette=B(0x16);
    for (unsigned k=0;k<4;k++) r.palettes[k]=B(0x19+k);
    r.variant61=B(0x61);
    /* 00436E: +5F bit 3 hides the draws on odd video frames (RAM 10800F). */
    r.suppress=(B(0x5f)&8u) && (c->w->env.frame&1u);
    neo_anim_actor_t actor={NULL,field_read,field_write,anim_sound,anim_call,c};
    neo_anim_result_t res=neo_anim_step_actor(a,&r,emit,c,1024,&actor);
    SB(0x17,a->flash);SB(0x18,a->field18);SW(0x24,a->timer);SW(0x26,a->speed);SL(0x50,a->field50_tag);
    SB(0x15,(uint8_t)r.attributes);
    if (c->unsupported) return false;
    if (res!=NEO_ANIM_MORE && res!=NEO_ANIM_COMPLETE) {fail(c,0x427a);return false;}
    return res==NEO_ANIM_COMPLETE;
}

/* ---- positions -------------------------------------------------------------------- */
static neo_contact_actor_t body(const neo_boss_node_t *n) {
    neo_contact_actor_t a;memset(&a,0,sizeof(a));
    a.x=S32(neo_boss_l(n,0x28));a.y=S32(neo_boss_l(n,0x2c));a.vx=S32(neo_boss_l(n,0x30));a.vy=S32(neo_boss_l(n,0x34));
    a.parent_tag=neo_boss_l(n,0x38);a.projected_x=S32(neo_boss_l(n,0x3c));a.projected_y=S32(neo_boss_l(n,0x40));
    a.cached_x=S32(neo_boss_l(n,0x44));a.cached_y=S32(neo_boss_l(n,0x48));a.mark=n->f[0x4c];a.correction=n->f[0x4d];
    a.flags15=n->f[0x15];a.flags5f=n->f[0x5f];a.timer5a=n->f[0x5a];a.word1e=neo_boss_w(n,0x1e);a.field66=neo_boss_w(n,0x66);
    a.kind_angle=neo_boss_w(n,0x6c);a.scalar62=S32(neo_boss_l(n,0x62));
    return a;
}
static void store_body(neo_boss_node_t *n,const neo_contact_actor_t *a) {
    set_l(n,0x28,(uint32_t)a->x);set_l(n,0x2c,(uint32_t)a->y);set_l(n,0x30,(uint32_t)a->vx);set_l(n,0x34,(uint32_t)a->vy);
    set_l(n,0x38,a->parent_tag);set_l(n,0x3c,(uint32_t)a->projected_x);set_l(n,0x40,(uint32_t)a->projected_y);
    set_l(n,0x44,(uint32_t)a->cached_x);set_l(n,0x48,(uint32_t)a->cached_y);n->f[0x4c]=a->mark;n->f[0x4d]=a->correction;
    n->f[0x15]=a->flags15;n->f[0x5f]=a->flags5f;n->f[0x5a]=a->timer5a;set_w(n,0x1e,a->word1e);set_w(n,0x66,a->field66);
    set_w(n,0x6c,a->kind_angle);
}
/* Projection parent: another boss node (its +28 block) or a scene object. */
static bool parent_of(ctx_t *c,uint32_t tag,neo_contact_parent_t *p) {
    memset(p,0,sizeof(*p));p->tag=tag;
    if (!tag) return true;
    neo_boss_node_t *n=tag_node(c,tag);
    if (n) {
        p->x=S32(neo_boss_l(n,0x3c));p->y=S32(neo_boss_l(n,0x40));
        p->previous_x=S32(neo_boss_l(n,0x44));p->previous_y=S32(neo_boss_l(n,0x48));p->correction=n->f[0x4d];
        return true;
    }
    if (!SV->scene || !SV->scene(SV->context,tag,p)) return false;
    p->tag=tag;return true;
}
/* ROM 01ABC8/01ABCC on node n. */
static void project_node(ctx_t *c,neo_boss_node_t *n) {
    neo_contact_parent_t p;
    if (!parent_of(c,neo_boss_l(n,0x38),&p)) {fail(c,0x1abcc);return;}
    neo_contact_actor_t a=body(n);
    neo_contact_project(&a,neo_boss_l(n,0x38)?&p:NULL,c->w->env.epoch);
    store_body(n,&a);
}
static void project(ctx_t *c) {project_node(c,N);}
/* ROM 0143E4: air step (0128DA, 01444A..01448C, kernel, 014492 commit). */
static void air(ctx_t *c) {
    neo_contact_actor_t a=body(N);
    neo_contact_request_t q=neo_contact_prepare_air(&a);
    store_body(N,&a);
    neo_boss_sweep_t r;
    if (!SV->sweep || !SV->sweep(SV->context,&q,&r)) {fail(c,0x143e4);return;}
    neo_contact_parent_t p;
    if (!parent_of(c,r.parent,&p)) {fail(c,0x14506);return;}
    a=body(N);
    neo_contact_result_t res={r.x,r.y,r.vx,r.vy,r.parent,(uint32_t)r.kind<<8,r.flags};
    neo_contact_commit(&a,&res,r.parent?&p:NULL,c->w->env.epoch);
    store_body(N,&a);
}
/* ROM 0146E8: floor search, attach, project, grounded bit. */
static void ground(ctx_t *c) {
    neo_boss_sweep_t r;
    if (!SV->floor || !SV->floor(SV->context,S32(L(0x28)),S32(L(0x2c)),L(0x38),&r)) {fail(c,0x146e8);return;}
    SW(0x6c,r.kind);
    SL(0x28,r.x);SL(0x2c,r.y);SL(0x30,0);SL(0x34,0);SL(0x38,r.parent);
    project(c);
    SB(0x15,B(0x15)|0x40u);
}
/* ROM 01AB98 / 01AD6E: place at (dx,dy) pixels relative to the creator's
 * +28 block (the creator becomes the projection parent). */
static void place_on_creator(ctx_t *c,int16_t dx,int16_t dy) {
    SL(0x38,NEO_BOSS_TAG(N->creator));
    SL(0x28,(uint32_t)(uint16_t)dx<<16);SL(0x2c,(uint32_t)(uint16_t)dy<<16);
    SW(0x4c,0);
    project(c);
}
/* ROM 01B220 on child `ch` created by the current node (a6): copy +1E/+66
 * and the flip bits, mirror the offsets with the child's flips, then
 * 01ACF2 places it in the scene root at the creator's projected position. */
static void place_root(ctx_t *c,neo_boss_node_t *ch,int16_t dx,int16_t dy);
static void place_child(ctx_t *c,neo_boss_node_t *ch,int16_t dx,int16_t dy) {
    set_w(ch,0x1e,W(0x1e));set_w(ch,0x66,W(0x66));
    if (B(0x15)&1u) ch->f[0x15]|=1u;
    if (B(0x15)&2u) ch->f[0x15]|=2u;
    if (B(0x15)&4u) ch->f[0x15]|=4u;
    if (ch->f[0x15]&1u) dx=(int16_t)-dx;
    if (ch->f[0x15]&2u) dy=(int16_t)-dy;
    place_root(c,ch,dx,dy);
}
/* ROM 01ACF2: child at the current node's projected position + (dx,dy),
 * in the scene root (velocity = the node's history delta). */
static void place_root(ctx_t *c,neo_boss_node_t *ch,int16_t dx,int16_t dy) {
    neo_contact_parent_t root;
    if (!SV->scene || !SV->scene(SV->context,0x10e968u,&root)) {fail(c,0x1acf2);return;}
    uint32_t d0=(uint32_t)(uint16_t)dx<<16,d1=(uint32_t)(uint16_t)dy<<16,d2=d0,d3=d1;
    d0+=L(0x3c);d1+=L(0x40);d2+=L(0x44);d3+=L(0x48);
    d0-=(uint32_t)root.x;d1-=(uint32_t)root.y;d2-=(uint32_t)root.previous_x;d3-=(uint32_t)root.previous_y;
    d2-=d0;d3-=d1;d2=0u-d2;d3=0u-d3;
    set_l(ch,0x28,d0);set_l(ch,0x2c,d1);set_l(ch,0x30,d2);set_l(ch,0x34,d3);set_l(ch,0x38,0x10e968u);
    project_node(c,ch);
}

/* ---- palettes (009EA8, 004520, 009F88) ---------------------------------------------- */
static void palette(ctx_t *c,uint16_t resource) {
    uint8_t k=B(0x1d);
    if (k>=4) {fail(c,0x9eb2);return;}      /* original TRAP #15 */
    uint8_t slot;
    if (!SV->palette || !SV->palette(SV->context,resource,&slot)) {fail(c,0x9ea8);return;}
    SB(0x19+k,slot);SB(0x14,slot);SB(0x1d,k+1);
}
/* ROM 004520: allocate, keep the slot in +16 (flash palette), restore +14. */
static void palette_flash(ctx_t *c,uint16_t resource) {
    uint8_t keep=B(0x14);
    palette(c,resource);if (c->unsupported) return;
    SB(0x16,B(0x14));SB(0x17,0);SB(0x14,keep);
}
static void palette_release(ctx_t *c) {
    /* ROM 009F88: the allocator's reference counts are the world's. */
    if (SV->palette_release && B(0x1d)) SV->palette_release(SV->context,&N->f[0x19],B(0x1d));
    SB(0x1d,0);
}

/* ---- difficulty tables (0260C8) -------------------------------------------------- */
/* ROM data the boss reads (difficulty tables, paths, velocity rows, rectangles),
 * exported offline as regions by tools/extract_boss_tables.py. */
static const uint8_t *rom_at(uint32_t address,unsigned bytes) {
    for (unsigned i=0;i<BOSS_ROM_TABLE_COUNT;i++) {
        const boss_rom_table_t *t=&boss_rom_tables[i];
        if (address>=t->label && address+bytes<=t->label+t->length) return t->bytes+(address-t->label);
    }
    return NULL;
}
static const uint8_t *rom_table(uint32_t label) {return rom_at(label,2);}
static uint16_t rd16(const uint8_t *p) {return (uint16_t)(p[0]<<8|p[1]);}
static uint32_t rd32(const uint8_t *p) {return (uint32_t)rd16(p)<<16|rd16(p+2);}
static uint16_t random16(ctx_t *c) {
    if (!SV->random) {fail(c,0x29c6);return 0;}
    return SV->random(SV->context);
}
static uint16_t table(ctx_t *c,uint32_t label) {
    const uint8_t *t=rom_table(label);
    if (!t) {fail(c,label);return 0;}
    /* 0260A6: d7 = 1 when both player nodes are live. */
    unsigned d7=(ENV->p1_node && ENV->p2_node)?1u:0u;
    uint8_t d6=t[0],d5=t[1];
    uint8_t d1=ENV->rank;
    if (ENV->players88!=2) d1=3;
    if (d1>=7 && d7>=2) d7=1;
    unsigned mask=(1u<<((d1&7u)+1u))-1u;
    uint8_t d0=boss_bit_counts[d5&mask];
    if (d0<1) {fail(c,0x26118);return 0;}
    unsigned index=(unsigned)(d0-1u)*2u+d7;
    uint8_t v68=B(0x68);
    if (v68>=12) {fail(c,0x2613e);return 0;}
    if (d6!=2) return rd16(t+2+index*12u+v68*2u);
    /* type 2: a byte row, 16-byte groups, a random word of 8 */
    unsigned off=(unsigned)t[2+index*6u+v68]<<4;
    off+=(unsigned)boss_bit_counts[d5]*12u;
    uint16_t r=random16(c);
    return rd16(t+2+off+(r&0xeu));
}

/* ---- children (0040BC) ------------------------------------------------------------- */
/* A failed allocation (pool short of `reserve`) returns the original's
 * scratch node: later writes land there and are discarded. */
static neo_boss_node_t scratch_node;
static neo_boss_node_t *allocate(ctx_t *c,uint32_t ctor,uint16_t reserve,int16_t *index) {
    uint8_t image[NEO_BOSS_FIELDS];
    *index=-1;
    if (!SV->allocate) {fail(c,0x40bc);return &scratch_node;}
    if (!SV->allocate(SV->context,reserve,image)) {memset(&scratch_node,0,sizeof(scratch_node));return &scratch_node;}
    int16_t i=link_after(c->w,c->i,ctor,image);
    if (i<0) {fail(c,0x40bc);return &scratch_node;}
    if (c->w->born_count<16) c->w->born[c->w->born_count++]=i;
    *index=i;
    return &c->w->nodes[i];
}
/* Boss part: allocate, then 01B220 at (dx,dy). */
static neo_boss_node_t *spawn_part(ctx_t *c,uint32_t ctor,uint16_t reserve,int16_t dx,int16_t dy,int16_t *child) {
    int16_t i;neo_boss_node_t *ch=allocate(c,ctor,reserve,&i);
    if (!c->unsupported && i>=0) place_child(c,ch,dx,dy);
    if (child) *child=i;
    return ch;
}
/* A generic actor the boss creates (explosions 01C706/01C71E, prisoners
 * 099DCC): allocated and placed like a part, then handed to the world with
 * the seed image; it is not simulated here. */
static void effect_commit(ctx_t *c,neo_boss_node_t *ch,uint32_t ctor);
static neo_boss_node_t *effect_child(ctx_t *c,uint32_t ctor,uint16_t reserve,int16_t dx,int16_t dy) {
    int16_t i;neo_boss_node_t *ch=allocate(c,ctor,reserve,&i);
    if (!c->unsupported && i>=0) place_child(c,ch,dx,dy);
    return ch;
}
/* Hand a just-created generic child over to the world. */
static void effect_commit(ctx_t *c,neo_boss_node_t *ch,uint32_t ctor) {
    if (ch==&scratch_node || c->unsupported) return;
    int16_t i=(int16_t)(ch-c->w->nodes);
    neo_boss_effect_t e;memset(&e,0,sizeof(e));
    e.kind=NEO_BOSS_EFFECT_SPAWN;e.label=ctor;e.node=c->i;
    memcpy(e.image,ch->f,NEO_BOSS_FIELDS);e.has_image=true;
    kill(c->w,i);
    if (!SV->effect || !SV->effect(SV->context,&e)) fail(c,ctor);
}
/* ROM 01DA40: debris spawner (generic 01D764 actor with ROM table). */
static void debris(ctx_t *c,uint32_t label) {
    int16_t i;neo_boss_node_t *ch=allocate(c,0x1d764u,0x1c,&i);
    if (c->unsupported) return;
    if (i>=0) {
        place_child(c,ch,0,0);
        if (B(0x15)&4u) ch->f[0x15]|=4u;
        set_l(ch,0x86,label);
    }
    effect_commit(c,ch,0x1d764u);
}
/* ROM 01DC8E: smoke spawner (generic 01D86C actor, +70 = parameter). */
static void smoke(ctx_t *c,uint16_t param) {
    int16_t i;neo_boss_node_t *ch=allocate(c,0x1d86cu,0x1c,&i);
    if (c->unsupported) return;
    if (i>=0) place_child(c,ch,0,0);
    ch->f[0x94]=0;set_w(ch,0x6e,0);set_w(ch,0x70,param);set_w(ch,0x72,0x3f);set_w(ch,0x74,0x1f);
    effect_commit(c,ch,0x1d86cu);
}
/* The current node jumps into generic code (01C71E, 01C706, 08EEBC): the
 * world continues it from this image. */
static void convert(ctx_t *c,uint32_t label) {
    neo_boss_effect_t e;memset(&e,0,sizeof(e));
    e.kind=NEO_BOSS_EFFECT_CONVERT;e.label=label;e.node=c->i;
    memcpy(e.image,N->f,NEO_BOSS_FIELDS);e.has_image=true;
    if (!SV->effect || !SV->effect(SV->context,&e)) {fail(c,label);return;}
    /* Its descendants (shell trails reading the shell's palette and depth)
     * follow it into the world: they now depend on generic state. */
    for (;;) {
        int16_t k=N->next;
        if (k<0 || c->w->nodes[k].depth<=N->depth) break;
        neo_boss_node_t *d=&c->w->nodes[k];
        if (d->routine!=DEAD) {
            neo_boss_effect_t f;memset(&f,0,sizeof(f));
            f.kind=NEO_BOSS_EFFECT_CONVERT;f.label=d->routine;f.node=k;
            memcpy(f.image,d->f,NEO_BOSS_FIELDS);f.has_image=true;
            if (!SV->effect(SV->context,&f)) {fail(c,d->routine);return;}
        }
        d->routine=DEAD;N->next=d->next;d->routine=0;d->next=-1;
    }
    kill(c->w,c->i);c->handed_off=true;
}
static void sound(ctx_t *c,uint16_t id) {if (SV->sound) SV->sound(SV->context,id);}
/* ROM 003F90: release the palette references and the object platform. */
static void release(ctx_t *c,int16_t i) {
    neo_boss_node_t *n=&c->w->nodes[i];
    if (SV->palette_release && n->f[0x1d]) SV->palette_release(SV->context,&n->f[0x19],n->f[0x1d]);
    if (n->f[0x6a]!=0xff) {
        if (SV->platform_release) SV->platform_release(SV->context,i,n->f[0x6a]);
        n->f[0x6a]=0xff;
    }
}
/* ROM 004188: delete this node and leave the update. */
static void delete_self(ctx_t *c) {release(c,c->i);kill(c->w,c->i);c->deleted=true;}
/* ROM 0041AC: delete the following deeper nodes (boss parts here; generic
 * descendants are the world's), then this node. */
static void delete_tree(ctx_t *c) {
    neo_boss_effect_t e;memset(&e,0,sizeof(e));
    e.kind=NEO_BOSS_EFFECT_DELETE_TREE;e.label=0x41ac;e.node=c->i;
    if (SV->effect && !SV->effect(SV->context,&e)) {fail(c,0x41ac);return;}
    for (;;) {
        int16_t k=N->next;
        if (k<0 || c->w->nodes[k].depth<=N->depth) break;
        if (c->w->nodes[k].routine!=DEAD) release(c,k);
        kill(c->w,k);
        if (c->w->nodes[k].routine==DEAD) break;   /* still waiting for deeper nodes */
    }
    delete_self(c);
}
/* ROM 00409A: carry set while the creator is alive. */
static bool creator_alive(ctx_t *c) {neo_boss_node_t *p=creator(c);return N->creator<0 || (p && p->routine!=DEAD);}
/* ROM 0044F6: share the creator's flash timer (+17). */
static bool share_flash(ctx_t *c) {
    neo_boss_node_t *p=creator(c);
    uint8_t d0=p?p->f[0x17]:0;
    if (!d0) return false;
    if (!(d0&0x80u)) d0++;
    SB(0x17,d0);
    return !(d0&1u);
}

/* ---- world interfaces ---------------------------------------------------------------- */
/* ROM 0140C0 with table `label` (A0). */
static void platform(ctx_t *c,uint32_t label) {
    int32_t pos[4]={S32(L(0x28)),S32(L(0x2c)),S32(L(0x30)),S32(L(0x34))};
    uint8_t slot=B(0x6a),out=slot;
    if (!SV->platform || !SV->platform(SV->context,c->i,slot,label,pos,L(0x38),&out)) {fail(c,0x140c0);return;}
    SB(0x6a,out);
}
/* ROM 013F1E. */
static void platform_release(ctx_t *c) {
    uint8_t slot=B(0x6a);
    if (slot==0xff) return;
    if (slot>=8) {fail(c,0x13f34);return;}
    if (SV->platform_release) SV->platform_release(SV->context,c->i,slot);
    SB(0x6a,0xff);
}
/* ROM 004C24: register the hurtbox; carry (returned) when +5A is set. */
static bool hurtbox(ctx_t *c) {
    if (L(0x50)) {
        uint8_t off=0,idx=0;bool kill=false;
        if (!SV->hurtbox || !SV->hurtbox(SV->context,c->i,&off,&idx,&kill)) {fail(c,0x4c24);return false;}
        if (kill) {
            /* 004BAC: kill-all condition, unless the table's class byte is 1. */
            const uint8_t *t=rom_table(L(0x50));
            if (!t) {fail(c,L(0x50));return false;}
            if (t[1]!=1) {SW(0x4e,0);SB(0x5f,B(0x5f)|0x80u);SB(0x5a,6);return true;}
        }
        if (off==0x58 || off==0x59) SB(off,idx);
    }
    return B(0x5a)!=0;
}
/* ROM 0049E8 with the node's attack table (+96 or an explicit ROM table). */
static uint16_t attack(ctx_t *c,uint32_t rows) {
    uint16_t r=0;
    if (!SV->attack || !SV->attack(SV->context,c->i,rows,&r)) {fail(c,0x49e8);return 0;}
    return r;
}
static bool bounds(ctx_t *c,neo_cull_bounds_t *b) {
    if (!SV->bounds || !SV->bounds(SV->context,b)) {fail(c,0x19504);return false;}
    return true;
}
/* ROM 019504: carry set when the rectangle is outside the scene. */
static bool outside(ctx_t *c,uint32_t label) {
    const uint8_t *t=rom_table(label);
    if (!t) {fail(c,label);return false;}
    neo_cull_bounds_t b;if (!bounds(c,&b)) return false;
    uint16_t rect[4]={rd16(t),rd16(t+2),rd16(t+4),rd16(t+6)};
    neo_contact_actor_t a=body(N);
    return neo_cull_rectangle_outside(&a,&b,rect);
}

/* ---- shared boss helpers ------------------------------------------------------------- */
static uint32_t rom_long(ctx_t *c,uint32_t address) {
    const uint8_t *p=rom_at(address,4);
    if (!p) {fail(c,address);return 0;}
    return rd32(p);
}
static uint16_t rom_word(ctx_t *c,uint32_t address) {
    const uint8_t *p=rom_at(address,2);
    if (!p) {fail(c,address);return 0;}
    return rd16(p);
}
static uint8_t rom_byte(ctx_t *c,uint32_t address) {
    const uint8_t *p=rom_at(address,1);
    if (!p) {fail(c,address);return 0;}
    return p[0];
}
/* Program from a ROM table of program labels (MOVEA.L (A1,D0.W),A1). */
static void anim_table(ctx_t *c,uint32_t table_label,unsigned index) {
    uint32_t tag=rom_long(c,table_label+index*4u);
    if (!c->unsupported) anim_start(c,tag);
}
/* (word << 16) >> 8 with ASR.L, the original velocity encoding. */
static uint32_t speed8(uint16_t w) {return (uint32_t)ASRL(S32((uint32_t)w<<16),8);}
/* Player tag -> projected words (the original reads +3C/+40 of the node). */
static bool player_xy(ctx_t *c,uint32_t tag,int16_t *x,int16_t *y) {
    if (tag==NEO_BOSS_PLAYER1) {*x=ENV->p1x;*y=ENV->p1y;return true;}
    if (tag==NEO_BOSS_PLAYER2) {*x=ENV->p2x;*y=ENV->p2y;return true;}
    fail(c,tag);return false;
}
/* ROM 01AF9E: choose among the players of `mask` (01AE8E bits). Carry
 * (returned) when no player is live; the tag is then player 1. */
static bool choose_player(ctx_t *c,uint8_t mask,uint32_t *tag) {
    *tag=NEO_BOSS_PLAYER1;
    if (!mask) return true;
    if (mask==1) return false;
    if (mask==2) {*tag=NEO_BOSS_PLAYER2;return false;}
    int16_t d0=S16(W(0x3c));
    int16_t d1=(int16_t)(ENV->p1x-d0),d2=(int16_t)(ENV->p2x-d0);
    if (d1<0) d1=(int16_t)-d1;
    if (d2<0) d2=(int16_t)-d2;
    if (d2<d1) *tag=NEO_BOSS_PLAYER2;
    return false;
}
/* ROM 01AF9A. */
static bool nearest_player(ctx_t *c,uint32_t *tag) {return choose_player(c,ENV->alive_mask,tag);}
/* ROM 01AF3C: carry (true) when the player is outside rectangle `rect`
 * (x,y,w,h) relative to this node, mirrored by its flips. */
static bool player_outside(ctx_t *c,uint32_t tag,const uint8_t *rect) {
    int16_t px,py;if (!player_xy(c,tag,&px,&py)) return true;
    int16_t d1=(int16_t)(py-S16(W(0x40))),d2=S16(rd16(rect+2)),d3=(int16_t)(d2+S16(rd16(rect+6)));
    if (B(0x15)&2u) d1=(int16_t)-d1;
    if (d1<d2 || d1>d3) return true;
    d1=(int16_t)(px-S16(W(0x3c)));d2=S16(rd16(rect));d3=(int16_t)(d2+S16(rd16(rect+4)));
    if (B(0x15)&1u) d1=(int16_t)-d1;
    return d1<d2 || d1>d3;
}
/* ROM 01AFFC with rectangle `label`. */
static bool player_in_rect(ctx_t *c,uint32_t label,uint32_t *tag) {
    const uint8_t *rect=rom_at(label,8);
    if (!rect) {fail(c,label);return true;}
    uint8_t mask=ENV->alive_mask;
    if (mask) {
        if ((mask&1u) && player_outside(c,NEO_BOSS_PLAYER1,rect)) mask^=1u;
        if ((mask&2u) && player_outside(c,NEO_BOSS_PLAYER2,rect)) mask^=2u;
    }
    return choose_player(c,mask,tag);
}
/* ROM 0D70F2: follow the creator (+1E = creator +1E + own +80, offsets
 * +7B/+7C copied and used as the X/Y pixel words). Projects first. */
static neo_boss_node_t *follow(ctx_t *c) {
    project(c);
    neo_boss_node_t *p=creator(c);
    if (!p) {fail(c,0xd70f8);return NULL;}
    SW(0x1e,W(0x80)+neo_boss_w(p,0x1e));
    uint8_t d0=p->f[0x7b],d1=p->f[0x7c];
    SB(0x7b,d0);SB(0x7c,d1);
    SW(0x28,(uint16_t)S8(d0));SW(0x2c,(uint16_t)S8(d1));
    return p;
}
/* ROM 0D7126: follow, then restart the program of table +92 when the
 * creator's state byte +7A changes. */
static void follow_state(ctx_t *c) {
    neo_boss_node_t *p=follow(c);CHECK;
    uint8_t d0=p->f[0x7a];
    if (d0==B(0x7a)) return;
    SB(0x7a,d0);
    anim_table(c,L(0x92),d0&0x1fu);
}
/* ROM 0D7164: gun offsets (lower when the body's phase is 4 or more). */
static void gun_offset(ctx_t *c) {
    neo_boss_node_t *p=creator(c);
    if (!p) {fail(c,0xd7168);return;}
    uint16_t d0=0x58;
    if (S8(p->f[0x79])>=4) d0=0x76;
    SW(0x2c,W(0x2c)+d0);SW(0x28,W(0x28)+0xffe0u);
    project(c);
    SW(0x66,0xe0ed);SW(0x1e,0xe0ed);
}
/* ROM 0D7198: missile pod offsets from the creator's +7E/+7F. */
static void pod_offset(ctx_t *c) {
    neo_boss_node_t *p=follow(c);CHECK;
    uint8_t d0=p->f[0x7e];uint16_t d1=2;
    if (B(0x7d)&1u) {d0=p->f[0x7f];d1=0xfff6;}
    SW(0x2c,W(0x2c)+d0);SW(0x28,W(0x28)+d1);
    project(c);
}
/* ROM 0D7062: carry when the walk must stop (screen edges / home X). */
static bool walk_blocked(ctx_t *c) {
    if (!(B(0x77)&1u)) return S16(W(0x3c))<0xa0;
    if (S16(W(0x3c))>0x118) return true;
    return S16(W(0x70))<S16(W(0x28));
}
/* ROM 0D709C: carry when the walk must stop because a player is close. */
static bool player_close(ctx_t *c) {
    if (B(0x77)&1u) return false;
    uint32_t tag;
    if (nearest_player(c,&tag)) return false;
    int16_t px,py;if (!player_xy(c,tag,&px,&py)) return false;
    return (int16_t)(S16(W(0x3c))-px)<0xc8;
}
/* ROM 0D704E. */
static bool walk_stop(ctx_t *c) {return walk_blocked(c) || player_close(c);}
/* ROM 0D70CC: carry when +28 is within 8 px of the home X (+6E). */
static bool at_home(ctx_t *c) {
    int16_t d0=(int16_t)(S16(W(0x6e))-8),d1=(int16_t)(S16(W(0x6e))+8),x=S16(W(0x28));
    return d0<x && d1>x;
}

/* ---- main body 0D5976 ----------------------------------------------------------------- */
#define GO(label,fn) do { N->routine=(label);fn(c);return; } while (0)
static void d5a92(ctx_t *c);static void d5ad4(ctx_t *c);static void d5b1e(ctx_t *c);static void d5b7e(ctx_t *c);
static void d5bba(ctx_t *c);static void d5bf8(ctx_t *c);static void d5c2c(ctx_t *c);static void d5c90(ctx_t *c);
static void d5cd2(ctx_t *c);static void d5cec(ctx_t *c);static void d5d04(ctx_t *c);static void d5d24(ctx_t *c);
static void d5d5c(ctx_t *c);static void d5d84(ctx_t *c);static void d5e5c(ctx_t *c);static void d5e88(ctx_t *c);
static void d5ea6(ctx_t *c);static void d5ebc(ctx_t *c);static void d5eda(ctx_t *c);
/* 0D5E4A: the body's attack rows against the players. */
static void body_attack(ctx_t *c) {if (L(0x96)) attack(c,L(0x96));}
static void d5976(ctx_t *c) {
    sound(c,0x1073);
    palette(c,0x2ec);CHECK;
    SL(0x96,0xe90d4);SL(0x50,0xe9064);SL(0x9a,0xe93b2);
    SW(0x66,0x8800);SW(0x1e,0x8800);
    neo_boss_node_t *ch=spawn_part(c,0xd600c,0x0e,0,0,NULL);CHECK;set_w(ch,0x80,0);
    ch=spawn_part(c,0xd6464,0x0e,0,0,NULL);CHECK;set_w(ch,0x80,0xffff);
    ch=spawn_part(c,0xd6500,0x0e,0,0,NULL);CHECK;set_w(ch,0x80,0xffff);
    spawn_part(c,0xd680a,0x0e,0,0,NULL);CHECK;
    SB(0x77,0);SB(0x79,0);SW(0xa8,0x140);SB(0x76,ENV->facing606);SW(0x4e,0x7fff);SB(0x5f,B(0x5f)|0x40u);
    anim_table(c,0xe919c,0);CHECK;
    SL(0x96,0xe90d4);
    GO(0xd5a92,d5a92);
}
/* Waits for the player to turn (RAM 10C606 against +76). */
static void d5a92(ctx_t *c) {
    if (L(0x9a)) {platform(c,L(0x9a));CHECK;}
    project(c);CHECK;
    anim_step(c);CHECK;
    if ((ENV->facing606^B(0x76))&1u) N->routine=0xd5ad4;
    hurtbox(c);CHECK;
    SB(0x5a,0);
    body_attack(c);
}
static void d5ad4(ctx_t *c) {
    sound(c,0x2c);
    ground(c);CHECK;
    palette_flash(c,0x319);CHECK;
    SW(0x4e,table(c,0x2ca81e));CHECK;
    SW(0x86,table(c,0x2ca880));CHECK;
    SB(0x5f,B(0x5f)&~0x40u);
    smoke(c,0x3c);CHECK;
    GO(0xd5b1e,d5b1e);
}
/* 0D5DF0..0D5E5A: phase 1 tail (hit flash, phase 2 threshold +86). */
static void phase1_tail(ctx_t *c) {
    if (hurtbox(c)) {
        CHECK;SB(0x5a,0);
        if (!B(0x79) && S16(W(0x4e))<=S16(W(0x86))) {
            SB(0x79,1);
            debris(c,0xe932a);CHECK;
            sound(c,0x1032);
            N->routine=0xd5c2c;c->w->phase_changes++;
        }
    }
    CHECK;
    if (B(0x5f)&0x80u) {SW(0x4e,0x14);SB(0x5f,B(0x5f)&~0x80u);}
    body_attack(c);
}
static void d5b1e(ctx_t *c) {
    if (B(0x79)) GO(0xd5c2c,d5c2c);
    if (walk_blocked(c)) GO(0xd5bba,d5bba);
    CHECK;
    uint16_t d0=W(0xa8);unsigned d1=4;
    if (!(B(0x77)&1u)) {d1=0;d0=(uint16_t)(0u-d0);}
    SL(0x30,speed8(d0));
    anim_table(c,0xe91bc,(B(0x78)&3u)+d1);CHECK;
    SL(0x96,0xe90c4);
    GO(0xd5b7e,d5b7e);
}
static void d5b7e(ctx_t *c) {
    if (L(0x9a)) {platform(c,L(0x9a));CHECK;}
    air(c);CHECK;
    if (walk_stop(c)) N->routine=0xd5bba;
    CHECK;
    if (!L(0x30)) N->routine=0xd5bba;
    anim_step(c);CHECK;
    phase1_tail(c);
}
static void d5bba(ctx_t *c) {
    SB(0x77,B(0x77)^1u);SL(0x30,0);SW(0x72,0x3c);
    anim_table(c,0xe919c,B(0x78)&7u);CHECK;
    SL(0x96,0xe90d4);
    GO(0xd5bf8,d5bf8);
}
static void d5bf8(ctx_t *c) {
    if (L(0x9a)) {platform(c,L(0x9a));CHECK;}
    project(c);CHECK;
    anim_step(c);CHECK;
    SW(0x72,W(0x72)-1u);
    if (S16(W(0x72))<=0) N->routine=0xd5b1e;
    phase1_tail(c);
}
/* Phase 2 entry: walk back home (+6E). */
static void d5c2c(ctx_t *c) {
    uint16_t d2=W(0x6e),d0=(uint16_t)ASRW(S16(W(0xa8)),1);unsigned d1=4;
    SB(0x77,1);
    if (!(S16(d2)>=S16(W(0x28)))) {d1=0;d0=(uint16_t)(0u-d0);SB(0x77,0);}
    SL(0x30,speed8(d0));
    anim_table(c,0xe91dc,(B(0x78)&3u)+d1);CHECK;
    SL(0x96,0xe90c4);SW(0x72,0xb4);
    GO(0xd5c90,d5c90);
}
static void d5c90(ctx_t *c) {
    if (L(0x9a)) {platform(c,L(0x9a));CHECK;}
    air(c);CHECK;
    if (at_home(c)) N->routine=0xd5cd2;
    anim_step(c);CHECK;
    SW(0x72,W(0x72)-1u);
    if (S16(W(0x72))<=0) N->routine=0xd5cd2;
    phase1_tail(c);
}
static void d5cd2(ctx_t *c) {
    SW(0x72,0x14);SB(0x79,2);SL(0x96,0xe90d4);
    GO(0xd5cec,d5cec);
}
static void d5cec(ctx_t *c) {
    SW(0x72,W(0x72)-1u);
    if (S16(W(0x72))<=0) N->routine=0xd5d04;
    d5d24(c);
}
static void d5d04(ctx_t *c) {
    sound(c,0x10a7);
    anim_start(c,0x2accba);CHECK;
    GO(0xd5d24,d5d24);
}
/* 0D5D24: transformation animation; 0D5E34 tail. */
static void d5d24(ctx_t *c) {
    if (L(0x9a)) {platform(c,L(0x9a));CHECK;}
    project(c);CHECK;
    if (anim_step(c)) N->routine=0xd5d5c;
    CHECK;
    if (hurtbox(c)) SB(0x5a,0);
    CHECK;
    if (B(0x5f)&0x80u) {SW(0x4e,0x14);SB(0x5f,B(0x5f)&~0x80u);}
    body_attack(c);
}
static void d5d5c(ctx_t *c) {
    SW(0x9e,table(c,0x2ca9a6));CHECK;
    SB(0xa0,0);
    platform_release(c);CHECK;
    SL(0x9a,0xe94ac);
    GO(0xd5d84,d5d84);
}
/* Phase 2 loop: +9E is the armour timer; a hit while it runs is undone. */
static void d5d84(ctx_t *c) {
    if (L(0x9a)) {platform(c,L(0x9a));CHECK;}
    project(c);CHECK;
    anim_step(c);CHECK;
    if (hurtbox(c)) SB(0x5a,0);
    CHECK;
    SW(0x9e,W(0x9e)-1u);
    if (!(S16(W(0x9e))>-1)) SW(0x9e,0xffff);
    if (B(0x5f)&0x80u) {
        if (S16(W(0x9e))>0) {SB(0x5f,B(0x5f)&~0x80u);SW(0x4e,0x14);}
        else {N->routine=0xd5e5c;c->w->deaths++;}
    }
    body_attack(c);
}
/* Death: mission complete (018F92), then two programs and the wreck. */
static void d5e5c(ctx_t *c) {
    if (SV->mission) SV->mission(SV->context,c->i,1);
    c->w->mission_complete=true;
    sound(c,0x1033);
    SB(0x79,6);
    anim_start(c,0x2acde4);CHECK;
    GO(0xd5e88,d5e88);
}
static void d5e88(ctx_t *c) {
    project(c);CHECK;
    if (anim_step(c)) {
        CHECK;
        if (SV->mission) SV->mission(SV->context,c->i,2);
        N->routine=0xd5ea6;
    }
}
static void d5ea6(ctx_t *c) {anim_start(c,0x2acdf4);CHECK;GO(0xd5ebc,d5ebc);}
static void d5ebc(ctx_t *c) {
    project(c);CHECK;
    if (anim_step(c)) N->routine=0xd5eda;
    CHECK;
    SB(0xa0,0xff);
}
static void d74b8(ctx_t *c) {debris(c,0xe933c);}
static void d5eda(ctx_t *c) {
    SW(0x40,W(0x40)+0x20u);
    sound(c,0x102f);
    static const int16_t xs[3]={-0x38,0,0x38};
    for (unsigned k=0;k<3;k++) {
        neo_boss_node_t *ch=effect_child(c,0x1c71e,0x1c,xs[k],0);CHECK;
        effect_commit(c,ch,0x1c71e);CHECK;
    }
    d74b8(c);CHECK;
    SW(0x40,W(0x40)-0x20u);
    anim_step(c);CHECK;
    /* 0D5F6C..0D5FF0: the protection check of the original hardware (RAM
     * 10F00A/10F00C, 2FFFE0) only decides between this delete and a crash. */
    delete_self(c);
}

/* ---- parts ---------------------------------------------------------------------------- */
static void d5ffc(ctx_t *c) {delete_self(c);}
static void d6004(ctx_t *c) {delete_tree(c);}
/* 0D600C: front armour (hurtbox 0E9144, 100 HP), programs by body state. */
static void d605e(ctx_t *c);static void d60b4(ctx_t *c);
static void d600c(ctx_t *c) {
    place_on_creator(c,0,0);CHECK;
    SL(0x92,0xe91fc);SL(0x50,0xe9144);SW(0x4e,0x64);
    palette(c,0x2ed);CHECK;palette_flash(c,0x31a);CHECK;
    anim_start(c,0x2ace04);CHECK;
    GO(0xd605e,d605e);
}
static void d605e(ctx_t *c) {
    follow_state(c);CHECK;
    project(c);CHECK;
    anim_step(c);CHECK;
    if (hurtbox(c)) SB(0x5a,0);
    CHECK;
    if (B(0x5f)&0x80u) N->routine=0xd60b4;
    neo_boss_node_t *p=creator(c);
    if (!p) {fail(c,0xd608c);return;}
    if (p->f[0x79]) N->routine=0xd60b4;
    if (p->f[0x7a]==6) N->routine=0xd60b4;
}
static void d60b4(ctx_t *c) {SL(0x50,0);SW(0x2c,W(0x2c)+0x40u);convert(c,0x1c71e);}
/* 0D60C8: turn the gun (+A6, 0..15) one step toward the creator's target
 * (+8A). Carry (true) when already aimed. */
static bool aim(ctx_t *c) {
    neo_boss_node_t *p=creator(c);
    if (!p) {fail(c,0xd60c8);return false;}
    int16_t px,py;if (!player_xy(c,neo_boss_l(p,0x8a),&px,&py)) return false;
    int16_t d0=(int16_t)(px-S16(W(0x3c)));
    if (!(d0>-0x100)) d0=-0x100;
    if (!(d0<0x100)) d0=0x100;
    unsigned i=(uint16_t)(d0+0x100)>>4;
    uint8_t v=(uint8_t)(rom_byte(c,0xe9288+i)&15u);
    if (c->unsupported) return false;
    if (v==B(0xa6)) return true;
    SB(0xa6,B(0xa6)+(S8((uint8_t)(v-B(0xa6)))>0?1:0xff));
    return false;
}
/* 0D6130: turret gun on 0D61F2. */
static void d6168(ctx_t *c);
static void d6130(ctx_t *c) {
    place_on_creator(c,0,0);CHECK;
    palette(c,0x347);CHECK;
    SB(0xa6,8);
    anim_start(c,0x2ada3e);CHECK;
    GO(0xd6168,d6168);
}
static void d6168(ctx_t *c) {
    aim(c);CHECK;
    neo_boss_node_t *p=creator(c);
    if (!p) {fail(c,0xd616c);return;}
    SW(0x2c,(uint16_t)S8(p->f[0x7c]));
    SW(0x1e,neo_boss_w(p,0x1e)+W(0x80));
    project(c);CHECK;
    if (anim_step(c)) {CHECK;anim_start(c,0x2ada3e);}
    CHECK;
    p=creator(c);
    /* 0D61B2: firing (08FA1A spawns the generic bullet 08EFFA) is not recovered. */
    if (p && p->f[0x61]) FAIL(0x8fa1a);
    if (ENV->mission_over) convert(c,0x8eebc);
}
/* 0D61F2: turret base on 0D6464. */
static void d624a(ctx_t *c);static void d6254(ctx_t *c);static void d62a2(ctx_t *c);static void d62bc(ctx_t *c);
static void d62d6(ctx_t *c);static void d62f2(ctx_t *c);static void d632e(ctx_t *c);static void d6348(ctx_t *c);
static void d6362(ctx_t *c);static void d637e(ctx_t *c);static void d63ae(ctx_t *c);static void d63c4(ctx_t *c);
static void d61f2(ctx_t *c) {
    place_on_creator(c,0x20,0);CHECK;
    palette(c,0x347);CHECK;
    anim_start(c,0x2ada14);CHECK;
    int16_t k;neo_boss_node_t *ch=spawn_part(c,0xd6130,0x0e,0,0,&k);CHECK;
    if (k<0) FAIL(0xd6240);    /* the original keeps the scratch node's address */
    SL(0x8e,NEO_BOSS_TAG(k));
    set_w(ch,0x80,0xffff);
    GO(0xd624a,d624a);
}
static void d624a(ctx_t *c) {SB(0x61,0);GO(0xd6254,d6254);}
/* 0D639C */
static void turret_tail(ctx_t *c) {if (ENV->mission_over) N->routine=0xd63ae;}
static neo_boss_node_t *gun_of(ctx_t *c) {
    neo_boss_node_t *g=tag_node(c,L(0x8e));
    if (!g) fail(c,L(0x8e));
    return g;
}
static void d6254(ctx_t *c) {
    uint32_t tag;nearest_player(c,&tag);CHECK;
    SL(0x8a,tag);
    neo_boss_node_t *p=creator(c);
    if (!p) {fail(c,0xd625e);return;}
    SW(0x2c,(uint16_t)S8(p->f[0xa5]));
    SW(0x1e,neo_boss_w(p,0x1e)+W(0x80));
    project(c);CHECK;
    anim_step(c);CHECK;
    neo_boss_node_t *g=gun_of(c);CHECK;
    if (g->f[0xa6]>=0x0d) {SB(0x61,1);N->routine=0xd62a2;}
    turret_tail(c);
}
static void d62a2(ctx_t *c) {SB(0x61,0);anim_start(c,0x2adac4);CHECK;GO(0xd62bc,d62bc);}
static void d62bc(ctx_t *c) {project(c);CHECK;if (anim_step(c)) N->routine=0xd62d6;CHECK;turret_tail(c);}
static void d62d6(ctx_t *c) {anim_start(c,0x2ada14);CHECK;SW(0x72,0x0a);GO(0xd62f2,d62f2);}
static void d62f2(ctx_t *c) {
    project(c);CHECK;anim_step(c);CHECK;
    SW(0x72,W(0x72)-1u);
    if (!W(0x72)) {
        neo_boss_node_t *g=gun_of(c);CHECK;
        if (g->f[0xa6]>=0x0d) {SB(0x61,1);N->routine=0xd632e;}
        else N->routine=0xd6362;
    }
    turret_tail(c);
}
static void d632e(ctx_t *c) {SB(0x61,0);anim_start(c,0x2adac4);CHECK;GO(0xd6348,d6348);}
static void d6348(ctx_t *c) {project(c);CHECK;if (anim_step(c)) N->routine=0xd6362;CHECK;turret_tail(c);}
static void d6362(ctx_t *c) {anim_start(c,0x2ada14);CHECK;SW(0x72,0x3c);GO(0xd637e,d637e);}
static void d637e(ctx_t *c) {
    project(c);CHECK;anim_step(c);CHECK;
    SW(0x72,W(0x72)-1u);
    if (!W(0x72)) N->routine=0xd624a;
    turret_tail(c);
}
static void d63ae(ctx_t *c) {anim_start(c,0x2adb1e);CHECK;GO(0xd63c4,d63c4);}
static void d63c4(ctx_t *c) {
    project(c);CHECK;
    /* 0D63D4: protection check, then delete (see 0D5F6C). */
    if (anim_step(c)) {CHECK;delete_self(c);}
}
/* 0D6464: turret mount (owns 0D61F2 through +8E). */
static void d64d8(ctx_t *c);
static void d6464(ctx_t *c) {
    place_on_creator(c,0,0);CHECK;
    SB(0x60,0);
    int16_t k;neo_boss_node_t *ch=spawn_part(c,0xd61f2,0x0e,0,0,&k);CHECK;
    if (k<0) FAIL(0xd6498);
    SL(0x8e,NEO_BOSS_TAG(k));
    set_w(ch,0x80,0xffff);
    SL(0x92,0xe9218);
    palette(c,0x2ed);CHECK;palette_flash(c,0x31a);CHECK;
    anim_start(c,0x2ace38);CHECK;
    GO(0xd64d8,d64d8);
}
static void d64d8(ctx_t *c) {
    follow_state(c);CHECK;
    share_flash(c);
    project(c);CHECK;
    anim_step(c);CHECK;
    if (!creator_alive(c)) N->routine=0xd5ffc;
}
/* 0D6500: missile pod (fires 0D6A84 through 0D726E). */
static void d6568(ctx_t *c);
static void d726e(ctx_t *c);
static void d6500(ctx_t *c) {
    place_on_creator(c,0,0);CHECK;
    SB(0x7d,0);
    SB(0x76,table(c,0x2cab9e));CHECK;
    SW(0x72,table(c,0x2cac00));CHECK;
    SL(0x92,0xe9234);
    palette(c,0x2ee);CHECK;palette_flash(c,0x31b);CHECK;
    anim_start(c,0x2acfa6);CHECK;
    GO(0xd6568,d6568);
}
static void d6568(ctx_t *c) {
    follow_state(c);CHECK;
    share_flash(c);
    project(c);CHECK;
    anim_step(c);CHECK;
    neo_boss_node_t *p=creator(c);
    if (!p) {fail(c,0xd657e);return;}
    if (!(p->f[0x5f]&0x40u)) {
        SW(0x72,W(0x72)-1u);
        if (S16(W(0x72))<=0) {
            d726e(c);CHECK;
            SW(0x72,table(c,0x2cac96));CHECK;
            SB(0x76,B(0x76)-1u);
            if (S8(B(0x76))<=0) {
                SB(0x76,table(c,0x2cab9e));CHECK;
                SW(0x72,table(c,0x2cac00));CHECK;
            }
        }
    }
    if (!creator_alive(c)) N->routine=0xd5ffc;
}
/* 0D726E: launch a missile when the body's phase is 5 or more. */
static void d726e(ctx_t *c) {
    if (B(0x7a)==5) return;
    SB(0xa2,0);
    neo_boss_node_t *p=creator(c);
    if (!p) {fail(c,0xd727c);return;}
    if (S8(p->f[0x79])<5) return;
    if (p->f[0x79]==2 || p->f[0x79]==3) return;
    if (p->f[0x79]==4) SB(0xa2,0);
    neo_boss_node_t *m=spawn_part(c,0xd6a84,0x0e,0,0,NULL);CHECK;
    m->f[0xa4]=B(0xa2);m->f[0x7d]=B(0x7d);
    SB(0x7d,B(0x7d)+1u);
}
/* 0D65EE: rear hatch (object platform 0E95A6), drops 0D6D30 and the
 * prisoners when destroyed. */
static void d6634(ctx_t *c);static void hatch_tail(ctx_t *c);static void d668e(ctx_t *c);static void d66b0(ctx_t *c);
static void d66dc(ctx_t *c);static void d6710(ctx_t *c);static void d672a(ctx_t *c);static void d6756(ctx_t *c);
static void d678e(ctx_t *c);static void d6792(ctx_t *c);static void d67dc(ctx_t *c);static void d67e6(ctx_t *c);
static void d7402(ctx_t *c);
static void d65ee(ctx_t *c) {
    SW(0x4e,table(c,0x2ca8e2));CHECK;
    SL(0x9a,0xe95a6);
    palette(c,0x2ec);CHECK;palette_flash(c,0x31a);CHECK;
    anim_start(c,0x2ad0e0);CHECK;
    GO(0xd6634,d6634);
}
static void d6634(ctx_t *c) {
    if (L(0x9a)) {platform(c,L(0x9a));CHECK;}
    project(c);CHECK;
    if (anim_step(c)) N->routine=0xd668e;
    CHECK;
    hatch_tail(c);
}
/* 0D665A */
static void hatch_tail(ctx_t *c) {
    if (hurtbox(c)) SB(0x5a,0);
    CHECK;
    if (B(0x5f)&0x80u) N->routine=0xd678e;
    neo_boss_node_t *p=creator(c);
    if (!p) {fail(c,0xd6678);return;}
    if (p->f[0x7a]==5) N->routine=0xd6792;
}
static void d668e(ctx_t *c) {
    spawn_part(c,0xd6d30,0x0e,0,0,NULL);CHECK;
    d66b0(c);
}
static void d66b0(ctx_t *c) {
    SB(0x60,0);
    SW(0x72,table(c,0x2caa7a));CHECK;
    anim_start(c,0x2ad0fa);CHECK;
    GO(0xd66dc,d66dc);
}
static void d66dc(ctx_t *c) {
    if (L(0x9a)) {platform(c,L(0x9a));CHECK;}
    project(c);CHECK;
    anim_step(c);CHECK;
    SW(0x72,W(0x72)-1u);
    if (S16(W(0x72))<=0) N->routine=0xd6710;
    hatch_tail(c);
}
static void d6710(ctx_t *c) {SB(0x61,0);SW(0x72,table(c,0x2cab2c));CHECK;GO(0xd672a,d672a);}
static void d672a(ctx_t *c) {
    SW(0x72,W(0x72)-1u);
    if (S16(W(0x72))<=0) {
        SB(0x60,1);
        anim_start(c,0x2ad106);CHECK;
        N->routine=0xd6756;
    }
    d6756(c);
}
static void d6756(ctx_t *c) {
    if (L(0x9a)) {platform(c,L(0x9a));CHECK;}
    project(c);CHECK;
    anim_step(c);CHECK;
    if (B(0x61)) N->routine=0xd66b0;
    neo_boss_node_t *p=creator(c);
    if (!p) {fail(c,0xd6780);return;}
    p->f[0xa1]=1;
    hatch_tail(c);
}
static void d678e(ctx_t *c) {d7402(c);CHECK;d6792(c);}
static void d6792(ctx_t *c) {
    platform_release(c);CHECK;
    SL(0x9a,0);
    neo_boss_node_t *p=creator(c);
    if (!p) {fail(c,0xd67a0);return;}
    p->f[0x79]=5;
    sound(c,0x102c);
    SL(0x50,0);
    anim_start(c,0x2ad134);CHECK;
    N->routine=0xd67dc;
    SB(0xa0,0xff);
    d67e6(c);
}
static void d67dc(ctx_t *c) {SB(0xa0,0);GO(0xd67e6,d67e6);}
static void d67e6(ctx_t *c) {
    share_flash(c);
    project(c);CHECK;
    anim_step(c);CHECK;
    if (!creator_alive(c)) N->routine=0xd5ffc;
}
/* 0D7402: three prisoners (generic 099DCC) behind the hatch. */
static void d7402(ctx_t *c) {
    static const int16_t dx[3]={-0x28,-0x08,0x18};
    static const uint16_t w6e[3]={2,0,0},w74[3]={0x1b,0,6};
    for (unsigned k=0;k<3;k++) {
        neo_boss_node_t *ch=effect_child(c,0x99dcc,0x1c,dx[k],0x20);CHECK;
        set_w(ch,0x6e,w6e[k]);set_w(ch,0x70,0);set_w(ch,0x72,1);set_w(ch,0x74,w74[k]);
        effect_commit(c,ch,0x99dcc);CHECK;
    }
}
/* 0D680A: main gun (fires through the program callback 0D739C). */
static void d687c(ctx_t *c);static void d6882(ctx_t *c);static void gun_tail(ctx_t *c);
static void d691e(ctx_t *c);static void d697a(ctx_t *c);
static void d680a(ctx_t *c) {
    place_on_creator(c,0,0);CHECK;
    SW(0x72,table(c,0x2cad58));CHECK;
    SB(0x76,table(c,0x2cae7e));CHECK;
    SB(0x7d,0);
    SB(0xa3,table(c,0x2caef0));CHECK;
    palette(c,0x2ed);CHECK;palette_flash(c,0x31a);CHECK;
    SL(0x92,0xe9250);
    anim_start(c,0x2ad15c);CHECK;
    d687c(c);
}
static void d687c(ctx_t *c) {GO(0xd6882,d6882);}
static void d6882(ctx_t *c) {
    follow_state(c);CHECK;gun_offset(c);CHECK;
    share_flash(c);
    neo_boss_node_t *p=creator(c);
    if (!p) {fail(c,0xd688c);return;}
    if (!(p->f[0x5f]&0x40u)) {
        uint8_t d0=p->f[0xa1];p->f[0xa1]=0;
        if (d0) {SB(0x7d,0);SB(0xa3,table(c,0x2caef0));CHECK;}
        else if (p->f[0x79]!=2 && p->f[0x79]!=3) {
            SW(0x72,W(0x72)-1u);
            if (S16(W(0x72))<=0) N->routine=0xd691e;
        }
    }
    anim_step(c);CHECK;
    gun_tail(c);
}
/* 0D68F8 */
static void gun_tail(ctx_t *c) {
    neo_boss_node_t *p=creator(c);
    if (!p) {fail(c,0xd68f8);return;}
    if (p->f[0x79]==6) N->routine=0xd5ffc;
    if (!creator_alive(c)) N->routine=0xd5ffc;
}
static void d691e(ctx_t *c) {
    neo_boss_node_t *p=creator(c);
    if (!p) {fail(c,0xd691e);return;}
    if (S8(p->f[0x79])<=2) SW(0x72,table(c,0x2cae1c)); else SW(0x72,table(c,0x2cadba));
    CHECK;
    anim_table(c,0xe926c,p->f[0x7a]&0x1fu);CHECK;
    GO(0xd697a,d697a);
}
static void d697a(ctx_t *c) {
    follow(c);CHECK;gun_offset(c);CHECK;
    share_flash(c);
    if (anim_step(c)) {
        CHECK;
        N->routine=0xd687c;
        SB(0x76,B(0x76)-1u);
        if (S8(B(0x76))<=0) {
            SB(0x7d,0);
            SB(0xa3,table(c,0x2caef0));CHECK;
            SW(0x72,table(c,0x2cad58));CHECK;
            SB(0x76,table(c,0x2cae7e));CHECK;
        }
    }
    CHECK;
    gun_tail(c);
}
/* 0D69DC: cover plate (program callback 0D72E4). */
static void d6a16(ctx_t *c);static void d6a3e(ctx_t *c);static void d6a54(ctx_t *c);
static void d69dc(ctx_t *c) {
    palette(c,0x2ec);CHECK;palette_flash(c,0x319);CHECK;
    SW(0x66,0xe0ed);SW(0x1e,0xe0ed);
    anim_start(c,0x2ad384);CHECK;
    GO(0xd6a16,d6a16);
}
static void d6a16(ctx_t *c) {
    project(c);CHECK;
    share_flash(c);
    anim_step(c);CHECK;
    neo_boss_node_t *p=creator(c);
    if (!p) {fail(c,0xd6a28);return;}
    if (p->f[0x7a]==5) N->routine=0xd6a3e;
}
static void d6a3e(ctx_t *c) {anim_start(c,0x2ad444);CHECK;GO(0xd6a54,d6a54);}
static void d6a54(ctx_t *c) {
    project(c);CHECK;
    anim_step(c);CHECK;
    if (!creator_alive(c)) N->routine=0xd5ffc;
}
/* 0D6A84: missile (path program at +A0, run by 021288). */
static void d6ac6(ctx_t *c);static void missile_tail(ctx_t *c);static void d6b10(ctx_t *c);static void d6bac(ctx_t *c);
static void missile_attack(ctx_t *c);static void d6c66(ctx_t *c);static void d6a72(ctx_t *c);
static void d6a84(ctx_t *c) {
    place_on_creator(c,0,0);CHECK;
    SB(0x15,B(0x15)|0x20u);
    palette(c,0x223);CHECK;
    SW(0x82,0x10);SW(0x80,0);
    anim_start(c,0x2ad456);CHECK;
    GO(0xd6ac6,d6ac6);
}
static void d6ac6(ctx_t *c) {
    pod_offset(c);CHECK;
    if (anim_step(c)) {CHECK;sound(c,0x10a4);N->routine=0xd6b10;}
    CHECK;
    missile_tail(c);
}
/* 0D6AE4 */
static void missile_tail(ctx_t *c) {
    neo_boss_node_t *p=creator(c);
    if (!p) {fail(c,0xd6ae4);return;}
    if (p->f[0x7a]==5) N->routine=0xd6c66;
    if (outside(c,0xe918c)) N->routine=0xd6004;
}
static void d6b10(ctx_t *c) {
    SW(0x84,0);
    SW(0x4e,table(c,0x2ca944));CHECK;
    SW(0xa8,0x400);
    SW(0x28,W(0x28)+0xffc8u);SW(0x2c,W(0x2c)+0x0fu);
    uint32_t rows=0xe92ea;
    if (!B(0xa4)) {
        neo_boss_node_t *p=creator(c),*g=p?node_at(c->w,p->creator):NULL;
        if (!g) {fail(c,0xd6b48);return;}
        if (g->f[0x79]==5) {SB(0x5e,B(0x5e)|0x80u);rows=0xe930a;}  /* 004C1C */
    }
    uint16_t d0=(uint16_t)(rom_word(c,rows+(B(0x7d)&15u)*2u)&15u);CHECK;
    SL(0xa0,rom_long(c,0xe92aa+d0*4u));CHECK;
    SW(0xa4,0);SB(0xa6,0);
    SW(0x66,0xe000);SW(0x1e,0xe000);
    anim_start(c,0x2ad4a8);CHECK;
    GO(0xd6bac,d6bac);
}
/* ROM 0214B4: move `d0` along heading `d1` (sine table 0029FC/002A7C). */
static void path_move(ctx_t *c,uint16_t d0,uint8_t d1) {
    int16_t s=S16(boss_sine[d1]),k=S16(boss_sine[d1+64u]);
    int32_t d2=(int32_t)S16(d0)*s,d3=(int32_t)S16(d0)*k;
    SL(0x30,(uint32_t)d3);SL(0x34,(uint32_t)d2);
    SL(0x28,L(0x28)+(uint32_t)d3);SL(0x2c,L(0x2c)+(uint32_t)d2);
}
/* ROM 021288: path interpreter. Returns carry (path finished). */
static bool path(ctx_t *c) {
    project(c);if (c->unsupported) return false;
    uint16_t d5=W(0xa8);
    for (unsigned guard=0;guard<256;guard++) {
        uint32_t a=L(0xa0);
        uint8_t op=rom_byte(c,a);if (c->unsupported) return false;
        if (op>=0x0b) {fail(c,0x212a4);return false;}
        switch (op) {
        case 0:SW(0xaa,(uint16_t)rom_byte(c,a+1)<<8);SL(0xa0,a+2);break;
        case 1:case 2: {
            if (!d5) return false;
            uint32_t d0=op==1?((uint32_t)rom_word(c,a+2)<<16):((uint32_t)rom_word(c,a+4)<<8);
            if (c->unsupported) return false;
            d0-=L(0xa4)&0xffffff00u;
            if (d0<0x1000000u && d5>=(uint16_t)(d0>>8)) {
                uint16_t step=(uint16_t)(d0>>8);
                d5=(uint16_t)(d5-step);
                SB(0xa6,0);SW(0xa4,0);
                path_move(c,step,B(0xaa));
                if (op==1) SL(0xa0,a+4);
                else {
                    SB(0xaa,B(0xaa)+rom_byte(c,a+1));
                    if (B(0xaa)==rom_byte(c,a+2)) SL(0xa0,a+6);
                    if (c->unsupported) return false;
                }
                break;
            }
            /* 021430: the whole speed is spent on this segment. */
            SL(0xa4,L(0xa4)+((uint32_t)d5<<8));
            path_move(c,d5,B(0xaa));
            return false;
        }
        case 3:
            SW(0xa4,W(0xa4)+1u);
            if (rom_word(c,a+2)>W(0xa4)) return false;
            SW(0xa4,0);SL(0xa0,a+4);
            return false;
        case 4:SL(0xa0,rom_long(c,a+2));break;
        case 5: {uint16_t o=rom_word(c,a+2);if (o>=0xb0) {fail(c,0x21396);return false;}SL(o,rom_long(c,a+4));SL(0xa0,a+8);break;}
        case 6: {uint16_t o=rom_word(c,a+4);if (o>=0xb0) {fail(c,0x21376);return false;}SW(o,rom_word(c,a+2));SL(0xa0,a+6);break;}
        case 7: {uint16_t o=rom_word(c,a+2);if (o>=0xb0) {fail(c,0x21356);return false;}SB(o,rom_byte(c,a+1));SL(0xa0,a+4);break;}
        case 8:fail(c,rom_long(c,a+2));return false;   /* path callback (0D6C0E homing) */
        case 9:sound(c,rom_word(c,a+2));SL(0xa0,a+4);break;
        case 10:path_move(c,d5,B(0xaa));return true;
        }
        if (c->unsupported) return false;
    }
    fail(c,0x21296);return false;
}
static void d6bac(ctx_t *c) {
    if (path(c)) N->routine=0xd6c66;
    CHECK;
    SW(0x82,(B(0xaa)&0xffu)>>3);
    anim_step(c);CHECK;
    missile_attack(c);
}
/* 0D6BD0 */
static void missile_attack(ctx_t *c) {
    uint16_t hit=0;
    if (L(0x96)) {hit=attack(c,L(0x96));CHECK;}
    if (hit) N->routine=0xd6a72;
    if (hurtbox(c)) SB(0x5a,0);
    CHECK;
    if (B(0x5f)&0x80u) N->routine=0xd6c66;
    missile_tail(c);
}
static void d6c66(ctx_t *c) {sound(c,0x1022);d6a72(c);}
static void d6a72(ctx_t *c) {SW(0x66,0x1001);SW(0x1e,0x1001);convert(c,0x1c706);}
/* 0D6D30: flame vent under the hatch. */
static void d6d6a(ctx_t *c);static void vent_tail(ctx_t *c);static void d6dc0(ctx_t *c);static void d6dd6(ctx_t *c);
static void d6dee(ctx_t *c);static void d6e26(ctx_t *c);static void d6e5e(ctx_t *c);static void d6e86(ctx_t *c);
static void d6ea0(ctx_t *c);static void d6ebe(ctx_t *c);static void d6d4c(ctx_t *c);
static void d6d30(ctx_t *c) {
    place_on_creator(c,-0x30,-8);CHECK;
    palette(c,0x2ef);CHECK;
    d6d4c(c);
}
static void d6d4c(ctx_t *c) {SL(0x96,0xe917c);anim_start(c,0x2ad6c6);CHECK;GO(0xd6d6a,d6d6a);}
static void d6d6a(ctx_t *c) {
    project(c);CHECK;
    anim_step(c);CHECK;
    neo_boss_node_t *p=creator(c);
    if (!p) {fail(c,0xd6d76);return;}
    if (p->f[0x60]==1) N->routine=0xd6dc0;
    vent_tail(c);
}
/* 0D6D8A */
static void vent_tail(ctx_t *c) {
    if (L(0x96)) {attack(c,L(0x96));CHECK;}
    neo_boss_node_t *p=creator(c);
    if (!p) {fail(c,0xd6d9a);return;}
    if (p->routine==0xd67dc) N->routine=0xd6ea0;
    if (!creator_alive(c)) N->routine=0xd6ea0;
}
static void d6dc0(ctx_t *c) {anim_start(c,0x2ad758);CHECK;GO(0xd6dd6,d6dd6);}
static void d6dd6(ctx_t *c) {project(c);CHECK;if (anim_step(c)) N->routine=0xd6dee;CHECK;vent_tail(c);}
static void d6dee(ctx_t *c) {
    sound(c,0x10c6);
    SL(0x96,0xe916c);
    SB(0x76,table(c,0x2caa08));CHECK;
    anim_start(c,0x2ad80e);CHECK;
    GO(0xd6e26,d6e26);
}
static void d6e26(ctx_t *c) {
    project(c);CHECK;
    if (anim_step(c)) {
        CHECK;anim_start(c,0x2ad85e);CHECK;
        SB(0x76,B(0x76)-1u);
        if (S8(B(0x76))<=0) N->routine=0xd6e5e;
    }
    CHECK;
    vent_tail(c);
}
static void d6e5e(ctx_t *c) {
    neo_boss_node_t *p=creator(c);
    if (!p) {fail(c,0xd6e5e);return;}
    p->f[0x61]=0xff;
    SL(0x96,0);
    anim_start(c,0x2ad900);CHECK;
    GO(0xd6e86,d6e86);
}
static void d6e86(ctx_t *c) {project(c);CHECK;if (anim_step(c)) N->routine=0xd6d4c;CHECK;vent_tail(c);}
static void d6ea0(ctx_t *c) {SL(0x96,0);anim_start(c,0x2ad900);CHECK;GO(0xd6ebe,d6ebe);}
static void d6ebe(ctx_t *c) {project(c);CHECK;if (anim_step(c)) N->routine=0xd6004;}
/* 0D6FFC: muzzle flash. */
static void d7020(ctx_t *c);
static void d6ffc(ctx_t *c) {palette(c,0x393);CHECK;anim_start(c,0x2ad968);CHECK;GO(0xd7020,d7020);}
static void d7020(ctx_t *c) {
    project(c);CHECK;
    if (anim_step(c)) N->routine=0xd6004;
    CHECK;
    if (outside(c,0xe9194)) N->routine=0xd6004;
}
/* 0D6F90: shell, continued by the generic projectile code 02A260. */
static void p2a260(ctx_t *c,uint32_t gravity);static void p2a2ac(ctx_t *c);static void p2a2fe(ctx_t *c);
static void p2a244(ctx_t *c);
static void d6f90(ctx_t *c) {
    SW(0x66,0xe0ef);SW(0x1e,0xe0ef);
    neo_boss_node_t *p=creator(c),*g=p?node_at(c->w,p->creator):NULL;
    if (!p || !g) {fail(c,0xd6f9c);return;}
    unsigned d0=p->f[0xa3];
    uint32_t t=0x2caf92;
    if (S8(g->f[0x79])>3) t=0x2cafa2;
    uint32_t row=rom_long(c,t+d0*4u);CHECK;
    unsigned k=(B(0x7d)&7u)*4u;
    SL(0x30,speed8(rom_word(c,row+k)));
    SL(0x34,0x10000);
    uint32_t gravity=speed8(rom_word(c,row+k+2));CHECK;
    p2a260(c,gravity);
}
/* ROM 02A5EA: the shell's three palettes. */
static void p2a5ea(ctx_t *c) {
    palette(c,0x357);CHECK;palette(c,0x358);CHECK;palette(c,0x359);CHECK;
    SB(0x14,B(0x19));
}
/* ROM 02A50A: aim at a player in rectangle 02A7DA (or the nearest one);
 * speed row by distance from tables 2CB0B2/2CB114, random X spread. */
static void p2a50a(ctx_t *c) {
    uint32_t tag;
    if (player_in_rect(c,0x2a7da,&tag)) {CHECK;nearest_player(c,&tag);}
    CHECK;
    int16_t px,py;if (!player_xy(c,tag,&px,&py)) return;
    uint16_t d0=(uint16_t)(S16(W(0x3c))-px);
    if (!(S16(d0)>0x20)) d0=0x20;
    d0=(uint16_t)((uint16_t)(d0-0x20)>>4);
    if (!(d0<0x0f)) d0=0x0f;
    uint32_t off=(uint32_t)d0*12u;
    uint16_t sel=(uint16_t)(table(c,0x2cb0b2)&3u);CHECK;
    uint32_t row=rom_long(c,0x2cb114+sel*4u)+off;CHECK;
    SL(0x30,rom_long(c,row+8));SL(0x7a,rom_long(c,row+4));SL(0x34,rom_long(c,row));CHECK;
    uint16_t r=random16(c);CHECK;
    SL(0x30,L(0x30)+(uint32_t)((int32_t)r-0x8000));
    if (B(0x15)&1u) SL(0x30,0u-L(0x30));
}
static void p2a260(ctx_t *c,uint32_t gravity) {
    SL(0x7a,gravity);
    SB(0x86,0);SB(0x86,B(0x86)|0x40u);SB(0x86,B(0x86)|0x10u);
    p2a50a(c);CHECK;
    SB(0x15,B(0x15)|0x20u);
    p2a5ea(c);CHECK;
    SW(0x66,0xe0f0);SW(0x1e,0xe0f0);
    anim_start(c,0x236c74);CHECK;
    GO(0x2a2ac,p2a2ac);
}
static void p2a2ac(ctx_t *c) {
    SL(0x34,L(0x34)+L(0x7a));
    air(c);CHECK;
    if (B(0x15)&0x80u) N->routine=0x2a2fe;
    anim_step(c);CHECK;
    if (attack(c,0x2a794)) N->routine=0x2a244;
    CHECK;
    if (outside(c,0x2a7ea)) N->routine=0x2a18a;
}
static void p2a2fe(ctx_t *c) {
    sound(c,(B(0x86)&0x10u)?0x1022:0x1021);
    if (B(0x86)&0x40u) {p2a244(c);return;}
    FAIL(0x2a32a);   /* ground burst 02A32A..02A3B2: not recovered */
}
static void p2a244(ctx_t *c) {
    palette_release(c);
    SW(0x66,0x1001);SW(0x1e,0x1001);SL(0x50,0);
    convert(c,0x1c71e);
}
/* ROM 02A680 (program callback): trail sprite 02A3B4 behind the shell. The
 * allocation result is not tested: on failure the writes go to the scratch. */
static void p2a680(ctx_t *c) {
    int16_t i;neo_boss_node_t *ch=allocate(c,0x2a3b4,0x2a,&i);
    if (c->unsupported) return;
    ch->f[0x86]=B(0x86);
    place_root(c,ch,0,0);
}
/* ROM 02A3B4: trail sprite, follows its shell's palette and depth. */
static void p2a3ce(ctx_t *c);
static void p2a3b4(ctx_t *c) {p2a5ea(c);CHECK;anim_start(c,0x236c9a);CHECK;GO(0x2a3ce,p2a3ce);}
static void p2a3ce(ctx_t *c) {
    project(c);CHECK;
    neo_boss_node_t *p=creator(c);
    if (!p) {fail(c,0x2a3d4);return;}
    SB(0x14,p->f[0x14]);
    SW(0x1e,neo_boss_w(p,0x1e)-1u);
    if (anim_step(c)) N->routine=0x2a18a;
}
/* ROM 02A6A2 (program callback): cycle the shell palette. */
static void p2a6a2(ctx_t *c) {
    unsigned d0=(B(0x85)+1u)&3u;
    if (!(d0<3)) d0=0;
    SB(0x85,d0);SB(0x14,B(0x19+d0));
}
/* Program callbacks of the body/gun programs. */
static void d72e4(ctx_t *c) {spawn_part(c,0xd69dc,0x0e,0,0,NULL);}
static void d7308(ctx_t *c) {neo_boss_node_t *ch=spawn_part(c,0xd65ee,0x0e,0,0,NULL);CHECK;set_w(ch,0x80,0);}
static void d7330(ctx_t *c) {neo_boss_node_t *ch=effect_child(c,0x1c706,0x1c,0,0);CHECK;effect_commit(c,ch,0x1c706);}
static void d7354(ctx_t *c) {
    sound(c,0x1021);
    neo_boss_node_t *ch=effect_child(c,0x1c71e,0x1c,0,0);CHECK;effect_commit(c,ch,0x1c71e);CHECK;
    debris(c,0xe9360);
}
static void d738e(ctx_t *c) {debris(c,0xe934e);}
static void d739c(ctx_t *c) {
    sound(c,0x10ea);
    spawn_part(c,0xd6ffc,0x1c,0,0,NULL);CHECK;
    neo_boss_node_t *s=spawn_part(c,0xd6f90,0x1c,-0x1c,-4,NULL);CHECK;
    s->f[0x7d]=B(0x7d);
    SB(0x7d,(B(0x7d)+1u)&7u);
}

/* ---- dispatch (004246) ---------------------------------------------------------------- */
typedef void (*routine_fn)(ctx_t *c);
typedef struct { uint32_t label; routine_fn fn; } entry_t;
static void p2a18a(ctx_t *c) {delete_tree(c);}
static void shell_ctor(ctx_t *c) {d6f90(c);}
static const entry_t routines[]={
    {0xd5976,d5976},{0xd5a92,d5a92},{0xd5ad4,d5ad4},{0xd5b1e,d5b1e},{0xd5b7e,d5b7e},{0xd5bba,d5bba},{0xd5bf8,d5bf8},
    {0xd5c2c,d5c2c},{0xd5c90,d5c90},{0xd5cd2,d5cd2},{0xd5cec,d5cec},{0xd5d04,d5d04},{0xd5d24,d5d24},{0xd5d5c,d5d5c},
    {0xd5d84,d5d84},{0xd5e5c,d5e5c},{0xd5e88,d5e88},{0xd5ea6,d5ea6},{0xd5ebc,d5ebc},{0xd5eda,d5eda},{0xd5ffc,d5ffc},
    {0xd6004,d6004},{0xd600c,d600c},{0xd605e,d605e},{0xd60b4,d60b4},{0xd6130,d6130},{0xd6168,d6168},{0xd61f2,d61f2},
    {0xd624a,d624a},{0xd6254,d6254},{0xd62a2,d62a2},{0xd62bc,d62bc},{0xd62d6,d62d6},{0xd62f2,d62f2},{0xd632e,d632e},
    {0xd6348,d6348},{0xd6362,d6362},{0xd637e,d637e},{0xd63ae,d63ae},{0xd63c4,d63c4},{0xd6464,d6464},{0xd64d8,d64d8},
    {0xd6500,d6500},{0xd6568,d6568},{0xd65ee,d65ee},{0xd6634,d6634},{0xd668e,d668e},{0xd66b0,d66b0},{0xd66dc,d66dc},
    {0xd6710,d6710},{0xd672a,d672a},{0xd6756,d6756},{0xd678e,d678e},{0xd6792,d6792},{0xd67dc,d67dc},{0xd67e6,d67e6},
    {0xd680a,d680a},{0xd687c,d687c},{0xd6882,d6882},{0xd691e,d691e},{0xd697a,d697a},{0xd69dc,d69dc},{0xd6a16,d6a16},
    {0xd6a3e,d6a3e},{0xd6a54,d6a54},{0xd6a72,d6a72},{0xd6a84,d6a84},{0xd6ac6,d6ac6},{0xd6b10,d6b10},{0xd6bac,d6bac},
    {0xd6c66,d6c66},{0xd6d30,d6d30},{0xd6d4c,d6d4c},{0xd6d6a,d6d6a},{0xd6dc0,d6dc0},{0xd6dd6,d6dd6},{0xd6dee,d6dee},
    {0xd6e26,d6e26},{0xd6e5e,d6e5e},{0xd6e86,d6e86},{0xd6ea0,d6ea0},{0xd6ebe,d6ebe},{0xd6f90,shell_ctor},
    {0xd6ffc,d6ffc},{0xd7020,d7020},
    {0x2a18a,p2a18a},{0x2a244,p2a244},{0x2a2ac,p2a2ac},{0x2a2fe,p2a2fe},{0x2a3b4,p2a3b4},{0x2a3ce,p2a3ce},
};
neo_boss_result_t neo_boss_step(neo_boss_t *w,int16_t i) {
    neo_boss_node_t *n=node_at(w,i);
    if (!n) return NEO_BOSS_UNSUPPORTED;
    if (n->routine==DEAD) return NEO_BOSS_MORE;      /* 003F1E */
    ctx_t ctx={w,n,i,false,false,false},*c=&ctx;
    w->born_count=0;
    /* Routine lookup through a small direct-mapped cache of the table
     * (same result as the linear search; the table is constant). */
    static struct { uint32_t label; routine_fn fn; } cache[64];
    unsigned h=(n->routine>>1)&63u;
    routine_fn fn=NULL;
    if (cache[h].label==n->routine && cache[h].fn) fn=cache[h].fn;
    else {
        for (unsigned k=0;k<sizeof(routines)/sizeof(routines[0]);k++) if (routines[k].label==n->routine) {fn=routines[k].fn;break;}
        if (fn) {cache[h].label=n->routine;cache[h].fn=fn;}
    }
    if (!fn) {w->unsupported_label=n->routine;return NEO_BOSS_UNSUPPORTED;}
    fn(c);
    if (c->unsupported) return NEO_BOSS_UNSUPPORTED;
    if (c->handed_off) return NEO_BOSS_HANDED_OFF;
    return c->deleted?NEO_BOSS_DELETED:NEO_BOSS_MORE;
}
neo_boss_result_t neo_boss_tick(neo_boss_t *w) {
    int16_t i=w->first;
    while (i>=0) {
        /* 00424A reads +8 after the update: a deleted node's successor is
         * its successor at the time of the delete. */
        int16_t next=w->nodes[i].next;
        neo_boss_result_t r=neo_boss_step(w,i);
        if (r==NEO_BOSS_UNSUPPORTED) return r;
        if (w->nodes[i].routine) next=w->nodes[i].next;
        i=next;
    }
    return NEO_BOSS_MORE;
}

/* ---- damage intake (004892..0049E0) ---------------------------------------------------- */
bool neo_boss_receive(neo_boss_t *w,int16_t i,const neo_boss_hit_t *h) {
    neo_boss_node_t *n=node_at(w,i);
    if (!n || n->routine==DEAD || !neo_boss_l(n,0x50)) return false;
    if (h->kind>n->f[0x5a]) {
        n->f[0x5a]=h->kind;n->f[0x17]|=6u;n->f[0x5b]=0;
        if (S16((uint16_t)(neo_boss_w(n,0x3c)-(uint16_t)((uint32_t)h->cx>>16)))<0) n->f[0x5b]|=0x80u;
        if (S16((uint16_t)(neo_boss_w(n,0x40)-(uint16_t)((uint32_t)h->cy>>16)))<0) n->f[0x5b]|=0x40u;
        uint32_t dx=(uint32_t)h->px-(uint32_t)h->cx-neo_boss_l(n,0x3c)+neo_boss_l(n,0x44);
        uint32_t dy=(uint32_t)h->py-(uint32_t)h->cy-neo_boss_l(n,0x40)+neo_boss_l(n,0x48);
        n->f[0x5b]|=(uint8_t)((neo_angle(S16((uint16_t)(dx>>16)),S16((uint16_t)(dy>>16)))>>4)&15u);
        n->f[0x5c]=(uint8_t)ASRW(h->impact_x,1);n->f[0x5d]=(uint8_t)ASRW(h->impact_y,1);
    }
    if (!(n->f[0x5f]&0xc0u)) {
        uint16_t hp=neo_boss_w(n,0x4e),d0=h->damage;
        if (h->scale_row) d0=(uint16_t)(((uint32_t)h->scale*h->damage)>>8);
        if (d0) {
            if (hp>d0) hp=(uint16_t)(hp-d0);
            else {n->f[0x5f]|=0x80u;hp=0;}
            set_w(n,0x4e,hp);
            /* 0049CC: the attacker's +5E bits 0..1 go to bits 2..3 (the
             * score call 0578FE before it is the attacker's). */
            n->f[0x5e]=(uint8_t)(((h->attacker5e&3u)<<2)|(n->f[0x5e]&0xf3u));
        }
    }
    return true;
}
uint32_t neo_boss_anim_address(const neo_boss_node_t *n) {
    if (!n->anim.program) return neo_boss_l(n,0x20);
    for (unsigned i=0;i<BOSS_PROGRAM_COUNT;i++) if (&boss_programs[i].program==n->anim.program)
        if (n->anim.cursor<n->anim.program->count) {
            /* a negative entry is an explicit fall-through JUMP into that address */
            int32_t a=boss_programs[i].address[n->anim.cursor];
            return (uint32_t)(a<0?-a:a);
        }
    return 0;
}
uint16_t neo_boss_health(const neo_boss_t *w) {
    for (int16_t i=w->first;i>=0;i=w->nodes[i].next) if (w->nodes[i].routine && w->nodes[i].creator<0) return neo_boss_w(&w->nodes[i],0x4e);
    return 0;
}
const uint8_t *neo_boss_rom(uint32_t address,unsigned bytes) {return rom_at(address,bytes);}
void neo_boss_stop(neo_boss_t *w,int16_t i) {
    neo_boss_node_t *n=node_at(w,i);
    if (!n || n->routine==DEAD) return;
    const neo_boss_services_t *s=&w->services;
    if (s->palette_release && n->f[0x1d]) s->palette_release(s->context,&n->f[0x19],n->f[0x1d]);
    if (n->f[0x6a]!=0xff && s->platform_release) {s->platform_release(s->context,i,n->f[0x6a]);n->f[0x6a]=0xff;}
    kill(w,i);
}
uint8_t neo_boss_phase(const neo_boss_t *w) {
    for (int16_t i=w->first;i>=0;i=w->nodes[i].next) if (w->nodes[i].routine && w->nodes[i].creator<0) return w->nodes[i].f[0x79];
    return 0;
}
