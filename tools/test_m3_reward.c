/* Original event-state/request equivalence plus the independent full-helper
 * fixture: 08CECC/057948/01ACBE execute on the original CPU. Its allocator
 * and sound queue are observed boundaries, not emulated here. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/neo_region.h"
#include "m3_reward_fixture.h"
#ifdef N64
#include <libdragon.h>
#endif
#define COUNT(a) (sizeof(a)/sizeof((a)[0]))
static struct {
    neo_gactor_t owner,*event,labels[2];neo_contact_parent_t root;
    unsigned sounds,labels_count;
    uint16_t tokens[2];
    struct {uint32_t x,y,px,py;} calls[2];
} context;
static unsigned current,failures;
#define CHECK(c) do {if (!(c)) {printf("M3 REWARD FAIL case=%u line=%u: %s\n",current,__LINE__,#c);failures++;}} while (0)
static bool parent(void *ctx,uint32_t tag,neo_contact_parent_t *out) {
    (void)ctx;
    if (tag==0x10e968) {*out=context.root;return true;}
    if (tag==NEO_GA_TAG(0)) {
        *out=(neo_contact_parent_t){.tag=tag,.x=(int32_t)ga_l(&context.owner,0x3c),
            .y=(int32_t)ga_l(&context.owner,0x40),.previous_x=(int32_t)ga_l(&context.owner,0x44),
            .previous_y=(int32_t)ga_l(&context.owner,0x48),.correction=ga_b(&context.owner,0x4d)};
        return true;
    }
    return false;
}
static neo_gactor_t *creator(void *ctx,neo_gactor_t *a) {(void)ctx;(void)a;return &context.owner;}
static bool creator_alive(void *ctx,neo_gactor_t *a) {
    (void)ctx;return !ga_l(a,0xc) || ga_l(&context.owner,0)!=0x3f1e;
}
static void sound(void *ctx,uint16_t token) {
    (void)ctx;CHECK(token==0x1121);CHECK(context.sounds<2);
    if (context.sounds<2) {
        neo_gactor_t *a=context.event;unsigned i=context.sounds++;
        context.tokens[i]=token;
        context.calls[i].x=ga_l(a,0x28);context.calls[i].y=ga_l(a,0x2c);
        context.calls[i].px=ga_l(a,0x3c);context.calls[i].py=ga_l(a,0x40);
    }
}
static neo_gactor_t *spawn(void *ctx,neo_gactor_t *a,uint32_t routine,bool after_script) {
    (void)ctx;CHECK(!a && routine==0x8d330 && after_script);
    CHECK(context.labels_count<2);if (context.labels_count>=2) return NULL;
    neo_gactor_t *label=&context.labels[context.labels_count++];
    memset(label,0,sizeof(*label));label->active=true;ga_sl(label,0,routine);return label;
}
static void normalize(uint8_t *record) {
    if (record[0x38]==0 && record[0x39]==0x10 && record[0x3a]==0x18 && record[0x3b]==0x28) {
        uint32_t tag=NEO_GA_TAG(0);
        for (unsigned i=0;i<4;i++) record[0x38+i]=(uint8_t)(tag>>(24-8*i));
    }
}
int main(void) {
#ifdef N64
    debug_init_emulog();
#endif
    neo_gactor_t a={0};unsigned requests=0,deletions=0;
    for (current=0;current<COUNT(m3_reward_cases);current++) {
        const __typeof__(*m3_reward_cases) *c=&m3_reward_cases[current];
        uint8_t before[192],expected[192],effect_after[192];memcpy(before,c->before,192);normalize(before);
        memcpy(expected,c->after,192);normalize(expected);
        memcpy(effect_after,c->effects.after,192);normalize(effect_after);
        if (c->preserve) CHECK(!memcmp(a.m,before,192));
        else {memset(&a,0,sizeof(a));memcpy(a.m,before,192);}
        a.active=true;a.index=1;
        memset(&context,0,sizeof(context));memcpy(context.owner.m,c->owner,192);context.owner.index=0;
        context.event=&a;context.root=(neo_contact_parent_t){0x10e968,c->rx,c->ry,c->old_rx,c->old_ry,c->correction};
        neo_pickup_env_t pickup={0};neo_genv_t e={0};e.parent=parent;e.creator=creator;e.creator_alive=creator_alive;
        memcpy(pickup.score,c->effects.score_before,sizeof(pickup.score));
        e.epoch=c->epoch;e.players.active_mask=c->mask;e.live_players_valid=true;
        e.pickup=&pickup;e.spawn=spawn;e.sound=sound;
        for (unsigned p=0;p<2;p++) {e.live_player[p].x=c->player[p].x;e.live_player[p].y=c->player[p].y;e.live_player[p].flags15=c->player[p].flags;}
        ga_result_t result=neo_m3reward_tick(&a,&e);
        CHECK(!e.unsupported);CHECK(result==(c->deleted?GA_DELETE:GA_MORE));
        for (unsigned j=0;j<192;j++) if (a.m[j]!=expected[j]) {
            printf("M3 REWARD state case=%u %s offset=%02x got=%02x expected=%02x\n",current,c->name,j,a.m[j],expected[j]);
            failures++;break;
        }
        CHECK(!memcmp(a.m,effect_after,192));
        CHECK(context.sounds==c->count && context.labels_count==c->count && pickup.popups==c->count);
        CHECK(context.sounds==c->effects.sound_count && context.labels_count==c->effects.label_count);
        CHECK(context.labels_count==c->effects.allocation_count);
        CHECK(!memcmp(context.tokens,c->effects.sounds,context.sounds*sizeof(context.tokens[0])));
        for (unsigned i=0;i<c->count && i<context.labels_count;i++) {
            const m3_reward_request_t *r=&c->requests[i];neo_gactor_t *label=&context.labels[i];
            CHECK(r->d1==1 && (r->tag==0xb99e0 || r->tag==0xb99e4));
            CHECK(context.calls[i].x==r->x && context.calls[i].y==r->y);
            CHECK(context.calls[i].px==r->px && context.calls[i].py==r->py);
            CHECK(ga_l(label,0x6e)==r->value && ga_b(label,0x8a)==r->player);
            CHECK(!ga_l(label,0x38));
            CHECK(ga_l(label,0x28)==(r->px&0xffff0000u) && ga_l(label,0x2c)==(r->py&0xffff0000u));
            /* 00411C owns +00/+0C and the native pool identity. Everything
             * 01ACBE and 08CECC write is compared without that allocator. */
            CHECK(!memcmp(label->m+0x28,c->effects.labels[i]+0x28,0x4e - 0x28));
            CHECK(!memcmp(label->m+0x6e,c->effects.labels[i]+0x6e,4));
            CHECK(label->m[0x8a]==c->effects.labels[i][0x8a]);
            CHECK(c->effects.allocations[i].reserve==0xe && c->effects.allocations[i].routine==0x8d330 &&
                c->effects.allocations[i].creator==0x101414);
        }
        CHECK(!memcmp(pickup.score,c->effects.score,sizeof(pickup.score)));
        requests+=c->count;deletions+=c->deleted;
    }
    neo_genv_t absent={0};ga_sl(&a,0,0xb9780);
    CHECK(neo_m3reward_tick(&a,&absent)==GA_UNSUPPORTED && absent.unsupported_at==0x1aeb0);
    if (failures) {printf("M3 REWARD FAIL count=%u\n",failures);return 1;}
    printf("PASS: M3 platform reward cases=%u requests=%u deletions=%u; original score/sound/label effects\n",(unsigned)COUNT(m3_reward_cases),requests,deletions);
#ifdef N64
    debugf("MSX M3 REWARD PASS cases=%u requests=%u deletions=%u effects=original\n",(unsigned)COUNT(m3_reward_cases),requests,deletions);
    for (;;) wait_ms(100);
#endif
    return 0;
}
