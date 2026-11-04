#!/usr/bin/env python3
"""Inventory cartridge vectors and two verified static code-copy paths.

Copied containers can include data. Mapping context is part of identity;
no RAM execution coverage or whole code/data partition is inferred.
"""
import argparse,bisect,csv,hashlib,json,struct
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--input-rom',type=Path,required=True);p.add_argument('--ram-snapshot',type=Path);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
b=a.input_rom.read_bytes();expected=json.loads((ROOT/'decomp/program-set.json').read_text())['programs'][0]['cpu_order_sha256'];assert hashlib.sha256(b).hexdigest()==expected
ranges=sorted((int(x['start'],16),int(x['end_inclusive'],16)+1,x['owner']) for x in csv.DictReader((ROOT/'decomp/analysis/p1-program-model/model-ranges.tsv').open(),delimiter='\t'));starts=[x[0] for x in ranges]
def owner(pc):
 i=bisect.bisect_right(starts,pc)-1
 return ranges[i][2] if i>=0 and ranges[i][0]<=pc<ranges[i][1] else None
vectors=[]
for index in range(1,64):
 target=struct.unpack_from('>I',b,index*4)[0]
 vectors.append(dict(vector=index,table_offset=f'0x{index*4:06X}',target=f'0x{target:08X}',mapping='fixed_P1' if 0x100<=target<0x100000 else 'BIOS_import' if 0xc00000<=target<0xc20000 else 'unclassified_or_reserved',model_owner=owner(target) if 0x100<=target<0x100000 else None))
# Exact original transfer-loop operands, not guessed disassembler references.
assert b[0x1024:0x102a]==bytes.fromhex('41fa01324e71')
assert b[0x102a:0x1034]==bytes.fromhex('43f900100000303c08a4')
assert b[0x103a:0x1046]==bytes.fromhex('32d851c8fff64eb900100000')
source1=0x1026+int.from_bytes(b[0x1026:0x1028],'big',signed=True)
size1=(int.from_bytes(b[0x1032:0x1034],'big')+1)*2
assert b[0x10a8:0x10ae]==bytes.fromhex('4bf900108000')
assert b[0x10da:0x10ee]==bytes.fromhex('45fa001247ed7c0036d20c5a4e7566f84eed7c00')
source2=0x10dc+int.from_bytes(b[0x10dc:0x10de],'big',signed=True)
end2=next(i+2 for i in range(source2,len(b)-1,2) if b[i:i+2]==b'\x4e\x75')
ram2=0x108000+int.from_bytes(b[0x10e0:0x10e2],'big',signed=True)
copies=[]
for tag,source,size,destination,site in [('word_count_copy',source1,size1,0x100000,0x1040),('copy_through_first_RTS_word',source2,end2-source2,ram2,0x10ea)]:
 copies.append(dict(id=tag,cpu='m68000',source_mapping='fixed_P1',source_offset=f'0x{source:06X}',container_bytes=size,source_bytes_sha256=hashlib.sha256(b[source:source+size]).hexdigest(),execution_mapping=f'RAM_COPY_{destination:06X}',runtime_pc=f'0x{destination:06X}',transfer_site=f'0x{site:06X}',source_model_owner=owner(source),status='STATIC_COPY_AND_CONTROL_TRANSFER_PATH_VERIFIED',limits='Container may contain code and embedded data; decode at runtime base and preserve lifetime/relocation context. Dynamic entry execution and domain completeness not established.'))
snapshot=None
if a.ram_snapshot:
 ram=a.ram_snapshot.read_bytes();assert len(ram)==65536
 observed=ram[ram2-0x100000:ram2-0x100000+end2-source2]
 assert observed==b[source2:end2]
 snapshot=dict(bytes=len(ram),sha256=hashlib.sha256(ram).hexdigest(),runtime_range_start=f'0x{ram2:06X}',matched_bytes=len(observed),matched_sha256=hashlib.sha256(observed).hexdigest(),scope='Existing frame-2360 RAM snapshot equality only; harness context and actual execution are not certified by byte equality')
internal=sorted({v['target'] for v in vectors if v['mapping']=='fixed_P1'})
report=dict(status='STATIC_ROOTS_AND_RAM_COPY_CONTEXTS_NOT_GLOBAL_COVERAGE',input_cpu_order_sha256=expected,cartridge_vectors=vectors,distinct_internal_vector_targets=internal,unmodeled_internal_vectors=[pc for pc in internal if owner(int(pc,16)) is None],runtime_copy_paths=copies,ram_presence_observation=snapshot,location_identity=['cpu','mapping_or_bank','runtime_pc','source_image_offset','copy_version_or_lifetime'],limits='Cartridge vector table may be hidden by BIOS vector selection at a given time. Physical P1 offsets and RAM execution addresses are different locations. Never flatten RAM copies, P2 bank windows or Z80 bank states into one PC-only map. These observations do not prove all bytes executable or a completed recovery.')
a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('Unmodeled vector targets:',report['unmodeled_internal_vectors']);print('Code-copy containers:',[(c['source_offset'],c['runtime_pc'],c['container_bytes']) for c in copies])
