#!/usr/bin/env python3
"""Capture named input experiments, not an assumed complete animation catalog."""
import argparse,json,os,subprocess,hashlib,shutil
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
# MAME 0.264+ and Xvfb: the emulator tree when present, else the system ones.
MAME=next((m for m in ('/workspace/emulator/root/usr/games/mame','/usr/games/mame') if os.path.exists(m)),'mame')
XVFB=next((m for m in ('/workspace/emulator/root/usr/bin/Xvfb','/usr/bin/Xvfb') if os.path.exists(m)),'Xvfb')
# Names describe input conditions only. Classify animation from actual evidence.
CASES=[('idle_right',512,()),('crouch_right',64,('Down',)),('crouch_fire_right',96,('Down','fire')),
 ('stand_fire_right',96,('fire',)),('aim_up_right',64,('Up',)),('aim_up_fire_right',96,('Up','fire')),
 ('walk_right',40,('Right',)),('run_fire_right',40,('Right','fire')),('walk_left',40,('Left',)),
 ('run_fire_left',40,('Left','fire')),('stand_fire_left',96,('fire',)),('crouch_left',64,('Down',)),
 ('crouch_fire_left',96,('Down','fire')),('aim_up_fire_left',96,('Up','fire')),
 ('jump_left',96,('jump',)),('jump_fire_left',96,('jump','fire')),('jump_up_fire_left',96,('jump','Up','fire')),
 ('jump_down_fire_left',96,('jump','Down','fire')),('run_jump_right',64,('Right','jump')),
 ('run_jump_fire_right',64,('Right','jump','fire')),('run_jump_left',64,('Left','jump')),
 ('run_jump_fire_left',64,('Left','jump','fire')),('grenade_stand',80,('grenade',)),
 ('grenade_crouch',80,('Down','grenade')),('grenade_jump',96,('jump','grenade')),('idle_long',360,()),
 ('run_cycle_right',96,('Right',)),('run_cycle_left',96,('Left',)),('idle_after_cycles',64,())]
def schedule(cases=CASES):
 events={};rows=[];at=2100
 def event(f,button,value):events.setdefault(f,[]).append(('P1 '+button,value))
 for name,length,buttons in cases:
  assert isinstance(name,str) and name and 16<=length<=1200 and set(buttons)<={'Up','Down','Left','Right','fire','jump','grenade'}
  for b in ('Up','Down','Left','Right','A','B','C'):event(at,b,0)
  for b in buttons:
   if b not in ('fire','jump','grenade'):event(at,b,1)
  if 'fire' in buttons:
   for f in range(at,at+length-1,8):event(f,'A',1);event(f+2,'A',0)
  if 'jump' in buttons:event(at,'B',1);event(at+2,'B',0)
  if 'grenade' in buttons:event(at+(8 if 'jump' in buttons else 0),'C',1);event(at+12,'C',0)
  rows.append(dict(name=name,first=at,last=at+length-1,inputs=buttons));at+=length
 return rows,events

def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--rom-dir',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--command-trace',action='store_true');p.add_argument('--paired-trace',action='store_true');p.add_argument('--end-frame',type=int);p.add_argument('--controller-trace',action='store_true');p.add_argument('--logic-only',action='store_true');p.add_argument('--queue-trace',action='store_true');p.add_argument('--upper-trace',action='store_true');p.add_argument('--body-trace',action='store_true');p.add_argument('--motion-trace',action='store_true');p.add_argument('--ammo-trace',action='store_true');p.add_argument('--stance-trace',action='store_true');p.add_argument('--idle-trace',action='store_true');p.add_argument('--plan',type=Path,help='JSON list of named duration/button experiments');p.add_argument('--dump-routines',help='capture only VRAM/palette/screen while the player routine is in these hex ranges (lo-hi,...), and 4 frames after');p.add_argument('--give-hgun',action='store_true',help='capture only: at the first plan frame write the H (id 0A, rounds kept at 200, +94 = 1) like a pickup, as tools/capture_weapons.lua plan hold');p.add_argument('--give-cgun',action='store_true',help='capture only: at the first plan frame write the C (id 0C, 40 rounds kept, cap 3, +94 = 1) like a pickup, as tools/capture_weapons.lua plan air');p.add_argument('--keep-bombs',action='store_true',help='capture only: bombs kept at 10 (several throws per plan)');a=p.parse_args()
 assert not (a.give_hgun and a.give_cgun)
 out=a.output.resolve();out.mkdir(parents=True,exist_ok=False);(out/'cfg').mkdir();(out/'nvram').mkdir()
 cases=CASES
 if a.plan:
  cases=[(c['name'],c['length'],tuple(c['inputs'])) for c in json.loads(a.plan.read_text())]
  assert cases
 rows,events=schedule(cases);last=rows[-1]['last'];assert last<=12000
 if a.end_frame is not None:
  assert 2104<=a.end_frame<=last;last=a.end_frame
  rows=[dict(c,last=min(c['last'],last)) for c in rows if c['first']<=last]
 script=out/'capture.lua';shutil.copyfile(ROOT/'tools/mame/capture_marco_catalog.lua',script)
 (out/'plan.json').write_text(json.dumps(dict(cases=rows,first=2100,last=last,limits='Input experiments; completeness and animation labels require validation.'),indent=2)+'\n')
 lines=['return {first=2100,last='+str(last)+',events={']
 for f,items in sorted(events.items()):lines.append('['+str(f)+']={'+','.join('{"'+b+'",'+str(v)+'}' for b,v in items)+'},')
 (out/'plan.lua').write_text('\n'.join(lines+['}}'])+'\n')
 env=dict(os.environ,MSX_CAPTURE_DIR=str(out),LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu')
 if a.command_trace:env['MSX_COMMAND_TRACE']='1'
 if a.paired_trace:env['MSX_PAIRED_TRACE']='1'
 if a.controller_trace:env['MSX_CONTROLLER_TRACE']='1'
 if a.logic_only:env['MSX_LOGIC_ONLY']='1'
 if a.queue_trace:env['MSX_QUEUE_TRACE']='1'
 if a.upper_trace:env['MSX_UPPER_TRACE']='1'
 if a.body_trace:env['MSX_BODY_TRACE']='1'
 if a.ammo_trace:env['MSX_AMMO_TRACE']='1'
 if a.give_hgun:env['MSX_GIVE_HGUN']='1'
 if a.give_cgun:env['MSX_GIVE_CGUN']='1'
 if a.keep_bombs:env['MSX_KEEP_BOMBS']='1'
 if a.dump_routines:env['MSX_DUMP_ROUTINES']=a.dump_routines
 assert sum((a.body_trace,a.motion_trace,a.stance_trace,a.idle_trace))<=1,'Block tracers share a debugger latch'
 if a.idle_trace:env['MSX_IDLE_TRACE']='1'
 if a.stance_trace:env['MSX_STANCE_TRACE']='1'
 if a.motion_trace:
  env['MSX_MOTION_TRACE']='1'
 with (out/'xvfb.log').open('w') as xlog:
  server=subprocess.Popen([XVFB,'-displayfd','1','-screen','0','1024x768x24','-ac','-nolisten','tcp'],stdout=subprocess.PIPE,stderr=xlog,text=True,env=env)
  try:
   number=server.stdout.readline().strip();assert number.isdigit();env['DISPLAY']=':'+number
   cmd=[MAME,'mslugx','-rompath',str(a.rom_dir.resolve()),'-bios','euro','-video','soft','-sound','none','-window','-skip_gameinfo','-nothrottle','-debug','-debugger','none','-debuglog','-debugscript',str(ROOT/'tools/mame/start.cmd'),'-autoboot_delay','0','-autoboot_script',str(script),'-cfg_directory',str(out/'cfg'),'-nvram_directory',str(out/'nvram')]
   (out/'run-command.json').write_text(json.dumps(cmd,indent=2)+'\n')
   with (out/'mame.log').open('w') as log:subprocess.run(cmd,cwd=out,env=env,stdout=log,stderr=subprocess.STDOUT,check=True,timeout=int(os.environ.get('MSX_CATALOG_TIMEOUT','480')))
   log=(out/'mame.log').read_text();debug=(out/'debug.log').read_text();assert 'LUA ERROR' not in log and 'Error in command' not in debug and 'MARCO_DRAW' in debug
   assert len(list(out.glob('actor*.bin')))==last-2100+1
   sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
   (out/'capture.json').write_text(json.dumps(dict(status='CAPTURED',frames=last-2100+1,script_sha256=sha(script),plan_sha256=sha(out/'plan.json'),program_fixed_sha256=sha(out/'program-fixed.bin'),program_bank_sha256=sha(out/'program-bank.bin')),indent=2)+'\n')
   print('CAPTURED',last-2100+1,'frames /',len(rows),'input conditions')
  finally:server.terminate();server.wait(timeout=8)
if __name__=='__main__':main()
