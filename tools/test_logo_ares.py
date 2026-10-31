#!/usr/bin/env python3
"""Boot a cartridge in ares 146 and check the 3D NEOGEO boot logo (docs/LOGO_NEOGEO.md).

Checks, from the emulator log: the logo starts with its mesh and jingle, runs to
its end (or is skipped with Start: --skip), reports its frame times, and hands
over to what follows without a crash: the X64 title (MSX TITLE start, in
mission1-internal.z64) or the game (MSX first frame ready, in the game and
reference cartridges). Screenshots are taken at the logo's own markers.

--record also writes a review video: ares' ALSA output goes to a file sink
(its byte count is the emulated clock, since ares may run slower than real
time here), screenshots are timed against that clock, and ffmpeg muxes both at
real speed into an H.264/AAC MP4. Review material only; no hardware claim."""
import argparse,json,os,re,subprocess,threading,time
from pathlib import Path
from PIL import ImageGrab
ROOT=Path(__file__).resolve().parents[1]
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('rom',type=Path);p.add_argument('--output',type=Path,required=True)
p.add_argument('--skip',action='store_true',help='Press Start two seconds into the logo')
p.add_argument('--ares',default='/workspace/emulator/build/rundir/bin/ares')
p.add_argument('--record',type=Path,help='Write a review MP4 here')
p.add_argument('--record-after',type=float,default=6.0,help='Seconds recorded after the logo ends')
p.add_argument('--scale',type=int,default=2,help='Video scale of the 320x240 picture')
a=p.parse_args()
out=a.output.resolve();assert not out.exists() or not any(out.iterdir()),'Use a new output directory'
out.mkdir(parents=True,exist_ok=True)
env=dict(os.environ,XDG_DATA_HOME='/workspace/emulator/data146',XDG_CONFIG_HOME='/workspace/emulator/config146',
         XDG_RUNTIME_DIR='/workspace/emulator/runtime',LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu',
         VK_DRIVER_FILES='/workspace/emulator/root/usr/share/vulkan/icd.d/lvp_icd.json',LIBGL_ALWAYS_SOFTWARE='1')
pcm=out/'ares-audio.raw'
if a.record:
    (out/'asound.conf').write_text('pcm.!default {\n type file\n slave { pcm { type null } }\n file "%s"\n format "raw"\n}\n'%pcm)
    env['ALSA_CONFIG_PATH']=str(out/'asound.conf');driver='ALSA'
else:driver='None'
XD='/usr/bin/xdotool';process=None;log_path=out/'ares.log'
def log():return log_path.read_text(errors='replace')
def wait(pattern,seconds=90):
    deadline=time.monotonic()+seconds
    while time.monotonic()<deadline:
        text=log()
        assert 'ASSERTION FAILED' not in text and 'RSP CRASH' not in text and 'MSX ERROR:' not in text,text[-3000:]
        m=re.search(pattern,text)
        if m:return m
        assert process.poll() is None,'ares exited'
        time.sleep(.03)
    raise AssertionError('Timeout: '+pattern+'\n'+log()[-2000:])
frames=[];recording=threading.Event();stop=threading.Event()
def grabber():
    # Screenshots stamped with the emulated clock (bytes in the ALSA sink).
    k=0
    while not stop.is_set():
        if not recording.is_set():time.sleep(.01);continue
        size=pcm.stat().st_size if pcm.exists() else 0
        im=ImageGrab.grab(xdisplay=env['DISPLAY'])
        path=out/'rec'/f'{k:05d}.png';im.save(path,compress_level=1);frames.append((size,path));k+=1
def picture_box(path):
    """The emulated picture inside ares' window: between the light menu bar and
    the dark status bar, 4:3 and horizontally centred."""
    import numpy as np
    from PIL import Image
    image=Image.open(path);g=np.asarray(image.convert('L')).astype(int)[:,2]
    menu=[y for y in range(len(g)) if g[y]>200];status=[y for y in range(menu[-1],len(g)) if 20<=g[y]<=48]
    top,bottom=menu[-1]+2,status[0];h=bottom-top;w=h*4//3;x=(image.width-w)//2
    return (x,top,x+w,bottom)
xvfb=subprocess.Popen(['/usr/bin/Xvfb','-displayfd','1','-screen','0','1024x768x24','-ac','-nolisten','tcp'],stdout=subprocess.PIPE,stderr=subprocess.DEVNULL,text=True)
try:
    env['DISPLAY']=':'+xvfb.stdout.readline().strip()
    cmd=[a.ares,'--setting','Video/Driver=OpenGL 3.2','--setting',f'Audio/Driver={driver}','--setting','General/HomebrewMode=true',
         '--system','Nintendo 64','--no-file-prompt',str(a.rom.resolve())]
    with log_path.open('w') as lf:
        process=subprocess.Popen(cmd,stdout=lf,stderr=subprocess.STDOUT,env=env,cwd=ROOT)
        if a.record:(out/'rec').mkdir();threading.Thread(target=grabber,daemon=True).start();recording.set()
        start=wait(r'MSX LOGO start caps=(\d+) edges=(\d+) cap_tris=(\d+) size=([\d.]+)x([\d.]+)x([\d.]+) audio=(\d)')
        assert int(start[7])==1,'Jingle missing'
        shots=[]
        if a.skip:
            windows=subprocess.check_output([XD,'search','--onlyvisible','--class','ares'],env=env,text=True).split()
            subprocess.run([XD,'windowfocus','--sync',windows[-1]],env=env,check=True)
            wait(r'MSX LOGO shot frame=150');subprocess.run([XD,'keydown','Return'],env=env,check=True)
            time.sleep(.3);subprocess.run([XD,'keyup','Return'],env=env,check=True)
        else:
            for n in (60,150,230,330):
                wait(rf'MSX LOGO shot frame={n} ');time.sleep(.05)
                ImageGrab.grab(xdisplay=env['DISPLAY']).save(out/f'logo-{n:03d}.png');shots.append(f'logo-{n:03d}.png')
        done=wait(r'MSX LOGO complete skipped=(\d)');assert int(done[1])==int(a.skip),'Unexpected skip state'
        timing=re.search(r'MSX LOGO timing frames=(\d+) seconds=([\d.]+) frame_max_us=(\d+) cpu_max_us=(\d+) cpu_mean_us=(\d+) late=(\d+) max_triangles=(\d+)',log())
        assert timing
        nm=wait(r'MSX TITLE start|MSX first frame ready',60);time.sleep(1.0)
        ImageGrab.grab(xdisplay=env['DISPLAY']).save(out/'after-logo.png')
        if a.record:time.sleep(a.record_after)
        stop.set();time.sleep(.2)
        assert process.poll() is None,'ares exited'
    from PIL import Image
    box=picture_box(out/'after-logo.png')
    for name in shots+['after-logo.png']:Image.open(out/name).crop(box).resize((640,480),Image.NEAREST).save(out/('picture-'+name))
    result=dict(status='PASS',emulator='ares 146',rom=str(a.rom),cap_points=int(start[1]),side_edges=int(start[2]),cap_triangles=int(start[3]),
                jingle=True,skipped=a.skip,next=nm[0],frames=int(timing[1]),seconds=float(timing[2]),
                frame_max_us=int(timing[3]),cpu_max_us=int(timing[4]),cpu_mean_us=int(timing[5]),late_frames=int(timing[6]),max_triangles_drawn=int(timing[7]),
                screenshots=shots+['after-logo.png'],
                limits='Emulator frame times (emulated clock); MiSTer/hardware not measured here.')
    if a.record and frames:
        import numpy as np
        from PIL import Image
        raw=np.frombuffer(pcm.read_bytes(),dtype=np.int16);rate=48000
        # ares' ALSA stream: 48 kHz stereo s16 (checked against the run's emulated length).
        stereo=raw[:len(raw)//2*2].reshape(-1,2)
        box=picture_box(out/'after-logo.png')
        fps=30;dur=len(stereo)/rate;W,H=320*a.scale,240*a.scale
        tmp=out/'video-frames';tmp.mkdir()
        times=[s/(4*rate) for s,_ in frames];j=0
        for i in range(int(dur*fps)):
            t=i/fps
            while j+1<len(frames) and times[j+1]<=t:j+=1
            Image.open(frames[j][1]).crop(box).resize((W,H),Image.NEAREST).save(tmp/f'{i:05d}.png')
        import wave
        with wave.open(str(out/'audio.wav'),'wb') as w:
            w.setnchannels(2);w.setsampwidth(2);w.setframerate(rate);w.writeframes(stereo.tobytes())
        a.record.parent.mkdir(parents=True,exist_ok=True)
        subprocess.run(['ffmpeg','-y','-loglevel','error','-framerate',str(fps),'-i',str(tmp/'%05d.png'),'-i',str(out/'audio.wav'),
                        '-c:v','libx264','-pix_fmt','yuv420p','-crf','20','-preset','slow','-c:a','aac','-b:a','160k','-shortest',str(a.record)],check=True)
        result['video']=dict(path=str(a.record),seconds=round(dur,2),fps=fps,scale=a.scale,
                             clock='ALSA file-sink bytes (emulated time)',screenshots=len(frames))
    (out/'validation.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=1))
finally:
    stop.set()
    if process is not None:
        process.terminate()
        try:process.wait(timeout=8)
        except subprocess.TimeoutExpired:process.kill();process.wait()
    xvfb.terminate();xvfb.wait(timeout=8)
