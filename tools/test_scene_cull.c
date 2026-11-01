/* Host check for src/scene_cull.c: for every camera of the mission map and
 * both render modes, the run-indexed walks of scene_renderer.c visit exactly
 * the cells (and order) of the original full scans. Random windows over
 * random cell sets check the generic search. */
#include <stdio.h>
#include <stdlib.h>
#include "../src/scene_cull.h"
#include "../src/retro25d.h"
#include "../src/mission_camera.h"
#include "scene_meta.h"
static unsigned fails;
static void check(int ok,const char *what,int camera) {
    if (!ok && fails++<10) fprintf(stderr,"scene_cull mismatch: %s camera=%d\n",what,camera);
}
/* Original scene_renderer.c tests, verbatim in effect. */
static bool full_draw(bool depth,const retro_plane_view_t *v,int offset,const scene_map_cell_t *c) {
    if (!depth) {int x=c->x-offset;return !(x+16<=8 || x>=312);}
    float x=c->x*v->scale+v->offset_x;return !(x+16*v->scale<=8 || x>=312);
}
static bool full_prefetch(bool depth,const retro_plane_view_t *v,int offset,const scene_map_cell_t *c) {
    float x=depth?c->x*v->scale+v->offset_x:(float)(c->x-offset);return !(x< -48 || x>=352);
}
static scene_cell_window_t window(bool depth,const retro_plane_view_t *v,int offset,float size,float left,float right,bool inclusive) {
    if (depth) return (scene_cell_window_t){v->scale,v->offset_x,size,left,right,inclusive};
    return (scene_cell_window_t){1.0f,(float)-offset,size,left,right,inclusive};
}
static unsigned walk(const scene_cell_index_t *ix,unsigned layer,const scene_cell_window_t *w,uint16_t *out) {
    unsigned n=0;
    for (unsigned r=ix->layer_runs[layer];r<ix->layer_runs[layer+1];r++) {
        unsigned first,end;scene_cell_run_window(ix,r,w,&first,&end);
        for (unsigned k=first;k<end;k++) out[n++]=ix->order[k];
    }
    return n;
}
static void compare_camera(const scene_cell_index_t *ix,const scene_map_cell_t *cells,unsigned count,int camera,bool depth,uint16_t *a,uint16_t *b) {
    retro_camera_t cam={(float)camera,-(float)msx_camera_height(camera),0,320};
    for (unsigned layer=0;layer<5;layer++) {
        unsigned source=layer<4?layer:3;int offset=msx_layer_scroll(source,camera);
        retro_plane_view_t v;retro_layered_2d_projection(cam,layer,&v);
        for (unsigned pass=0;pass<2;pass++) {
            unsigned n=0;
            for (unsigned i=0;i<count;i++) if (cells[i].layer==layer &&
                (pass?full_draw(depth,&v,offset,&cells[i]):full_prefetch(depth,&v,offset,&cells[i]))) a[n++]=(uint16_t)i;
            scene_cell_window_t w=pass?window(depth,&v,offset,depth?16*v.scale:16,8,312,false):window(depth,&v,offset,0,-48,352,true);
            unsigned m=walk(ix,layer,&w,b);
            int same=n==m;for (unsigned k=0;same && k<n;k++) same=a[k]==b[k];
            check(same,pass?"draw":"prefetch",camera);
        }
    }
}
int main(void) {
    unsigned count=SCENE_MAP_CELL_COUNT;scene_cell_index_t ix;
    if (!scene_cell_index_build(&ix,scene_map_cells_data,count)) {fprintf(stderr,"index build failed\n");return 1;}
    check(ix.layer_major,"pack is not layer-major",0);
    uint16_t *a=malloc(count*2),*b=malloc(count*2);
    for (int camera=-64;camera<=MSX_CAMERA_MAX+64;camera++)
        for (unsigned depth=0;depth<2;depth++) compare_camera(&ix,scene_map_cells_data,count,camera,depth,a,b);
    /* Random cells (any order, any layer mix) and random windows. */
    srand(1234);
    for (unsigned trial=0;trial<300;trial++) {
        unsigned n=1+(unsigned)rand()%400;scene_map_cell_t *cells=calloc(n,sizeof(*cells));
        for (unsigned i=0;i<n;i++) {cells[i]=(scene_map_cell_t){(uint16_t)(rand()%5),(int16_t)((rand()%40-10)*(trial&1?16:100)),0,{0}};cells[i].y=(int16_t)(16*(rand()%30-10));}
        scene_cell_index_t r;if (!scene_cell_index_build(&r,cells,n)) {fprintf(stderr,"random build failed\n");return 1;}
        uint16_t *p=malloc(n*2),*q=malloc(n*2);
        for (unsigned k=0;k<20;k++) {
            scene_cell_window_t w={.5f+(float)(rand()%1000)/500,(float)(rand()%6000-3000)/3,(float)(rand()%32),(float)(rand()%400-100),(float)(rand()%600-100),rand()&1};
            for (unsigned layer=0;layer<5;layer++) {
                unsigned m=0;
                for (unsigned i=0;i<n;i++) if (cells[i].layer==layer && scene_cell_left_ok(&w,cells[i].x) && scene_cell_right_ok(&w,cells[i].x)) p[m++]=(uint16_t)i;
                unsigned got=walk(&r,layer,&w,q);
                int same=m==got;for (unsigned j=0;same && j<m;j++) same=p[j]==q[j];
                check(same,"random window",(int)trial);
                /* The renderer's vertical skip (scene_cell_column_*) visits
                 * exactly the cells of the window that reach rows top..bottom. */
                int top=-(rand()%200),bottom=top+16+rand()%300;
                unsigned a2=0,b2=0;
                for (unsigned j=0;j<m;j++) if (cells[p[j]].y+16>top && cells[p[j]].y<bottom) p[a2++]=p[j];
                for (unsigned rr=r.layer_runs[layer];rr<r.layer_runs[layer+1];rr++) {
                    unsigned first,end;scene_cell_run_window(&r,rr,&w,&first,&end);
                    for (unsigned j=first;j<end;) {
                        int y=cells[r.order[j]].y;
                        if (y+16<=top) {j=scene_cell_column_from(&r,j,scene_cell_column_end(&r,j,end),top-15);continue;}
                        if (y>=bottom) {j=scene_cell_column_end(&r,j,end);continue;}
                        q[b2++]=r.order[j++];
                    }
                }
                same=a2==b2;for (unsigned j=0;same && j<a2;j++) same=p[j]==q[j];
                check(same,"vertical skip",(int)trial);
            }
        }
        free(p);free(q);free(r.order);free(r.runs);free(cells);
    }
    if (fails) {fprintf(stderr,"scene_cull: %u mismatches\n",fails);return 1;}
    printf("scene_cull: %u cells, %u runs; cameras -64..%d and 300 random sets match the full scans\n",count,ix.run_count,MSX_CAMERA_MAX+64);
    return 0;
}
