#include "scene_strips.h"
#include <stddef.h>
/* Cells d and c (same x, d 16*n below c) draw as one rectangle when d's
 * texels follow c's in the same tall page (format 2) in every video phase:
 * the merge of column_run (scene_renderer.c) for any phase. */
static bool continues(const scene_map_cell_t *c,const scene_image_t *const *first,const scene_map_cell_t *d,unsigned n,const scene_strip_env_t *env) {
    if (d->x!=c->x || d->y!=c->y+16*(int)n || d->anim!=c->anim) return false;
    unsigned phases=c->anim==SCENE_CELL_STATIC?1:8;
    for (unsigned p=0;p<phases;p++) {
        const scene_image_t *im=first[p],*jm=env->image(scene_cell_phase(d,p));
        if (im->format!=2 || jm->page!=im->page || jm->palette!=im->palette || jm->x!=im->x || jm->y!=im->y+16*(int)n) return false;
    }
    return true;
}
bool scene_strip_build(scene_strip_t *s,scene_strip_pool_t *pool,const scene_cell_index_t *index,unsigned layer,int x0,int x1,int y0,int y1,const scene_strip_env_t *env) {
    const scene_map_cell_t *cells=index->cells;const uint16_t *order=index->order;
    unsigned items=pool->items_used,groups=pool->groups_used;
    *s=(scene_strip_t){.valid=false,.x0=x0,.x1=x1,.y0=y0,.y1=y1,.first_item=items,.first_group=groups};
    scene_cell_window_t w={1.0f,0.0f,16.0f,(float)x0,(float)x1,false};
    const scene_strip_item_t *prev=NULL;
    for (unsigned r=index->layer_runs[layer];r<index->layer_runs[layer+1];r++) {
        /* x never decreases inside a run: its first and last cells bound it
         * (two reads instead of the binary searches of most runs). */
        const scene_cell_run_t *run=&index->runs[r];
        if (!scene_cell_left_ok(&w,cells[order[run->end-1]].x) || !scene_cell_right_ok(&w,cells[order[run->first]].x)) continue;
        unsigned first,end;scene_cell_run_window(index,r,&w,&first,&end);
        for (unsigned k=first;k<end;) {
            const scene_map_cell_t *c=&cells[order[k]];
            if (c->y+16<=y0) {k=scene_cell_column_from(index,k,scene_cell_column_end(index,k,end),y0-15);continue;}
            if (c->y>=y1) {k=scene_cell_column_end(index,k,end);continue;}
            scene_strip_item_t item={c->x,c->y,c->phase,1,c->anim};
            if (env->may_hide && env->may_hide(c)) {item.image=order[k];item.anim=SCENE_STRIP_CELL;}
            else {
                /* The column's first images, one per phase (the merge test
                 * compares the following cells' images with them). */
                const scene_image_t *first[8];unsigned phases=c->anim==SCENE_CELL_STATIC?1:8;
                for (unsigned p=0;p<phases;p++) first[p]=env->image(scene_cell_phase(c,p));
                if (first[0]->format==2) while (k+item.n<end && item.n<SCENE_STRIP_MAX_CELLS) {
                    const scene_map_cell_t *d=&cells[order[k+item.n]];
                    if (d->y>=y1 || !continues(c,first,d,item.n,env)) break;
                    item.n++;
                }
            }
            if (items>=pool->item_capacity) return false;
            if (!prev || groups==s->first_group || item.x<prev->x || (item.x==prev->x && item.y<=prev->y)) {
                if (groups>=pool->group_capacity) return false;
                pool->groups[groups++]=(scene_strip_group_t){(uint16_t)items,(uint16_t)items,item.x,item.x};
            }
            scene_strip_group_t *g=&pool->groups[groups-1];
            pool->items[items]=item;g->end=(uint16_t)(items+1);
            if (item.x<g->xmin) g->xmin=item.x;
            if (item.x>g->xmax) g->xmax=item.x;
            prev=&pool->items[items++];k+=item.n;
        }
        prev=NULL;   /* a new run starts a new group (runs may restart at a smaller x) */
    }
    s->items=items-s->first_item;s->groups=groups-s->first_group;
    pool->items_used=items;pool->groups_used=groups;s->valid=true;
    return true;
}
