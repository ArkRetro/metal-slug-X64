#!/usr/bin/env python3
"""Render the gameplay sound commands of data/sound_map.json in isolation.

Runs tools/mame/render_sound_commands.lua (MAME 0.264): Mission 1 is reached
with inputs only, the 68000 is suspended and each command is written to the
sound latch after the driver reset/stop pair 03,06 and a silent gap. The
-wavwrite output (48 kHz stereo) is cut at the recorded emulated times:
onset = first sample above the threshold after the command, end = last
sample above it. Writes filesystem/audio-src/<label>.wav and render.json
(ROM-derived, gitignored). tools/pack_game_audio.py converts them to WAV64."""
import argparse,json,os,subprocess,wave
from pathlib import Path
import numpy as np
ROOT=Path(__file__).resolve().parents[1]
RATE=48000
def seconds(text):
    # MAME attotime: seconds.milliseconds,microseconds,nanoseconds
    whole,_,frac=text.partition('.');return int(whole)+int(frac.replace(',',''))/1e9 if frac else float(whole)
def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--rom-dir',type=Path,required=True);p.add_argument('--work',type=Path,required=True)
    p.add_argument('--map',type=Path,default=ROOT/'data/sound_map.json')
    p.add_argument('--output',type=Path,default=ROOT/'filesystem/audio-src')
    p.add_argument('--labels',nargs='*',help='subset of labels');p.add_argument('--sfx-frames',type=int,default=300)
    p.add_argument('--gap-frames',type=int,default=120,help='silent frames after the reset/stop pair 03,06')
    p.add_argument('--threshold',type=int,default=48);p.add_argument('--mame',default='/usr/games/mame')
    a=p.parse_args()
    sounds=[s for s in json.loads(a.map.read_text())['sounds'] if not a.labels or s['label'] in a.labels]
    plan=[]
    for s in sounds:
        word=int(s['word'],16);data=[int(b,16) for b in s.get('pre',[])]+[word>>8,word&255]
        plan.append('%s=%s:%d'%(s['label'],','.join('%02X'%b for b in data),s.get('frames',a.sfx_frames)))
    work=a.work.resolve();assert not work.exists() or not any(work.iterdir()),'Use an empty work directory'
    for n in ('cfg','nvram'):(work/n).mkdir(parents=True)
    env=dict(os.environ,MSX_CAPTURE_DIR=str(work),MSX_RENDER_PLAN=';'.join(plan),MSX_GAP_FRAMES=str(a.gap_frames))
    cmd=[a.mame,'mslugx','-rompath',str(a.rom_dir.resolve()),'-bios','euro','-video','none','-sound','none','-samplerate',str(RATE),
         '-wavwrite',str(work/'render.wav'),'-skip_gameinfo','-nothrottle','-debug','-debugger','none','-debuglog',
         '-debugscript',str(ROOT/'tools/mame/start.cmd'),'-autoboot_delay','0','-autoboot_script',str(ROOT/'tools/mame/render_sound_commands.lua'),
         '-cfg_directory',str(work/'cfg'),'-nvram_directory',str(work/'nvram')]
    (work/'run-command.json').write_text(json.dumps(dict(cmd=cmd,plan=plan),indent=1)+'\n')
    with (work/'mame.log').open('w') as log:r=subprocess.run(cmd,cwd=work,env=env,stdout=log,stderr=subprocess.STDOUT,timeout=3600)
    assert r.returncode==0 and '[LUA ERROR]' not in (work/'mame.log').read_text(),'MAME render failed'
    with wave.open(str(work/'render.wav')) as w:
        assert w.getframerate()==RATE and w.getnchannels()==2 and w.getsampwidth()==2
        pcm=np.frombuffer(w.readframes(w.getnframes()),dtype='<i2').reshape(-1,2)
    events=[json.loads(l) for l in (work/'render.jsonl').open() if l.strip()]
    a.output.mkdir(parents=True,exist_ok=True);report={}
    for s in sounds:
        mine=[e for e in events if e['label']==s['label']]
        start=next(e for e in mine if e['kind']=='start');end=next(e for e in mine if e['kind']=='end')
        writes=[e for e in mine if e['kind']=='write' and e['frame']>=start['frame']]
        gap=pcm[int(seconds(mine[1]['time'])*RATE)+RATE//2:int(seconds(start['time'])*RATE)]
        assert not np.abs(gap).max()>a.threshold,'%s: driver not silent before the command'%s['label']
        first=int(seconds(start['time'])*RATE);last=int(seconds(end['time'])*RATE)
        seg=pcm[first:last];loud=np.nonzero(np.abs(seg).max(axis=1)>a.threshold)[0]
        entry=dict(word=s['word'],bytes=[e['value'] for e in writes],command_time=seconds(writes[-1]['time']))
        if not len(loud):entry.update(status='SILENT');report[s['label']]=entry;continue
        on,off=int(loud[0]),int(loud[-1])+1+RATE//100
        clip=seg[on:min(off,len(seg))]
        tail_active=bool(np.abs(seg[-RATE//10:]).max()>a.threshold)
        stereo=bool(np.abs(clip[:,0].astype(int)-clip[:,1]).max()>2)
        with wave.open(str(a.output/(s['label']+'.wav')),'wb') as w:
            w.setnchannels(2);w.setsampwidth(2);w.setframerate(RATE);w.writeframes(clip.astype('<i2').tobytes())
        spec=np.abs(np.fft.rfft(clip.mean(axis=1)));freq=np.fft.rfftfreq(len(clip),1/RATE)
        entry.update(status='ACTIVE_AT_WINDOW_END' if tail_active else 'OK',latency_ms=round((first+on)/RATE*1000-entry['command_time']*1000,1),
            samples=len(clip),seconds=round(len(clip)/RATE,3),peak=int(np.abs(clip).max()),stereo=stereo,
            energy_above_11khz=round(float((spec[freq>11025]**2).sum()/(spec**2).sum()),5),
            energy_above_8khz=round(float((spec[freq>8000]**2).sum()/(spec**2).sum()),5),
            onset_sample=first+on)
        report[s['label']]=entry
    (a.output/'render.json').write_text(json.dumps(dict(source='MAME 0.264 isolated latch render',plan=plan,sounds=report),indent=1)+'\n')
    for k,v in report.items():print(k,v)
if __name__=='__main__':main()
