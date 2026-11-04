#!/usr/bin/env python3
"""Reconcile candidate P1 graph and native subsets without changing verification state."""
import argparse,bisect,csv,hashlib,json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--graph',type=Path,default=ROOT/'decomp/analysis/p1-program-model');p.add_argument('--output',type=Path,default=ROOT/'decomp/analysis');a=p.parse_args();a.output.mkdir(parents=True,exist_ok=True)
registry=json.loads((ROOT/'decomp/native-components.json').read_text());models={r['entry']:r for r in csv.DictReader((a.graph/'function-models.tsv').open(),delimiter='\t')};priority=list(csv.DictReader((a.graph/'dependency-review.tsv').open(),delimiter='\t'))
ranges=sorted((int(r['start'],16),int(r['end_inclusive'],16)+1,r['owner']) for r in csv.DictReader((a.graph/'model-ranges.tsv').open(),delimiter='\t'));starts=[r[0] for r in ranges]
def owner(entry):
 pc=int(entry,16);i=bisect.bisect_right(starts,pc)-1
 return ranges[i][2] if i>=0 and ranges[i][0]<=pc<ranges[i][1] else '-'
rows=[];by_model={};context_rows=[]
layout=json.loads((ROOT/'decomp/canonical-model/layout.json').read_text())
contexts={c['id']:c for c in layout['contexts']}
context_report=json.loads((ROOT/'decomp/canonical-model/model-validation.json').read_text())
context_owners={}
for program in context_report['programs']:
 for row in csv.DictReader((ROOT/'decomp/canonical-model'/program['program']/'instructions.tsv').open(),delimiter='\t'):
  context_owners[row['location']]=row['owner']
for c in registry['components']:
 for name in c['sources']+c['evidence']:assert (ROOT/name).is_file(),name
 for symbol in c['symbols']:assert any(symbol in (ROOT/name).read_text() for name in c['sources']),symbol
 for entry in c['entries']:
  model=owner(entry);rows.append(dict(entry=entry,model_owner=model,component=c['id'],native_state=c['state'],scope=c['scope']))
  if model!='-':by_model.setdefault(model,set()).add(c['id'])
 for entry in c.get('execution_entries',[]):
  context=contexts[entry['mapping']]
  assert context['cpu']==entry['cpu'] and context['sha256']==entry['code_sha256']
  location=entry['mapping']+':'+f'{int(entry["pc"],16):08x}'
  context_rows.append(dict(cpu=entry['cpu'],mapping=entry['mapping'],pc=entry['pc'],model_owner=context_owners.get(location,'-'),component=c['id'],native_state=c['state'],scope=c['scope']))
with (a.output/'native-model-overlay.tsv').open('w') as f:
 fields=['entry','model_owner','component','native_state','scope'];w=csv.DictWriter(f,fieldnames=fields,delimiter='\t',lineterminator='\n');w.writeheader();w.writerows(rows)
with (a.output/'native-context-overlay.tsv').open('w') as f:
 fields=['cpu','mapping','pc','model_owner','component','native_state','scope'];w=csv.DictWriter(f,fieldnames=fields,delimiter='\t',lineterminator='\n');w.writeheader();w.writerows(context_rows)
work=[]
for row in priority[:40]:
 model=row['entry'];components=sorted(by_model.get(model,set()))
 work.append(dict(entry=model,modeled_unique_callers=int(row['unique_direct_callers']),model_bytes=int(row['model_body_bytes']),immediate_flag_sites=int(row['immediate_flag_use_sites']),native_subsets=components,next_action='Audit and extend existing native subset/domain' if components else 'Identify original contract, then recover or locate an unregistered native implementation',status='MODEL_PRIORITY_REQUIRES_RUNTIME_CORROBORATION'))
report=dict(status='NATIVE_AND_CANDIDATE_RECONCILIATION',registry_exhaustive=registry['exhaustive'],components=len(registry['components']),associated_entries=len(rows),associated_model_owners=len(by_model),priority_candidates=work,registry_sha256=hashlib.sha256((ROOT/'decomp/native-components.json').read_bytes()).hexdigest(),graph_summary_sha256=hashlib.sha256((a.graph/'summary.json').read_bytes()).hexdigest(),limits='Graph caller counts are model edges, not execution frequency. Native components have bounded evidence and are not complete-function proofs. Missing overlay is not proof that no implementation exists; this registry is intentionally incomplete. No progress percentage is calculated.')
report['associated_context_entries']=len(context_rows)
report['context_model_sha256']=hashlib.sha256((ROOT/'decomp/canonical-model/model-validation.json').read_bytes()).hexdigest()
(a.output/'recovery-worklist.json').write_text(json.dumps(report,indent=2)+'\n');print('Reconciled',len(rows),'entries across',len(by_model),'model owners;',len(work),'candidate priorities')
