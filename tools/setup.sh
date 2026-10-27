#!/usr/bin/env bash
set -euo pipefail
sdk_root="${MSX_SDK_ROOT:-/workspace/toolchains}"
export N64_INST="$sdk_root/package/opt/libdragon"
mkdir -p "$sdk_root"
if [ ! -x "$N64_INST/bin/mips64-elf-gcc" ]; then
    package_file=$(mktemp /tmp/msx-toolchain.XXXXXX.deb)
    trap 'rm -f "$package_file"' EXIT
    curl -fL --retry 2 https://github.com/DragonMinded/libdragon/releases/download/toolchain-continuous-prerelease/gcc-toolchain-mips64-amd64.deb -o "$package_file"
    echo "fac8e6572493a66468b7d45df41b042f90b1a630ec96aa218bd5fb1f320a0a4d  $package_file" | sha256sum -c -
    dpkg-deb -x "$package_file" "$sdk_root/package"
fi
if [ ! -d "$sdk_root/libdragon/.git" ]; then
    git clone https://github.com/DragonMinded/libdragon.git "$sdk_root/libdragon"
    git -C "$sdk_root/libdragon" checkout --detach e356bf3f56f7afbf7e5246329562f145965cfdfc
fi
test "$(git -C "$sdk_root/libdragon" rev-parse HEAD)" = e356bf3f56f7afbf7e5246329562f145965cfdfc
make -C "$sdk_root/libdragon" -j4 install tools-install
printf 'SDK preparado. N64_INST=%s\n' "$N64_INST"
