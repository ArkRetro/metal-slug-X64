/* Player 1's port block 10C35A (src/neo_end.c neo_end_port_block, ROM 03CC10..03CC54) against the MAME capture of the
 * name entry with the stick and the buttons (captures-m6/fam/NE1; tools/extract_end_ports_fixture.py): every update of
 * the name entry, the new presses 10C35D and, when the node read them, the repeat byte 10C35E and its count 10C35F. */
#include <stdio.h>
#include "../src/neo_end.h"
#include "end_ports_fixture.h"
static uint8_t ram[0x10000];
static uint8_t rd(void *c,uint32_t ad) {(void)c;return ram[ad&0xffffu];}
static void wr(void *c,uint32_t ad,uint8_t v) {(void)c;ram[ad&0xffffu]=v;}
int main(void) {
    neo_end_ram_t m={0};m.rd=rd;m.wr=wr;
    ram[0xc35c]=END_PORTS_HELD_BEFORE;ram[0xc35f]=END_PORTS_COUNT_BEFORE;ram[0xc380]=END_PORTS_DELAY;ram[0xc381]=END_PORTS_REPEAT;   /* 03CAB4(0x2004) */
    unsigned bad=0,news=0,reps=0,read=0,held=0;
    for (unsigned i=0;i<END_PORTS_ROWS;i++) {
        const int *r=end_ports_rows[i];
        neo_end_port_block(&m,0x10c35a,(uint8_t)r[1]);
        if (r[1]) held++;
        if (ram[0xc35d]!=r[2]) {if (bad++<8) fprintf(stderr,"update %d: new presses %02x, MAME %02x\n",r[0],ram[0xc35d],r[2]);}
        if (r[2]) news++;
        if (r[3]>=0) {
            read++;
            if (ram[0xc35e]!=r[3] || ram[0xc35f]!=r[4]) {if (bad++<8) fprintf(stderr,"update %d: repeat %02x count %02x, MAME %02x %02x\n",r[0],ram[0xc35e],ram[0xc35f],r[3],r[4]);}
            else if (r[3] && !r[2]) reps++;
        }
    }
    if (bad) {fprintf(stderr,"FAIL: end ports: %u differences\n",bad);return 1;}
    printf("PASS: end ports (03CC10): %u updates of the name entry equal to MAME (%u with a held byte, %u new presses, %u repeat bytes and counts read, %u repeats)\n",(unsigned)END_PORTS_ROWS,held,news,read,reps);
    return 0;
}
