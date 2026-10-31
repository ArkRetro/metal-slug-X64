/* Closed-loop test of the vulcan turret control (src/neo_vulcan.c, ROM
 * 03B57C..03B808) against every turret update of the weapons family traces
 * camelfam (Camel Slug turret, +72 = 5: 83 bullets 01BE24) and tankfam
 * (SV-001 turret 101784, +72 = 0: 152 bullets 01B6A0):
 * tools/extract_vulcan_fixture.py -> data/vulcan_fixture.json.gz ->
 * tools/compile_vulcan_fixture.py -> build/vulcan_fixture.h.
 *
 * The control fields (+5E, +72..+89: aim, turn speed, burst counters,
 * spread, +88 pose) are native state from the first update of each turret,
 * never reloaded; the other bytes (placement, animation, palette: not this
 * translation) are taken from the original after each update. Inputs are
 * what the update read (vehicle alive and +5F, anchor word, rider, input
 * words, repeat byte, the sweep's spark answer). Every update must leave
 * the control fields as the original did, and fire the same bullets with
 * the same +6E. */
#include <stdio.h>
#include <string.h>
#include "../src/neo_vulcan.h"
typedef struct {
    uint32_t frame; uint8_t source; uint32_t node; uint8_t alive,creator5f; uint16_t anchor; int rider; uint16_t fresh,held; uint8_t repeat;
    int bullet; uint8_t spark,pad; uint8_t pre[0xc0],post[0xc0];
} vulcan_update_t;
#include "vulcan_fixture.h"
static bool control(unsigned o) {return o==0x5e || (o>=0x72 && o<0x8a);}
int main(void) {
    static uint8_t m[0xc0];uint32_t node=0;unsigned n=sizeof(vulcan_updates)/sizeof(vulcan_updates[0]),bullets=0,compared=0,spreads=0,turns=0;int fails=0;
    for (unsigned k=0;k<n;k++) {
        const vulcan_update_t *u=&vulcan_updates[k];
        if (u->node!=node) {node=u->node;memcpy(m,u->pre,0xc0);}
        /* Untranslated bytes as the original left them before this update. */
        for (unsigned o=0;o<0xc0;o++) if (!control(o)) m[o]=u->pre[o];
        neo_vulcan_in_t in={u->alive!=0,u->creator5f,u->anchor,u->rider,u->fresh,u->held,u->repeat!=0,vulcan_aim[m[0x72]<6?m[0x72]:0]};
        neo_vulcan_out_t out;bool spark;uint16_t before=(uint16_t)(m[0x7c]<<8|m[0x7d]);
        neo_vulcan_step(m,&in,u->spark!=0,&out,&spark);
        if (out.state==NEO_VULCAN_GONE) {fprintf(stderr,"frame %u: turret gone (03B86E, not in the traces)\n",u->frame);fails++;break;}
        if (out.fire!=(u->bullet>=0) || (out.fire && out.bullet6e!=(uint8_t)u->bullet)) {
            fprintf(stderr,"frame %u: fire %d +6E %02x, original %d %02x\n",u->frame,out.fire,out.bullet6e,u->bullet>=0,(unsigned)(uint8_t)u->bullet);fails++;
        }
        if (out.fire) {bullets++;if (out.bullet6e!=out.heading) spreads++;}
        if (spark!=(u->spark!=0)) {fprintf(stderr,"frame %u: spark %d\n",u->frame,spark);fails++;}
        if ((uint16_t)(m[0x7c]<<8|m[0x7d])!=before) turns++;
        for (unsigned o=0;o<0xc0;o++) if (control(o)) {
            if (m[o]!=u->post[o]) {fprintf(stderr,"frame %u node %06x: +%02X = %02x, original %02x\n",u->frame,(unsigned)u->node,o,m[o],u->post[o]);fails++;}
            compared++;
        }
        if (fails>20) break;
    }
    if (fails) {fprintf(stderr,"FAIL: vulcan turret, %d differences\n",fails);return 1;}
    printf("PASS: vulcan turret control (%s): %u closed-loop updates, %u bullets (%u spread +-2), %u turning updates, %u control bytes\n",VULCAN_SOURCES,n,bullets,spreads,turns,compared);
    return 0;
}
