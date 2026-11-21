#include "msx_sfx.h"
/* The table: tools/compile_sound_policy.py (the driver's headers). The pair
 * renders of tools/measure_sound_pairs.py (data/sound_policy.json) check
 * the rule: tools/test_msx_sfx.c. */
const msx_sfx_def_t msx_sfx_defs[MSX_SFX_COUNT]={MSX_SFX_POLICY_DEFS};
int msx_sfx_lookup(uint16_t word) {
    for (int i=0;i<MSX_SFX_COUNT;i++) if (msx_sfx_defs[i].word==word) return i;
    return -1;
}
void msx_sfx_reset(msx_sfx_state_t *s) {
    for (unsigned c=0;c<MSX_SFX_CHANNELS;c++) s->current[c]=-1;
    s->stop_mask=0;s->started=s->replaced=s->dropped=s->unmapped=s->stopped=0;
}
void msx_sfx_finished(msx_sfx_state_t *s,unsigned channel) {
    if (channel<MSX_SFX_CHANNELS) s->current[channel]=-1;
}
int msx_sfx_request(msx_sfx_state_t *s,uint16_t word) {
    s->stop_mask=0;
    int id=msx_sfx_lookup(word);
    if (id<0) {s->unmapped++;return -1;}
    const msx_sfx_def_t *d=&msx_sfx_defs[id];
    /* 5204: the stop list. */
    for (unsigned k=0;k<4 && d->stops[k];k++)
        for (unsigned c=0;c<MSX_SFX_CHANNELS;c++)
            if (s->current[c]>=0 && msx_sfx_defs[s->current[c]].word==d->stops[k]) {s->current[c]=-1;s->stop_mask|=1u<<c;s->stopped++;}
    /* 50CF: the group's slot. */
    int playing=s->current[d->channel];
    if (playing>=0 && d->group && msx_sfx_defs[playing].priority<d->priority) {s->dropped++;return -1;}
    if (playing>=0) s->replaced++;
    s->current[d->channel]=(int8_t)id;s->started++;
    return d->channel;
}
