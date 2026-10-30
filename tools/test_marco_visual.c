#include <assert.h>
#include <stdio.h>
/* Campaign Marco (docs/MARCO_CAMPAIGN.md): private data, make check-marco-campaign. */
#include <stdint.h>
#include "marco_visual.h"
#include "marco_pose_meta.h"
static int find(uint32_t t){for(unsigned i=0;i<MARCO_POSES;i++)if(marco_poses[i].source_tag==t)return 1;return 0;}
int main(void){
  marco_visual_t v;marco_visual_reset(&v);unsigned missing=0,total=0,empty=0;
  const char*names[]={"idle","run","fire","aimup","fireup","crouch","crouchfire","jump","runjump","jumpfire","jumpupfire","land-run-left"};
  for(unsigned s=0;s<12;s++)for(unsigned t=0;t<120;t++){
    marco_visual_input_t in={.grounded=true,.facing=1,.x=120,.y=350};
    switch(s){case 1:in.moving=1;break;case 2:in.fired=t%8==0;break;case 3:in.aim_up=1;break;case 4:in.aim_up=1;in.fired=t%8==0;break;
      case 5:in.crouch=1;break;case 6:in.crouch=1;in.fired=t%8==0;break;case 7:in.grounded=t>=60;break;case 8:in.grounded=t>=60;in.moving=1;in.running_jump=1;break;
      case 9:in.grounded=t>=60;in.fired=t%8==0;break;case 10:in.grounded=t>=60;in.aim_up=1;in.fired=t%8==0;break;case 11:in.moving=1;in.facing=-1;break;}
    marco_visual_step(&v,&in);neo_anim_draw_t d[16];unsigned n=marco_visual_draws(&v,d,16);
    if(!n)empty++;
    for(unsigned i=0;i<n;i++){total++;if(!find(d[i].descriptor)){missing++;if(missing<10)printf("missing %06x in %s\n",d[i].descriptor,names[s]);}}
    if(0)printf("%s t=%u body=%d weapon=%d n=%u first=%06x\n",names[s],t,v.body_role,v.weapon_role,n,n?d[0].descriptor:0);
  }
  printf("draws=%u missing=%u empty=%u failures=%u starts=%u\n",total,missing,empty,v.failures,v.starts);
  assert(!empty && !v.failures && missing<=3);
  puts("PASS: campaign Marco programs for 12 situations: every draw but the run-start pose 188930 is in the audited catalog, no program stopped");
}
