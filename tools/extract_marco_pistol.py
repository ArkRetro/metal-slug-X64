#!/usr/bin/env python3
"""Extract read-only live pistol initialization and actor-call evidence."""
import argparse, collections, csv, hashlib, json, re, struct, zipfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
p = argparse.ArgumentParser(description=__doc__)
p.add_argument('capture', type=Path)
p.add_argument('--rom', type=Path, required=True)
p.add_argument('--plan', type=Path, required=True)
p.add_argument('--output', type=Path, default=ROOT/'data/original-pistol-trace.csv')
p.add_argument('--validation', type=Path, default=ROOT/'docs/marco-pistol-validation.json')
a = p.parse_args()
with zipfile.ZipFile(a.rom) as archive:
    raw = archive.read('250-p1.p1')
assert hashlib.sha1(raw).hexdigest() == json.loads((ROOT/'tools/rom_manifest.json').read_text())['250-p1.p1']['sha1']
rom = bytearray(raw)
rom[::2], rom[1::2] = raw[1::2], raw[::2]
observed = (a.capture/'program-fixed.bin').read_bytes()
ranges = [(0x29fc, 640), (0x2e7c, 28), (0x1b782, 188), (0xc4204, 192), (0xc42aa, 64), (0xc4372, 512)]
assert all(observed[s:s+n] == rom[s:s+n] for s, n in ranges)
assert struct.unpack_from('>I',rom,0xc42aa)[0] == 0x1b782
assert struct.unpack_from('>I',rom,0x1b796)[0] == 0x100000
assert [struct.unpack_from('>hh',rom,s) for s in (0xc4204,0xc4214,0xc4224,0xc4234)] == [(0,-16),(16,10),(0,40),(16,16)]

def signed16(v): return v if v < 32768 else v-65536
def fields(line): return {k:int(v,16) for k,v in re.findall(r'(\w+)=([0-9A-F]+)',line)}

rows, pending, previous, birth = [], collections.deque(), {}, {}
steps, draws, sounds = 0, 0, 0
first_draw_delays = collections.Counter()
for line in (a.capture/'debug.log').read_text().splitlines():
    match = re.match(r'PISTOL_(\w+) frame=(\d+) (.*)',line)
    if not match: continue
    kind, frame, r = match[1], int(match[2]), fields(match[3])
    if kind == 'EVENT':
        assert r['kind'] == r['form'] == 0
        pending.append(dict(frame=frame, **r))
    elif kind == 'MUZZLE':
        assert pending and 'dx' not in pending[-1]
        pending[-1].update(r)
    elif kind == 'VECTOR':
        e = pending.popleft()
        assert frame-e['frame'] == 2
        assert r['x'] == (e['x']+signed16(e['dx'])*65536)&0xffffffff
        assert r['y'] == (e['y']+signed16(e['dy'])*65536)&0xffffffff
        angle = e['angle']
        vx = (0x100000,0,-0x100000,0)[angle//64]
        vy = (0,0x100000,0,-0x100000)[angle//64]
        assert angle in (0,64,128,192) and r['angle'] == angle
        assert r['vx'] == vx&0xffffffff and r['vy'] == vy&0xffffffff
        rows.append([e['frame'],frame,e['x'],e['y'],e['desired'],e['aim'],e['attr'],e['offsets'],e['dx'],e['dy'],angle,r['x'],r['y'],r['vx'],r['vy']])
        previous[r['actor']] = None
        birth[r['actor']] = frame
    elif kind == 'STEP':
        old = previous.get(r['actor'])
        if old:
            old_frame, old_r = old
            assert frame-old_frame == 2
            assert r['x'] == (old_r['x']+old_r['vx'])&0xffffffff
            assert r['y'] == (old_r['y']+old_r['vy'])&0xffffffff
            steps += 1
        previous[r['actor']] = frame,r
    elif kind == 'SOUND':
        assert r['token'] == 0x10f7
        sounds += 1
    elif kind == 'DRAW':
        angle = r['angle']
        assert angle in (0,64,128,192)
        assert r['descriptor'] == (0x1c7298 if angle&64 else 0x1c7270)
        assert r['attr']&3 == (0,1,3,2)[angle//64]
        if r['actor'] in birth:
            first_draw_delays[frame-birth.pop(r['actor'])] += 1
        draws += 1
assert not pending and len(rows) == 134 and steps == 633 and sounds == 134 and draws == 625
assert first_draw_delays == {2:84}
assert {r[10] for r in rows} == {0,64,128,192}
with a.output.open('w') as out:
    writer = csv.writer(out,lineterminator='\n')
    writer.writerow('event_frame first_step_frame x y desired aim attributes pose_offsets dx dy angle child_x child_y vx vy'.split())
    writer.writerows(rows)
sha = lambda f: hashlib.sha256(f.read_bytes()).hexdigest()
report = dict(
    source='MAME 0.276, supplied mslugx ROM/euro BIOS; scheduled joypad input and read-only breakpoints',
    cases=len(rows), cardinal_angles=[0,64,128,192], successive_projectile_steps=steps,
    allocation_to_first_step_video_ticks=2, projectile_actor_step_video_ticks=2,
    allocation_to_first_draw_video_ticks=4, visible_children=84,
    original_draws=draws, initializer_sound_calls=sounds, initializer_sound_token='0x10F7',
    sound_handler='0x0120F8 validates the token; 0x012182..0x01218C sends 10,F7 via 0x01209C',
    cardinal_graphics=[dict(angle=angle,descriptor=hex(descriptor),flips=flips)
        for angle,descriptor,flips in ((0,0x1c7270,0),(64,0x1c7298,1),(128,0x1c7270,3),(192,0x1c7298,2))],
    scalar_q16=0x100000, callback_target='0x0C4372', initializer='0x01B782',
    provenance={str(f):sha(f) for f in (a.capture/'debug.log',a.capture/'program-fixed.bin',a.plan,ROOT/'tools/mame/probe_marco_pistol.lua',a.output)},
    selected_source_ranges_match_supplied_rom=True,
    limits=[
        'Normal form, weapon kind zero. Original cardinal muzzle/velocity initialization and captured actor-call cadence are verified.',
        'Other weapons/forms, damage values, hit shapes, impact animations, audio playback and the complete original projectile collision/scheduler remain outside this backend.',
        'World storage capacity and world collisions are native scene services; the original pistol cap is FF.',
        'The CSV is host-test evidence only and is never read by campaign runtime.'
    ])
a.validation.write_text(json.dumps(report,indent=2)+'\n')
print(f'Extracted {len(rows)} pistol initializations and {steps} successive actor steps')
