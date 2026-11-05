#include "marco_campaign_queries.h"
#include <string.h>

bool marco_campaign_hit_descriptor(uint32_t tag,marco_campaign_hit_descriptor_t *d) {
    if(!d) return false;
    /* P2 physical bank 2, 0D0AA8..0D0AE3. These are selected by the
     * recovered native stance/motion/crouch controllers, not frame fixtures.
     * Header is class/groups; each four-word rectangle ends with 8000. */
    switch(tag) {
    case 0x2d0aa8: *d=(marco_campaign_hit_descriptor_t){5,1,{0xfff8,4,16,28}};return true;
    case 0x2d0ab4: *d=(marco_campaign_hit_descriptor_t){5,1,{0xfff8,0,19,14}};return true;
    case 0x2d0ac0: *d=(marco_campaign_hit_descriptor_t){5,1,{0xfff6,14,17,24}};return true;
    case 0x2d0acc: *d=(marco_campaign_hit_descriptor_t){5,1,{10,8,14,19}};return true;
    case 0x2d0ad8: *d=(marco_campaign_hit_descriptor_t){5,1,{5,8,12,20}};return true;
    /* P1 03BB40..03BB4B, assigned by soldier animation 23D510 FIELD50.
     * A scene may use this only for its native grounded soldier shape. */
    case 0x03bb40: *d=(marco_campaign_hit_descriptor_t){5,2,{0xfff6,4,20,32}};return true;
    default:return false;
    }
}

static int signed16(uint16_t v) {
    return (int)(v&0x7fffu)-(v&0x8000u?32768:0);
}
static neo_hit_rectangle_t projected(const marco_campaign_hit_actor_t *a,
    const neo_hit_rectangle_t *r) {
    uint16_t x=r->x,y=r->y;
    if(a->flags15&1u) x=(uint16_t)(0u-(uint16_t)(x+r->width));
    if(a->flags15&2u) y=(uint16_t)(0u-(uint16_t)(y+r->height));
    return (neo_hit_rectangle_t){(uint16_t)(x+a->projected_x),
        (uint16_t)(y+a->projected_y),r->width,r->height};
}
/* 004CAA..004D38/004A58..004ABE: clip the projected rectangle to
 * [-32,351] x [264,503], then mark every inclusive 32-pixel cell. The
 * returned masks retain the original broad-phase endpoint convention. */
static bool cells(const neo_hit_rectangle_t *r,uint16_t *xmask,uint8_t *ymask) {
    int x=signed16(r->x)+32,y=signed16(r->y)-264;
    int width=signed16(r->width),height=signed16(r->height);
    if(x<0) {width+=x;if(width<0) return false;x=0;}
    if(x>383) return false;
    int right=x+width;if(right>383) right=383;
    if(right==x) return false;
    if(y<0) {height+=y;if(height<0) return false;y=0;}
    if(y>239) return false;
    int top=y+height;if(top>239) top=239;
    if(top==y) return false;
    if(right<0 || top<0 || right<x || top<y) return false;
    *xmask=0;*ymask=0;
    for(int i=x>>5;i<=(right>>5);++i) *xmask|=(uint16_t)(1u<<i);
    for(int i=y>>5;i<=(top>>5);++i) *ymask|=(uint8_t)(1u<<i);
    return true;
}

void marco_campaign_hit_registry_reset(marco_campaign_hit_registry_t *r) {
    if(r) memset(r,0,sizeof(*r));
}
bool marco_campaign_hit_registry_refresh(marco_campaign_hit_registry_t *r,
    const marco_campaign_hit_actor_t *a) {
    if(!r || !a || r->count>MARCO_CAMPAIGN_HIT_CAPACITY) return false;
    if(a->field50_tag) {
        marco_campaign_hit_descriptor_t d;
        if(!marco_campaign_hit_descriptor(a->field50_tag,&d)) return false;
    }
    for(unsigned i=0;i<r->count;++i) if(r->entries[i].actor.identity==a->identity)
        r->entries[i].actor=*a;
    return true;
}
bool marco_campaign_hit_register(marco_campaign_hit_registry_t *r,
    marco_campaign_hit_actor_t *a,bool kill_nonplayer,uint16_t *value,bool *carry) {
    if(!r || !a || !value || !carry || r->count>MARCO_CAMPAIGN_HIT_CAPACITY) return false;
    if(a->field50_tag) {
        marco_campaign_hit_descriptor_t d;
        if(!marco_campaign_hit_descriptor(a->field50_tag,&d)) return false;
        /* 004BAC: player-owned descriptors have group byte exactly one. */
        if(kill_nonplayer && d.groups!=1) {
            a->hitpoints=0;a->flags5f|=128u;a->timer5a=6;
        } else if(r->count<MARCO_CAMPAIGN_HIT_CAPACITY) {
            neo_hit_rectangle_t rectangle=projected(a,&d.rectangle);
            uint16_t xm;uint8_t ym;
            a->registry_index=255;
            if(cells(&rectangle,&xm,&ym)) {
                a->registry_index=r->count;
                r->entries[r->count++]=(marco_campaign_hit_registration_t){*a,xm,ym,d.groups};
            }
        }
    }
    /* 004DA6..004DC8. Impact offsets are consumed separately by their owner. */
    *value=a->timer5a;*carry=a->timer5a!=0;return true;
}
bool marco_campaign_hit_proximity(const marco_campaign_hit_registry_t *r,
    const marco_campaign_hit_actor_t *a,uint16_t *value,bool *carry) {
    if(!r || !a || !value || !carry || r->count>MARCO_CAMPAIGN_HIT_CAPACITY) return false;
    /* P2 physical bank 2 ROM 2D0AE4, passed by common tail C374A. */
    static const neo_hit_rectangle_t local={0xfffc,0,34,36};
    neo_hit_rectangle_t query=projected(a,&local);
    uint16_t xm;uint8_t ym;uint16_t found=0;
    if(cells(&query,&xm,&ym)) for(unsigned i=0;i<r->count;++i) {
        const marco_campaign_hit_registration_t *entry=&r->entries[i];
        const marco_campaign_hit_actor_t *target=&entry->actor;
        if(target->identity==a->identity || !target->field50_tag || !(entry->groups&2u) ||
            !(entry->x_cells&xm) || !(entry->y_cells&ym)) continue;
        marco_campaign_hit_descriptor_t descriptor;
        if(!marco_campaign_hit_descriptor(target->field50_tag,&descriptor)) return false;
        if(neo_hit_rectangle(&query,target->projected_x,target->projected_y,
            target->flags15,&descriptor.rectangle,NULL,NULL) && descriptor.actor_class>=found)
            found=descriptor.actor_class;
        /* 004854 updates max class BEFORE damage compatibility, invincibility,
         * timer or hitpoint filtering. Query type/damage are both zero, so the
         * later damage path cannot write target timers or hitpoints. */
    }
    *value=found;*carry=false;return true;
}

bool marco_campaign_melee_descriptor(uint32_t tag,marco_campaign_melee_descriptor_t *d) {
    if(!d) return false;
    /* P2 physical bank2: rect, type, group, damage, FFFF terminator. */
    switch(tag) {
    case 0x2d0af4:*d=(marco_campaign_melee_descriptor_t){{0xfffc,0,36,36},2,1,30};return true;
    case 0x2d0b04:*d=(marco_campaign_melee_descriptor_t){{0xfffc,0,36,36},3,1,30};return true;
    default:return false;
    }
}
bool marco_campaign_hit_melee_requests(const marco_campaign_hit_registry_t *r,
    const marco_campaign_hit_actor_t *a,uint32_t tag,
    marco_campaign_melee_request_handler_t handler,void *context,uint16_t *value,bool *carry) {
    marco_campaign_melee_descriptor_t attack;
    if(!r || !a || !handler || !value || !carry || r->count>MARCO_CAMPAIGN_HIT_CAPACITY ||
        !marco_campaign_melee_descriptor(tag,&attack)) return false;
    neo_hit_rectangle_t query=projected(a,&attack.rectangle);
    uint16_t xm,found=0;uint8_t ym;
    if(cells(&query,&xm,&ym)) for(unsigned i=0;i<r->count;++i) {
        const marco_campaign_hit_registration_t *entry=&r->entries[i];
        const marco_campaign_hit_actor_t *target=&entry->actor;
        if(target->identity==a->identity || !target->field50_tag ||
            !(entry->groups&(attack.groups<<1)) || !(entry->x_cells&xm) || !(entry->y_cells&ym)) continue;
        marco_campaign_hit_descriptor_t descriptor;
        if(!marco_campaign_hit_descriptor(target->field50_tag,&descriptor)) return false;
        uint16_t ix,iy;
        if(!neo_hit_rectangle(&query,target->projected_x,target->projected_y,
            target->flags15,&descriptor.rectangle,&ix,&iy)) continue;
        if(descriptor.actor_class>=found) found=descriptor.actor_class;
        marco_campaign_melee_request_t request={.attacker=a->identity,.target=target->identity,
            .type=attack.type,.damage=attack.damage,.impact_x=ix,.impact_y=iy,
            .target_class=descriptor.actor_class};
        if(!handler(context,&request)) return false;
    }
    *value=found;*carry=false;return true;
}
