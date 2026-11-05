#include "marco_campaign_world.h"
#include "neo_collision.h"
#include "contact_tables.h"
#include <string.h>

static int32_t signed32(uint32_t v) {
    return v<=INT32_MAX?(int32_t)v:(int32_t)((int64_t)v-INT64_C(4294967296));
}
static int32_t add32(int32_t a,int32_t b) {return signed32((uint32_t)a+(uint32_t)b);}
static int32_t sub32(int32_t a,int32_t b) {return signed32((uint32_t)a-(uint32_t)b);}
static uint32_t kind_angle(const neo_surface_t *s) {
    return ((uint32_t)contact_group_map[s->flags>>5]<<16)|contact_angles[s->shape];
}
static void import_actor(neo_contact_actor_t *a,const marco_crouch_t *p) {
    a->x=p->x;a->y=p->y;a->vx=p->vx;a->vy=p->vy;a->scalar62=p->motion62;
    a->flags15=(uint8_t)p->upper.input.attributes;a->flags5f=p->upper.input.flags5f;
    a->kind_angle=(uint16_t)((a->kind_angle&255u)|((uint16_t)p->field6c<<8));
}
static void export_actor(const neo_contact_actor_t *a,marco_crouch_t *p) {
    p->x=a->x;p->y=a->y;p->vx=a->vx;p->vy=a->vy;
    p->upper.input.attributes=(uint16_t)((p->upper.input.attributes&0xff00u)|a->flags15);
    p->upper.input.flags5f=a->flags5f;p->field6c=(uint8_t)(a->kind_angle>>8);
}
static void commit(marco_campaign_world_t *w,neo_contact_actor_t *a,
    const neo_contact_result_t *r,marco_crouch_t *p) {
    neo_contact_commit(a,r,r->parent_tag?&w->terrain:NULL,w->epoch);
    w->contact=*a;export_actor(a,p);
}
bool marco_campaign_world_init(marco_campaign_world_t *w,marco_crouch_t *p,int32_t x,int32_t y) {
    if(!w || !p) return false;
    neo_surface_t s;
    if(!neo_surface_at(x,y,0,0,&s) || s.shape>13) return false;
    memset(w,0,sizeof(*w));
    /* A local native identity, unrelated to the original actor address. */
    w->terrain.tag=1;w->epoch=1;
    w->contact=(neo_contact_actor_t){.x=x,.y=y,.projected_x=x,.projected_y=y,
        .cached_x=x,.cached_y=y,.parent_tag=1,.kind_angle=(uint16_t)(kind_angle(&s)>>8),
        .field66=0x8834,.word1e=(uint16_t)(0x8834-((uint32_t)y>>16)),
        .flags15=(uint8_t)((p->upper.input.attributes&63u)|64u),
        .flags5f=p->upper.input.flags5f,.mark=1};
    w->initialized=true;export_actor(&w->contact,p);return true;
}
bool marco_campaign_world_relocate(marco_campaign_world_t *w,marco_crouch_t *p,
    int32_t x,int32_t y,bool grounded) {
    if(!w || !p || !w->initialized) return false;
    neo_surface_t support;
    if(grounded && (!neo_surface_at(x,y,0,0,&support) || support.shape>13)) return false;
    neo_contact_actor_t actor=w->contact;import_actor(&actor,p);
    actor.x=x;actor.y=y;
    actor.flags15=(uint8_t)((actor.flags15&~64u)|(grounded?64u:0u));
    actor.parent_tag=grounded?w->terrain.tag:0;
    actor.kind_angle=grounded?(uint16_t)(kind_angle(&support)>>8):0;
    neo_contact_project(&actor,grounded?&w->terrain:NULL,w->epoch);
    /* A scene-owned relocation establishes fresh history for its next body
     * call; stale prior-support coordinates must not re-enter rendering. */
    actor.cached_x=actor.projected_x;actor.cached_y=actor.projected_y;actor.mark=w->epoch;
    if(grounded && (actor.field66&0xf000u)==0x8000u)
        actor.word1e=(uint16_t)(actor.field66-((uint32_t)actor.projected_y>>16));
    w->contact=actor;export_actor(&actor,p);return true;
}
bool marco_campaign_world_set_policy(marco_campaign_world_t *w,const uint16_t words[4],
    int32_t camera_x,int32_t scroll_y,int32_t projected_y_offset) {
    if(!w || !w->initialized || !words) return false;
    memcpy(w->policy_words,words,sizeof(w->policy_words));
    w->camera_x=camera_x;w->scroll_y=scroll_y;w->projected_y_offset=projected_y_offset;
    w->policy_enabled=true;return true;
}
bool marco_campaign_world_clip_request(marco_campaign_world_t *w,neo_contact_request_t *r) {
    if(!w || !w->initialized || !r) return false;
    w->boundary_hits=0;
    if(!w->policy_enabled) return true;
    const uint16_t *words=w->policy_words;
    int32_t x=sub32(r->x,w->camera_x),y=add32(r->y,w->projected_y_offset),limit;
    if(r->dx>0) {
        limit=signed32((uint32_t)(uint16_t)(words[0]+words[2])<<16);
        limit=sub32(sub32(limit,1),x);if(limit<0) limit=0;
        if(r->dx>limit) {r->dx=limit;w->boundary_hits|=1;}
    } else if(r->dx<0) {
        limit=sub32(signed32((uint32_t)words[0]<<16),x);if(limit>=0) limit=0;
        if(r->dx<limit) {r->dx=limit;w->boundary_hits|=2;}
    }
    if(r->dy>0) {
        limit=signed32((uint32_t)(uint16_t)(words[1]+words[3])<<16);
        limit=sub32(sub32(sub32(limit,w->scroll_y),1),y);if(limit<0) limit=0;
        if(r->dy>limit) {r->dy=limit;w->boundary_hits|=4;}
    } else if(r->dy<0) {
        limit=sub32(sub32(signed32((uint32_t)words[1]<<16),w->scroll_y),y);
        /* 013952..01395A first tests BMI, then BGE, on CMP.L's flags.
         * Preserve that distinction even for synthetic overflowing inputs. */
        int32_t difference=sub32(r->dy,limit);
        bool overflow=(((uint32_t)r->dy^(uint32_t)limit)&
            ((uint32_t)r->dy^(uint32_t)difference)&UINT32_C(0x80000000))!=0;
        if(difference<0 && !overflow) {r->dy=limit;w->boundary_hits|=8;}
    }
    return true;
}
bool marco_campaign_world_ground(marco_campaign_world_t *w,marco_crouch_t *p) {
    if(!w || !p || !w->initialized) return false;
    neo_contact_actor_t actor=w->contact;import_actor(&actor,p);
    neo_contact_request_t request=neo_contact_prepare_ground(&actor);
    if(!marco_campaign_world_clip_request(w,&request)) return false;
    neo_contact_result_t result;
    /* The exact already-recovered plane kernel takes precedence. */
    if(neo_contact_plane_ground(&actor,&request,&result)) {
        commit(w,&actor,&result,p);return true;
    }
    /* Existing measured terrain adapter: follow the reachable floor within
     * four pixels. This also handles the authored vehicle's stamped wreck
     * surfaces and the roof over the lower floor. It is not the full original
     * corner/edge or moving-object resolver. */
    int32_t x=add32(request.x,request.dx),predicted=add32(request.y,request.dy);
    neo_surface_t next;
    /* Query down from the upper limit, preserving neo_walk_step's preference
     * for the highest reachable plane when a wreck overlaps the old floor. */
    bool attached=neo_surface_at(x,add32(predicted,4*NEO_UNIT),0,8*NEO_UNIT,&next);
    result=(neo_contact_result_t){.x=x,.y=predicted,.vx=request.dx,.vy=request.dy};
    if(attached) {
        result.y=next.height;result.vy=sub32(next.height,request.y);
        result.parent_tag=w->terrain.tag;result.kind_angle_word=kind_angle(&next);result.flags=1;
    }
    commit(w,&actor,&result,p);return true;
}
bool marco_campaign_world_air(marco_campaign_world_t *w,marco_crouch_t *p,bool wrapper) {
    if(!w || !p || !w->initialized) return false;
    neo_contact_actor_t actor=w->contact;import_actor(&actor,p);
    if(wrapper) {
        /* C109C consumes D0 from the preceding C0FEA. Re-evaluate into a copy
         * to recover that value without an extra input-filter side effect. */
        marco_input_state_t input=p->upper.input;
        uint32_t buttons=marco_input_filter(&input,p->upper.packed,p->upper.gate);
        if((buttons&80u) && (p->upper.input.flags9a&8u)) actor.flags15^=1u;
    }
    int32_t old_vy=actor.vy;
    neo_contact_request_t request=neo_contact_prepare_air(&actor);
    if(!marco_campaign_world_clip_request(w,&request)) return false;
    neo_air_t air={add32(request.x,request.dx),add32(request.y,request.dy),request.dx,request.dy};
    bool landed=neo_land_step(&air,request.y);neo_surface_t next;
    neo_contact_result_t result={.x=air.x,.y=air.y,.vx=air.vx,.vy=air.vy};
    if(landed) {
        if(!neo_surface_at(air.x,air.y,0,0,&next)) return false;
        result.parent_tag=w->terrain.tag;result.kind_angle_word=kind_angle(&next);
        result.flags=0x43; /* Original downward plane clip + attachment. */
    }
    /* C10B6..C10D6 preserves upward velocity unless contact sets ceiling bit.
     * Downward landing velocity remains the actual displacement. */
    commit(w,&actor,&result,p);
    if(wrapper && old_vy>=0) {
        p->vy=(p->upper.input.flags5f&2u)?0:old_vy;w->contact.vy=p->vy;
    }
    return true;
}
bool marco_campaign_world_motion_move(void *context,marco_motion_service_t kind,
    marco_motion_t *m,marco_animation_t *a) {
    if(!m || !a || a->failed) return false;
    marco_campaign_world_t *w=context;
    if(kind==MMM_GROUND) return marco_campaign_world_ground(w,&m->player);
    if(kind==MMM_AIR) return marco_campaign_world_air(w,&m->player,true);
    if(kind==MMM_FALL_SUPPORT) return marco_campaign_world_air(w,&m->player,false);
    return false;
}
bool marco_campaign_world_crouch_move(void *context,marco_crouch_t *p,marco_animation_t *a) {
    return a && !a->failed && marco_campaign_world_ground(context,p);
}
bool marco_campaign_world_view(const marco_campaign_world_t *w,const marco_crouch_t *p,
    marco_animation_view_t *v) {
    if(!w || !p || !v || !w->initialized) return false;
    v->render.x=(uint16_t)((uint32_t)w->contact.projected_x>>16);
    v->render.y=(uint16_t)((uint32_t)w->contact.projected_y>>16);
    v->cached_x=(uint16_t)((uint32_t)w->contact.cached_x>>16);
    v->cached_y=(uint16_t)((uint32_t)w->contact.cached_y>>16);
    v->render.attributes=p->upper.input.attributes;v->render.correction=w->contact.correction;
    v->render.z=w->contact.word1e;return true;
}
