#!/usr/bin/env python3
"""SOLDIER SELECT music after the select movie's audio (B44).

The arcade's select screen plays the music command 0x0030 (MAME fight1:
0000 then 0030 from 0208A2 at frame 3453) and keeps playing it until the
choice (06 at 3804, before the mission music). The select movie's audio
(filesystem/cinematics/select.wav, frames 3460..3619) holds only 2.70 s of
it, so the port's hold on the last frame was silent.

Inputs: the isolated render of 0x0030 (tools/render_sound_commands.py
--map data/select_music_map.json --labels music_select: 7200 frames, still
playing at the end of the window) and select.wav. Steps, all measured:
* alignment: the last second of select.wav against the render (waveform
  cross-correlation): select.wav ends at render sample `end_in_render`;
* loop: pack_game_audio.find_loop (period, start of the repeated part) and
  splice (seam correlation >= 0.9), searched only after that end;
* head = select.wav + render[end_in_render, t0) with a 10 ms crossfade at
  the junction (the render overlaps it), body = render[t0, t0+period+d).
Output (32 kHz stereo, the cinematic voice's rate; ROM-derived, private):
filesystem/cinematics/select-music-head.wav64 (uncompressed, like the
movie's audio), select-music-loop.wav64 (VADPCM, looped whole) and
select-music.json. src/main.c plays head then body (msx_audio_open_loop)
on the select hold, instead of select.wav64, when both files exist."""
import argparse,hashlib,json,os,subprocess,sys
from pathlib import Path
import numpy as np
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'tools'))
import pack_game_audio as P
RATE=32000;SRC=P.SRC
def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--render',type=Path,required=True,help='music_select.wav from render_sound_commands.py')
    p.add_argument('--select',type=Path,default=ROOT/'filesystem/cinematics/select.wav')
    p.add_argument('--output',type=Path,default=ROOT/'filesystem/cinematics')
    p.add_argument('--audioconv',default=None);a=p.parse_args()
    conv=a.audioconv or os.path.join(os.environ.get('N64_INST','/workspace/toolchains/package/opt/libdragon'),'bin/audioconv64')
    render=P.read(a.render);select=P.read(a.select)
    # Alignment: last second of the movie's audio inside the render.
    seg=select[-SRC:].mean(1);mono=render.mean(1);search=mono[:SRC*10]
    n=1<<int(np.ceil(np.log2(len(search)+len(seg))))
    c=np.fft.irfft(np.fft.rfft(search,n)*np.conj(np.fft.rfft(seg,n)),n)[:len(search)-len(seg)]
    k=int(np.argmax(c));align=float(c[k]/np.sqrt((seg**2).sum()*(search[k:k+len(seg)]**2).sum()))
    assert align>=0.6,('select.wav not found in the render',align)
    end=k+len(seg)   # render sample where select.wav ends
    loop=P.find_loop(render,5.0);period=int(round(loop['period_s']*SRC))
    start=max(int(round(loop['start_s']*SRC)),end-SRC)   # splice searches from start + 1 s
    t0,stop,seam=P.splice(render,start,period)
    assert t0>=end and seam>=0.9,(t0,end,seam)
    fade=SRC//100;w=np.linspace(0,1,fade)[:,None]
    head=np.concatenate([select[:-fade],select[-fade:]*(1-w)+render[end-fade:end]*w,render[end:t0]])
    head_out=P.resample(head,RATE)
    body_len=int((stop-t0)*RATE/SRC)//16*16   # VADPCM loop length: multiple of 16
    body_out=P.resample(render[t0:stop],RATE,body_len)
    staging=a.output/'select-music-wav';staging.mkdir(parents=True,exist_ok=True)
    P.write(staging/'select-music-head.wav',head_out,RATE);P.write(staging/'select-music-loop.wav',body_out,RATE)
    subprocess.run([conv,'--wav-resample',str(RATE),'--wav-compress','0','--wav-loop','false','-o',str(a.output),str(staging/'select-music-head.wav')],check=True,stdout=subprocess.DEVNULL)
    subprocess.run([conv,'--wav-resample',str(RATE),'--wav-compress','1','--wav-loop','true','-o',str(a.output),str(staging/'select-music-loop.wav')],check=True,stdout=subprocess.DEVNULL)
    # B43: the VADPCM encoder can fail on a signal: decode the loop and
    # compare it with its source (tools/wav64_vadpcm.py).
    import wav64_vadpcm as W
    check=W.check(W.decode(a.output/'select-music-loop.wav64')[0],np.clip(np.round(body_out),-32768,32767),RATE)
    assert check['ok'],check
    import shutil;shutil.rmtree(staging)   # 6 MB of intermediate PCM
    files={}
    for name in ('select-music-head.wav64','select-music-loop.wav64'):
        b=(a.output/name).read_bytes();files[name]=dict(bytes=len(b),sha256=hashlib.sha256(b).hexdigest())
    report=dict(word='0030',source=dict(render=a.render.name,render_sha256=hashlib.sha256(a.render.read_bytes()).hexdigest(),
                select_sha256=hashlib.sha256(a.select.read_bytes()).hexdigest()),
        alignment=dict(end_in_render=end,end_s=round(end/SRC,4),correlation=round(align,4)),loop=loop,
        splice_source_sample=t0,end_source_sample=stop,seam_correlation=round(seam,4),crossfade_samples=fade,vadpcm_check=check,
        head_s=round(len(head_out)/RATE,3),loop_s=round(body_len/RATE,3),rate=RATE,files=files)
    (a.output/'select-music.json').write_text(json.dumps(report,indent=1)+'\n')
    print(json.dumps({k:report[k] for k in ('alignment','splice_source_sample','seam_correlation','head_s','loop_s','files')}))
if __name__=='__main__':main()
