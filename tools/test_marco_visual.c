#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
/* Campaign Marco (docs/MARCO_CAMPAIGN.md): private data, make check-marco-campaign. */
#include <stdint.h>
#include "marco_visual.h"
#include "marco_hgun.h"
#include "marco_death.h"
#include "legs_fixture.h"
#include "marco_throw_fixture.h"
#include "marco_mummy_fixture.h"
#include "run_weapon_fixture.h"
#include "run_turn_fixture.h"
#include "throw_fixture_driver.h"
static bool reserve(void *c){(void)c;return true;}
#include "marco_pose_meta.h"
static int find(uint32_t t){for(unsigned i=0;i<MARCO_POSES;i++)if(marco_poses[i].source_tag==t)return 1;return 0;}
int main(void){
  /* B83 (docs/BUG_MATRIX.md): running, the weapon program 24E2C8 picks its
   * pose by +77 and finishes; 0C3B34/0C37CE choose it again every update.
   * B84: the run's turn 0C2746 (body 24EF0E, no weapon), or with +98 = 2
   * only the facing (0C28C8). MAME 0.264 marco-run-fire-turn: every draw
   * (descriptor and facing bit) of 226 updates of running, firing and
   * turning both ways. */
  {unsigned k=0,diff=0;
   for(unsigned s=0;s<sizeof run_weapon_segments/sizeof run_weapon_segments[0];s++){
    marco_visual_t rv;marco_visual_reset(&rv);
    for(unsigned t=0;t<run_weapon_segments[s];t++,k++){
     const run_weapon_row_t*r=&run_weapon_rows[k];
     marco_visual_input_t in={.grounded=true,.moving=r->moving,.facing=r->facing,.fired=r->fired,.hgun_program=-1};
     marco_visual_step(&rv,&in);neo_anim_draw_t d[16];unsigned n=marco_visual_draws(&rv,d,16);bool same=n==r->count;
     for(unsigned i=0;i<n && same;i++)same=d[i].descriptor==r->draw[i] && (d[i].attributes&1u)==r->flip[i];
     if(!same && diff++<4)printf("B83 frame %u: port %06x/%u %06x/%u, MAME %06x/%u %06x/%u\n",r->frame,n?d[0].descriptor:0,n?d[0].attributes&1u:0,
       n>1?d[1].descriptor:0,n>1?d[1].attributes&1u:0,r->draw[0],r->flip[0],r->draw[1],r->flip[1]);
    }}
   printf("B83 run weapon: %u updates, %u differences\n",k,diff);assert(!diff);
   /* A turn after a finished run weapon: both parts take the new facing
    * (004364 reads +15 at every pose; none of these poses has a mirror flag). */
   marco_visual_t rv;marco_visual_reset(&rv);unsigned mixed=0;
   for(unsigned t=0;t<80;t++){
    marco_visual_input_t in={.grounded=true,.moving=1,.facing=t<50?1:-1,.fired=t==4,.hgun_program=-1};
    marco_visual_step(&rv,&in);neo_anim_draw_t d[16];unsigned n=marco_visual_draws(&rv,d,16);
    for(unsigned i=1;i<n;i++)mixed+=(d[i].attributes&1u)!=(d[0].attributes&1u);
   }
   printf("B83 turn after the run weapon: %u draws with the other facing\n",mixed);assert(!mixed);
   puts("PASS: B83 the run's weapon is chosen again every update (MAME marco-run-fire-turn, 0 differences)");}
  /* B84 (docs/BUG_MATRIX.md): the run's turn 0C2746 (0BEE58, +92 > 0
   * 0BEE98, form 1 +0x10) with no weapon (0C0F16: a press in its two
   * updates is no shot), the run's start against the facing from the idle
   * and the stop (+80 = 0 0C1EFA: 0BEE18; a press or up there, the idle
   * 0C1F4A). MAME 0.264 marco-run-turn, pistol, H and C: every draw of
   * every update, and every press: the turn gate (gameplay.c) blocks it
   * exactly when the original took no shot. */
  {unsigned k=0,diff=0,gate_diff=0,presses=0,lost=0;
   for(unsigned s=0;s<sizeof run_turn_segments/sizeof run_turn_segments[0];s++){
    const run_turn_segment_t*g=&run_turn_segments[s];
    marco_visual_t rv;marco_visual_reset(&rv);
    for(unsigned t=0;t<g->rows;t++,k++){
     const run_turn_row_t*r=&run_turn_rows[k];
     marco_visual_input_t in={.grounded=true,.moving=r->moving,.facing=r->facing,.aim_up=r->aim_up,.form=g->form,.hgun_program=-1};
     if(g->cgun){in.cgun=true;in.cgroup=1;in.cgun_fire=r->fired;in.fired=r->fired;}
     else if(g->form==1){in.hgun_program=r->hgun;in.hgun_start=r->hgun>=0;}
     else in.fired=r->fired;
     marco_turn_gate_t gate=marco_visual_turn_gate(&rv);
     if(r->pressed){presses++;lost+=r->lost;bool b=marco_turn_gate_blocks(&gate,true,false,r->moving,r->facing);
      if(b!=(bool)r->lost && gate_diff++<4)printf("B84 %s frame %u: press blocked %d, MAME lost %d\n",g->capture,r->frame,b,r->lost);}
     marco_visual_step(&rv,&in);neo_anim_draw_t d[16];unsigned n=marco_visual_draws(&rv,d,16);bool same=n==r->count;
     for(unsigned i=0;i<n && same;i++)same=d[i].descriptor==r->draw[i] && (d[i].attributes&1u)==r->flip[i];
     if(!same && diff++<12)printf("B84 %s frame %u: port %u %06x/%u %06x/%u, MAME %u %06x/%u %06x/%u\n",g->capture,r->frame,n,n?d[0].descriptor:0,n?d[0].attributes&1u:0,
       n>1?d[1].descriptor:0,n>1?d[1].attributes&1u:0,r->count,r->draw[0],r->flip[0],r->draw[1],r->flip[1]);
    }}
   printf("B84 run turn and stop: %u updates, %u differences; %u presses (%u lost in the original), %u gate differences\n",k,diff,presses,lost,gate_diff);
   assert(!diff && !gate_diff);
   puts("PASS: B84 the run's turn (pistol, H, C; level and up), its lost shot and the stop (MAME marco-run-turn, 0 differences)");}
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
  /* Grenade throws, the C's fire and the H's down programs
   * (data/marco_throw_fixture.json, marco-cat1/marco-g1/marco-c1): from the update
   * the original chose the program, the pose its part drew in every step. */
  unsigned tok=0,tbad=0;throw_fixture_run(throw_cases,THROW_CASES,throw_descriptors,&tok,&tbad);
  /* The mummy update by update (data/marco_mummy_fixture.json, marco-mz2:
   * 1475 updates from the transformation through 29 input cases): the
   * situation derived from the original controller, the body and weapon
   * descriptors the original drew. */
  {
    marco_visual_reset(&v);unsigned bok=0,bbad=0,wok=0,wbad=0,shown=0;
    for(unsigned k=0;k<MUMMY_STEPS;k++){
      const mummy_step_t*m=&mummy_steps[k];
      marco_visual_input_t in={.grounded=m->grounded,.moving=m->moving,.crouch=m->crouch,.aim_up=m->aim_up,.aim_down=m->aim_down,.fired=m->fired,
        .facing=m->facing,.x=120,.y=350,.form=(uint8_t)(k?3:0),.hgun_program=-1,.thrown=m->thrown,.transforming=m->transforming,.takeoff=m->takeoff};
      marco_visual_step(&v,&in);
      uint32_t b=v.body_count?v.body_draws[0].descriptor:0,w=v.weapon_role>=0&&v.weapon_count?v.weapon_draws[0].descriptor:0;
      if(!k)continue;   /* form 0, mid-program in the capture */
      if(b==m->body)bok++;else{bbad++;if(shown++<40||getenv("MUMMY_ALL"))printf("mummy step %u: body %06x, capture %06x\n",k,(unsigned)b,(unsigned)m->body);}
      if(w==m->weapon)wok++;else{wbad++;if(shown++<40||getenv("MUMMY_ALL"))printf("mummy step %u: weapon %06x, capture %06x\n",k,(unsigned)w,(unsigned)m->weapon);}
    }
    printf("mummy: body %u equal %u different, weapon %u equal %u different, %u stops\n",bok,bbad,wok,wbad,v.stops);
    /* Known weapon differences (12): the landing update's shot aims down in
     * the original (its fire state runs in the air routine; the port's
     * player has landed: 4 steps from 3903) and the gun raised/lowered in
     * the air 25F7A0/25F794 at the takeoff (0C3A5E not translated: 2 steps,
     * then the idle program 6 steps out of phase). */
    assert(!bbad && wbad<=12 && !v.failures);
    puts("PASS: the mummy (form 3) against marco-mz2 update by update: transformation, idle, walk, turn, jumps, landings, crouch, crouched fire, stand-up (body exact)");
  }
  printf("throws/C/S/F/H down/knife/descent/mummy/fat: %u steps equal, %u different (%u cases)\n",tok,tbad,THROW_CASES);
  assert(tok>=780&&!tbad);
  puts("PASS: grenade throws (pistol, H, C: standing, running, in the air, crouched), C fire, H down, knife cuts (pistol, H: standing, crouched), the parachute descent (hanging, cut), the mission end's victory (pistol, H), the fire of weapons 02 and 04 and the mummy's and fat Marco's fire and throw pose by pose against the captures");
}
