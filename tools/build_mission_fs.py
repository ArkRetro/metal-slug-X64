#!/usr/bin/env python3
"""Stage the internal mission build without the 48 MiB graphics laboratory ROM."""
import shutil
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];out=ROOT/'build/mission1-romfs';out.mkdir(parents=True,exist_ok=True)
for name,source in [('scene.pack',ROOT/'romfs/scene.pack'),('entry.msxc',ROOT/'filesystem/cinematics/entry.msxc'),('entry.wav64',ROOT/'filesystem/cinematics/entry.wav64')]:shutil.copyfile(source,out/name)
shutil.copytree(ROOT/'romfs/vehicles',out/'vehicles',dirs_exist_ok=True)
print('Internal mission filesystem:',sum(p.stat().st_size for p in out.rglob('*') if p.is_file()),'bytes')
