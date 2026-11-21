"""The game's own clock for the blind pads of the ares tests.

The pads of tools/test_mission*_ares.py press keys on a schedule (a jump every
N seconds, 0.15 s turns, 12 s fight cycles) and the tests end after --seconds.
Counted on the host's wall clock, the same schedule means fewer game updates
when the machine is loaded (another ares running: the emulator falls below
full speed), so a run's outcome changed with the load. The cartridge prints
its update count in the periodic stage line (`... ticks=N`, src/actor.c, at
least every 120 updates; `MSX clock ticks=N` in Mission 1); this clock turns those counts into seconds of game
time (60 updates per second) and, between two lines, goes on at the speed
measured between the last two (never faster than full speed, never beyond the
next expected line). A cartridge without `ticks=` (older builds) leaves it on
the wall clock.

    clock = EmuClock(); ...; t = clock.now(log_text_tail)
"""
import re, time

TICKS = re.compile(r' ticks=(\d+)\n')   # the stage line's last field, or `MSX clock ticks=N` (Mission 1)
HZ = 60.0
STEP = 120    # the stage line's longest gap in updates


class EmuClock:
    def __init__(self):
        self.t0 = time.monotonic()
        self.base = 0.0        # game seconds at the last line seen
        self.last = None       # its tick count
        self.wall = None       # wall time when it was first seen
        self.rate = 1.0        # game seconds per wall second
        self.out = 0.0

    def now(self, tail):
        w = time.monotonic()
        m = TICKS.findall(tail)
        if not m:
            if self.last is None:
                return w - self.t0          # no tick line yet (or an older cartridge)
        else:
            n = int(m[-1])
            if self.last is None:
                self.base = w - self.t0; self.last = n; self.wall = w
            elif n != self.last:
                d = (n - self.last) / HZ
                if d < 0:                   # the count restarted (a new game): keep going from here
                    d = 0.0
                if w > self.wall and d > 0:
                    self.rate = min(1.0, d / (w - self.wall))
                self.base += d; self.last = n; self.wall = w
        t = self.base + min((w - self.wall) * self.rate, STEP / HZ)
        self.out = max(self.out, t)         # never backwards
        return self.out


def game_sleep(clock, log, seconds, poll=.03):
    """Sleep `seconds` of game time on `clock` (`log()` returns the ares log): the same number of game updates at any
    machine load. Before the cartridge's first tick line the clock is the host's."""
    end = clock.now(log()[-4000:]) + seconds
    while clock.now(log()[-4000:]) < end:
        time.sleep(poll)
