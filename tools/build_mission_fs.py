#!/usr/bin/env python3
"""Stage the internal mission build without the 48 MiB graphics laboratory ROM."""
import shutil,hashlib,json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];out=ROOT/'build/mission1-romfs';out.mkdir(parents=True,exist_ok=True)
title=json.loads((ROOT/'filesystem/cinematics/title-motion.json').read_text())
assert title.get('announcement',{}).get('asset_sha256')==hashlib.sha256((ROOT/'art/audio/sixty-four.wav').read_bytes()).hexdigest(), 'Rebuild title audio: python3 tools/title_announcement.py CAPTURE'
assert title['audio_sha256']==hashlib.sha256((ROOT/'filesystem/cinematics/title-motion.wav64').read_bytes()).hexdigest(), 'Title WAV64 does not match verified metadata'
(out/'title-reference.wav64').unlink(missing_ok=True)
(out/'title-announcement.wav64').unlink(missing_ok=True)
for name,source in [('title.msxc',ROOT/'filesystem/cinematics/title-motion.msxc'),('scene.pack',ROOT/'romfs/scene.pack'),('entry.msxc',ROOT/'filesystem/cinematics/entry.msxc'),('entry.wav64',ROOT/'filesystem/cinematics/entry.wav64'),('title-motion.wav64',ROOT/'filesystem/cinematics/title-motion.wav64')]:shutil.copyfile(source,out/name)
shutil.copytree(ROOT/'romfs/vehicles',out/'vehicles',dirs_exist_ok=True)
print('Internal mission filesystem:',sum(p.stat().st_size for p in out.rglob('*') if p.is_file()),'bytes')
