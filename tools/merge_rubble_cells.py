#!/usr/bin/env python3
"""Add the rubble columns of the domed building seen right after its collapse
to the rest-of-mission map (filesystem/mission1/rest/map.json, swapped_cells).

0B58CC loads the rubble map 0B53A8 into the foreground layer 10C72E while the
camera is still at 1920; the arcade camera then scrolls on (2029 thirty
frames later), so the long-period captures (run8) only saw the rubble columns
from x = 2016. A capture with a dump every frame across the collapse
(tools/run_capture_rest.sh with MSX_REST_EXTRA_DUMPS=6980..7035, e.g.
captures-m1full/col1) sees 10C72E already on 0B53A8 (layers.csv +2E) from its
first post-swap frame, so tools/extract_mission_map.py records those cells as
ordinary layer-3 cells (no pointer change inside the capture):

  python3 tools/extract_mission_map.py CAPTURE --frames 6997:7035 \\
      --allow-backdrop-timeline --output OUT
  python3 tools/merge_rubble_cells.py OUT/map.json

Every rubble cell already in swapped_cells must be identical (code, flip,
palette contents) in the new observation; the new columns are added with
their palettes mapped to the rest map's palette list. Nothing is guessed:
only observed cells are added."""
import argparse,json
from pathlib import Path
RUBBLE_FIRST_X=0x74*16   # 1856: map column 0x74 of 0B58CC
def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('observed',type=Path,help='map.json of the post-collapse capture (layer-3 cells = rubble 0B53A8)')
    p.add_argument('--map',type=Path,default=Path(__file__).resolve().parent.parent/'filesystem/mission1/rest/map.json')
    a=p.parse_args()
    new=json.loads(a.observed.read_text());rest=json.loads(a.map.read_text())
    key=lambda m,i:json.dumps(m['palettes'][i])
    index={json.dumps(pal):i for i,pal in enumerate(rest['palettes'])}
    have={(c['x'],c['y']):c for c in rest['swapped_cells']}
    lo=min(c['x'] for c in rest['swapped_cells']);hi=max(c['x'] for c in rest['swapped_cells'])
    cells=[c for c in new['cells'] if c['layer']==3 and RUBBLE_FIRST_X<=c['x']<=hi]
    checked=added=0
    for c in cells:
        k=(c['x'],c['y'])
        if k in have:
            s=have[k]
            assert (s['code'],s['flip'],s.get('animation_mask',0),key(rest,s['palette']))==(c['code'],c['flip'],c.get('animation_mask',0),key(new,c['palette'])),('rubble cell differs',k)
            checked+=1;continue
        assert c['x']<lo,('new rubble cell inside the known columns',k)
        pal=key(new,c['palette'])
        if pal not in index:rest['palettes'].append(json.loads(pal));index[pal]=len(rest['palettes'])-1
        rest['swapped_cells'].append(dict(layer=3,map='0x0B53A8',x=c['x'],y=c['y'],code=c['code'],flip=c['flip'],
            animation_mask=c.get('animation_mask',0),palette=index[pal],observations=c.get('observations',1)))
        added+=1
    rest['swapped_cells'].sort(key=lambda c:(c['x'],c['y']))
    a.map.write_text(json.dumps(rest,separators=(',',':'))+'\n')
    print(json.dumps(dict(checked=checked,added=added,swapped_cells=len(rest['swapped_cells']),columns=sorted({c['x'] for c in rest['swapped_cells']}))))
if __name__=='__main__':main()
