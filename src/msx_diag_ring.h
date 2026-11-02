#ifndef MSX_DIAG_RING_H
#define MSX_DIAG_RING_H
/* Fixed-size ring of the most recent diagnostic events (msx_diag.h). Pure
 * C, no allocation: the newest MSX_DIAG_RING entries are kept, older ones
 * are overwritten and only counted. Host test: tools/test_msx_diag.c. */
#include <stdint.h>
enum { MSX_DIAG_RING=128 };   /* power of two */
typedef struct {
    uint32_t frame;   /* presented frame number when it happened */
    uint8_t type;     /* msx_diag_event_t */
    uint8_t a;        /* small argument: kind, pool, stage... */
    uint16_t b;       /* 16-bit argument: count, camera... */
    uint32_t c;       /* 32-bit argument: routine address, constructor... */
} msx_diag_entry_t;
typedef struct {
    msx_diag_entry_t e[MSX_DIAG_RING];
    uint32_t total;   /* entries ever pushed */
} msx_diag_ring_t;
_Static_assert((MSX_DIAG_RING&(MSX_DIAG_RING-1))==0,"ring size is a power of two");
static inline void msx_diag_ring_push(msx_diag_ring_t *r,msx_diag_entry_t e) {
    r->e[r->total&(MSX_DIAG_RING-1u)]=e;r->total++;
}
/* Entries held (at most MSX_DIAG_RING) and entries overwritten. */
static inline unsigned msx_diag_ring_count(const msx_diag_ring_t *r) {
    return r->total<MSX_DIAG_RING?(unsigned)r->total:MSX_DIAG_RING;
}
static inline uint32_t msx_diag_ring_lost(const msx_diag_ring_t *r) {
    return r->total-msx_diag_ring_count(r);
}
/* i-th held entry, 0 = oldest. */
static inline const msx_diag_entry_t *msx_diag_ring_at(const msx_diag_ring_t *r,unsigned i) {
    return &r->e[(r->total-msx_diag_ring_count(r)+i)&(MSX_DIAG_RING-1u)];
}
#endif
