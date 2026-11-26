#!/usr/bin/env python3
"""Code overlays of the console cartridge (docs/RENDIMIENTO.md, "Superposiciones de código").

The four mission bosses (neo_m3boss, neo_m4boss, neo_m5boss, neo_m6boss) run
one mission at a time, so on the console their code shares one region of the
program's bss (msx_overlay_region, src/msx_overlay.c): the main program keeps
only each module's routine test (`neo_mXboss_routine`, which every dispatch
asks) and the boss's `_tick` code is linked here, after the main program, at
the region's address against the main program's symbols (--just-symbols),
from the same object file the main link used (compiled -G0: no gp-relative
data). The result goes into the cartridge as a rompak file (`.ovl`, found at
run time with rompak_search_ext) and src/msx_overlay.c copies a boss into the
region when its mission starts (or at its first tick).

The functions the boss code calls in the main program are kept there by the
EXTERN list (`--externs`, every symbol the overlay objects leave undefined),
so the main link's --gc-sections cannot drop them; an overlay that still
needs a symbol the main program lacks fails here (ld: undefined reference).

  link_overlays.py --externs OUT.externs OBJ...     (before the main link)
  link_overlays.py ELF MAP OUT.ovl                  (after it; nothing without the region)

A second region (msx_overlay_code_region) holds, one mission at a time, the
rest of each mission's own code (Missions 2 to 6: props, vehicles, enemies
that only that mission's lists make; one image per mission, several ticks).

File layout (big endian): 'MSXO', count, then per region (address, bytes),
then per overlay (offset, bytes); each image is 16-byte aligned and begins with
its own header: 'MSXo', id, image bytes, tick count, TICKS_MAX tick addresses."""
import os, re, struct, subprocess, sys, tempfile
from pathlib import Path

B = Path(os.environ.get('N64_INST', '/home/user/toolchains/package/opt/libdragon')) / 'bin' / 'mips64-elf-'
# id order = enum msx_overlay_id_t (src/msx_overlay.h): (image, region, [(module, tick), ...]);
# the ticks' order is the slot order of msx_overlay_code_tick (src/neo_region.c).
OVERLAYS = [('neo_m3boss', 0, [('neo_m3boss', 'neo_m3boss_tick')]),
            ('neo_m4boss', 0, [('neo_m4boss', 'neo_m4boss_tick')]),
            ('neo_m5boss', 0, [('neo_m5boss', 'neo_m5boss_tick')]),
            ('neo_m6boss', 0, [('neo_m6boss', 'neo_m6boss_tick')]),
            ('neo_ending', 0, [('neo_ending', 'neo_ending_tick')]),   # the ending: results, credits, ranking, title (src/neo_ending.c)
            # Each mission's own code but its boss (region 1, msx_overlay_code_region): one image per mission.
            ('m2code', 1, [('neo_m2boss', 'neo_m2boss_tick'), ('neo_m2props', 'neo_m2props_tick'),
                           ('neo_m2traps', 'neo_m2traps_tick'), ('neo_m2creature', 'neo_m2creature_tick')]),
            ('m3code', 1, [('neo_m3mid', 'neo_m3mid_tick'), ('neo_m3veh', 'neo_m3veh_tick'), ('neo_m3props', 'neo_m3props_tick')]),
            ('m4code', 1, [('neo_m4props', 'neo_m4props_tick'), ('neo_m4veh', 'neo_m4veh_tick')]),
            ('m5code', 1, [('neo_m5enemy', 'neo_m5enemy_tick'), ('neo_m5mech', 'neo_m5mech_tick'), ('neo_m5props', 'neo_m5props_tick'),
                           ('neo_m5sewer', 'neo_m5sewer_tick'), ('neo_m5train', 'neo_m5train_tick')]),
            ('m6code', 1, [('neo_m6enemy', 'neo_m6enemy_tick'), ('neo_m6props', 'neo_m6props_tick')])]
TICKS_MAX = 5   # image head: 'MSXo', id, bytes, tick count, TICKS_MAX tick addresses (src/msx_overlay.c)
REGIONS = ['msx_overlay_region', 'msx_overlay_code_region']
SCRIPT = """ENTRY({entry})
SECTIONS {{
  . = 0xffffffff{addr:08x};
  .ovl : {{
    __ovl_start = .;
    LONG(0x4d53586f) LONG({id}) LONG(__ovl_end - __ovl_start) LONG({count}) {ticks}
    *(.text .text.*)
    *(.rodata .rodata.* .data .data.* .lit4 .lit8)
    *(.bss .bss.* COMMON)
    . = ALIGN(16);
    __ovl_end = .;
  }}
  /DISCARD/ : {{ *(.MIPS.abiflags) *(.reginfo) *(.gnu.attributes) *(.pdr) *(.comment) *(.mdebug*) *(.gcc_compiled*) *(.eh_frame) }}
}}
"""


def tool(name, *args):
    return subprocess.check_output([str(B) + name, *map(str, args)], text=True)


def externs(out, objs):
    syms = set()
    for o in objs:
        for l in tool('nm', '-u', o).splitlines():
            p = l.split()
            if len(p) == 2 and p[0] == 'U': syms.add(p[1])
    Path(out).write_text('EXTERN(%s)\n' % ' '.join(sorted(syms)))


def link(elf, mapf, out):
    regions = {}
    for l in tool('nm', '-S', elf).splitlines():
        p = l.split()
        if len(p) == 4 and p[3] in REGIONS: regions[REGIONS.index(p[3])] = (int(p[0], 16), int(p[1], 16))
    if not regions:   # a cartridge without the dispatch: no overlay file
        Path(out).unlink(missing_ok=True); return
    if len(regions) != len(REGIONS): sys.exit(f'{elf}: not every overlay region ({", ".join(REGIONS)})')
    loaded = {}
    for l in Path(mapf).read_text().splitlines():
        m = re.match(r'^LOAD (\S+/(\w+)\.o)$', l)
        if m: loaded[m[2]] = m[1]
    images = []
    with tempfile.TemporaryDirectory() as tmp:
        tmp = Path(tmp)
        for i, (name, reg, mods) in enumerate(OVERLAYS):
            addr, size = regions[reg]
            los = []
            for mod, tick in mods:
                obj = loaded.get(mod)
                if not obj: sys.exit(f'{elf}: {mod}.o not in the link map')
                if 'GPREL' in tool('readelf', '-r', '-W', obj): sys.exit(f'{obj}: gp-relative relocations (compile it with -G0)')
                lo = tmp / f'{mod}.o'
                # Only the tick stays global: the routine test and any other global
                # of the module are also in the main program, here they bind locally.
                subprocess.check_call([str(B) + 'objcopy', f'--keep-global-symbol={tick}', obj, lo])
                los.append(lo)
            ticks = [t for _, t in mods]
            if len(ticks) > TICKS_MAX: sys.exit(f'{name}: more than {TICKS_MAX} ticks')
            ld = tmp / f'{name}.ld'
            ld.write_text(SCRIPT.format(entry=ticks[0], addr=addr, id=i, count=len(ticks),
                                        ticks=' '.join(f'LONG({t})' for t in ticks + ['0'] * (TICKS_MAX - len(ticks)))))
            oe = Path(out).with_suffix(f'.{name}.elf')   # kept: addr2line of a crash in the overlay
            subprocess.check_call([str(B) + 'ld', '-T', ld, f'--just-symbols={elf}', '--gc-sections', '-nostdlib',
                                   *[f'--require-defined={t}' for t in ticks],
                                   '-Map', Path(out).with_suffix(f'.{name}.map'), '-o', oe, *los])
            ob = tmp / f'{name}.bin'
            subprocess.check_call([str(B) + 'objcopy', '-O', 'binary', '-j', '.ovl', oe, ob])
            data = ob.read_bytes()
            magic, oid, n, count = struct.unpack('>4I', data[:16])
            entries = struct.unpack(f'>{TICKS_MAX}I', data[16:16 + 4 * TICKS_MAX])
            if magic != 0x4d53586f or oid != i or n != len(data) or count != len(ticks) \
                    or not all(addr <= t < addr + n for t in entries[:count]):
                sys.exit(f'{name}: bad overlay image')
            if n > size: sys.exit(f'{name}: {n} bytes, over the region {REGIONS[reg]} ({size} bytes, src/msx_overlay.h)')
            images.append((name, data))
    head = struct.pack('>2I', 0x4d53584f, len(images)) + b''.join(struct.pack('>2I', *regions[r]) for r in range(len(REGIONS)))
    at = len(head) + 8 * len(images); at += -at % 16
    table, body = b'', b''
    for mod, data in images:
        table += struct.pack('>2I', at + len(body), len(data)); body += data + b'\0' * (-len(data) % 16)
    blob = head + table; blob += b'\0' * (at - len(blob))
    Path(out).write_bytes(blob + body)
    print(f'{out}: ' + ', '.join(f'region {r} 0x{regions[r][0]:08x} {regions[r][1]} bytes' for r in range(len(REGIONS))) + '; '
          + ', '.join(f'{m} {len(d)}' for m, d in images))


if __name__ == '__main__':
    if len(sys.argv) >= 3 and sys.argv[1] == '--externs': externs(sys.argv[2], sys.argv[3:])
    elif len(sys.argv) == 4: link(*sys.argv[1:])
    else: sys.exit(__doc__)
