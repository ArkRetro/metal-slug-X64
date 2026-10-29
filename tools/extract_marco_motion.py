#!/usr/bin/env python3
"""Extract measured native movement-controller boundaries, not a full player replay."""
import argparse
from pathlib import Path
from extract_marco_crouch import extract
p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
extract(a.capture,a.output,profile='motion')
