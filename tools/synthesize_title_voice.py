#!/usr/bin/env python3
"""Generate an emphatic two-part male announcer tag with Kokoro ONNX."""
import argparse, wave
from pathlib import Path
import numpy as np
import onnxruntime as ort
from kokoro_onnx import Kokoro

ROOT=Path(__file__).resolve().parents[1]
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('--model-dir',type=Path,default=ROOT.parent/'toolchains/kokoro')
a=p.parse_args()
options=ort.SessionOptions();options.intra_op_num_threads=2;options.inter_op_num_threads=1
session=ort.InferenceSession(str(a.model_dir/'kokoro-v1.0.onnx'),sess_options=options,providers=['CPUExecutionProvider'])
k=Kokoro.from_session(session,str(a.model_dir/'voices-v1.0.bin'))
voice=.65*k.get_voice_style('am_fenrir')+.35*k.get_voice_style('am_onyx')
out=ROOT/'build/title-audio';out.mkdir(parents=True,exist_ok=True)
for name,text,speed in [('sixty','Sixty!',1.0),('four','FOUR!',.90)]:
    samples,rate=k.create(text,voice,speed=speed,lang='en-us',sentence_pause=0,clause_pause=0)
    samples=np.clip(samples,-1,1)
    path=out/f'{name}-epic-raw.wav'
    with wave.open(str(path),'wb') as w:
        w.setnchannels(1);w.setsampwidth(2);w.setframerate(rate);w.writeframes((samples*32767).astype('<i2').tobytes())
    print(name,rate,len(samples)/rate,flush=True)
