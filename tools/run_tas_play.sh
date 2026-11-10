#!/bin/sh
# Usage: tools/run_tas_play.sh OUTPUT_DIR   (MSX_TAS_* options exported; docs/TAS.md)
# Replays the converted TAS (tools/tas_convert.py) on MAME 0.264 from power-on
# with fresh cfg/nvram (as tools/run_capture_fullgame.sh), headless, and the
# wall clock fixed at the movie's RTC InitialTime 2010-01-01T00:00:00.
# MSX_TAS_DEBUG=1 adds the debugger (MSX_TAS_DEBUG_SCRIPT: a debugger script).
set -eu
here=$(cd "$(dirname "$0")" && pwd)
out=$1; mkdir -p "$out"; out=$(cd "$out" && pwd)
rm -rf "$out/cfg" "$out/nvram"; mkdir -p "$out/cfg" "$out/nvram"
ROMS=${MSX_ROMPATH:-/home/user/roms}
: "${MSX_TAS_INPUTS:=/home/user/captures-tas/p1_inputs.txt}"
export MSX_TAS_INPUTS MSX_TAS_OUT=$out
debug=""
if [ "${MSX_TAS_DEBUG:-0}" = 1 ]; then debug="-debug -debugger none -debuglog -debugscript ${MSX_TAS_DEBUG_SCRIPT:-$here/mame/start.cmd}"; fi
# The movie's RTC: MAME's wall clock fixed at InitialTime (MSX_FIXED_TIME).
shim=${MSX_TAS_SHIM:-/home/user/captures-tas/fixed_time.so}
[ -f "$shim" ] || gcc -shared -fPIC -O2 -o "$shim" "$here/mame/fixed_time.c" -ldl
cd "$out"
TZ=UTC LD_PRELOAD=$shim MSX_FIXED_TIME=${MSX_FIXED_TIME:-1262304000} timeout ${MSX_TIMEOUT:-7200} /usr/games/mame mslugx -rompath "$ROMS" -bios euro \
  -video none -sound none $debug -skip_gameinfo -nothrottle -autoboot_delay 0 -joystick_contradictory \
  -autoboot_script "$here/${MSX_TAS_SCRIPT:-tas_play.lua}" \
  -state_directory "${MSX_TAS_STATES:-$out/states}" -cfg_directory "$out/cfg" -nvram_directory "$out/nvram" > "$out/mame.log" 2>&1
