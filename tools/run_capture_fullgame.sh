#!/bin/sh
# Usage: tools/run_capture_fullgame.sh OUTPUT_DIR (MSX_FULL_* options exported)
# Runs tools/capture_fullgame.lua headless on MAME 0.264 with fresh cfg/nvram.
set -eu
here=$(cd "$(dirname "$0")" && pwd)
out=$1; mkdir -p "$out" "$out/cfg" "$out/nvram"; out=$(cd "$out" && pwd)
ROMS=${MSX_ROMPATH:-/home/user/roms}
debug=""
if [ "${MSX_FULL_DEBUG:-0}" = 1 ]; then debug="-debug -debugger none -debuglog -debugscript $here/mame/start.cmd"; fi
cd "$out"
MSX_CAPTURE_DIR=$out timeout ${MSX_TIMEOUT:-7200} /usr/games/mame mslugx -rompath "$ROMS" -bios euro \
  -video none -sound none $debug -skip_gameinfo -nothrottle -autoboot_delay 0 \
  -autoboot_script "$here/${MSX_FULL_SCRIPT:-capture_fullgame.lua}" \
  -state_directory "${MSX_FULL_STATES:-$out/states}" -cfg_directory "$out/cfg" -nvram_directory "$out/nvram" > "$out/mame.log" 2>&1
