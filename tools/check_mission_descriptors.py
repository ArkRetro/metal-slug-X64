#!/usr/bin/env python3
"""Check that a mission pack holds every descriptor the mission can draw (docs/MISION6.md "Recorte").

The descriptors the mission references are the POSE assets of the animation programs it loads
(data/soldier_programs.json: programs without a group and those of pack_mission.GROUPS[mission]) that the
exports have (filesystem/missionN/rest-native.json, or the scene pack's soldier / boss / rest exports), plus
the exported descriptors no program poses (drawn by code). Each must be in the scene pack's exports or in the
mission pack's actor table (romfs/missionN.pack). Descriptors referenced but never exported are counted (a
missing drawing stops explicitly at run time) and listed with --verbose."""
import argparse,json,struct,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
import pack_mission,pack_store
ROOT=pack_mission.ROOT
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('mission',type=int)
    p.add_argument('--pack',type=Path);p.add_argument('--verbose',action='store_true');a=p.parse_args()
    m=a.mission;pack=pack_store.MissionPack(a.pack or ROOT/f'romfs/mission{m}.pack')
    in_pack={struct.unpack_from('>I',pack.raw,pack.actors_at+8*i)[0] for i in range(pack.actors)}
    scene=set()
    for name in ('soldier-native.json','boss-native.json','rest-native.json'):
        d=json.loads((ROOT/'filesystem'/name).read_text())
        scene|={int(x['descriptor'],16) for x in d['frames']}|{int(e,16) for e in d.get('empty',[])}
    ex=json.loads((ROOT/f'filesystem/mission{m}/rest-native.json').read_text())
    exported={int(x['descriptor'],16) for x in ex['frames']}|{int(e,16) for e in ex.get('empty',[])}
    sp=json.loads((ROOT/'data/soldier_programs.json').read_text())
    posed=set();ref=set()
    for prog in sp['programs']:
        ok=prog.get('group') is None or prog['group'] in pack_mission.GROUPS[m]
        for c in prog['commands']:
            if c['kind']=='POSE':
                posed.add(c['asset'])
                if ok:ref.add(c['asset'])
    ref|={d for d in exported if d not in posed}
    have=scene|exported
    missing=sorted(d for d in ref&have if d not in scene and d not in in_pack)
    never=sorted(ref-have-{0})
    dropped=sorted(d for d in in_pack if d not in scene and d in posed and d not in ref)
    print(f'mission {m}: {len(ref&have)} referenced descriptors exported, {len(in_pack)} in the pack, '
          f'{len(missing)} referenced and exported but not in the pack, {len(dropped)} in the pack the rule drops, '
          f'{len(never)} referenced but never exported (counted)')
    if a.verbose:print('never exported:',' '.join('%06X'%d for d in never))
    if missing or dropped:
        print('FAIL: not in the pack:',' '.join('%06X'%d for d in missing[:40]),'| dropped by the rule:',' '.join('%06X'%d for d in dropped[:40]))
        sys.exit(1)
    print(f'PASS: mission {m} pack holds every descriptor the mission references')
if __name__=='__main__':main()
