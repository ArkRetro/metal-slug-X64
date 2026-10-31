#!/usr/bin/env python3
"""Convert the isolated original renders to WAV64 for the N64 game.

Input: filesystem/audio-src/<label>.wav and render.json written by
tools/render_sound_commands.py (48 kHz stereo, ROM-derived, gitignored).
Output: filesystem/audio/<label>.wav64 and filesystem/audio/manifest.json.

* Effects: every render is mono (render.json 'stereo': false, L == R), so
  they are stored as mono VADPCM at 24 kHz. The runtime plays each sound on
  one channel only (src/msx_sfx.c), so its decoder state is never shared. Their content is the YM2610
  ADPCM-A output (18.5 kHz sample rate): <= 3.7 % of the energy lies above
  8 kHz in every render.
* Music: the loop is measured, not assumed. A 10 ms log-spectrogram
  self-similarity gives the period (best median similarity over lags) and
  the start of the repeated part (first time the similarity at that lag
  stays high). The driver output is not sample-identical between passes
  (FM phase, tick jitter), so the splice is placed where the waveform at t0
  and t0+period+d correlates best (d within +-25 ms); the WAV64 holds
  [0, t0+period+d) and loops back to t0. Stored as stereo VADPCM, 24 kHz
  (loop start/length rounded to 16 samples for VADPCM).
Resampling is an FFT band limit (numpy only)."""
import argparse,hashlib,json,subprocess,wave
from pathlib import Path
import numpy as np
ROOT=Path(__file__).resolve().parents[1]
SRC=48000
def read(path):
    with wave.open(str(path)) as w:
        assert w.getframerate()==SRC and w.getnchannels()==2 and w.getsampwidth()==2
        return np.frombuffer(w.readframes(w.getnframes()),dtype='<i2').reshape(-1,2).astype(np.float64)
def write(path,pcm,rate):
    pcm=np.clip(np.round(pcm),-32768,32767).astype('<i2')
    with wave.open(str(path),'wb') as w:
        w.setnchannels(1 if pcm.ndim==1 else pcm.shape[1]);w.setsampwidth(2);w.setframerate(rate);w.writeframes(pcm.tobytes())
def resample(x,rate,m=None):
    n=len(x);m=m or int(round(n*rate/SRC));X=np.fft.rfft(x,axis=0);k=m//2+1
    Y=np.zeros((k,)+x.shape[1:],dtype=complex);Y[:min(k,len(X))]=X[:min(k,len(X))]
    return np.fft.irfft(Y,m,axis=0)*m/n
def spectral_frames(mono,hop=480,size=4096,bands=64):
    win=np.hanning(size);idx=np.linspace(0,1024,bands+1).astype(int)
    rows=[]
    for i in range(0,len(mono)-size,hop):
        spec=np.abs(np.fft.rfft(mono[i:i+size]*win))[:1024]
        rows.append(np.add.reduceat(spec,idx[:-1]))
    F=np.log1p(np.array(rows));F-=F.mean(axis=1,keepdims=True)
    return F/(np.linalg.norm(F,axis=1,keepdims=True)+1e-9)
def lag_score(F,lag,lo=None,hi=None):
    s=(F[:len(F)-lag]*F[lag:]).sum(axis=1);return float(np.median(s[lo:hi])),s
def find_loop(pcm,min_period=30.0):
    """Coarse search at 100 ms, refinement at 10 ms around the best lags."""
    mono=pcm.mean(axis=1);hop=480;F=spectral_frames(mono,hop);C=F[::10];n=len(C)
    coarse=sorted(((lag_score(C,lag,-min(300,n-lag))[0],lag) for lag in range(int(min_period*10),n-50)),reverse=True)
    best=coarse[0]
    # Multiples of the period score alike: take the shortest lag that divides
    # the best one and scores within 0.15 of it.
    period=min(l for c,l in coarse[:40] if c>=best[0]-0.15 and any(abs(l*k-best[1])<=1 for k in range(1,6)))
    fine=max(range(period*10-15,period*10+16),key=lambda l:lag_score(F,l,1000,5000)[0])
    level,s=lag_score(F,fine,1000,5000);smooth=np.convolve(s,np.ones(50)/50,'same')
    start=next(t for t in range(len(smooth)) if all(smooth[t:t+300]>level-0.15))
    return dict(period_s=fine*hop/SRC,start_s=start*hop/SRC,similarity=round(level,3),
                candidates=[dict(lag_s=l/10,similarity=round(c,3)) for c,l in coarse[:6]])
def splice(pcm,start,period,good=0.9):
    """Splice candidates every 0.25 s of the repeated part; take the earliest
    whose seam correlation reaches `good` (or the best one): a shorter file."""
    mono=pcm.mean(axis=1);half=1024
    def corr(t0,d):
        a=mono[t0-half:t0+half];a=a-a.mean();b=mono[t0+period+d-half:t0+period+d+half];b=b-b.mean()
        return float((a*b).sum()/np.sqrt((a*a).sum()*(b*b).sum()+1e-9))
    found=[]
    for t0 in range(start+SRC,start+period-SRC,SRC//4):
        c,d=max((corr(t0,d),d) for d in range(-SRC//40,SRC//40+1,4))
        c,d=max((corr(t0,dd),dd) for dd in range(d-4,d+5))
        found.append((c,t0,d))
    top=max(c for c,_,_ in found)
    c,t0,d=min((f for f in found if f[0]>=min(good,top)),key=lambda f:f[1])
    return t0,t0+period+d,c
def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source',type=Path,default=ROOT/'filesystem/audio-src')
    p.add_argument('--output',type=Path,default=ROOT/'filesystem/audio')
    p.add_argument('--audioconv',default=None);p.add_argument('--sfx-rate',type=int,default=24000)
    p.add_argument('--music-rate',type=int,default=24000);a=p.parse_args()
    import os;conv=a.audioconv or os.path.join(os.environ.get('N64_INST','/workspace/toolchains/package/opt/libdragon'),'bin/audioconv64')
    render=json.loads((a.source/'render.json').read_text())['sounds']
    staging=a.output/'wav';staging.mkdir(parents=True,exist_ok=True);manifest={}
    old=json.loads((a.output/'manifest.json').read_text())['sounds'] if (a.output/'manifest.json').exists() else {}
    for label,info in render.items():
        if info['status']=='SILENT':manifest[label]=dict(word=info['word'],status='SILENT: not packed');continue
        # The music render (77 MB) need not stay on disk: keep its pack.
        if label.startswith('music') and not (a.source/(label+'.wav')).exists() and label in old:manifest[label]=old[label];continue
        # A sound still playing at the end of its render window is a loop
        # (engine 10A5, 1026), not a one-shot effect: not packed.
        if not label.startswith('music') and info['status']=='ACTIVE_AT_WINDOW_END':manifest[label]=dict(word=info['word'],status='LOOP: not packed');continue
        pcm=read(a.source/(label+'.wav'))
        if label.startswith('music'):
            loop=find_loop(pcm);period=int(round(loop['period_s']*SRC));start=int(round(loop['start_s']*SRC))
            t0,end,seam=splice(pcm,start,period)
            # VADPCM loops need a loop start and loop length multiple of 16
            # samples (libdragon wav64_vadpcm.c): round both down.
            offset=int(t0*a.music_rate/SRC)//16*16
            length=offset+int((end-t0)*a.music_rate/SRC)//16*16
            out=resample(pcm[:end],a.music_rate,length)
            # The pinned libdragon's VADPCM reader only wraps whole files, so
            # the music is split: <label>_intro (once, [0, offset)) and
            # <label> (the loop body [offset, length), looped whole). The game
            # starts the body on the same channel exactly when the intro ends
            # (mixer event, src/msx_audio.c).
            write(staging/(label+'_intro.wav'),out[:offset],a.music_rate)
            subprocess.run([conv,'--wav-resample',str(a.music_rate),'--wav-compress','1','--wav-loop','false','-o',str(a.output),str(staging/(label+'_intro.wav'))],check=True,stdout=subprocess.DEVNULL)
            write(staging/(label+'.wav'),out[offset:],a.music_rate)
            cmd=[conv,'--wav-resample',str(a.music_rate),'--wav-compress','1','--wav-loop','true','-o',str(a.output),str(staging/(label+'.wav'))]
            intro=a.output/(label+'_intro.wav64')
            entry=dict(loop,splice_source_sample=t0,end_source_sample=end,seam_correlation=round(seam,4),
                       loop_offset=offset,samples=len(out)-offset,intro_samples=offset,intro_bytes=intro.stat().st_size,
                       intro_sha256=hashlib.sha256(intro.read_bytes()).hexdigest(),rate=a.music_rate,channels=2,compression='vadpcm')
        else:
            assert not info['stereo'],label
            out=resample(pcm[:,0],a.sfx_rate)
            write(staging/(label+'.wav'),out,a.sfx_rate)
            cmd=[conv,'--wav-mono','--wav-compress','1','--wav-loop','false','-o',str(a.output),str(staging/(label+'.wav'))]
            entry=dict(samples=len(out),rate=a.sfx_rate,channels=1,compression='vadpcm')
        subprocess.run(cmd,check=True,stdout=subprocess.DEVNULL)
        wav64=a.output/(label+'.wav64')
        entry.update(word=info['word'],seconds=round(entry['samples']/entry['rate'],3),bytes=wav64.stat().st_size,
                     sha256=hashlib.sha256(wav64.read_bytes()).hexdigest(),conversion=cmd[:-3]+[cmd[-1].rsplit('/',1)[-1]])
        manifest[label]=entry
    total=sum(e.get('bytes',0)+e.get('intro_bytes',0) for e in manifest.values())
    (a.output/'manifest.json').write_text(json.dumps(dict(source='tools/pack_game_audio.py from tools/render_sound_commands.py renders',total_bytes=total,sounds=manifest),indent=1)+'\n')
    for k,v in manifest.items():print(k,{x:v[x] for x in v if x in ('status','seconds','bytes','period_s','start_s','seam_correlation','similarity','loop_offset')})
    print('total',total)
if __name__=='__main__':main()
