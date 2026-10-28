#!/usr/bin/env python3
"""Verify every decoded cinematic frame on native MIPS in ares."""
import argparse,hashlib,json,os,re,subprocess,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
fixture=json.loads((ROOT/'filesystem/cinematics/entry.json').read_text())
p=argparse.ArgumentParser(description=__doc__);p.add_argument('rom',type=Path);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
out=a.output.resolve();assert not out.exists() or not any(out.iterdir());out.mkdir(parents=True,exist_ok=True)
env=dict(os.environ,LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu');process=None
with (out/'xvfb.log').open('w') as xlog:
 server=subprocess.Popen(['/workspace/emulator/root/usr/bin/Xvfb','-displayfd','1','-screen','0','1024x768x24','-ac','-nolisten','tcp'],stdout=subprocess.PIPE,stderr=xlog,text=True,env=env)
 try:
  display=server.stdout.readline().strip();assert display.isdigit();env['DISPLAY']=':'+display
  with (out/'ares.log').open('w') as log:
   process=subprocess.Popen(['bash',str(ROOT/'tools/run_ares.sh'),str(a.rom.resolve())],stdout=log,stderr=subprocess.STDOUT,env=env)
   deadline=time.monotonic()+60
   while time.monotonic()<deadline:
    text=(out/'ares.log').read_text();assert 'ASSERTION FAILED' not in text and 'Assertion failed' not in text,text[-2500:]
    match=re.search(r'MSX CINEMATIC REGRESSION PASS frames=(\d+) framebuffers=(\d+)',text)
    if match:break
    assert process.poll() is None,'ares exited';time.sleep(.1)
   else:raise AssertionError('Cinematic regression did not complete; inspect ares.log')
  sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
  result=dict(status='PASS',emulator='ares 146',frames=int(match[1]),framebuffers=int(match[2]),rom_sha256=sha(a.rom),log_sha256=sha(out/'ares.log'),movie_sha256=fixture['movie_sha256'],limits='All native decoded pixel CRCs and three RDP framebuffer CRCs, including prepared-frame cache paths. This is not a timing/audio/gameplay test.')
  assert result['frames']==fixture['frames'] and result['framebuffers']==3
  (out/'validation.json').write_text(json.dumps(result,indent=2)+'\n');print(result)
 finally:
  if process is not None:
   process.terminate()
   try:process.wait(timeout=8)
   except subprocess.TimeoutExpired:process.kill();process.wait()
  server.terminate();server.wait(timeout=8)
