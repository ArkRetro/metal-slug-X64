#!/usr/bin/env python3
"""Compile recovered collision geometry without including the original program."""
import json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
# The rest-of-mission grid (480 columns) starts with the validated region
# grid (prefix_columns_verified); use it when present.
rest=ROOT/'filesystem/mission1/rest/collision.json'
m=json.loads((rest if rest.exists() else ROOT/'filesystem/mission1/collision.json').read_text())
data=bytes.fromhex(m['cells_hex']);assert len(data)==m['columns']*32
lines=['/* Generated from original observed 8x8 cells. */',
       f'#define NEO_GRID_COLUMNS {m["columns"]}',
       'static const uint8_t neo_grid[]={']
for start in range(0,len(data),32):lines.append('  '+','.join(hex(v) for v in data[start:start+32])+',')
lines.append('};')
(ROOT/'build').mkdir(exist_ok=True)
(ROOT/'build/mission_collision.h').write_text('\n'.join(lines)+'\n')
