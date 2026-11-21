/* Closed-loop test of the weapon block 10C718..10C72B (src/marco_ammo.c)
 * against the capture_weapons.lua runs (tools/extract_weapon_block.py).
 * The block starts from the first recorded bytes of each run and is never
 * reloaded: every frame that changes it applies the frame's events through
 * the native routines (grant 0C0928, bombs 0C0B32, expiry 0C337A, life
 * 0C0E6E, fire 0C0D64, throw 0C4786/0C0808, grenade 0C0D34/0C0D48, release
 * 0C0DFC) and must leave the recorded bytes. */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/marco_ammo.h"
typedef struct {int kind,arg;} weapon_event_t;
typedef struct {unsigned frame;uint8_t before[MARCO_WEAPON_BLOCK],after[MARCO_WEAPON_BLOCK],slot[0x24];
    uint8_t f93,f5f;unsigned count;const weapon_event_t *events;} weapon_step_t;
typedef struct {const char *name;const weapon_step_t *steps;unsigned count;} weapon_run_t;
#include "weapon_block_fixture.h"
static void dump(const char *what,const uint8_t *b) {
    printf("  %s",what);for (unsigned i=0;i<MARCO_WEAPON_BLOCK;i++) printf(" %02x",b[i]);printf("\n");
}
int main(void) {
    unsigned steps=0,events=0,grants=0,bombs=0,expiries=0;
    for (unsigned r=0;r<sizeof(weapon_runs)/sizeof(weapon_runs[0]);r++) {
        const weapon_run_t *run=&weapon_runs[r];
        marco_weapons_t w;memcpy(w.b,run->steps[0].before,MARCO_WEAPON_BLOCK);
        for (unsigned i=0;i<run->count;i++) {
            const weapon_step_t *s=&run->steps[i];
            if (memcmp(w.b,s->before,MARCO_WEAPON_BLOCK)) {printf("FAIL %s frame %u: block before\n",run->name,s->frame);dump("native",w.b);dump("orig  ",s->before);return 1;}
            marco_weapon_player_t m={true,(s->f5f&0x80u)!=0,s->f93,0xff,0,false};
            uint8_t slot[0x24];memcpy(slot,s->slot,sizeof(slot));
            for (unsigned k=0;k<s->count;k++) {
                const weapon_event_t *e=&s->events[k];uint16_t sound;marco_ammo_t a;bool ok=true;
                switch (e->kind) {
                case 0: break;
                case 1: ok=marco_weapons_grant(&w,0,(uint8_t)e->arg,&m,&sound) && sound==marco_weapon_sound[e->arg] && m.form94==marco_weapon_form[e->arg];grants++;break;
                case 2: ok=marco_weapons_bombs(&w,0,(uint8_t)e->arg,&m,slot,&sound)==MW_ACCEPTED && sound==0x1124;bombs++;break;
                case 3: {uint8_t form=m.form93;ok=marco_weapons_expire(&w,0,&m) && (form!=1 || m.form94==0);expiries++;break;}
                case 4: marco_weapons_life(&w,0,&m);ok=m.form93==0 && m.form94==0xff;break;
                case 5: a=marco_weapons_ammo(&w,0);ok=marco_ammo_reserve(&a,a.kind,false,false);marco_weapons_set_ammo(&w,0,&a);break;
                case 6: case 7: marco_weapons_throw(&w,0,false);break;
                case 8: ok=marco_weapons_armed(&w,0);break;
                case 9: ok=marco_weapons_landed(&w,0);break;
                case 10: a=marco_weapons_ammo(&w,0);marco_ammo_release(&a,a.kind);marco_weapons_set_ammo(&w,0,&a);break;
                case 12: marco_weapons_death(&w,0);break;
                case 11: a=marco_weapons_ammo(&w,0);ok=!marco_ammo_reserve(&a,a.kind,false,false);marco_weapons_set_ammo(&w,0,&a);break;
                }
                if (!ok) {printf("FAIL %s frame %u: event %d(%d) result\n",run->name,s->frame,e->kind,e->arg);return 1;}
                events+=e->kind!=0;
            }
            if (memcmp(w.b,s->after,MARCO_WEAPON_BLOCK)) {printf("FAIL %s frame %u: block after\n",run->name,s->frame);dump("native",w.b);dump("orig  ",s->after);return 1;}
            steps++;
        }
    }
    /* Table edges the captures do not reach. */
    marco_weapons_t w;marco_weapons_new_game(&w);marco_weapon_player_t m={true,false,0,0xff,5,false};
    marco_weapons_life(&w,1,&m);
    assert(marco_weapons_id(&w,1)==0 && marco_weapons_bomb_count(&w,1)==10 && marco_weapons_rounds(&w,1)==0xffff && w.b[11]==1 && w.b[5]==0xff);
    uint16_t sound;
    assert(!marco_weapons_grant(&w,1,16,&m,&sound) && !sound);
    m.form93=3;assert(!marco_weapons_grant(&w,1,10,&m,&sound));m.form93=0;
    assert(marco_weapons_grant(&w,1,10,&m,&sound) && sound==0x112d && marco_weapons_rounds(&w,1)==200 && w.b[5]==0xff);
    for (unsigned i=0;i<6;i++) assert(marco_weapons_grant(&w,1,10,&m,&sound));
    assert(marco_weapons_rounds(&w,1)==999);                           /* 0C09AA */
    w.b[9]=95;assert(marco_weapons_bombs(&w,1,10,&m,NULL,&sound)==MW_ACCEPTED && w.b[9]==0x63);
    assert(marco_weapons_bombs(&w,1,10,&m,NULL,&sound)==MW_REFUSED);
    uint8_t slot[0x24]={0};slot[1]=0x34;slot[0x21]=1;slot[0x1f]=5;slot[0x20]=4;w.b[9]=3;
    assert(marco_weapons_bombs(&w,1,10,&m,slot,&sound)==MW_ACCEPTED && slot[0x1f]==15 && w.b[9]==3);
    slot[0x21]=2;assert(marco_weapons_bombs(&w,1,10,&m,slot,&sound)==MW_ACCEPTED && w.b[9]==13);
    slot[0x21]=4;assert(marco_weapons_bombs(&w,1,10,&m,slot,&sound)==MW_STOP);
    m.dead=true;assert(marco_weapons_expire(&w,1,&m) && marco_weapons_id(&w,1)==0);
    assert(!marco_weapons_armed(&w,2) && !marco_weapons_landed(&w,2));
    printf("PASS: weapon block: %u closed-loop frames, %u events (%u grants, %u bomb pickups, %u expiries)\n",steps,events,grants,bombs,expiries);
    return 0;
}
