#!/usr/bin/env python3
"""Compile measured movement-controller boundaries, without original assets."""
import argparse
from pathlib import Path
from compile_marco_crouch import compile_fixture
MODES=dict(zip((0xc2642,0xc2654,0xc2692,0xc2706,0xc271c,0xc2746,0xc27a8,0xc27e8,0xc27fa,0xc285c,0xc2888,0xc1ebe,0xc2214,0xc1f4a,0xc1efa,0xc215e),('MMR_RUN_JUMP','MMR_JUMP','MMR_AIR','MMR_FALL_ACTION','MMR_FALL','MMR_TURN_START','MMR_TURN','MMR_TURN_ACTION_START','MMR_TURN_ACTION','MMR_RUN_START','MMR_RUN','MMR_LAND_REQUEST','MMR_CROUCH_REQUEST','MMR_IDLE_FAST_REQUEST','MMR_IDLE_REQUEST','MMR_MELEE_REQUEST')))
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--fixture',type=Path,default=Path('data/marco_motion_fixture.json'));p.add_argument('--output',type=Path,default=Path('build/marco-motion/marco_motion_fixture.h'));a=p.parse_args()
compile_fixture(a.fixture,a.output,profile='motion',modes=MODES)
