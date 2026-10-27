#include "neo_throw_actor.h"
#include <string.h>
void neo_throw_actor_start(neo_throw_actor_t *s,const neo_contact_actor_t *seed,
    uint16_t field6e,uint8_t flash,const neo_anim_render_t *graphics_seed) {
    memset(s,0,sizeof(*s));s->body=*seed;s->field6e=field6e;s->animation.flash=flash;
    s->render=*graphics_seed;s->phase=NEO_THROW_NEW;s->active=true;
}
static bool palette(const neo_throw_services_t *service,neo_anim_render_t *render,
    uint16_t asset,unsigned slot,bool replace) {
    uint8_t handle;
    if (!service->palette || !service->palette(service->context,asset,replace,&handle)) return false;
    render->palettes[slot]=handle;
    render->attributes=(uint16_t)((render->attributes&255u)|((unsigned)handle<<8));return true;
}
static neo_throw_result_t done(neo_throw_actor_t *s,const neo_throw_services_t *service) {
    s->active=false;if (service->release) service->release(service->context);return NEO_THROW_DONE;
}
neo_throw_result_t neo_throw_actor_step(neo_throw_actor_t *s,const neo_throw_frame_t *f,
    const neo_throw_services_t *service,neo_anim_emit_t emit,void *context,unsigned *events) {
    if (events) *events=0;
    if (!s->active) return NEO_THROW_DONE;
    if (!f || !f->scene || !f->players || !f->bounds || !service || !service->flight || !service->burst ||
        !service->flight_rectangle || !service->burst_rectangle) return NEO_THROW_UNSUPPORTED;
    if (s->phase==NEO_THROW_NEW) {
        if (!palette(service,&s->render,0x35d,0,true) || !palette(service,&s->render,0x364,1,false) ||
            !palette(service,&s->render,0x365,2,false)) return NEO_THROW_UNSUPPORTED;
        neo_anim_start(&s->animation,service->flight,0x100);
        neo_throw_initialize(&s->body,f->players,s->field6e);s->phase=NEO_THROW_FLYING;
    } else if (s->phase==NEO_THROW_BURST) {
        s->body.field66=0x8820;s->body.word1e=0x8820;
        if (events) *events|=NEO_THROW_AUDIO_0;
        neo_anim_start(&s->animation,service->burst,0x100);
        if (!palette(service,&s->render,0x36c,0,true)) return NEO_THROW_UNSUPPORTED;
        s->body.flags15&=253u;s->animation.field50_tag=0;s->phase=NEO_THROW_EFFECT;
    }
    bool flying=s->phase==NEO_THROW_FLYING;
    if (flying) {if (!neo_throw_free_step(&s->body,f->scene,f->epoch,0)) return NEO_THROW_UNSUPPORTED;}
    else if (s->phase==NEO_THROW_EFFECT) neo_contact_project(&s->body,f->scene,f->epoch);
    else return NEO_THROW_UNSUPPORTED;
    s->render.x=(uint16_t)((uint32_t)s->body.projected_x>>16);s->render.y=(uint16_t)((uint32_t)s->body.projected_y>>16);
    s->render.attributes=(uint16_t)((s->render.attributes&0xff00u)|s->body.flags15);
    s->render.z=s->body.word1e;s->render.correction=s->body.correction;
    neo_anim_result_t result=neo_anim_step(&s->animation,&s->render,emit,context,1024);
    if (result!=NEO_ANIM_MORE && result!=NEO_ANIM_COMPLETE) return NEO_THROW_UNSUPPORTED;
    if (!flying && result==NEO_ANIM_COMPLETE) return done(s,service);
    if (flying) {
        if ((s->body.flags15&128u) || f->region_code || f->other_contact) s->phase=NEO_THROW_BURST;
        unsigned index=f->video_phase&3u;if (index>=3) index=1;
        s->render.attributes=(uint16_t)((s->render.attributes&255u)|((unsigned)s->render.palettes[index]<<8));
    }
    if (neo_cull_rectangle_outside(&s->body,f->bounds,flying?service->flight_rectangle:service->burst_rectangle)) return done(s,service);
    return NEO_THROW_MORE;
}
