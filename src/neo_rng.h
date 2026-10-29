#ifndef MSX_NEO_RNG_H
#define MSX_NEO_RNG_H
#include <stdint.h>
/* ROM 002976/0029C6. Native numerical state, no emulated RAM or CPU pointers.
 * 55 words correspond to RAM 100016..100082; cursor replaces RAM 100084. */
enum { NEO_RNG_WORDS=55 };
typedef struct { uint16_t words[NEO_RNG_WORDS]; uint8_t cursor; } neo_rng_t;
/* A zero request seeds with zero. Otherwise increment the caller's original
 * counter (RAM 10F006), modulo 16 bits, and seed with that value. Returns the
 * final low word of D0, independently recorded at ROM 0029C4. */
uint16_t neo_rng_seed(neo_rng_t *rng,uint16_t request,uint16_t *counter);
/* Final EOR.W flags describe mixed_word, not value (the word returned in D0).
 * Callers using BPL/BEQ must test mixed_word's sign/zero explicitly. */
typedef struct { uint16_t value,mixed_word; } neo_rng_result_t;
neo_rng_result_t neo_rng_next_result(neo_rng_t *rng);
uint16_t neo_rng_next(neo_rng_t *rng);
#endif
