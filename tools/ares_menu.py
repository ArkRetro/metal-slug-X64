"""The test menu of SOLDIER SELECT for the ares tests, driven by what the cartridge logs.

The tests used to count presses on the host's clock (Up seven times, 0.1 s each): with the machine loaded ares runs at a
third of its speed, a press shorter than one pad read is lost and the count lands on another mission. Here every press
is held until the cartridge logs its effect (`MSX MSELECT cursor=N`, `lifebar=N`, `mission=M stage=S`; src/main.c) and
the cursor is read back from the log, so a lost or doubled press only costs another one.

    choose(key, log, proc, mission, stage, lifebar)    # from the boot to `MSX MSELECT mission=M stage=S`

`key(action, name)` is the test's xdotool call, `log()` returns the ares log, `proc` is the ares process.
"""
import re, time

# src/main.c mselect_items, read from the source so the list cannot drift from the cartridge's (13, 1 is ENDING).
from pathlib import Path as _P
ITEMS = [(int(a), int(b)) for a, b in re.findall(r'\{(\d+),(\d+)\}', re.search(r'mselect_items\[\]=\{(.*?)\};', (_P(__file__).resolve().parents[1] / 'src/main.c').read_text()).group(1))]
BOOT = (('MSX TITLE start', 'MSX TITLE complete'), ('MSX CINE howto start', 'MSX CINE howto complete'), ('MSX OPTIONS open', 'MSX OPTIONS close'))


def held(key, name, log, pattern, seconds=8.0):
    """Hold `name` until the log grows a line matching `pattern` (or `seconds` of the host pass). Returns the host
    seconds it took, or None. The caller waits before the next press: the cartridge must read the key released."""
    mark = len(log()); t0 = time.monotonic(); took = None
    key('keydown', name)
    try:
        while time.monotonic() - t0 < seconds:
            if re.search(pattern, log()[mark:]):
                took = time.monotonic() - t0; break
            time.sleep(.02)
    finally:
        key('keyup', name)
    time.sleep(max(.4, 3 * (took if took is not None else .5)))
    return took


def start_through_boot(key, log):
    """One Start while the title, HOW TO PLAY or the options screen runs (never later: it would choose the soldier)."""
    t = log()
    if any(a in t and b not in t for a, b in BOOT):
        key('keydown', 'Return'); time.sleep(.3); key('keyup', 'Return')
        return True
    return False


def cinematic_running(text):
    """True while the title, HOW TO PLAY, SOLDIER SELECT, the confirmation or the options screen runs (its start logged
    and not its end): a Start then skips it. Never in the entry film: it ends by itself, and a press landing in the game
    is the ROM's pause."""
    last = None
    for m in re.finditer(r'MSX (TITLE|CINE (?:howto|select|confirm|entry)) (start|complete)', text):
        last = m
    if 'MSX OPTIONS open' in text and 'MSX OPTIONS close' not in text:
        return True
    return last is not None and last.group(2) == 'start' and 'entry' not in last.group(1)


def choose(key, log, proc, mission, stage, lifebar=False, seconds=900):
    target = ITEMS.index((mission, stage)); end = time.monotonic() + seconds
    def alive():
        assert proc.poll() is None and time.monotonic() < end, 'no test menu\n' + log()[-1500:]
    # Start through the boot movies; at SOLDIER SELECT, L + R (keys d, e) held and A until the menu opens.
    while 'MSX MSELECT menu open' not in log():
        alive()
        assert 'MSX CINE select chosen' not in log(), 'SOLDIER SELECT chose a soldier before the menu combo'
        if re.search(r'MSX CINE select (start|waiting)', log()):
            time.sleep(1.5); key('keydown', 'd'); key('keydown', 'e'); time.sleep(.6)
            held(key, 'a', log, r'MSX MSELECT menu open', 4.0)
            key('keyup', 'e'); key('keyup', 'd')
            continue
        start_through_boot(key, log); time.sleep(1.0)
    time.sleep(.5)
    # The cursor to the item, by the shorter way round; the life bar; A.
    n = len(ITEMS)
    while True:
        alive()
        cur = int(re.findall(r'MSX MSELECT (?:menu open )?cursor=(\d+)', log())[-1])
        if cur == target:
            break
        held(key, 'Up' if (cur - target) % n <= (target - cur) % n else 'Down', log, r'MSX MSELECT cursor=\d+')
    while True:
        alive()
        m = re.findall(r'MSX MSELECT lifebar=(\d)', log())
        if (m[-1] == '1' if m else False) == bool(lifebar):
            break
        held(key, 'z', log, r'MSX MSELECT lifebar=\d')
    while 'MSX MSELECT mission=' not in log():
        alive()
        held(key, 'a', log, r'MSX MSELECT mission=')
    m = re.findall(r'MSX MSELECT mission=(\d+) stage=(\d+)', log())[-1]
    assert (int(m[0]), int(m[1])) == (mission, stage), 'the menu chose mission %s stage %s' % m
