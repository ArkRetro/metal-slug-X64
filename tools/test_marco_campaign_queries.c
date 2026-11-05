#include <assert.h>
#include <stdio.h>
#include "../src/marco_campaign_queries.h"

/* Optional original CPU-order P1 and P2-bank2 inputs independently verify all
 * converted descriptor fields and their 8000 terminators against actual ROM. */
static unsigned word(FILE *f) {
    int hi=fgetc(f),lo=fgetc(f);assert(hi>=0 && lo>=0);return (unsigned)((hi<<8)|lo);
}
static void rom_descriptor(FILE *f,long offset,uint32_t tag) {
    marco_campaign_hit_descriptor_t d;assert(marco_campaign_hit_descriptor(tag,&d));
    assert(!fseek(f,offset,SEEK_SET));
    assert(fgetc(f)==d.actor_class && fgetc(f)==d.groups);
    assert(word(f)==d.rectangle.x && word(f)==d.rectangle.y);
    assert(word(f)==d.rectangle.width && word(f)==d.rectangle.height);
    assert(word(f)==0x8000);
}
int main(int argc,char **argv) {
    if(argc==3) {
        FILE *p1=fopen(argv[1],"rb"),*p2=fopen(argv[2],"rb");assert(p1 && p2);
        rom_descriptor(p1,0x3bb40,0x3bb40);
        rom_descriptor(p2,0xd0aa8,0x2d0aa8);rom_descriptor(p2,0xd0ab4,0x2d0ab4);
        rom_descriptor(p2,0xd0ac0,0x2d0ac0);
        rom_descriptor(p2,0xd0acc,0x2d0acc);rom_descriptor(p2,0xd0ad8,0x2d0ad8);
        assert(!fseek(p2,0xd0ae4,SEEK_SET));
        assert(word(p2)==0xfffc && !word(p2) && word(p2)==34 && word(p2)==36);
        assert(!word(p2) && word(p2)==1 && !word(p2) && word(p2)==0xffff);
        fclose(p1);fclose(p2);
    } else assert(argc==1);
    marco_campaign_hit_registry_t registry;
    marco_campaign_hit_registry_reset(&registry);
    marco_campaign_hit_actor_t player={.identity=1,.field50_tag=0x2d0aa8,
        .projected_x=100,.projected_y=320};
    marco_campaign_hit_actor_t enemy={.identity=2,.field50_tag=0x3bb40,
        .projected_x=129,.projected_y=320,.hitpoints=32,.timer5a=3,.flags5f=64};
    uint16_t value=123;bool carry=true;
    assert(marco_campaign_hit_register(&registry,&player,false,&value,&carry));
    assert(registry.count==1 && player.registry_index==0 && !value && !carry);
    assert(marco_campaign_hit_proximity(&registry,&player,&value,&carry) && !value && !carry);
    assert(marco_campaign_hit_register(&registry,&enemy,false,&value,&carry));
    assert(registry.count==2 && enemy.registry_index==1 && value==3 && carry);
    assert(marco_campaign_hit_proximity(&registry,&player,&value,&carry) && value==5 && !carry);
    /* Common query sees class even when target is protected, dead or already
     * damaged. Its type/damage zero does not alter the registered target. */
    assert(registry.entries[1].actor.timer5a==3 && registry.entries[1].actor.hitpoints==32);
    enemy.flags5f=192;enemy.hitpoints=0;enemy.projected_x=140;
    assert(marco_campaign_hit_registry_refresh(&registry,&enemy));
    assert(marco_campaign_hit_proximity(&registry,&player,&value,&carry) && !value);
    enemy.projected_x=139;
    assert(marco_campaign_hit_registry_refresh(&registry,&enemy));
    assert(marco_campaign_hit_proximity(&registry,&player,&value,&carry) && value==5);
    enemy.field50_tag=0;assert(marco_campaign_hit_registry_refresh(&registry,&enemy));
    assert(marco_campaign_hit_proximity(&registry,&player,&value,&carry) && !value);
    enemy.field50_tag=0x3bb40;enemy.projected_x=129;
    assert(marco_campaign_hit_register(&registry,&enemy,true,&value,&carry));
    assert(registry.count==2 && enemy.hitpoints==0 && (enemy.flags5f&128) && value==6 && carry);
    assert(marco_campaign_hit_register(&registry,&player,true,&value,&carry) && registry.count==3);
    marco_campaign_hit_actor_t hidden=enemy;hidden.identity=3;hidden.projected_y=100;
    assert(marco_campaign_hit_register(&registry,&hidden,false,&value,&carry));
    assert(registry.count==3 && hidden.registry_index==255);
    hidden.field50_tag=0x123456;
    assert(!marco_campaign_hit_register(&registry,&hidden,false,&value,&carry));
    assert(!marco_campaign_hit_registry_refresh(&registry,&hidden));
    marco_campaign_hit_registry_reset(&registry);
    enemy.flags5f=0;enemy.timer5a=0;
    for(unsigned i=0;i<MARCO_CAMPAIGN_HIT_CAPACITY;++i) {
        enemy.identity=10+i;
        assert(marco_campaign_hit_register(&registry,&enemy,false,&value,&carry));
    }
    assert(registry.count==64);
    enemy.registry_index=77;
    assert(marco_campaign_hit_register(&registry,&enemy,false,&value,&carry));
    assert(registry.count==64 && enemy.registry_index==77);
    puts("PASS: original ROM hit metadata, registration, live refresh and read-only common proximity");
}
