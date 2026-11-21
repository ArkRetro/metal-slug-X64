#!/usr/bin/env python3
"""make check: the compressed packs give the game exactly the data the packs
of before gave it (docs/COMPRESION.md).

From the same sources, two sets of cartridge files are built: the old one
(pack_scene.py --legacy and pack_mission.py --format 4: run-pair chunks,
every page stored in its own pack) and the new one (LZ4 chunks, version 5
mission packs that take shared actor pages from another mission's pack). The
loader itself, src/scene_assets.c, is compiled for the host
(tools/test_pack_loader.c) and run over both; what it hands to the game,
table by table and page by page, is written out and must be byte-identical.

The cartridge's own packs (romfs/) are then loaded the same way: they must
load (every chunk decodes to its CRC, the packs a mission takes pages from
are the ones it was built against) and, if they are not what the sources
give now, a warning says to rebuild them (tools/build_packs.py).
"""
import filecmp,os,shutil,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'tools'))
import build_packs
def run(*cmd):subprocess.run([str(c) for c in cmd],check=True)
def main():
    if not (ROOT/'romfs/scene.pack').exists() or not (ROOT/'filesystem/sprites.bin').exists():
        print('SKIP: pack compression test (no private data)');return
    host=ROOT/'build/host';old=host/'pack-old';new=host/'pack-new'
    for d in (old,new):
        shutil.rmtree(d,ignore_errors=True);d.mkdir(parents=True)
    run(sys.executable,ROOT/'tools/pack_scene.py','--legacy',old)
    shutil.copyfile(ROOT/'romfs/scene.pack',new/'scene.pack')
    missions=build_packs.build(old,4,quiet=True);assert build_packs.build(new,5,quiet=True)==missions
    cc=os.environ.get('HOST_CC','cc').split()+'-std=gnu11 -Wall -Wextra -Werror -Wno-unused-function -O1'.split()
    cc+=[f'-I{ROOT}/tools/host_n64',f'-I{ROOT}/src',str(ROOT/'tools/test_pack_loader.c')]
    # The old scene.pack has its own chunk table (scene_meta.h); the rest of the header is the same file.
    run(*cc,f'-I{old}',f'-I{ROOT}/build','-o',host/'pack-loader-old')
    run(*cc,f'-I{ROOT}/build','-o',host/'pack-loader')
    run(host/'pack-loader-old',old,host/'pack-old.txt');run(host/'pack-loader',new,host/'pack-new.txt')
    a=(host/'pack-old.txt').read_bytes();b=(host/'pack-new.txt').read_bytes()
    if a!=b:
        la=a.split(b'\n');lb=b.split(b'\n');i=next((i for i,(x,y) in enumerate(zip(la,lb)) if x!=y),min(len(la),len(lb)))
        sys.exit(f'FAIL: the compressed packs differ from the old ones at line {i+1} of build/host/pack-old.txt / pack-new.txt')
    sizes=lambda d:sum(f.stat().st_size for f in d.glob('*.pack'))
    print(f'PASS: scene.pack and missions {missions}: the loader gives the same {len(a)} bytes of tables, images and pages from the old packs ({sizes(old)} B) and the compressed ones ({sizes(new)} B)')
    # The cartridge's packs.
    cart=host/'pack-cart';shutil.rmtree(cart,ignore_errors=True);cart.mkdir()
    for f in (ROOT/'romfs').glob('*.pack'):os.symlink(f.resolve(),cart/f.name)
    run(host/'pack-loader',cart,host/'pack-cart.txt')
    stale=[f.name for f in sorted(new.glob('mission*.pack')) if not (ROOT/'romfs'/f.name).exists() or not filecmp.cmp(f,ROOT/'romfs'/f.name,shallow=False)]
    if stale:print('WARNING: '+', '.join(stale)+' in romfs/ are not what the sources give now: python3 tools/build_packs.py')
    else:print('PASS: romfs/ mission packs are the ones the sources give')
    for d in (old,new,cart):shutil.rmtree(d,ignore_errors=True)
    for f in ('pack-old.txt','pack-new.txt','pack-cart.txt'):(host/f).unlink()
if __name__=='__main__':main()
