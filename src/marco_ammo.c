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
