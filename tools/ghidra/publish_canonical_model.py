#!/usr/bin/env python3
"""Publish verified context/model structure, never promoting candidate code to C."""
import argparse,csv,hashlib,json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--model',type=Path,required=True);p.add_argument('--output',type=Path,default=ROOT/'decomp/canonical-model');a=p.parse_args()
public=a.output;layout=json.loads((public/'layout.json').read_text());report=json.loads((a.model/'validation.json').read_text())
assert report['layout_sha256']==hashlib.sha256((public/'layout.json').read_bytes()).hexdigest()
expected_contexts={c['id']:c for c in layout['contexts']};roots=[];programs=[]
for program in report['programs']:
 name=program['program'];out=public/name;out.mkdir(exist_ok=False)
 for filename,digest in program['files'].items():
  source=a.model/name/filename;assert hashlib.sha256(source.read_bytes()).hexdigest()==digest
  with source.open() as f:r=csv.DictReader(f,delimiter='\t');fields=r.fieldnames;rows=list(r)
  assert all(set(row)==set(fields) and all(value is not None for value in row.values()) for row in rows)
  with (out/filename).open('w') as f:w=csv.DictWriter(f,fieldnames=fields,delimiter='\t',lineterminator='\n');w.writeheader();w.writerows({k:v if v else '-' for k,v in row.items()} for row in rows)
 program_roots=list(csv.DictReader((out/'roots.tsv').open(),delimiter='\t'))
 for r in program_roots:
  assert r['context'] in expected_contexts and r['instruction_present']=='true'
  assert r['location'].startswith(r['context']+':') and r['owner'].startswith(r['context']+':')
 roots+=program_roots
 programs.append(dict(program=name,processor=program['processor'],roots=len(program_roots),candidate_functions=program['models'],files={filename:hashlib.sha256((out/filename).read_bytes()).hexdigest() for filename in program['files']}))
main_memory=list(csv.DictReader((public/'M68K_FIXED_P1/memory.tsv').open(),delimiter='\t'));memory={r['context']:r for r in main_memory}
for c in layout['contexts']:
 if c['cpu']!='m68000':continue
 r=memory[c['id']];assert int(r['start'],16)==c['cpu_base'] and int(r['end_inclusive'],16)+1-c['cpu_base']==c['size']
 if c['id']!='M68K_FIXED_P1':assert r['address_space']==c['id'],'Overlay contexts must retain identity'
 if c['id'].startswith('M68K_P2_BANK_'):assert r['executable']=='false','Unproven P2 code must not become seeded executable'
assert {int(r['pc'],16) for r in roots if r['context']=='M68K_FIXED_P1'} >= {0x22a2,0x22ac,0x2322}
assert {(r['context'],int(r['pc'],16)) for r in roots} >= {('RAM_COPY_100000',0x100000),('RAM_COPY_10FC00',0x10fc00)}
summary=dict(status='STRUCTURAL_CONTEXT_MODEL_PASS_SEMANTICS_OPEN',layout_sha256=report['layout_sha256'],mapping_validation_sha256=hashlib.sha256((public/'mapping-validation.json').read_bytes()).hexdigest(),contexts=len(layout['contexts']),seeded_context_roots=len(roots),programs=programs,known_vector_roots_seeded=True,known_ram_copy_roots_seeded=True,initial_z80_cart_roots_seeded=True,p2_code_data_partition_verified=False,ram_copy_native_domains_closed=False,all_runtime_z80_contexts_recovered=False,whole_program_decompiled=False,limits=report['limits'])
(public/'model-validation.json').write_text(json.dumps(summary,indent=2)+'\n')
(public/'context-roots.json').write_text(json.dumps(dict(status='MODEL_ROOTS_ONLY_NOT_NATIVE_RECOVERY',layout_sha256=report['layout_sha256'],roots=roots),indent=2)+'\n')
print('PASS: 9 distinct source contexts,',len(roots),'seeded roots, P2 code/data unclassified; no native-completion claim')
