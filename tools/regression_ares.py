#!/usr/bin/env python3
"""Regression matrix in ares 146 on the full release cartridge (docs/FALLOS.md).

Runs every release-cart ares test one after another on one ROM, plus the
Mission 1 -> 2 test on an m1end cartridge, and counts in each test's ares.log
the lines that point to a fault: `native draw missing`, `unsupported`,
`missing:` (pack or part), `stop`. The counts are compared with a baseline
JSON (data/regression_baseline.json): a test that passed in the baseline and
fails now, or a count that grows, is a regression. --write-baseline stores the
current counts as the new baseline (only after a reviewed green run).

  python3 tools/regression_ares.py mslugx-explorer.z64 --m1end mslugx-m1end.z64 \
      --out OUT [--only m6s3,m5s3] [--write-baseline]

Before running: the cartridge must be at most 67,043,328 bytes (above that
ares writes no log and every test hangs) — the script refuses a larger one.
"""
import argparse, json, pathlib, re, subprocess, sys, time

ROOT = pathlib.Path(__file__).resolve().parent.parent
LIMIT = 67_043_328
BASELINE = ROOT / 'data' / 'regression_baseline.json'

# name: (script, extra args, timeout in s, output flag)
TESTS = {
    'logo':  ('test_logo_ares.py', [], 600, '--output'),
    'pause': ('test_pause_ares.py', [], 600, '--output'),
    'chars': ('test_characters_ares.py', [], 1800, '--output'),
    'm4':    ('test_mission4_ares.py', [], 600, '--out'),
    'm5s1':  ('test_mission5_ares.py', ['--stage', '1', '--progress', '100', '--fire', '--jump', '2.5'], 600, '--out'),
    'm5s2':  ('test_mission5_ares.py', ['--stage', '2', '--progress', '80', '--fire', '--jump', '2.5'], 600, '--out'),
    'm5s3':  ('test_mission5_ares.py', ['--stage', '3', '--progress', '100', '--fire', '--jump', '2.5'], 900, '--out'),
    'm6s1':  ('test_mission6_ares.py', ['--stage', '1'], 600, '--out'),
    'm6s2':  ('test_mission6_ares.py', ['--stage', '2'], 600, '--out'),
    'm6s3':  ('test_mission6_ares.py', ['--stage', '3'], 600, '--out'),
}
M1M2 = ('test_m1m2_ares.py', [], 1200, '--output')

COUNTS = {
    'missing_draw': re.compile(rb'native draw missing'),
    'unsupported': re.compile(rb'unsupported routine'),
    'missing_part': re.compile(rb'missing:'),
    'stopped': re.compile(rb'stopped program|stop routine'),
}


def run(name, script, extra, timeout, outflag, rom, out):
    d = out / name
    cmd = [sys.executable, str(ROOT / 'tools' / script), str(rom), outflag, str(d)] + extra
    t0 = time.time()
    try:
        p = subprocess.run(cmd, cwd=ROOT, capture_output=True, timeout=timeout)
        text = p.stdout + p.stderr
        # test_logo_ares.py reports its verdict as the JSON field "status" instead of a RESULT line.
        result = 'PASS' if b'RESULT: PASS' in text or re.search(rb'"status": "PASS"', text) else 'FAIL'
    except subprocess.TimeoutExpired as e:
        text = (e.stdout or b'') + (e.stderr or b'')
        result = 'TIMEOUT'
    (out / f'{name}.txt').write_bytes(text)
    logs = list(d.rglob('*.log')) if d.exists() else []
    blob = b''.join(l.read_bytes() for l in logs)
    counts = {k: len(r.findall(blob)) for k, r in COUNTS.items()}
    return {'result': result, 'seconds': round(time.time() - t0), **counts}


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('rom')
    ap.add_argument('--m1end')
    ap.add_argument('--out', required=True)
    ap.add_argument('--only')
    ap.add_argument('--write-baseline', action='store_true')
    a = ap.parse_args()
    rom = pathlib.Path(a.rom).resolve()
    if rom.stat().st_size > LIMIT:
        sys.exit(f'{rom}: {rom.stat().st_size} bytes, over {LIMIT}: ares writes no log; make the cart smaller first')
    out = pathlib.Path(a.out).resolve(); out.mkdir(parents=True, exist_ok=True)
    names = a.only.split(',') if a.only else list(TESTS) + (['m1m2'] if a.m1end else [])
    base = json.loads(BASELINE.read_text()) if BASELINE.exists() else {}
    now, bad = {}, []
    for n in names:
        if n == 'm1m2':
            if not a.m1end:
                sys.exit('m1m2 needs --m1end')
            r = run(n, *M1M2, pathlib.Path(a.m1end).resolve(), out)
        else:
            r = run(n, *TESTS[n], rom, out)
        now[n] = r
        b = base.get(n)
        why = []
        if r['result'] != 'PASS' and (b is None or b['result'] == 'PASS'):
            why.append(f"{r['result']} (baseline {b['result'] if b else 'none'})")
        if b:
            why += [f'{k} {b[k]}->{r[k]}' for k in COUNTS if r[k] > b[k]]
        if why:
            bad.append(n)
        print(f"{n:6} {r['result']:7} {r['seconds']:5}s  " + ' '.join(f'{k}={r[k]}' for k in COUNTS)
              + ('   REGRESSION: ' + '; '.join(why) if why else ''), flush=True)
    (out / 'summary.json').write_text(json.dumps(now, indent=1))
    if a.write_baseline:
        base.update(now); BASELINE.write_text(json.dumps(base, indent=1, sort_keys=True) + '\n')
        print(f'baseline written: {BASELINE}')
    print('RESULT: ' + ('FAIL (' + ', '.join(bad) + ')' if bad else 'PASS'))
    sys.exit(1 if bad else 0)


if __name__ == '__main__':
    main()
