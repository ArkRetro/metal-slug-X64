#!/usr/bin/env python3
"""Exercise actual RSP audio mixing and AI submission in ares; no demo release."""
import argparse,hashlib,json,os,re,subprocess,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
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
    match=re.search(r'MSX AUDIO REGRESSION PASS buffers=(\d+) nonzero=(\d+) checksum=([0-9a-fA-F]+) queued=(\d+) rendered=(\d+)',text)
    if match:break
    assert process.poll() is None,'ares exited';time.sleep(.1)
   else:raise AssertionError('Audio regression did not complete; inspect ares.log')
  sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
  result=dict(status='PASS',emulator='ares 146',buffers=int(match[1]),nonzero_samples=int(match[2]),pcm_checksum=match[3],ai_buffers_queued=int(match[4]),rdp_frames=int(match[5]),rom_sha256=sha(a.rom),log_sha256=sha(out/'ares.log'),asset_sha256=sha(ROOT/'build/audio-romfs/title-reference.wav64'),limits='Native title-reference decode/mix and AI submission. No acoustic capture, full intro, game integration, original SFX/music driver or hardware audio validation.')
  assert result['ai_buffers_queued']>=24 and result['nonzero_samples']>1024 and result['rdp_frames']>0
  (out/'validation.json').write_text(json.dumps(result,indent=2)+'\n');print(result)
 finally:
  if process is not None:
   process.terminate()
   try:process.wait(timeout=8)
   except subprocess.TimeoutExpired:process.kill();process.wait()
  server.terminate();server.wait(timeout=8)
