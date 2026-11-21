"""--record / --pad-script for the playing ares tests (B78, docs/FALLOS.md "Mando por guion").

    import ares_padtest as pt; pt.add_args(p); ...; a=p.parse_args(); pt.prepare(a, out)
    key = pt.keyer(a, real_key)            # a replay presses nothing
    ... after the pad loop: pt.finish(a, proc, logp, ends)   # stops ares, cuts the log at the end condition

The ROM must be the test cartridge (mslugx-testpad.z64). Both modes copy it into --out (a fresh EEPROM); a replay adds
the script (tools/padscript.py). `ends` are regexes of end lines; the log is also cut at the first ` ticks=N` line with
N >= --seconds * 60 when the test has --seconds.
"""
import re, shutil, time
from pathlib import Path


def add_args(p):
    p.add_argument('--pad-script', help='replay this pad script on the test cartridge (mslugx-testpad.z64): no xdotool')
    p.add_argument('--record', action='store_true', help='the test cartridge with the real pad: its log is the script')


def active(a):
    return bool(a.pad_script or a.record)


def prepare(a, out):
    if not active(a):
        return
    rom = Path(out) / 'testpad.z64'
    for f in (Path(out) / 'testpad.eeprom', Path(out) / 'testpad.pak'):
        if f.exists():
            f.unlink()
    if a.pad_script:
        from padscript import apply, read_script
        apply(a.rom, read_script(a.pad_script), rom)
    else:
        shutil.copyfile(a.rom, rom)
    a.rom = str(rom)


def keyer(a, real):
    return (lambda action, name: None) if a.pad_script else real


def finish(a, proc, logp, ends=()):
    if not active(a):
        return
    time.sleep(1.0); proc.terminate()
    try:
        proc.wait(10)
    except Exception:
        proc.kill(); proc.wait()
    t = Path(logp).read_text(errors='replace'); cuts = []
    for pat in ends:
        m = re.search(pat + r'[^\n]*\n', t)
        if m:
            cuts.append(m.end())
    secs = getattr(a, 'seconds', None)
    if secs:
        for m in re.finditer(r' ticks=(\d+)\n', t):
            if int(m.group(1)) >= secs * 60:
                cuts.append(m.end()); break
    if cuts:
        Path(logp).write_text(t[:min(cuts)])
