#!/usr/bin/env python3
"""Audit of the Mission 1 script spawns against the native world.

Walks data/mission1_script.json (tools/decode_level_script.py: every op 7
record with the camera of the last camera wait before it) and asks the host
helper tools/script_native.c (neo_world_spawn_native) which constructors the
port creates. Prints every group-counted spawn the port drops (it holds a
script wait in the arcade, so the port's event timeline diverges there),
then the uncounted ones. Report only: it does not fail; the list shrinks as
families become native."""
import argparse,json,subprocess
from pathlib import Path
def walk(statements,camera):
    for s in statements:
        if s['op']==0:camera[0]=s['camera']
        yield s,camera[0]
        for key in ('body','then'):
            if key in s:yield from walk([s[key]],camera)
        for key in ('statements','alternatives'):
            if key in s:yield from walk(s[key],camera)
def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('helper',type=Path);p.add_argument('--script',type=Path,default=Path(__file__).resolve().parent.parent/'data/mission1_script.json')
    a=p.parse_args();d=json.loads(a.script.read_text())
    spawns=[(s,c) for s,c in walk(d['statements'],[0]) if s['op']==7]
    ctors=sorted({s['constructor'] for s,_ in spawns})
    out=subprocess.run([str(a.helper)]+ctors,check=True,capture_output=True,text=True).stdout.split('\n')
    native={int(l.split()[0],16):l.split()[1]=='1' for l in out if l.strip()}
    stops=sorted('0x%06X'%int(l.split()[0],16) for l in out if l.strip() and l.split()[1]=='2')
    counted=[(s,c) for s,c in spawns if s['group_counted'] and s['group']<9 and not native[int(s['constructor'],16)]]
    print('script spawn audit: %d records, %d constructors, %d native'%(len(spawns),len(ctors),sum(native.values())))
    print('counted spawns the port drops (%d):'%len(counted))
    for s,c in counted:print('  camera %4d record %s constructor %s group %d'%(c,s['address'],s['constructor'],s['group']))
    unc=sorted({s['constructor'] for s,_ in spawns if not (s['group_counted'] and s['group']<9) and not native[int(s['constructor'],16)] and s['constructor'] not in stops})
    print('constructors stopped explicitly (%d, logged, not created): %s'%(len(stops),' '.join(stops)))
    print('uncounted constructors the port drops (%d): %s'%(len(unc),' '.join(unc)))
if __name__=='__main__':main()
