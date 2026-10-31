/* The H muzzle (src/marco_legs.c: 0C4244/0C453A plus the body programs'
 * +72/+74) against every H bullet born in the captures hold2, hfam, hudp2
 * and hudv2 (data/hmuzzle_fixture.json, tools/extract_hmuzzle_fixture.py):
 * the bullet's birth relative to the player's +28/+2C and its angle +6E.
 * The angle is a fan step (0C44A6: base 0, 0x40 or 0xC0, mirrored facing
 * left, +-2) or a sweep step (0C453A: 08/18/28/38, 30/20/10, mirrored);
 * the position must be the muzzle of that aim with the +72/+74 of a pose
 * of the body program of the player's routine: idle 0C1FA6 (24C5EA),
 * crouched 0C2588, walking 0C2888 (24DD18), airborne 0C2692 (24E4E0,
 * 24E858, 24DF6C), 0C1EBE (any). Bit-exact pixels, sine table of 002E7C. */
#include <stdio.h>
#include <stdlib.h>
#include "../src/marco_legs.h"
#include "../src/neo_gactor.h"
#include "hmuzzle_fixture.h"
static bool pose_fits(marco_legs_program_t p,const hbirth_t *b,bool sweep,int aim,uint8_t angle) {
    int8_t x,y;
    for (unsigned i=0;marco_legs_entry(p,i,&x,&y);i++) {
        marco_legs_t l={0};l.x72=x;l.y74=y;
        marco_muzzle_t m=sweep?marco_muzzle_sweep(&l,angle,ga_sine(angle),ga_cosine(angle)):marco_muzzle_aim(&l,aim,1,b->left!=0);
        if (m.x==b->dx && m.y==b->dy) return true;
    }
    return false;
}
int main(void) {
    unsigned ok=0,bad=0,sweeps=0,fans=0;
    for (unsigned i=0;i<HBIRTHS;i++) {
        const hbirth_t *b=&hbirths[i];uint8_t a=(uint8_t)b->angle;
        /* The aim: a fan step around a base, else a sweep step. */
        int aim=0;bool sweep=true;
        static const uint8_t bases[3]={0x00,0x40,0xc0};
        for (unsigned k=0;k<3;k++) {
            uint8_t base=b->left?(uint8_t)(0x80-bases[k]):bases[k];
            int d=(int8_t)(uint8_t)(a-base);
            if (d==0 || d==2 || d==-2) {sweep=false;aim=k==1?1:k==2?-1:0;break;}
        }
        if (sweep) {
            uint8_t s=b->left?(uint8_t)(0x80-a):a;
            if (!(s==0x08||s==0x18||s==0x28||s==0x38||s==0x30||s==0x20||s==0x10)) {fprintf(stderr,"birth %u: angle %02X neither fan nor sweep\n",b->frame,a);bad++;continue;}
        }
        marco_legs_program_t progs[3];unsigned n=0;
        switch (b->routine) {
        case 0xc1fa6: progs[n++]=MARCO_LEGS_IDLE;break;
        case 0xc2588: progs[n++]=MARCO_LEGS_CROUCH;break;
        case 0xc2888: progs[n++]=MARCO_LEGS_RUN;break;
        case 0xc2692: progs[n++]=MARCO_LEGS_JUMP;progs[n++]=MARCO_LEGS_RUN_JUMP;progs[n++]=MARCO_LEGS_RUN_LOOP;break;
        default: progs[n++]=MARCO_LEGS_IDLE;progs[n++]=MARCO_LEGS_RUN;progs[n++]=MARCO_LEGS_RUN_LOOP;break;
        }
        bool fit=false;
        for (unsigned k=0;k<n && !fit;k++) fit=pose_fits(progs[k],b,sweep,aim,a);
        if (fit) {ok++;sweeps+=sweep;fans+=!sweep;}
        else {fprintf(stderr,"birth %u routine %06X %s angle %02X: (%d,%d) is no muzzle of its body program\n",b->frame,b->routine,b->left?"left":"right",a,b->dx,b->dy);bad++;}
    }
    printf("muzzle: %u H births equal (%u fan, %u sweep), %u different\n",ok,fans,sweeps,bad);
    if (bad) return 1;
    puts("PASS: H muzzles 0C4244/0C453A with the body programs' +72/+74");
    return 0;
}
