#!/bin/sh
# Usage: tools/run_capture_fat_weapons.sh OUTPUT_DIR STATE_DIR (MSX_FAT_* exported)
# Runs tools/capture_fat_weapons.lua on MAME 0.264 from a saved state with
# the debugger on (sound and allocation lines go to OUTPUT_DIR/debug.log).
set -eu
here=$(cd "$(dirname "$0")" && pwd)
out=$1; mkdir -p "$out" "$out/cfg" "$out/nvram"; out=$(cd "$out" && pwd)
ROMS=${MSX_ROMPATH:-/home/user/roms}
cd "$out"
MSX_CAPTURE_DIR=$out timeout ${MSX_TIMEOUT:-3600} /usr/games/mame mslugx -rompath "$ROMS" -bios euro \
  -video none -sound none -debug -debugger none -debuglog -debugscript "$here/mame/start.cmd" \
  -skip_gameinfo -nothrottle -autoboot_delay 0 -autoboot_script "$here/capture_fat_weapons.lua" \
  -state_directory "$2" -cfg_directory "$out/cfg" -nvram_directory "$out/nvram" > "$out/mame.log" 2>&1
