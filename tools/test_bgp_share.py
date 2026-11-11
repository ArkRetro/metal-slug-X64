#!/usr/bin/env python3
"""Shared columns of the pre-rendered backgrounds (docs/COMPRESION.md,
"Duplicados"): renders every cartridge .bgp (the Makefile's PRERENDER_SPECS,
the same layer selection as `prerender_stage.py build`) twice, with each
column stored on its own and with equal columns stored once, then reads every
column the way src/scene_prerender.c does (header, layer records, column
table, `bytes` from `offset`) and requires the same header, layer records,
column sizes, run counts and column bytes from both files. Also requires the
shared file to be the one in build/bgp when that exists. Skips without the
private packs. Usage: test_bgp_share.py PACK:STAGES..."""
import struct,sys,tempfile,io,contextlib
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
import prerender_stage as ps

def columns(d):
    assert d[:4]==b'MSXB',"magic"
    head=d[:16];layers=[];cols=[]
    for l in range(4):
        x0,n,slot,z,to,z2=struct.unpack_from('>hHHHII',d,16+16*l);layers.append((x0,n,slot,z,z2))
        for c in range(n):
            a,b,r=struct.unpack_from('>IHH',d,to+8*c)
            assert a+b<=len(d),"column outside the file"
            cols.append((l,c,b,r,d[a:a+b] if b else b''))
    return head,layers,cols

def main(specs):
    tested=0;saved=0
    with tempfile.TemporaryDirectory() as tmp:
        for spec in specs:
            path,stages=spec.rsplit(':',1)
            if not Path(path).exists():continue
            pk=ps.Pack(path)
            for k in (range(len(pk.stages)) if stages=='all' else [int(v)-1 for v in stages.split(',')]):
                pre=ps.select_layers(pk,k)
                if ps.sky_cells(pk,k):pre=pre|{0}
                if not pre:continue
                a=Path(tmp)/'own.bgp';b=Path(tmp)/'shared.bgp'
                with contextlib.redirect_stdout(io.StringIO()):
                    ps.render(path,k,a,pre,share=False);ps.render(path,k,b,pre,share=True)
                da,db=a.read_bytes(),b.read_bytes()
                ha,la,ca=columns(da);hb,lb,cb=columns(db)
                name=f'm{pk.mission}s{k+1}.bgp'
                assert ha==hb and la==lb and len(ca)==len(cb),name+": header or layers differ"
                for x,y in zip(ca,cb):assert x==y,f"{name}: layer {x[0]} column {x[1]} differs"
                built=Path('build/bgp')/name
                if built.exists():assert built.read_bytes()==db,f"{name}: build/bgp is not the shared render"
                print(f'{name}: {len(ca)} columns equal, {len(da)} -> {len(db)} bytes')
                tested+=1;saved+=len(da)-len(db)
    if not tested:print('bgp share: no private packs, skipped');return
    print(f'PASS: bgp share: {tested} files, every column the same, {saved} bytes saved')
if __name__=='__main__':main(sys.argv[1:])
