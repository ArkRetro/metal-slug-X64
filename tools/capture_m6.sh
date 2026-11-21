#!/bin/sh
# Mission 6 capture (docs/MISION6.md), one MAME 0.264 run per stage record:
# tools/capture_fullgame.lua from the section states of run4 (states4/sec09_292683,
# sec10_304271, sec11_322339; frame-exact replay of run4, checked against its trace.csv
# by tools/compare_m5_replay.py), everything at once:
#  debug.log   the probes of tools/capture_m4_actors.sh (ALLOC, FREE, SPAWN, OP, WAIT, ANIM,
#              SND, ITEM, CAM, END) and of tools/capture_m4_map.sh (PATCH 0C70FC, STAMP 0C88B0)
#  cells.txt   every cell 0C79FE read from a layer ring (MSX_FULL_CELL_TAP)
#  events.csv  every write to the rings with its PC (MSX_FULL_GRID_ALL=1)
#  camera.csv  the camera block and the layer objects per camera update (MSX_FULL_CAMERA=1)
# MSX_M6_LATCH=1: debug.log with SND and LATCH (every byte written to the sound latch 320000) only.
# Usage: tools/capture_m6.sh STAGE(9|10|11) OUT [LAST]; MSX_FULL_SAVE_AT=f,f,... saves states at_<f>
S=$1;OUT=$2
case $S in
 9) LO=292683;HI=${3:-304270};;
 10) LO=304271;HI=${3:-322338};;
 11) LO=322339;HI=${3:-358820};;
 *) echo "stage 9, 10 or 11";exit 2;;
esac
export MSX_FULL_LOAD=sec$(printf %02d $S)_$LO MSX_FULL_LOAD_FRAME=$LO MSX_FULL_STATES=${MSX_FULL_STATES:-/home/user/captures-m6/states}
export MSX_FULL_LOAD_SECTION=1 MSX_FULL_LOAD_BLOCKED=${MSX_FULL_LOAD_BLOCKED:-20}
export MSX_FULL_FIRST_DUMP=${MSX_FULL_FIRST_DUMP:-9999999} MSX_FULL_GRID_ALL=1 MSX_FULL_CAMERA=1 MSX_FULL_LAST=$HI MSX_FULL_CELL_TAP=$LO-$HI MSX_FULL_DEBUG=1 MSX_FULL_NO_PROBES=1
C='cam=%d'
A="bpset 410e,1,{printf \"ALLOC f=%d n=%08X r=%08X c=%08X cr=%08X ret=%08X o=%08X $C\n\",temp9,a0,d@(sp),a6,d@a6,d@(sp+4),d@(sp+8),w@10e99e;g}"
F="bpset 4126,1,{printf \"FREE f=%d n=%08X r=%08X\n\",temp9,a6,d@a6;g}"
P="bpset 1a132,1,{printf \"SPAWN f=%d n=%08X entry=%08X rec=%08X cr=%08X $C\n\",temp9,a0,d@(sp),d@(sp+4),a6,w@10e99e;g}"
O="bpset 1a0a6,1,{printf \"OP f=%d n=%08X a0=%08X op=%02X $C\n\",temp9,a6,a0,b@a0,w@10e99e;g}"
W="bpset 19f7e,(a6==101414)&&(a0!=temp8),{temp8=a0;printf \"WAIT f=%d n=%08X a0=%08X op=%02X $C\n\",temp9,a6,a0,b@a0,w@10e99e;g}"
N="bpset 4256,1,{printf \"ANIM f=%d n=%08X r=%08X p=%08X\n\",temp9,a6,d@a6,a1;g}"
D="bpset 120f8,1,{printf \"SND f=%d n=%08X r=%08X w=%04X ret=%08X\n\",temp9,a6,d@a6,d0&ffff,d@(sp);g}"
I="bpset 8c380,1,{printf \"ITEM f=%d n=%08X k=%04X c=%08X d=%02X\n\",temp9,a6,w@(a6+6e),d@(a6+c),b@(a6+8a);g}"
M="wpset 10e964,4,w,1,{printf \"CAM f=%d v=%08X $C\n\",temp9,d@10e964,w@10e99e;g}"
E="bpset 18f92,1,{printf \"END f=%d pc=018F92 n=%08X r=%08X $C\n\",temp9,a6,d@a6,w@10e99e;g}|bpset 18870,1,{printf \"END f=%d pc=018870 n=%08X r=%08X $C\n\",temp9,a6,d@a6,w@10e99e;g}"
PAT='printf "PATCH f=%d a0=%08X a1=%08X d0=%04X d1=%04X d2=%04X d3=%04X d4=%04X d5=%04X a6=%08X\n",temp9,a0,a1,d0&ffff,d1&ffff,d2&ffff,d3&ffff,d4&ffff,d5&ffff,a6'
# STAMP2: the second template stamper 0C8A3C (the Mission 5 sewer's 0x1F gate walls; docs/MISION5.md pending 2)
STA2='printf "STAMP2 f=%d a0=%08X a1=%08X a2=%08X d0=%08X d1=%08X d2=%08X d3=%08X a6=%08X r=%08X ret=%08X\n",temp9,a0,a1,a2,d0,d1,d2,d3,a6,d@a6,d@(sp)'
STA='printf "STAMP f=%d a0=%08X a1=%08X a2=%08X d0=%08X d1=%08X d2=%08X d3=%08X a6=%08X\n",temp9,a0,a1,a2,d0,d1,d2,d3,a6'
# MSX_M6_LATCH=1: only the sound requests (SND) and every byte the 68000 writes to the sound latch 320000
# (LATCH: bank bytes, music, fades), without the taps (a light run; docs/MISION6.md "Sonido").
L='wpset 320000:maincpu,1,w,1,{printf "LATCH f=%d pc=%06X v=%02X\n",temp9,curpc,wpdata;g}'
if [ "${MSX_M6_LATCH:-0}" = 1 ]; then
 unset MSX_FULL_GRID_ALL MSX_FULL_CAMERA MSX_FULL_CELL_TAP;export MSX_FULL_GRID_TAP=0 MSX_FULL_DEBUG_CMDS="$D|$L"
else
 export MSX_FULL_DEBUG_CMDS="$A|$F|$P|$O|$W|$N|$D|$I|$M|$E|bpset c70fc,1,{$PAT;g}|bpset c88b0,1,{$STA;g}|bpset c8a3c,1,{$STA2;g}|$L"
fi
exec sh "$(dirname "$0")/run_capture_fullgame.sh" $OUT
