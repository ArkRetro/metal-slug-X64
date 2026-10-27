#include <assert.h>
#include <stdio.h>
#include "../src/neo_soldier.h"
#include "soldier_fixture.h"
int main(void) {
    neo_soldier_motion_seed_t seed=neo_soldier_seed_motion(
        SOLDIER_RNG0_LOW16,SOLDIER_RNG1_LOW16,SOLDIER_FLAGS15,SOLDIER_FIELD92);
    assert(seed.value_62==SOLDIER_EXPECTED_SIGNED_62);
    assert(seed.value_26==SOLDIER_EXPECTED_WORD_26);
    puts("PASS: native AF65A arithmetic equals captured M68000 RNG inputs and RAM outputs; complete AI remains unported");
}
