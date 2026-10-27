#!/usr/bin/env python3
"""Run measured backend fixtures on the actual MIPS build in ares, internally."""
import argparse,hashlib,json,os,subprocess,time
from pathlib import Path
from PIL import ImageGrab
ROOT=Path(__file__).resolve().parents[1]
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('rom',type=Path);p.add_argument('--output',type=Path,required=True)
p.add_argument('--suite',choices=('rng','animation'),default='rng')
a=p.parse_args();out=a.output.resolve()
fixture_path=ROOT/('data/soldier_rng_fixture.json' if a.suite=='rng' else 'data/soldier_animation_fixture.json')
fixture=json.loads(fixture_path.read_text())
if a.suite=='rng':
    counts=dict(seeds=len(fixture['seeds']),calls=len(fixture['steps']),initializers=len(fixture['initializers']),
        decisions=len(fixture['decisions']),damping=len(fixture['damping']))
else:
    counts=dict(starts=sum(c['kind']=='init' for c in fixture['cases']),steps=sum(c['kind']=='step' for c in fixture['cases']),
        draws=sum(len(c.get('draws',[])) for c in fixture['cases']))
marker='MSX '+('RNG' if a.suite=='rng' else 'ANIMATION')+' REGRESSION PASS '+' '.join(f'{key}={value}' for key,value in counts.items())
assert not out.exists() or not any(out.iterdir()),'Preserve previous results; use a fresh output'
out.mkdir(parents=True,exist_ok=True)
env=dict(os.environ,LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu')
process=None
with (out/'xvfb.log').open('w') as xlog:
    server=subprocess.Popen(['/workspace/emulator/root/usr/bin/Xvfb','-displayfd','1','-screen','0','1024x768x24','-ac','-nolisten','tcp'],stdout=subprocess.PIPE,stderr=xlog,env=env,text=True)
    try:
        number=server.stdout.readline().strip();assert number.isdigit(),'Xvfb failed'
        env['DISPLAY']=':'+number
        with (out/'ares.log').open('w') as log:
            process=subprocess.Popen(['bash',str(ROOT/'tools/run_ares.sh'),str(a.rom.resolve())],stdout=log,stderr=subprocess.STDOUT,env=env)
            deadline=time.monotonic()+45
            while time.monotonic()<deadline:
                text=(out/'ares.log').read_text()
                assert 'ASSERTION FAILED' not in text and 'Assertion failed' not in text,text[-2000:]
                if marker in text:break
                assert process.poll() is None,'ares exited before completing the fixture'
                time.sleep(.1)
            else:raise AssertionError('No backend completion marker in ares.log')
            ImageGrab.grab(xdisplay=env['DISPLAY']).save(out/'pass.png')
        report=dict(status='PASS',emulator='ares 146, retained workspace build',
            rom_sha256=hashlib.sha256(a.rom.read_bytes()).hexdigest(),
            log_sha256=hashlib.sha256((out/'ares.log').read_bytes()).hexdigest(),
            fixture_sha256=hashlib.sha256(fixture_path.read_bytes()).hexdigest(),suite=a.suite,
            **counts,
            limit='Internal CPU backend regression; not a full game, rendering or frame-rate test. No release.')
        (out/'validation.json').write_text(json.dumps(report,indent=2)+'\n');print('PASS: measured',a.suite,'fixtures executed as native MIPS in ares')
    finally:
        if process is not None:
            process.terminate()
            try:process.wait(timeout=8)
            except subprocess.TimeoutExpired:process.kill();process.wait()
        server.terminate();server.wait(timeout=8)
