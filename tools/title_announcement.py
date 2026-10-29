#!/usr/bin/env python3
"""Prepare source title PCM plus the authored synthetic 'sixty-four' tail.

Always start from the private source capture, so repeated builds never stack tags.
The insertion point is an authored audio edit, not a discovered ROM address.
"""
import argparse
import hashlib
import json
import subprocess
import wave
from pathlib import Path
import numpy as np
from pack_cinematic import nanos

ROOT = Path(__file__).resolve().parents[1]
TAIL = ROOT/'art/audio/sixty-four.wav'
START_SECONDS = 8.60


def append_tail(pcm, rate):
    original = np.frombuffer(pcm, dtype='<i2').reshape(-1, 2)
    with wave.open(str(TAIL)) as voice:
        assert voice.getframerate() == rate and voice.getnchannels() == 1 and voice.getsampwidth() == 2
        tag = np.frombuffer(voice.readframes(voice.getnframes()), dtype='<i2')
    start = round(START_SECONDS * rate)
    end = start + len(tag)
    assert end <= len(original), 'Announcement must finish before title handoff'
    mixed = original.astype(np.int32)
    mixed[start:end] += tag[:, None]
    peak = int(np.abs(mixed).max())
    assert peak < 32767, 'Do not clip original effects or synthetic announcement'
    result = mixed.astype('<i2').tobytes()
    assert result[:start*4] == pcm[:start*4]
    assert result[end*4:] == pcm[end*4:]
    return result, dict(text='Sixty-four!', voice='Piper stock en_US-ryan-high; synthetic, not a clone',
        start_seconds=START_SECONDS, end_seconds=end/rate, placement='Authored after the spoken X, over its remaining echo',
        previous_audio_unchanged=True, peak_pcm16=peak, asset_sha256=hashlib.sha256(TAIL.read_bytes()).hexdigest())


def prepare(capture, output, frames, first_frame, last_frame, add_tail):
    with wave.open(str(capture/'reference.wav')) as source:
        assert source.getframerate() == 48000 and source.getnchannels() == 2 and source.getsampwidth() == 2
        first = nanos(frames[first_frame]['time'])*48000//10**9
        last = nanos(frames[last_frame+1]['time'])*48000//10**9
        source.setpos(first)
        pcm = source.readframes(last-first)
    announcement = None
    if add_tail:
        pcm, announcement = append_tail(pcm, 48000)
    with wave.open(str(output/'title-motion.wav'), 'wb') as target:
        target.setnchannels(2); target.setsampwidth(2); target.setframerate(48000); target.writeframes(pcm)
    subprocess.run([str(ROOT.parent/'toolchains/package/opt/libdragon/bin/audioconv64'),
        '--wav-resample', '32000', '--wav-compress', '0', '--wav-loop', 'false',
        '-o', str(output), str(output/'title-motion.wav')], check=True)
    return announcement


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('capture', type=Path)
    args = parser.parse_args()
    output = ROOT/'filesystem/cinematics'
    path = output/'title-motion.json'
    metadata = json.loads(path.read_text())
    for name in ('reference.wav', 'frames.jsonl'):
        assert hashlib.sha256((args.capture/name).read_bytes()).hexdigest() == metadata['provenance'][name]
    frames = {row['frame']: row for line in (args.capture/'frames.jsonl').read_text().splitlines() if (row := json.loads(line))}
    metadata['announcement'] = prepare(args.capture, output, frames, metadata['first_frame'], metadata['last_frame'], True)
    metadata['audio_sha256'] = hashlib.sha256((output/'title-motion.wav64').read_bytes()).hexdigest()
    metadata['limits'] = 'Animated X64 title with original motion/effects and synthetic sixty-four tail. Not a full Mission 1 demo or original actor voice clone.'
    path.write_text(json.dumps(metadata, indent=2)+'\n')
    print(json.dumps(metadata['announcement'], indent=2))

if __name__ == '__main__':
    main()
