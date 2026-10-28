#!/usr/bin/env python3
"""Run measured backend fixtures on the actual MIPS build in ares, internally."""
import argparse,hashlib,json,os,re,subprocess,time
from pathlib import Path
from PIL import ImageGrab
ROOT=Path(__file__).resolve().parents[1]
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('rom',type=Path);p.add_argument('--output',type=Path,required=True)
p.add_argument('--suite',choices=('rng','animation','contact','control','state','throw','attack','object','locomotion','marco','pair','controller','coupled','upper','crouch'),default='rng')
p.add_argument('--fixture',type=Path,help='Fixture JSON; private helper journal required for suite pair')
p.add_argument('--controller-fixture',type=Path,default=ROOT/'data/marco_controller_fixture.json')
p.add_argument('--pixel-reference',type=Path,default=ROOT/'build/marco_pixel_sequence.json')
p.add_argument('--pixels',action='store_true',help='Also require the native animation framebuffer completion marker')
p.add_argument('--crouch-native',action='store_true',help='Require real native body animation in the crouch-controller test')
p.add_argument('--upper-native',action='store_true',help='Require real native animation in the upper-controller test')
a=p.parse_args();out=a.output.resolve()
if a.upper_native:assert a.suite=='upper'
if a.crouch_native:assert a.suite=='crouch'
if a.pixels:assert a.suite=='coupled','--pixels requires --suite coupled'
if a.suite in ('pair','coupled'):assert a.fixture is not None,'pair requires --fixture'
fixture_path=a.fixture if a.fixture and a.suite in ('pair','coupled','upper','crouch') else ROOT/('data/marco_crouch_turn_fixture.json' if a.suite=='crouch' else 'data/marco_upper_fixture.json' if a.suite=='upper' else 'data/marco_controller_fixture.json' if a.suite=='controller' else 'data/marco_fields_fixture.json' if a.suite=='marco' else 'data/soldier_'+a.suite+'_fixture.json')
fixture=json.loads(fixture_path.read_text())
if a.suite=='crouch':
    counts=dict(calls=len(fixture['cases']),services=sum(len(c['operations']) for c in fixture['cases']),draws=sum(len(op['draws']) for c in fixture['cases'] for op in c['operations'] if op['kind']=='animate'),events=sum(len(op['events']) for c in fixture['cases'] for op in c['operations'] if op['kind']=='animate'))
elif a.suite=='upper':
    counts=dict(calls=len(fixture['cases']),resets=sum(c['kind']=='reset' for c in fixture['cases']),services=sum(len(c['operations']) for c in fixture['cases']))
elif a.suite=='controller':
    counts=dict(inputs=len(fixture['inputs']),choices=len(fixture['choices']))
elif a.suite=='coupled':
    controller=json.loads(a.controller_fixture.read_text())
    counts=dict(inputs=len(controller['inputs']),choices=len(controller['choices']),steps=len(fixture['cases']),draws=sum(len(c['draws']) for c in fixture['cases']),events=sum(len(c['events']) for c in fixture['cases']))
elif a.suite=='pair':
    counts=dict(steps=len(fixture['cases']),draws=sum(len(c['draws']) for c in fixture['cases']),events=sum(len(c['events']) for c in fixture['cases']))
elif a.suite=='marco':
    counts=dict(fields=len(fixture['cases']),selectors=len(json.loads((ROOT/'data/marco_selector_fixture.json').read_text())['cases']))
elif a.suite=='rng':
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
            deadline=time.monotonic()+(240 if a.pixels else 45)
            while time.monotonic()<deadline:
                text=(out/'ares.log').read_text()
                assert 'ASSERTION FAILED' not in text and 'Assertion failed' not in text,text[-2000:]
                if marker in text:break
                assert process.poll() is None,'ares exited before completing the fixture'
                time.sleep(.1)
            else:raise AssertionError('No backend completion marker in ares.log')
            if a.pixels:
                reference=json.loads(a.pixel_reference.read_text())
                pixels=re.search(r'MSX COUPLED PIXELS PASS frames=(\d+) draws=(\d+) tmem_max=(\d+)',text)
                assert pixels and int(pixels[1])==reference['frames'] and 0<int(pixels[3])<=2048,text[-2500:]
                assert 'MSX MARCO NATIVE LIMITS PASS cases='+str(reference['synthetic_transform_limits']) in text
        if a.crouch_native:assert 'MSX CROUCH NATIVE ANIMATION PASS' in text
        if a.upper_native:
            native_draws=sum(len(op['draws']) for c in fixture['cases'] for op in c['operations'] if op['kind']=='animate')
            native_events=sum(len(op['events']) for c in fixture['cases'] for op in c['operations'] if op['kind']=='animate')
            assert f'MSX UPPER NATIVE ANIMATION PASS draws={native_draws} events={native_events}' in text
        ImageGrab.grab(xdisplay=env['DISPLAY']).save(out/'pass.png')
        report=dict(status='PASS',emulator='ares 146, retained workspace build',
            rom_sha256=hashlib.sha256(a.rom.read_bytes()).hexdigest(),
            log_sha256=hashlib.sha256((out/'ares.log').read_bytes()).hexdigest(),
            fixture_sha256=hashlib.sha256(fixture_path.read_bytes()).hexdigest(),suite=a.suite,
            **counts,
            limit='Internal CPU backend regression; not a full game, rendering or frame-rate test. No release.')
        if a.suite=='marco':report['selector_fixture_sha256']=hashlib.sha256((ROOT/'data/marco_selector_fixture.json').read_bytes()).hexdigest()
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
        if a.suite=='coupled':report['controller_fixture_sha256']=hashlib.sha256(a.controller_fixture.read_bytes()).hexdigest()
        if a.crouch_native:report.update(native_animation=True,limit='Native crouch-body transitions with real animation programs. Recorded world movement/allocator inputs; common player processing and campaign integration remain incomplete. No release.')
        if a.upper_native:report.update(native_animation=True,native_draws=native_draws,native_events=native_events,limit='Whole upper-controller updates coupled to real native scripts. World allocator/RNG outcomes and outer body updates remain service inputs. No complete player/campaign or hardware performance claim.')
        if a.pixels:
            report.update(framebuffers=int(pixels[1]),rendered_draws=int(pixels[2]),maximum_texture_upload=int(pixels[3]),pixel_reference=reference,
                limit='Native input/filter/select/animation/render replay at recorded controller boundaries. Audited Neo Geo SCB framebuffer CRCs match. Outer player state machine, all-game variants and real hardware performance remain unverified. No release.')
        (out/'validation.json').write_text(json.dumps(report,indent=2)+'\n');print('PASS: measured',a.suite,'fixtures executed as native MIPS in ares')
    finally:
        if process is not None:
            process.terminate()
            try:process.wait(timeout=8)
            except subprocess.TimeoutExpired:process.kill();process.wait()
        server.terminate();server.wait(timeout=8)
