#!/usr/bin/env python3
"""Run the original-descriptor pixel regression on native MIPS/RDP in ares."""
import argparse,hashlib,json,os,re,subprocess,time
from pathlib import Path
from PIL import ImageGrab
ROOT=Path(__file__).resolve().parents[1]
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('rom',type=Path);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
 assets=json.loads((ROOT/'build/marco_pose_assets.json').read_text());poses=len(assets['poses']);parts=len(assets['parts']);palettes=len(assets['palette_rgba5551'])
 marker=f'MSX MARCO PIXELS PASS poses={poses} frames={poses*2*palettes} chunks={parts} palettes={palettes}'
 out=a.output.resolve();out.mkdir(parents=True,exist_ok=False)
 env=dict(os.environ,LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu');process=None
 with (out/'xvfb.log').open('w') as xlog:
  server=subprocess.Popen(['/workspace/emulator/root/usr/bin/Xvfb','-displayfd','1','-screen','0','1024x768x24','-ac','-nolisten','tcp'],stdout=subprocess.PIPE,stderr=xlog,env=env,text=True)
  try:
   number=server.stdout.readline().strip();assert number.isdigit();env['DISPLAY']=':'+number
   with (out/'ares.log').open('w') as log:
    process=subprocess.Popen(['bash',str(ROOT/'tools/run_ares.sh'),str(a.rom.resolve())],stdout=log,stderr=subprocess.STDOUT,env=env)
    until=time.monotonic()+180
    while time.monotonic()<until:
     text=(out/'ares.log').read_text();assert 'ASSERTION FAILED' not in text and 'mismatch' not in text,text[-2500:]
     if marker in text:break
     assert process.poll() is None,'ares exited before completion';time.sleep(.1)
    else:raise AssertionError('Native Marco pixel regression did not finish')
    ImageGrab.grab(xdisplay=env['DISPLAY']).save(out/'pass.png')
   match=re.search(re.escape(marker)+r' tmem_max=(\d+)',text);assert match and 0<int(match[1])<=2048
   sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
   report=dict(status='PASS',emulator='ares 146',poses=poses,framebuffers=poses*2*palettes,palettes=palettes,ci4_chunks=parts,maximum_texture_upload=int(match[1]),texture_upload_limit=2048,rom_sha256=sha(a.rom),log_sha256=sha(out/'ares.log'),asset_metadata_sha256=sha(ROOT/'build/marco_pose_assets.json'),limits='Observed descriptor resource rendering only. All tested framebuffer CRCs match; full player controller and all-game animation coverage are not established. No demo.')
   (out/'validation.json').write_text(json.dumps(report,indent=2)+'\n');print(report)
  finally:
   if process is not None:
    process.terminate()
    try:process.wait(timeout=8)
    except subprocess.TimeoutExpired:process.kill();process.wait()
   server.terminate();server.wait(timeout=8)
if __name__=='__main__':main()
