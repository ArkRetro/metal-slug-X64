/* Fat Marco's shots (+93 = 2, docs/MISSION3_DEBUG.md G21) against the MAME
 * capture of tools/capture_fat_weapons.lua (data/fatmuzzle_fixture.json,
 * tools/extract_fat_muzzle_fixture.py): every projectile, flash and grenade
 * birth relative to the player's +28/+2C, and the fat walk 259D68's +74
 * update by update (src/marco_legs.c, the fat body programs). Bit-exact:
 *  - 0C436A: 0C4244 with the form-2 column plus +72/+74 (crouched, +90 =
 *    FF: 0C4234 (16,16)), x negated facing left; +6E the aim (the H: + 3 x
 *    ((+95 % 3) - 1));
 *  - 0C4610 (the H's sweeps): marco_muzzle_sweep_fat;
 *  - the flash 0C58D2 at its projectile's point;
 *  - the throw 0C4806: (+72, 4 + +74), crouched (0, 16) with the second
 *    constructor of 0C476E;
 *  - the player's +72/+74 at each birth is a pose of its fat body program. */
#include <stdio.h>
#include <stdlib.h>
#include "../src/marco_legs.h"
#include "../src/neo_gactor.h"
#include "fatmuzzle_fixture.h"
static bool fat_pose(uint32_t routine,int x72,int y74) {
    int8_t x,y;marco_legs_program_t p=routine==0xc2888u?MARCO_LEGS_RUN:MARCO_LEGS_IDLE;
    for (unsigned i=0;marco_legs_fat_entry(p,i,&x,&y);i++) if (x==x72 && y==y74) return true;
    return false;
}
int main(void) {
    unsigned ok=0,bad=0,sweeps=0,fans=0,flashes=0,throws=0;
    for (unsigned i=0;i<FAT_BIRTHS;i++) {
        const fat_birth_t *b=&fat_births[i];bool left=b->left!=0,good=true;
        if (!fat_pose(b->routine,b->x72,b->y74)) {fprintf(stderr,"birth %u: +72/+74 (%d,%d) no pose of routine %06X\n",b->frame,b->x72,b->y74,b->routine);good=false;}
        marco_legs_t l={0};l.x72=b->x72;l.y74=b->y74;
        marco_muzzle_t m={0,0};
        if (b->ctor==0xc58d2u) {
            bool partner=false;
            for (unsigned k=0;k<FAT_BIRTHS;k++) if (fat_births[k].frame==b->frame && fat_births[k].ctor!=0xc58d2u) {m.x=fat_births[k].dx;m.y=fat_births[k].dy;partner=true;}
            /* Its projectile placed later (left out of the fixture): the muzzle itself. */
            if (!partner) {int aim=b->f92==1?1:b->f92==0xff?-1:0;m=b->f90==0xffu?(marco_muzzle_t){(int16_t)(left?-16:16),16}:marco_muzzle_aim(&l,aim,2,left);}
            flashes++;
        } else if (b->ctor==0x1bedau || b->ctor==0x1bea4u) {
            bool crouched=b->f90==0xffu;
            m=crouched?(marco_muzzle_t){0,16}:(marco_muzzle_t){b->x72,(int16_t)(4+b->y74)};
            good&=(b->ctor==0x1bea4u)==crouched;throws++;
        } else {
            static const uint8_t bases[3]={0x00,0x40,0xc0};
            int aim=b->f92==1?1:b->f92==0xff?-1:0;
            uint8_t base=b->f90==0xffu?0:bases[aim==1?1:aim<0?2:0];if (left) base=(uint8_t)(0x80-base);
            int d=(int8_t)(uint8_t)(b->angle-base);bool h=b->ctor==0x1bdd2u || b->ctor==0x1b668u;
            if (d==0 || (h && (d==3 || d==-3))) {
                m=b->f90==0xffu?(marco_muzzle_t){(int16_t)(left?-16:16),16}:marco_muzzle_aim(&l,aim,2,left);fans+=h;
            } else if (h) {
                uint8_t pa=left?(uint8_t)(0x80-b->angle):b->angle,fa=marco_sweep_fat_angle(pa);
                m=marco_muzzle_sweep_fat(fa,ga_sine(fa),ga_cosine(fa),left);sweeps++;
            } else good=false;
        }
        if (good && m.x==b->dx && m.y==b->dy) ok++;
        else {fprintf(stderr,"birth %u %06X routine %06X %s 90=%02X 92=%02X angle %02X: (%d,%d), native (%d,%d)\n",b->frame,b->ctor,b->routine,left?"left":"right",b->f90,b->f92,b->angle,b->dx,b->dy,m.x,m.y);bad++;}
    }
    unsigned wok=0,wbad=0;
    for (unsigned i=0;i<FAT_WALKS;i++) {
        marco_legs_t l;marco_legs_reset(&l);marco_legs_set_fat(&l,true);
        marco_legs_step(&l,true,false,false,false,false,false);
        for (unsigned k=0;k<fat_walks[i].count;k++) {
            marco_legs_step(&l,true,true,false,false,false,false);
            if (l.x72==0 && l.y74==fat_walks[i].y74[k]) wok++;
            else {if (wbad<10) fprintf(stderr,"walk %u update %u: +74 %d, capture %d\n",fat_walks[i].frame,k,l.y74,fat_walks[i].y74[k]);wbad++;}
        }
    }
    printf("fat muzzle: %u births equal (%u H fan, %u H sweep, %u flashes, %u throws), %u different; walk %u updates equal, %u different\n",ok,fans,sweeps,flashes,throws,bad,wok,wbad);
    if (bad || wbad) return 1;
    puts("PASS: fat Marco's muzzles 0C436A/0C4610, flashes, throws and walk +72/+74 (form 2) bit-exact");
    return 0;
}
