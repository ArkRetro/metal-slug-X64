#!/usr/bin/env python3
"""Compile only measured handoff/CRCs; does not need libdragon or ROM pixels."""
import json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
f=json.loads((ROOT/'filesystem/cinematics/entry.json').read_text())
lines=['/* Measured entry state and reference CRCs. */','#define MSX_ENTRY_X '+str(f['handoff']['x'])+'L','#define MSX_ENTRY_Y '+str(f['handoff']['y'])+'L','#define MSX_ENTRY_FRAMES '+str(f['frames']),'#define MSX_ENTRY_VX '+str(f['handoff']['vx'])+'L','#define MSX_ENTRY_VY '+str(f['handoff']['vy']-(1<<32 if f['handoff']['vy']>>31 else 0))+'L','#define MSX_ENTRY_DESCENT '+('1' if f['handoff']['routine']==0xc162a else '0'),'static const unsigned long msx_entry_crc[]={'+','.join(str(c)+'UL' for c in f['frame_crc32'])+'};']
(ROOT/'build').mkdir(exist_ok=True);(ROOT/'build/entry_meta.h').write_text('\n'.join(lines)+'\n')
