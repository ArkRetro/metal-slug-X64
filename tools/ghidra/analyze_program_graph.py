#!/usr/bin/env python3
"""Build dependency/ABI review queues from the whole stored P1 Ghidra model.

These are model hypotheses, not a verified code partition or native coverage.
"""
import argparse,bisect,collections,csv,hashlib,json,shutil
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--graph',type=Path,required=True);p.add_argument('--input-rom',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
a.output.mkdir(parents=True,exist_ok=False)
program_set=json.loads((ROOT/'decomp/program-set.json').read_text());expected=next(x['cpu_order_sha256'] for x in program_set['programs'] if x['source']=='250-p1.p1')
assert hashlib.sha256(a.input_rom.read_bytes()).hexdigest()==expected,'Input does not match supplied CPU-order P1'
names=['function-models.tsv','model-ranges.tsv','model-edges.tsv','return-consumers.tsv','isa-review.tsv'];tables={}
for name in names:
    with (a.graph/name).open() as f:reader=csv.DictReader(f,delimiter='\t');fields=reader.fieldnames;rows=list(reader)
    assert all(set(row)==set(fields) and all(v is not None for v in row.values()) for row in rows),name
    with (a.output/name).open('w') as f:
        writer=csv.DictWriter(f,fieldnames=fields,delimiter='\t',lineterminator='\n');writer.writeheader();writer.writerows({k:v if v else '-' for k,v in row.items()} for row in rows)
    tables[name]=rows
models={r['entry']:r for r in tables['function-models.tsv']};edges=tables['model-edges.tsv'];nodes=set(models)
assert len(models)==len(tables['function-models.tsv'])
parse_regs=lambda value:set(value.split(',')) if value and value!='-' else set()
intervals=sorted((int(r['start'],16),int(r['end_inclusive'],16)+1,r['owner']) for r in tables['model-ranges.tsv']);starts=[r[0] for r in intervals]
def owner_for(pc):
    i=bisect.bisect_right(starts,pc)-1
    return intervals[i][2] if i>=0 and intervals[i][0]<=pc<intervals[i][1] else None
union=0;previous_end=0;overlaps=0
for lo,hi,owner in intervals:
    assert 0<=lo<hi<=0x100000 and owner in models
    if lo<previous_end:overlaps+=1
    union+=max(0,hi-max(previous_end,lo));previous_end=max(previous_end,hi)
direct={n:set() for n in nodes};scheduling={n:set() for n in nodes}
for e in edges:
    assert e['owner'] in nodes
    target=e['target_owner']
    if target not in nodes:continue
    if e['kind'] in ('call','tail_jump'):direct[e['owner']].add(target)
    if e['kind'] in ('call','tail_jump','state_assignment_candidate'):scheduling[e['owner']].add(target)

def components(graph):
    # Iterative Kosaraju; avoids recursion limits on candidate graphs.
    seen=set();order=[]
    for root in sorted(nodes):
        if root in seen:continue
        stack=[(root,False)]
        while stack:
            n,done=stack.pop()
            if done:order.append(n);continue
            if n in seen:continue
            seen.add(n);stack.append((n,True));stack.extend((v,False) for v in sorted(graph[n],reverse=True) if v not in seen)
    reverse={n:set() for n in nodes}
    for n,targets in graph.items():
        for target in targets:reverse[target].add(n)
    seen=set();result=[]
    for root in reversed(order):
        if root in seen:continue
        group=[];stack=[root];seen.add(root)
        while stack:
            n=stack.pop();group.append(n)
            for v in reverse[n]:
                if v not in seen:seen.add(v);stack.append(v)
        result.append(sorted(group))
    assert set().union(*(set(g) for g in result))==nodes
    return result,reverse
clusters,callers=components(direct);schedule_clusters,_=components(scheduling)
roots={f'{pc:06x}':owner_for(pc) for pc in (0x2432,0x2658,0x2800,0x27f6)}
def reachable(graph):
    seen=set();stack=[v for v in roots.values() if v]
    while stack:
        n=stack.pop()
        if n in seen:continue
        seen.add(n);stack.extend(graph[n]-seen)
    return seen
reachable_direct=reachable(direct);reachable_scheduling=reachable(scheduling)
indexed={r['owner']:r for r in csv.DictReader((ROOT/'decomp/full-p1-index.tsv').open(),delimiter='\t')}
empty=[]
for n in sorted(nodes & set(indexed)):
    row=indexed[n]
    if not row['status'].startswith('tentative'):continue
    import re
    text=(ROOT/'decomp'/row['file']).read_text()
    if re.search(r'\{\s*return\s*;\s*\}\s*$',text) and parse_regs(models[n]['register_writes']):empty.append(n)
next_uses={r['call_site']:r for r in tables['return-consumers.tsv']};flag_names={'CF','VF','ZF','NF','XF','SR'};abi=[]
for e in edges:
    if e['kind']!='call' or e['target_owner'] not in models or e['site'] not in next_uses:continue
    nxt=next_uses[e['site']];possible=parse_regs(models[e['target_owner']]['register_writes']) & parse_regs(nxt['register_reads'])
    if possible:abi.append(dict(caller=e['owner'],site=e['site'],callee=e['target_owner'],next=nxt['next'],mnemonic=nxt['mnemonic'],possible_return_registers=','.join(sorted(possible)),flags_read=','.join(sorted(possible&flag_names)) or '-'))
with (a.output/'abi-review.tsv').open('w') as f:
    fields=['caller','site','callee','next','mnemonic','possible_return_registers','flags_read'];w=csv.DictWriter(f,fieldnames=fields,delimiter='\t',lineterminator='\n');w.writeheader();w.writerows(abi)
abi_count=collections.Counter(x['callee'] for x in abi);flag_count=collections.Counter(x['callee'] for x in abi if x['flags_read']!='-')
ranked=sorted(nodes,key=lambda n:(-len(callers[n]),-flag_count[n],int(models[n]['body_bytes']),n))
with (a.output/'dependency-review.tsv').open('w') as f:
    fields=['entry','unique_direct_callers','model_body_bytes','immediate_return_use_sites','immediate_flag_use_sites','computed_sites','in_header_model_closure','index_status','review_kind'];w=csv.DictWriter(f,fieldnames=fields,delimiter='\t',lineterminator='\n');w.writeheader()
    for n in ranked:w.writerow(dict(entry=n,unique_direct_callers=len(callers[n]),model_body_bytes=models[n]['body_bytes'],immediate_return_use_sites=abi_count[n],immediate_flag_use_sites=flag_count[n],computed_sites=models[n]['computed_sites'],in_header_model_closure=int(n in reachable_scheduling),index_status=indexed.get(n,{}).get('status','not_indexed'),review_kind='MODEL_HYPOTHESIS_REQUIRES_ORIGINAL_REFERENCE'))
summary=dict(status='WHOLE_STORED_P1_MODEL_ANALYSIS_NOT_VERIFIED_CODE',input_cpu_order_sha256=expected,language='68000:BE:32:MC68020',canonical_processor='MAME M68000 must validate candidate instructions; Ghidra language is a superset',models=len(nodes),model_edge_rows=len(edges),edge_kinds=dict(collections.Counter(e['kind'] for e in edges)),model_ranges=len(intervals),overlapping_model_ranges=overlaps,modeled_body_bytes=union,p1_bytes_outside_model_bodies=0x100000-union,code_data_partition_verified=False,header_targets_to_model_owners=roots,header_direct_model_closure=len(reachable_direct),header_scheduling_model_closure=len(reachable_scheduling),direct_sccs=len(clusters),largest_direct_scc=max(map(len,clusters)),scheduling_sccs=len(schedule_clusters),largest_scheduling_scc=max(map(len,schedule_clusters)),model_functions_without_index=sorted(nodes-set(indexed)),indexed_addresses_without_model=sorted(set(indexed)-nodes),empty_automatic_c_with_model_register_writes=empty,immediate_possible_live_return_sites=len(abi),immediate_possible_live_flag_sites=sum(x['flags_read']!='-' for x in abi),later_isa_review_sites=len(tables['isa-review.tsv']),top_dependency_models=[dict(entry=n,unique_callers=len(callers[n]),model_bytes=int(models[n]['body_bytes']),immediate_flag_sites=flag_count[n]) for n in ranked[:20]],largest_direct_clusters=sorted(clusters,key=lambda c:(-len(c),c))[:10],files={name:hashlib.sha256((a.output/name).read_bytes()).hexdigest() for name in names+['abi-review.tsv','dependency-review.tsv']},limits='Candidate ownership/edges/ISA come from stored Ghidra analysis and may decode data or omit indirect reachability. Raw p-code reads/writes are not complete calling conventions or liveness proofs. Empty C plus register writes is a review signal, not proof of a bug. Model byte coverage and header closure are NOT recovery percentages. P2/Z80/BIOS/native overlays are not part of this P1 graph.')
(a.output/'summary.json').write_text(json.dumps(summary,indent=2)+'\n')
print(json.dumps({k:summary[k] for k in ('models','model_edge_rows','immediate_possible_live_return_sites','immediate_possible_live_flag_sites','later_isa_review_sites','header_scheduling_model_closure')}))
