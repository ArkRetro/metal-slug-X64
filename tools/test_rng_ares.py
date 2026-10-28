#!/usr/bin/env python3
"""Run measured backend fixtures on the actual MIPS build in ares, internally."""
import argparse,hashlib,json,os,subprocess,time
from pathlib import Path
from PIL import ImageGrab
ROOT=Path(__file__).resolve().parents[1]
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('rom',type=Path);p.add_argument('--output',type=Path,required=True)
p.add_argument('--suite',choices=('rng','animation','contact','control','state','throw','attack','object','locomotion'),default='rng')
a=p.parse_args();out=a.output.resolve()
fixture_path=ROOT/('data/soldier_'+a.suite+'_fixture.json')
fixture=json.loads(fixture_path.read_text())
if a.suite=='rng':
    counts=dict(seeds=len(fixture['seeds']),calls=len(fixture['steps']),initializers=len(fixture['initializers']),
        decisions=len(fixture['decisions']),damping=len(fixture['damping']))
elif a.suite=='throw':
    counts=dict(steps=len(fixture['cases']))
elif a.suite=='locomotion':
    counts=dict(steps=len(fixture['cases']),starts=sum(c['init'] for c in fixture['cases']),draws=len(fixture['cases']),rng=sum(c['rng_calls'] for c in fixture['cases']))
elif a.suite=='object':
    counts=dict(steps=len(fixture['cases']),draws=len(fixture['cases']),palettes=4,sounds=1,failures=11)
elif a.suite=='attack':
    throw=json.loads((ROOT/'data/soldier_throw_fixture.json').read_text())
    counts=dict(steps=sum(c['kind']=='step' for c in fixture['cases']),draws=sum(len(c.get('draws',[])) for c in fixture['cases']),events=sum(len(c.get('calls',[])) for c in fixture['cases']),flight=len(throw['cases']),child=len(json.loads((ROOT/'data/soldier_object_fixture.json').read_text())['cases']),child_draws=len(json.loads((ROOT/'data/soldier_object_fixture.json').read_text())['cases']))
elif a.suite=='animation':
    counts=dict(starts=sum(c['kind']=='init' for c in fixture['cases']),steps=sum(c['kind']=='step' for c in fixture['cases']),
        draws=sum(len(c.get('draws',[])) for c in fixture['cases']))
elif a.suite=='contact':
    animation=json.loads((ROOT/'data/soldier_animation_fixture.json').read_text())
    counts=dict(steps=len(fixture['cases']),draws=sum(len(c.get('draws',[])) for c in animation['cases']))
else:
    control=fixture if a.suite=='control' else json.loads((ROOT/'data/soldier_control_fixture.json').read_text())
    contact=json.loads((ROOT/'data/soldier_contact_fixture.json').read_text())
    animation=json.loads((ROOT/'data/soldier_animation_fixture.json').read_text())
    counts=dict(cycles=len(control['cases']),rng=len(control['rng']),moves=len(contact['cases']),
        draws=sum(len(c.get('draws',[])) for c in animation['cases']))
if a.suite=='state':counts=dict(culls=len(json.loads((ROOT/'data/soldier_cull_fixture.json').read_text())['cases']),actions=len(json.loads((ROOT/'data/soldier_action_fixture.json').read_text())['cases']),states=len(fixture['cases']),**counts)
marker='MSX '+a.suite.upper()+' REGRESSION PASS '+' '.join(f'{key}={value}' for key,value in counts.items())
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
        if a.suite in ('contact','control','state'):report['animation_fixture_sha256']=hashlib.sha256((ROOT/'data/soldier_animation_fixture.json').read_bytes()).hexdigest()
        if a.suite in ('control','state'):report['contact_fixture_sha256']=hashlib.sha256((ROOT/'data/soldier_contact_fixture.json').read_bytes()).hexdigest()
        if a.suite=='state':report['control_fixture_sha256']=hashlib.sha256((ROOT/'data/soldier_control_fixture.json').read_bytes()).hexdigest()
        if a.suite=='state':report['action_fixture_sha256']=hashlib.sha256((ROOT/'data/soldier_action_fixture.json').read_bytes()).hexdigest()
        if a.suite=='state':report['cull_fixture_sha256']=hashlib.sha256((ROOT/'data/soldier_cull_fixture.json').read_bytes()).hexdigest()
        if a.suite in ('attack','object'):report['throw_fixture_sha256']=hashlib.sha256((ROOT/'data/soldier_throw_fixture.json').read_bytes()).hexdigest()
        if a.suite in ('contact','control','state','throw','attack','object','locomotion'):report['terrain_source_sha256']=hashlib.sha256((ROOT/'filesystem/mission1/collision.json').read_bytes()).hexdigest()
        if a.suite=='attack':report['attack_state_fixture_sha256']=hashlib.sha256((ROOT/'data/soldier_attack_state_fixture.json').read_bytes()).hexdigest()
        if a.suite=='attack':report['main_states']=20;report['ground_queries']=40;report['hit_geometry_candidates']=8;report['hit_geometry_overlaps']=1
        if a.suite=='attack':report['hit_geometry_fixture_sha256']=hashlib.sha256((ROOT/'data/soldier_hit_geometry_fixture.json').read_bytes()).hexdigest()
        if a.suite=='attack':report['object_fixture_sha256']=hashlib.sha256((ROOT/'data/soldier_object_fixture.json').read_bytes()).hexdigest()
        (out/'validation.json').write_text(json.dumps(report,indent=2)+'\n');print('PASS: measured',a.suite,'fixtures executed as native MIPS in ares')
    finally:
        if process is not None:
            process.terminate()
            try:process.wait(timeout=8)
            except subprocess.TimeoutExpired:process.kill();process.wait()
        server.terminate();server.wait(timeout=8)
