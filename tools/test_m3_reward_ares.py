#!/usr/bin/env python3
"""Run the original-reference platform event test on compiled N64 code."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import time

ROOT=Path(__file__).resolve().parents[1]

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('rom',type=Path);p.add_argument('--output',type=Path,required=True)
    args=p.parse_args();out=args.output.resolve();out.mkdir(parents=True,exist_ok=False)
    env=dict(os.environ,LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu')
    emulator=None
    with (out/'xvfb.log').open('w') as xlog:
        server=subprocess.Popen(['/workspace/emulator/root/usr/bin/Xvfb','-displayfd','1',
            '-screen','0','640x480x24','-ac','-nolisten','tcp'],
            stdout=subprocess.PIPE,stderr=xlog,env=env,text=True)
        try:
            number=server.stdout.readline().strip();assert number.isdigit();env['DISPLAY']=':'+number
            with (out/'ares.log').open('w') as log:
                emulator=subprocess.Popen(['bash',str(ROOT/'tools/run_ares.sh'),str(args.rom.resolve())],
                    stdout=log,stderr=subprocess.STDOUT,env=env)
                until=time.monotonic()+90
                while time.monotonic()<until:
                    text=(out/'ares.log').read_text()
                    assert not re.search(r'ASSERTION FAILED|Assertion failed|M3 REWARD FAIL|RSP CRASH',text),text[-3000:]
                    match=re.search(r'MSX M3 REWARD PASS cases=(\d+) requests=(\d+) deletions=(\d+) effects=original',text)
                    if match:break
                    assert emulator.poll() is None,text[-3000:]
                    time.sleep(.1)
                else:raise AssertionError('MIPS platform reward test timeout\n'+text[-3000:])
            assert tuple(map(int,match.groups()))==(140,45,31)
            sha=lambda f:hashlib.sha256(f.read_bytes()).hexdigest()
            report=dict(status='PASS',emulator='ares146',cases=140,requests=45,deletions=31,
                rom_sha256=sha(args.rom),log_sha256=sha(out/'ares.log'),
                fixture_sha256=sha(ROOT/'data/m3_platform_reward_fixture.json'),
                effects_fixture_sha256=sha(ROOT/'data/m3_platform_reward_effects_fixture.json'),
                limits='Controlled original event, BCD score and label-placement references on MIPS. Allocator/sound queue are stub boundaries in MAME; no full mission, visual, PCM or hardware performance claim.')
            (out/'validation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
        finally:
            if emulator is not None:
                emulator.terminate()
                try:emulator.wait(timeout=8)
                except subprocess.TimeoutExpired:emulator.kill();emulator.wait()
            server.terminate();server.wait(timeout=8)

if __name__=='__main__':main()
