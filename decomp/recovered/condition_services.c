#include "condition_services.h"
#include "../../src/neo_player_query.h"
#include "../../src/neo_player_positions.h"
#include <assert.h>
#include <string.h>
typedef struct {const msx_condition_bus *memory;bool trap15;} services_t;
static uint32_t address(uint32_t a) {return a&UINT32_C(0xffffff);}
static uint8_t rb(void *ctx,uint32_t a) {services_t *s=ctx;return s->memory->read8(s->memory->context,address(a));}
static uint16_t rw(void *ctx,uint32_t a) {services_t *s=ctx;return s->memory->read16(s->memory->context,address(a));}
static uint32_t rl(void *ctx,uint32_t a) {uint32_t hi=rw(ctx,a);return (hi<<16)|rw(ctx,a+2u);}
static void wb(void *ctx,uint32_t a,uint8_t value) {services_t *s=ctx;s->memory->write8(s->memory->context,address(a),value);}
static void ww(void *ctx,uint32_t a,uint16_t value) {services_t *s=ctx;s->memory->write16(s->memory->context,address(a),value);}
static void wl(void *ctx,uint32_t a,uint32_t value) {ww(ctx,a,(uint16_t)(value>>16));ww(ctx,a+2u,(uint16_t)value);}
static bool helper(void *ctx,msx_condition_helper kind,msx_condition_registers *r) {
    services_t *s=ctx;uint32_t a5=r->a[5];
    if(kind==MSX_QUERY_AAA88) {
        unsigned selected=(uint8_t)r->d[0]!=0;
        uint32_t object=a5-(selected?0x6a8cu:0x6b3cu),table=a5+(selected?0x462cu:0x4608u);
        neo_player_query_slot_t slots[2]={{0}};
        neo_player_query_slot_t *slot=&slots[selected];
        slot->object_header=rl(ctx,object);
        if(slot->object_header!=0x3f1eu) {
            slot->active=rw(ctx,table);
            if(slot->active) {
                slot->state21=rb(ctx,table+0x21u);
                if(slot->state21) {
                    slot->field1f=rb(ctx,table+0x1fu);slot->field20=rb(ctx,table+0x20u);
                    slot->field18=rw(ctx,table+0x18u);slot->field14=rw(ctx,table+0x14u);
                }
            }
        }
        neo_player_query_registers_t q={.sr=r->sr};memcpy(q.d,r->d,sizeof(q.d));
        neo_player_query_status_t result=neo_player_query(slots,&q);
        memcpy(r->d,q.d,sizeof(q.d));r->sr=q.sr;r->a[0]=object;r->a[4]=table;
        /* Last JSR stack residue from AAAA2 -> 40AC, whose RTS restores A7.
         * Preserve data effects; bus timing/word write order remain excluded. */
        wl(ctx,r->a[7]-4u,0x0aaaa8u);
        if(result==NEO_PLAYER_QUERY_TRAP15){s->trap15=true;return false;}
        return true;
    }
    if(kind==MSX_QUERY_1903A) {
        neo_player_positions_frame_t f={.fallback_x=rw(ctx,a5+0x69b8u),
            .fallback_y=rw(ctx,a5+0x69bau),.enabled=rb(ctx,a5+0x4322u)};
        if(f.enabled) {
            for(unsigned p=0;p<2;p++) {
                uint32_t object=a5-(p?0x6a8cu:0x6b3cu);
                neo_position_player_t *player=&f.players[p];player->object_header=rl(ctx,object);
                if(player->object_header==0x3f1eu)continue;
                player->use_x=rb(ctx,object+0x7au);player->use_y=rb(ctx,object+0x7bu);
                if(player->use_x)player->x=rw(ctx,object+0x3cu);
                if(player->use_y)player->y=rw(ctx,object+0x40u);
            }
        }
        neo_player_positions_registers_t q={.sr=r->sr};memcpy(q.d,r->d,sizeof(q.d));
        neo_player_positions(&f,&q);memcpy(r->d,q.d,sizeof(q.d));r->sr=q.sr;
        if(f.enabled)r->a[0]=a5-0x6a8cu;
        wl(ctx,r->a[7]-4u,f.enabled?0x019088u:0x01904cu);
        return true;
    }
    return false;
}
msx_native_condition_status msx_condition_evaluate_native(msx_condition_registers *r,const msx_condition_bus *memory) {
    assert(r && memory && memory->read8 && memory->read16 && memory->write8 && memory->write16);
    services_t s={.memory=memory};
    msx_condition_bus bus={.context=&s,.read8=rb,.read16=rw,.write8=wb,.write16=ww,.helper=helper};
    msx_condition_status result=msx_rom_01a5ca(r,&bus);
    if(s.trap15)return MSX_NATIVE_CONDITION_TRAP15;
    if(result==MSX_CONDITION_UNSUPPORTED_INDEX)return MSX_NATIVE_CONDITION_UNSUPPORTED;
    return result==MSX_CONDITION_OK?MSX_NATIVE_CONDITION_OK:MSX_NATIVE_CONDITION_FAILURE;
}
