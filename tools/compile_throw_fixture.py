#!/usr/bin/env python3
"""Compile original throw/spawn numerical fixtures and constant ROM lookups."""
import json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
f=json.loads((ROOT/'data/soldier_throw_fixture.json').read_text())
def rec(v):return '{'+','.join('.'+k+'='+str(int(x)) for k,x in v.items())+'}'
lines=['/* Original measured outputs, not computed by the native implementation. */',
    'static const unsigned throw_spawn_frame='+str(f['spawn']['frame'])+';',
    'static const unsigned throw_flight_frames[]={'+','.join(str(c['frame']) for c in f['cases'])+'};',
    'static const neo_contact_actor_t throw_source='+rec(f['spawn']['source'])+';',
    'static const neo_contact_actor_t throw_spawned='+rec(f['spawn']['child'])+';',
    'static const neo_contact_parent_t throw_scene='+rec(f['spawn']['scene'])+';',
    'static const neo_contact_actor_t throw_initialized='+rec(f['initialization']['after'])+';',
    'static const neo_control_frame_t throw_players='+rec(f['initialization']['scene'])+';',
    'static const uint16_t throw_field6e='+str(f['initialization']['field6e'])+';',
    'static const uint8_t throw_spawn_epoch='+str(f['initialization']['before']['mark'])+';',
    'static const struct { neo_contact_actor_t before,accelerated,after; neo_contact_request_t request; neo_contact_result_t kernel; neo_contact_parent_t parent; uint8_t epoch; } throw_cases[]={']
for c in f['cases']:
    lines.append('{'+','.join([rec(c[k]) for k in ('before','after_acceleration','after','request','kernel','parent')])+','+str(c['epoch'])+'},')
lines.append('};');(ROOT/'build/throw_fixture.h').write_text('\n'.join(lines)+'\n')
t=f['tables'];(ROOT/'build/throw_tables.h').write_text('/* ROM 03BC20 and 012E6A+4000, provenance in fixture. */\nstatic const int16_t throw_target_rectangle[]={'+','.join(str(v) for v in t['target_rectangle'])+'};\nstatic const uint32_t throw_air_kind='+str(t['air_kind'])+';\n')
