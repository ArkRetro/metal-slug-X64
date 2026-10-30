#!/usr/bin/env python3
"""Make visual-mode changes rebuild the dependent object, without stale CFLAGS."""
import argparse
from pathlib import Path
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('mode',choices=('classic','depth25d'));p.add_argument('--scenario',choices=('campaign','vehicle'),default='campaign');p.add_argument('--output',type=Path,required=True)
# Optional depth25d effects (bitmask): 1 perspective layers, 2 dynamic lights, 4 layered fog.
p.add_argument('--depth-style',type=int,default=0,choices=range(8))
a=p.parse_args();value='#define MSX_RETRO25D '+('1' if a.mode=='depth25d' else '0')+'\n'
value+='#define MSX_DEPTH_STYLE %d\n'%a.depth_style
value+='#define MSX_VEHICLE_REFERENCE '+('1' if a.scenario=='vehicle' else '0')+'\n'
a.output.parent.mkdir(parents=True,exist_ok=True)
if not a.output.exists() or a.output.read_text()!=value:a.output.write_text(value)
