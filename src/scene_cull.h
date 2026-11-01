#ifndef MSX_SCENE_CULL_H
#define MSX_SCENE_CULL_H
/* Horizontal culling of map cells without visiting every cell (docs/RENDIMIENTO.md).
 * Cells of each layer are kept in their pack order and split into runs whose
 * x never decreases. Every cull test of the renderer is v=x*scale+offset
 * against fixed bounds, and v is monotonic in x (scale>0, IEEE rounding is
 * monotonic), so inside a run the cells that pass form one contiguous range
 * found by binary search with the very same float expression. Iterating the
 * runs in order visits exactly the cells the full scan would draw, in the
 * same order. Host-testable: no libdragon. */
#include <stdbool.h>
#include <stdint.h>
#include "scene_assets.h"
enum { SCENE_CULL_LAYERS=5 };
typedef struct { uint16_t first,end; } scene_cell_run_t; /* positions in order[] */
typedef struct {
    const scene_map_cell_t *cells;
    uint16_t *order;            /* cell indices grouped by layer, pack order inside each */
    scene_cell_run_t *runs;
    unsigned layer_runs[SCENE_CULL_LAYERS+1]; /* runs of layer l: [layer_runs[l],layer_runs[l+1]) */
    unsigned count,run_count;
    bool layer_major;           /* pack order == layer order: per-layer walks equal one array walk */
} scene_cell_index_t;
/* Window: a cell passes when (v+size > left, or >= when left_inclusive) and v < right. */
typedef struct { float scale,offset,size,left,right;bool left_inclusive; } scene_cell_window_t;
bool scene_cell_index_build(scene_cell_index_t *index,const scene_map_cell_t *cells,unsigned count);
/* Same index in caller memory (count entries of each): a mission stage's
 * index lives with its tables (scene_stage_load). */
bool scene_cell_index_build_into(scene_cell_index_t *index,const scene_map_cell_t *cells,unsigned count,uint16_t *order,scene_cell_run_t *runs);
static inline float scene_cell_value(const scene_cell_window_t *w,int16_t x) {return x*w->scale+w->offset;}
static inline bool scene_cell_left_ok(const scene_cell_window_t *w,int16_t x) {
    float v=scene_cell_value(w,x);
    return w->left_inclusive?v+w->size>=w->left:v+w->size>w->left;
}
static inline bool scene_cell_right_ok(const scene_cell_window_t *w,int16_t x) {return scene_cell_value(w,x)<w->right;}
/* Positions [*first,*end) of run r (in order[]) whose cells pass the window. */
static inline void scene_cell_run_window(const scene_cell_index_t *index,unsigned r,const scene_cell_window_t *w,unsigned *first,unsigned *end) {
    const scene_cell_run_t *run=&index->runs[r];
    unsigned lo=run->first,hi=run->end;
    while (lo<hi) { /* first position whose left test passes */
        unsigned mid=(lo+hi)/2;
        if (scene_cell_left_ok(w,index->cells[index->order[mid]].x)) hi=mid; else lo=mid+1;
    }
    unsigned start=lo;hi=run->end;
    while (lo<hi) { /* first position whose right test fails */
        unsigned mid=(lo+hi)/2;
        if (scene_cell_right_ok(w,index->cells[index->order[mid]].x)) lo=mid+1; else hi=mid;
    }
    *first=start;*end=lo;
}
/* Inside a run, the cells of one column (same x) are contiguous and sorted
 * by increasing y (scene_cell_index_build). End of the column at k: */
static inline unsigned scene_cell_column_end(const scene_cell_index_t *index,unsigned k,unsigned end) {
    int16_t x=index->cells[index->order[k]].x;unsigned lo=k+1,hi=end;
    while (lo<hi) {unsigned mid=(lo+hi)/2;if (index->cells[index->order[mid]].x==x) lo=mid+1; else hi=mid;}
    return lo;
}
/* First position in [k,end) (one column) whose y is >= y. */
static inline unsigned scene_cell_column_from(const scene_cell_index_t *index,unsigned k,unsigned end,int y) {
    unsigned lo=k,hi=end;
    while (lo<hi) {unsigned mid=(lo+hi)/2;if (index->cells[index->order[mid]].y<y) lo=mid+1; else hi=mid;}
    return lo;
}
#endif
