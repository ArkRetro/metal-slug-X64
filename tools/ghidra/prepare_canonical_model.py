#!/usr/bin/env python3
"""Prepare versioned CPU contexts; do not flatten banks, mirrors or RAM copies."""
import argparse,csv,hashlib,json,zipfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--inputs',type=Path,required=True);p.add_argument('--bios-zip',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--layout',type=Path,required=True);a=p.parse_args()
a.output.mkdir(parents=True,exist_ok=False)
manifest=json.loads((ROOT/'decomp/program-set.json').read_text());proof=json.loads((ROOT/'decomp/canonical-model/mapping-validation.json').read_text());assert proof['status']=='PASS_CONTROLLED_CPU_MAPPING_COMPARISON'
files={'250-p1.p1':'m68k-p1.bin','250-p2.ep1':'m68k-p2-physical.bin','250-m1.m1':'z80-m1-physical.bin'};images={}
for image in manifest['programs']:
 b=(a.inputs/files[image['source']]).read_bytes();assert len(b)==image['size'] and hashlib.sha256(b).hexdigest()==image['cpu_order_sha256'];assert proof['inputs'][image['source']]==image['source_sha1'];images[image['source']]=b
with zipfile.ZipFile(a.bios_zip) as z:sm1=z.read('sm1.sm1')
assert hashlib.sha1(sm1).hexdigest()==proof['audio_bios_sha1']
p1,p2,m1=[images[name] for name in files]
contexts=[];jobs=[]
def add(name,cpu,base,data,source_pieces,executable,roots=(),lifetime='immutable_image'):
 filename=name.lower()+'.bin';(a.output/filename).write_bytes(data)
 contexts.append(dict(id=name,cpu=cpu,cpu_base=base,size=len(data),source_pieces=source_pieces,sha256=hashlib.sha256(data).hexdigest(),initial_analysis_executable=executable,classification='unverified_internal_code_data',root_pcs=list(roots),lifetime=lifetime))
 jobs.append(dict(context=name,cpu=cpu,base=f'{base:x}',size=f'{len(data):x}',file=filename,execute=str(int(executable)),roots=','.join(f'{n:x}' for n in roots) or '-'))
vector_targets=sorted({int.from_bytes(p1[i:i+4],'big') for i in range(4,0x100,4)});roots={x for x in vector_targets if 0x100<=x<0x100000 and not x%2}
header=[]
for at in range(0x100,0x200,2):
 if p1[at:at+2]==bytes.fromhex('4ef9'):
  dest=int.from_bytes(p1[at+2:at+6],'big')
  if 0x100<=dest<0x100000 and not dest%2:roots.add(dest);header.append(dict(site=at,target=dest))
roots.update((0x1000,0x1004,0x1070))
add('M68K_FIXED_P1','m68000',0,p1,[dict(image='250-p1.p1',offset=0,size=len(p1))],True,sorted(roots))
for bank in range(4):add(f'M68K_P2_BANK_{bank}','m68000',0x200000,p2[bank*0x100000:(bank+1)*0x100000],[dict(image='250-p2.ep1',offset=bank*0x100000,size=0x100000)],False)
copy_roots=json.loads((ROOT/'decomp/analysis/program-roots.json').read_text())
for c in copy_roots['runtime_copy_paths']:
 offset=int(c['source_offset'],16);base=int(c['runtime_pc'],16);size=c['container_bytes'];data=p1[offset:offset+size];assert hashlib.sha256(data).hexdigest()==c['source_bytes_sha256']
 add(c['execution_mapping'],'m68000',base,data,[dict(image='250-p1.p1',offset=offset,size=size)],True,[base],lifetime='initial_post_copy_image; subsequent RAM writes and alternate versions unresolved')
for name,fixed,source in [('Z80_INITIAL_CART',m1[:0x8000],'250-m1.m1'),('Z80_INITIAL_SYSTEM',sm1[:0x8000],'BIOS:sm1.sm1')]:
 add(name,'z80',0,fixed+m1[0x8000:0xf800],[dict(image=source,offset=0,size=0x8000),dict(image='250-m1.m1',offset=0x8000,size=0x7800)],name=='Z80_INITIAL_CART',[0,0x38,0x66] if name=='Z80_INITIAL_CART' else (),lifetime='initial bank selectors F000=1E,E000=0E,C000=06,8000=02; other full map states selected by rules')
with (a.output/'contexts.tsv').open('w') as f:
 w=csv.DictWriter(f,fieldnames=['context','cpu','base','size','file','execute','roots'],delimiter='\t',lineterminator='\n');w.writeheader();w.writerows(jobs)
rules=[dict(id='main_vectors',cpu='m68000',start=0,end_inclusive=0x7f,selector='use_cart_vectors',sources=['BIOS:c00000+offset','250-p1.p1:offset'],control_addresses=[0x3a0003,0x3a0013],verified_modes=2,note='Only first 128 bytes switch; vector table 0x80..0xFF remains P1'),dict(id='main_p2',cpu='m68000',start=0x200000,end_inclusive=0x2fffff,access=['read','execute'],selector='data & 7; values 4..7 select P2 bank 0',selector_write_start=0x2ffff0,selector_write_end=0x2fffff,read_overrides=[dict(start=0x2fffe0,end_inclusive=0x2fffef,device='MSLUGX_protection',semantics='not_recovered')],verified_selectors=8),dict(id='main_work_ram',cpu='m68000',start=0x100000,end_inclusive=0x1fffff,physical_size=0x10000,mirror_mask=0x0f0000,note='Mutable data and runtime code versions; aliases retain execution-PC context'),dict(id='z80_fixed',cpu='z80',start=0,end_inclusive=0x7fff,selector='use_cart_audio',sources=['BIOS:sm1.sm1','250-m1.m1'],control_addresses_main_cpu=[0x3a000b,0x3a001b],verified_modes=2)]
for region,(port,base,size) in enumerate([(8,0xf000,0x800),(9,0xe000,0x1000),(10,0xc000,0x2000),(11,0x8000,0x4000)]):
 rules.append(dict(id=f'z80_window_{base:04x}',cpu='z80',start=base,end_inclusive=base+size-1,source='250-m1.m1',selector_io_port=port,port_low_byte_mirror_mask=0xf0,selector_value='IO port upper byte',source_offset_expression=f'(selector << {11+region}) & 0x1FFFF',verified_selector_values=256,unique_source_pages=len(m1)//size))
rules.append(dict(id='z80_work_ram',cpu='z80',start=0xf800,end_inclusive=0xffff,classification='mutable; executable use unresolved'))
layout=dict(schema_version=1,status='STRUCTURAL_CPU_CONTEXTS_PREPARED_NOT_CODE_DATA_CLOSED',input_programs=manifest['programs'],platform_imports=[dict(id='BIOS:sm1.sm1',size=len(sm1),sha1=hashlib.sha1(sm1).hexdigest(),role='Platform audio BIOS; not game M1 source')],location_identity=['cpu','full_mapping_state','runtime_pc','source_image_offset','code_version_or_lifetime'],contexts=contexts,mapping_rules=rules,header_jump_roots=header,cartridge_internal_vector_targets=[x for x in vector_targets if 0x100<=x<0x100000],z80_full_map_identity=['fixed_source','selector_8000','selector_C000','selector_E000','selector_F000','RAM_code_version'],mapping_validation='decomp/canonical-model/mapping-validation.json',limits='Verified mapping byte views do not classify code/data or prove execution. Z80 contexts are two initial full maps, not all runtime selector tuples. P2 banks remain non-executable analysis containers until code evidence. RAM contexts are only initial copy images; writes/lifetimes and native semantics remain unresolved. BIOS/protection hardware contracts must be recovered separately.')
a.layout.parent.mkdir(parents=True,exist_ok=True);a.layout.write_text(json.dumps(layout,indent=2)+'\n');(a.output/'layout.json').write_text(json.dumps(layout,indent=2)+'\n');print('Prepared',len(contexts),'contexts; fixed P1 roots:',[hex(x) for x in sorted(roots)])
