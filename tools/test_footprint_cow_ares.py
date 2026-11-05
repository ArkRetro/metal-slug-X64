#!/usr/bin/env python3
"""Check persistent foreground edits on the actual N64 RDP in ares."""
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
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('rom',type=Path)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args()
    out=args.output.resolve();out.mkdir(parents=True,exist_ok=False)
    env=dict(os.environ,LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu')
    emulator=None
    with (out/'xvfb.log').open('w') as xlog:
        server=subprocess.Popen(['/workspace/emulator/root/usr/bin/Xvfb','-displayfd','1',
            '-screen','0','1024x768x24','-ac','-nolisten','tcp'],
            stdout=subprocess.PIPE,stderr=xlog,env=env,text=True)
        try:
            display=server.stdout.readline().strip();assert display.isdigit()
            env['DISPLAY']=':'+display
            with (out/'ares.log').open('w') as log:
                emulator=subprocess.Popen(['bash',str(ROOT/'tools/run_ares.sh'),str(args.rom.resolve())],
                    stdout=log,stderr=subprocess.STDOUT,env=env)
                deadline=time.monotonic()+180
                while time.monotonic()<deadline:
                    text=(out/'ares.log').read_text()
                    assert not re.search(r'ASSERTION FAILED|Assertion failed|RSP CRASH|MSX FOOTPRINT COW (FAIL|mismatch)',text),text[-4000:]
                    result=re.search(r'MSX FOOTPRINT COW PASS samples=(\d+) pixels=(\d+) transparent=(\d+) changed=(\d+) noops=(\d+) multi=(\d+) max_upload=(\d+)',text)
                    if result:break
                    assert emulator.poll() is None,text[-4000:]
                    time.sleep(.1)
                else:raise AssertionError('Footprint RDP regression timeout\n'+text[-4000:])
            assert int(result[1])==22 and int(result[2])==5377536
            assert int(result[3])>0 and int(result[4])>0 and int(result[5])==2 and int(result[6])==10
            assert 0<int(result[7])<=2048
            sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
            report=dict(status='PASS',emulator='ares146',samples=int(result[1]),
                checked_pixels=int(result[2]),transparent_pen_visits=int(result[3]),
                changed_edits=int(result[4]),no_op_samples=int(result[5]),multi_edit_samples=int(result[6]),
                maximum_texture_upload=int(result[7]),rom_sha256=sha(args.rom),
                log_sha256=sha(out/'ares.log'),
                checks=['Full framebuffer equivalence to independently checked original-tile overlay',
                    'Classic and layered backgrounds, single and multiple source edits',
                    'Transparent erasure, shadows, idempotence, reset, immutable source pages'],
                limits='Foreground edit rendering only; no campaign completion or hardware FPS claim.')
            (out/'validation.json').write_text(json.dumps(report,indent=2)+'\n')
            print(json.dumps(report))
        finally:
            if emulator is not None:
                emulator.terminate()
                try:emulator.wait(timeout=8)
                except subprocess.TimeoutExpired:emulator.kill();emulator.wait()
            server.terminate();server.wait(timeout=8)

if __name__=='__main__':main()
