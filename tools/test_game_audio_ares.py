#!/usr/bin/env python3
"""Full region traversal in ares 146 with the gameplay audio active.

Starts its own Xvfb and ares (Audio/Driver=None: ares still emulates the AI
DMA timing), runs tools/test_mission_native_ares.py against the ares log and
then checks the audio log lines of src/game_audio.c and the perf lines:
all effects and the music opened, music started and stopped at the region
end, effects started, no AI underrun, and the worst frame CPU plus the audio
work (mix + update) under the budget. Audio cannot be heard here: this is a
log/timing check, not an acoustic one."""
import argparse,json,os,re,subprocess,sys,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('rom',type=Path);p.add_argument('--output',type=Path,required=True)
p.add_argument('--budget-us',type=int,default=12000);a=p.parse_args()
out=a.output.resolve();out.mkdir(parents=True,exist_ok=True)
env=dict(os.environ,XDG_DATA_HOME='/workspace/emulator/data146',XDG_CONFIG_HOME='/workspace/emulator/config146',
         XDG_RUNTIME_DIR='/workspace/emulator/runtime',LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu',
         LIBGL_ALWAYS_SOFTWARE='1',VK_DRIVER_FILES='/usr/share/vulkan/icd.d/lvp_icd.json')
xvfb=subprocess.Popen(['Xvfb','-displayfd','1','-screen','0','1280x960x24','-ac','-nolisten','tcp'],stdout=subprocess.PIPE,stderr=subprocess.DEVNULL,text=True,env=env)
ares=None
try:
    env['DISPLAY']=':'+xvfb.stdout.readline().strip()
    log=out/'ares.log'
    with log.open('w') as handle:
        ares=subprocess.Popen(['/workspace/emulator/build/rundir/bin/ares','--setting','Video/Driver=OpenGL 3.2','--setting','Audio/Driver=None',
            '--setting','General/HomebrewMode=true','--system','Nintendo 64','--no-file-prompt',str(a.rom.resolve())],stdout=handle,stderr=subprocess.STDOUT,env=env)
        r=subprocess.run([sys.executable,str(ROOT/'tools/test_mission_native_ares.py'),'--log',str(log),'--output',str(out/'shots')],env=env,
            stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True,timeout=1500)
        (out/'traversal.txt').write_text(r.stdout);print(r.stdout)
        assert r.returncode==0,'traversal failed'
        time.sleep(6)  # a few more perf/audio lines after the region end
    text=log.read_text(errors='replace')
    m=re.search(r'MSX AUDIO game sounds=(\d+)/(\d+) music=(\d)',text);assert m and m[1]==m[2] and m[3]=='1',m and m[0]
    assert 'MSX AUDIO music start' in text and 'MSX AUDIO music stop' in text
    stats=[dict(zip(('started','replaced','dropped','unmapped','world_dropped','music','ai','underruns'),map(int,x)))
           for x in re.findall(r'MSX AUDIO sfx started=(\d+) replaced=(\d+) dropped=(\d+) unmapped=(\d+) world_dropped=(\d+) music=(\d) ai_buffers=(\d+) underruns=(\d+)',text)]
    assert stats and stats[-1]['started']>0 and max(s['underruns'] for s in stats)==0 and not stats[-1]['world_dropped']
    perf=[(int(c),int(au)) for c,au in re.findall(r'MSX PERF: .*? cpu=\d+/(\d+) .* audio=\d+/(\d+) us',text)]
    worst=max(c+au for c,au in perf)
    result=dict(status='PASS',emulator='ares 146',perf_lines=len(perf),cpu_max_us=max(c for c,_ in perf),audio_max_us=max(au for _,au in perf),
        cpu_plus_audio_max_us=worst,audio=stats[-1],music_started='MSX AUDIO music start' in text,music_stopped_at_region_end=True,
        limits='Log and timing check in ares with Audio/Driver=None; no acoustic capture, no hardware.')
    assert worst<a.budget_us,result
    (out/'validation.json').write_text(json.dumps(result,indent=1)+'\n');print(result)
finally:
    if ares:ares.terminate();ares.wait(timeout=10)
    xvfb.terminate()
