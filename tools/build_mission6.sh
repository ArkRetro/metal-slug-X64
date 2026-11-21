#!/bin/sh
# Mission 6 private data and pack (docs/MISION6.md): per stage record (016E5E the coast and the
# bridge, 016E6E the factory, 016E7E the last arena) the ROM grid and the ROM map model of the
# stage's camera list, checked against the observed capture (only the placement check and
# the colours come from it), then romfs/mission6.pack. Usage: tools/build_mission6.sh [CAPTURE=run4]
set -eu
C=${1:-/home/user/captures-fullgame/run4}
# stage dir, descriptor, frames of the observed map, frames of the observed grid, palette RAM snapshot per map
stage() {
 D=filesystem/mission6/$1;mkdir -p $D
 [ -f $D/map_observed.json ] || { python3 tools/extract_mission_map.py $C --ram-layers --allow-backdrop-timeline --skip-fade --frames $3 --output $D/observed;mv $D/observed/map.json $D/map_observed.json;rmdir $D/observed; }
 # (016E5E: 10D7B6 has a collision map too, 0BC6E6: the stage grid is 10C72E's, the second terrain object is phase 2)
 [ -f $D/collision_observed.json ] || python3 tools/extract_region_collision.py $C --rom /home/user/roms/mslugx.zip --descriptor-layers --layer-camera --both-layers foreground --frames $4 --output $D/collision_observed.json
 python3 tools/export_stage_rom.py $2 --output $D/collision.json --observed $D/collision_observed.json --keep-going \
  --model $D/map_observed.json --rom-model $D/map.json --palette-capture $C --palette-frames $5 --report $D/rom_report.json ${6:-}
}
stage s1 16e5e 292683:304270 292738:304200 0BC6BA:294000,0BC6FC:294000,0BC712:293400,0BC728:295200,0BC73E:300000
stage s2 16e6e 304271:322338 304349:322260 0BDB86:310500,0BDB9C:310500
# s3: the last enemy's second form copies the map 04522E (0x1A x 8) into 10D7B6 (neo_m6boss.c k41e02): model layer 2 at x
# 0x4000, as the Mission 4 boss's maps. Its palette number 56 alternates three colour sets (009CCE on a fixed slot, not
# drawn yet): the snapshots 348300 and 348600 hold the same one.
stage s3 16e7e 322339:358800 322416:358800 0BE932:322500,0BE948:322500 "--actor-maps 04522E:1:2:0x4000:348300 --actor-map-frames 348300:348600"
# s3: the five floor patches of 044978 (sub-map 045244), each checked on the dump after it (captures-m6/fam/SJ:
# LOAD=at_340000 LOADF=340000 FROM=346700 DUMP0=346710 LAST=346960 PERIOD=1 RANGE=41500-46800 NAME=SJ sh runS1.sh).
J=${MSX_M6_SJ:-/home/user/captures-m6/fam/SJ}
[ -d $J ] && python3 tools/extract_map_patches.py 6 filesystem/mission6/s3 --ram-colours --patch-frames 0:$J/vram346750.bin,1:$J/vram346790.bin,2:$J/vram346830.bin,3:$J/vram346880.bin,4:$J/vram346920.bin
[ "${MSX_NO_PACK:-0}" = 1 ] || python3 tools/build_packs.py 6   # version 5; after a change of an earlier mission's data: python3 tools/build_packs.py
