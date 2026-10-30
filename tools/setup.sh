#!/usr/bin/env bash
set -euo pipefail
sdk_root="${MSX_SDK_ROOT:-/workspace/toolchains}"
repo_dir="$(cd "$(dirname "$0")/.." && pwd)"
export N64_INST="$sdk_root/package/opt/libdragon"
# libdragon pin: this commit plus the patches in tools/libdragon-patches
# (upstream rspq fixes not yet in trunk; see docs/RENDIMIENTO.md, "Cuelgue del RSP").
libdragon_base=e356bf3f56f7afbf7e5246329562f145965cfdfc
libdragon_patches=2
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
    git -C "$sdk_root/libdragon" checkout --detach "$libdragon_base"
fi
# A tree still at the base commit (fresh clone or an SDK prepared before the
# patches existed) gets the patches applied once, on a local branch.
if [ "$(git -C "$sdk_root/libdragon" rev-parse HEAD)" = "$libdragon_base" ]; then
    git -C "$sdk_root/libdragon" checkout -q -B msx-pin "$libdragon_base"
    git -C "$sdk_root/libdragon" -c user.name=msx-setup -c user.email=msx-setup@localhost \
        am "$repo_dir"/tools/libdragon-patches/*.patch
fi
test "$(git -C "$sdk_root/libdragon" merge-base HEAD "$libdragon_base")" = "$libdragon_base"
test "$(git -C "$sdk_root/libdragon" rev-list --count "$libdragon_base..HEAD")" = "$libdragon_patches"
make -C "$sdk_root/libdragon" -j4 install tools-install
printf 'SDK preparado. N64_INST=%s (libdragon %s + %s parches)\n' "$N64_INST" "${libdragon_base:0:7}" "$libdragon_patches"
