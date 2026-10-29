#include <assert.h>
#include <stdio.h>
#include "../src/marco_stance.h"
#include "marco_idle_fixture.h"
#ifdef __mips__
#include <libdragon.h>
#endif
int main(void) {
#ifdef __mips__
 debug_init_emulog();console_init();console_set_render_mode(RENDER_MANUAL);
#endif
 unsigned count=sizeof(idle_cases)/sizeof(*idle_cases);
 for(unsigned i=0;i<count;i++) {
  const uint32_t *b=idle_cases[i].before,*e=idle_cases[i].after;
  marco_motion_t m={0};marco_upper_t *u=&m.player.upper;
  m.player.field6c=b[I_F6C];m.player.field9d=b[I_F9D];
  u->input=(marco_input_state_t){b[I_ATTR],b[I_F5F],b[I_F9A],b[I_F9B],b[I_F9C]};
  u->packed=b[I_PACKED];u->gate=(marco_input_gate_t){b[I_GATE_MODE],b[I_GATE_CURRENT],b[I_GATE_LIMIT]};
  assert(marco_stance_idle_branch(&m)==idle_cases[i].branch);
  uint32_t actual[]={u->input.attributes,u->input.flags5f,m.player.field6c,u->input.flags9a,u->input.accumulator9b,u->input.mask9c,m.player.field9d,u->packed,u->gate.mode,u->gate.current,u->gate.limit};
  for(unsigned j=0;j<I_COUNT;j++) assert(actual[j]==e[j]);
 }
 printf("PASS: idle decisions=%u\n",count);
#ifdef __mips__
 debugf("MSX IDLE REGRESSION PASS calls=%u\n",count);console_render();while(1) {}
#endif
 return 0;
}
