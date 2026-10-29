#!/usr/bin/env python3
"""Master an actually instruction-directed shout without altering its delivery."""
import argparse,hashlib,json,math,subprocess,wave
from pathlib import Path
import numpy as np
from scipy.signal import butter,fftconvolve,sosfilt

ROOT=Path(__file__).resolve().parents[1]
p=argparse.ArgumentParser(description=__doc__);p.add_argument('raw',type=Path);a=p.parse_args()
work=ROOT/'build/title-audio';work.mkdir(parents=True,exist_ok=True)
source=json.loads((work/'sixty-four-shout-source.json').read_text())
assert source['raw_sha256']==hashlib.sha256(a.raw.read_bytes()).hexdigest()
# Preserve natural shouted pitch, attack, coarticulation and word timing.
filters=('silenceremove=start_periods=1:start_threshold=-46dB:start_silence=0.025,'
    'areverse,silenceremove=start_periods=1:start_threshold=-46dB:start_silence=0.035,areverse,'
    'highpass=f=65,lowpass=f=9500,equalizer=f=2500:t=q:w=0.9:g=1,'
    'acompressor=threshold=0.10:ratio=2:attack=12:release=90:makeup=1.1')
clean=work/'sixty-four-shout-dry.wav'
subprocess.run(['ffmpeg','-nostdin','-hide_banner','-loglevel','error','-y','-i',str(a.raw),
    '-af',filters,'-ar','48000','-ac','1','-c:a','pcm_s16le',str(clean)],check=True)
with wave.open(str(clean)) as w:dry=np.frombuffer(w.readframes(w.getnframes()),dtype='<i2').astype(float)
rate=48000
rng=np.random.default_rng(64);t=np.arange(round(1.05*rate))/rate
impulse=rng.standard_normal(len(t))*np.exp(-np.log(1000)*t/1.20);impulse[t<.045]=0
impulse=sosfilt(butter(2,[140,4800],btype='bandpass',fs=rate,output='sos'),impulse)
impulse/=np.sqrt(np.sum(impulse**2))
for seconds,gain in ((.053,.55),(.089,.43),(.137,.32),(.203,.20)):
    impulse[round(seconds*rate)]+=gain
wet=fftconvolve(dry,impulse)
wet*=np.sqrt(np.mean(dry**2))/max(1e-9,np.sqrt(np.mean(wet[:len(dry)]**2)))
front=np.pad(dry,(0,len(wet)-len(dry)))
envelope=sosfilt(butter(1,12,fs=rate,output='sos'),abs(front))
envelope/=max(1,np.max(envelope))
# Keep the shouted attack in front; the hall opens up between words and at the end.
result=front+.55*(1-.45*envelope)*wet
result*=15500/max(abs(result));fade=round(.025*rate);result[-fade:]*=np.linspace(1,0,fade)
duration=len(result)/rate
hold=max(60,math.ceil((8.60+duration+.30)*59.185606)-601)
assert hold<=300,'Unexpectedly long utterance: regenerate rather than cutting the shout'
out=ROOT/'art/audio/sixty-four.wav'
with wave.open(str(out),'wb') as target:
    target.setnchannels(1);target.setsampwidth(2);target.setframerate(rate);target.writeframes(result.astype('<i2').tobytes())
profile=dict(voice='Qwen3-TTS 1.7B CustomVoice / Ryan, directed male shout',synthetic=True,
    source_model='https://huggingface.co/Qwen/Qwen3-TTS-12Hz-1.7B-CustomVoice',
    direction=source['instruction'],synthesis_seed=source['seed'],natural_delivery=True,
    pitch_shift=False,time_stretch=False,word_splicing=False,source_raw_sha256=source['raw_sha256'],
    peak_pcm16=15500,title_hold_frames=hold,
    reverb=dict(type='large diffuse hall, ducked behind the shouted attack',t60_seconds=1.20,wet_rms_ratio=.55,tail_seconds=1.05),
    processing='Instruction-generated full-voice yell; natural pitch/timing; light mastering and hall reverb',
    dry_duration_seconds=len(dry)/rate,duration_seconds=duration,asset_sha256=hashlib.sha256(out.read_bytes()).hexdigest())
(ROOT/'art/audio/sixty-four.json').write_text(json.dumps(profile,indent=2)+'\n');print(json.dumps(profile,indent=2))
