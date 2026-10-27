#!/usr/bin/env python3
"""Correlate actual MAME watchpoints, CPU trace, frame RAM, SCBs and Ghidra.

Only assertions supported by this capture are emitted as verified. Functional
state names and complete actor-table bounds deliberately remain unknown.
"""
import argparse,csv,hashlib,json,re,struct,sys
from pathlib import Path
from PIL import Image
from export_marco import reconstruct
from neo_scene import columns
ROOT=Path(__file__).resolve().parents[1]
SLOT=0x101f14

def hex32(value):return f'0x{value:08x}'
def signed16(value):return value if value<0x8000 else value-0x10000

def animation_frames(bank,address):
    cursor=address;frames=[]
    for _ in range(64):
        offset=cursor-0x200000;opcode=bank[offset]
        if opcode==0x20:
            assert bank[offset+1]==0x50;cursor+=6
        elif opcode==4:
            mode,dx,dy=bank[offset+1:offset+4]
            assert mode==0x40 and bank[offset+8]==0
            frames.append(dict(entry=cursor,descriptor=struct.unpack_from('>I',bank,offset+4)[0],dx=dx,dy=dy))
            cursor+=10
        elif opcode==8:return frames
        else:raise AssertionError(('Unsupported animation command',hex(cursor),opcode))
    raise AssertionError('Animation did not terminate')

def descriptor_cells(physical,address,origin_x,origin_y):
    at=address-0x100000;groups,attr=physical[at:at+2];at+=2;result=[]
    for _ in range(groups):
        dx,dy,width,height=struct.unpack_from('>hhBB',physical,at);at+=6
        for col in range(width):
            for row in range(height):
                code=struct.unpack_from('>H',physical,at)[0]|(attr<<12);at+=2
                result.append((origin_x+dx+col*16,origin_y-dy+row*16,code,attr))
    return result

def verify(captures,ghidra):
    debugger=(captures/'debug.log').read_text();trace=(captures/'instructions.log').read_text()
    assert 'MAME debugger version 0.276' in debugger,'This experiment is pinned to MAME 0.276'
    assert 'Error in command' not in debugger and 'unknown command' not in debugger
    assert 'CONSTRUCTOR_WRITE frame=2351' in debugger
    for target in ('0AF17E','0AF65A','0AF6C0'):
        assert re.search(r'A6=00101F14 .* '+target+r':',trace),target
    for address,text in (('004246','movea.l (A6), A0'),('004248','jsr     (A0)'),('0AF2B2','move.l  #$af65a, (A6)'),('0AF6BA','move.l  #$af6c0, (A6)')):
        assert any(address+':' in line and text in line for line in trace.splitlines()),address
    instructions=list(csv.DictReader((ghidra/'instructions.tsv').open(),delimiter='\t'))
    for address in ('00004246','00004248','000af2b2','000af6ba'):
        assert any(row['address']==address for row in instructions),address
    paths=sorted(captures.glob('ram*.bin'),key=lambda p:int(p.stem[3:]))
    assert [int(p.stem[3:]) for p in paths]==list(range(2100,2461))
    snapshots={int(p.stem[3:]):p.read_bytes() for p in paths}
    assert all(len(r)==65536 for r in snapshots.values())
    provenance=json.loads((captures/'inputs-provenance.json').read_text())
    for name,entry in provenance['files'].items():
        assert hashlib.sha256((captures/name).read_bytes()).hexdigest()==entry['sha256'],('Changed evidence file',name)
    series=hashlib.sha256()
    for frame,ram in sorted(snapshots.items()):series.update(frame.to_bytes(4,'big'));series.update(ram)
    transitions=[];last=None;motion=[];previous=None
    for frame in range(2352,2461):
        ram=snapshots[frame];o=SLOT-0x100000
        routine=struct.unpack_from('>I',ram,o)[0]
        if routine!=last:transitions.append(dict(first_snapshot=frame,routine=hex32(routine)));last=routine
        x,y,vx=struct.unpack_from('>i',ram,o+0x28)[0],struct.unpack_from('>i',ram,o+0x2c)[0],struct.unpack_from('>i',ram,o+0x30)[0]
        if previous is not None and x!=previous:
            assert x-previous==vx,(frame,'X delta differs from stored VX')
            motion.append(frame)
        previous=x
    before=snapshots[2352];after=snapshots[2354];o=SLOT-0x100000
    def register_before(pc):
        match=re.search(r'A6=00101F14 A0=[0-9A-F]+ D0=([0-9A-F]+) '+pc+r':',trace)
        assert match,pc
        return int(match.group(1),16)
    rng0=register_before('0AF670')&0xffff;rng1=register_before('0AF67C')&0xffff
    fixture=dict(source='MAME debugger trace + post-frame RAM, euro BIOS',slot=hex32(SLOT),
        rng0_low16=hex(rng0),rng1_low16=hex(rng1),flags15=hex(before[o+0x15]),
        field92=hex(struct.unpack_from('>H',before,o+0x92)[0]),
        expected_signed_62=struct.unpack_from('>i',after,o+0x62)[0],
        expected_word_62=hex32(struct.unpack_from('>I',after,o+0x62)[0]),
        expected_word_26=hex(struct.unpack_from('>H',after,o+0x26)[0]),
        trace_sha256=hashlib.sha256((captures/'instructions.log').read_bytes()).hexdigest(),
        ram_before_frame=2352,ram_after_frame=2354)
    # Resolve displayed descriptors by SCB identity, not by guessing that the
    # current stream pointer is the already-presented frame.
    physical=(captures/'rom-physical-bank1.bin').read_bytes();bank=(captures/'program-bank.bin').read_bytes()
    poses=animation_frames(bank,0x23d510);crom=(ROOT/'filesystem/sprites.bin').read_bytes();visual=[]
    for frame in (2380,2400):
        ram=snapshots[frame];v=struct.unpack('>34304H',(captures/f'vram{frame}.bin').read_bytes())
        palette=struct.unpack('>4096H',(captures/f'palette{frame}.bin').read_bytes())
        assert ram[o+0x4d]==1 and not ram[o+0x15]&1,'Unverified display transform'
        origin_x=struct.unpack_from('>h',ram,o+0x44)[0]-1
        origin_y=(512-struct.unpack_from('>h',ram,o+0x48)[0])&511
        pal=ram[o+0x14];actual=[]
        for col in columns(v):
            slot=col['slot']
            for row in range(col['rows']):
                low,attr=v[slot*64+row*2:slot*64+row*2+2]
                if attr>>8==pal:actual.append((col['x'],(col['y']+row*16)&511,low|((attr<<12)&0xf0000),attr&255))
        candidates=[pose for pose in poses if sorted(descriptor_cells(physical,pose['descriptor'],origin_x+pose['dx'],origin_y-pose['dy']))==sorted(actual)]
        assert len(candidates)==1,(frame,'No unique descriptor/SCB correspondence',len(candidates))
        image,_=reconstruct(v,palette,crom,pal);reference=Image.open(captures/f'game{frame}.png').convert('RGB')
        matched=0
        for y in range(16,240):
            for x in range(8,312):
                color=image.getpixel((x,y))
                if color[3]:
                    assert max(abs(a-b) for a,b in zip(color[:3],reference.getpixel((x,y-16))))<=1,(frame,x,y)
                    matched+=1
        visual.append(dict(frame=frame,actor_slot=hex32(SLOT),palette=hex(pal),
            displayed_descriptor_physical_rom=hex32(candidates[0]['descriptor']),
            displayed_animation_entry_cpu=hex32(candidates[0]['entry']),
            ram_animation_pointer=hex32(struct.unpack_from('>I',ram,o+0x20)[0]),
            matching_scb_cells=len(actual),matched_mame_pixels=matched))
    report=dict(status='VERIFICADO en esta captura; no identificación universal de todos los soldados',
        tools=dict(mame='0.276, actual -debug / -debugger none / wpset / bpset / trace',ghidra='11.4.2 headless, analysis script completed',
            language='68000:BE:32:MC68020',language_limit='Packaged 68000-family decoder is an MC68020 superset; executed instruction addresses are cross-checked against the actual M68000 MAME trace.'),
        frame_convention='RAM is saved after frame_done; debugger temp9 is the last completed frame. Event at temp9=2351 is first seen in RAM2352.',
        verified=dict(slot=hex32(SLOT),constructor=hex32(0xaf17e),initial_state=hex32(0xaf65a),next_state=hex32(0xaf6c0),
            indirect_dispatch=[hex32(0x4246),hex32(0x4248)],routine_stores=[hex32(0xaf2b2),hex32(0xaf6ba)],
            routine_offset=0,links=dict(previous_offset='0x04',next_offset='0x08',parent_offset='0x0c'),
            ram_frames=len(paths),ram_snapshot_bytes=65536,observed_transitions=transitions,
            x_delta_matches_stored_vx_samples=len(motion),visual_identity=visual),
        hypotheses_not_verified=['Functional names for every state (pursue/throw/die) require controlled input/damage traces.',
            'Complete actor-pool bounds, actor count and universal stride are not established.',
            'Do not assume the vehicle health offset has the same semantics for this actor.',
            'Ghidra decompiler types, inferred parameters, carry-flag logic and pointer-looking fixed-point constants are tentative.',
            'Complete AI and native RNG call scheduling remain incomplete; separately verified RNG work is documented in docs/SOLDIER_RNG.md.'],
        previous_provisional_runtime='The existing authored three-guard positions, health, firing cadence and checkpoint rules are not verified original soldier logic.',
        ram_series_sha256=series.hexdigest(),
        ghidra_artifacts={name:hashlib.sha256((ghidra/name).read_bytes()).hexdigest()
            for name in ('headless.log','instructions.tsv','functions.tsv','memory-map.tsv')},
        inputs=provenance)
    return report,fixture

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('captures',type=Path);p.add_argument('ghidra',type=Path)
    p.add_argument('--output',type=Path,required=True);p.add_argument('--fixture',type=Path,required=True);a=p.parse_args()
    report,fixture=verify(a.captures,a.ghidra)
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n')
    a.fixture.parent.mkdir(parents=True,exist_ok=True);a.fixture.write_text(json.dumps(fixture,indent=2)+'\n')
    print('PASS: actual watchpoint/dispatch chain, 361 RAM frames,',report['verified']['x_delta_matches_stored_vx_samples'],'motion samples and',sum(v['matched_mame_pixels'] for v in report['verified']['visual_identity']),'independent MAME sprite pixels')
