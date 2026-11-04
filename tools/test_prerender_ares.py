#!/usr/bin/env python3
"""Compare actual RDP pixels/commands for source tiles and baked backgrounds."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import time

ROOT=Path(__file__).resolve().parents[1]
def capture(rom,output):
    output.mkdir(parents=True,exist_ok=False)
    env=dict(os.environ,LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu')
    emulator=None
    with (output/'xvfb.log').open('w') as xlog:
        server=subprocess.Popen(['/workspace/emulator/root/usr/bin/Xvfb','-displayfd','1','-screen','0','1024x768x24','-ac','-nolisten','tcp'],stdout=subprocess.PIPE,stderr=xlog,env=env,text=True)
        try:
            display=server.stdout.readline().strip();assert display.isdigit()
            env['DISPLAY']=':'+display
            with (output/'ares.log').open('w') as log:
                emulator=subprocess.Popen(['bash',str(ROOT/'tools/run_ares.sh'),str(rom.resolve())],stdout=log,stderr=subprocess.STDOUT,env=env)
                deadline=time.monotonic()+90
                while time.monotonic()<deadline:
                    text=(output/'ares.log').read_text()
                    assert not re.search(r'ASSERTION FAILED|Assertion failed|RSP CRASH',text),text[-3000:]
                    if 'MSX BACKGROUND PASS samples=128' in text:break
                    assert emulator.poll() is None,text[-3000:]
                    time.sleep(.1)
                else:raise AssertionError('Background readback timeout')
            rows=[dict(zip(('mode','camera','phase','crc','draws','uploads','tlut','bytes'),r)) for r in re.findall(r'MSX BACKGROUND mode=(\d) camera=(\d+) phase=(\d) crc=([0-9a-f]{8}) draws=(\d+) uploads=(\d+) tlut=(\d+) bytes=(\d+)',text)]
            assert len(rows)==128
            assert [(int(r['mode']),int(r['camera']),int(r['phase'])) for r in rows]==[(m,c,p) for m in range(2) for c in (0,31,64,320,872,1022,1023,1920) for p in range(8)]
            switch=re.search(r'MSX BACKGROUND SWITCH crc=([0-9a-f]{8})',text);assert switch,'Missing mixed CI8/CI4 palette ownership check'
            return dict(samples=rows,palette_switch_crc=switch[1],rom_sha256=hashlib.sha256(rom.read_bytes()).hexdigest(),log_sha256=hashlib.sha256((output/'ares.log').read_bytes()).hexdigest())
        finally:
            if emulator is not None:
                emulator.terminate()
                try:emulator.wait(timeout=8)
                except subprocess.TimeoutExpired:emulator.kill();emulator.wait()
            server.terminate();server.wait(timeout=8)

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--tiles',type=Path,required=True)
    p.add_argument('--prerender',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();assert not a.output.exists(),'Preserve previous evidence'
    tiles=capture(a.tiles,a.output/'tiles');baked=capture(a.prerender,a.output/'prerender')
    differences=[dict(mode=x['mode'],camera=x['camera'],phase=x['phase'],tiles=x['crc'],prerender=y['crc']) for x,y in zip(tiles['samples'],baked['samples']) if x['crc']!=y['crc']]
    totals={name:{k:sum(int(r[k]) for r in data['samples']) for k in ('draws','uploads','tlut','bytes')} for name,data in [('tiles',tiles),('prerender',baked)]}
    assert tiles['palette_switch_crc']==baked['palette_switch_crc'],'CI8/CI4 palette transition changed actor/background pixels'
    report=dict(status='FAIL' if differences else 'PASS',emulator='ares 146',tiles=tiles,prerender=baked,differences=differences,totals=totals,limits='128 fixed backgrounds: both presentations, eight cameras, eight animation phases. Command counts do not establish frame rate or hardware performance; campaign completion is not tested.')
    (a.output/'validation.json').write_text(json.dumps(report,indent=2)+'\n')
    assert not differences,f'{len(differences)} RDP framebuffer mismatches; inspect validation.json'
    print(json.dumps(dict(status='PASS',samples=128,totals=totals)))
if __name__=='__main__':main()
