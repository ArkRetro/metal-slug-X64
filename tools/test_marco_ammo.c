#include <assert.h>
#include <stdio.h>
#include "../src/marco_ammo.h"
#include "marco_ammo_fixture.h"
#ifdef __mips__
#include <libdragon.h>
#endif
int main(void) {
#ifdef __mips__
    debug_init_emulog();console_init();console_set_render_mode(RENDER_MANUAL);
#endif
    unsigned count=sizeof(ammo_cases)/sizeof(*ammo_cases);
    for(unsigned i=0;i<count;i++) {
        marco_ammo_t s=ammo_cases[i].before;
        assert(marco_ammo_reserve(&s,ammo_cases[i].request,ammo_cases[i].unlimited,ammo_cases[i].special)==ammo_cases[i].allowed);
        marco_ammo_t expected=ammo_cases[i].after;
        assert(s.ammunition==expected.ammunition && s.active==expected.active && s.cap==expected.cap && s.kind==expected.kind);
    }
    /* Synthetic boundaries, distinguished from captured gameplay above. */
    marco_ammo_t s={.ammunition=1,.active=2,.cap=2,.kind=3};
    assert(!marco_ammo_reserve(&s,4,false,false) && s.ammunition==1);
    assert(!marco_ammo_reserve(&s,3,false,false) && s.ammunition==0 && s.active==2);
    s.active=0;assert(!marco_ammo_reserve(&s,3,false,false));
    assert(marco_ammo_reserve(&s,3,true,false) && s.ammunition==0 && s.active==1);
    s.ammunition=65535;s.cap=255;s.active=255;
    assert(marco_ammo_reserve(&s,3,false,false) && s.ammunition==65535 && s.active==255);
    s.ammunition=0;assert(marco_ammo_reserve(&s,3,false,true) && s.ammunition==0);
    s.active=1;s.ammunition=9;
    marco_ammo_release(&s,4);assert(s.active==1 && s.ammunition==9);
    marco_ammo_release(&s,3);assert(s.active==0 && s.ammunition==9);
    marco_ammo_release(&s,3);assert(s.active==0 && s.ammunition==9);
    printf("PASS: ammunition calls=%u and synthetic accounting boundaries\n",count);
#ifdef __mips__
    debugf("MSX AMMO REGRESSION PASS calls=%u\n",count);
    console_render();while(1) {}
#endif
}
