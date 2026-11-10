#!/bin/sh
# Mission 5 private data and pack (docs/MISION5.md): per stage record (016E2E the street,
# 016E3E the underground, 016E4E the sewer) the ROM grid and the ROM map model of the
# stage's camera list, checked against the observed capture (only the placement check and
# the colours come from it), then romfs/mission5.pack. Usage: tools/build_mission5.sh [CAPTURE=run4]
set -eu
C=${1:-/home/user/captures-fullgame/run4}
# stage dir, descriptor, frames of the observed map, frames of the observed grid, palette RAM snapshot per map
stage() {
 D=filesystem/mission5/$1;mkdir -p $D
 [ -f $D/map_observed.json ] || { python3 tools/extract_mission_map.py $C --ram-layers --allow-backdrop-timeline --skip-fade --frames $3 --output $D/observed;mv $D/observed/map.json $D/map_observed.json;rmdir $D/observed; }
 [ -f $D/collision_observed.json ] || python3 tools/extract_region_collision.py $C --rom /home/user/roms/mslugx.zip --descriptor-layers --layer-camera --frames $4 --output $D/collision_observed.json
 python3 tools/export_stage_rom.py $2 --output $D/collision.json --observed $D/collision_observed.json --keep-going \
  --model $D/map_observed.json --rom-model $D/map.json --palette-capture $C --palette-frames $5 --report $D/rom_report.json
}
stage s1 16e2e 247661:252826 247716:252720 0BB5DE:250020,0BB5F4:250020
stage s2 16e3e 252827:274386 252903:274310 0BB60A:262620
stage s3 16e4e 274387:292600 274462:292000 0BB620:278100
python3 tools/build_packs.py 5   # version 5; after a change of an earlier mission's data: python3 tools/build_packs.py
