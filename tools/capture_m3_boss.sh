#!/bin/sh
# Mission 3 boss captures (docs/JEFE_MISION3.md): MAME 0.264, tools/capture_fullgame.lua.
#
#  tools/capture_m3_boss.sh states OUT
#      Frame-exact replay of run4 from states4/sec04_117966 (as capture_m3_actors.sh) to
#      144410, saving the small states at_132300 (before the mid-boss 0A4A94) and
#      at_144400 (before the boss 04A2C2) in $MSX_M3BOSS_STATES (~25 KB each).
#  tools/capture_m3_boss.sh boss OUT [FROM LAST]     (default 144410 149300)
#  tools/capture_m3_boss.sh mid  OUT [FROM LAST]     (default 132310 141100)
#      Load at_144400 (boss) / sec04_117966 (mid: at_132300 with a fresh Lua gives
#      another fight, 3500 frames instead of run4's 8615) and write the family trace (OUT/boss-trace.txt:
#      U/V/R/W/X/A/B/F lines, Z with MSX_FULL_VRAM_TRACE=1) from FROM to LAST, plus
#      debugger lines in OUT/debug.log (RAM/stack reads only: reading memory at an
#      arbitrary pointer from a breakpoint changed run4 in an earlier capture):
#        ALLOC 00410E node allocation (node, constructor, A6, saved A6 = creator)
#        FREE 004126   ANIM 004256 program start   SND 0120F8 sound word
#        END 018F92 / 018F40 / 018870 (mission end, stage done, mission end flow)
#      Every boss/mid tranche loads the same state, so all tranches belong to one
#      (deterministic) run; with a fresh Lua the input plan counters restart at the
#      load, so that run is not run4 (frames are close, see the doc).
#      MSX_M3BOSS_DUMPS=FRAME: vram/ram/palette dumps every 30 frames from FRAME (sprite export).
#      MSX_M3BOSS_FILTER=1: boss-trace.txt is a FIFO filtered to U/V/B/X/A/F lines
#      (research: a few MB instead of ~60 MB per 1000 frames; not usable for fixtures,
#      which need R/W: tools/extract_family_fixture.py).
set -eu
here=$(cd "$(dirname "$0")" && pwd)
mode=$1;OUT=$2
ST=${MSX_M3BOSS_STATES:-/home/user/captures-m3boss/states}
mkdir -p "$ST/mslugx" "$OUT"
[ -f "$ST/mslugx/sec04_117966.sta" ] || cp /home/user/captures-fullgame/states4/mslugx/sec04_117966.sta "$ST/mslugx/"
export MSX_FULL_STATES=$ST MSX_FULL_GRID_TAP=0 MSX_FULL_FIRST_DUMP=${MSX_M3BOSS_DUMPS:-9999999}
case $mode in
states)
  export MSX_FULL_LOAD=sec04_117966 MSX_FULL_LOAD_FRAME=117966 MSX_FULL_SAVE_AT=132300,144400 MSX_FULL_LAST=144410
  exec sh "$here/run_capture_fullgame.sh" "$OUT";;
boss) export MSX_FULL_LOAD=at_144400 MSX_FULL_LOAD_FRAME=144400 MSX_FULL_FAMILY_ROOTS=4a2c2 MSX_FULL_FAMILY_RANGE=4a2c2-4e577
  FROM=${3:-144410};LAST=${4:-149300};;
mid) export MSX_FULL_LOAD=sec04_117966 MSX_FULL_LOAD_FRAME=117966 MSX_FULL_FAMILY_ROOTS=a4a94 MSX_FULL_FAMILY_RANGE=a4a94-a6ce9
  FROM=${3:-132310};LAST=${4:-141100};;
*) echo "usage: $0 states|boss|mid OUT [FROM LAST]" >&2;exit 2;;
esac
export MSX_FULL_FAMILY_FROM=$FROM MSX_FULL_LAST=$LAST MSX_FULL_DEBUG=1 MSX_FULL_NO_PROBES=1
A="bpset 410e,1,{printf \"ALLOC f=%d n=%08X r=%08X c=%08X cr=%08X ret=%08X o=%08X\n\",temp9,a0,d@(sp),a6,d@a6,d@(sp+4),d@(sp+8);g}"
F="bpset 4126,1,{printf \"FREE f=%d n=%08X r=%08X\n\",temp9,a6,d@a6;g}"
N="bpset 4256,1,{printf \"ANIM f=%d n=%08X r=%08X p=%08X\n\",temp9,a6,d@a6,a1;g}"
D="bpset 120f8,1,{printf \"SND f=%d n=%08X r=%08X w=%04X ret=%08X\n\",temp9,a6,d@a6,d0&ffff,d@(sp);g}"
E="bpset 18f92,1,{printf \"END f=%d pc=018F92 n=%08X r=%08X\n\",temp9,a6,d@a6;g}|bpset 18f40,1,{printf \"END f=%d pc=018F40 n=%08X r=%08X\n\",temp9,a6,d@a6;g}|bpset 18870,1,{printf \"END f=%d pc=018870 n=%08X r=%08X\n\",temp9,a6,d@a6;g}"
export MSX_FULL_DEBUG_CMDS="$A|$F|$N|$D|$E"
if [ "${MSX_M3BOSS_FILTER:-0}" = 1 ]; then
  rm -f "$OUT/boss-trace.txt";mkfifo "$OUT/boss-trace.txt"
  grep -a -E '^[UVBXAF] ' < "$OUT/boss-trace.txt" > "$OUT/boss-uv.txt" &
fi
sh "$here/run_capture_fullgame.sh" "$OUT"
wait
