#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
# Uses the tested emulator retained in this cloud workspace. Requires X11.
export DISPLAY="${DISPLAY:-:99}"
export XDG_DATA_HOME=/workspace/emulator/data146
export XDG_CONFIG_HOME=/workspace/emulator/config146
export XDG_RUNTIME_DIR=/workspace/emulator/runtime
export LD_LIBRARY_PATH=/workspace/emulator/root/usr/lib/x86_64-linux-gnu
export VK_DRIVER_FILES=/workspace/emulator/root/usr/share/vulkan/icd.d/lvp_icd.json
export LIBGL_ALWAYS_SOFTWARE=1
exec /workspace/emulator/build/rundir/bin/ares --setting 'Video/Driver=OpenGL 3.2' \
    --setting 'General/HomebrewMode=true' --system 'Nintendo 64' --no-file-prompt \
    "$PWD/mslugx-explorer.z64"
