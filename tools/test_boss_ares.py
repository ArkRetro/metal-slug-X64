#!/usr/bin/env python3
"""Replay the recorded boss fights through the native boss as MIPS code in
ares 146 (boss-regression.z64: tools/test_neo_boss.c built for N64, fight
streams on DFS) and require the completion marker with the fixture counts."""
import argparse,gzip,hashlib,json,os,subprocess,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('rom',type=Path);p.add_argument('--output',type=Path,required=True);p.add_argument('--timeout',type=float,default=900)
a=p.parse_args();out=a.output.resolve()
fights=sorted((ROOT/'data').glob('boss_fight_*.json.gz'))
updates=sum(len(json.load(gzip.open(f))['updates']) for f in fights)
marker=f'MSX BOSS REGRESSION PASS fights={len(fights)} updates={updates} '
assert not out.exists() or not any(out.iterdir()),'Preserve previous results; use a fresh output'
out.mkdir(parents=True,exist_ok=True)
assert subprocess.run(['pgrep','-x','ares'],capture_output=True).returncode!=0,'another ares is running'
env=dict(os.environ,LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu')
process=None;start=time.monotonic()
with (out/'xvfb.log').open('w') as xlog:
    server=subprocess.Popen(['/workspace/emulator/root/usr/bin/Xvfb','-displayfd','1','-screen','0','1024x768x24','-ac','-nolisten','tcp'],stdout=subprocess.PIPE,stderr=xlog,env=env,text=True)
    try:
        number=server.stdout.readline().strip();assert number.isdigit(),'Xvfb failed'
        env['DISPLAY']=':'+number
        with (out/'ares.log').open('w') as log:
            process=subprocess.Popen(['bash',str(ROOT/'tools/run_ares.sh'),str(a.rom.resolve())],stdout=log,stderr=subprocess.STDOUT,env=env)
            deadline=time.monotonic()+a.timeout
            while time.monotonic()<deadline:
                text=(out/'ares.log').read_text(errors='replace')
                assert 'ASSERTION FAILED' not in text and 'Assertion failed' not in text and 'BOSS REGRESSION FAIL' not in text,text[-3000:]
                if marker in text:break
                assert process.poll() is None,'ares exited before completing the fixture'
                time.sleep(.5)
            else:raise AssertionError('No completion marker in ares.log')
        line=[l for l in text.splitlines() if marker in l][-1]
        report=dict(status='PASS',emulator='ares 146',seconds=round(time.monotonic()-start,1),marker=line.strip(),
            rom_sha256=hashlib.sha256(a.rom.read_bytes()).hexdigest(),
            fixtures={f.name:hashlib.sha256(f.read_bytes()).hexdigest() for f in fights},
            limit='Internal CPU regression of the boss logic (closed loop, service results replayed); not a rendering or frame-rate test.')
        (out/'validation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
    finally:
        if process is not None:
            process.terminate()
            try:process.wait(timeout=8)
            except subprocess.TimeoutExpired:process.kill();process.wait()
        server.terminate();server.wait(timeout=8)
