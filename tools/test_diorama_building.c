#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "../src/diorama_building.h"
static unsigned triangles;
static float leftmost;
static void collect(void *ctx,const float *a,const float *b,const float *c) {
 (void)ctx;const float *v[]={a,b,c};triangles++;
 for(unsigned i=0;i<3;i++) {
  assert(isfinite(v[i][0]) && isfinite(v[i][1]));
  if(v[i][0]<leftmost) leftmost=v[i][0];
  for(unsigned j=2;j<6;j++) assert(v[i][j]>=0 && v[i][j]<=1);
 }
}
int main(void) {
 diorama_building_t b={704,96,832,176,96};
 assert(diorama_building_cell(&b,3,704,96));
 assert(!diorama_building_cell(&b,2,704,96));
 assert(!diorama_building_cell(&b,3,704,176));
 assert(!diorama_building_cell(&b,3,697,96));
 for(unsigned camera=0;camera<=1920;camera+=4) {
  triangles=0;leftmost=10000;
  unsigned n=diorama_building_sides((retro_camera_t){camera,0,0,320},&b,collect,NULL);
  assert(n==triangles && n<=4 && n+88<=RETRO_MAX_TRIANGLES);
 }
 triangles=0;leftmost=10000;
 assert(diorama_building_sides((retro_camera_t){480,0,0,320},&b,collect,NULL)==4);
 assert(leftmost<704-480); /* Receding left wall extends past the front edge. */
 b.depth=0;assert(!diorama_building_sides((retro_camera_t){480,0,0,320},&b,collect,NULL));
 puts("PASS: building cutout bounds, projected side extent and full-region triangle budget");
}
