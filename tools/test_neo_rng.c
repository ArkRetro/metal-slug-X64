#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/neo_rng.h"
#include "../src/neo_soldier.h"
#include "rng_fixture.h"
#ifdef N64
#include <libdragon.h>
#endif
#define COUNT(a) (sizeof(a)/sizeof((a)[0]))
int main(void) {
#ifdef N64
    debug_init_emulog();console_init();
#endif
    neo_rng_t rng;
    for (unsigned i=0;i<COUNT(rng_seeds);i++) {
        uint16_t counter=rng_seeds[i].counter_before;
        assert(neo_rng_seed(&rng,rng_seeds[i].request,&counter)==rng_seeds[i].d0);
        assert(counter==rng_seeds[i].counter_after);
        assert(rng.cursor==rng_seeds[i].cursor);
        assert(memcmp(rng.words,rng_seeds[i].words,sizeof(rng.words))==0);
    }
    memcpy(rng.words,rng_initial_words,sizeof(rng.words));rng.cursor=rng_initial_cursor;
    uint16_t expected[NEO_RNG_WORDS];memcpy(expected,rng_initial_words,sizeof(expected));
    unsigned initializers=0;
    for (unsigned i=0;i<COUNT(rng_steps);i++) {
        for (unsigned j=0;j<COUNT(rng_initializers);j++) if (rng_initializers[j].step==i) {
            neo_rng_t copy=rng;
            neo_soldier_motion_seed_t seed=neo_soldier_seed_from_rng(&copy,
                rng_initializers[j].flags15,(int16_t)rng_initializers[j].field92);
            assert(seed.value_62==rng_initializers[j].expected62);
            assert(seed.value_26==rng_initializers[j].expected26);initializers++;
        }
        assert(neo_rng_next(&rng)==rng_steps[i].value);
        if (rng_steps[i].index>=0) expected[(unsigned)rng_steps[i].index]=rng_steps[i].changed;
        assert(rng.cursor==rng_steps[i].cursor);
        assert(memcmp(rng.words,expected,sizeof(expected))==0);
    }
    assert(initializers==COUNT(rng_initializers));
    for (unsigned i=0;i<COUNT(soldier_decisions);i++)
        assert(neo_soldier_ground_select_prefix(soldier_decisions[i].before,
            &soldier_decisions[i].input)==soldier_decisions[i].after);
    for (unsigned i=0;i<COUNT(soldier_damping);i++)
        assert(neo_soldier_damp_motion(soldier_damping[i].before)==soldier_damping[i].after);
    printf("PASS: %u seed calls, %u RNG states, %u initializers, %u decisions, %u damping updates\n",
        (unsigned)COUNT(rng_seeds),(unsigned)COUNT(rng_steps),initializers,(unsigned)COUNT(soldier_decisions),(unsigned)COUNT(soldier_damping));
#ifdef N64
    debugf("MSX RNG REGRESSION PASS seeds=%u calls=%u initializers=%u decisions=%u damping=%u\n",
        (unsigned)COUNT(rng_seeds),(unsigned)COUNT(rng_steps),initializers,(unsigned)COUNT(soldier_decisions),(unsigned)COUNT(soldier_damping));
    for (;;) {wait_ms(100);}
#endif
}
