#!/usr/bin/env python3
"""Streaming budget of the boss bank in the scene pack (build/scene_meta.h):
for every logic pass of the recorded fights (data/boss_fight_*.json.gz), the
16 KiB chunks and 2 KiB pages of the descriptors the native boss draws, plus
the largest number of distinct pages one pass uploads to TMEM (each part is
one <=64x64 CI4 page = 2048 bytes, the TMEM limit with TLUT)."""
import gzip,json,re
from pathlib import Path
from check_scene_budget import table
ROOT=Path(__file__).resolve().parents[1]
def main():
    meta=(ROOT/'build/scene_meta.h').read_text()
    banks=table(meta,'scene_banks');images=table(meta,'scene_images')
    body=meta[meta.index('scene_native_descriptors[]={'):];body=body[:body.index('};')]
    descs={int(m.group(1),16):(int(m.group(2)),int(m.group(3))) for m in re.finditer(r'\{0x([0-9a-f]+),(\d+),(\d+)\}',body)}
    start=banks[5][0]
    assert all(images[start+f+i][4]<=64 and images[start+f+i][5]<=64 for f,c in descs.values() for i in range(c)),'part above one TMEM page'
    report={};empty={int(x,16) for x in json.loads((ROOT/'filesystem/boss-native.json').read_text())['empty']}
    for path in sorted((ROOT/'data').glob('boss_fight_*.json.gz')):
        d=json.load(gzip.open(path));main_id=d['nodes'][0]['id']
        passes=[];worst=dict(chunks=0,pages=0,draws=0,parts=0);allc=set();missing=set()
        for u in d['updates']:
            if u['n']==main_id or not passes:passes.append(dict(pages=set(),draws=0,parts=0))
            q=passes[-1]
            if u.get('handoff'):continue   # drawn by the generic code the node jumped into
            for e in u['ev']:
                if e[0]!='draw':continue
                tag=e[1][4]<<16|e[1][5]
                if tag not in descs:
                    if tag not in empty:missing.add(tag)
                    continue
                f,c=descs[tag];q['draws']+=1;q['parts']+=c
                for i in range(c):q['pages'].add(images[start+f+i][0])
        for q in passes:
            ch={pg//8 for pg in q['pages']};allc|=ch
            worst=dict(chunks=max(worst['chunks'],len(ch)),pages=max(worst['pages'],len(q['pages'])),draws=max(worst['draws'],q['draws']),parts=max(worst['parts'],q['parts']))
        report[d['name']]=dict(passes=len(passes),worst_pass=worst,fight_chunks=len(allc),missing_descriptors=sorted(hex(t) for t in missing if t))
    chunks=int(re.search(r'SCENE_CHUNK_COUNT (\d+)',meta).group(1))
    report['pack']=dict(chunks=chunks,boss_chunks=int(re.search(r'SCENE_BOSS_CHUNKS (\d+)',meta).group(1)),
        boss_pages=int(re.search(r'SCENE_BOSS_PAGES (\d+)',meta).group(1)),cache_slots_4MiB=32,cache_slots_8MiB=192)
    print(json.dumps(report,indent=1))
    for k,v in report.items():
        if k!='pack':assert not v['missing_descriptors'],(k,v['missing_descriptors'])
    return report
if __name__=='__main__':main()
