#!/usr/bin/env python3
"""Convert a bounded original title soundtrack reference for internal native tests.

This is a mixed cinematic recording, not isolated effects or a gameplay driver.
Never infer sound-command semantics or trim at frame/60.
"""
import argparse,hashlib,json,subprocess,wave
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('--start-frame',type=int,default=360);p.add_argument('--end-frame',type=int,default=660);a=p.parse_args()
def nanos(s):
 # MAME attotime prints seconds.milliseconds,microseconds,nanoseconds.
 return int(s.replace('.','').replace(',',''))
rows={r['frame']:r for l in (a.capture/'frames.jsonl').read_text().splitlines() if (r:=json.loads(l))}
start,end=(nanos(rows[f]['time']) for f in (a.start_frame,a.end_frame));assert 0<=start<end
with wave.open(str(a.capture/'reference.wav')) as source:
 assert source.getframerate()==48000 and source.getnchannels()==2 and source.getsampwidth()==2
 first=(start*48000)//10**9;last=(end*48000)//10**9;assert last<=source.getnframes()
 source.setpos(first);pcm=source.readframes(last-first);assert any(pcm)
 out=ROOT/'filesystem/audio-reference';out.mkdir(parents=True,exist_ok=True)
 with wave.open(str(out/'title-reference.wav'),'wb') as wav:wav.setnchannels(2);wav.setsampwidth(2);wav.setframerate(48000);wav.writeframes(pcm)
target=ROOT/'build/audio-romfs';target.mkdir(parents=True,exist_ok=True)
cmd=['/workspace/toolchains/package/opt/libdragon/bin/audioconv64','--wav-resample','32000','--wav-compress','1','--wav-loop','false','-o',str(target),str(out/'title-reference.wav')];subprocess.run(cmd,check=True)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
report=dict(source='MAME 0.276 stereo reference; title animation frames, no gameplay effects isolation',first_frame=a.start_frame,end_frame=a.end_frame,first_sample=first,end_sample=last,original_rate=48000,wav_sha256=sha(a.capture/'reference.wav'),converted_sha256=sha(target/'title-reference.wav64'),conversion=cmd,limits='Offline mixed title reference only. Not the complete intro, music loop, effects bank or game sound integration.')
(out/'conversion.json').write_text(json.dumps(report,indent=2)+'\n');print(report)
