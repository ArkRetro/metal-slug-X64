#!/usr/bin/env python3
"""Exercise bank switches, masked writes and independent snapshot checks."""
import json,struct,tempfile
from pathlib import Path
from render_intro_reference import bank_palette_states
with tempfile.TemporaryDirectory() as tmp:
 p=Path(tmp);before=[0]*8192;before[0]=0x1101;before[4096]=9
 after=before.copy();after[0]=0x1122;after[4096]=10
 def save(frame,values):(p/f'palette-banks{frame}.bin').write_bytes(struct.pack('>8192H',*values))
 save(0,before);save(1,after)
 (p/'frames.jsonl').write_text('\n'.join(json.dumps(dict(frame=f,palette_bank=0,screen_shadow=0)) for f in (0,1)))
 writes=[dict(frame=0,line=250,palette_bank_write=True,address=0x3a001e,bank_before=0),dict(frame=0,line=251,palette_index=0,palette_bank=4096,word=9,data=10,mask=65535),dict(frame=0,line=30,palette_bank_write=True,address=0x3a000e,bank_before=4096),dict(frame=0,line=31,palette_index=0,palette_bank=0,word=0x1101,data=0x22,mask=255)]
 (p/'raster.jsonl').write_text('\n'.join(map(json.dumps,writes)))
 states,count=bank_palette_states(p,1)
 assert count==4 and states[16][0]==10 and states[29][0]==10
 assert states[30][0]==0x1101 and states[31][0]==0x1122
 after[4097]=1;save(1,after)
 try:bank_palette_states(p,1)
 except AssertionError as e:assert 'Missing physical-bank' in str(e)
 else:raise AssertionError('Unobserved palette mutation was accepted')
print('PASS: physical palette banks, scanline switches, masked writes and missing-write rejection')
