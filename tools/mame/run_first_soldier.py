#!/usr/bin/env python3
"""Run the reproducible first-soldier debugger experiment in fresh directories."""
import argparse,json,os,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('--rom-dir',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
p.add_argument('--mame',default='/workspace/emulator/root/usr/games/mame')
p.add_argument('--xvfb',default='/workspace/emulator/root/usr/bin/Xvfb')
p.add_argument('--library-path',default='/workspace/emulator/root/usr/lib/x86_64-linux-gnu')
a=p.parse_args();out=a.output.resolve()
assert not out.exists() or not any(out.iterdir()),'Use a new empty output directory; do not overwrite evidence'
out.mkdir(parents=True,exist_ok=True);(out/'cfg').mkdir();(out/'nvram').mkdir()
env=dict(os.environ,MSX_CAPTURE_DIR=str(out))
if a.library_path:env['LD_LIBRARY_PATH']=a.library_path
with (out/'xvfb.log').open('w') as xlog:
    server=subprocess.Popen([a.xvfb,'-displayfd','1','-screen','0','1024x768x24','-ac','-nolisten','tcp'],stdout=subprocess.PIPE,stderr=xlog,text=True,env=env)
    try:
        number=server.stdout.readline().strip();assert number.isdigit(),'Xvfb failed'
        env['DISPLAY']=':'+number
        cmd=[a.mame,'mslugx','-rompath',str(a.rom_dir.resolve()),'-bios','euro','-video','soft','-sound','none','-window','-skip_gameinfo','-nothrottle','-debug','-debugger','none','-debuglog','-debugscript',str(ROOT/'tools/mame/start.cmd'),'-autoboot_delay','0','-autoboot_script',str(ROOT/'tools/mame/trace_first_soldier.lua'),'-cfg_directory',str(out/'cfg'),'-nvram_directory',str(out/'nvram')]
        (out/'run-command.json').write_text(json.dumps(cmd,indent=2)+'\n')
        with (out/'mame.log').open('w') as log:result=subprocess.run(cmd,env=env,cwd=out,stdout=log,stderr=subprocess.STDOUT,timeout=300)
        assert result.returncode==0,('MAME failed',result.returncode)
        text=(out/'debug.log').read_text()
        assert 'Error in command' not in text and 'CONSTRUCTOR_WRITE' in text and (out/'instructions.log').is_file()
        print('MAME debugger capture complete:',out)
    finally:server.terminate();server.wait(timeout=8)
