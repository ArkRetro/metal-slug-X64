#!/bin/sh
# Mission 5 from the TAS (docs/TAS.md, docs/MISION5.md): tools/tas_updates.lua replays the movie by game
# update on MAME 0.264 from the section state of /home/user/captures-tas/sync264 with NO memory write at
# all (only the input fields), and its read-only taps write camera.csv, cells.txt and events.csv in the
# formats of tools/capture_fullgame.lua. Usage: tools/capture_m5_tas.sh STAGE(6|7|8) OUT [STATE:U]
# The section states (default: the camera fixtures) are saved when the script pointer changes, after the
# stage load cleared the cell ring; for the map reads the capture starts from the state before it
# (STATE:U = u12840:12840, u13740:13740, u16380:16380), so the ring's whole history is in events.csv.
S=$1;OUT=$2
case $S in
 6) U=12887;LAST=13810;;
 7) U=13810;LAST=16484;;
 8) U=16484;LAST=18160;;
 *) echo "stage 6, 7 or 8";exit 2;;
esac
export MSX_TAS_SCRIPT=tas_updates.lua MSX_TAS_UPD_PLAY=${MSX_TAS_UPD_PLAY:-/home/user/captures-tas/updates251.txt}
LOAD=${3:-sec0${S}_u$U:$U};U=${LOAD#*:}
export MSX_TAS_STATES=${MSX_TAS_STATES:-/home/user/captures-tas/sync264/states} MSX_TAS_UPD_LOAD=$LOAD
export MSX_TAS_CAPTURE_FROM=$U MSX_TAS_UPD_LAST=$LAST MSX_TAS_CAMERA=1 MSX_TAS_CELLS=1 MSX_TAS_LAST=60000
exec sh "$(dirname "$0")/run_tas_play.sh" "$OUT"
