#include <assert.h>
#include <stdio.h>
#include "../src/marco_campaign_queries.h"

static unsigned requests;
static marco_campaign_melee_request_t last;
static bool request(void *context,const marco_campaign_melee_request_t *hit) {
    assert(context==&requests);last=*hit;++requests;return true;
}
static unsigned word(FILE *f) {
    int hi=fgetc(f),lo=fgetc(f);assert(hi>=0 && lo>=0);return (unsigned)((hi<<8)|lo);
}
static void source_descriptor(FILE *f,long offset,uint32_t tag) {
    marco_campaign_melee_descriptor_t descriptor;
    assert(marco_campaign_melee_descriptor(tag,&descriptor));
    assert(!fseek(f,offset,SEEK_SET));
    assert(word(f)==descriptor.rectangle.x && word(f)==descriptor.rectangle.y);
    assert(word(f)==descriptor.rectangle.width && word(f)==descriptor.rectangle.height);
    assert(word(f)==descriptor.type && word(f)==descriptor.groups && word(f)==descriptor.damage);
    assert(word(f)==0xffff);
}
int main(int argc,char **argv) {
    if(argc==2) {
        FILE *source=fopen(argv[1],"rb");assert(source);
        source_descriptor(source,0xd0af4,0x2d0af4);
        source_descriptor(source,0xd0b04,0x2d0b04);fclose(source);
    } else assert(argc==1);
    marco_campaign_hit_registry_t registry;
    marco_campaign_hit_registry_reset(&registry);
    marco_campaign_hit_actor_t player={.identity=1,.projected_x=100,.projected_y=320};
    marco_campaign_hit_actor_t target={.identity=2,.field50_tag=0x03bb40,
        .projected_x=142,.projected_y=320,.hitpoints=2,.timer5a=3,.flags5f=64};
    uint16_t value=123;bool carry=true;
    assert(marco_campaign_hit_register(&registry,&target,false,&value,&carry));
    assert(marco_campaign_hit_melee_requests(&registry,&player,0x2d0af4,request,&requests,&value,&carry));
    assert(!value && !carry && !requests); /* Touching an edge is not overlap. */
    target.projected_x=141;
    assert(marco_campaign_hit_registry_refresh(&registry,&target));
    assert(marco_campaign_hit_melee_requests(&registry,&player,0x2d0af4,request,&requests,&value,&carry));
    assert(value==5 && !carry && requests==1);
    assert(last.attacker==1 && last.target==2 && last.type==2 && last.damage==30 && last.target_class==5);
    /* Damage policy belongs to the handler. The producer only emits actual
     * geometry/impact requests and never mutates timers, protection or HP. */
    assert(registry.entries[0].actor.hitpoints==2 && registry.entries[0].actor.timer5a==3);
    player.flags15=1;target.projected_x=59;
    assert(marco_campaign_hit_registry_refresh(&registry,&target));
    /* Refresh preserves old masks; a scene's next registration rebuilds them. */
    marco_campaign_hit_registry_reset(&registry);
    assert(marco_campaign_hit_register(&registry,&target,false,&value,&carry));
    assert(marco_campaign_hit_melee_requests(&registry,&player,0x2d0b04,request,&requests,&value,&carry));
    assert(value==5 && requests==2 && last.type==3);
    assert(!marco_campaign_hit_melee_requests(&registry,&player,0xdeadbe,request,&requests,&value,&carry));
    puts("PASS: actual melee descriptors, facing, contact edges and typed world damage requests");
    return 0;
}
