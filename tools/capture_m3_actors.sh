#!/bin/sh
# Mission 3 actor inventory capture (docs/MISION3_ACTORES.md): tools/capture_fullgame.lua
# from states4/sec04_117966 (frame-exact replay of run4) to the start of Mission 4,
# with debugger probes only (MSX_FULL_NO_PROBES=1, no dumps, no grid tap):
#  ALLOC 0040BC node allocation (at 00410E: node, routine, anchor node + routine; through
#        00411C (ret 004122) the real creator is o = the A6 it saved; reading memory at
#        an arbitrary pointer here (e.g. its routine) changed the run from 138446 on: only
#        RAM/stack reads in these probes)
#  FREE  004126 node release        SPAWN 01A132 script op 7 (record, creator)
#  OP    01A0A6 non-blocking script op   WAIT 019F7E blocking op of the main script (101414), on change
#  ANIM  004256 sprite program start (a1)   SND 0120F8 sound request (d0)
#  ITEM  08C380 item kind +6E at its constructor   CAM write of the camera list pointer 10E964
#  END   018F92 / 018870 mission end
# Usage: tools/capture_m3_actors.sh OUT [FIRST LAST]; then tools/compare_m3_events.py OUT/debug.log
OUT=$1;LO=${2:-117966};HI=${3:-149830}
export MSX_FULL_LOAD=sec04_117966 MSX_FULL_LOAD_FRAME=117966 MSX_FULL_STATES=${MSX_FULL_STATES:-/home/user/captures-fullgame/states4}
export MSX_FULL_FIRST_DUMP=9999999 MSX_FULL_GRID_TAP=0 MSX_FULL_LAST=$HI MSX_FULL_DEBUG=1 MSX_FULL_NO_PROBES=1
C='cam=%d'
A="bpset 410e,1,{printf \"ALLOC f=%d n=%08X r=%08X c=%08X cr=%08X ret=%08X o=%08X $C\n\",temp9,a0,d@(sp),a6,d@a6,d@(sp+4),d@(sp+8),w@10e99e;g}"
F="bpset 4126,1,{printf \"FREE f=%d n=%08X r=%08X\n\",temp9,a6,d@a6;g}"
S="bpset 1a132,1,{printf \"SPAWN f=%d n=%08X entry=%08X rec=%08X cr=%08X $C\n\",temp9,a0,d@(sp),d@(sp+4),a6,w@10e99e;g}"
O="bpset 1a0a6,1,{printf \"OP f=%d n=%08X a0=%08X op=%02X $C\n\",temp9,a6,a0,b@a0,w@10e99e;g}"
W="bpset 19f7e,(a6==101414)&&(a0!=temp8),{temp8=a0;printf \"WAIT f=%d n=%08X a0=%08X op=%02X $C\n\",temp9,a6,a0,b@a0,w@10e99e;g}"
N="bpset 4256,1,{printf \"ANIM f=%d n=%08X r=%08X p=%08X\n\",temp9,a6,d@a6,a1;g}"
D="bpset 120f8,1,{printf \"SND f=%d n=%08X r=%08X w=%04X ret=%08X\n\",temp9,a6,d@a6,d0&ffff,d@(sp);g}"
I="bpset 8c380,1,{printf \"ITEM f=%d n=%08X k=%04X c=%08X d=%02X\n\",temp9,a6,w@(a6+6e),d@(a6+c),b@(a6+8a);g}"
M="wpset 10e964,4,w,1,{printf \"CAM f=%d v=%08X $C\n\",temp9,d@10e964,w@10e99e;g}"
E="bpset 18f92,1,{printf \"END f=%d pc=018F92 n=%08X r=%08X $C\n\",temp9,a6,d@a6,w@10e99e;g}|bpset 18870,1,{printf \"END f=%d pc=018870 n=%08X r=%08X $C\n\",temp9,a6,d@a6,w@10e99e;g}"
export MSX_FULL_DEBUG_CMDS="$A|$F|$S|$O|$W|$N|$D|$I|$M|$E"
exec sh "$(dirname "$0")/run_capture_fullgame.sh" $OUT
