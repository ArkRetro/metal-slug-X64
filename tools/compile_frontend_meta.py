#!/usr/bin/env python3
"""Compile frontend frame counts/hashes; no ROM graphics are embedded in C."""
import json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
source=ROOT/'filesystem/cinematics/frontend/frontend.json'
f=json.loads(source.read_text());assert f['status']=='ALL_FRAMES_MATCH'
lines=['/* Original frontend frame boundaries and RGB5551 CRCs. */', '#ifndef MSX_FRONTEND_META_H', '#define MSX_FRONTEND_META_H']
for segment in f['segments']:
    name=segment['name'].replace('-','_').upper()
    lines.extend([f'#define MSX_FRONTEND_{name}_FRAMES {segment["frames"]}u', f'#define MSX_FRONTEND_{name}_FIRST_CRC {segment["frame_crc32"][0]}UL', f'#define MSX_FRONTEND_{name}_LAST_CRC {segment["frame_crc32"][-1]}UL'])
lines.append('#endif')
(ROOT/'build').mkdir(exist_ok=True)
(ROOT/'build/frontend_meta.h').write_text('\n'.join(lines)+'\n')

fixture=['/* Original decoded RGB5551 CRCs for native cinematic verification. */']
for segment in f['segments']:
    name=segment['name'].replace('-','_')
    fixture.append('static const unsigned long frontend_'+name+'_crc[]={'+','.join(str(c)+'UL' for c in segment['frame_crc32'])+'};')
(ROOT/'build/frontend_fixture.h').write_text('\n'.join(fixture)+'\n')
