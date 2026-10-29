#!/usr/bin/env python3
import json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];f=json.loads((ROOT/'data/soldier_history_fixture.json').read_text())
(ROOT/'build').mkdir(exist_ok=True);(ROOT/'build/history_fixture.h').write_text('static const unsigned char history_cases[][2]={'+','.join('{'+str(r['before'])+','+str(r['data'])+'}' for r in f['rows'])+'};\n')
