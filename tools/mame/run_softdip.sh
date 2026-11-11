#!/bin/sh
# Usage: tools/mame/run_softdip.sh OUTPUT_DIR   (MSX_DIP, MSX_CAPTURE_END_FRAME as in capture_softdip.lua)
set -eu
here=$(cd "$(dirname "$0")" && pwd)
out=$1; mkdir -p "$out/cfg" "$out/nvram"; out=$(cd "$out" && pwd)
cd "$out"
MSX_CAPTURE_DIR=$out timeout ${MSX_TIMEOUT:-1200} /usr/games/mame mslugx -rompath "${MSX_ROMPATH:-/home/user/roms}" -bios euro \
  -video none -sound none -skip_gameinfo -nothrottle -autoboot_delay 0 \
  -autoboot_script "$here/capture_softdip.lua" \
  -cfg_directory "$out/cfg" -nvram_directory "$out/nvram" > "$out/mame.log" 2>&1
