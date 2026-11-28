#include "scene_cull.h"
#include <stdlib.h>
bool scene_cell_index_build(scene_cell_index_t *index,const scene_map_cell_t *cells,unsigned count) {
    uint16_t *order=malloc((count?count:1)*sizeof(*order));
    scene_cell_run_t *runs=malloc((count?count:1)*sizeof(*runs));
    return scene_cell_index_build_into(index,cells,count,order,runs);
}
bool scene_cell_index_build_into(scene_cell_index_t *index,const scene_map_cell_t *cells,unsigned count,uint16_t *order,scene_cell_run_t *run_memory) {
    *index=(scene_cell_index_t){.cells=cells,.count=count,.layer_major=true};
    if (count>UINT16_MAX) return false;
    index->order=order;index->runs=run_memory;
    if (!index->order || !index->runs) return false;
    for (unsigned i=0;i<count;i++) {
        if (cells[i].layer>=SCENE_CULL_LAYERS) return false;
        if (i && cells[i].layer<cells[i-1].layer) index->layer_major=false;
    }
    unsigned at=0,runs=0;
    for (unsigned layer=0;layer<SCENE_CULL_LAYERS;layer++) {
        index->layer_runs[layer]=runs;
        for (unsigned i=0;i<count;i++) {
            if (cells[i].layer!=layer) continue;
            const scene_map_cell_t *prev=at?&cells[index->order[at-1]]:NULL;
            /* A run also breaks where a column's y stops increasing: inside
             * a run every column (same x) is then sorted by y, which the
             * renderer's vertical skip (scene_cell_column_*) relies on. */
            if (at==0 || runs==index->layer_runs[layer] || cells[i].x<prev->x || (cells[i].x==prev->x && cells[i].y<=prev->y))
                index->runs[runs++]=(scene_cell_run_t){(uint16_t)at,(uint16_t)at};
            index->order[at++]=(uint16_t)i;index->runs[runs-1].end=(uint16_t)at;
        }
    }
    index->layer_runs[SCENE_CULL_LAYERS]=runs;index->run_count=runs;
    return true;
}
