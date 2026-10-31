/* Closed-loop test of the vehicle-slot services (src/neo_vslot.c, ROM
 * 0AA458..0AABF2) against every call the original made in the weapons
 * family traces camelfam (Camel Slug 03458E, its turret, player 1) and
 * tankfam (SV-001 092A32, its turret 101784, player 1):
 * tools/extract_vslot_fixture.py -> data/vslot_fixture.json.gz ->
 * tools/compile_vslot_fixture.py -> build/vslot_fixture.h.
 *
 * The slots (RAM 10C608..10C64F) and the records 10C2E8/10C2EC are native
 * state carried across all calls of a trace, never reloaded: a slot byte is
 * known once the original read it (first sighting) or the native code wrote
 * it; every later read of a known byte by the original must see the native
 * value. A call's node and the player nodes are built from what the call
 * read (the caller's actor state is an input). After each call the native
 * slots and records must equal the state before plus the call's writes, and
 * the node bytes the call wrote must match (no other node byte may change).
 * The selection's carry must match the caller binding right after it. */
#include <stdio.h>
#include <string.h>
#include "../src/neo_vslot.h"
typedef struct { uint8_t w; uint32_t addr; uint16_t mask,data; } vslot_access_t;
typedef struct {
    uint32_t frame; uint8_t source,service; int8_t p; uint32_t node; uint16_t node_word; uint32_t site;
    uint16_t a1,a2,a3,a4; uint8_t hit; int16_t rect[4]; uint8_t bound; uint32_t first;
} vslot_call_t;
#include "vslot_fixture.h"
enum { ACCEPT,RELEASE,FOLLOW,EXIT_READ,SELECT,BIND,EXIT,MOUNT,SHELL,DAMAGE,WHICH,RIDER,ANGLE,RIDDEN };
static const char *names[]={"accept","release","follow","exit_read","select","bind","exit","mount","shell","damage","which","rider","angle","ridden"};
enum { SLOT0=0x10c608u, SLOTS=0x48u, REC0=0x10c2e8u };
static neo_vslots_t v;
static uint8_t known[SLOTS],rec_known[2];
static uint8_t *slot_byte(unsigned o) {return &v.slot[o/0x24u].b[o%0x24u];}
/* The byte(s) an access covers: mask FF00 the even address, 00FF the odd. */
typedef void (*byte_fn)(uint32_t addr,uint8_t value,void *ctx);
static void each_byte(const vslot_access_t *x,byte_fn f,void *ctx) {
    if (x->mask&0xff00u) f(x->addr,(uint8_t)(x->data>>8),ctx);
    if (x->mask&0x00ffu) f(x->addr+1,(uint8_t)x->data,ctx);
}
/* Node bytes of the call: input = the value of the first access when it is a
 * read; expected after = the last write, else the input. */
typedef struct { uint32_t base; uint8_t m[0xc0],in[0xc0],out[0xc0],seen[0xc0],wrote[0xc0]; } node_t;
static node_t nodes[3];          /* call node, player 1, player 2 */
static uint8_t call_written[SLOTS+8],slot_seen[SLOTS];
static int fails;
static uint32_t cur_frame;
static uint8_t expect_slot[SLOTS];static uint32_t expect_rec[2];
static void node_access(uint32_t a,uint8_t val,int w) {
    for (unsigned n=0;n<3;n++) {
        node_t *q=&nodes[n];
        if (a<q->base || a>=q->base+0xc0u) continue;
        unsigned o=a-q->base;
        if (!q->seen[o]) {q->seen[o]=1;if (!w) q->in[o]=val;}
        if (w) {q->wrote[o]=1;q->out[o]=val;}
    }
}
static void read_in(uint32_t a,uint8_t val,void *ctx) {
    (void)ctx;node_access(a,val,0);
    if (a<SLOT0 || a>=SLOT0+SLOTS) return;
    unsigned o=a-SLOT0;
    if (call_written[o] || slot_seen[o]) return;
    slot_seen[o]=1;
    if (!known[o]) {*slot_byte(o)=val;known[o]=1;return;}
    if (*slot_byte(o)!=val) {fprintf(stderr,"frame %u: original read slot %06x = %02x, native %02x\n",cur_frame,(unsigned)a,val,*slot_byte(o));fails++;}
}
static void write_out(uint32_t a,uint8_t val,void *ctx) {
    (void)ctx;node_access(a,val,1);
    if (a>=SLOT0 && a<SLOT0+SLOTS) {expect_slot[a-SLOT0]=val;call_written[a-SLOT0]=1;return;}
    if (a>=REC0 && a<REC0+8u) {unsigned r=(a-REC0)/4u,sh=24u-8u*((a-REC0)%4u);expect_rec[r]=(expect_rec[r]&~(0xffu<<sh))|((uint32_t)val<<sh);call_written[SLOTS+(a-REC0)]=1;}
}
static uint16_t no_rng(void *ctx) {(void)ctx;fprintf(stderr,"frame %u: selection tie (0029C6) not in the fixture\n",cur_frame);fails++;return 0;}
int main(void) {
    unsigned n=sizeof(vslot_calls)/sizeof(vslot_calls[0])-1,checked=0,bytes=0,reads=0,binds=0,accepts=0,exits=0,selects=0;
    int source=-1;
    for (unsigned k=0;k<n;k++) {
        const vslot_call_t *c=&vslot_calls[k];cur_frame=c->frame;
        if (c->source!=source) {source=c->source;memset(&v,0,sizeof(v));memset(known,0,sizeof(known));memset(rec_known,0,sizeof(rec_known));}
        memset(call_written,0,sizeof(call_written));memset(slot_seen,0,sizeof(slot_seen));memset(nodes,0,sizeof(nodes));
        nodes[0].base=c->node;nodes[1].base=0x1014c4u;nodes[2].base=0x101574u;
        const vslot_access_t *acc=&vslot_access[c->first];unsigned na=vslot_calls[k+1].first-c->first;
        expect_rec[0]=v.record[0];expect_rec[1]=v.record[1];
        /* Slot reads see the native state (before the call's own writes). */
        for (unsigned i=0;i<na;i++) {each_byte(&acc[i],acc[i].w?write_out:read_in,NULL);if (!acc[i].w) reads++;}
        for (unsigned o=0;o<SLOTS;o++) if (!call_written[o]) expect_slot[o]=*slot_byte(o);
        for (unsigned q=0;q<3;q++) {
            memcpy(nodes[q].m,nodes[q].in,0xc0);
            for (unsigned o=0;o<0xc0;o++) if (!nodes[q].wrote[o]) nodes[q].out[o]=nodes[q].in[o];
        }
        uint8_t video_hi=0,video_lo=0,halted=0,system=0,unlimited=0;bool have_exists[2]={false,false};uint32_t head[2]={0,0};
        for (unsigned i=0;i<na;i++) if (!acc[i].w) {
            const vslot_access_t *x=&acc[i];
            if (x->addr==0x10000eu) {video_hi=(uint8_t)(x->data>>8);video_lo=(uint8_t)x->data;}
            if (x->addr==0x10c300u && (x->mask&0xffu)) halted=(uint8_t)x->data;      /* 10C301 (018F24) */
            if (x->addr==0x10c300u && (x->mask&0xff00u)) system=(uint8_t)(x->data>>8); /* 10C300 (019424) */
            if (x->addr==0x10f000u && (x->mask&0xffu)) unlimited=(uint8_t)((x->data&4u)!=0);
            for (unsigned q=0;q<2;q++) {
                uint32_t b=q?0x101574u:0x1014c4u;
                if (x->addr==b) {head[q]=(head[q]&0xffffu)|((uint32_t)x->data<<16);have_exists[q]=true;}
                if (x->addr==b+2) head[q]=(head[q]&0xffff0000u)|x->data;
            }
        }
        neo_vslot_player_t pl[2];
        for (unsigned q=0;q<2;q++) {pl[q].exists=!have_exists[q] || head[q]!=0x3f1eu;pl[q].m=nodes[1+q].m;}
        /* A node that is a player is the same record. */
        if (c->node==0x1014c4u) pl[0].m=nodes[0].m;
        if (c->node==0x101574u) pl[1].m=nodes[0].m;
        uint16_t video=(uint16_t)(video_hi<<8|video_lo);
        unsigned p=c->p<0?0:(unsigned)c->p;uint8_t *m=nodes[0].m;
        switch (c->service) {
        case ACCEPT:neo_vslot_accept(&v,p,video,halted!=0);accepts++;break;
        case RELEASE:neo_vslot_release(&v,p);break;
        case FOLLOW:neo_vslot_follow(&v,p,m);break;
        case EXIT_READ:{uint8_t kind,cause;neo_vslot_exit_place(&v,p,m,&kind,&cause);
            /* 0143EE runs between (not slot code), then 0AA5F2 clears +30/+34. */
            memset(m+0x30,0,8);exits++;break;}
        case SELECT:{int r=neo_vslot_select(&v,m,pl,c->rect,halted!=0,system!=0,no_rng,NULL);selects++;
            if ((r>=0)!=(c->bound!=0)) {fprintf(stderr,"frame %u: selection %d, original %s\n",c->frame,r,c->bound?"bound":"did not bind");fails++;}
            break;}
        case BIND:{uint16_t p6e;if (c->p<0) {fprintf(stderr,"frame %u: bind without slot\n",c->frame);fails++;break;}
            neo_vslot_bind(&v,p,m,c->node_word,&pl[p],video,(int16_t)c->a2,(int16_t)c->a3,&p6e);binds++;break;}
        case EXIT:neo_vslot_exit(&v,p,m,c->node_word,(uint8_t)c->a1);exits++;break;
        case MOUNT:neo_vslot_mount(&v,p,m,(uint8_t)c->a1,(uint8_t)c->a2,(uint8_t)c->a3,c->a4);break;
        case SHELL:neo_vslot_shell(&v,p,unlimited!=0);break;
        case DAMAGE:neo_vslot_damage(&v,p,m,c->hit!=0);break;
        case WHICH:(void)neo_vslot_which(&v,c->node_word);break;
        case RIDER:(void)neo_vslot_rider(&v,c->a1,pl);break;
        case ANGLE:neo_vslot_angle(&v,p,c->a1);break;
        case RIDDEN:{uint8_t k1,s1,t1;uint16_t h1,d41;
            if (!neo_vslot_ridden(&v,p,pl[p].exists,&k1,&s1,&t1,&h1,&d41)) {fprintf(stderr,"frame %u: ridden kind 0 (TRAP #F)\n",c->frame);fails++;}
            break;}
        }
        for (unsigned o=0;o<SLOTS;o++) {
            if (*slot_byte(o)!=expect_slot[o] && (known[o] || call_written[o])) {
                fprintf(stderr,"frame %u %s (site %06x): slot %06x = %02x, original %02x\n",c->frame,names[c->service],(unsigned)c->site,SLOT0+o,*slot_byte(o),expect_slot[o]);fails++;
            }
            if (call_written[o]) known[o]=1;
            bytes++;
        }
        for (unsigned r=0;r<2;r++) if (call_written[SLOTS+4*r] || rec_known[r]) {
            if (v.record[r]!=expect_rec[r]) {fprintf(stderr,"frame %u %s: record %u = %06x, original %06x\n",c->frame,names[c->service],r,(unsigned)v.record[r],(unsigned)expect_rec[r]);fails++;}
            rec_known[r]=1;
        }
        for (unsigned q=0;q<3;q++) for (unsigned o=0;o<0xc0;o++) {
            if (q && nodes[q].base==c->node) continue;
            if (nodes[q].m[o]!=nodes[q].out[o]) {
                fprintf(stderr,"frame %u %s: node %06x +%02X = %02x, original %02x\n",c->frame,names[c->service],(unsigned)nodes[q].base,o,nodes[q].m[o],nodes[q].out[o]);fails++;
            }
        }
        if (fails>20) break;
        checked++;
    }
    /* 0CA1B8: the held word of every stable input sample (and the A+B ones). */
    unsigned routed=0,ab=0;
    for (unsigned i=0;i<sizeof(vslot_routing)/sizeof(vslot_routing[0]);i++) {
        uint8_t fresh,h=neo_vslot_route((uint8_t)vslot_routing[i][1],0,neo_vslot_masks,&fresh);
        if (h!=vslot_routing[i][2]) {fprintf(stderr,"frame %u: inputs %02x route to %02x, original %02x\n",vslot_routing[i][0],vslot_routing[i][1],h,vslot_routing[i][2]);fails++;}
        routed++;if (h&0x80u) ab++;
    }
    if (fails) {fprintf(stderr,"FAIL: vehicle slot, %d differences\n",fails);return 1;}
    printf("PASS: vehicle slot services (%s): %u calls closed-loop (%u bindings, %u accepts, %u selections, %u exits), %u slot bytes compared, %u reads of native state; 0CA1B8 %u input samples (%u with A+B)\n",
        VSLOT_SOURCES,checked,binds,accepts,selects,exits,bytes,reads,routed,ab);
    return 0;
}
