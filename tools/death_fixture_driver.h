/* One death of build/death_fixture.h (tools/compile_death_fixture.py) with
 * the death programs in use (marco_death_use_programs and
 * marco_death_use_fat_program): from the update of 0C2956, the velocities
 * +30/+34 of every airborne update, the speed +26, the poses of every
 * update while dying and the draws while lying, the palette of +19, the
 * update of 0C2F06 (program end), of the actor's end (003F1E), the new life
 * one update later, the respawn x (when the capture has the progress) and
 * the entrance length (0C229C). Shared by tools/test_marco_death.c (Marco)
 * and tools/test_character.c (Tarma, Eri, Fio). */
typedef struct {unsigned bad,skipped,velocities,speeds,ends,respawns,entrances,poses,palettes,forms;} death_stats_t;
static void death_case_run(const death_case_t *c,death_stats_t *st) {
#define bad st->bad
#define skipped st->skipped
#define velocities st->velocities
#define speeds st->speeds
#define ends st->ends
#define respawns st->respawns
#define entrances st->entrances
#define poses st->poses
#define palettes st->palettes
#define forms st->forms
        const death_update_t *u=&death_updates[c->first];

        marco_death_t d;int32_t vx=(int32_t)u[0].v30,vy=(int32_t)u[0].v34;int facing=0;
        bool ground=(u[0].f15&0x40u)!=0;
        /* The deaths by form (data/form_death_fixture.json, MAME mzd1 / fatd1):
         * form 3 0C2C40 for a hit of kind 0, form 2 0C2A36; the other characters in tools/test_character.c. */
        if (!marco_death_start_form(&d,(uint8_t)c->kind,(uint8_t)c->form,false,ground,(c->f5b&0x80u)!=0,&vx,&vy,&facing)) {fprintf(stderr,"death %u: form %u not translated\n",c->frame,c->form);bad++;}
        forms+=c->form!=0;
        /* 009EA8's palette of +19 (0BEDE8 burnt, 0BEE08 the mummy's, else kept). */
        if (c->palette_table!=0xffffffffu) {
            uint32_t want=c->palette_table==0xbedd0u?0u:c->palette_table;
            if (d.palette_table!=want) {fprintf(stderr,"death %u: palette table %06X, capture %06X\n",c->frame,(unsigned)d.palette_table,c->palette_table);bad++;}
            else palettes++;
        }
        if (c->kind!=2 && !d.fat) ground=false;                /* 0C2E7E */
        unsigned k=1,end=0,gone=0;uint32_t got[256]={0};marco_life_phase_t phase[256]={0};
        for (;k<c->count && d.phase!=MARCO_GONE;k++) {
            marco_death_velocity(&d,ground,&vx,&vy);
            bool now=(u[k].f15&0x40u)!=0;
            /* Updates whose motion the sweep changed (the position moved by
             * other than +34, another parent, or the ceiling bit +15 bit 7) and kind 7's first update
             * (+30/+34 = -/+3.80 written outside 0C2C7A, not located:
             * explicit stop) are not compared. */
            bool free=u[k].p38==u[k-1].p38 && !(u[k].f15&0x80u) && u[k].y2c-u[k-1].y2c==u[k].v34 && !(c->kind==7 && k==1);
            if (!free) skipped++;
            else if (!now && d.phase==MARCO_DYING) {
                if ((uint32_t)vx!=u[k].v30 || (uint32_t)vy!=u[k].v34) {if (bad<20) fprintf(stderr,"death %u update %u: +30/+34 %08X %08X, capture %08X %08X\n",c->frame,k,(unsigned)vx,(unsigned)vy,u[k].v30,u[k].v34);bad++;}
                else velocities++;
            }
            vx=(int32_t)u[k].v30;vy=(int32_t)u[k].v34;        /* the motion (and landing) of the capture */
            marco_life_phase_t before=d.phase;
            marco_death_after(&d,now,&vx,(int16_t)(u[k].x3c>>16),(int16_t)(u[k].y40>>16));
            if (before==MARCO_DYING) {
                if (d.anim.speed!=u[k].f26) {if (bad<20) fprintf(stderr,"death %u update %u: +26 %04X, capture %04X\n",c->frame,k,d.anim.speed,u[k].f26);bad++;}
                else speeds++;
            }
            /* The pose of the update (0 none: lying, every other update). */
            {neo_anim_draw_t dd;got[k]=(d.phase!=MARCO_GONE || d.final_draw) && marco_death_draw(&d,0,0,1,&dd)?dd.descriptor:0;phase[k]=before;}
            if (before==MARCO_DYING && d.phase==MARCO_LYING) end=k;
            if (d.phase==MARCO_GONE) gone=k;
            ground=now;
        }
        /* The poses (MARCO_DRAW of 004406, logged by marco-d2): every
         * update's pose while dying, in order, and the number of draws while
         * lying (every other update: the body blinks). */
        if (c->pose_count) {
            unsigned n=0,lying=0;
            for (unsigned j=1;j<k;j++) {
                if (phase[j]==MARCO_DYING) {
                    if (n<c->pose_count && got[j]==death_poses[c->pose_first+n]) poses++;
                    else {if (bad<40) fprintf(stderr,"death %u (kind %X) dying pose %u: %06X, capture %06X\n",c->frame,c->kind,n,(unsigned)got[j],n<c->pose_count?death_poses[c->pose_first+n]:0u);bad++;}
                    n++;
                } else if (phase[j]==MARCO_LYING) lying+=got[j]!=0;
            }
            if (n!=c->pose_count || lying!=c->lying_draws) {fprintf(stderr,"death %u: %u dying poses, %u lying draws; capture %u, %u\n",c->frame,n,lying,c->pose_count,c->lying_draws);bad++;}
        }
        /* The capture: 0C2F06 written at the end update, 003F1E at the actor's end. */
        unsigned want_end=0,want_gone=0,want_entry=0,want_normal=0;
        for (unsigned j=1;j<c->count;j++) {
            if (!want_end && u[j].routine==0xc2f06) want_end=j;
            if (!want_gone && u[j].routine==0x3f1e) want_gone=j;
            if (!want_entry && u[j].routine==0xc149e) want_entry=j;
            if (u[j].routine==0xc229c) want_normal=j;
        }
        if (end!=want_end || gone!=want_gone) {fprintf(stderr,"death %u (kind %X): end %u gone %u, capture %u %u\n",c->frame,c->kind,end,gone,want_end,want_gone);bad++;}
        else ends++;
        /* The new life: one update after the end (panel 0C95AE). */
        if (want_entry!=gone+1) {fprintf(stderr,"death %u: new life at %u, capture %u\n",c->frame,gone+1,want_entry);bad++;}
        const death_update_t *e=&u[want_entry];
        int32_t root_x=(int32_t)(e->x3c-e->x28),root_y=(int32_t)(e->y40-e->y2c);
        marco_respawn_t r=marco_respawn_point((uint16_t)c->progress,root_x,root_y);
        if (!c->respawn) {}
        else if ((uint32_t)(r.x+root_x)!=e->x3c || r.type) {fprintf(stderr,"death %u: respawn x %08X, capture %08X\n",c->frame,(unsigned)(r.x+root_x),e->x3c);bad++;}
        else respawns++;
        marco_entrance_start(&d);unsigned n=0;
        while (marco_entrance_step(&d)) n++;
        if (want_entry+n!=want_normal) {fprintf(stderr,"death %u: entrance %u updates, capture %u\n",c->frame,n+1,want_normal-want_entry);bad++;}
        else entrances++;
#undef bad
#undef skipped
#undef velocities
#undef speeds
#undef ends
#undef respawns
#undef entrances
#undef poses
#undef palettes
#undef forms
}
