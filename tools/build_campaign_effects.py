#!/usr/bin/env python3
"""Pack verified isolated original sound-board effects for native playback.

The capture contains direct verified sound-token byte pairs after the original
Z80 driver restart command03. A matching no-token run must be digital silence.
No subtraction, filtering, normalization or voice removal is performed.
"""
import argparse
import hashlib
import json
import re
import struct
import subprocess
import wave
import zipfile
from pathlib import Path

import numpy as np
from import_rom import validate

ROOT = Path(__file__).resolve().parents[1]


def read(path):
    with wave.open(str(path)) as f:
        assert (f.getnchannels(),f.getsampwidth(),f.getframerate()) == (2,2,48000)
        return np.frombuffer(f.readframes(f.getnframes()),dtype='<i2').reshape(-1,2)


def build(args):
    validate(args.rom)
    with zipfile.ZipFile(args.rom) as archive:
        audio_roms = {name:hashlib.sha256(archive.read(name)).hexdigest()
            for name in archive.namelist() if name.startswith(('250-m','250-v'))}
        raw = archive.read('250-p1.p1')
        fixed = bytearray(raw)
        fixed[::2],fixed[1::2] = raw[1::2],raw[::2]
    for site,token in ((0xc41d8,0x10fb),(0xc41e4,0x10f9),(0xc41f6,0x1069)):
        assert fixed[site:site+10] == struct.pack('>HHHI',0x303c,token,0x4eb9,0x120f8)
    pcm = read(args.capture/'event/reference.wav')
    baseline = read(args.capture/'baseline/reference.wav')
    assert pcm.shape == baseline.shape and not np.any(baseline), 'Baseline must be exactly digital silence'
    log = (args.capture/'event/debug.log').read_text()
    assert 'Error in command' not in log
    injections = [(int(f),int(v,16),label) for f,v,label in re.findall(
        r'EFFECT_INJECT frame=(\d+) value=([0-9A-F]+) label=(\w+)',log)]
    tokens = [(injections[i][1]<<8)|injections[i+1][1] for i in range(len(injections)-1)
        if injections[i][2]=='token_high' and injections[i+1][2]=='token_low']
    assert tokens in ([0x10f7,0x10fc,0x1146,0x10f7],
        [0x10f7,0x10fc,0x1146,0x10f7,0x10fb,0x10f9,0x1069])
    # Whole silence intervals belong to the untouched captured stream. Include
    # 128 preceding / 256 following zero frames with each effect's full tail.
    nz = np.flatnonzero(np.any(pcm,axis=1))
    cuts = np.r_[0,np.flatnonzero(np.diff(nz)>24000)+1,len(nz)]
    spans = [(int(nz[lo]),int(nz[hi-1])+1) for lo,hi in zip(cuts[:-1],cuts[1:])]
    assert len(spans) == len(tokens)
    args.wav_output.mkdir(parents=True,exist_ok=True)
    args.romfs_output.mkdir(parents=True,exist_ok=True)
    outputs = []
    names = {0x10f7:'pistol',0x10fc:'body-10fc',0x1146:'body-1146',
        0x10fb:'melee-10fb',0x10f9:'melee-10f9',0x1069:'melee-1069'}
    seen = set()
    for token,(start,end) in zip(tokens,spans):
        if token in seen: continue
        seen.add(token)
        name = names[token]
        assert not np.any(pcm[start-128:start]) and not np.any(pcm[end:end+256])
        source = pcm[start-128:end+256]
        path = args.wav_output/f'{name}.wav'
        with wave.open(str(path),'wb') as f:
            f.setnchannels(2);f.setsampwidth(2);f.setframerate(48000);f.writeframes(source.tobytes())
        subprocess.run([str(args.audioconv),'--wav-resample','32000','--wav-compress','0',
            '--wav-loop','false','-o',str(args.romfs_output),str(path)],check=True,capture_output=True,text=True)
        native = args.romfs_output/f'{name}.wav64'
        header = native.read_bytes()[:32]
        assert header[:6] == b'WV64\x02\x00' and header[6:8] == bytes((2,16))
        assert struct.unpack_from('>I',header,8)[0] == 32000
        outputs.append(dict(token=f'0x{token:04X}',path='rom:/effects/'+native.name,
            source_pcm_frames=[start-128,end+256],audible_frames=end-start,
            source_rate=48000,native_rate=32000,channels=2,
            peak=int(np.max(np.abs(source.astype(np.int32)))),
            source_wav_sha256=hashlib.sha256(path.read_bytes()).hexdigest(),
            wav64_sha256=hashlib.sha256(native.read_bytes()).hexdigest(),native_bytes=native.stat().st_size))
    # The repeated pistol is an independent driver invocation. Its YM clock
    # phase changes the resampling fraction, so do not claim byte identity.
    first = pcm[spans[0][0]:spans[0][1]]
    second = pcm[spans[3][0]:spans[3][1]]
    n = min(len(first),len(second))
    correlation = float(np.corrcoef(first[:n].ravel(),second[:n].ravel())[0,1])
    assert correlation > .99 and abs(len(first)-len(second)) <= 2
    assert np.max(abs(first.astype(np.int32))) == np.max(abs(second.astype(np.int32)))
    files = [args.capture/'event/reference.wav',args.capture/'baseline/reference.wav',
        args.capture/'event/debug.log',args.capture/'baseline/debug.log',args.capture/'capture.lua',
        ROOT/'tools/build_campaign_effects.py',args.audioconv]
    report = dict(source='MAME 0.276 / supplied mslugx euro sound-board driver, controlled original token injection',
        isolation=f'Matching no-token baseline is exactly zero across all stereo PCM frames; event PCM contains only {len(tokens)} separated requested effect responses',
        control='After frame200 original01222A queue sends are skipped, executing the original read-index increment at012232. Original Z80 restart byte03 is sent via actual maincpu soundlatch320000. Only verified token high/low byte pairs are added; no YM register/voice patches.',
        source_token_paths={'0x10F7':'01B782 -> 0120F8 -> 01209C -> 01222A; latch bytes10,F7',
            '0x10FC':'Recovered body animation requests0120F8 token10FC; latch bytes10,FC',
            '0x1146':'Recovered body animation requests0120F8 token1146; latch bytes11,46',
            '0x10FB':'0C41CE tests class5 target type2; 0C41D8 ->0120F8 token10FB; latch bytes10,FB',
            '0x10F9':'0C41CE class5 target type!=2; 0C41E4 ->0120F8 token10F9; latch bytes10,F9',
            '0x1069':'0C41EE tests target class4; 0C41F6 ->0120F8 token1069; latch bytes10,69'},
        melee_source_immediates_match_supplied_p1=True,
        baseline_pcm_frames=len(baseline),baseline_nonzero_samples=0,
        repeated_pistol=dict(first_audible_frames=len(first),second_audible_frames=len(second),
            waveform_correlation=correlation,interpretation='Same peak and support, correlation>.99; raw samples differ with YM/output clock phase'),
        processing='Untouched stereo16bit PCM windows including original digital-zero margins; libdragon resamples48000->32000; WAV64 compression0, loopingfalse. No music separation/subtraction, normalization, filtering or synthesized replacement.',
        outputs=outputs,audio_rom_sha256=audio_roms,
        validated_rom_sha256=hashlib.sha256(args.rom.read_bytes()).hexdigest(),
        provenance={str(path):hashlib.sha256(path.read_bytes()).hexdigest() for path in files})
    args.report.write_text(json.dumps(report,indent=2)+'\n')
    print('Packed isolated original effects: '+', '.join(o['path'] for o in outputs))


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('capture',type=Path)
    p.add_argument('--rom',type=Path,required=True)
    p.add_argument('--audioconv',type=Path,required=True)
    p.add_argument('--wav-output',type=Path,default=ROOT/'filesystem/audio-effects')
    p.add_argument('--romfs-output',type=Path,default=ROOT/'romfs/effects')
    p.add_argument('--report',type=Path,default=ROOT/'docs/campaign-effects-validation.json')
    build(p.parse_args())
