#!/bin/sh
# Content stamp of the private Marco inputs: BUILD/marco-inputs.sum holds
# the digest of BUILD/marco_pose_meta.h, BUILD/marco_campaign.h and
# romfs/marco/*.sprite and is rewritten only when that content changes.
# The Makefile runs it at parse time and the objects that include those
# headers (and the romfs stamps) depend on it, so inputs copied with their
# old times (cp -p, rsync -a) still rebuild them. Without it ROM 18 linked a
# marco_renderer.o compiled against the 411-pose catalog with the sprites of
# the 562-pose one: the console rejected every part (six-state fallback).
# Usage: marco_inputs_sum.sh BUILD_DIR
set -e
b=${1:-build}
mkdir -p "$b"
new="$b/marco-inputs.sum.new"
{
    for f in "$b/marco_pose_meta.h" "$b/marco_campaign.h"; do
        if [ -f "$f" ]; then cksum < "$f"; else echo "none $f"; fi
    done
    if [ -d romfs/marco ]; then ls romfs/marco | LC_ALL=C sort | (cd romfs/marco && xargs cat) | cksum; else echo "none romfs/marco"; fi
} > "$new"
if cmp -s "$new" "$b/marco-inputs.sum"; then rm -f "$new"; else mv -f "$new" "$b/marco-inputs.sum"; fi
