#!/usr/bin/env python3
"""Check the isolated renders against real gameplay captures.

For every latch write of a rendered command in a capture (debug.log LATCH
lines, the driver bank byte < 0x20 tracked as the last one written), the
render is cross-correlated with the gameplay PCM (left channel) in a window
of -20..+80 ms around the end of that video frame (frames.jsonl). The score
is the normalised correlation of the best lag; the baseline is the same
measure at 200 random positions of the same capture. Gameplay audio has the
music and other effects mixed in, so the score is expected well below 1 but
clearly above the baseline when the render is the sound that played."""
import argparse,json,random,re,wave
from pathlib import Path
import numpy as np
R=48000
def sec(t):
    w,_,f=t.partition('.');return int(w)+int(f.replace(',',''))/1e9 if f else float(w)
def score(pcm,clip,at):
    n=min(len(clip),R//4);c=clip[:n]-clip[:n].mean();best=0.0
    lo=max(0,at-R//50);hi=min(len(pcm)-n,at+R*8//100)
    if hi<=lo:return None
    seg=pcm[lo:hi+n]
    for k in range(0,hi-lo,8):
        x=seg[k:k+n];x=x-x.mean();d=np.sqrt((x*x).sum()*(c*c).sum())
        if d>0:best=max(best,float((x*c).sum()/d))
    return best
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('captures',nargs='+',type=Path)
    p.add_argument('--renders',type=Path,default=Path(__file__).resolve().parents[1]/'filesystem/audio-src')
    p.add_argument('--output',type=Path);a=p.parse_args()
    info=json.loads((a.renders/'render.json').read_text())['sounds']
    clips={}
    for label,e in info.items():
        if e.get('status')!='OK' or label.startswith('music'):continue
        with wave.open(str(a.renders/(label+'.wav'))) as w:clips[int(e['word'],16)]=(label,np.frombuffer(w.readframes(w.getnframes()),dtype='<i2').reshape(-1,2)[:,0].astype(float))
    result={};rng=random.Random(1)
    for cap in a.captures:
        with wave.open(str(cap/'reference.wav')) as w:pcm=np.frombuffer(w.readframes(w.getnframes()),dtype='<i2').reshape(-1,2)[:,0].astype(float)
        times={json.loads(l)['frame']:sec(json.loads(l)['time']) for l in (cap/'frames.jsonl').open() if l.strip()}
        bank=0;hits=[]
        for line in (cap/'debug.log').open(errors='replace'):
            m=re.match(r'LATCH frame=(\d+) pc=\w+ value=(\w+)',line)
            if not m:continue
            f,v=int(m[1]),int(m[2],16)
            if v<0x20:bank=v;continue
            word=bank<<8|v
            if word in clips and f in times:hits.append((word,int(times[f]*R)))
        for word,at in hits:
            s=score(pcm,clips[word][1],at)
            if s is not None:result.setdefault(clips[word][0],[]).append(s)
        base=[score(pcm,clips[w][1],rng.randrange(R*70,len(pcm)-R)) for w in list(clips)[:1] for _ in range(200)]
        result.setdefault('_baseline_'+clips[list(clips)[0]][0],[]).extend(x for x in base if x is not None)
    summary={k:dict(n=len(v),median=round(float(np.median(v)),3),max=round(float(max(v)),3)) for k,v in sorted(result.items())}
    for k,v in summary.items():print(k,v)
    if a.output:a.output.write_text(json.dumps(summary,indent=1)+'\n')
if __name__=='__main__':main()
