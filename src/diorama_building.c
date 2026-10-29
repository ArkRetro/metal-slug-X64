#include "diorama_building.h"
bool diorama_building_cell(const diorama_building_t *b,unsigned layer,int x,int y) {
 return layer==3 && x>=b->left && x+16<=b->right && y>=b->top && y+16<=b->bottom;
}
unsigned diorama_building_sides(retro_camera_t camera,const diorama_building_t *b,retro_emit_fn emit,void *ctx) {
 if(!b || !emit || b->depth<=0 || b->left>=b->right || b->top>=b->bottom) return 0;
 if(b->right<camera.x-96 || b->left>camera.x+416) return 0;
 unsigned count=0;
 for(unsigned side=0;side<2;side++) {
  float x=side?b->right:b->left;
  retro_vec3_t points[4]={{x,b->top,0},{x,b->bottom,0},{x,b->bottom,b->depth},{x,b->top,b->depth}};
  float v[4][6];bool valid=true;
  for(unsigned i=0;i<4;i++) {
   retro_projected_t q;if(!retro_project(camera,points[i],&q)) {valid=false;break;}
   v[i][0]=q.x;v[i][1]=q.y;
   float shade=i<2?.52f:.30f;
   v[i][2]=shade*.83f;v[i][3]=shade*.80f;v[i][4]=shade;v[i][5]=1;
  }
  if(valid) {emit(ctx,v[0],v[1],v[2]);emit(ctx,v[0],v[2],v[3]);count+=2;}
 }
 return count;
}
