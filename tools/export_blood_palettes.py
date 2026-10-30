#!/usr/bin/env python3
"""Palettes of the BLOOD ON branch (soft-DIP option 6, RAM 10FD8F = 1).

Reads a tools/capture_blood.lua capture (palette RAM dumps and the 009EA8
resource table at RAM 1068EA of the same frames) and writes
filesystem/blood-native.json with the logical palette resources that only
BLOOD ON requests and that the other exports lack: 0x37A (death splash
040F98) and 0x18 (swordsman pieces 0A95F2/0A9876). The resources ON shares
with OFF (0x17, 0x20B, 0x20E, 0x3BA) are compared against the existing
exports: they must be identical, which checks that the capture and the
conversion agree with them. ROM-derived output: filesystem/ is not in git.

Usage: tools/export_blood_palettes.py CAPTURE_DIR"""
import argparse,collections,json,struct,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
from mission_tiles import rgba16
from export_soldier_sprites import resources
ROOT=Path(__file__).resolve().parents[1]
NEW=(0x37a,0x18)
SHARED=(0x17,0x20b,0x20e,0x3ba)
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path)
    a=p.parse_args()
    seen=collections.defaultdict(collections.Counter)
    for pal_file in sorted(a.capture.glob('palette*.bin')):
        frame=int(pal_file.stem[7:]);ram_file=a.capture/f'ram{frame}.bin'
        if not ram_file.exists():continue
        pw=struct.unpack('>4096H',pal_file.read_bytes())
        for slot,rid in resources(ram_file.read_bytes()).items():
            if rid in NEW+SHARED:seen[rid][tuple(rgba16(pw[slot*16:slot*16+16]))]+=1
    known={}
    for name in ('soldier-native.json','rest-native.json'):
        f=ROOT/'filesystem'/name
        if f.exists():known.update({int(k,16):v for k,v in json.loads(f.read_text())['palettes'].items()})
    report={}
    for rid in SHARED:
        if rid not in seen or rid not in known:continue
        best=list(seen[rid].most_common(1)[0][0])
        assert best==known[rid],('palette %#x differs from the existing export'%rid)
        report[hex(rid)]=dict(observations=sum(seen[rid].values()),variants=len(seen[rid]),matches_export=True)
    out={}
    for rid in NEW:
        assert rid in seen,('palette %#x not in the capture'%rid)
        best,n=seen[rid].most_common(1)[0];out[hex(rid)]=list(best)
        report[hex(rid)]=dict(observations=sum(seen[rid].values()),variants=len(seen[rid]),most_common=n)
    (ROOT/'filesystem/blood-native.json').write_text(json.dumps(dict(
        source='MAME 0.264 mslugx euro palette RAM, tools/capture_blood.lua (RAM 10FD8F = 1)',palettes=out),separators=(',',':'))+'\n')
    print(json.dumps(report))
if __name__=='__main__':main()
