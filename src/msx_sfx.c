#include "msx_sfx.h"
/* Channel/priority evidence (pair renders, docs/AUDIO_JUEGO.md):
 *  WEAPON: 10F7 retriggers itself; 108D cuts 10F7; 10F7 is dropped while
 *          108D plays -> 10F7 < 108D.
 *  IMPACT: 1027 retriggers; 10FA cuts 1027, 1027 dropped under 10FA; 1023
 *          cuts 10FA and 1027, 10FA dropped under 1023; 1033 cuts 1023 and
 *          1027 -> 1027 < 10FA < 1023 <= 1033.
 *  VOICE:  1041 retriggers; 103E, 1040 and 1042 cut 1041 and 1041 cuts
 *          them (equal level); 103F and 1043 are cut by 1041/103E but are
 *          dropped while 1041 plays -> {103F,1043} < {103E,1040,1041,1042}.
 * Pairs without a measurement follow the same rule with these levels. */
const msx_sfx_def_t msx_sfx_defs[MSX_SFX_COUNT]={
    [MSX_SFX_PISTOL]={0x10f7,MSX_SFX_CH_WEAPON,0,"pistol"},
    [MSX_SFX_VEHICLE_HIT]={0x108d,MSX_SFX_CH_WEAPON,1,"vehicle_hit"},
    [MSX_SFX_EXPLOSION]={0x1027,MSX_SFX_CH_IMPACT,0,"explosion"},
    [MSX_SFX_KNIFE_HIT]={0x10fa,MSX_SFX_CH_IMPACT,1,"knife_hit"},
    [MSX_SFX_VEHICLE_SMOKE]={0x1023,MSX_SFX_CH_IMPACT,2,"vehicle_smoke"},
    [MSX_SFX_VEHICLE_BURST]={0x1033,MSX_SFX_CH_IMPACT,2,"vehicle_burst"},
    [MSX_SFX_REBEL_103E]={0x103e,MSX_SFX_CH_VOICE,1,"rebel_103e"},
    [MSX_SFX_REBEL_103F]={0x103f,MSX_SFX_CH_VOICE,0,"rebel_103f"},
    [MSX_SFX_REBEL_1040]={0x1040,MSX_SFX_CH_VOICE,1,"rebel_1040"},
    [MSX_SFX_REBEL_1041]={0x1041,MSX_SFX_CH_VOICE,1,"rebel_1041"},
    [MSX_SFX_REBEL_1042]={0x1042,MSX_SFX_CH_VOICE,1,"rebel_1042"},
    [MSX_SFX_REBEL_1043]={0x1043,MSX_SFX_CH_VOICE,0,"rebel_1043"},
    MSX_SFX_POLICY_DEFS
};
int msx_sfx_lookup(uint16_t word) {
    for (int i=0;i<MSX_SFX_COUNT;i++) if (msx_sfx_defs[i].word==word) return i;
    return -1;
}
void msx_sfx_reset(msx_sfx_state_t *s) {
    for (unsigned c=0;c<MSX_SFX_CHANNELS;c++) s->current[c]=-1;
    s->started=s->replaced=s->dropped=s->unmapped=0;
}
void msx_sfx_finished(msx_sfx_state_t *s,unsigned channel) {
    if (channel<MSX_SFX_CHANNELS) s->current[channel]=-1;
}
int msx_sfx_request(msx_sfx_state_t *s,uint16_t word) {
    int id=msx_sfx_lookup(word);
    if (id<0) {s->unmapped++;return -1;}
    const msx_sfx_def_t *d=&msx_sfx_defs[id];int playing=s->current[d->channel];
    if (playing>=0 && msx_sfx_defs[playing].priority>d->priority) {s->dropped++;return -1;}
    if (playing>=0) s->replaced++;
    s->current[d->channel]=(int8_t)id;s->started++;
    return d->channel;
}
