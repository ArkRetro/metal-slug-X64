#!/usr/bin/env python3
"""A boot movie's PCM16 WAV64 (tools/pack_cinematic.py, 32 kHz stereo) as
VADPCM, the codec of the game music (tools/pack_game_audio.py): the choice
movies of Tarma, Eri and Fio, whose three PCM tracks (454 KB each) would
push the cartridge past the size ares logs from (docs/PERSONAJES.md). The
samples are the WAV64's own, not resampled again; the result is decoded
back (tools/wav64_vadpcm.py, the reference decoder) and must pass the check
of the game's effects (no 10 ms window with an error over its own level).
Same as pack_cinematic.py --wav-compress 1 on the capture.
Usage: recompress_cinematic_audio.py filesystem/cinematics/confirm-c1.wav64 ..."""
import hashlib,json,struct,subprocess,sys,tempfile,wave
from pathlib import Path
import numpy as np
import wav64_vadpcm as W
ROOT=Path(__file__).resolve().parents[1]
CONV=ROOT.parent/'toolchains/package/opt/libdragon/bin/audioconv64'
for name in sys.argv[1:]:
    path=Path(name);b=path.read_bytes();assert b[:4]==b'WV64'
    version,fmt,channels,bits=struct.unpack_from('>bbbb',b,4);rate,=struct.unpack_from('>I',b,8);frames,=struct.unpack_from('>I',b,12);start,=struct.unpack_from('>I',b,20)
    assert (fmt,channels,bits,rate,start)==(0,2,16,32000,24),(name,'not the PCM16 32 kHz stereo WAV64 of pack_cinematic.py',fmt,channels,bits,rate,start)
    pcm=np.frombuffer(b[start:start+frames*4],dtype='>i2').reshape(-1,2)
    with tempfile.TemporaryDirectory() as d:
        src=Path(d)/(path.stem+'.wav')
        with wave.open(str(src),'wb') as w:w.setnchannels(2);w.setsampwidth(2);w.setframerate(rate);w.writeframes(pcm.astype('<i2').tobytes())
        subprocess.run([str(CONV),'--wav-compress','1','--wav-loop','false','-o',d,str(src)],check=True,stdout=subprocess.DEVNULL)
        out=(Path(d)/(path.stem+'.wav64')).read_bytes()
        (Path(d)/'x.wav64').write_bytes(out);decoded,r=W.decode(Path(d)/'x.wav64')
    assert r==rate and len(decoded)>=frames-16,(r,len(decoded),frames)
    check=W.check(decoded,pcm.astype(float),rate);assert check['ok'],(name,check)
    report=path.with_suffix('.json');j=json.loads(report.read_text()) if report.exists() else {}
    j.update(audio_compression='vadpcm',audio_pcm_sha256=hashlib.sha256(b).hexdigest(),audio_sha256=hashlib.sha256(out).hexdigest(),audio_vadpcm_check=check);report.write_text(json.dumps(j,indent=2)+'\n')
    path.write_bytes(out);print(path.name,len(b),'->',len(out),check)
