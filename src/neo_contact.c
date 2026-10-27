#include "neo_contact.h"
#include "neo_motion.h"
#include "neo_collision.h"
#include "contact_tables.h"
#include <assert.h>

static int32_t word32(uint32_t v) {return v<=0x7fffffffu?(int32_t)v:(int32_t)((int64_t)v-0x100000000LL);}
static int32_t add32(int32_t a,int32_t b) {return word32((uint32_t)a+(uint32_t)b);}
static int32_t sub32(int32_t a,int32_t b) {return word32((uint32_t)a-(uint32_t)b);}
static int16_t word16(uint16_t v) {return (int16_t)((int)(v&0x7fffu)-(v&0x8000u?32768:0));}
void neo_contact_vector(int32_t scalar,uint8_t angle,int32_t *vx,int32_t *vy) {
    int16_t magnitude=word16((uint16_t)neo_asr(scalar,8));
    *vx=(int32_t)magnitude*contact_coefficients[angle];
    *vy=-(int32_t)magnitude*contact_coefficients[(unsigned)angle+64];
}
neo_contact_request_t neo_contact_prepare_ground(neo_contact_actor_t *a) {
    neo_contact_request_t r={a->x,a->y,a->scalar62,0,a->parent_tag,0,
        a->flags15&32u?0:255,255,0,0};
    if (a->flags15&64u) {
        uint8_t angle=(uint8_t)a->kind_angle;
        if (!(a->flags5f&32u) && angle==64) r.mode|=0x7feu;
        if (a->flags5f&16u) {r.mode|=0x28009800u;if (angle!=64) r.mode|=0x7feu;}
        neo_contact_vector(a->scalar62,angle,&r.dx,&r.dy);
        if ((a->kind_angle>>8)==13u) { // 014648..01466A, statically decoded path.
            int32_t vy=sub32(a->vy,0x4000);
            r.dx=add32(a->vx,neo_asr(sub32(r.dx,a->vx),5));
            r.dy=add32(vy,neo_asr(sub32(r.dy,vy),5));
        }
    }
    bool right=a->flags15&8u,left=a->flags15&16u;a->flags15&=(uint8_t)~24u;
    if ((right && r.dx>=0) || (left && r.dx<0)) r.dx=r.dy=0;
    return r;
}
void neo_contact_commit(neo_contact_actor_t *a,const neo_contact_result_t *r,
    const neo_contact_parent_t *parent,uint8_t epoch) {
    assert(!r->parent_tag || (parent && parent->tag==r->parent_tag));
    a->x=r->x;a->y=r->y;a->vx=r->vx;a->vy=r->vy;a->parent_tag=r->parent_tag;
    a->flags15=(uint8_t)((a->flags15&63u)|((r->flags&1u)?64u:0)|((r->flags&2u)?128u:0));
    a->flags5f&=248u;
    if (r->flags&128u) {a->flags5f|=128u;a->timer5a=15;}
    if (r->flags&4u) a->flags5f|=1u;
    if (r->flags&8u) a->flags5f|=4u;
    if (r->flags&16u) a->flags5f|=2u;
    a->kind_angle=(uint16_t)(r->kind_angle_word>>8);
    int32_t oldx=a->projected_x,oldy=a->projected_y;
    a->projected_x=a->x;a->projected_y=a->y;a->correction=0;
    if (r->parent_tag) {
        unsigned carryx=((uint32_t)a->x&65535u)+((uint32_t)parent->x&65535u)>65535u;
        unsigned carryy=((uint32_t)a->y&65535u)+((uint32_t)parent->y&65535u)>65535u;
        a->correction=(uint8_t)(carryx|(carryy<<1)|parent->correction);
        a->projected_x=add32(a->x,parent->x);a->projected_y=add32(a->y,parent->y);
    }
    if (!a->mark) {a->cached_x=a->projected_x;a->cached_y=a->projected_y;a->mark=epoch;}
    else if (a->mark!=epoch) {a->cached_x=oldx;a->cached_y=oldy;a->mark=epoch;}
    if ((a->flags15&64u) && (a->field66&0xf000u)==0x8000u)
        a->word1e=(uint16_t)(a->field66-((uint32_t)a->projected_y>>16));
}
bool neo_contact_plane_ground(const neo_contact_actor_t *a,const neo_contact_request_t *r,
    neo_contact_result_t *out) {
    if (!(a->flags15&64u) || !r->parent_tag || r->parent_tag!=a->parent_tag) return false;
    if (r->mode8!=255 || r->mode9!=255 || r->modea || r->moded ||
        (r->mode && r->mode!=0x28009ffeu)) return false;
    neo_surface_t old,next;
    int32_t x=add32(r->x,r->dx),predicted=add32(r->y,r->dy);
    if (!neo_surface_unique(r->x,&old) || !neo_surface_unique(x,&next)) return false;
    if (old.flags!=0xc0u || next.flags!=0xc0u) return false;
    if (!((old.shape>=1 && old.shape<=4) || (old.shape>=6 && old.shape<=9) || old.shape==11) ||
        !((next.shape>=1 && next.shape<=4) || next.shape==11)) return false;
    int64_t start=(int64_t)r->y-old.height,end=(int64_t)predicted-next.height;
    if (start && ((start<0 && end<0) || (start>0 && end>0))) return false;
    *out=(neo_contact_result_t){x,next.height,r->dx,sub32(next.height,r->y),
        r->parent_tag,((uint32_t)contact_group_map[next.flags>>5]<<16)|contact_angles[next.shape],1};
    return true;
}
bool neo_contact_ground_step(neo_contact_actor_t *a,const neo_contact_parent_t *parent,
    uint8_t epoch,neo_contact_result_t *kernel_output,uint32_t *returned_word) {
    // Commit only a supported result; an unsupported case leaves the actor intact.
    neo_contact_actor_t candidate=*a;
    neo_contact_request_t request=neo_contact_prepare_ground(&candidate);neo_contact_result_t result;
    if (!neo_contact_plane_ground(&candidate,&request,&result)) return false;
    neo_contact_commit(&candidate,&result,parent,epoch);*a=candidate;
    if (kernel_output) *kernel_output=result;
    if (returned_word) *returned_word=(result.kind_angle_word<<16)|(result.kind_angle_word>>16);
    return true;
}
