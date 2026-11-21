/* Host cost of end_zoom_rdp_entry (docs/MISION6.md): bench_end_zoom endzoom.bin ENTRIES [reps]; ENTRIES: the zoomed
 * list entries of a capture, 12 bytes each (x, y, zoom, attribute as little-endian u16, descriptor u32), e.g. from
 * check_end_zoom.listed. Run under valgrind --tool=callgrind with reps 1 and 3: the difference / 2 is one pass. */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "end_zoom_rdp.h"
static unsigned n;static void rect(void *c,const end_zoom_rect_t *r){(void)c;(void)r;n++;}
int main(int argc,char **argv){(void)argc;
 FILE *f=fopen(argv[1],"rb");fseek(f,0,SEEK_END);long sz=ftell(f);rewind(f);uint8_t *d=malloc(sz);if(fread(d,1,sz,f)!=(size_t)sz)return 1;
 end_zoom_file_t ez;end_zoom_parse(&ez,d,d+16,sz-16);end_zoom_rdp_t rt;end_zoom_rdp_parse(&rt,d+ez.l0_offset+65536,sz-ez.l0_offset-65536);
 FILE *e=fopen(argv[2],"rb");struct {uint16_t x,y,z,a;uint32_t dsc;} E[400];int ne=fread(E,12,400,e);
 void *w=malloc(END_ZOOM_WORK_BYTES);static uint16_t maps[64][4096];for(unsigned q=0;q<ez.descriptors;q++)end_zoom_rdp_local_map(&ez,q,maps[q]);static uint16_t lw[END_ZOOM_COLUMNS][32];int usemap=getenv("NOMAP")==0;end_zoom_rect_t *s=malloc(2*sizeof(end_zoom_rect_t)*END_ZOOM_RECTS);
 clock_t t=clock();int reps=argc>3?atoi(argv[3]):200;
 for(int r=0;r<reps;r++)for(int i=0;i<ne;i++){int k=end_zoom_find_descriptor(&ez,E[i].dsc);end_zoom_rdp_io_t io={rect,0,w,s,END_ZOOM_RECTS,usemap?maps[k]:0,lw,0,0,0,0,0};end_zoom_rdp_entry(&ez,&rt,k,E[i].x,E[i].y,E[i].z,E[i].a,&io);}
 double us=(double)(clock()-t)/CLOCKS_PER_SEC*1e6/reps;printf("%d entries %u rects: %.2f us/entry %.3f us/rect\n",ne,n/reps,us/ne,us/(n/reps));}
