#!/usr/bin/env python3
"""Compare recovered C with controlled original-MAME fixture outputs, offline."""
import argparse
import ctypes as ct
import json
import subprocess
import tempfile
from pathlib import Path
from condition_harness import ROOT, Bus, Registers, run_c

p=argparse.ArgumentParser(description=__doc__)
p.add_argument('--sanitize',action='store_true')
a=p.parse_args()
fixture=json.loads((ROOT/'decomp/recovered/rom_01a5ca.validation.json').read_text())
assert fixture['status']=='PASS_CONTROLLED_M68000_HANDLERS_WITH_INTERCEPTED_HELPERS'
with tempfile.TemporaryDirectory(prefix='msx-conditions-') as directory:
    path=Path(directory)/'conditions.so'
    cmd=['cc','-std=c11','-Wall','-Wextra','-Werror','-shared','-fPIC']
    if a.sanitize:cmd+=['-fsanitize=undefined','-fno-sanitize-recover=all']
    subprocess.run(cmd+[str(ROOT/'decomp/recovered/rom_01a5ca.c'),'-o',str(path)],check=True)
    lib=ct.CDLL(str(path));lib.msx_rom_01a5ca.argtypes=[ct.POINTER(Registers),ct.POINTER(Bus)]
    for row in fixture['results']:
        actual,_=run_c(lib,row['input'])
        assert actual==row['expected'],(row['input']['index'],actual,row['expected'])
    # Contract-only checks: unsupported destinations and unavailable services
    # must be distinguishable from a successful condition returning carry=0.
    case=dict(fixture['results'][0]['input']);case['opcode']=255
    actual,_=run_c(lib,case)
    assert actual['status']==1 and not actual['calls']
    case=next(row['input'] for row in fixture['results'] if row['input']['opcode']==11)
    actual,_=run_c(lib,case,helper_available=False)
    assert actual['status']==2
print(f"PASS: {fixture['cases']} original-M68000 condition fixtures and explicit failure contracts")
