#!/usr/bin/env python3
"""Compile audited frame CRCs for the shared native C stream verifier."""
import argparse,json
from pathlib import Path
p=argparse.ArgumentParser();p.add_argument('metadata',type=Path);p.add_argument('output',type=Path);a=p.parse_args()
d=json.loads(a.metadata.read_text());n=d['frames'];crcs=d['frame_crc32'];assert 0<n<=6000 and len(crcs)==n and all(0<=c<2**32 for c in crcs)
a.output.parent.mkdir(parents=True,exist_ok=True)
a.output.write_text('/* Audited source-frame CRCs; legacy names used by shared verifier. */\n#define MSX_ENTRY_FRAMES '+str(n)+'\nstatic const unsigned long msx_entry_crc[]={'+','.join(str(c)+'UL' for c in crcs)+'};\n')
