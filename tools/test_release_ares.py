#!/usr/bin/env python3
"""Compare eight actual RDP framebuffer CRCs between normal/release modules."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import time
ROOT=Path(__file__).resolve().parents[1]
def capture(rom, output):
    output.mkdir(parents=True,exist_ok=False)
    env=dict(os.environ,LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu')
    emulator=None
    with (output/'xvfb.log').open('w') as xlog:
        server=subprocess.Popen(['/workspace/emulator/root/usr/bin/Xvfb','-displayfd','1','-screen','0','1024x768x24','-ac','-nolisten','tcp'],stdout=subprocess.PIPE,stderr=xlog,env=env,text=True)
        try:
            display=server.stdout.readline().strip();assert display.isdigit(),'Xvfb failed'
            env['DISPLAY']=':'+display
            with (output/'ares.log').open('w') as log:
                emulator=subprocess.Popen(['bash',str(ROOT/'tools/run_ares.sh'),str(rom.resolve())],stdout=log,stderr=subprocess.STDOUT,env=env)
                deadline=time.monotonic()+55
                while time.monotonic()<deadline:
                    text=(output/'ares.log').read_text()
                    assert 'ASSERTION FAILED' not in text and 'Assertion failed' not in text,text[-3000:]
                    if 'MSX RELEASE REGRESSION PASS samples=8' in text:break
                    assert emulator.poll() is None,'ares exited early'
                    time.sleep(.1)
                else:raise AssertionError('No rendering completion marker')
            rows=re.findall(r'MSX RELEASE SAMPLE mode=(\d) camera=(\d+) crc=([0-9a-f]{8})',text)
            assert len(rows)==8 and len(set(rows))==8,rows
            assert [(int(m),int(c)) for m,c,_ in rows]==[(m,c) for m in range(2) for c in (0,64,320,872)]
            return dict(samples=rows,rom_sha256=hashlib.sha256(rom.read_bytes()).hexdigest(),log_sha256=hashlib.sha256((output/'ares.log').read_bytes()).hexdigest())
        finally:
            if emulator is not None:
                emulator.terminate()
                try:emulator.wait(timeout=8)
                except subprocess.TimeoutExpired:emulator.kill();emulator.wait()
            server.terminate();server.wait(timeout=8)
def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--debug',type=Path,default=ROOT/'scene-debug.z64')
    p.add_argument('--release',type=Path,default=ROOT/'scene-release.z64')
    p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();assert not a.output.exists(),'Preserve previous results'
    debug=capture(a.debug,a.output/'debug');release=capture(a.release,a.output/'release')
    assert debug['samples']==release['samples'],'NDEBUG changed rendered framebuffer pixels'
    report=dict(status='PASS',emulator='ares 146',debug=debug,release=release,
        limit='Eight fixed scene frames; verifies rendering equivalence, not full Mission 1 or hardware performance.')
    (a.output/'validation.json').write_text(json.dumps(report,indent=2)+'\n')
    print('PASS: all eight debug/release RDP framebuffers match')
if __name__=='__main__':main()
