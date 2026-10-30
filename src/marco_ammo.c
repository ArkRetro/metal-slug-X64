#include "marco_ammo.h"
bool marco_ammo_reserve(marco_ammo_t *s,uint8_t kind,bool unlimited,bool special) {
    if(!s || kind!=s->kind) return false;
    if(!unlimited && !special) {
        if(!s->ammunition) return false;
        if(s->ammunition!=UINT16_MAX) --s->ammunition;
    }
    if(s->cap==UINT8_MAX) return true;
    if(s->active>=s->cap) return false;
    ++s->active;return true;
}
void marco_ammo_release(marco_ammo_t *s,uint8_t kind) {
    if(s && s->kind==kind && s->active) --s->active;
}
/* Weapon tables 0C08A8 / 0C08B8 / 0C08D8 / 0C08F8 / 0C0918, by id. */
const uint8_t marco_weapon_cap[MARCO_WEAPON_IDS]={0xff,4,1,2,1,0xff,0xff,1,2,1,0xff,0xff,3,2,3,2};
const uint16_t marco_weapon_rounds[MARCO_WEAPON_IDS]={0xffff,8,30,30,30,200,200,15,30,30,200,200,40,30,30,20};
const uint16_t marco_weapon_refill[MARCO_WEAPON_IDS]={0,30,10,20,15,150,150,10,20,15,150,150,20,10,10,5};
const uint16_t marco_weapon_sound[MARCO_WEAPON_IDS]={0,0,0x112f,0x112e,0x112c,0x112d,0x1130,0x112f,0x112e,0x112c,0x112d,0x1130,0x1158,0x115a,0x115b,0x1159};
const uint8_t marco_weapon_form[MARCO_WEAPON_IDS]={0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
enum { W_FLIGHT=0, W_ACTIVE=2, W_CAP=4, W_ID=6, W_BOMBS=8, W_TYPE=10, W_ROUNDS=12 };
static void set_word(marco_weapons_t *w,unsigned o,uint16_t v) {w->b[o]=(uint8_t)(v>>8);w->b[o+1]=(uint8_t)v;}
static uint16_t word(const marco_weapons_t *w,unsigned o) {return (uint16_t)(w->b[o]<<8|w->b[o+1]);}
void marco_weapons_new_game(marco_weapons_t *w) {
    set_word(w,W_BOMBS,0);set_word(w,W_FLIGHT,0);set_word(w,W_ACTIVE,0);
    set_word(w,W_CAP,0xffff);set_word(w,W_ID,0);
}
void marco_weapons_pistol(marco_weapons_t *w,unsigned p,marco_weapon_player_t *m) {
    m->combo95=0;w->b[W_ID+p]=0;w->b[W_ACTIVE+p]=0;
    w->b[W_CAP+p]=marco_weapon_cap[0];set_word(w,W_ROUNDS+2*p,marco_weapon_rounds[0]);
}
void marco_weapons_death(marco_weapons_t *w,unsigned p) {
    w->b[W_FLIGHT+p]=0;w->b[W_ID+p]=0;w->b[W_BOMBS+p]=0;set_word(w,W_ROUNDS+2*p,0);
}
void marco_weapons_life(marco_weapons_t *w,unsigned p,marco_weapon_player_t *m) {
    m->form93=0;w->b[W_BOMBS+p]=10;
    marco_weapons_pistol(w,p,m);
    m->form94=0xff;m->combo95=0;w->b[W_TYPE+p]=1;
}
bool marco_weapons_grant(marco_weapons_t *w,unsigned p,uint8_t id,marco_weapon_player_t *m,uint16_t *sound) {
    if (sound) *sound=0;
    if (!m->exists || m->form93==3 || id>=MARCO_WEAPON_IDS) return false;
    m->combo95=0;
    uint8_t old=w->b[W_ID+p];w->b[W_ID+p]=id;
    m->form94=marco_weapon_form[id];w->b[W_CAP+p]=marco_weapon_cap[id];
    if (id!=old) {w->b[W_ACTIVE+p]=0;set_word(w,W_ROUNDS+2*p,marco_weapon_rounds[id]);}
    else {
        uint16_t v=(uint16_t)(marco_weapon_refill[id]+word(w,W_ROUNDS+2*p));   /* ADD.W wraps */
        set_word(w,W_ROUNDS+2*p,v<1000u?v:999u);
    }
    if (sound) *sound=marco_weapon_sound[id];
    return true;
}
/* 0AA898 with D2 = 0: a ridden vehicle (slot +00 != 0) of kind other than
 * 2, 5, 6, 7 takes the bombs into its +1F (99 at most) and returns D1 = its
 * bomb type +20; 0 otherwise. Kind 4 first reads the HUD routine 0C0764. */
static int vehicle_bombs(uint8_t *slot,uint8_t count) {
    if (!slot || !(slot[0]|slot[1])) return 0;
    uint8_t kind=slot[0x21];
    if (kind==2 || kind==5 || kind==6 || kind==7) return 0;
    if (kind==4) return -1;                                      /* 0C0764 */
    if (slot[0x1f]==0x63) return 0;
    unsigned v=(unsigned)count+slot[0x1f];
    slot[0x1f]=(uint8_t)(v>0xffu || (uint8_t)v>=0x64u?0x63u:v);
    return slot[0x20];
}
marco_weapon_result_t marco_weapons_bombs(marco_weapons_t *w,unsigned p,uint8_t count,const marco_weapon_player_t *m,
    uint8_t *slot,uint16_t *sound) {
    if (sound) *sound=0;
    if (!m->exists) return MW_REFUSED;
    int type=vehicle_bombs(slot,count);
    if (type<0) return MW_STOP;
    /* 0C0B60 tests the returned type, not the carry: a vehicle of bomb
     * type 0 that took them gives them to the player as well. */
    if (!type) {
        if (w->b[W_BOMBS+p]>=0x63u) return MW_REFUSED;
        uint8_t n=(uint8_t)(count+w->b[W_BOMBS+p]);
        w->b[W_BOMBS+p]=n<0x64u?n:0x63u;
    }
    if (sound) *sound=0x1124;
    return MW_ACCEPTED;
}
bool marco_weapons_expire(marco_weapons_t *w,unsigned p,marco_weapon_player_t *m) {
    if (word(w,W_ROUNDS+2*p) && !m->dead) return false;
    marco_weapons_pistol(w,p,m);
    if (m->form93==1) m->form94=0;
    return true;
}
bool marco_weapons_throw(marco_weapons_t *w,unsigned p,bool unlimited) {
    if (unlimited) return true;
    if (!w->b[W_BOMBS+p]) return false;
    w->b[W_BOMBS+p]--;return true;
}
bool marco_weapons_armed(marco_weapons_t *w,unsigned p) {
    if (p>=2) return false;
    w->b[W_FLIGHT+p]++;return true;
}
bool marco_weapons_landed(marco_weapons_t *w,unsigned p) {
    if (p>=2) return false;
    if (w->b[W_FLIGHT+p]) w->b[W_FLIGHT+p]--;
    return true;
}
marco_ammo_t marco_weapons_ammo(const marco_weapons_t *w,unsigned p) {
    marco_ammo_t a={word(w,W_ROUNDS+2*p),w->b[W_ACTIVE+p],w->b[W_CAP+p],w->b[W_ID+p]};return a;
}
void marco_weapons_set_ammo(marco_weapons_t *w,unsigned p,const marco_ammo_t *a) {
    set_word(w,W_ROUNDS+2*p,a->ammunition);w->b[W_ACTIVE+p]=a->active;w->b[W_CAP+p]=a->cap;w->b[W_ID+p]=a->kind;
}
