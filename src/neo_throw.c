#include "neo_throw.h"
#include "neo_motion.h"
#include "neo_collision.h"
#include "throw_tables.h"
#include <string.h>
static int32_t s32(uint32_t v) {return v<=0x7fffffffu?(int32_t)v:(int32_t)((int64_t)v-0x100000000LL);}
static int s16(uint16_t v) {return (int)(v&32767u)-(v&32768u?32768:0);}
static uint16_t abs16(uint16_t v) {return v&32768u?(uint16_t)(0u-v):v;}
void neo_throw_spawn(neo_contact_actor_t *a,const neo_contact_actor_t *source,
    const neo_contact_parent_t *scene,uint8_t epoch) {
    memset(a,0,sizeof(*a));
    uint32_t x=(uint32_t)source->projected_x-(uint32_t)scene->x;
    uint32_t y=(uint32_t)source->projected_y+0x200000u-(uint32_t)scene->y;
    uint32_t oldx=(uint32_t)source->cached_x-(uint32_t)scene->previous_x;
    uint32_t oldy=(uint32_t)source->cached_y+0x200000u-(uint32_t)scene->previous_y;
    a->x=s32(x);a->y=s32(y);a->vx=s32(x-oldx);a->vy=s32(y-oldy);a->parent_tag=scene->tag;
    a->projected_x=source->projected_x;a->projected_y=s32((uint32_t)source->projected_y+0x200000u);
    a->cached_x=a->projected_x;a->cached_y=a->projected_y;a->mark=epoch;
    a->flags15=source->flags15&3u;
    unsigned carryx=(x&65535u)+((uint32_t)scene->x&65535u)>65535u;
    unsigned carryy=(y&65535u)+((uint32_t)scene->y&65535u)>65535u;
    a->correction=(uint8_t)((carryy<<1)|carryx|scene->correction);
}
void neo_throw_initialize(neo_contact_actor_t *a,const neo_control_frame_t *scene,uint16_t field6e) {
    a->flags15|=32u;a->field66=0xe0ef;a->word1e=0xe0ef;
    if (field6e && field6e!=1) a->flags15^=1u;
    neo_target_t target=neo_target_pick(a,scene,throw_target_rectangle);
    uint16_t horizontal=32,vertical=64;
    if (target.valid) {
        uint16_t dy=(uint16_t)(target.y-((uint32_t)a->projected_y>>16));
        vertical=s16((uint16_t)((uint32_t)a->projected_y>>16))>416?64:80;
        if (s16((uint16_t)((uint32_t)a->projected_y>>16))<=416 && s16(dy)>96) vertical=112;
        uint16_t distance=(uint16_t)neo_asr(s16((uint16_t)(0u-dy)),1);
        distance=(uint16_t)(distance+abs16((uint16_t)(target.x-((uint32_t)a->projected_x>>16))));
        uint16_t limit=(uint16_t)(vertical+(vertical>>1));
        if (distance>=limit) distance=limit;
        horizontal=(uint16_t)((((distance+32u)&0xffc0u)-32u)+64u);
    }
    a->vy=(int32_t)((uint32_t)vertical*0x1b33u);
    uint32_t vx=(uint32_t)neo_asr(s32((uint32_t)horizontal<<16),5);
    if (!(a->flags15&1u)) vx=0u-vx;
    a->vx=s32(vx);
}
void neo_throw_accelerate(neo_contact_actor_t *a) {
    int32_t delta=s32(0xfff40000u-(uint32_t)a->vy);
    a->vy=s32((uint32_t)a->vy+(uint32_t)neo_asr(delta,5));
    a->vx-=neo_asr(a->vx,5);
}

bool neo_throw_free_step(neo_contact_actor_t *a,const neo_contact_parent_t *scene,
    uint8_t epoch,neo_contact_result_t *kernel_output) {
    if (!scene || a->parent_tag!=scene->tag || (a->flags15&64u) ||
        scene->x!=scene->previous_x || scene->y!=scene->previous_y) return false;
    neo_contact_actor_t candidate=*a;neo_throw_accelerate(&candidate);
    neo_contact_request_t r=neo_contact_prepare_air(&candidate);
    if (r.dx < -4*NEO_UNIT || r.dx > 4*NEO_UNIT) return false;
    int32_t x=s32((uint32_t)r.x+(uint32_t)r.dx),y=s32((uint32_t)r.y+(uint32_t)r.dy);
    int start=neo_asr(r.x,16),end=neo_asr(x,16);
    int low=start<end?start:end,high=start>end?start:end;
    int32_t lowest_y=r.y<y?r.y:y;
    for (int column=low;column<=high+1;column++) {
        neo_surface_t floor;int64_t grid_x=(int64_t)column*NEO_UNIT;
        if (grid_x<INT32_MIN || grid_x>INT32_MAX) return false;
        if (!neo_surface_unique((int32_t)grid_x,&floor) || lowest_y<=floor.height) return false;
    }
    neo_contact_result_t result={x,y,r.dx,r.dy,r.parent_tag,throw_air_kind,0};
    neo_contact_commit(&candidate,&result,scene,epoch);
    *a=candidate;if (kernel_output) *kernel_output=result;return true;
}
