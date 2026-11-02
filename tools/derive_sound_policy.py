#!/usr/bin/env python3
"""Voice policy of the gameplay sound effects from measured pairs.

Input: the pair results of tools/measure_sound_pairs.py (explicit mode; one
or more JSON files, each pair 'A>B' with the waveform fit 'wave' and the
power-envelope fit 'envelope'). A pair's outcome is the waveform fit when it
explains the mix (residual below half the mix RMS and ahead of the second
mode by 10 % of it: ADPCM-A samples are sample-exact between renders), else
the envelope fit when its margin is at least 0.05 (FM/SSG parts are not), else
unclear.

Channels: the three measured ones of docs/AUDIO_JUEGO.md (weapon 10F7/108D,
impact 1027/10FA/1023/1033, voice 103E..1043) and new ones: two sounds share
a channel when one of their pairs (either order) is not an overlap; groups
are the connected components. Levels inside a channel: B cutting A gives
level(B) >= level(A), B dropped under A gives level(B) < level(A); the known
levels stay fixed and the rest take the lowest level meeting every
constraint, never leaving the interval its pairs with the known sounds
give (the outcomes between new sounds are not always a total order: the
violated constraints are reported).
A sound with an unclear pair against every member of its channel keeps the
channel of its clear pairs. Output: data/sound_policy.json (numbers)."""
import argparse,json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
KNOWN={'pistol':('weapon',0),'vehicle_hit':('weapon',1),'explosion':('impact',0),'knife_hit':('impact',1),
       'vehicle_smoke':('impact',2),'vehicle_burst':('impact',2),'rebel_103e':('voice',1),'rebel_103f':('voice',0),
       'rebel_1040':('voice',1),'rebel_1041':('voice',1),'rebel_1042':('voice',1),'rebel_1043':('voice',0)}
def outcome(r):
    w,e=r.get('wave') or {},r.get('envelope') or {}
    if w.get('mode') in ('overlap','cut','drop') and w['residual']<0.5*w['mix_rms'] and w['second_residual']-w['residual']>0.1*w['mix_rms']:
        return w['mode'],'wave'
    if e.get('mode') in ('overlap','cut','drop') and e.get('margin',0)>=0.05:return e['mode'],'envelope'
    return 'unclear',None
def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('pairs',nargs='+',type=Path);p.add_argument('--map',type=Path,default=ROOT/'data/sound_map.json')
    p.add_argument('--output',type=Path,default=ROOT/'data/sound_policy.json')
    # Pairs not used to join channels (listed in 'bridges'): each is a
    # waveform-only cut, residual at the half-mix limit, that the envelope
    # fit reads as an overlap, and it would merge the pickup voices of the
    # 1056 group into the impact channel although every other measured
    # cross pair of the two groups overlaps (none cuts or drops; dozens are
    # clear overlaps in both orders). B36: 1159/115B; docs/AUDIO_JUEGO.md 14.
    p.add_argument('--bridge',nargs='*',default=['sfx_1159>sfx_1032','sfx_115b>vehicle_burst'])
    a=p.parse_args()
    pairs={}
    for f in a.pairs:
        d=json.loads(f.read_text());pairs.update(d.get('pairs',d))
    res={k:outcome(v) for k,v in pairs.items()}
    labels=sorted({l for k in pairs for l in k.split('>')})
    parent={l:l for l in labels}
    def find(x):
        while parent[x]!=x:parent[x]=parent[parent[x]];x=parent[x]
        return x
    for l in labels:
        if l in KNOWN:
            for m in labels:
                if m in KNOWN and KNOWN[m][0]==KNOWN[l][0]:parent[find(m)]=find(l)
    # A non-overlap pair joins the two sounds' groups unless the union would
    # hold two measured channels (a single pair against the rest: listed in
    # 'bridges', not used).
    def known_of(root):return {KNOWN[m][0] for m in labels if m in KNOWN and find(m)==root}
    bridges=[]
    for k,(mode,_) in sorted(res.items()):
        x,y=k.split('>')
        if mode not in ('cut','drop'):continue
        if len(known_of(find(x))|known_of(find(y)))>1 or k in a.bridge:bridges.append(k);continue
        parent[find(y)]=find(x)
    comps={}
    for l in labels:comps.setdefault(find(l),[]).append(l)
    channels={};names={}
    for root,members in comps.items():
        known=[KNOWN[m][0] for m in members if m in KNOWN]
        assert len(set(known))<=1,('measured channels merged',members)
        name=known[0] if known else 'ch_'+sorted(members)[0].replace('sfx_','')
        names[root]=name
        for m in members:channels[m]=name
    # Levels: B after A on one channel.
    level={m:KNOWN[m][1] for m in labels if m in KNOWN};cons=[];contradictions=[]
    for k,(mode,_) in res.items():
        x,y=k.split('>')
        if x==y or channels[x]!=channels[y]:continue
        if mode=='cut':cons.append((y,x,0))       # level(y) >= level(x)
        elif mode=='drop':cons.append((x,y,1))    # level(x) >= level(y)+1
    for m in labels:level.setdefault(m,0)
    # Bounds from the measured levels first (a new sound between two known
    # ones), then the new-new constraints inside those bounds: the driver's
    # outcomes between new sounds are not always a total order (cycles), so
    # a level never leaves the interval its pairs with known sounds give.
    top={m:max([0]+[v[1] for v in KNOWN.values() if v[0]==channels[m]])+1 for m in labels}
    for hi,lo,d in cons:
        if hi in KNOWN and lo not in KNOWN:top[lo]=min(top[lo],level[hi]-d)
        if lo in KNOWN and hi not in KNOWN:level[hi]=max(level[hi],level[lo]+d)
    for _ in range(len(labels)+2):
        changed=False
        for hi,lo,d in cons:
            if hi in KNOWN:continue
            want=min(level[lo]+d,top[hi])
            if level[hi]<want:level[hi]=want;changed=True
        if not changed:break
    for hi,lo,d in cons:
        if level[hi]<level[lo]+d:contradictions.append('%s >= %s + %d'%(hi,lo,d))
    words={s['label']:s['word'] for s in json.loads(a.map.read_text())['sounds']}
    out=dict(source='tools/derive_sound_policy.py from MAME 0.264 isolated pair renders (tools/measure_sound_pairs.py)',
        rule='same channel: a cut or drop in either order; level: cut >=, drop <',
        sounds={l:dict(word=words.get(l),channel=channels[l],level=level[l],
            pairs={k:res[k][0] for k in res if l in k.split('>')}) for l in labels},
        channels=sorted(set(channels.values())),contradictions=contradictions,bridges=bridges,
        unclear=sorted(k for k,v in res.items() if v[0]=='unclear'))
    a.output.write_text(json.dumps(out,indent=1)+'\n')
    for c in out['channels']:print(c,sorted((level[l],l) for l in labels if channels[l]==c))
    print('contradictions',contradictions);print('bridges',bridges);print('unclear pairs',len(out['unclear']))
if __name__=='__main__':main()
