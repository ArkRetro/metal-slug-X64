#!/bin/sh
# Usage: tools/run_capture_fullgame.sh OUTPUT_DIR (MSX_FULL_* options exported)
# Runs tools/capture_fullgame.lua headless with fresh cfg/nvram. Historical
# references use MAME 0.264; saved states must match the configured version.
# MSX_MAME overrides the system/workspace MAME executable.
set -eu
here=$(cd "$(dirname "$0")" && pwd)
out=${1:?Usage: tools/run_capture_fullgame.sh OUTPUT_DIR}
if [ -n "${MSX_MAME:-}" ]; then
  mame=$MSX_MAME
elif [ -x /usr/games/mame ]; then
  mame=/usr/games/mame
elif [ -x /workspace/emulator/root/usr/games/mame ]; then
  mame=/workspace/emulator/root/usr/games/mame
else
  mame=mame
fi
if ! mame=$(command -v "$mame"); then
  printf '%s\n' 'MAME executable not found; set MSX_MAME to an installed executable.' >&2
  exit 1
fi
# Resolve relative overrides before changing into the capture directory.
case $mame in
  /*) ;;
  *) mame=$(cd "$(dirname "$mame")" && pwd)/$(basename "$mame");;
esac
if [ ! -x "$mame" ]; then
  printf '%s\n' "MAME executable is not executable: $mame" >&2
  exit 1
fi
case $mame in
  /workspace/emulator/root/usr/games/mame)
    LD_LIBRARY_PATH=/workspace/emulator/root/usr/lib/x86_64-linux-gnu${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}
    export LD_LIBRARY_PATH;;
esac
mkdir -p "$out" "$out/cfg" "$out/nvram"; out=$(cd "$out" && pwd)
ROMS=${MSX_ROMPATH:-/home/user/roms}
debug=""
if [ "${MSX_FULL_DEBUG:-0}" = 1 ]; then debug="-debug -debugger none -debuglog -debugscript $here/mame/start.cmd"; fi
cd "$out"
MSX_CAPTURE_DIR=$out timeout ${MSX_TIMEOUT:-7200} "$mame" mslugx -rompath "$ROMS" -bios euro \
  -video none -sound none $debug -skip_gameinfo -nothrottle -autoboot_delay 0 \
  -autoboot_script "$here/${MSX_FULL_SCRIPT:-capture_fullgame.lua}" \
  -state_directory "${MSX_FULL_STATES:-$out/states}" -cfg_directory "$out/cfg" -nvram_directory "$out/nvram" > "$out/mame.log" 2>&1
