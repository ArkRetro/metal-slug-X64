#include "neo_player_query.h"
#include <assert.h>
static void result_flags(neo_player_query_registers_t *r, uint32_t value, uint32_t sign) {
    r->sr=(uint16_t)((r->sr&0xfff0u)|(value==0?4u:0u)|(value&sign?8u:0u));
}
neo_player_query_status_t neo_player_query(const neo_player_query_slot_t slots[2],
                                          neo_player_query_registers_t *r) {
    assert(slots && r);
    r->selected_slot=(uint8_t)((uint8_t)r->d[0]!=0);
    const neo_player_query_slot_t *p=&slots[r->selected_slot];
    /* 0040AC: header 00003F1E is the invalid-object sentinel. Other headers
     * return carry=1. The caller then tests the independent active word. */
    if(p->object_header==UINT32_C(0x3f1e) || !p->active) {
        r->d[0]&=UINT32_C(0xffff0000); /* MOVE.W #0 preserves upper 16 bits. */
        for(unsigned i=1;i<5;i++)r->d[i]=0;
        result_flags(r,0,UINT32_C(0x80000000));
        return NEO_PLAYER_QUERY_OK;
    }
    r->d[0]=(r->d[0]&UINT32_C(0xffffff00))|p->state21;
    result_flags(r,p->state21,0x80u); /* CMPI.B #0 preserves X. */
    if(!p->state21)return NEO_PLAYER_QUERY_TRAP15; /* Original TRAP #15. */
    r->d[1]=(r->d[1]&UINT32_C(0xffffff00))|p->field1f;
    r->d[2]=(r->d[2]&UINT32_C(0xffffff00))|p->field20;
    r->d[3]=(r->d[3]&UINT32_C(0xffff0000))|p->field18;
    r->d[4]=(r->d[4]&UINT32_C(0xffff0000))|p->field14;
    result_flags(r,p->field14,0x8000u);
    return NEO_PLAYER_QUERY_OK;
}
