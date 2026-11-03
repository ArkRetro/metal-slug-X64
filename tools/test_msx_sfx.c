/* Host test of the gameplay sound policy (src/msx_sfx.c). No SDK, no assets. */
#include <assert.h>
#include <stdio.h>
#include "../src/msx_sfx.h"
int main(void) {
    msx_sfx_state_t s;msx_sfx_reset(&s);
    // Table: unique words, valid channels, names present.
    for (int i=0;i<MSX_SFX_COUNT;i++) {
        assert(msx_sfx_defs[i].name && msx_sfx_defs[i].channel<MSX_SFX_CHANNELS);
        assert(msx_sfx_lookup(msx_sfx_defs[i].word)==i);
    }
    assert(msx_sfx_lookup(0x1026)<0 && msx_sfx_lookup(0x0021)<0); // 1026 left out of the policy (AUDIO_JUEGO §13) / music: not effects
    // Unmapped request: ignored and counted.
    assert(msx_sfx_request(&s,0x1026)==-1 && s.unmapped==1);      // loop sound (render still active at the window end): not an effect
    // Same sound retriggers on its channel (pistol fire rate).
    assert(msx_sfx_request(&s,0x10f7)==MSX_SFX_CH_WEAPON);
    assert(msx_sfx_request(&s,0x10f7)==MSX_SFX_CH_WEAPON && s.replaced==1);
    // Different channels overlap: pistol, explosion and a rebel cry together.
    assert(msx_sfx_request(&s,0x1027)==MSX_SFX_CH_IMPACT);
    assert(msx_sfx_request(&s,0x1041)==MSX_SFX_CH_VOICE);
    assert(s.current[MSX_SFX_CH_WEAPON]==MSX_SFX_PISTOL && s.current[MSX_SFX_CH_IMPACT]==MSX_SFX_EXPLOSION);
    // Lower priority on a busy channel is dropped; equal/higher replaces.
    assert(msx_sfx_request(&s,0x103f)==-1 && s.dropped==1 && s.current[MSX_SFX_CH_VOICE]==MSX_SFX_REBEL_1041);
    assert(msx_sfx_request(&s,0x1042)==MSX_SFX_CH_VOICE && s.current[MSX_SFX_CH_VOICE]==MSX_SFX_REBEL_1042);
    assert(msx_sfx_request(&s,0x10fa)==MSX_SFX_CH_IMPACT);              // knife cuts explosion
    assert(msx_sfx_request(&s,0x1027)==-1);                            // explosion dropped under knife
    assert(msx_sfx_request(&s,0x1023)==MSX_SFX_CH_IMPACT);              // smoke cuts knife
    assert(msx_sfx_request(&s,0x10fa)==-1);                            // knife dropped under smoke
    assert(msx_sfx_request(&s,0x1033)==MSX_SFX_CH_IMPACT);              // burst cuts smoke
    assert(msx_sfx_request(&s,0x108d)==MSX_SFX_CH_WEAPON);              // hit on metal cuts pistol
    assert(msx_sfx_request(&s,0x10f7)==-1);                            // pistol dropped under it
    // A finished channel accepts any sound again.
    msx_sfx_finished(&s,MSX_SFX_CH_WEAPON);
    assert(msx_sfx_request(&s,0x10f7)==MSX_SFX_CH_WEAPON);
    msx_sfx_finished(&s,MSX_SFX_CH_VOICE);
    assert(msx_sfx_request(&s,0x103f)==MSX_SFX_CH_VOICE && msx_sfx_request(&s,0x1043)==MSX_SFX_CH_VOICE);
    msx_sfx_reset(&s);
    for (unsigned c=0;c<MSX_SFX_CHANNELS;c++) assert(s.current[c]==-1);
    puts("PASS: sound request map, channel retrigger/priority policy");
    return 0;
}
