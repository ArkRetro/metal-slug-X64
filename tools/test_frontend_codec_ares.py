#!/usr/bin/env python3
"""Verify all original frontend decoded CRCs on native MIPS, including compact cache."""
import argparse
import hashlib
import json
import os
import re
import subprocess
import time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('rom',type=Path);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    fixture=json.loads((ROOT/'filesystem/cinematics/frontend/frontend.json').read_text())
    out=a.output.resolve();assert not out.exists() or not any(out.iterdir());out.mkdir(parents=True,exist_ok=True)
    env=dict(os.environ,LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu');process=None
    with (out/'xvfb.log').open('w') as xlog:
        server=subprocess.Popen(['/workspace/emulator/root/usr/bin/Xvfb','-displayfd','1','-screen','0','1024x768x24','-ac','-nolisten','tcp'],stdout=subprocess.PIPE,stderr=xlog,text=True,env=env)
        try:
            number=server.stdout.readline().strip();assert number.isdigit();env['DISPLAY']=':'+number
            with (out/'ares.log').open('w') as log:
                process=subprocess.Popen(['bash',str(ROOT/'tools/run_ares.sh'),str(a.rom.resolve())],stdout=log,stderr=subprocess.STDOUT,env=env)
                deadline=time.monotonic()+180
                while time.monotonic()<deadline:
                    text=(out/'ares.log').read_text()
                    assert all(s not in text for s in ('ASSERTION FAILED','Assertion failed','RSP CRASH','MSX ERROR:')),text[-3000:]
                    match=re.search(r'MSX FRONTEND CODEC PASS frames=(\d+)',text)
                    if match:break
                    assert process.poll() is None,'ares exited';time.sleep(.1)
                else:raise AssertionError('Native frontend CRC regression timeout')
            assert int(match[1])==sum(s['frames'] for s in fixture['segments'])
            for i,source in enumerate(fixture['segments']):
                assert f"MSX FRONTEND CODEC case={i} frames={source['frames']} PASS" in text
            sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
            result=dict(status='PASS',emulator='ares 146',decoded_frame_crcs=int(match[1]),rom_sha256=sha(a.rom),log_sha256=sha(out/'ares.log'),frontend_movie_sha256={s['name']:s['movie_sha256'] for s in fixture['segments']},limits='All3050 decoded RGBA5551 CRCs on native MIPS, including compact cache. Nine smoke-test RDP blits have no readback comparison. No timing/audio/gameplay/hardware claim.')
            (out/'validation.json').write_text(json.dumps(result,indent=2)+'\n');print(result)
        finally:
            if process is not None:
                process.terminate()
                try:process.wait(timeout=8)
                except subprocess.TimeoutExpired:process.kill();process.wait()
            server.terminate();server.wait(timeout=8)

if __name__=='__main__':main()
