/* Slug gauge and gas can (docs/ARMAS_VEHICULOS.md §11, BUG_MATRIX B41).
 *
 * 0AAAE0 (neo_vslot_gauge): a bound slot of a vehicle kind 1, 3, 4, 8..B has a
 * gauge (+18 health, +14 maximum); with d1 != 0 and health < maximum the
 * pending change +16 takes d1. The gas can, item kind 7 (record 0E58C2,
 * pickup 08B710 -> 0C0BA8 -> 0AAAE0 with d1 = 0x10): accepted by a ridden
 * SV-001 or Slugnoid below its maximum (no value, label 08C752 external, no
 * sound), else the record's value (1000 points, 08B850) and sound 1121.
 * The vehicle's update (0AA9E4, neo_vslot_damage) moves +16 into +18 and +4E.
 * Hits (0AAA32) take 0x10 each: the 0x30 of the SV-001/Slugnoid is 3 hits. */
#include <stdio.h>
#include <string.h>
#include "../src/neo_region.h"

#include "../src/neo_vslot.h"
static unsigned fails;
#define CHECK(c) do { if (!(c)) {fprintf(stderr,"%s:%d: %s\n",__FILE__,__LINE__,#c);fails++;} } while (0)
static unsigned sounds[8],nsounds,externals;
static void sound(void *ctx,uint16_t id) {(void)ctx;if (nsounds<8) sounds[nsounds++]=id;}
static void external(void *ctx,uint32_t r) {(void)ctx;CHECK(r==0x8c752u);externals++;}
static uint16_t rw(const uint8_t *b,unsigned o) {return (uint16_t)(b[o]<<8|b[o+1]);}
static void ww(uint8_t *b,unsigned o,uint16_t v) {b[o]=(uint8_t)(v>>8);b[o+1]=(uint8_t)v;}
/* One pickup of the gas can by player 1 riding a vehicle of `kind` (0: on
 * foot) with `health`/`max`. Returns true when accepted. */
static bool take(neo_vslots_t *v,uint8_t kind,uint16_t health,uint16_t max,neo_pickup_env_t *pk) {
    static marco_weapons_t weapons;static marco_weapon_player_t player;
    memset(v,0,sizeof *v);
    uint8_t *s=v->slot[0].b;
    if (kind) {ww(s,0,0x1234);s[0x21]=kind;ww(s,0x14,max);ww(s,0x18,health);}
    memset(pk,0,sizeof *pk);
    marco_weapons_new_game(&weapons);player.exists=true;marco_weapons_life(&weapons,0,&player);
    pk->weapons=&weapons;pk->player[0]=&player;pk->slot[0]=s;pk->mission=1;
    neo_genv_t e;memset(&e,0,sizeof e);e.pickup=pk;e.sound=sound;e.external=external;
    neo_gactor_t a;memset(&a,0,sizeof a);ga_sw(&a,0x6e,7);
    nsounds=0;externals=0;
    CHECK(neo_item_take(&a,&e,0));
    return pk->refuels==1;
}
int main(void) {
    /* 0AAAE0 kinds and the refill rule. */
    for (unsigned k=0;k<0x10;k++) {
        uint8_t s[NEO_VSLOT_BYTES]={0};ww(s,0,1);s[0x21]=(uint8_t)k;ww(s,0x14,0x30);ww(s,0x18,0x20);
        uint16_t h,m;bool g=neo_vslot_gauge(s,0,&h,&m);
        CHECK(g==(k==1||k==3||k==4||(k>=8&&k<=0xb)));
        if (g) {CHECK(h==0x20 && m==0x30);}
        CHECK(rw(s,0x16)==0);
    }
    {uint8_t s[NEO_VSLOT_BYTES]={0};s[0x21]=1;ww(s,0x14,0x30);uint16_t h,m;CHECK(!neo_vslot_gauge(s,0,&h,&m));}   /* unbound slot */
    {uint8_t s[NEO_VSLOT_BYTES]={0};ww(s,0,1);s[0x21]=3;ww(s,0x14,0x30);ww(s,0x18,0x30);uint16_t h,m;
     CHECK(neo_vslot_gauge(s,0,&h,&m));CHECK(!neo_vslot_gauge(s,0x10,&h,&m));CHECK(rw(s,0x16)==0);}           /* full: refused */
    /* The gas can. */
    neo_vslots_t v;neo_pickup_env_t pk;
    CHECK(take(&v,1,0x20,0x30,&pk));                                  /* SV-001 after one hit */
    CHECK(rw(v.slot[0].b,0x16)==0x10 && pk.points[0]==0 && externals==1 && nsounds==0 && pk.labels==1);
    CHECK(!memcmp(pk.score[0],"\0\0\0\0",4) && pk.taken==1 && pk.item_taken[7]==1);
    {uint8_t veh[0xc0]={0};CHECK(!neo_vslot_damage(&v,0,veh,false));   /* the vehicle's next update */
     CHECK(rw(v.slot[0].b,0x18)==0x30 && rw(v.slot[0].b,0x16)==0 && rw(veh,0x4e)==0x30);
     /* Three hits (cause 0) empty the SV-001: 30 -> 20 -> 10 -> 0, +5F bit 7. */
     for (unsigned i=0;i<3;i++) {veh[0x5a]=1;neo_vslot_damage(&v,0,veh,true);}
     CHECK(rw(v.slot[0].b,0x18)==0 && (veh[0x5f]&0x80u));}
    CHECK(take(&v,3,0x08,0x30,&pk) && rw(v.slot[0].b,0x16)==0x10);   /* Slugnoid */
    static const struct {uint8_t kind;uint16_t health,max;} refused[]={{1,0x30,0x30},{0,0,0},{2,0,0},{1,0x38,0x30}};
    for (unsigned i=0;i<sizeof refused/sizeof refused[0];i++) {
        CHECK(!take(&v,refused[i].kind,refused[i].health,refused[i].max,&pk));
        CHECK(rw(v.slot[0].b,0x16)==0 && pk.points[0]==0x1000u && nsounds==1 && sounds[0]==0x1121u && externals==0);
        CHECK(pk.score[0][2]==0x10 && pk.score[0][3]==0 && pk.popups==1);   /* 1000 points (record +0C, 08B850) */
    }
    if (fails) {fprintf(stderr,"FAIL: %u checks\n",fails);return 1;}
    printf("PASS: slug gauge 0AAAE0 and gas can 08B710 (kind 7): refill 0x10 below the maximum, else 1000 points and sound 1121\n");
    return 0;
}
