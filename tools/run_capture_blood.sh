#!/bin/sh
# Usage: tools/run_capture_blood.sh OUTPUT_DIR [extra env already exported]
# MSX_BLOOD_PALREQ=1 adds the debugger (debug.log: every 009EA8 request).
# Runs tools/capture_blood.lua headless on MAME 0.264 with fresh cfg/nvram.
set -eu
here=$(cd "$(dirname "$0")" && pwd)
out=$1; mkdir -p "$out" "$out/cfg" "$out/nvram"; out=$(cd "$out" && pwd)
ROMS=${MSX_ROMPATH:-/home/user/roms}
debug=""
if [ "${MSX_REST_DEBUG:-0}" = 1 ] || [ "${MSX_BLOOD_PALREQ:-0}" = 1 ]; then debug="-debug -debugger none -debuglog -debugscript $here/mame/start.cmd"; fi
cd "$out"
MSX_CAPTURE_DIR=$out timeout ${MSX_TIMEOUT:-3600} /usr/games/mame mslugx -rompath "$ROMS" -bios euro \
  -video none -sound none $debug -skip_gameinfo -nothrottle -autoboot_delay 0 \
  -autoboot_script "$here/capture_blood.lua" \
  -cfg_directory "$out/cfg" -nvram_directory "$out/nvram" > "$out/mame.log" 2>&1
