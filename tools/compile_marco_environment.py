#!/usr/bin/env python3
"""Compile numerical output of the controlled original-M68000 probe."""
import json
from pathlib import Path
root=Path(__file__).resolve().parents[1]
f=json.loads((root/'data/marco_environment_fixture.json').read_text());assert f['status']=='CONTROLLED_M68000_PROBE'
rows=f['cases'];assert len(rows)==256
out=root/'build/marco_environment_fixture.h';out.parent.mkdir(exist_ok=True)
fields=('input80','input_vy','output80','output_vy')
out.write_text('/* Controlled original CPU outputs, not invented C reference values. */\nstatic const uint32_t env_cases[][4]={\n'+''.join('{'+','.join(str(r[k])+'u' for k in fields)+'},\n' for r in rows)+'};\n')
