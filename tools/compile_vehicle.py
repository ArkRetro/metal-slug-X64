#!/usr/bin/env python3
"""Compile decoded data into native C tables; no 68000 instructions are emitted."""
import json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
vehicle=json.loads((ROOT/'filesystem/vehicle.json').read_text())
lines=['/* Generated native animation data, no original bytecode. */']
for name,animation in vehicle['animations'].items():
    lines.append(f'static const neo_vehicle_anim_frame_t vehicle_{name}[]={{')
    lines.extend('  {'+str(f['pose'])+','+str(f['ticks'])+'},' for f in animation['frames'])
    lines.append('};')
(ROOT/'build').mkdir(exist_ok=True)
(ROOT/'build/vehicle_anim.h').write_text('\n'.join(lines)+'\n')
wreck=json.loads((ROOT/'data/vehicle_wreckage.json').read_text())
assert (wreck['width'],wreck['height'])==(13,3)
lines=['/* Generated world-cell footprint. */','static const uint8_t vehicle_wreckage[3][13]={']
lines.extend('  {'+','.join(map(str,row))+'},' for row in wreck['rows_high_to_low']);lines.append('};')
(ROOT/'build/vehicle_wreckage.h').write_text('\n'.join(lines)+'\n')
