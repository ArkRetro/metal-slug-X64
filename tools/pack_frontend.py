#!/usr/bin/env python3
"""Recover tutorial and Marco-only selection movies from original ROM graphics.

Every source frame is reconstructed from measured VRAM/raster/palette state and
compared with an independent MAME screenshot. Screenshots never become assets.
Only Marco was confirmed in this source run; other character control is pending.
"""
import argparse
import multiprocessing
from concurrent.futures import ProcessPoolExecutor
import hashlib
import json
import struct
import subprocess
import wave
import zipfile
import zlib
from pathlib import Path

import numpy as np
from PIL import Image
from pack_cinematic import delta, nanos
from render_intro_reference import Renderer, raster_states, raster_records, bank_palette_records

ROOT = Path(__file__).resolve().parents[1]
SEGMENTS = (("tutorial", 780, 3459), ("select", 3460, 3619),
            ("select-confirm", 3620, 3829))


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()



def reconstruct(frame):
    global _previous_signature, _previous_rgb
    meta = _frames[frame]
    assert meta['fixed_source'] == 1
    # Full pre/final state and semantic raster writes form the reuse key.
    # Excluding capture time/frame affects no renderer input.
    signature = hashlib.sha256()
    for n in (frame-1, frame):
        for source in ('vram', 'palette-banks'):
            signature.update((_capture/f'{source}{n}.bin').read_bytes())
        signature.update(bytes((_frames[n]['palette_bank']//4096,
                                _frames[n]['screen_shadow'])))
    signature.update(bytes((0 if meta['animation_disabled'] else meta['animation_counter'] & 7,
                            meta['animation_disabled'])))
    for source in (_records.get(frame-1, []), _palette_records.get(frame-1, [])):
        signature.update(json.dumps([{k:v for k,v in r.items() if k not in ('frame','time')}
                                     for r in source], sort_keys=True).encode())
    signature = signature.digest()
    if signature != _previous_signature:
        vram = struct.unpack('>34304H', (_capture/f'vram{frame}.bin').read_bytes())
        palette = struct.unpack('>4096H', (_capture/f'palette{frame}.bin').read_bytes())
        states, _, colors, _ = raster_states(_capture, frame, True)
        rgb, _ = _renderer.render(vram, palette, meta['animation_counter'],
                                 states, meta['animation_disabled'], colors)
        _previous_signature, _previous_rgb = signature, rgb
    else:
        rgb = _previous_rgb
    reference = np.asarray(Image.open(_capture/f'game{frame}.png').convert('RGB'))
    assert reference.shape == rgb.shape
    difference = np.max(abs(rgb.astype(np.int16)-reference.astype(np.int16)), axis=2)
    assert not (difference[:,8:312] > 1).any(), ('Unexplained frontend difference', frame, int((difference[:,8:312] > 1).sum()))
    active = rgb[:,8:312].astype(np.uint16)
    pixels = (((active[:,:,0]>>3)<<11)|((active[:,:,1]>>3)<<6)|((active[:,:,2]>>3)<<1)|1).reshape(-1)
    return pixels

def build(capture, bios, output, workers=3):
    output.mkdir(parents=True, exist_ok=True)
    frames = {r['frame']: r for line in (capture/'frames.jsonl').read_text().splitlines()
              if (r := json.loads(line))}
    with zipfile.ZipFile(bios) as archive:
        zoom = archive.read('000-lo.lo')
    renderer = Renderer((ROOT/'filesystem/sprites.bin').read_bytes(),
                        (ROOT/'filesystem/fixed.bin').read_bytes(), zoom)
    records = raster_records(capture)
    palette_records = bank_palette_records(capture)
    segments = []
    global _capture, _frames, _renderer, _records, _palette_records
    global _previous_signature, _previous_rgb
    _capture, _frames, _renderer = capture, frames, renderer
    _records, _palette_records = records, palette_records
    _previous_signature = _previous_rgb = None
    with ProcessPoolExecutor(max_workers=workers, mp_context=multiprocessing.get_context('fork')) as pool:
        for name, first, last in SEGMENTS:
            previous = np.zeros(304*224, dtype=np.uint16)
            packets, checks = [], []
            for frame, pixels in zip(range(first,last+1),pool.map(reconstruct,range(first,last+1),chunksize=24)):
                encoded = delta(pixels, previous)
                packets.append(struct.pack('>I', zlib.crc32(encoded))+encoded)
                checks.append(zlib.crc32(pixels.astype('>u2').tobytes()))
                previous = pixels
                if (frame-first) % 120 == 0:
                    print(name, 'matched original frame', frame, flush=True)
            count = len(packets)
            offset = 32+4*(count+1)
            offsets = [offset]
            for packet in packets:
                offset += len(packet)
                offsets.append(offset)
            movie = struct.pack('>4s7I', b'MSXC',1,304,224,count,max(map(len,packets)),59185606,1000000)
            movie += struct.pack('>'+str(count+1)+'I', *offsets)+b''.join(packets)
            (output/f'{name}.msxc').write_bytes(movie)
            with wave.open(str(capture/'reference.wav')) as source:
                assert source.getframerate()==48000 and source.getnchannels()==2 and source.getsampwidth()==2
                start = nanos(frames[first]['time'])*48000//10**9
                end = nanos(frames[last+1]['time'])*48000//10**9
                assert end <= source.getnframes()
                source.setpos(start)
                pcm = source.readframes(end-start)
            with wave.open(str(output/f'{name}.wav'), 'wb') as target:
                target.setnchannels(2);target.setsampwidth(2);target.setframerate(48000);target.writeframes(pcm)
            subprocess.run([str(ROOT/'../toolchains/package/opt/libdragon/bin/audioconv64'),
                            '--wav-resample','32000','--wav-compress','1','--wav-loop','false',
                            '-o',str(output),str(output/f'{name}.wav')], check=True)
            segments.append(dict(name=name,first_frame=first,last_frame=last,frames=count,
                                 frame_crc32=checks,movie_bytes=len(movie),maximum_packet=max(map(len,packets)),
                                 movie_sha256=sha(output/f'{name}.msxc'),audio_sha256=sha(output/f'{name}.wav64'),
                                 audio_first_sample=start,audio_last_sample=end))
    report = dict(status='ALL_FRAMES_MATCH',segments=segments,
                  source_pixels_checked=sum(s['frames'] for s in segments)*304*224,
                  provenance={n:sha(capture/n) for n in ('capture.json','frames.jsonl','raster.jsonl','reference.wav','inputs.jsonl')},
                  limits='Original tutorial and Marco-only confirmation graphics/audio; other character selection and complete gameplay actions remain pending. Scene split frames are presentation edits, not recovered original controller entry points.')
    (output/'frontend.json').write_text(json.dumps(report,indent=2)+'\n')
    print('PASS: all frontend source frames independently matched', flush=True)
    return report


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('capture',type=Path)
    parser.add_argument('--bios',type=Path,required=True)
    parser.add_argument('--workers',type=int,default=3,choices=range(1,9))
    parser.add_argument('--output',type=Path,default=ROOT/'filesystem/cinematics/frontend')
    args = parser.parse_args()
    build(args.capture.resolve(),args.bios.resolve(),args.output.resolve(),args.workers)
