/* Host check for src/scene_strips.c (B47): for the Mission 1 map and random
 * mission-like maps (tall pages, merged columns, animated cells, columns
 * that may hide cells), the items a strip yields for a screen window draw
 * exactly the texels the renderer's direct walk draws (draw_layer_direct in
 * scene_renderer.c, reproduced here): same (page, palette, texel) under
 * every on-screen cell, nothing more, in every video phase; and the strip
 * stays valid while the window is inside its margins. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/scene_strips.h"
#include "scene_meta.h"
int8_t scene_cell_patterns[SCENE_CELL_PATTERNS][8];
static const scene_image_t *images;static unsigned image_count;
static const scene_image_t *image(unsigned id) { if (id>=image_count) {fprintf(stderr,"image %u out of range\n",id);exit(1);} return &images[id]; }
static int hide_x0=1,hide_x1=0,hide_y0,hide_y1;   /* cells inside are hidden; columns inside [x0,x1) may hide */
static bool may_hide(const scene_map_cell_t *c) { return c->x>=hide_x0 && c->x<hide_x1; }
static bool hidden(const scene_map_cell_t *c) { return may_hide(c) && c->y>=hide_y0 && c->y<hide_y1; }
/* A drawn 16-row slice: what pixels of cell (x,y) show. */
typedef struct { int16_t x,y; uint16_t page,palette,s,t; } slice_t;
static slice_t *out;static unsigned outs,out_cap;
static void draw(const scene_image_t *im,int x,int y,unsigned n,int offset,int rise) {
    int sx=x-offset,sy=8+y+rise;
    if (sx+16<=8 || sx>=312 || sy+16*(int)n<=8 || sy>=232) return;
    for (unsigned i=0;i<n;i++) {
        if (sy+16*(int)i+16<=8 || sy+16*(int)i>=232) continue;   /* rows off screen: no pixels */
        if (outs==out_cap) {out_cap=out_cap?out_cap*2:1024;out=realloc(out,out_cap*sizeof(*out));}
        out[outs++]=(slice_t){(int16_t)x,(int16_t)(y+16*(int)i),im->page,im->palette,im->x,(uint16_t)(im->y+16*i)};
    }
}
static unsigned column_run(const scene_map_cell_t *cells,const uint16_t *order,unsigned k,unsigned end,unsigned tick) {
    const scene_map_cell_t *c=&cells[order[k]];const scene_image_t *im=image(scene_cell_phase(c,tick));unsigned n=1;
    if (im->format!=2) return 1;
    while (k+n<end && n<16) {
        const scene_map_cell_t *d=&cells[order[k+n]];const scene_image_t *jm=image(scene_cell_phase(d,tick));
        if (d->layer!=c->layer || d->x!=c->x || d->y!=c->y+16*(int)n || jm->page!=im->page || jm->palette!=im->palette || jm->x!=im->x || jm->y!=im->y+16*(int)n) break;
        n++;
    }
    return n;
}
static void direct(const scene_cell_index_t *ix,unsigned layer,int offset,int rise,unsigned tick) {
    const scene_map_cell_t *cells=ix->cells;scene_cell_window_t w={1.0f,(float)-offset,16,8,312,false};
    for (unsigned r=ix->layer_runs[layer];r<ix->layer_runs[layer+1];r++) {
        unsigned first,end;scene_cell_run_window(ix,r,&w,&first,&end);
        for (unsigned k=first;k<end;) {
            int sy=8+cells[ix->order[k]].y+rise;
            if (sy+16<=8) {k=scene_cell_column_from(ix,k,scene_cell_column_end(ix,k,end),-rise-15);continue;}
            if (sy>=232) {k=scene_cell_column_end(ix,k,end);continue;}
            unsigned n=column_run(cells,ix->order,k,end,tick);
            if (may_hide(&cells[ix->order[k]])) {
                for (unsigned j=0;j<n;j++) {const scene_map_cell_t *c=&cells[ix->order[k+j]];if (!hidden(c)) draw(image(scene_cell_phase(c,tick)),c->x,c->y,1,offset,rise);}
                k+=n;continue;
            }
            const scene_map_cell_t *c=&cells[ix->order[k]];draw(image(scene_cell_phase(c,tick)),c->x,c->y,n,offset,rise);k+=n;
        }
    }
}
static void strip(const scene_strip_pool_t *pool,const scene_strip_t *s,const scene_cell_index_t *ix,int offset,int rise,unsigned tick) {
    scene_cell_window_t w={1.0f,(float)-offset,16,8,312,false};
    for (unsigned g=s->first_group;g<s->first_group+s->groups;g++) {
        unsigned first,end;
        if (!scene_strip_group_window(pool,&pool->groups[g],&w,&first,&end)) continue;
        for (unsigned k=first;k<end;k++) {
            const scene_strip_item_t *it=&pool->items[k];
            if (it->anim==SCENE_STRIP_CELL) {const scene_map_cell_t *c=&ix->cells[it->image];if (!hidden(c)) draw(image(scene_cell_phase(c,tick)),c->x,c->y,1,offset,rise);continue;}
            unsigned id=it->anim==SCENE_CELL_STATIC?it->image:(unsigned)(it->image+scene_cell_patterns[it->anim][tick&7]);
            draw(image(id),it->x,it->y,it->n,offset,rise);
        }
    }
}
static int cmp(const void *a,const void *b) { return memcmp(a,b,sizeof(slice_t)); }
static unsigned fails,checks;
static void compare(const char *what,const scene_cell_index_t *ix,unsigned layer,int offset,int rise,unsigned tick,const scene_strip_pool_t *pool,const scene_strip_t *s) {
    outs=0;direct(ix,layer,offset,rise,tick);unsigned n=outs;
    strip(pool,s,ix,offset,rise,tick);unsigned m=outs-n;checks++;
    bool same=n==m;
    if (same) {qsort(out,n,sizeof(slice_t),cmp);qsort(out+n,m,sizeof(slice_t),cmp);same=!memcmp(out,out+n,n*sizeof(slice_t));}
    /* The screen never shows the same cell twice: a duplicate slice would mean overlapping draws. */
    for (unsigned i=1;same && i<n;i++) if (out[i].x==out[i-1].x && out[i].y==out[i-1].y) same=false;
    if (!same && fails++<10) fprintf(stderr,"scene_strips mismatch: %s layer=%u offset=%d rise=%d tick=%u direct=%u strip=%u\n",what,layer,offset,rise,tick,n,m);
}
int main(void) {
    static scene_strip_item_t items[4096];static scene_strip_group_t groups[1024];
    scene_strip_pool_t pool={items,groups,4096,1024,0,0};scene_strip_env_t env={image,may_hide};
    /* Mission 1 map: every camera, both strip rebuild policies. */
    images=scene_map_images;image_count=sizeof(scene_map_images)/sizeof(*scene_map_images);
    memcpy(scene_cell_patterns,(int8_t[SCENE_CELL_PATTERNS][8])SCENE_CELL_PATTERNS_INIT,sizeof(scene_cell_patterns));
    scene_cell_index_t ix;
    if (!scene_cell_index_build(&ix,scene_map_cells_data,SCENE_MAP_CELL_COUNT)) {fprintf(stderr,"index build failed\n");return 1;}
    for (unsigned layer=0;layer<5;layer++) {
        scene_strip_t s={0};
        for (int camera=-64;camera<=SCENE_MAP_CAMERA_MAX+64;camera+=3) {
            int offset=camera*(int)(layer+1)/4,rise=(camera/7)%40-20;   /* a parallax and a height per layer */
            int wx0=offset+8,wx1=offset+312,wy0=-rise,wy1=224-rise;
            if (!scene_strip_covers(&s,wx0,wx1,wy0,wy1)) {
                pool.items_used=pool.groups_used=0;
                if (!scene_strip_build(&s,&pool,&ix,layer,wx0-96,wx1+96,wy0-96,wy1+96,&env)) {fprintf(stderr,"mission 1 strip did not fit\n");return 1;}
            }
            compare("mission 1",&ix,layer,offset,rise,(unsigned)camera,&pool,&s);
        }
    }
    free(ix.order);free(ix.runs);
    /* Random mission-like maps: tall pages (16 tiles each), columns of
     * consecutive codes, animated cells (patterns of image deltas), a
     * replaced column range, and pack order (layer, page, x, y). */
    srand(4321);
    for (unsigned trial=0;trial<200;trial++) {
        unsigned pages=4+rand()%12,n_images=pages*16;scene_image_t *im=calloc(n_images,sizeof(*im));
        for (unsigned i=0;i<n_images;i++) im[i]=(scene_image_t){.page=(uint16_t)(i/16),.palette=(uint16_t)(i/16%3),.x=0,.y=(int16_t)(16*(i%16)),.width=16,.height=256,.format=2};
        images=im;image_count=n_images;
        for (unsigned p=0;p<SCENE_CELL_PATTERNS;p++) for (unsigned k=0;k<8;k++) scene_cell_patterns[p][k]=(int8_t)(p?(rand()%3)-1:0);
        unsigned columns=20+rand()%60,rows=4+rand()%40,n=0,cap=5*columns*rows;scene_map_cell_t *cells=malloc(cap*sizeof(*cells));
        for (unsigned layer=0;layer<5;layer++) for (unsigned c=0;c<columns;c++) {
            if (rand()%4==0) continue;   /* gaps */
            unsigned code=(unsigned)rand()%n_images;bool anim=rand()%6==0;
            for (unsigned r=0;r<rows;r++) {
                if (rand()%7==0) continue;
                uint16_t phase=(uint16_t)((code+r)%n_images);
                if (anim) phase=(uint16_t)(1+rand()%(n_images-2));
                cells[n++]=(scene_map_cell_t){.layer=(uint8_t)layer,.anim=anim?(uint8_t)(1+rand()%(SCENE_CELL_PATTERNS-1)):SCENE_CELL_STATIC,.x=(int16_t)(16*(int)c-64),.y=(int16_t)(16*(int)r-48),.phase=phase};
            }
        }
        /* Pack order: layer, page of the first phase, x, y. */
        for (unsigned i=1;i<n;i++) for (unsigned j=i;j>0;j--) {
            const scene_map_cell_t *a=&cells[j-1],*b=&cells[j];
            long ka=((long)a->layer<<40)|((long)im[a->phase].page<<24)|((long)(a->x+32768)<<16)|(a->y+32768),kb=((long)b->layer<<40)|((long)im[b->phase].page<<24)|((long)(b->x+32768)<<16)|(b->y+32768);
            if (ka<=kb) break;
            scene_map_cell_t t=cells[j-1];cells[j-1]=cells[j];cells[j]=t;
        }
        scene_cell_index_t r;if (!scene_cell_index_build(&r,cells,n)) {fprintf(stderr,"random build failed\n");return 1;}
        hide_x0=16*(rand()%columns)-64;hide_x1=hide_x0+16*(1+rand()%5);hide_y0=16*(rand()%rows)-48;hide_y1=hide_y0+16*(1+rand()%6);
        if (trial%3==0) hide_x1=hide_x0;   /* nothing replaced */
        for (unsigned layer=0;layer<5;layer++) {
            scene_strip_t s={0};pool.items_used=pool.groups_used=0;
            int offset=rand()%(16*(int)columns)-200,rise=rand()%300-150;
            for (unsigned step=0;step<60;step++) {
                int wx0=offset+8,wx1=offset+312,wy0=-rise,wy1=224-rise;
                if (!scene_strip_covers(&s,wx0,wx1,wy0,wy1)) {
                    pool.items_used=pool.groups_used=0;int margin=rand()%3==0?0:96;
                    if (!scene_strip_build(&s,&pool,&r,layer,wx0-margin,wx1+margin,wy0-margin,wy1+margin,&env)) {fprintf(stderr,"random strip did not fit (trial %u)\n",trial);return 1;}
                }
                compare("random",&r,layer,offset,rise,(unsigned)rand(),&pool,&s);
                offset+=rand()%9-4;rise+=rand()%9-4;
            }
        }
        free(r.order);free(r.runs);free(cells);free(im);
    }
    if (fails) {fprintf(stderr,"scene_strips: %u mismatches in %u checks\n",fails,checks);return 1;}
    printf("scene_strips: %u windows (Mission 1 cameras and 200 random maps) draw the texels of the direct walk\n",checks);
    return 0;
}
