/* The fixture cases of tools/extract_throw_fixture.py against marco_visual_step
 * (tools/test_marco_visual.c for Marco, tools/test_character.c for the other
 * characters with their own captures): from the update the original chose
 * the program, the pose its part drew in every step. */
#ifndef THROW_FIXTURE_DRIVER_H
#define THROW_FIXTURE_DRIVER_H
static void throw_fixture_run(const throw_case_t *throw_cases,unsigned cases,const unsigned *throw_descriptors,unsigned *ok,unsigned *different){
  marco_visual_t v;
  unsigned tok=0,tbad=0;
  for(unsigned i=0;i<cases;i++){
    const throw_case_t*c=&throw_cases[i];
    /* c_/s_/f_: the C's, 0BFFD8.. (ids 02/07) and 0C00D8.. (ids 04/09) fire
     * tables (marco-c1, marco-w2, marco-w4); m_: the mummy, form 3
     * (marco-mz2), whose fire is the pistol's. */
    /* g_: fat Marco, form 2 (marco-f2): the pistol's fire through the rows +0x20. */
    bool mummy=c->name[0]=='m' || c->name[0]=='g';
    bool grenade=strstr(c->name,"grenade")!=NULL,cgun=c->name[0]=='c' || c->name[0]=='s' || c->name[0]=='f',down=strstr(c->name,"down")!=NULL,knife=strstr(c->name,"knife")!=NULL,hang=!strncmp(c->name,"d_hang",6),cut=!strncmp(c->name,"d_cut",5);
    marco_visual_reset(&v);
    marco_visual_input_t in={.grounded=true,.facing=1,.x=120,.y=350,.form=(uint8_t)c->form,.hgun_program=-1,.cgun=cgun,
      .cgroup=(uint8_t)(c->name[0]=='s'?2:c->name[0]=='f'?3:1)};
    if(c->situation==3)in.moving=1;
    if(cut){in.grounded=false;in.hanging=true;}   /* the descent: hanging before the cut */
    /* v_: the mission end's victory (marco-v0/v1, --victory): 0C2046 sets
     * the routine 0C1E84 in the update before its program starts. */
    bool victory=c->name[0]=='v';
    for(unsigned k=0;k<6;k++){if(c->situation==2&&k==4&&!hang)in.grounded=false;if(c->situation==1&&!grenade)in.crouch=1;if(victory&&k==5)in.victory=true;marco_visual_step(&v,&in);}
    if(c->situation==1)in.crouch=1;
    in.aim_up=strstr(c->name,"_up")!=NULL;in.aim_down=down;
    unsigned bad=0;
    for(unsigned k=0;k<c->count;k++){
      if(k==0&&hang){in.grounded=false;in.hanging=true;}if(k==0&&cut)in.hanging=false;
      in.thrown=grenade&&k==0;in.cgun_fire=cgun&&!grenade&&k==0;in.knife=knife&&k==0;in.fired=mummy&&!grenade&&k==0;
      if(c->name[0]=='h'&&down){in.hgun_program=strstr(c->name,"sweep")?4:2;in.hgun_start=k==0;}
      marco_visual_step(&v,&in);
      unsigned n=c->part?(v.weapon_role>=0?v.weapon_count:0):v.body_count;
      uint32_t got=n?(c->part?v.weapon_draws[0].descriptor:v.body_draws[0].descriptor):0;
      if(got==throw_descriptors[c->first+k])tok++;
      else{tbad++;if(bad++<3)printf("%s step %u: %06x, capture %06x\n",c->name,k,(unsigned)got,throw_descriptors[c->first+k]);}
    }
    assert(!v.failures);
  }
  *ok=tok;*different=tbad;
}
#endif
