#!/usr/bin/env python3
"""Every effect of msx_sfx_defs (src/msx_sfx.c and src/msx_sfx_policy.h) has
its WAV64 in the cartridge's audio directory: game_audio.c opens each one at
start (streamed ones with wav64_open, which asserts), so a missing render
(16-XI: sfx_1089 of B75 compiled into the policy without its file) stopped
the game at boot. Run by the game-romfs rule when the audio is present.

  python3 tools/check_sfx_assets.py build/game-romfs/audio
"""
import re,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
names=set()
for f in ('src/msx_sfx.c','src/msx_sfx_policy.h'):
    names|=set(re.findall(r'\{0x[0-9a-fA-F]+,MSX_SFX_CH_\w+,\d+,"([^"]+)"\}',(ROOT/f).read_text()))
d=Path(sys.argv[1])
missing=sorted(n for n in names if not (d/(n+'.wav64')).exists())
if missing:
    sys.exit(f'check_sfx_assets: {len(missing)} of {len(names)} effects without a WAV64 in {d}: '+', '.join(missing))
print(f'check_sfx_assets: {len(names)} effects, every WAV64 present in {d}')
