#!/usr/bin/env python3
"""Compile observed walk coordinates, not an inferred arcade collision ROM."""
import csv
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
rows=list(csv.DictReader((ROOT/'data/mission1-ground.csv').open()))
lines=['/* Observed ground samples; interpolated between captures. */',
       'static const struct { int32_t x,y; } mission_ground[]={']
lines.extend('  {'+r['x16']+','+r['y16']+'},' for r in rows)
lines.append('};')
(ROOT/'build').mkdir(exist_ok=True)
(ROOT/'build/mission_ground.h').write_text('\n'.join(lines)+'\n')
