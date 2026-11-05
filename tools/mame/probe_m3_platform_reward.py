#!/usr/bin/env python3
"""Record controlled original 0B9748/0B9780 platform-reward calls in MAME.

Synthetic scene/actor RAM isolates this object. Original projection, creator
guard, player-mask/grounded tests and rectangle helpers execute unchanged.
The default probe stops at deletion 004188, and intercepts 08CECC only to
record the reward request before returning to its caller. --score-effects
instead executes 08CECC, its original BCD addition and label placement, with
only the label allocator and sound queue replaced by explicit test boundaries.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import zipfile

ROOT = Path(__file__).resolve().parents[2]
ACTOR, CREATOR, SCENE = 0x101000, 0x101800, 0x10e968
FIXED_SHA256 = 'ff01fa7dc27b2d0db15e8e4f6f75aaf6fa943761277c0e00ca96c730fcebe37a'


def record():
    return bytearray(0xc0)


def put(data, offset, value, size):
    data[offset:offset + size] = (value & ((1 << (8 * size)) - 1)).to_bytes(size, 'big')


def make_case(name, *, entry=0xb9780, creator_x=4000, creator_y=400,
              root_x=0, root_y=0, mask=1, player_specs=None, flags=0,
              pending=(0, 0), paid=(0, 0), consumed=0, creator_alive=True,
              preserve=False, chain='', epoch=7, parent_current_low=0,
              root_correction=0, creator_tag=CREATOR):
    root = dict(x=root_x * 65536, y=root_y * 65536,
                prev_x=(root_x - 1) * 65536, prev_y=(root_y - 1) * 65536,
                correction=root_correction)
    owner = record()
    put(owner, 0, 0xb9400 if creator_alive else 0x3f1e, 4)
    put(owner, 0x28, creator_x * 65536, 4)
    put(owner, 0x2c, creator_y * 65536, 4)
    put(owner, 0x38, SCENE, 4)
    put(owner, 0x3c, creator_x * 65536 + root['x'] + parent_current_low, 4)
    put(owner, 0x40, creator_y * 65536 + root['y'], 4)
    put(owner, 0x44, creator_x * 65536 + root['prev_x'], 4)
    put(owner, 0x48, creator_y * 65536 + root['prev_y'], 4)
    put(owner, 0x4c, epoch, 1)
    put(owner, 0x4d, root_correction, 1)
    actor = record()
    put(actor, 0, entry, 4)
    put(actor, 0xc, creator_tag, 4)
    put(actor, 0x15, flags, 1)
    put(actor, 0x28, -80 * 65536 + 0x8000, 4)
    put(actor, 0x2c, 40 * 65536 + 0x4000, 4)
    put(actor, 0x38, CREATOR + 0x28, 4)
    put(actor, 0x3c, (creator_x - 81 + root_x) * 65536 + 0x5000, 4)
    put(actor, 0x40, (creator_y + 39 + root_y) * 65536 + 0x6000, 4)
    put(actor, 0x44, 0x11223344, 4)
    put(actor, 0x48, 0x55667788, 4)
    put(actor, 0x4c, epoch - 1, 1)
    put(actor, 0x66, 0xffff, 2)
    put(actor, 0x1e, 0xffff, 2)
    for offset, value in ((0x76, pending[0]), (0x77, pending[1]),
                          (0x78, paid[0]), (0x79, paid[1]), (0x7a, consumed)):
        put(actor, offset, value, 1)
    x, y = creator_x - 80 + root_x, creator_y + 40 + root_y
    specs = player_specs or [(0, 0, 0x40), (0, 0, 0x40)]
    players = [dict(x=(x + dx) & 0xffff, y=(y + dy) & 0xffff,
                    flags15=pf, routine=0xc1d96) for dx, dy, pf in specs]
    return dict(name=name, input=dict(entry=entry, actor_hex=actor.hex(), creator_hex=owner.hex(),
        root=root, epoch=epoch, active_mask=mask, players=players, preserve_previous=preserve,
        chain=chain))


def cases():
    result = [make_case('constructor-clears-three-flags', entry=0xb9748, flags=0xf7,
                        pending=(0x81, 0x82), paid=(0x83, 0x84), consumed=0x85, mask=0)]
    for mask in (0, 1, 2, 3, 4, 0xff):
        result.append(make_case(f'player-mask-{mask}', mask=mask, pending=(1, 1)))
    for grounded in (0, 0x40):
        for dx in (-37, -36, -29, -28, 0, 4, 5):
            for dy in (-9, -8, -7, -4, 0, 4, 5):
                result.append(make_case(f'contact-{grounded}-{dx}-{dy}',
                    player_specs=[(dx, dy, grounded), (0, 0, 0x40)], pending=(1, 0)))
    for threshold in (3983, 3984, 3985, 4000):
        result.append(make_case(f'limit-{threshold}', creator_x=threshold, pending=(1, 0)))
    for consumed in (1, 0xff):
        result.append(make_case(f'consumed-{consumed}', mask=3, pending=(1, 1), consumed=consumed))
    for paid in ((1, 0), (0, 1), (1, 1)):
        result.append(make_case(f'paid-{paid[0]}-{paid[1]}', mask=3, pending=(1, 1), paid=paid))
    for pending in ((0, 0), (1, 0), (0, 1), (1, 1)):
        for before in (True, False):
            result.append(make_case(f'both-order-{pending[0]}-{pending[1]}-{before}',
                creator_x=3970 if before else 4000, mask=3, pending=pending))
    for alive in (True, False):
        for owner_tag in (CREATOR, 0):
            result.append(make_case(f'creator-{alive}-{owner_tag:x}', mask=0,
                                   creator_alive=alive, creator_tag=owner_tag))
    result.append(make_case('offscreen-negative-x', creator_x=40, mask=0))
    result.append(make_case('fractional-parent-and-scene', root_x=-123, root_y=17,
                           pending=(1, 0), parent_current_low=0xc000, root_correction=3))
    for flags in (1, 2, 3):
        result.append(make_case(f'preserved-orientation-{flags}', flags=flags, pending=(1, 0),
                               player_specs=[(-27, 0, 0x40), (0, 0, 0x40)]))
    # A pending arm is retained while the player moves from far left to the
    # platform; a premature ordinary reward consumes that player's chance.
    for chain, before, mask in (('armed-landing', False, 1), ('premature-landing', True, 1),
                                ('two-player-arm', False, 3)):
        result.append(make_case(chain + '-arm', entry=0xb9748,
            creator_x=3970 if before else 4000, mask=mask,
            player_specs=[(-40, 0, 0x40), (-40, 0, 0x40)], chain=chain))
        result.append(make_case(chain + '-land', creator_x=3970 if before else 4000,
            mask=mask, preserve=True, chain=chain, epoch=8))
        result.append(make_case(chain + '-again', mask=mask, preserve=True, chain=chain, epoch=9))
    return result


def run(args):
    out = args.output.resolve();out.mkdir(parents=True, exist_ok=False)
    for name in ('cfg', 'nvram'):(out / name).mkdir()
    rom_dir = args.rom_dir.resolve()
    with zipfile.ZipFile(rom_dir / 'mslugx.zip') as archive:raw = archive.read('250-p1.p1')
    fixed = bytearray(raw);fixed[::2], fixed[1::2] = raw[1::2], raw[::2]
    assert hashlib.sha256(fixed).hexdigest() == FIXED_SHA256, 'Unexpected supplied P1 cartridge'
    inputs = cases()
    if args.score_effects:
        for c in inputs:c['input']['score_hex']=['00000000','00000000']
    # Actor hex strings and names require normal Lua quoting. JSON is valid
    # data but not Lua syntax; serialize the small input tree explicitly.
    def to_lua(value):
        if isinstance(value, dict):return '{' + ','.join(k + '=' + to_lua(v) for k, v in value.items()) + '}'
        if isinstance(value, list):return '{' + ','.join(to_lua(v) for v in value) + '}'
        if isinstance(value, bool):return 'true' if value else 'false'
        return json.dumps(value)
    lua = ('local cases=' + to_lua(inputs) + '\nlocal output=' + json.dumps(str(out)) +
           '\nlocal score_effects=' + to_lua(args.score_effects) + '\n')
    lua += r'''
local cpu=manager.machine.devices[':maincpu'].spaces.program
local debug=manager.machine.debugger
local frame,index,pending=0,0,false
local actor,creator,scene=0x101000,0x101800,0x10e968
local data=assert(io.open(output..'/states.txt','w'))
local effects=score_effects and assert(io.open(output..'/effects.txt','w'))
local labels=score_effects and assert(io.open(output..'/labels.txt','w'))
local pre,previous
local function hex(bytes) return (bytes:gsub('.',function(c)return string.format('%02x',string.byte(c))end)) end
local function loadhex(address,bytes)
 for offset=1,#bytes,2 do cpu:write_u8(address+(offset-1)//2,tonumber(bytes:sub(offset,offset+1),16)) end
end
local function pc() return manager.machine.devices[':maincpu'].state['PC'].value end
emu.register_frame_done(function()
 frame=frame+1;if frame<2 then return end
 if pending then
  if cpu:read_u8(0x10fd00)~=1 then print(string.format('PROBE_STUCK case=%d pc=%x',index-1,pc()));manager.machine:exit();return end
  previous=hex(cpu:read_range(actor,actor+0xbf,8))
  data:write(string.format('%d %s %s\n',index-1,pre,previous));data:flush();pending=false
  if score_effects then
   local count=cpu:read_u16(0x10fd02)
   effects:write(string.format('%d %s %s %d\n',index-1,
       hex(cpu:read_range(0x10c46a,0x10c46d,8)),hex(cpu:read_range(0x10c472,0x10c475,8)),count))
   for i=0,count-1 do
    local node=0x102000+i*0xc0
    labels:write(string.format('%d %d %x %s\n',index-1,i,node,hex(cpu:read_range(node,node+0xbf,8))))
   end
   effects:flush();labels:flush()
  end
 end
 if index==#cases then
  data:close();if score_effects then effects:close();labels:close() end
  manager.machine:exit();return
 end
 local c=cases[index+1].input
 for addr=0x100000,0x10fffe,2 do cpu:write_u16(addr,0) end
 cpu:write_u8(0x300001,0)
 cpu:write_u16(0x10ff00,0x4e71);cpu:write_u16(0x10ff02,0x60fe)
 loadhex(actor,c.preserve_previous and assert(previous) or c.actor_hex)
 cpu:write_u32(actor,c.entry);loadhex(creator,c.creator_hex)
 for i=1,2 do
  local p=c.players[i];local ad=(i==1 and 0x1014c4 or 0x101574)
  cpu:write_u32(ad,p.routine);cpu:write_u16(ad+0x3c,p.x);cpu:write_u16(ad+0x40,p.y);cpu:write_u8(ad+0x15,p.flags15)
 end
 cpu:write_u32(scene+0x14,c.root.x);cpu:write_u32(scene+0x18,c.root.y)
 cpu:write_u32(scene+0x1c,c.root.prev_x);cpu:write_u32(scene+0x20,c.root.prev_y)
 cpu:write_u8(scene+0x25,c.root.correction);cpu:write_u8(0x10c33a,c.epoch)
 cpu:write_u8(0x10c33c,c.active_mask)
 if score_effects then loadhex(0x10c46a,c.score_hex[1]);loadhex(0x10c472,c.score_hex[2]) end
 cpu:write_u32(0x10fe00,0x10ff00)
 pre=hex(cpu:read_range(actor,actor+0xbf,8))
 if index==0 then
  debug:command('bpset 10ff00,1,{printf "RETURN case=%d\n",temp0;b@10fd00=1;pc=10ff02;g}')
  debug:command('bpset 4188,1,{printf "DELETE case=%d pc=%06X\n",temp0,pc;b@10fd00=1;pc=10ff02;g}')
  local request='printf "REWARD case=%d player=%d d1=%d tag=%06X w0=%04X w1=%04X x=%08X y=%08X px=%08X py=%08X\n",temp0,d0&ffff,d1&ffff,a0,w@a0,w@(a0+2),d@(a6+28),d@(a6+2c),d@(a6+3c),d@(a6+40)'
  if score_effects then
   debug:command('bpset 8cecc,1,{'..request..';g}')
   debug:command('bpset 411c,1,{printf "ALLOC case=%d index=%d reserve=%d routine=%06X creator=%06X\n",temp0,temp2,d0&ffff,a0,a1;a0=102000+temp2*c0;temp2=temp2+1;w@10fd02=temp2;pc=d@(sp);sp=sp+4;g}')
   debug:command('bpset 120f8,1,{printf "SOUND case=%d token=%04X\n",temp0,d0&ffff;pc=d@(sp);sp=sp+4;g}')
  else
   debug:command('bpset 8cecc,1,{'..request..';pc=d@(sp);sp=sp+4;g}')
  end
 end
 debug:command('temp0='..string.format('%x',index))
 debug:command('temp2=0')
 for n=0,7 do debug:command('d'..n..'=0') end
 for n=0,4 do debug:command('a'..n..'=0') end
 debug:command('a5=108000');debug:command('a6=101000');debug:command('sp=10fe00');debug:command('sr=2700')
 debug:command('pc='..string.format('%x',c.entry));debug:command('g')
 index=index+1;pending=true
end,'frame')
'''
    (out / 'capture.lua').write_text(lua)
    env = dict(os.environ, LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu',
               SDL_VIDEODRIVER='dummy', SDL_AUDIODRIVER='dummy')
    command = [str(args.mame), 'mslugx', '-rompath', str(rom_dir), '-bios', 'euro',
        '-video', 'none', '-sound', 'none', '-skip_gameinfo', '-nothrottle', '-debug',
        '-debugger', 'none', '-debuglog', '-debugscript', str(ROOT / 'tools/mame/start.cmd'),
        '-autoboot_delay', '0', '-autoboot_script', str(out / 'capture.lua'),
        '-cfg_directory', str(out / 'cfg'), '-nvram_directory', str(out / 'nvram')]
    with (out / 'mame.log').open('w') as log:
        subprocess.run(command, cwd=out, env=env, stdout=log, stderr=subprocess.STDOUT,
                       check=True, timeout=90)
    mlog=(out / 'mame.log').read_text();assert 'LUA ERROR' not in mlog and 'PROBE_STUCK' not in mlog, mlog[-3000:]
    debuglog=(out / 'debug.log').read_text()
    states=(out / 'states.txt').read_text().splitlines()
    assert len(states)==len(inputs), (len(states), len(inputs), mlog[-3000:], debuglog[-3000:])
    returned={int(i) for i in re.findall(r'RETURN case=(\d+)', debuglog)}
    deleted={int(i) for i in re.findall(r'DELETE case=(\d+)', debuglog)}
    assert returned.isdisjoint(deleted) and returned|deleted==set(range(len(inputs)))
    requests={i:[] for i in range(len(inputs))}
    pattern=r'REWARD case=(\d+) player=(\d+) d1=(\d+) tag=([0-9A-F]+) w0=([0-9A-F]+) w1=([0-9A-F]+) x=([0-9A-F]+) y=([0-9A-F]+) px=([0-9A-F]+) py=([0-9A-F]+)'
    for match in re.finditer(pattern, debuglog):
        values=match.groups();index=int(values[0])
        request=dict(player=int(values[1]), d1=int(values[2]), kind_tag=int(values[3],16),
            kind_words=[int(values[4],16),int(values[5],16)],
            actor_x=int(values[6],16),actor_y=int(values[7],16),
            projected_x=int(values[8],16),projected_y=int(values[9],16))
        requests[index].append(request)
    for index, line in enumerate(states):
        n, pre, post=line.split();assert int(n)==index and len(pre)==len(post)==384
        inputs[index]['input']['actor_hex']=pre
        inputs[index]['expected']=dict(actor_hex=post,deleted=index in deleted,requests=requests[index])
    if args.score_effects:
        all_labels={i:[] for i in range(len(inputs))}
        for line in (out/'labels.txt').read_text().splitlines():
            i, j, node, label=line.split();index=int(i)
            assert int(j)==len(all_labels[index]) and len(label)==384
            all_labels[index].append(dict(node=int(node,16),actor_hex=label))
        sounds={i:[] for i in range(len(inputs))}
        for match in re.finditer(r'SOUND case=(\d+) token=([0-9A-F]+)',debuglog):
            i, token=match.groups();assert int(token,16)==0x1121
            sounds[int(i)].append(int(token,16))
        allocations={i:[] for i in range(len(inputs))}
        for match in re.finditer(r'ALLOC case=(\d+) index=(\d+) reserve=(\d+) routine=([0-9A-F]+) creator=([0-9A-F]+)',debuglog):
            i, j, reserve, routine, creator=match.groups();index=int(i)
            assert int(j)==len(allocations[index]) and int(reserve)==14
            assert int(routine,16)==0x8d330 and int(creator,16)==0x101414
            allocations[index].append(dict(reserve=int(reserve),routine=int(routine,16),creator=int(creator,16)))
        rows=(out/'effects.txt').read_text().splitlines();assert len(rows)==len(inputs)
        for index, line in enumerate(rows):
            i, first, second, count=line.split();assert int(i)==index
            assert int(count)==len(allocations[index])==len(all_labels[index])==len(sounds[index])==len(requests[index])
            inputs[index]['expected'].update(score_hex=[first,second],sounds=sounds[index],
                labels=all_labels[index],allocations=allocations[index])
    sha=lambda path:hashlib.sha256(Path(path).read_bytes()).hexdigest()
    report=dict(status='ORIGINAL_REFERENCE_RECORDED',source='MAME0.276 supplied mslugx euro P1',
        entry_points=[0xb9748,0xb9780],actor_tag=ACTOR,creator_tag=CREATOR,scene_tag=SCENE,
        rom_cpu_order_sha256=hashlib.sha256(fixed).hexdigest(),
        source_ranges={f'{lo:06x}-{hi:06x}':hashlib.sha256(fixed[lo:hi]).hexdigest() for lo,hi in
            ((0xb9748,0xb97d8),(0xb9860,0xb99e8),(0x1ab98,0x1ac38),(0x1ad6e,0x1ad8c),
             (0x1aeb0,0x1aeda),(0x1af1a,0x1af9a),(0x409a,0x40bc))},
        cases=inputs,counts=dict(cases=len(inputs),deletion_requests=len(deleted),
            reward_requests=sum(map(len,requests.values()))),
        evidence_sha256={str(p):sha(p) for p in (out/'debug.log',out/'states.txt',out/'capture.lua')},
        limits='Injected actor/scene/player RAM; IRQ masked. Original projection, player-mask, grounded, rectangle and creator guards execute. Stops before004188 deletion; intercepts08CECC only to record request and return. Score/label/sound lifecycle, natural campaign production, global scheduler, timing and allocator side effects are not covered. Anti-tamper counter10F00C is initialized zero so its normal no-carry path reaches004188.')
    if args.score_effects:
        report['score_effects']=True
        report['counts']['labels']=sum(len(c['expected']['labels']) for c in inputs)
        report['counts']['sounds']=sum(len(c['expected']['sounds']) for c in inputs)
        for lo,hi in ((0x8cecc,0x8cf2e),(0x57948,0x5798c),(0x1acbe,0x1acdc)):
            report['source_ranges'][f'{lo:06x}-{hi:06x}']=hashlib.sha256(fixed[lo:hi]).hexdigest()
        for name in ('effects.txt','labels.txt'):
            report['evidence_sha256'][str(out/name)]=sha(out/name)
        report['limits']='Injected actor/scene/player RAM and zero initial scores; IRQ masked. Original0B9748/0B9780,08CECC,057948 BCD addition and01ACBE label placement execute. Stops before004188 deletion.00411C is a successful synthetic allocator returning a zeroed192-byte label at102000+i*C0, with reserve/routine/creator arguments verified;0120F8 is intercepted to record1121 and return. No allocator linkage/default fields, label update lifecycle, real sound queue/PCM, natural campaign production, timing or full scheduler claimed. Anti-tamper counter10F00C starts zero.'
    (out / 'validation.json').write_text(json.dumps(report,indent=2)+'\n')
    if args.fixture:
        args.fixture.parent.mkdir(parents=True,exist_ok=True)
        args.fixture.write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report['counts']))


if __name__ == '__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--rom-dir',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--fixture',type=Path)
    parser.add_argument('--mame',type=Path,default=Path('/workspace/emulator/root/usr/games/mame'))
    parser.add_argument('--score-effects',action='store_true',help='Execute actual BCD reward and label placement; stub only allocation and sound queue')
    run(parser.parse_args())
