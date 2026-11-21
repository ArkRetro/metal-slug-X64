#!/bin/sh
# Mission 3 map reconnaissance capture (docs/MISION3.md "Reconocimiento del mapa"):
# tools/capture_fullgame.lua from states4/sec04_117966 with the cell tap and the
# player sweep / 0C70FC / 0C88B0 breakpoints. Usage: tools/capture_m3_map.sh OUT [FIRST LAST]
OUT=$1;LO=${2:-117966};HI=${3:-149300}
export MSX_FULL_LOAD=sec04_117966 MSX_FULL_LOAD_FRAME=117966 MSX_FULL_STATES=${MSX_FULL_STATES:-/home/user/captures-fullgame/states4}
export MSX_FULL_FIRST_DUMP=9999999 MSX_FULL_GRID_TAP=0 MSX_FULL_LAST=$HI MSX_FULL_CELL_TAP=$LO-$HI MSX_FULL_DEBUG=1 MSX_FULL_NO_PROBES=1
IN1='printf "IN f=%d d0=%08X d1=%08X d2=%08X d3=%08X a0=%08X m4004=%08X b4008=%02X b4009=%02X b400a=%02X b400b=%02X b400c=%02X b400d=%02X w4068=%04X w4018=%04X",temp9,d0,d1,d2,d3,a0,d@10c004,b@10c008,b@10c009,b@10c00a,b@10c00b,b@10c00c,b@10c00d,w@10c068,w@10c018'
IN2='printf " box=%04X,%04X,%04X,%04X shake=%08X t=%08X,%08X,%08X,%08X",w@10c01c,w@10c01e,w@10c020,w@10c022,d@10e9cc,d@10c742,d@10c746,d@10c74a,d@10c74e'
IN3='printf " r=%08X,%08X,%08X,%08X m=%08X,%08X,%08X,%08X o=%04X g=%08X mm=%08X",d@10e97c,d@10e980,d@10e984,d@10e988,d@10d7ca,d@10d7ce,d@10d7d2,d@10d7d6,w@10d7e8,d@10c00e,d@10d7e4'
DYN='printf " s0=%08X,%08X,%08X,%08X,%08X s1=%08X,%08X,%08X,%08X,%08X s2=%08X,%08X,%08X,%08X,%08X",d@10c094,d@10c082,d@10c086,d@10c08a,d@10c08e,d@10c0d4,d@10c0c2,d@10c0c6,d@10c0ca,d@10c0ce,d@10c114,d@10c102,d@10c106,d@10c10a,d@10c10e;printf " s3=%08X,%08X,%08X,%08X,%08X s4=%08X,%08X,%08X,%08X,%08X s5=%08X,%08X,%08X,%08X,%08X",d@10c154,d@10c142,d@10c146,d@10c14a,d@10c14e,d@10c194,d@10c182,d@10c186,d@10c18a,d@10c18e,d@10c1d4,d@10c1c2,d@10c1c6,d@10c1ca,d@10c1ce;printf " s6=%08X,%08X,%08X,%08X,%08X s7=%08X,%08X,%08X,%08X,%08X\n",d@10c214,d@10c202,d@10c206,d@10c20a,d@10c20e,d@10c254,d@10c242,d@10c246,d@10c24a,d@10c24e'
OUTC='printf "OUT f=%d d0=%08X d1=%08X d2=%08X d3=%08X a0=%08X d4=%08X d6=%08X d7=%08X\n",temp9,d0,d1,d2,d3,a0,d4,d6,d7'
PAT='printf "PATCH f=%d a0=%08X a1=%08X d0=%04X d1=%04X d2=%04X d3=%04X d4=%04X d5=%04X a6=%08X\n",temp9,a0,a1,d0&ffff,d1&ffff,d2&ffff,d3&ffff,d4&ffff,d5&ffff,a6'
STA='printf "STAMP f=%d a0=%08X a1=%08X a2=%08X d0=%08X d1=%08X d2=%08X d3=%08X a6=%08X\n",temp9,a0,a1,a2,d0,d1,d2,d3,a6'
export MSX_FULL_DEBUG_CMDS="bpset 1448c,a6==1014c4,{$IN1;$IN2;$IN3;$DYN;g}|bpset 14492,a6==1014c4,{$OUTC;g}|bpset c70fc,1,{$PAT;g}|bpset c88b0,1,{$STA;g}"
exec sh "$(dirname "$0")/run_capture_fullgame.sh" $OUT
