#!/usr/bin/env python3
"""Add map cells seen in another capture to a recovered map
(filesystem/mission1/map.json by default), never changing a known cell.

The region map was recovered from captures whose camera rise (944..1008:
RAM 10E980 from 256 down to 240) happened at a different moment; the bottom
row of the foreground layer (y = 208) at x >= 1216 is only on screen while
the view has not finished rising, so it was never observed and the port
drew a strip of backdrop at the bottom right (the user's report: after the
truck 02916E, standing on its wreck). The soldier capture m2x has VRAM dumps
during the rise (6370..6420, camera 879..1012) and after it (to 1920):

  python3 tools/extract_mission_map.py /path/captures-soldiers/m2x \\
      --frames 6370:9400 --allow-backdrop-timeline --output OUT
  for l in 3 2 1 0; do python3 tools/merge_observed_cells.py OUT/map.json --layer $l --x=0:4000; done

(100 cells: the foreground's bottom row y = 208 at x 1216..1440 and rows
above the view top that the rise reveals, layers 0..3.)

Every cell of the observation that the map already has must be identical
(code, flip, animation mask, palette contents); only absent cells are
added, with their palettes mapped to the map's palette list; a cell with
recorded coordinate variants ('conflicts') may show any of them."""
import argparse,json
from pathlib import Path
def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('observed',type=Path)
    p.add_argument('--map',type=Path,default=Path(__file__).resolve().parent.parent/'filesystem/mission1/map.json')
    p.add_argument('--layer',type=int,required=True)
    p.add_argument('--x',required=True,help='LO:HI inclusive cell x range')
    a=p.parse_args();lo,hi=(int(v) for v in a.x.split(':'))
    new=json.loads(a.observed.read_text());base=json.loads(a.map.read_text())
    key=lambda m,i:json.dumps(m['palettes'][i])
    index={json.dumps(pal):i for i,pal in enumerate(base['palettes'])}
    have={(c['layer'],c['x'],c['y']):c for c in base['cells']}
    # Cells with recorded coordinate variants (map 'conflicts'): any of the
    # recorded choices is consistent.
    variants={(c['layer'],c['x'],c['y']):{(v['code'],v['flip'],json.dumps(v['palette'])) for v in c['choices']} for c in base.get('conflicts',[])}
    checked=added=0;new_cells=[]
    for c in new['cells']:
        if c['layer']!=a.layer or not lo<=c['x']<=hi:continue
        k=(c['layer'],c['x'],c['y'])
        if k in have:
            s=have[k]
            if k in variants and (c['code'],c['flip'],key(new,c['palette'])) in variants[k]:checked+=1;continue
            assert (s['code'],s['flip'],s.get('animation_mask',0),key(base,s['palette']))==(c['code'],c['flip'],c.get('animation_mask',0),key(new,c['palette'])),('cell differs',k)
            checked+=1;continue
        pal=key(new,c['palette'])
        if pal not in index:base['palettes'].append(json.loads(pal));index[pal]=len(base['palettes'])-1
        cell={f:c[f] for f in c if f!='palette'};cell['palette']=index[pal]
        base['cells'].append(cell);new_cells.append(k);added+=1
    a.map.write_text(json.dumps(base,separators=(',',':'))+'\n')
    print(json.dumps(dict(checked=checked,added=added,cells=sorted(new_cells))))
if __name__=='__main__':main()
