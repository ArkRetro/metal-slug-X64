#!/usr/bin/env python3
"""Internal native MIPS module regressions in ares; no campaign claim."""
import argparse,os,subprocess,time,json,hashlib
from pathlib import Path
p=argparse.ArgumentParser();p.add_argument('rom',type=Path);p.add_argument('--output',type=Path,required=True);p.add_argument('--suite',choices=('body','common','upperform','environment','nativelogic','bootselect'),default='body');a=p.parse_args()
a.output.mkdir(parents=True,exist_ok=False);root=Path(__file__).resolve().parents[1]
env=dict(os.environ,LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu');process=None
with (a.output/'xvfb.log').open('w') as xlog,(a.output/'ares.log').open('w') as log:
 server=subprocess.Popen(['/workspace/emulator/root/usr/bin/Xvfb','-displayfd','1','-screen','0','1024x768x24','-ac','-nolisten','tcp'],stdout=subprocess.PIPE,stderr=xlog,text=True,env=env)
 try:
  display=server.stdout.readline().strip();assert display.isdigit();env['DISPLAY']=':'+display
  process=subprocess.Popen(['bash',str(root/'tools/run_ares.sh'),str(a.rom.resolve())],stdout=log,stderr=subprocess.STDOUT,env=env)
  deadline=time.monotonic()+45
  while time.monotonic()<deadline:
   text=(a.output/'ares.log').read_text()
   assert 'ASSERTION FAILED' not in text and 'Assertion failed' not in text and 'RSP CRASH' not in text
   if 'MSX '+a.suite.upper()+' REGRESSION PASS' in text:break
   assert process.poll() is None;time.sleep(.1)
  else:raise AssertionError('No completion marker')
  result=dict(status='PASS',emulator='ares 146',rom_sha256=hashlib.sha256(a.rom.read_bytes()).hexdigest(),suite=a.suite,limit='447 original condition calls with intercepted helper outputs plus 96 original player queries, 96 original position queries, 451 condition calls with real native services, and 264 event-consumer transitions. CPU regression only; no campaign/rendering claim.' if a.suite=='nativelogic' else '48 original boot-selector CPU fixtures with synthetic platform imports. MIPS registers/SR, import count/FNV32 and NV memory checked; exact byte streams compared by the MAME/host probe. No native platform/campaign binding or timing claim.' if a.suite=='bootselect' else '256 controlled original-M68000 arithmetic outputs plus synthetic branch/service tests; not gameplay/world/campaign coverage.' if a.suite=='environment' else 'Synthetic native backend/service boundaries only; not dynamic original-game replay, rendering or campaign integration.')
  if a.suite=='bootselect':
   cases=json.loads((root/'data/boot_select_fixture.json').read_text())['cases'];assert 'MSX BOOTSELECT REGRESSION PASS cases='+str(cases) in text;result['cases']=cases
  (a.output/'validation.json').write_text(json.dumps(result,indent=2)+'\n');print('PASS: native MIPS',a.suite,'in ares')
 finally:
  if process is not None:
   process.terminate()
   try:process.wait(timeout=8)
   except subprocess.TimeoutExpired:process.kill();process.wait()
  server.terminate();server.wait(timeout=8)
