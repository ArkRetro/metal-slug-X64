#!/bin/sh
# Mission 6 from the TAS (docs/TAS.md, docs/MISION6.md): tools/tas_updates.lua replays the movie by game
# update on MAME 0.264 from the section state of /home/user/captures-tas/sync264 with NO memory write at
# all (only the input fields), and its read-only taps write camera.csv, cells.txt and events.csv in the
# formats of tools/capture_fullgame.lua. Usage: tools/capture_m6_tas.sh STAGE(9|10|11) OUT [STATE:U]
# The section states (default: the camera fixtures) are saved when the script pointer changes, after the
# stage load cleared the cell ring; for the map reads the capture starts from the state before it
# (STATE:U = u18060:18060, u20160:20160, u23460:23460), so the ring's whole history is in events.csv.
S=$1;OUT=$2
case $S in
 9) U=18679;LAST=20251;;
 10) U=20251;LAST=23570;;
 11) U=23570;LAST=26480;;
 *) echo "stage 9, 10 or 11";exit 2;;
esac
export MSX_TAS_SCRIPT=tas_updates.lua MSX_TAS_UPD_PLAY=${MSX_TAS_UPD_PLAY:-/home/user/captures-tas/updates251.txt}
LOAD=${3:-sec$(printf %02d $S)_u$U:$U};U=${LOAD#*:}
export MSX_TAS_STATES=${MSX_TAS_STATES:-/home/user/captures-tas/sync264/states} MSX_TAS_UPD_LOAD=$LOAD
export MSX_TAS_CAPTURE_FROM=$U MSX_TAS_UPD_LAST=$LAST MSX_TAS_CAMERA=1 MSX_TAS_CELLS=1 MSX_TAS_LAST=60000
exec sh "$(dirname "$0")/run_tas_play.sh" "$OUT"
