#!/usr/bin/env python3
"""Check a built Marco pose catalog (build_marco_poses.py --out-dir DIR)
against audited MAME captures (audit_marco_catalog.py, status PASS).

For every descriptor an audit verified (its SCB pens equal the ROM decode
and its colours the screenshot, 0 pixels unexplained):
  1. the pose is in the catalog;
  2. the catalog's parts (DIR/build/marco-png/pN.png, the mksprite sources,
     with the FNV of the header) hold exactly the pens of the ROM decode
     (export_character.pose_parts), at the catalog's anchor;
  3. every palette bank the capture drew it with (MARCO_DRAW attribute,
     palette<frame>.bin of a dumped frame) is a palette of the catalog,
     reported by its MARCO_PALETTE_ name (the dump three frames after the
     draw: the frame that shows it).
Also, with --base OLD_DIR (a catalog built before): every pose of OLD_DIR
has the same anchor, parts, pixel checks (CRC per old palette, both mirrors)
and part sprite bytes in DIR. Usage:
check_marco_catalog.py DIR --rom mslugx.zip [--base OLD] CAPTURE[...]"""
import argparse,filecmp,json,re,struct,sys
from pathlib import Path
from PIL import Image
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'tools'))
from export_character import Rom,pose_parts,rgba

def parse(path):
    t=path.read_text()
    pal=t[t.index('marco_palettes['):];pal=pal[pal.index('={')+2:pal.index('};')]
    palettes=[list(map(int,r.split(','))) for r in re.findall(r'\{([\d,]+)\}',pal)]
    parts=[tuple(map(int,m)) for m in re.findall(r'^\{(-?\d+),(-?\d+),(\d+),(\d+),(\d+)\},$',t[t.index('marco_parts[]'):t.index('marco_poses[]')],re.M)]
    poses=[]
    for line in t[t.index('marco_poses[]'):].splitlines()[1:]:
        m=re.match(r'^\{(\d+)u,(\d+),(\d+),(-?\d+),(-?\d+)',line)
        if not m:break
        poses.append(dict(tag=int(m[1]),first=int(m[2]),count=int(m[3]),anchor=(int(m[4]),int(m[5])),crc=[int(x) for x in re.findall(r'(\d+)u',line)[1:]]))
    if 'marco_pose_crc' in t:   # the checks moved under #ifdef MARCO_POSE_CRC
        for pose,line in zip(poses,t[t.index('marco_pose_crc'):].splitlines()[1:]):pose['crc']=[int(x) for x in re.findall(r'(\d+)u',line)]
    named={int(v):n for n,v in re.findall(r'#define MARCO_PALETTE_(\w+) (\d+)u',t)}
    return palettes,parts,poses,named

def main():
    p=argparse.ArgumentParser(description=__doc__,formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument('catalog',type=Path);p.add_argument('captures',nargs='+',type=Path)
    p.add_argument('--rom',type=Path,required=True);p.add_argument('--base',type=Path)
    a=p.parse_args()
    palettes,parts,poses,named=parse(a.catalog/'build/marco_pose_meta.h');by_tag={q['tag']:q for q in poses}
    crom=(ROOT/'filesystem/sprites.bin').read_bytes();failed=0
    if a.base:
        bp,bparts,bposes,bnamed=parse(a.base/'build/marco_pose_meta.h');files=0
        assert palettes[:len(bp)]==bp,'the old palettes keep their indices'
        assert all(named.get(i)==n for i,n in bnamed.items()),'the old names keep their indices'
        for q in bposes:
            r=by_tag[q['tag']];assert r['anchor']==q['anchor'] and r['count']==q['count'] and r['crc'][:len(q['crc'])]==q['crc'],hex(q['tag'])
            for k in range(q['count']):
                assert bparts[q['first']+k]==parts[r['first']+k],hex(q['tag'])
                assert filecmp.cmp(a.base/f"romfs/marco/p{q['first']+k}.sprite",a.catalog/f"romfs/marco/p{r['first']+k}.sprite",shallow=False),hex(q['tag'])
                files+=1
        print('base %s: %d poses and %d part sprites identical; %d palettes kept; %d new poses'%(a.base,len(bposes),files,len(bp),len(poses)-len(bposes)))
    for cap in a.captures:
        audit=json.loads((cap/'audit.json').read_text());assert audit['status']=='PASS',cap
        rom=Rom(a.rom,cap/'program-fixed.bin');text=(cap/'debug.log').read_text()
        draws={}
        for f,d,attr in re.findall(r'MARCO_DRAW frame=(\d+) descriptor=([0-9A-F]+) xy=[0-9A-F]+ attr=([0-9A-F]+)',text):
            draws.setdefault(int(d,16),set()).add((int(f),int(attr,16)>>8))
        report={}
        for tag in (int(t,16) for t in audit['descriptor_addresses']):
            q=by_tag.get(tag)
            if not q:print('  %06X: not in the catalog'%tag);failed+=1;continue
            own,anchor=pose_parts(rom,crom,tag)
            ok=anchor==q['anchor'] and len(own)==q['count']
            for k,part in enumerate(own if ok else ()):
                i=q['first']+k;x,y,w,h,fnv=parts[i]
                pens=list(Image.open(a.catalog/f'build/marco-png/p{i}.png').tobytes())
                ok=ok and (x,y,w,h,fnv)==(part['x'],part['y'],part['width'],part['height'],part['fnv']) and pens==part['pens']
            if not ok:print('  %06X: catalog pixels differ from the ROM decode'%tag);failed+=1;continue
            names=set()
            for f,bank in sorted(draws.get(tag,())):
                # The screen shows an update three frames later (the
                # submission delay audit_marco_catalog.py checks on the SCBs);
                # the palette 009EA8 requests is written by then.
                path=cap/f'palette{f+3}.bin'
                if not path.exists():continue
                words=list(struct.unpack('>4096H',path.read_bytes())[bank*16:bank*16+16])
                c=rgba(words)
                if c in palettes:names.add(named.get(palettes.index(c),'profile %d'%palettes.index(c)))
                else:names.add('MISSING %02X:%s'%(bank,','.join('%04X'%v for v in words[1:4])));failed+=1
            report[tag]=sorted(names)
        groups={}
        for tag,names in report.items():groups.setdefault(tuple(names),[]).append(tag)
        print('%s: %d descriptors, %d frames, %d pixels equal in MAME; all in the catalog with the ROM pens'%(cap.name,len(report),audit['verified_frames'],audit['matched_pixels']) if len(report)==len(audit['descriptor_addresses']) else cap.name)
        for names,tags in groups.items():print('  palettes %s: %s'%(' '.join(names) or '(no dumped frame)',' '.join('%06X'%t for t in tags)))
    if failed:sys.exit('FAILED: %d'%failed)
    print('PASS')
if __name__=='__main__':main()
