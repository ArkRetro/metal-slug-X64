#!/bin/sh
# Regression test for the six-state fallback of test ROM 18: the private
# Marco inputs copied with their old times must still rebuild the objects
# that include them (tools/marco_inputs_sum.sh, Makefile MARCO_INPUTS_SUM).
set -e
T=$(mktemp -d)
trap 'rm -rf "$T"' EXIT
fail() { echo "FAIL: $*"; exit 1; }
mt() { stat -c %Y "$1"; }
printf 'A\n' > "$T/marco_pose_meta.h"; printf 'C\n' > "$T/marco_campaign.h"
touch -d 2000-01-01 "$T/marco_pose_meta.h" "$T/marco_campaign.h"
sh tools/marco_inputs_sum.sh "$T"
[ -f "$T/marco-inputs.sum" ] || fail "no sum written"
touch -d 2001-01-01 "$T/marco-inputs.sum"; mkdir -p "$T/src"; touch -d 2002-01-01 "$T/src/marco_renderer.o"
sh tools/marco_inputs_sum.sh "$T"
[ "$(mt "$T/marco-inputs.sum")" = "$(mt "$T/src/marco_renderer.o")" ] && fail "sum time moved"
[ "$(mt "$T/marco-inputs.sum")" -lt "$(mt "$T/src/marco_renderer.o")" ] || fail "unchanged inputs rewrote the sum"
# A different catalog with the old time (cp -p): the sum is newer than the object.
printf 'B\n' > "$T/marco_pose_meta.h"; touch -d 2000-01-01 "$T/marco_pose_meta.h"
sh tools/marco_inputs_sum.sh "$T"
[ "$(mt "$T/marco-inputs.sum")" -gt "$(mt "$T/src/marco_renderer.o")" ] || fail "changed catalog with an old time did not refresh the sum"
# The Makefile makes the objects and the romfs stamps depend on the sum.
db=$(make -pq BUILD_DIR="$T" "$T/marco-inputs.sum" 2>/dev/null || true)
for t in src/marco_renderer.o src/marco_visual.o game-romfs.stamp marco-render-romfs/.stamp; do
    printf '%s\n' "$db" | grep -q "^$T/$t:.*$T/marco-inputs.sum" || fail "$t does not depend on marco-inputs.sum"
done
echo "PASS: Marco private inputs rebuild by content (old-time copies included)"
