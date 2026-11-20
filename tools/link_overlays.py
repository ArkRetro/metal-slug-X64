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

File layout (big endian): 'MSXO', count, region address, region bytes, then
per overlay (offset, bytes); each image is 16-byte aligned and begins with its
own header: 'MSXo', id, address of its tick function, image bytes."""
import os, re, struct, subprocess, sys, tempfile
from pathlib import Path

B = Path(os.environ.get('N64_INST', '/home/user/toolchains/package/opt/libdragon')) / 'bin' / 'mips64-elf-'
# id order = enum msx_overlay_id_t (src/msx_overlay.h).
OVERLAYS = [('neo_m3boss', 'neo_m3boss_tick'), ('neo_m4boss', 'neo_m4boss_tick'),
            ('neo_m5boss', 'neo_m5boss_tick'), ('neo_m6boss', 'neo_m6boss_tick')]
REGION = 'msx_overlay_region'
SCRIPT = """ENTRY({tick})
SECTIONS {{
  . = 0xffffffff{addr:08x};
  .ovl : {{
    __ovl_start = .;
    LONG(0x4d53586f) LONG({id}) LONG({tick}) LONG(__ovl_end - __ovl_start)
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
    region = None
    for l in tool('nm', '-S', elf).splitlines():
        p = l.split()
        if len(p) == 4 and p[3] == REGION: region = (int(p[0], 16), int(p[1], 16))
    if region is None:   # a cartridge without the bosses' dispatch: no overlay file
        Path(out).unlink(missing_ok=True); return
    addr, size = region
    loaded = {}
    for l in Path(mapf).read_text().splitlines():
        m = re.match(r'^LOAD (\S+/(\w+)\.o)$', l)
        if m: loaded[m[2]] = m[1]
    images = []
    with tempfile.TemporaryDirectory() as tmp:
        tmp = Path(tmp)
        for i, (mod, tick) in enumerate(OVERLAYS):
            obj = loaded.get(mod)
            if not obj: sys.exit(f'{elf}: {mod}.o not in the link map')
            if 'GPREL' in tool('readelf', '-r', '-W', obj): sys.exit(f'{obj}: gp-relative relocations (compile it with -G0)')
            lo = tmp / f'{mod}.o'
            # Only the tick stays global: the routine test and any other global
            # of the module are also in the main program, here they bind locally.
            subprocess.check_call([str(B) + 'objcopy', f'--keep-global-symbol={tick}', obj, lo])
            ld = tmp / f'{mod}.ld'; ld.write_text(SCRIPT.format(tick=tick, addr=addr, id=i))
            oe = Path(out).with_suffix(f'.{mod}.elf')   # kept: addr2line of a crash in the overlay
            subprocess.check_call([str(B) + 'ld', '-T', ld, f'--just-symbols={elf}', '--gc-sections', '-nostdlib',
                                   '-Map', Path(out).with_suffix(f'.{mod}.map'), '-o', oe, lo])
            ob = tmp / f'{mod}.bin'
            subprocess.check_call([str(B) + 'objcopy', '-O', 'binary', '-j', '.ovl', oe, ob])
            data = ob.read_bytes()
            magic, oid, entry, n = struct.unpack('>4I', data[:16])
            if magic != 0x4d53586f or oid != i or n != len(data) or not addr <= entry < addr + n:
                sys.exit(f'{mod}: bad overlay image')
            if n > size: sys.exit(f'{mod}: {n} bytes, over the region ({size} bytes, MSX_OVERLAY_BYTES in src/msx_overlay.h)')
            images.append((mod, data))
    head = struct.pack('>4I', 0x4d53584f, len(images), addr, size)
    at = len(head) + 8 * len(images); at += -at % 16
    table, body = b'', b''
    for mod, data in images:
        table += struct.pack('>2I', at + len(body), len(data)); body += data + b'\0' * (-len(data) % 16)
    blob = head + table; blob += b'\0' * (at - len(blob))
    Path(out).write_bytes(blob + body)
    print(f'{out}: region 0x{addr:08x} {size} bytes; ' + ', '.join(f'{m} {len(d)}' for m, d in images))


if __name__ == '__main__':
    if len(sys.argv) >= 3 and sys.argv[1] == '--externs': externs(sys.argv[2], sys.argv[3:])
    elif len(sys.argv) == 4: link(*sys.argv[1:])
    else: sys.exit(__doc__)
