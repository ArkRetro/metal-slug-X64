#ifndef MSX_MISSION_CAMERA_H
#define MSX_MISSION_CAMERA_H
#include <stdbool.h>
/*
 * Observed mission-1 layer transforms (docs/MISSION1.md, MISSION1_REST.md):
 * the camera itself is the native original (src/neo_camera.c); these rules
 * derive the layers from its displayed X and height.
 * Layers: 0 sky (1/2), 1 middle bank at 3/4, 2 same bank at full speed,
 * 3 foreground, 4 market props. Values are screen-pixel scroll offsets.
 */
enum {
    MSX_CAMERA_MAX=3520,MSX_REGION_CAMERA_MAX=1920,
    /* Both MAME traces reload the middle bank at full speed once camera X reaches 1023. */
    MSX_MID_FULL_SPEED_CAMERA=1023,
    /* Height starts rising at camera 944 and reaches 16 by 1008 in the jumping trace. */
    MSX_CAMERA_RISE_START=944,MSX_CAMERA_RISE_MAX=16
};
/*
 * Measured scroll-driven approximation: ceil((camera-944)/4) clamped to 0..16
 * matches all nine height changes of the jumping trace. The original routine
 * also follows the player (a no-jump trace stops at 14) and lifts the view
 * another 16 px later at camera 1920; neither is reproduced here.
 */
static inline int msx_camera_height(int camera) {
    if (camera<=MSX_CAMERA_RISE_START) return 0;
    int rise=(camera-MSX_CAMERA_RISE_START+3)/4;
    return rise>MSX_CAMERA_RISE_MAX?MSX_CAMERA_RISE_MAX:rise;
}
static inline bool msx_layer_visible(unsigned layer,int camera) {
    if (layer==1) return camera<MSX_MID_FULL_SPEED_CAMERA;
    if (layer==2) return camera>=MSX_MID_FULL_SPEED_CAMERA;
    return true;
}
/* Rounded fractions of the camera; the arcade accumulates 3/4 with +-1 px drift. */
static inline int msx_layer_scroll(unsigned layer,int value) {
    return layer==0?(value+1)/2:layer==1?(value*3+2)/4:value;
}
#endif
