#!/usr/bin/env python3
"""Rebuilds every mission pack, in order (docs/COMPRESION.md).

A version 5 pack takes the actor pages an earlier mission's pack already
holds from that pack, so the packs depend on each other: mission 3 must be
built after mission 2 and again whenever mission 2's pack changes (the
runtime refuses a pack whose partner has another size). This runs
tools/pack_mission.py for each mission that has its extracted stages, lowest
first, with the arguments each one needs (RECIPES).

usage: build_packs.py [--output DIR] [--format 4|5] [FIRST]
  default: every mission, version 5, into romfs/ (the cartridge's packs);
  FIRST: that mission and the later ones only
"""
import argparse,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
# Stage directories under filesystem/missionN and extra arguments (docs/MISION2.md, docs/MISION3.md, docs/MISION4.md, docs/MISION5.md).
RECIPES={2:(('s1','s2','s3'),()),3:(('s1',),('--loose-layers',)),4:(('s1',),('--rom-layers',)),5:(('s1','s2','s3'),('--rom-layers',)),6:(('s1','s2','s3'),('--rom-layers',))}

def build(output=None,fmt=5,first=2,quiet=False):
    """Packs of the missions >= first that have their extracted stages; returns their numbers."""
    output=Path(output) if output else ROOT/'romfs';built=[]
    for m in sorted(RECIPES):
        stages,extra=RECIPES[m];dirs=[ROOT/f'filesystem/mission{m}'/s for s in stages]
        if m<first or not all((d/'map.json').exists() for d in dirs):continue
        cmd=[sys.executable,str(ROOT/'tools/pack_mission.py'),str(m),*map(str,dirs),*extra,'--format',str(fmt),'--output',str(output/f'mission{m}.pack')]
        if output.resolve()==(ROOT/'romfs').resolve():cmd+=['--report',str(ROOT/f'build/mission{m}_pack_report.json')]
        r=subprocess.run(cmd,stdout=subprocess.PIPE,text=True,check=True)
        if not quiet:print(r.stdout.splitlines()[0])
        built.append(m)
    return built

def main():
    p=argparse.ArgumentParser(description=__doc__,formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument('first',type=int,nargs='?',default=2,help='lowest mission to rebuild (the later ones are rebuilt too: they point into it)')
    p.add_argument('--output',type=Path);p.add_argument('--format',type=int,choices=(4,5),default=5)
    a=p.parse_args()
    (ROOT/'build').mkdir(exist_ok=True)
    print('built: '+', '.join(f'mission{m}.pack' for m in build(a.output,a.format,a.first)))
if __name__=='__main__':main()
