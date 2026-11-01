#!/usr/bin/env python3
"""neo_kernel_train_cell (src/neo_kernel.c) against the ROM grid of map
0B8566 (tools/fill_collision_rom.py rom_grid). Skips without the ROM."""
import sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
rom=Path(sys.argv[1] if len(sys.argv)>1 else 'mslugx.zip')
if not rom.exists():print('SKIP: train grid (no %s)'%rom);sys.exit(0)
from decode_level_script import Program
from fill_collision_rom import rom_grid
g,gc,gr=rom_grid(Program(rom),0xb8566)
def native(c,r):
    if 384<=c<448:return 0xa0 if r>=46 else 0
    return 0x2b if r==36 else 0
bad=[(c,r) for c in range(gc) for r in range(gr) if g[c*gr+r]!=native(c,r)]
assert (gc,gr)==(784,48) and not bad,(gc,gr,bad[:5])
print('PASS: train grid 0B8566 (%d x %d cells equal to the ROM)'%(gc,gr))
