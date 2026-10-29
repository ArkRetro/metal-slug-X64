#include "neo_rng.h"
#include <assert.h>

static uint16_t rol1(uint16_t value) {
    return (uint16_t)(((uint32_t)value<<1)|(value>>15));
}
uint16_t neo_rng_seed(neo_rng_t *rng,uint16_t request,uint16_t *counter) {
    assert(rng && counter);
    uint16_t d0=0;
    if (request) { *counter=(uint16_t)(*counter+1u);d0=*counter; } // 00297C..00298A
    uint16_t d1=d0,d2=d0; // 002998..00299A: initialized once, outside DBF loop.
    for (unsigned i=0;i<NEO_RNG_WORDS;i++) {
        d1=(uint16_t)(d1+0x22b7u);d2=(uint16_t)(d2+0x4555u); // 00299C/0029A0
        rng->words[i]=d0; // 0029A4
        unsigned carry=d0>>15;d0=(uint16_t)((uint32_t)d0<<1); // ADD.W, not signed C.
        if (carry) d0=(uint16_t)~d0; // 0029A8/0029AC
        d1=rol1(d1^d0);d2=rol1(d2^d1);d0=rol1(d0^d2); // 0029AE..0029B8
    }
    rng->cursor=NEO_RNG_WORDS-1; // 0029BE/0029C0: points at last stored word.
    return d0;
}
neo_rng_result_t neo_rng_next_result(neo_rng_t *rng) {
    assert(rng && rng->cursor<NEO_RNG_WORDS);
    uint16_t result=rng->words[rng->cursor]; // 0029D2
    unsigned next=rng->cursor+1u;
    if (next==NEO_RNG_WORDS) next=0; // 0029D4..0029DE
    rng->cursor=(uint8_t)next;
    unsigned target=next+23u; // 0029E0: 0x2E bytes, i.e. 23 words.
    if (target>=NEO_RNG_WORDS) target-=NEO_RNG_WORDS; // 0029E4..0029EA
    rng->words[target]^=result; // 0029EE
    return (neo_rng_result_t){result,rng->words[target]};
}
uint16_t neo_rng_next(neo_rng_t *rng) {
    return neo_rng_next_result(rng).value;
}
