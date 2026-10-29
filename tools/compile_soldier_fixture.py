#!/usr/bin/env python3
"""Compile recorded debugger values into a portable host-test fixture."""
import json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
f=json.loads((ROOT/'data/first_soldier_init_fixture.json').read_text())
lines=['/* Recorded inputs/outputs; see data/first_soldier_init_fixture.json. */']
for key in ('rng0_low16','rng1_low16','flags15','field92','expected_signed_62','expected_word_26'):
    value=f[key];lines.append('#define SOLDIER_'+key.upper()+' '+str(value))
(ROOT/'build').mkdir(exist_ok=True);(ROOT/'build/soldier_fixture.h').write_text('\n'.join(lines)+'\n')
