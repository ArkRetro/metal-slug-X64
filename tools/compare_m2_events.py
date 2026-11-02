#!/usr/bin/env python3
"""Mission 2 chain of events: the arcade's against the port's.

Arcade: data/mission2_inventory.json (MAME 0.264 debugger playthrough
run4-debug, SPAWN probe ROM 01A132): every op 7 record the level script
executed, in order, with its frame, camera progress (10E99E) and node.
Port: the ares log of the reference cartridge (tools/test_mission2_ares.py),
lines `MSX script spawn record R constructor C native N camera P` written by
actor.c for every record neo_script executed (native: the world created
the actor; 0: dropped).

Prints, per script section (stage), the records executed by each side in
order, the longest common subsequence (order agreement), the records only
one side executed, and for every record of the arcade whether the port
created its actor. --markdown writes the table of docs/MISION2.md."""
import argparse,json,re
from pathlib import Path
SECTIONS=((1,0x2D7298,0x2D7516),(2,0x2D7516,0x2D87E4),(3,0x2D87E4,0x2DA000))
def section(record):
    for i,lo,hi in SECTIONS:
        if lo<=record<hi:return i
    return 0
def lcs(a,b):
    n,m=len(a),len(b);t=[[0]*(m+1) for _ in range(n+1)]
    for i in range(n-1,-1,-1):
        for j in range(m-1,-1,-1):
            t[i][j]=t[i+1][j+1]+1 if a[i]==b[j] else max(t[i+1][j],t[i][j+1])
    return t[0][0]
def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('log',type=Path,help='ares log of the reference cartridge')
    p.add_argument('--inventory',type=Path,default=Path(__file__).resolve().parent.parent/'data/mission2_inventory.json')
    p.add_argument('--markdown',action='store_true')
    a=p.parse_args()
    inv=json.loads(a.inventory.read_text())
    arcade=sorted(((s['frame'],int(s['record'],16),int(k['constructor'],16),s['camera']) for k in inv['kinds'] for s in k['spawns']))
    port=[];native={}
    for line in a.log.read_text(errors='replace').splitlines():
        m=re.search(r'MSX script spawn record ([0-9a-f]+) constructor ([0-9a-f]+) native (\d) camera (\d+)',line)
        if m:
            r,c,n,cam=int(m.group(1),16),int(m.group(2),16),int(m.group(3)),int(m.group(4))
            port.append((r,c,cam));native.setdefault(c,set()).add(n)
    rows=[]
    for sec,_,_ in SECTIONS:
        A=[r for _,r,_,_ in arcade if section(r)==sec];P=[r for r,_,_ in port if section(r)==sec]
        common=lcs(A,P)
        onlyA=sorted(set(A)-set(P));onlyP=sorted(set(P)-set(A))
        rows.append((sec,len(A),len(P),common,onlyA,onlyP))
        print(f'section {sec}: arcade {len(A)} records, port {len(P)}, in the same order {common}')
        if onlyA:print('  only the arcade:',' '.join('%06X'%r for r in onlyA))
        if onlyP:print('  only the port:',' '.join('%06X'%r for r in onlyP))
    ctor={c for _,_,c,_ in arcade}
    dropped=sorted(c for c in ctor if native.get(c)=={0})
    print('constructors the port executed but did not create:',' '.join('%06X'%c for c in dropped) or 'none')
    if a.markdown:
        print('\n| Etapa | Registros (arcade) | Registros (port) | Mismo orden | Solo arcade | Solo port |\n| --- | --- | --- | --- | --- | --- |')
        for sec,na,np_,common,oa,op in rows:
            print(f"| {sec} | {na} | {np_} | {common} | {' '.join('`%06X`'%r for r in oa) or '—'} | {' '.join('`%06X`'%r for r in op) or '—'} |")
if __name__=='__main__':main()
