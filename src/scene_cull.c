#include "scene_cull.h"
#include <stdlib.h>
bool scene_cell_index_build(scene_cell_index_t *index,const scene_map_cell_t *cells,unsigned count) {
    *index=(scene_cell_index_t){.cells=cells,.count=count,.layer_major=true};
    if (count>UINT16_MAX) return false;
    index->order=malloc((count?count:1)*sizeof(*index->order));
    index->runs=malloc((count?count:1)*sizeof(*index->runs));
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
            if (at==0 || runs==index->layer_runs[layer] || cells[i].x<cells[index->order[at-1]].x)
                index->runs[runs++]=(scene_cell_run_t){(uint16_t)at,(uint16_t)at};
            index->order[at++]=(uint16_t)i;index->runs[runs-1].end=(uint16_t)at;
        }
    }
    index->layer_runs[SCENE_CULL_LAYERS]=runs;index->run_count=runs;
    return true;
}
