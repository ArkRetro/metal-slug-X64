#!/usr/bin/env python3
"""Iterative knife plan: schedule each cut at the next frame where +9A bit 0 (target in range) is set."""
# Usage (from the repository root): plan_knife_cuts.py OUTPUT p|h CUTS END_FRAME
# p: pistol; h: --give-hgun. Writes OUTPUT.plan.json (tools/mame/plans/marco-knife-*.json) and OUTPUT.cuts.json.
import json,subprocess,sys,shutil,os
from pathlib import Path
WS=Path(__file__).resolve().parents[2];OUT=Path(sys.argv[1]);HG=sys.argv[2]=='h';N=int(sys.argv[3]);END=int(sys.argv[4])
st=OUT.with_suffix('.cuts.json')
cuts=json.loads(st.read_text()) if st.exists() else []
def base(t):return ['Right'] if t<2236 or (t-2236)%48<16 else []
def build(cuts,end):
  cases=[];t=2100
  def fill(a,b):
    nonlocal cases
    seg=[];x=a
    while x<b:
      inp=base(x);y=x
      while y<b and base(y)==inp:y+=1
      seg.append([inp,y-x]);x=y
    # merge short
    out=[]
    for inp,l in seg:
      if out and (l<16 or out[-1][1]<16):out[-1][1]+=l
      else:out.append([inp,l])
    if out and out[-1][1]<16:
      if len(out)>1:out[-2][1]+=out.pop()[1]
    for inp,l in out:cases.append(dict(name=f'k_fill_{len(cases)}',length=l,inputs=inp))
  for i,(f,kind) in enumerate(cuts):
    s=f-16 if kind=='crouch' else f
    assert s-t>=16 or s==t,(s,t)
    fill(t,s)
    if kind=='crouch':cases.append(dict(name=f'k_crouch_{i}',length=16,inputs=['Down']))
    cases.append(dict(name=f'k_cut_{kind}_{i}',length=24,inputs=(['Down','fire'] if kind=='crouch' else ['fire'])))
    t=f+24
  fill(t,end);return cases
def run(cases):
  plan=OUT.with_suffix('.plan.json');plan.write_text(json.dumps(cases,indent=1)+'\n')
  shutil.rmtree(OUT,ignore_errors=True)
  cmd=['python3','tools/mame/run_marco_catalog.py','--rom-dir','/home/user/roms','--output',str(OUT),'--command-trace','--paired-trace','--controller-trace','--plan',str(plan)]+(['--give-hgun'] if HG else [])
  subprocess.run(cmd,cwd=WS,check=True,stdout=subprocess.DEVNULL)
def inrange(f):
  p=OUT/f'actor{f}.bin'
  return p.exists() and p.read_bytes()[0x9a]&1
NORMAL={0xbfd18,0xbfdd8,0xbf058,0xbff98,0xbf6d8,0xbee18,0xbf858,0xbeed8,0xbf258,0xbf718,0xbf8d8}
def frames(cases):
  out=[];t=2100
  for c in cases:
    for k in range(c['length']):out.append((tuple(c['inputs']),'fire' in c['inputs'] and k%8==0 and k<c['length']-1, k==0))
  return out
def knifed(f):
  ch=json.loads((OUT/'controller.json').read_text())['choices']
  return [hex(x['choice']['table']) for x in ch if f<=x['choice']['frame']<=f+4 and x['choice']['table'] not in NORMAL]
def ctl():subprocess.run(['python3','tools/extract_marco_controller.py',str(OUT),'--output',str(OUT/'controller.json')],cwd=WS,check=True,stdout=subprocess.DEVNULL)
bad=set();verified=len(cuts)
while True:
  cases=build(cuts,END);run(cases);ctl()
  if len(cuts)>verified:
    f,kind=cuts[-1];k=knifed(f)
    print('try',f,kind,k,flush=True)
    if k:verified=len(cuts);st.write_text(json.dumps(cuts))
    else:bad.add(f);cuts.pop();continue
  if len(cuts)>=N:break
  prev=frames(cases)
  last=cuts[-1][0]+24+16+2 if cuts else 2100
  kind='crouch' if len(cuts)%2 else 'stand'
  f=None
  for g in range(last+(16 if kind=='crouch' else 0),END-40):
    if g+1 in bad or not inrange(g):continue
    s=g+1-(16 if kind=='crouch' else 0)
    try:new=frames(build(cuts+[[g+1,kind]],END))
    except AssertionError:continue
    if new[:s-2100]==prev[:s-2100]:f=g;break
  if f is None:print('no window',kind);break
  cuts.append([f+1,kind])
print('final',cuts)
