#ifndef MSX_SCENE_STRIPS_H
#define MSX_SCENE_STRIPS_H
/* Per-layer draw lists of the background (B47, docs/RENDIMIENTO.md).
 * The renderer used to walk the cell index every frame: for each run a
 * binary search, then every cell of the window (a random 8-byte read, its
 * image, the column merge of scene_renderer.c). In Mission 2 that walk cost
 * more than the RDP commands it produced (~2 us per cell visited: 4-5 ms a
 * frame with layers 2+3, 7 ms for the tall layer 0 of stage 3). A strip is
 * that walk done once for a window wider than the screen: its items are
 * the merged columns (position, image, cell count), grouped like the index
 * runs (x never decreases inside a group) so a frame finds the visible
 * items of a group with one binary search over a compact sequential array,
 * or skips the group by its x range. The strip is rebuilt when the camera
 * leaves its window or the replacement state changes. Host-testable: no
 * libdragon. */
#include <stdbool.h>
#include <stdint.h>
#include "scene_cull.h"
/* anim: SCENE_CELL_STATIC, a pattern index, or SCENE_STRIP_CELL (image is a
 * cell index: a column the renderer must check cell by cell for hidden cells). */
enum { SCENE_STRIP_CELL=254, SCENE_STRIP_MAX_CELLS=16 };
typedef struct { int16_t x,y; uint16_t image; uint8_t n,anim; } scene_strip_item_t;
typedef struct { uint16_t first,end; int16_t xmin,xmax; } scene_strip_group_t;
typedef struct {
    bool valid;int x0,x1,y0,y1;          /* cells with x+16>x0, x<x1, y+16>y0, y<y1 */
    unsigned first_item,items,first_group,groups;
} scene_strip_t;
typedef struct {
    scene_strip_item_t *items;scene_strip_group_t *groups;
    unsigned item_capacity,group_capacity,items_used,groups_used;
} scene_strip_pool_t;
typedef struct {
    const scene_image_t *(*image)(unsigned id);
    bool (*may_hide)(const scene_map_cell_t *cell);   /* NULL: no cell is ever hidden */
} scene_strip_env_t;
/* Items of `layer` for the window, in index order, appended to the pool.
 * false (strip invalid, pool unchanged) when the pool has no room. */
bool scene_strip_build(scene_strip_t *s,scene_strip_pool_t *pool,const scene_cell_index_t *index,unsigned layer,int x0,int x1,int y0,int y1,const scene_strip_env_t *env);
static inline bool scene_strip_covers(const scene_strip_t *s,int x0,int x1,int y0,int y1) {
    return s->valid && s->x0<=x0 && s->x1>=x1 && s->y0<=y0 && s->y1>=y1;
}
/* Positions [*first,*end) of the group's items that pass the window (the
 * same float test as scene_cell_run_window); false when none does. */
static inline bool scene_strip_group_window(const scene_strip_pool_t *pool,const scene_strip_group_t *g,const scene_cell_window_t *w,unsigned *first,unsigned *end) {
    if (!scene_cell_left_ok(w,g->xmax) || !scene_cell_right_ok(w,g->xmin)) return false;
    const scene_strip_item_t *items=pool->items;
    unsigned lo=g->first,hi=g->end;
    while (lo<hi) {unsigned mid=(lo+hi)/2;if (scene_cell_left_ok(w,items[mid].x)) hi=mid; else lo=mid+1;}
    unsigned start=lo;hi=g->end;
    while (lo<hi) {unsigned mid=(lo+hi)/2;if (scene_cell_right_ok(w,items[mid].x)) lo=mid+1; else hi=mid;}
    *first=start;*end=lo;return start<lo;
}
#endif
