#include "player_runtime.h"
#include "marco_campaign_palette.h"
#include "neo_collision.h"
#include <string.h>

enum { PLAYER_ID=1,PROTOTYPE_SOLDIER_ID=2 };

static bool stop(player_runtime_t *p,marco_campaign_status_t status,uint32_t reason) {
    if(p->campaign.status==MARCO_CAMPAIGN_READY) {
        p->campaign.status=status;p->campaign.reason=reason;
    }
    if(p->game) p->game->player_failed=true;
    return false;
}
static bool sound(player_runtime_t *p,uint16_t token) {
    if(p->sound_count==PLAYER_RUNTIME_SOUNDS)
        return stop(p,MARCO_CAMPAIGN_ANIMATION_EVENT,0x0120f8);
    p->sound_queue[(p->sound_head+p->sound_count)%PLAYER_RUNTIME_SOUNDS]=token;
    ++p->sound_count;return true;
}
bool player_runtime_pop_sound(player_runtime_t *p,uint16_t *token) {
    if(!p || !token || !p->sound_count) return false;
    *token=p->sound_queue[p->sound_head];
    p->sound_head=(p->sound_head+1)%PLAYER_RUNTIME_SOUNDS;--p->sound_count;
    return true;
}
bool player_runtime_stamp(const player_runtime_t *p,int column,int row) {
    return p && column>=0 && column<PLAYER_RUNTIME_STAMP_COLUMNS &&
        row>=0 && row<PLAYER_RUNTIME_STAMP_ROWS &&
        p->footprints[column*PLAYER_RUNTIME_STAMP_ROWS+row]!=0;
}
static int signed_word(uint16_t value) {
    return (int)(value&32767u)-(value&32768u?32768:0);
}
static int word_tile(int32_t position) {
    int word=signed_word((uint16_t)((uint32_t)position>>16));
    return word>=0?word/16:-((15-word)/16);
}
static uint16_t projected_x(const player_runtime_t *p,int32_t x) {
    return (uint16_t)(((uint32_t)x>>16)-(uint16_t)p->game->camera);
}
static uint16_t projected_y(int32_t y) {
    return (uint16_t)(((uint32_t)y>>16)+256u);
}
static marco_campaign_hit_actor_t player_view(player_runtime_t *p) {
    marco_crouch_t *b=&p->campaign.body.motion.player;
    return (marco_campaign_hit_actor_t){.identity=PLAYER_ID,
        .field50_tag=p->campaign.animation.field50_tag,
        .projected_x=projected_x(p,p->world.contact.projected_x),
        .projected_y=projected_y(p->world.contact.projected_y),
        .hitpoints=(uint16_t)p->game->health,
        .flags15=(uint8_t)b->upper.input.attributes,.flags5f=b->upper.input.flags5f,
        .timer5a=p->campaign.common.field5a};
}
static marco_campaign_hit_actor_t soldier_view(player_runtime_t *p) {
    const msx_enemy_t *e=&p->game->enemy;
    int32_t x=e->x*MSX_UNIT;
    /* This scene actor uses the actual grounded soldier geometry. Its AI,
     * authored spawn positions, health and damage response remain provisional. */
    return (marco_campaign_hit_actor_t){.identity=PROTOTYPE_SOLDIER_ID,
        .field50_tag=e->alive?0x03bb40:0,
        .projected_x=projected_x(p,x),
        .projected_y=projected_y(56*MSX_UNIT-msx_ground_offset(x)),
        .hitpoints=(uint16_t)e->health,.flags15=(uint8_t)(64u|(e->facing<0?1u:0u))};
}
static bool refresh_opposite(player_runtime_t *p) {
    marco_campaign_hit_actor_t player=player_view(p),soldier=soldier_view(p);
    marco_campaign_hit_registry_t *r=&p->registry[p->bank^1u];
    return marco_campaign_hit_registry_refresh(r,&player) &&
        marco_campaign_hit_registry_refresh(r,&soldier);
}
static bool begin_registry(player_runtime_t *p) {
    marco_campaign_hit_registry_t *r=&p->registry[p->bank];
    marco_campaign_hit_actor_t soldier=soldier_view(p);
    uint16_t value=0;bool carry=false;
    marco_campaign_hit_registry_reset(r);
    return marco_campaign_hit_register(r,&soldier,false,&value,&carry) && refresh_opposite(p);
}
static bool random16(void *context,uint16_t *value) {
    player_runtime_t *p=context;
    if(!value) return false;
    *value=neo_rng_next(&p->random);return true;
}
static bool idle_query(void *context,marco_idle_query_t key,int16_t argument,
    marco_motion_t *motion,uint16_t *value,bool *carry) {
    player_runtime_t *p=context;(void)argument;(void)motion;
    return marco_campaign_sensing_idle_query(&p->scene,key,0,value,carry);
}
static bool project(void *context,marco_campaign_t *campaign,marco_animation_view_t *v) {
    player_runtime_t *p=context;
    if(!marco_campaign_world_view(&p->world,&campaign->body.motion.player,v)) return false;
    v->render.x=(uint16_t)(v->render.x-(uint16_t)p->game->camera);
    v->render.y=(uint16_t)(v->render.y+256u);
    v->render.z=(uint16_t)(v->render.z-256u);
    v->cached_x=(uint16_t)(v->cached_x-(uint16_t)p->game->camera);
    v->cached_y=(uint16_t)(v->cached_y+256u);
    if((v->render.attributes>>8)==0x63 && p->palette_descriptor==0x390)
        v->render.attributes=(uint16_t)((v->render.attributes&255u)|0x6400u);
    v->render.flash_palette=0;
    v->render.palettes[0]=0x63;v->render.palettes[1]=0x64;
    v->render.palettes[2]=0x65;v->render.palettes[3]=0;
    v->render.suppress=(campaign->body.motion.player.upper.input.flags5f&8u) &&
        (p->game->ticks&1u);
    p->draw_camera=p->game->camera;
    return true;
}
static bool prepare(void *context,marco_common_t *common,marco_body_t *body,marco_animation_t *animation) {
    player_runtime_t *p=context;marco_crouch_t *b=&body->motion.player;
    (void)animation;
    common->projected_y=(int16_t)projected_y(p->world.contact.projected_y);
    common->death_line=p->scene_death_line;
    /* 019432 -> B5870. The outer grounded/group/scene tests and the inner
     * original foreground-parent/resource guard precede the tile mutation.
     * The native foreground owner retains original source-map edits and its
     * renderer recomposes each changed tile over the lower scenery layers. */
    if((b->upper.input.attributes&64u) && b->field6c==8 && !p->scene.field42f6 &&
        p->foreground_parent && p->foreground_resource_tag==0x0b533a) {
        int column=word_tile(b->x),row=7-word_tile(b->y);
        /* C70FC's source clipping is a successful return outside 72x5. */
        if(column>=0 && column<PLAYER_RUNTIME_STAMP_COLUMNS && row>=0 && row<PLAYER_RUNTIME_STAMP_ROWS) {
            unsigned index=(unsigned)(column*PLAYER_RUNTIME_STAMP_ROWS+row);
            if(!p->footprints[index]) {p->footprints[index]=1;++p->footprint_writes;}
        }
    }
    return true;
}
static bool palette_write(void *context,uint8_t bank,uint16_t descriptor) {
    player_runtime_t *p=context;
    /* The dedicated renderer owns these two real cartridge palette resources.
     * Other descriptors cannot be represented by silently reusing normal. */
    if(bank!=0x63 || (descriptor!=0x376 && descriptor!=0x390)) return false;
    p->palette_bank=bank;p->palette_descriptor=descriptor;
    return true;
}
static bool reset_weapon(void *context,marco_body_t *body) {
    player_runtime_t *p=context;
    if(!marco_campaign_sensing_reset_weapon(&body->motion.player.upper,&p->pistol.ammo)) return false;
    p->campaign.common.ammunition=p->pistol.ammo.ammunition;
    p->campaign.common.active_projectiles=p->pistol.ammo.active;return true;
}
static bool common_query(void *context,marco_common_query_t key,marco_body_t *body,
    uint16_t *value,bool *carry) {
    player_runtime_t *p=context;marco_crouch_t *b=&body->motion.player;
    if(!value || !carry) return false;
    switch(key) {
    case MCQ_PROTECTION_CA2F8:
        /* The native prototype world's actual protection timer owns this
         * return; it is never inferred from an animation or input fixture. */
        *carry=p->game->invulnerable!=0;return true;
    case MCQ_CONTACT_4C24: {
        marco_campaign_hit_actor_t actor=player_view(p);
        if(!marco_campaign_hit_register(&p->registry[p->bank],&actor,false,value,carry)) return false;
        *value=marco_campaign_sensing_registered_damage(actor.timer5a,actor.impact_x,actor.impact_y,
            &p->damage_offset_x,&p->damage_offset_y,carry);
        return true;
    }
    case MCQ_PROXIMITY_49E8: {
        if(!refresh_opposite(p)) return false;
        marco_campaign_hit_actor_t actor=player_view(p);
        return marco_campaign_hit_proximity(&p->registry[p->bank^1u],&actor,value,carry);
    }
    case MCQ_ENVIRONMENT_AA458: {
        bool result=marco_campaign_sensing_environment_update(&p->environment,0,
            (uint16_t)p->game->ticks,&p->scene,value,carry);
        /* Preserve the real 0128E8 policy assignment as typed scene state.
         * These words are not guessed to be the independent death boundary. */
        if(result && marco_campaign_sensing_environment_take_effect(&p->environment,0,&p->world_policy)) {
            ++p->world_policy_writes;p->pending_environment_effects=0;
        }
        return result;
    }
    case MCQ_FORM_C0BAE:
        (void)b;return false; /* Alternate-form world service has no native owner. */
    default:return false;
    }
}
static bool spawn(void *context,const marco_campaign_shot_t *shot) {
    player_runtime_t *p=context;msx_game_t *g=p->game;
    for(unsigned i=0;i<MSX_BULLETS;++i) if(!g->bullets[i].active && !p->projectile_owned[i]) {
        g->bullets[i]=(msx_bullet_t){.x=shot->x/MSX_UNIT,.y=248-shot->y/MSX_UNIT,
            .direction=shot->vx<0?-1:1,.active=true,.world_x=shot->x,.world_y=shot->y,
            .vx=shot->vx,.vy=shot->vy,.born_tick=g->ticks,
            .angle=shot->angle,.kind=shot->kind,.native=true};
        p->projectile_owned[i]=true;p->projectile_started[i]=false;
        p->projectile_born[i]=g->ticks;p->projectile_kind[i]=shot->kind;
        g->shooting=true;g->shot_age=0;return true;
    }
    return false;
}
static bool prototype_melee_damage(void *context,const marco_campaign_melee_request_t *request) {
    player_runtime_t *p=context;msx_game_t *g=p->game;
    if(request->target!=PROTOTYPE_SOLDIER_ID || request->target_class!=5) return false;
    p->last_melee_request=*request;++p->melee_requests;
    /* Explicit N64 prototype response. The authored encounter has health2;
     * source attack damage30 kills it. Original target handler004892 and the
     * soldier's original reaction/AI are not implemented by this response. */
    msx_enemy_t *e=&g->enemy;
    if(!e->alive) return true;
    ++g->hits;e->flash=10;
    if(request->damage>=e->health) {
        e->health=0;e->alive=false;e->respawn=90;++g->kills;
    } else e->health-=request->damage;
    return true;
}
static bool event(void *context,marco_campaign_t *campaign,
    const marco_campaign_event_t *e,neo_anim_render_t *render) {
    player_runtime_t *p=context;(void)render;
    if(e->kind!=NEO_ANIM_EVENT) return false;
    switch(e->target) {
    case 0x0c4372:return marco_campaign_pistol_event(&p->pistol,campaign,e,spawn,p);
    case 0x0c53c4: {
        if(e->length!=6) return false;
        uint16_t value=0;bool carry=false;
        if(!marco_campaign_sensing_idle_query(&p->scene,MSQ_191FC,0,&value,&carry)) return false;
        /* C53C4's actual scene==3 guard returns before effect allocation. */
        return value==3;
    }
    case 0x0120f8:
        if(e->length!=2) return false;
        {uint16_t token=(uint16_t)((uint16_t)e->payload[0]<<8|e->payload[1]);
        return (token==0x10fc || token==0x1146) && sound(p,token);}
    case 0x0c419e: {
        if(e->length!=4 || !refresh_opposite(p)) return false;
        uint32_t tag=(uint32_t)e->payload[0]<<24|(uint32_t)e->payload[1]<<16|
            (uint32_t)e->payload[2]<<8|e->payload[3];
        marco_campaign_melee_descriptor_t descriptor;
        if(!marco_campaign_melee_descriptor(tag,&descriptor)) return false;
        marco_campaign_hit_actor_t actor=player_view(p);uint16_t value=0;bool carry=false;
        if(!marco_campaign_hit_melee_requests(&p->registry[p->bank^1u],&actor,tag,
            prototype_melee_damage,p,&value,&carry)) return false;
        /* C41C0..C4202 chooses sounds from the real max overlapping class. */
        if((uint8_t)value==5) return sound(p,descriptor.type==2?0x10fb:0x10f9);
        if((uint8_t)value==4) return sound(p,0x1069);
        return true;
    }
    case 0x0c4786:
        return false; /* Grenade child actor has no native implementation yet. */
    default:return false;
    }
}
static void export_game(player_runtime_t *p) {
    msx_game_t *g=p->game;marco_crouch_t *b=&p->campaign.body.motion.player;
    g->x=b->x;g->y=56*MSX_UNIT-b->y;g->velocity_x=b->vx;g->velocity_y=-b->vy;
    g->facing=b->upper.input.attributes&1u?-1:1;
    g->grounded=(b->upper.input.attributes&64u)!=0;
    g->crouched=p->campaign.body.domain==MARCO_BODY_CROUCH;
    g->moving=b->motion62!=0;
}
static bool export_hitbox(player_runtime_t *p) {
    msx_game_t *g=p->game;
    g->player_hitbox.valid=false;
    uint32_t tag=p->campaign.animation.field50_tag;
    if(!tag) return true;
    marco_campaign_hit_descriptor_t descriptor;
    if(!marco_campaign_hit_descriptor(tag,&descriptor) || descriptor.groups!=1)
        return stop(p,MARCO_CAMPAIGN_SERVICE,0x004c24);
    int local_x=signed_word(descriptor.rectangle.x),local_y=signed_word(descriptor.rectangle.y);
    int width=descriptor.rectangle.width,height=descriptor.rectangle.height;
    uint16_t attributes=p->campaign.body.motion.player.upper.input.attributes;
    if(attributes&1u) local_x=-(local_x+width);
    if(attributes&2u) local_y=-(local_y+height);
    int x=signed_word((uint16_t)((uint32_t)p->world.contact.projected_x>>16));
    int y=signed_word((uint16_t)((uint32_t)p->world.contact.projected_y>>16));
    g->player_hitbox=(msx_player_hitbox_t){.x=x+local_x,.y=248-y-local_y-height,
        .width=width,.height=height,.valid=true};
    return true;
}
/* The authored world's health handler can move/respawn its player after the
 * recovered actor call. Import that real world outcome into the same actor. */
static bool import_game_position(player_runtime_t *p) {
    msx_game_t *g=p->game;marco_crouch_t *b=&p->campaign.body.motion.player;
    int32_t y=56*MSX_UNIT-g->y;
    if(b->x==g->x && b->y==y && b->vx==g->velocity_x && b->vy==-g->velocity_y &&
        ((b->upper.input.attributes&64u)!=0)==g->grounded) return true;
    int old_x=b->x/MSX_UNIT,old_y=b->y/MSX_UNIT;
    marco_crouch_t relocated=*b;
    relocated.vx=g->velocity_x;relocated.vy=-g->velocity_y;
    /* The world owner updates the real support kind/angle, projection,
     * correction, history and depth word together. A position-only copy
     * leaves stale slope/roof state after authored knockback or respawn. */
    if(!marco_campaign_world_relocate(&p->world,&relocated,g->x,y,g->grounded))
        return stop(p,MARCO_CAMPAIGN_SERVICE,0x013968);
    *b=relocated;
    p->draw_offset_x+=b->x/MSX_UNIT-old_x;
    p->draw_offset_y+=old_y-b->y/MSX_UNIT;
    return true;
}
bool player_runtime_step(void *context,msx_game_t *g,const msx_game_input_t *input) {
    player_runtime_t *p=context;
    if(!p || !g || !input || !p->initialized || p->game!=g ||
        p->campaign.status!=MARCO_CAMPAIGN_READY) return false;
    if(!import_game_position(p)) return false;
    p->draw_offset_x=0;p->draw_offset_y=0;
    ++p->world.epoch;
    if(!marco_campaign_world_set_policy(&p->world,p->world_policy.words,
        g->camera*MSX_UNIT,0,256*MSX_UNIT))
        return stop(p,MARCO_CAMPAIGN_SERVICE,0x0138a0);
    if(!begin_registry(p)) return stop(p,MARCO_CAMPAIGN_SERVICE,0x004c24);
    uint16_t held=(uint16_t)((input->up?1u:0u)|(input->down?2u:0u)|
        (input->direction<0?4u:input->direction>0?8u:0u)|
        (input->fire?16u:0u)|(input->jump?32u:0u)|(input->grenade?64u:0u));
    marco_campaign_services_t services={.context=p,
        .motion={.context=&p->world,.move=marco_campaign_world_motion_move},
        .crouch={.context=&p->world,.move=marco_campaign_world_crouch_move},
        .stance={.context=p,.idle_query=idle_query},
        .common={.context=p,.query=common_query,.prepare=prepare,
            .palette_lookup=marco_campaign_palette_lookup,.palette_write=palette_write,
            .reset_weapon=reset_weapon},
        .random16=random16,.project=project,.event=event};
    if(!marco_campaign_update(&p->campaign,held,input->pressed,&services)) {
        g->player_failed=true;return false;
    }
    marco_crouch_t *b=&p->campaign.body.motion.player;
    int minimum=(g->camera+28)*MSX_UNIT;
    int clamped=b->x<minimum?minimum:b->x>MSX_END_X?MSX_END_X:b->x;
    if(clamped!=b->x) {
        int32_t delta=clamped-b->x;
        p->draw_offset_x=clamped/MSX_UNIT-b->x/MSX_UNIT;
        b->x=clamped;b->vx=0;
        p->world.contact.x=clamped;p->world.contact.vx=0;
        p->world.contact.projected_x+=delta;p->world.contact.cached_x+=delta;
    }
    export_game(p);
    if(!export_hitbox(p)) return false;
    p->bank^=1u;return true;
}
bool player_runtime_start(player_runtime_t *p,msx_game_t *g) {
    if(!p || !g) return false;
    memset(p,0,sizeof(*p));p->game=g;
    marco_campaign_init(&p->campaign,g->x,56*MSX_UNIT-g->y,g->facing);
    p->campaign.animation.flash=0xff;
    if(!marco_campaign_world_init(&p->world,&p->campaign.body.motion.player,g->x,56*MSX_UNIT-g->y))
        return stop(p,MARCO_CAMPAIGN_SERVICE,0x013968);
    marco_campaign_pistol_init(&p->pistol);
    uint16_t counter=0;(void)neo_rng_seed(&p->random,0,&counter);
    p->campaign.body.motion.player.upper.bombs=10;
    p->campaign.body.motion.player.upper.input.attributes|=0x6300;
    p->campaign.common.field19=0x63;p->campaign.common.field1a=0x64;
    p->palette_bank=0x63;p->palette_descriptor=0x376;
    p->world_policy=(marco_campaign_environment_effect_t){.player=0,
        .policy=MARCO_CAMPAIGN_WORLD_POLICY_DEFAULT,.words={28,0,264,480}};
    /* Measured native Mission-1 scene A5+472C, before the death comparison. */
    p->scene_death_line=256;
    p->draw_camera=g->camera;p->foreground_parent=true;p->foreground_resource_tag=0x0b533a;
    p->initialized=true;g->player_failed=false;
    /* Bootstrap the scene's already-live grounded soldier into the prior
     * registration bank. Later calls alternate the actual native banks. */
    p->bank=1;
    if(!begin_registry(p)) return stop(p,MARCO_CAMPAIGN_SERVICE,0x004c24);
    p->bank=0;
    const msx_game_input_t neutral={0};
    return player_runtime_step(p,g,&neutral);
}
void player_runtime_after_world(player_runtime_t *p,msx_game_t *g) {
    if(!p || !g || !p->initialized || p->game!=g) return;
    for(unsigned i=0;i<MSX_BULLETS;++i) if(p->projectile_owned[i]) {
        msx_bullet_t *b=&g->bullets[i];
        /* Child 01B782 emits sound before its first movement/collision call.
         * A bullet retired during that first world call still owns its sound. */
        if(!g->player_failed && !p->projectile_started[i] &&
            g->ticks-p->projectile_born[i]>=2 && !(g->ticks&1u)) {
            if(!sound(p,0x10f7)) return;
            p->projectile_started[i]=true;
        }
        if(!b->active) {
            marco_campaign_pistol_release(&p->pistol,&p->campaign,p->projectile_kind[i]);
            p->projectile_owned[i]=false;p->projectile_started[i]=false;
        }
    }
    if(!import_game_position(p)) return;
    (void)export_hitbox(p);
}
