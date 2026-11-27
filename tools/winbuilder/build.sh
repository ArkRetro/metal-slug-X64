#!/bin/sh
# Builds the Windows generator of metalslugx64_rev1.0.z64 (developer machine).
#   tools/winbuilder/build.sh ROM.z64 mslugx.zip neogeo.zip OUT.exe
# payload.bin (encrypted cartridge, ROM-derived) and the .exe stay out of git.
set -e
cd "$(dirname "$0")"
[ $# -eq 4 ] || { echo "uso: $0 ROM.z64 mslugx.zip neogeo.zip OUT.exe" >&2; exit 2; }
go run . -pack "$1" payload.bin "$2" "$3"
GOOS=windows GOARCH=amd64 CGO_ENABLED=0 go build -trimpath -ldflags "-s -w" -o "$4" .
echo "listo: $4"
