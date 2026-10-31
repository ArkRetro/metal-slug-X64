#include <assert.h>
#include <stdio.h>
#include <string.h>
/* Campaign Marco (docs/MARCO_CAMPAIGN.md): private data, make check-marco-campaign. */
#include <stdint.h>
#include "marco_visual.h"
#include "marco_hgun.h"
#include "marco_death.h"
#include "legs_fixture.h"
static bool reserve(void *c){(void)c;return true;}
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
  /* Form 1 (the H): the fire states of marco_hgun.c start the programs. */
  const char*hnames[]={"h-idle","h-fire","h-aimup","h-aimup-fire","h-fire-after-up","h-crouch","h-crouch-fire","h-run","h-run-fire","h-jump","h-jump-fire","h-runjump","h-runjump-fire","h-jump-up-fire"};
  unsigned hmissing=0,htotal=0,hempty=0,hstarts=0;
  for(unsigned s=0;s<14;s++){
    marco_visual_reset(&v);marco_hgun_t h;marco_hgun_reset(&h);
    for(unsigned t=0;t<60;t++){
      marco_visual_input_t in={.grounded=true,.facing=1,.x=120,.y=350,.form=1,.hgun_program=-1};
      bool press=false;
      switch(s){case 1:press=t%6==0;break;case 2:in.aim_up=t<14;break;case 3:in.aim_up=1;press=t%6==0;break;
        case 4:in.aim_up=t<20;press=t%6==0;break;case 5:in.crouch=1;break;case 6:in.crouch=1;press=t%6==0;break;
        case 7:in.moving=1;break;case 8:in.moving=1;press=t%6==0;break;case 9:in.grounded=t>=40;break;
        case 10:in.grounded=t>=40;press=t%6==0;break;case 11:in.grounded=t>=40;in.moving=1;in.running_jump=1;break;
        case 12:in.grounded=t>=40;in.moving=1;in.running_jump=1;press=t%6==0;break;case 13:in.grounded=t>=40;in.aim_up=1;press=t%6==0;break;}
      if(press)marco_hgun_press(&h);
      marco_hgun_input_t hi={in.aim_up,false,false};marco_hgun_shot_t shots[4];
      marco_hgun_step(&h,&hi,reserve,NULL,shots,4);
      if(h.state==MARCO_HGUN_BURST||h.state==MARCO_HGUN_SWEEP){in.hgun_program=h.program;in.hgun_start=h.t==1;hstarts+=in.hgun_start;}
      marco_visual_step(&v,&in);neo_anim_draw_t d[16];unsigned n=marco_visual_draws(&v,d,16);
      if(!n)hempty++;
      for(unsigned i=0;i<n;i++){htotal++;if(!find(d[i].descriptor)&&d[i].descriptor!=0x188930){hmissing++;if(hmissing<10)printf("missing %06x in %s t=%u\n",d[i].descriptor,hnames[s],t);}}
    }
    assert(!v.failures);
  }
  printf("H draws=%u missing=%u empty=%u program starts=%u\n",htotal,hmissing,hempty,hstarts);
  assert(!hempty && !hmissing && hstarts>=30);
  puts("PASS: form-1 (H) programs for 14 situations, bursts and sweeps started by the H fire states: every draw but 188930 in the audited catalog, no program stopped");
  /* Deaths and the entrance (marco_death.c): every pose of 0BF298, 0BF318,
   * 0BF358 and 0BF498 is in the catalog (0BF3D8, the kind-2 ground death, was
   * not seen in a capture: its poses are counted, not required). */
  unsigned dposes=0,dmissing=0;
  for(unsigned k=0;k<5;k++){
    marco_death_t md;int32_t vx=0,vy=0;int facing=1;
    if(k==4)marco_entrance_start(&md);
    else marco_death_start(&md,(uint8_t)(k==0?0:k==1?0:k==2?7:2),k!=1,true,&vx,&vy,&facing);
    uint32_t last=0;
    for(unsigned t=0;t<80;t++){
      bool ground=k!=1 || t>=20;
      if(k==4){if(!marco_entrance_step(&md))break;}
      else if(marco_death_after(&md,ground,&vx,100,0x150)==MARCO_GONE)break;
      neo_anim_draw_t dd;
      if(marco_death_draw(&md,100,350,-1,&dd) && dd.descriptor!=last){last=dd.descriptor;dposes++;
        if(!find(dd.descriptor)){if(k==3)continue;dmissing++;printf("death %u missing %06x\n",k,dd.descriptor);}}
    }
  }
  printf("death/entrance poses=%u missing=%u\n",dposes,dmissing);
  assert(dposes>=40 && !dmissing);
  puts("PASS: death programs 0BF298/0BF318/0BF358 and the entrance 0BF498: every pose in the audited catalog");
  /* The run cycle frame by frame (data/legs_fixture.json, marco-cat1): the
   * body's pose of every update of walk_right and run_cycle_right from the
   * first step of 24DD18 (speed 0x1C1). The run-start pose 188930 lasts
   * less than an update in the arcade (never drawn): the step shows the next. */
  unsigned rok=0,rbad=0;
  for(unsigned i=0;i<LEGS_CASES;i++){
    const legs_case_t*c=&legs_cases[i];
    if(strcmp(c->name,"walk_right")&&strcmp(c->name,"run_cycle_right"))continue;
    marco_visual_reset(&v);
    marco_visual_input_t in={.grounded=true,.facing=1,.x=120,.y=350,.hgun_program=-1};
    marco_visual_step(&v,&in);
    in.moving=1;
    for(unsigned k=0;k<c->count;k++){
      const legs_step_t*st=&legs_steps[c->first+k];
      marco_visual_step(&v,&in);
      if(v.body_count&&v.body_draws[0].descriptor==st->descriptor)rok++;
      else{rbad++;if(rbad<10)printf("%s step %u: body %06x, capture %06x\n",c->name,k,v.body_count?v.body_draws[0].descriptor:0,st->descriptor);}
    }
  }
  printf("run cycle: %u updates equal, %u different\n",rok,rbad);
  assert(rok>=60&&!rbad);
  puts("PASS: run cycle 24DD18 at 0x1C1, pose by pose against marco-cat1");
}
