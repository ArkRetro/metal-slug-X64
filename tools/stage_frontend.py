#!/usr/bin/env python3
"""Stage only runtime tutorial/selection streams; exclude PCM intermediates."""
from pathlib import Path
import hashlib
import json
import shutil

ROOT=Path(__file__).resolve().parents[1]
source=ROOT/'filesystem/cinematics/frontend'
target=ROOT/'build/mission1-romfs/frontend'
target.mkdir(parents=True,exist_ok=True)
manifest=json.loads((source/'frontend.json').read_text())
assert manifest['status']=='ALL_FRAMES_MATCH','Frontend source audit must pass'
segments={s['name']:s for s in manifest['segments']}
for segment in ('tutorial','select','select-confirm'):
    for suffix in ('.msxc','.wav64'):
        expected=segments[segment]['movie_sha256' if suffix=='.msxc' else 'audio_sha256']
        assert hashlib.sha256((source/(segment+suffix)).read_bytes()).hexdigest()==expected, f'Stale frontend asset: {segment}{suffix}'
        shutil.copyfile(source/(segment+suffix),target/(segment+suffix))
    (target/(segment+'.wav')).unlink(missing_ok=True)
# Copy the manifest last: make uses it as the staging completion marker.
shutil.copyfile(source/'frontend.json',target/'frontend.json')
