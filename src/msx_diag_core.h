#ifndef MSX_DIAG_CORE_H
#define MSX_DIAG_CORE_H
/* Internal state of the diagnostic mode (msx_diag.h), shared by the
 * portable core and the console side; MSX_DIAG builds only. */
#include "msx_diag.h"
#include "msx_diag_ring.h"
typedef struct {
    uint16_t used,peak,peak_window;
    uint32_t peak_frame,fails,fails_window;
} msx_diag_pool_stat_t;
typedef struct { unsigned count;uint32_t routine; } msx_diag_stop_stat_t;
typedef struct { uint32_t frame;int16_t x,y,vx;uint8_t flags,dyn; } msx_diag_trail_t;
typedef struct {
    msx_diag_ring_t ring;
    uint32_t frame;
    msx_diag_pool_stat_t pool[MSX_POOL_COUNT];
    msx_diag_stop_stat_t stop[MSX_STOP_COUNT];
    uint32_t dumped_stops;   /* stop kinds whose first occurrence was dumped */
    unsigned dumps;
    uint32_t dumped_world_routine;unsigned world_dumps;   /* world stops: one dump per distinct routine */
    msx_diag_trail_t trail[MSX_DIAG_TRAIL];
    unsigned trail_next,trail_count;
    bool fatal;              /* an assertion or exception is being reported */
} msx_diag_state_t;
extern msx_diag_state_t msx_diag;
extern const char *const msx_diag_pool_names[MSX_POOL_COUNT];
extern const uint16_t msx_diag_pool_capacity[MSX_POOL_COUNT];
extern const char *const msx_diag_stop_names[MSX_STOP_COUNT];
void msx_diag_dump_pools(void);
void msx_diag_dump_stops(void);
#endif
