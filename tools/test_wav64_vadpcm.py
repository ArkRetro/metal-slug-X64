#!/usr/bin/env python3
"""B43: every VADPCM pack of the game audio decodes to its source.

1. The host decoder (tools/wav64_vadpcm.py) on a synthetic signal encoded by
   audioconv64 (no private data): the check passes and the decode is close.
2. When the private packs are present: every mono VADPCM effect in
   filesystem/audio with its staging source filesystem/audio/wav/<label>.wav
   passes the check (the truck engine 10A5 failed it as VADPCM and is now
   stored as PCM16: format 0), and the select music loop
   (filesystem/cinematics/select-music.json, B44) recorded a passing check."""
import json,os,subprocess,sys,tempfile,wave
from pathlib import Path
import numpy as np
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'tools'))
import wav64_vadpcm as W
conv=os.path.join(os.environ.get('N64_INST','/workspace/toolchains/package/opt/libdragon'),'bin/audioconv64')
with tempfile.TemporaryDirectory() as tmp:
    t=np.arange(24000)/24000.0;rng=np.random.default_rng(1)
    x=np.round(6000*np.sin(2*np.pi*440*t)*np.exp(-t)+800*rng.standard_normal(len(t))).astype('<i2')
    src=Path(tmp)/'synth.wav'
    with wave.open(str(src),'wb') as w:w.setnchannels(1);w.setsampwidth(2);w.setframerate(24000);w.writeframes(x.tobytes())
    subprocess.run([conv,'--wav-mono','--wav-compress','1','--wav-loop','false','-o',tmp,str(src)],check=True,stdout=subprocess.DEVNULL)
    pcm,rate=W.decode(Path(tmp)/'synth.wav64');r=W.check(pcm[:,0],x,rate)
    assert rate==24000 and len(pcm)==len(x) and r['ok'],r
    print('synthetic',r)
audio=ROOT/'filesystem/audio';checked=0
for f in sorted(audio.glob('*.wav64')) if audio.exists() else []:
    if f.stem.startswith('music'):continue
    head=f.read_bytes()[:8]
    if f.stem=='sfx_10a5':assert head[5]==0,'sfx_10a5 must be PCM16 (its VADPCM fails the check)'
    s=audio/'wav'/(f.stem+'.wav')
    if head[5]!=1 or not s.exists():continue
    with wave.open(str(s)) as w:x=np.frombuffer(w.readframes(w.getnframes()),dtype='<i2')
    pcm,rate=W.decode(f);r=W.check(pcm[:,0],x,rate);assert r['ok'],(f.name,r);checked+=1
sel=ROOT/'filesystem/cinematics/select-music.json'
if sel.exists():assert json.loads(sel.read_text())['vadpcm_check']['ok']
print('effects checked',checked,'select music',sel.exists())
