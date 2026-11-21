/* Host test of the gameplay sound rule (src/msx_sfx.c, the Z80 driver's
 * groups, priorities and stop lists) against the measured pairs of
 * data/sound_policy.json (build/sound_pairs_fixture.h). No SDK, no assets. */
#include <assert.h>
#include <stdio.h>
#include "../src/msx_sfx.h"
#include "sound_pairs_fixture.h"
/* Not translated (docs/AUDIO_JUEGO.md 19): the chip channel allocation
 * 4A2A/4422 drops the FM/SSG effects 109F, 10A3 and 10A4 after effects of
 * other groups (the rule overlaps them); and the two measured cuts
 * 1159 > 1032 and 115B > 1033 across groups 06/01 (no stop list, unexplained).
 * And the port plays an effect of group 0 (109B, 10BA) on one channel: a
 * second request restarts it where the driver takes a second slot. */
static bool known(uint16_t a,uint16_t b,char measured,char rule) {
    if (a==b && measured=='o' && rule=='c' && !msx_sfx_defs[msx_sfx_lookup(a)].group) return true;
    if (measured=='d' && rule=='o' && (b==0x109f || b==0x10a3 || b==0x10a4)) return true;
    if (measured=='c' && rule=='o' && ((a==0x1159 && b==0x1032) || (a==0x115b && b==0x1033))) return true;
    return false;
}
static char pair(uint16_t a,uint16_t b,msx_sfx_state_t *s) {
    msx_sfx_reset(s);
    int ca=msx_sfx_request(s,a),ia=msx_sfx_lookup(a);assert(ca>=0);
    int cb=msx_sfx_request(s,b);
    if (cb<0) return 'd';
    if (a==b) return 'c';
    return s->current[ca]==ia?'o':'c';
}
int main(void) {
    msx_sfx_state_t s;msx_sfx_reset(&s);
    // Table: unique words, valid channels, one channel per group.
    for (int i=0;i<MSX_SFX_COUNT;i++) {
        assert(msx_sfx_defs[i].name && msx_sfx_defs[i].channel<MSX_SFX_CHANNELS);
        assert(msx_sfx_lookup(msx_sfx_defs[i].word)==i);
        for (int j=0;j<i;j++) assert((msx_sfx_defs[i].channel==msx_sfx_defs[j].channel)==(msx_sfx_defs[i].group && msx_sfx_defs[i].group==msx_sfx_defs[j].group));
    }
    assert(msx_sfx_lookup(0x1026)<0 && msx_sfx_lookup(0x0021)<0); // 1026 left out of the policy (AUDIO_JUEGO §13) / music: not effects
    assert(msx_sfx_request(&s,0x1026)==-1 && s.unmapped==1);
    // The measured pairs.
    unsigned n=sizeof sound_pairs/sizeof sound_pairs[0],same=0,deviations=0,skipped=0;
    for (unsigned i=0;i<n;i++) {
        uint16_t a=sound_pairs[i].first,b=sound_pairs[i].second;
        if (msx_sfx_lookup(a)<0 || msx_sfx_lookup(b)<0) {skipped++;continue;}
        char r=pair(a,b,&s),m=sound_pairs[i].outcome;
        if (r==m) same++;
        else if (known(a,b,m,r)) deviations++;
        else {printf("FAIL: %04X then %04X measured %c, rule %c\n",(unsigned)a,(unsigned)b,m,r);return 1;}
    }
    // B75 (AUDIO_JUEGO.md 19): 108A/108B (group 07, 0x65) drop 108F..1092
    // (0x6E) as in the driver, and 108F is cut by them; 116E the same.
    for (uint16_t w=0x108f;w<=0x1092;w++) {
        assert(pair(0x108a,w,&s)=='d' && pair(w,0x108a,&s)=='c');
        assert(pair(0x108b,w,&s)=='d' && pair(0x116e,w,&s)=='d' && pair(w,0x116e,&s)=='c');
    }
    assert(pair(0x108e,0x108a,&s)=='d' && pair(0x108a,0x108e,&s)=='c');   // 108E (0x64) over 108A (0x65)
    assert(pair(0x1087,0x1123,&s)=='c' && pair(0x1123,0x1087,&s)=='c');   // Tarma's and Marco's voices: group 02, 0x01
    // Stop lists (5204): 1022 stops 1064, 1034 stops 10E9 (other groups).
    msx_sfx_reset(&s);int c64=msx_sfx_request(&s,0x1064);int c22=msx_sfx_request(&s,0x1022);
    assert(c64>=0 && c22>=0 && (s.stop_mask&(1u<<c64)) && s.stopped==1);
    msx_sfx_reset(&s);int ce9=msx_sfx_request(&s,0x10e9);assert(msx_sfx_request(&s,0x1034)>=0 && (s.stop_mask&(1u<<ce9)) && s.current[ce9]==-1);
    // A finished channel takes any effect again.
    msx_sfx_reset(&s);assert(msx_sfx_request(&s,0x1041)>=0);int cv=msx_sfx_defs[msx_sfx_lookup(0x1041)].channel;
    msx_sfx_finished(&s,(unsigned)cv);assert(s.current[cv]==-1 && msx_sfx_request(&s,0x104f)==cv);
    printf("PASS: sound rule of the Z80 driver (%u effects, %u channels): %u measured pairs equal, %u known deviations (chip channels 4422, two cross-group cuts), %u pairs of unpacked effects\n",
        (unsigned)MSX_SFX_COUNT,(unsigned)MSX_SFX_CHANNELS,same,deviations,skipped);
    return 0;
}
