#!/usr/bin/env python3
"""Stage verified live player resources, never gameplay fixture timelines."""
from pathlib import Path
import hashlib
import shutil
ROOT=Path(__file__).resolve().parents[1]
out=ROOT/'build/mission1-romfs'
for name in ('marco-campaign','campaign-footprints','effects'):
    source=ROOT/'romfs'/name
    shutil.copytree(source,out/name,dirs_exist_ok=True)
digest=hashlib.sha256()
for name in ('marco-campaign','campaign-footprints','effects'):
    for path in sorted((out/name).glob('*')):
        if path.is_file():digest.update(path.name.encode());digest.update(path.read_bytes())
(out/'live-player.stamp').write_text(digest.hexdigest()+'\n')
