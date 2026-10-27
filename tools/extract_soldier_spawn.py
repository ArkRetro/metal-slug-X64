#!/usr/bin/env python3
"""Verify the bounded level-data record actually copied into the first soldier."""
import argparse,csv,hashlib,json,struct
from pathlib import Path
from extract_soldier_rng import values

def extract(capture,ghidra):
    log=(capture/'debug.log').read_text();assert 'MAME debugger version 0.276' in log and 'Error in command' not in log
    found=[values(l) for l in log.splitlines() if l.startswith('SPAWN_RECORD ')]
    copied=[values(l) for l in log.splitlines() if l.startswith('SPAWN_COPY_OUT ')]
    assert len(found)==len(copied)==1
    a,b=found[0],copied[0];assert a['frame']==b['frame']==2351 and a['script']==b['script']==0x2d631a
    assert a['a0']==b['a0']==0x101f14 and a['a6']==b['a6']==0x101414
    record=(capture/'program-bank.bin').read_bytes()[a['script']-0x200000:a['script']-0x200000+22]
    assert int.from_bytes(record,'big')==a['words'] and record[:2]==bytes([7,22])
    words=list(struct.unpack('>11H',record));fields={f'+0x{offset:02X}':b[f'f{offset:x}'] for offset in range(0x6e,0x7a,2)}
    assert list(fields.values())==words[5:]
    addresses={int(r['address'],16) for r in csv.DictReader((ghidra/'instructions.tsv').open(),delimiter='\t')}
    assert {0x1a056,0x1a134,0x1a14e,0x1a17c,0x1a184,0x1a188,0x1a1e4}<=addresses
    digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
    return dict(source='MAME 0.276 probes at 01A134 and 01A152, checked against captured CPU ROM and Ghidra',
        frame=2351,actor_ram='0x101F14',parent_actor_ram='0x101414',record_cpu_rom='0x2D631A',opcode=7,length=22,
        unassigned_word4=words[2],coordinate_words=dict(x=words[3],y=words[4]),copied_fields=fields,
        record_words=words,next_record_cpu_rom=hex(a['script']+len(record)),
        provenance=dict(capture_sha256={n:digest(capture/n) for n in ('debug.log','run-command.json','capture-options.json','program-bank.bin')},
            ghidra_assembly_sha256=digest(ghidra/'instructions.tsv'),language='68000:BE:32:MC68020'),
        limits=['This identifies one executed spawn record, not the complete wave script.',
            'Word at record +4 retains no invented semantic name.',
            'Original script scheduling, difficulty filters, allocator and all other mission events remain incomplete.'])
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('ghidra',type=Path);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();f=extract(a.capture,a.ghidra);a.output.write_text(json.dumps(f,indent=2)+'\n');print('Verified spawn record 0x2D631A and all six copied actor words')
