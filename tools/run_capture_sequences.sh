#!/bin/sh
# Usage: MSX_SEQ_MODE=attract|gameover tools/run_capture_sequences.sh OUTPUT_DIR
set -eu
here=$(cd "$(dirname "$0")" && pwd)
out=$1; mkdir -p "$out" "$out/cfg" "$out/nvram"; out=$(cd "$out" && pwd)
ROMS=${MSX_ROMPATH:-/home/user/roms}
cd "$out"
MSX_CAPTURE_DIR=$out timeout ${MSX_TIMEOUT:-3600} /usr/games/mame mslugx -rompath "$ROMS" -bios euro \
  -video none -sound none -skip_gameinfo -nothrottle -autoboot_delay 0 \
  -autoboot_script "$here/capture_sequences.lua" \
  -cfg_directory "$out/cfg" -nvram_directory "$out/nvram" > "$out/mame.log" 2>&1
