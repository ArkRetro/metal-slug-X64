#!/bin/sh
# Mission 4 private data and pack (docs/MISION4.md): the ROM grid and ROM map
# model of the stage 016E1E, checked against the observed capture, then
# romfs/mission4.pack. Usage: tools/build_mission4.sh [CAPTURE=run4]
set -eu
C=${1:-/home/user/captures-fullgame/run4};D=filesystem/mission4/s1;mkdir -p $D
# Observed model and grid (only to check the ROM placement and to take the colours):
[ -f $D/map_observed.json ] || python3 tools/extract_mission_map.py $C --ram-layers --allow-backdrop-timeline --skip-fade --frames 149829:246904 --output $D/observed
[ -f $D/map_observed.json ] || { mv $D/observed/map.json $D/map_observed.json; rmdir $D/observed; }
[ -f $D/collision_observed.json ] || python3 tools/extract_region_collision.py $C --rom /home/user/roms/mslugx.zip --descriptor-layers --layer-camera --frames 149829:157580 --output $D/collision_observed.json
python3 tools/export_stage_rom.py 16e1e --output $D/collision.json --observed $D/collision_observed.json --keep-going \
  --model $D/map_observed.json --rom-model $D/map.json --camera-max 5808 --outside-limit 6144 \
  --palette-capture $C --palette-frames 0E6D7C:151500,0E6DA8:156000,0E6DD4:154500 --report $D/rom_report.json
python3 tools/extract_map_patches.py 4 $D --palette-capture $C --palette-frames 0E6D7C:151500,0E6DA8:156000 > build/mission4_patches_report.json   # the props' patches 0C70FC (neo_m4props.c)
python3 tools/build_packs.py 4   # version 5; after a change of Mission 2 or 3 data: python3 tools/build_packs.py
