#!/usr/bin/env python3
"""Render offline previews of the depth25d effects (no original artwork).

Compiles tools/preview_retro25d.c against the real src/retro25d.c and
src/retro_events.c, so the triangles, light pulses, shadows and fog are the
ones the N64 build submits. The level is a flat proxy built from the authored
ground profile and plane rectangles; actors are grey boxes. Each PNG shows the
frame on top and, below, the change the effects make (grey = unchanged).
"""
import argparse,json,os,subprocess
from pathlib import Path
from PIL import Image
ROOT=Path(__file__).resolve().parents[1]
SCENARIOS={'vehicle':'Vehicle contact shadow and explosion light, 3 ticks after the blast (camera 872).',
           'impact':'Airborne Marco (smaller, lighter shadow), muzzle flash, soldier shadow and a bullet-impact pulse (camera 560).',
           'decay':'Explosion pulse at 0, 6, 12 and 24 ticks: (1-t)^2 intensity decay over 30 ticks.'}

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output',type=Path,default=ROOT/'build/retro25d/previews')
    p.add_argument('--prefix',default='effects-')
    a=p.parse_args();a.output.mkdir(parents=True,exist_ok=True)
    assets=ROOT/'build/retro25d/retro_assets.h'
    if not assets.exists():subprocess.run(['python3',str(ROOT/'tools/build_retro_assets.py')],check=True)
    binary=ROOT/'build/host/preview-retro25d';binary.parent.mkdir(parents=True,exist_ok=True)
    subprocess.run([os.environ.get('HOST_CC','cc'),'-std=c11','-O2','-Wall','-Wextra','-Werror','-I',str(assets.parent),
        str(ROOT/'tools/preview_retro25d.c'),str(ROOT/'src/retro25d.c'),str(ROOT/'src/retro_events.c'),'-lm','-o',str(binary)],check=True)
    report={}
    for name,caption in SCENARIOS.items():
        ppm=a.output/f'{name}.ppm'
        stats=json.loads(subprocess.run([str(binary),name,str(ppm)],check=True,capture_output=True,text=True).stdout)
        png=a.output/f'{a.prefix}{name}.png';Image.open(ppm).save(png,optimize=True);ppm.unlink()
        assert stats['triangles']<=stats['limit'],stats
        report[png.name]=dict(caption=caption,triangles=stats['triangles'],limit=stats['limit'],bytes=png.stat().st_size)
        print(f'{png}: {stats["triangles"]}/{stats["limit"]} triangles, {png.stat().st_size} bytes')
    (a.output/f'{a.prefix}previews.json').write_text(json.dumps(dict(
        content='Proxy level (authored ground profile and planes) and grey actor boxes; contains no original artwork.',
        lower_half='0.5 + 4*(with effects - without) per channel; grey means unchanged',
        previews=report),indent=2)+'\n')

if __name__=='__main__':main()
