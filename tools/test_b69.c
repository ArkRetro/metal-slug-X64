/* B69 (docs/BUG_MATRIX.md): an enemy's class-0 attack row must reach a
 * registered class-0 body, the player's ridden vehicle (0049E8's class-0
 * list holds the player nodes and every body registered with class bit 0;
 * the rider is unregistered, 0C308A). Before the fix the world compared a
 * class-0 row with the player only, so no enemy ever damaged a vehicle on the
 * cartridge while the vehicles' family tests (recorded 0049E8 answers)
 * passed. Hurtboxes from the ROM: SV-001 092676 (class mask 01), Slugnoid
 * 0966BC (01); the Camel's 0A0B02 (02) is class 1 and must not be hit. */
#include <stdio.h>
#include <string.h>
#include "../src/neo_world.h"
static neo_world_t w;
static int check(uint32_t box,bool expect,const char *name) {
    neo_world_init(&w,0);
    neo_gactor_t *g=&w.gactors[0];memset(g,0,sizeof(*g));g->active=true;g->index=0;
    ga_sl(g,0x50,box);ga_sw(g,0x3c,100);ga_sw(g,0x40,0x180);ga_sw(g,0x4e,0x100);
    w.last_boxes[0].owner=0;w.last_boxes[0].xbins=0xffff;w.last_boxes[0].ybins=0xff;w.last_box_count=1;
    neo_contact_actor_t a;memset(&a,0,sizeof(a));
    a.projected_x=a.cached_x=100<<16;a.projected_y=a.cached_y=0x180<<16;
    for (int16_t kind=0;kind<20;kind++) {
        ga_sw(g,0x4e,0x100);ga_sb(g,0x5a,0);ga_sb(g,0x5f,0);
        const int16_t row[7]={-32,-32,64,64,kind,0,0x10};
        uint8_t t=neo_world_class0_attack(&w,&a,0,row);
        if (expect && t && ga_w(g,0x4e)!=0x100) {printf("PASS: B69 %s: class-0 row kind %d hits the vehicle (type %u, health %04x)\n",name,kind,t,ga_w(g,0x4e));return 0;}
        if (!expect && (t || ga_w(g,0x4e)!=0x100)) {printf("FAIL: B69 %s: a class-0 row hit a class-1 body\n",name);return 1;}
    }
    if (expect) {printf("FAIL: B69 %s: no class-0 row damages the vehicle\n",name);return 1;}
    printf("PASS: B69 %s: class-0 rows leave the class-1 body alone\n",name);return 0;
}
int main(void) {
    int r=0;
    r|=check(0x92676u,true,"SV-001 092676");
    r|=check(0x966bcu,true,"Slugnoid 0966BC");
    r|=check(0xa0b02u,false,"Camel 0A0B02");
    return r;
}
