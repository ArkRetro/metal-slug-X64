#ifndef MSX_CAMPAIGN_FOOTPRINT_RENDERER_H
#define MSX_CAMPAIGN_FOOTPRINT_RENDERER_H
#include <stdbool.h>
#include <stdint.h>

/* Original B53BE replacement tile plus exact lower-layer reconstruction.
 * Caller owns B5870 guards, active map window and live edit retention. Draw
 * retained edits after scenery, before actors. Every touched rectangle is
 * 16x16; no whole-map upload occurs. This invalidates scene TMEM/TLUT caches.
 * source_row is 7-(signed actor Y word ASR4); column is actor X word ASR4.
 * Coordinates outside the original 72x5 source map are successful no-ops,
 * matching the source C70FC clipping return. Missing resources fail. */
bool campaign_footprint_renderer_load(void);
/* Decode one original foreground edit for a persistent scene replacement.
 * Transparent pens overwrite old pixels. Source no-ops set changed=false. */
bool campaign_footprint_pixels(int column,int source_row,uint16_t rgba[256],bool *changed);
void campaign_footprint_renderer_close(void); /* Wait for pending RDP first. */
bool campaign_footprint_renderer_draw_stamp(int column,int source_row,unsigned camera,bool depth);
unsigned campaign_footprint_renderer_max_upload(void);
#endif
