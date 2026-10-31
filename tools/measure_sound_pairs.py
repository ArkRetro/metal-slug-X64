#!/usr/bin/env python3
"""Measure the driver's voice policy for gameplay sound commands (pairs).

Each pair renders command A, then command B 12 video frames (0.2 s, the
player's fire rate) later, in isolation on MAME 0.264 (the same Lua
renderer as tools/render_sound_commands.py: 68000 suspended, driver reset
03/06 and a silent gap before each pair). The mix is fitted against the
isolated renders of A and B (filesystem/audio-src/<label>.wav, left
channel): 'overlap' (A + B), 'cut' (A stops where B starts), 'drop' (B
never sounds). The smallest RMS residual wins; its margin over the second
best is kept.

Pairs measured for every label of --labels: X after X (retrigger), and X
before/after a reference of each known channel (--ref, default pistol 10F7
weapon, explosion 1027 impact, rebel_1041 voice). Labels that overlap all
references get the extra pairs against the first label of each new group
found so far (greedy), so every label ends in exactly one group.
Writes the JSON report (--output). ROM-derived audio stays local; the report
holds numbers only."""
import argparse,json,os,subprocess,wave
from pathlib import Path
import numpy as np
ROOT=Path(__file__).resolve().parents[1]
RATE=48000
def seconds(text):
    whole,_,frac=text.partition('.');return int(whole)+int(frac.replace(',',''))/1e9 if frac else float(whole)
def bytes_of(word):return [word>>8,word&255]
def load(src,label):
    with wave.open(str(src/(label+'.wav'))) as w:
        return np.frombuffer(w.readframes(w.getnframes()),dtype='<i2').reshape(-1,2).astype(float)[:,0]
def run(plan,work,rom_dir,mame,gap):
    done=work/'run-plan.json'
    if done.exists() and json.loads(done.read_text())==dict(plan=plan,gap=gap) and (work/'render.wav').exists():
        return read_round(work)                        # same plan already rendered: reuse it
    work.mkdir(parents=True,exist_ok=True)
    for n in ('cfg','nvram'):(work/n).mkdir(exist_ok=True)
    env=dict(os.environ,MSX_CAPTURE_DIR=str(work),MSX_RENDER_PLAN=';'.join(plan),MSX_GAP_FRAMES=str(gap))
    cmd=[mame,'mslugx','-rompath',str(rom_dir.resolve()),'-bios','euro','-video','none','-sound','none','-samplerate',str(RATE),
         '-wavwrite',str(work/'render.wav'),'-skip_gameinfo','-nothrottle','-debug','-debugger','none','-debuglog',
         '-debugscript',str(ROOT/'tools/mame/start.cmd'),'-autoboot_delay','0','-autoboot_script',str(ROOT/'tools/mame/render_sound_commands.lua'),
         '-cfg_directory',str(work/'cfg'),'-nvram_directory',str(work/'nvram')]
    with (work/'mame.log').open('w') as log:r=subprocess.run(cmd,cwd=work,env=env,stdout=log,stderr=subprocess.STDOUT,timeout=7200)
    assert r.returncode==0 and '[LUA ERROR]' not in (work/'mame.log').read_text(),'MAME render failed'
    done.write_text(json.dumps(dict(plan=plan,gap=gap)))
    return read_round(work)
def read_round(work):
    with wave.open(str(work/'render.wav')) as w:
        pcm=np.frombuffer(w.readframes(w.getnframes()),dtype='<i2').reshape(-1,2).astype(float)[:,0]
    events=[json.loads(l) for l in (work/'render.jsonl').open() if l.strip()]
    return pcm,events
def envelope(x,hop=480):
    """Power per 10 ms window (FM/SSG parts of the driver are not
    sample-identical between renders, their power envelope is)."""
    n=len(x)//hop;return (x[:n*hop].reshape(n,hop)**2).mean(axis=1)
def classify_env(mix,writes,A,B,threshold=48,hop=480):
    """Fit of the mix power envelope: overlap P_A+P_B, cut P_A until B's
    onset then P_B, drop P_A; mean absolute log10 error over the windows
    from A's onset to the end of the longer model."""
    def onset(x,frm):
        i=np.nonzero(np.abs(x[frm:])>threshold)[0];return frm+int(i[0]) if len(i) else None
    o1=onset(mix,writes[0])
    if o1 is None:return dict(mode='silent')
    M=envelope(mix[o1:],hop);PA=envelope(A,hop);PB=envelope(B,hop)
    w2=(writes[-1]-o1)//hop;eps=48.0**2
    def model(mode,d):
        out=np.zeros(len(M));a=PA[:len(M)].copy()
        if mode=='cut':a[d:]=0
        out[:len(a)]+=a
        if mode!='drop':
            b=PB[:max(0,len(M)-d)];out[d:d+len(b)]+=b
        return out
    best={}
    for d in range(max(w2,0),max(w2,0)+4):
        for mode in ('overlap','cut','drop'):
            m=model(mode,d);end=len(M)
            err=float(np.mean(np.abs(np.log10(M[:end]+eps)-np.log10(m[:end]+eps))))
            if mode not in best or err<best[mode]:best[mode]=err
    order=sorted(best,key=lambda k:best[k])
    return dict(mode=order[0],error=round(best[order[0]],3),second=order[1],second_error=round(best[order[1]],3),
                margin=round(best[order[1]]-best[order[0]],3))
def classify(mix,writes,A,B,threshold=48):
    """writes: sample offsets (in mix) of the latch writes; A's last byte is
    writes[1] (bank, low), B's the last one."""
    def onset(x,frm):
        i=np.nonzero(np.abs(x[frm:])>threshold)[0];return frm+int(i[0]) if len(i) else None
    n=len(mix);o1=onset(mix,writes[0])
    if o1 is None:return dict(mode='silent')
    def put(s,off,cut=None):
        out=np.zeros(n);L=min(len(s),n-off) if off<n else 0
        if cut is not None:L=min(L,max(cut,0))
        if L>0:out[off:off+L]=s[:L]
        return out
    rms=lambda x:float(np.sqrt((x**2).mean()))
    w2=writes[-1];best={}
    for o2 in range(w2,min(w2+1500,n),4):
        for mode in ('overlap','cut'):
            r=rms(mix-put(A,o1,None if mode=='overlap' else o2-o1)-put(B,o2))
            if mode not in best or r<best[mode][0]:best[mode]=(r,o2)
    best['drop']=(rms(mix-put(A,o1)),None)
    order=sorted(best,key=lambda m:best[m][0])
    return dict(mode=order[0],residual=round(best[order[0]][0],1),second=order[1],second_residual=round(best[order[1]][0],1),
                mix_rms=round(rms(mix[o1:]),1),latency_b=None if best[order[0]][1] is None else round((best[order[0]][1]-w2)/RATE*1000,1))
def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--rom-dir',type=Path,required=True);p.add_argument('--work',type=Path,required=True)
    p.add_argument('--map',type=Path,default=ROOT/'data/sound_map.json');p.add_argument('--source',type=Path,default=ROOT/'filesystem/audio-src')
    p.add_argument('--labels',nargs='+',default=[]);p.add_argument('--pairs',type=Path,help='JSON list of [A,B] label pairs (explicit mode)');p.add_argument('--chunk',type=int,default=160);p.add_argument('--ref',nargs='+',default=['pistol','explosion','rebel_1041'])
    p.add_argument('--spacing',type=int,default=12);p.add_argument('--gap',type=int,default=90)
    p.add_argument('--output',type=Path,required=True);p.add_argument('--mame',default='/usr/games/mame')
    a=p.parse_args()
    words={s['label']:int(s['word'],16) for s in json.loads(a.map.read_text())['sounds']}
    want=set(a.labels)|set(a.ref)|({l for q in json.loads(a.pairs.read_text()) for l in q} if a.pairs else set())
    clips={l:load(a.source,l) for l in want}
    def plan_for(pairs):
        plan=[]
        for x,y in pairs:
            bx,by=bytes_of(words[x]),bytes_of(words[y])
            if by[0]==bx[0]:by=by[1:]     # 0120A4 sends the bank byte only when it changes
            seq=bx+['+%d'%(a.spacing-len(by)+1)]+by
            frames=int(max(len(clips[x]),a.spacing*800+len(clips[y]))/800)+30
            plan.append('p_%s_%s=%s:%d'%(x,y,','.join(s if isinstance(s,str) else '%02X'%s for s in seq),frames))
        return plan
    results={};round_no=[0]
    def measure(pairs):
        pairs=[q for q in pairs if '%s>%s'%q not in results]
        if not pairs:return
        round_no[0]+=1
        pcm,events=run(plan_for(pairs),a.work/('round%d'%round_no[0]),a.rom_dir,a.mame,a.gap)
        for x,y in pairs:
            lab='p_%s_%s'%(x,y);mine=[e for e in events if e['label']==lab]
            st=next(e for e in mine if e['kind']=='start');en=next(e for e in mine if e['kind']=='end')
            t0=seconds(st['time']);mix=pcm[int(t0*RATE):int(seconds(en['time'])*RATE)]
            ws=[int((seconds(e['time'])-t0)*RATE) for e in mine if e['kind']=='write' and e['frame']>=st['frame']]
            results['%s>%s'%(x,y)]=classify_env(mix,ws,clips[x],clips[y])
            print(x,'>',y,results['%s>%s'%(x,y)],flush=True)
    if a.pairs:
        # Explicit pair list: rendered in chunks (each render deleted once
        # analysed: disk), both classifiers kept per pair.
        todo=[tuple(q) for q in json.loads(a.pairs.read_text())]
        out=json.loads(a.output.read_text()) if a.output.exists() else {}
        todo=[q for q in todo if '%s>%s'%q not in out]
        for i in range(0,len(todo),a.chunk):
            part=todo[i:i+a.chunk];work=a.work/('chunk%d'%(len(out)//a.chunk+i//a.chunk))
            pcm,events=run(plan_for(part),work,a.rom_dir,a.mame,a.gap)
            for x,y in part:
                lab='p_%s_%s'%(x,y);mine=[e for e in events if e['label']==lab]
                st=next(e for e in mine if e['kind']=='start');en=next(e for e in mine if e['kind']=='end')
                t0=seconds(st['time']);mix=pcm[int(t0*RATE):int(seconds(en['time'])*RATE)]
                ws=[int((seconds(e['time'])-t0)*RATE) for e in mine if e['kind']=='write' and e['frame']>=st['frame']]
                out['%s>%s'%(x,y)]=dict(wave=classify(mix,ws,clips[x],clips[y]),envelope=classify_env(mix,ws,clips[x],clips[y]))
            a.output.write_text(json.dumps(out,indent=0)+'\n');(work/'render.wav').unlink()
            print('chunk done',len(out),flush=True)
        return
    labels=[l for l in a.labels if l not in a.ref]
    measure([(x,x) for x in labels]+[q for x in labels for r in a.ref for q in ((r,x),(x,r))])
    def joins(x,r):return results['%s>%s'%(r,x)]['mode']!='overlap' or results['%s>%s'%(x,r)]['mode']!='overlap'
    groups={r:[r] for r in a.ref};pending=[]
    for x in labels:
        hit=[r for r in a.ref if joins(x,r)]
        if hit:groups[hit[0]].append(x)
        else:pending.append(x)
    # Batches: every pending label against every new group's first label;
    # the first label joining none starts the next group.
    while pending:
        reps=[g for g in groups if g not in a.ref]
        measure([q for x in pending for r in reps for q in ((r,x),(x,r))])
        rest=[]
        for x in pending:
            hit=[r for r in reps if joins(x,r)]
            if hit:groups[hit[0]].append(x)
            else:rest.append(x)
        if rest:groups[rest[0]]=[rest[0]]
        pending=rest[1:]
    a.output.write_text(json.dumps(dict(source='MAME 0.264 isolated pair renders (tools/measure_sound_pairs.py), spacing %d frames'%a.spacing,
        groups=groups,pairs=results),indent=1)+'\n')
    print(json.dumps(groups,indent=1))
if __name__=='__main__':main()
