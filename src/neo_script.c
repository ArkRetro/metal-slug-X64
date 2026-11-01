#include "neo_script.h"
#include <string.h>
#include "soldier_ai_tables.h"
/* Native translation of the level-script walkers. See neo_script.h. */
typedef struct { neo_script_t *s; const neo_script_env_t *e; int creator; } vm_t;
/* Script actor origin set by ROM 019E2C/019E34 (+28 = 0, +2C word = 0x100,
 * no parent); runners copy it every update (01A448). */
enum { ORIGIN_X=0, ORIGIN_Y=0x01000000 };
static uint8_t op(const vm_t *v,uint16_t pc) { return pc<v->s->size?v->s->code[pc]:0xff; }
static uint8_t len(const vm_t *v,uint16_t pc) { return pc+1u<v->s->size?v->s->code[pc+1]:0; }
static uint16_t word(const vm_t *v,uint16_t pc,unsigned off) {
    return pc+off+1u<v->s->size?(uint16_t)(v->s->code[pc+off]<<8|v->s->code[pc+off+1]):0;
}
static uint16_t halt(vm_t *v,uint16_t pc) {
    if (!v->s->halted) {v->s->halted=true;v->s->halt_address=v->s->base+pc;}
    return pc;
}
static bool ok(const vm_t *v) { return !v->s->halted; }
/* ROM 01A074: statements that run without blocking (ops 6..11). */
static bool nonblocking(uint8_t o) { return o>=6u && o<0x12u; }
static uint16_t next(vm_t *v,uint16_t pc) {
    if (!len(v,pc)) return halt(v,pc);
    return (uint16_t)(pc+len(v,pc));
}
/* ROM 01A372: skip one statement (with its controlled statement). */
static uint16_t skip(vm_t *v,uint16_t pc) {
    uint8_t o=op(v,pc);
    if (!ok(v) || o>=0x12u || o<6u) return halt(v,pc);
    switch (o) {
    case 8:case 0xb:case 0x11: pc=next(v,pc);return skip(v,pc);
    case 9: case 0xa: {
        uint8_t end=o==9?5:4;pc=next(v,pc);
        while (ok(v) && op(v,pc)!=end) pc=skip(v,pc);
        return ok(v)?next(v,pc):pc;
    }
    default: return next(v,pc);
    }
}
/* ROM 01A5CA: condition record [type][comparison][value.w]; true = carry. */
static bool condition(vm_t *v,uint16_t at) {
    uint8_t type=op(v,at),cmp=op(v,(uint16_t)(at+1));
    unsigned count=0;
    if (type<9u) count=v->s->groups[type];
    else if (type==9u) for (unsigned g=0;g<8u;g++) count=(count+v->s->groups[g])&0xffffu;
    else if (type==0xau) return true;
    else if (type>=0xbu && type<=0x13u) {
        /* ROM 01903A with RAM 10C322 set (observed in every capture): start
         * from the 69B8/69BA point, take each live player's +3C/+40 when its
         * +7A/+7B is set, copy the other player's point when one is absent.
         * D7.w: FFxx = P1 alive, xxFF = P2 alive. */
        const neo_script_player_t *p=v->e->players;
        uint16_t d0=v->s->w69b8,d1=v->s->w69ba,d2=d0,d3=d1,d7=0;
        if (p[0].alive) {d7=0xff00u;if (p[0].follow_x) d0=p[0].px;if (p[0].follow_y) d1=p[0].py;}
        if (p[1].alive) {d7|=0xffu;if (p[1].follow_x) d2=p[1].px;if (p[1].follow_y) d3=p[1].py;}
        if (!p[1].alive) {d2=d0;d3=d1;}
        if (!p[0].alive) {d0=d2;d1=d3;}
        uint16_t ax=(uint16_t)(((uint32_t)d0+d2)>>1),ay=(uint16_t)(((uint32_t)d1+d3)>>1); /* ADD + ROXR */
        bool lx=((uint16_t)(ax-v->s->w4330))&0x8000u,ly=((uint16_t)(ay-v->s->w4332))&0x8000u;
        bool s0=!(((uint16_t)(d0-v->s->w4330))&0x8000u),s2=!(((uint16_t)(d2-v->s->w4330))&0x8000u);
        bool t1=!(((uint16_t)(d1-v->s->w4332))&0x8000u),t3=!(((uint16_t)(d3-v->s->w4332))&0x8000u);
        switch (type) {
        case 0xb: return d7 && lx;          /* 01A69E */
        case 0xc: return d7 && !lx;         /* 01A6BE */
        case 0xd: return d7 && s0!=s2;      /* 01A6DE */
        case 0xe: return d7 && !ly;         /* 01A702 */
        case 0xf: return d7 && ly;          /* 01A722 */
        case 0x10: return d7 && t1!=t3;     /* 01A742 */
        case 0x11: return d7==0;            /* 01A766 */
        case 0x12: return d7 && d7!=0xffffu;/* 01A778 */
        default: return d7==0xffffu;        /* 01A790 */
        }
    }
    else {halt(v,at);return false;}
    uint16_t value=word(v,at,2);
    switch (cmp) {
    case 0: return count==value;
    case 1: return count>value;
    case 2: return count<value;
    case 3: return count!=value;
    default: halt(v,at);return false;
    }
}
static uint32_t spawn_entry(uint16_t offset,bool *found) {
    for (unsigned i=0;i<sizeof(mission1_spawn_table)/sizeof(mission1_spawn_table[0]);i++)
        if (mission1_spawn_table[i][0]==offset) {*found=true;return mission1_spawn_table[i][1];}
    *found=false;return 0;
}
static void mark_spawned(vm_t *v) {
    if (v->creator<0) v->s->spawned74=true; else v->s->runners[v->creator].spawned74=true;
}
static uint16_t run(vm_t *v,uint16_t pc);
/* ROM 01A0A6: execute one non-blocking statement. */
static uint16_t exec(vm_t *v,uint16_t pc) {
    uint8_t o=op(v,pc);
    if (!ok(v)) return pc;
    switch (o) {
    case 6: return next(v,pc);
    case 7: { /* 01A10A */
        bool found;uint32_t entry=spawn_entry(word(v,pc,4),&found);
        if (!found || len(v,pc)<0xau || (len(v,pc)-0xau)/2u>NEO_SCRIPT_SPAWN_WORDS) return halt(v,pc);
        if (!(entry&0x80000000u) && op(v,(uint16_t)(pc+2))<9u && v->e->native && !v->e->native(entry)) {
            if (!v->e->skip_unported) return halt(v,pc);
            v->s->skipped++;return next(v,pc);
        }
        mark_spawned(v);
        neo_script_spawn_t sp;memset(&sp,0,sizeof(sp));
        sp.record=v->s->base+pc;sp.constructor=entry;sp.creator=v->creator;
        uint8_t group=op(v,(uint16_t)(pc+2)),flags=op(v,(uint16_t)(pc+3));
        sp.group69=0xff;
        if (!(entry&0x80000000u) && group<9u) {sp.group69=group;v->s->groups[group]++;}
        sp.flags=flags;sp.variant68=flags&7u;
        sp.word_count=(uint8_t)((len(v,pc)-0xau)/2u);
        for (unsigned i=0;i<sp.word_count;i++) sp.words[i]=word(v,pc,0xa+2*i);
        uint32_t x=(uint32_t)word(v,pc,6)<<16,y=(uint32_t)word(v,pc,8)<<16;
        if (flags&0x80u) x=x+(uint32_t)ORIGIN_X-(uint32_t)v->e->scroll_x;
        if (flags&0x40u) y=y+(uint32_t)ORIGIN_Y-(uint32_t)v->e->scroll_y;
        sp.x=(int32_t)x;sp.y=(int32_t)y;
        if (v->e->spawn) v->e->spawn(v->e->context,&sp);
        return next(v,pc);
    }
    case 8: { /* 01A1E8 + runner constructor 01A422 */
        unsigned i;
        for (i=0;i<NEO_SCRIPT_RUNNERS && v->s->runners[i].active;i++) {}
        if (i==NEO_SCRIPT_RUNNERS) return halt(v,pc);
        neo_script_runner_t *r=&v->s->runners[i];
        r->active=true;r->record=pc;r->timer72=word(v,pc,2);r->count76=word(v,pc,8);r->spawned74=false;r->group69=0xff;
        if (v->e->runner) v->e->runner(v->e->context,v->creator,i);
        return skip(v,next(v,pc));
    }
    case 0x11: { /* 01A210: a runner (01A422) of the op 8 record at +4, its
                  * +69 = the word at +2 when < 9 (10C326 counted now) */
        unsigned i;
        for (i=0;i<NEO_SCRIPT_RUNNERS && v->s->runners[i].active;i++) {}
        if (i==NEO_SCRIPT_RUNNERS || op(v,(uint16_t)(pc+4))!=8u) return halt(v,pc);
        neo_script_runner_t *r=&v->s->runners[i];uint16_t rec=(uint16_t)(pc+4),group=word(v,pc,2);
        r->active=true;r->record=rec;r->timer72=word(v,rec,2);r->count76=word(v,rec,8);r->spawned74=false;
        r->group69=group<9u?(uint8_t)group:0xffu;
        if (group<9u) v->s->groups[group]++;
        if (v->e->runner) v->e->runner(v->e->context,v->creator,i);
        return skip(v,next(v,pc));
    }
    case 9: { /* 01A254 */
        pc=run(v,next(v,pc));
        if (!ok(v)) return pc;
        if (op(v,pc)!=5u) return halt(v,pc);
        return next(v,pc);
    }
    case 0xa: { /* 01A272 */
        uint16_t alt[16];unsigned n=0;
        pc=next(v,pc);
        while (ok(v) && op(v,pc)!=4u) {
            if (!nonblocking(op(v,pc)) || n==16u) return halt(v,pc);
            alt[n++]=pc;pc=skip(v,pc);
        }
        if (!ok(v)) return pc;
        if (n) {
            /* 0029F2: (random * n) >> 16; the stack holds the alternatives
             * in reverse push order, so index 0 is the last one. */
            uint16_t r=v->e->rng(v->e->context);
            unsigned k=(unsigned)(((uint32_t)r*n)>>16);
            exec(v,alt[n-1u-k]);
        }
        return ok(v)?next(v,pc):pc;
    }
    case 0xb: { /* 01A2D8 */
        bool c=condition(v,(uint16_t)(pc+2));
        uint16_t n=next(v,pc);
        if (!ok(v)) return pc;
        return c?exec(v,n):skip(v,n);
    }
    case 0xf: { /* 01A300 */
        uint16_t a=word(v,pc,2),b=word(v,pc,4);
        if (a!=0x8000u) v->s->w69b8=a;
        if (b!=0x8000u) v->s->w69ba=(uint16_t)(b+0x100u);
        return next(v,pc);
    }
    case 0xc: /* 01A2F0 -> 0C7C12: release the camera lock */
        if (v->e->camera_release) v->e->camera_release(v->e->context);
        return next(v,pc);
    case 0xd: case 0xe: /* 01A34A/01A35E -> 0C7B4E/0C7BBA: camera autoscroll */
        if (v->e->autoscroll) v->e->autoscroll(v->e->context,o==0xd);
        return next(v,pc);
    case 0x10: /* 01A32E */
        v->s->w4330=word(v,pc,2);v->s->w4332=(uint16_t)(word(v,pc,4)+0x100u);
        return next(v,pc);
    default: /* 0..5 trap */
        return halt(v,pc);
    }
}
/* ROM 01A056: run non-blocking statements up to the next blocking one. */
static uint16_t run(vm_t *v,uint16_t pc) {
    while (ok(v)) {
        uint8_t o=op(v,pc);
        if (o>=0x12u) return halt(v,pc);
        if (!nonblocking(o)) return pc;
        pc=exec(v,pc);
    }
    return pc;
}
/* ROM 019F7E: one blocking statement; returns the (possibly) new position. */
static uint16_t blocking(vm_t *v,uint16_t pc) {
    uint8_t o=op(v,pc);
    switch (o) {
    case 0:
        if (word(v,pc,4)!=0xffffu) {
            uint16_t p[4]={word(v,pc,6),word(v,pc,8),word(v,pc,10),word(v,pc,12)};
            if (v->e->camera_lock) v->e->camera_lock(v->e->context,word(v,pc,4),p);
        }
        if (v->e->camera>=word(v,pc,2)) return run(v,next(v,pc));
        return pc;
    case 1:
        if (!v->s->timer72) {
            v->s->timer72=word(v,pc,2);
            if (!v->s->timer72) return run(v,next(v,pc));
            return pc;
        }
        if (!--v->s->timer72) return run(v,next(v,pc));
        return pc;
    case 2:
        if (condition(v,(uint16_t)(pc+2)) && ok(v)) return run(v,next(v,pc));
        return pc;
    case 3:
        v->s->finished=true;return pc;
    default:
        return halt(v,pc);
    }
}
void neo_script_init(neo_script_t *s,const uint8_t *code,uint16_t size,uint32_t base) {
    memset(s,0,sizeof(*s));s->code=code;s->size=size;s->base=base;
}
void neo_script_tick(neo_script_t *s,const neo_script_env_t *e) {
    vm_t v={s,e,-1};
    while (!s->finished && ok(&v)) {
        uint16_t pc=blocking(&v,s->pc);
        if (pc==s->pc) break;
        s->pc=pc;
    }
}
bool neo_script_tick_runner(neo_script_t *s,unsigned index,const neo_script_env_t *e) {
    if (index>=NEO_SCRIPT_RUNNERS || !s->runners[index].active) return false;
    neo_script_runner_t *r=&s->runners[index];
    vm_t v={s,e,(int)index};
    uint16_t rec=r->record;
    /* 01A454: active while the camera has not passed the record's limit. */
    if (!(word(&v,rec,6)>=e->camera) || !condition(&v,(uint16_t)(rec+0xa))) {r->active=false;return false;}
    if (r->timer72) {r->timer72--;return true;}
    r->timer72=word(&v,rec,4);
    r->spawned74=false;
    exec(&v,next(&v,rec));
    if (r->spawned74 && !--r->count76) {r->active=false;return false;}
    return true;
}
bool neo_script_reached(const neo_script_t *s,uint32_t address) {
    return s->base+s->pc>=address;
}
