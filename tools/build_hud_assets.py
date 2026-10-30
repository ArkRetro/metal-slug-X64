#!/usr/bin/env python3
"""Fix-layer HUD palette (docs/ARMAS_VEHICULOS.md §11) for the game cartridge.

The fix palettes 0..15 (palette RAM 400000..4001FF) are the game's runtime
state; they are taken from MAME palette dumps of the weapons captures (every
dump given must hold the same 256 words) and converted with the repository's
Neo Geo colour model (export_marco.palette_rgb) to RGBA5551, pen 0 clear.
Writes filesystem/hud-palette.bin (512 bytes, big endian; ROM-derived, not in
Git). The S-ROM tiles are filesystem/fixed.bin (tools/import_rom.py).
Usage: build_hud_assets.py PALETTE_DUMP...
"""
import struct,sys
from pathlib import Path
from export_marco import palette_rgb
ROOT=Path(__file__).resolve().parents[1]
words=None
for p in sys.argv[1:]:
    w=struct.unpack('>256H',Path(p).read_bytes()[:512])
    if words is not None and w!=words:sys.exit(f'{p}: fix palettes differ from {sys.argv[1]}')
    words=w
if words is None:sys.exit(__doc__)
out=[]
for i,w in enumerate(words):
    if i%16==0:out.append(0);continue
    r,g,b=palette_rgb(w);out.append((r>>3)<<11|(g>>3)<<6|(b>>3)<<1|1)
(ROOT/'filesystem/hud-palette.bin').write_bytes(struct.pack('>256H',*out))
print(len(sys.argv)-1,'dumps agree ->',ROOT/'filesystem/hud-palette.bin')
