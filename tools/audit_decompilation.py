#!/usr/bin/env python3
"""Check necessary conditions for a complete C/MIPS recovery; fail while incomplete.

Counts and successful unit fixtures cannot certify a whole-game recovery.
This audit checks independent structural evidence, not just coverage.complete.
"""
import argparse,collections,csv,hashlib,json,re
from pathlib import Path
from ghidra.audit_canonical_model import inspect as inspect_context_model
from mame.audit_context_decoders import inspect as inspect_decoder_evidence
from mame.catalog_ram_lifetimes import catalog as catalog_ram_lifetimes
from mame.analyze_program_context_run import inspect_published as inspect_runtime_observations
ROOT=Path(__file__).resolve().parents[1]
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,default=ROOT/'decomp/completeness-audit.json');p.add_argument('--coverage',type=Path,default=ROOT/'decomp/coverage.json');a=p.parse_args()
coverage=json.loads(a.coverage.read_text());programs=json.loads((ROOT/'decomp/program-set.json').read_text())
blockers=[];integrity=[];indices={};signals=collections.Counter();samples={}
for label,name in [('m68000','full-p1-index.tsv'),('z80','full-audio-index.tsv')]:
    rows=list(csv.DictReader((ROOT/'decomp'/name).open(),delimiter='\t'));states=collections.Counter(row['status'] for row in rows)
    tentative=[row for row in rows if row['status'].startswith('tentative')]
    indices[label]=dict(addresses=len(rows),statuses=dict(states),tentative_entries=len(tentative))
    if tentative:blockers.append(dict(kind='UNVERIFIED_AUTOMATIC_C',cpu=label,count=len(tentative),index='decomp/'+name))
    for row in rows:
        path=(ROOT/'decomp'/row['file']).resolve()
        if not path.is_relative_to(ROOT) or not path.is_file():integrity.append(dict(index=name,owner=row['owner'],problem='missing_or_outside_source_tree'));continue
        if not row['status'].startswith('tentative'):continue
        text=path.read_text()
        for key,pattern in [('inferred_undefined_types',r'\bundefined(?:[1-9][0-9]*)?\b'),('implicit_register_values',r'\b(?:unaff_|extraout_|in_D[0-7]|in_A[0-7])'),('bad_instruction_diagnostics',r'halt_baddata|Control flow encountered bad instruction'),('m68000_runtime_intrinsics',r'__m68k_')]:
            if re.search(pattern,text):signals[key]+=1;samples.setdefault(key,[]);samples[key]=samples[key]+[str(path.relative_to(ROOT))] if len(samples[key])<3 else samples[key]

# Exact source output hashes, without treating them as whole-program proof.
for name in ['data/player_query_fixture.json','data/player_positions_fixture.json','data/native_conditions_fixture.json','data/condition_consumers_fixture.json','decomp/recovered/rom_008512.validation.json','decomp/recovered/rom_01a5ca.validation.json']:
    path=ROOT/name
    if not path.is_file():integrity.append(dict(file=name,problem='missing_fixture'));continue
    f=json.loads(path.read_text());hashes=f.get('source_sha256')
    if isinstance(hashes,dict):
        for src,expected in hashes.items():
            if not (ROOT/src).is_file() or hashlib.sha256((ROOT/src).read_bytes()).hexdigest()!=expected:integrity.append(dict(file=name,source=src,problem='source_hash_mismatch'))
    elif isinstance(hashes,str):
        src={'data/player_query_fixture.json':'src/neo_player_query.c','data/player_positions_fixture.json':'src/neo_player_positions.c','decomp/recovered/rom_008512.validation.json':'decomp/recovered/rom_008512.c','decomp/recovered/rom_01a5ca.validation.json':'decomp/recovered/rom_01a5ca.c'}[name]
        if hashlib.sha256((ROOT/src).read_bytes()).hexdigest()!=hashes:integrity.append(dict(file=name,source=src,problem='source_hash_mismatch'))

# A verified physical partition must account for every input byte, with proof.
partition_path=ROOT/'decomp/verified-rom-partition.json';partition_issues=[]
if not partition_path.is_file():partition_issues.append('No exhaustive verified physical code/data partition exists')
else:
    partitions=json.loads(partition_path.read_text()).get('regions',[])
    for image in programs['programs']:
        cursor=0
        for region in sorted((r for r in partitions if r.get('source')==image['source']),key=lambda r:r['offset']):
            proof=ROOT/region.get('proof','')
            if region.get('offset')!=cursor or region.get('size',0)<=0 or region.get('kind') not in ('verified_code','verified_data') or not proof.is_file():partition_issues.append(image['source']+': invalid/gapped/unproven region');break
            if region['kind']=='verified_code' and not region.get('c_sources'):partition_issues.append(image['source']+': code region has no C mapping')
            for source in region.get('c_sources',[]):
                if not (ROOT/source).is_file():partition_issues.append(image['source']+': missing C source')
            cursor+=region['size']
        if cursor!=image['size']:partition_issues.append(image['source']+': not all physical bytes accounted for')
if partition_issues:blockers.append(dict(kind='EXHAUSTIVE_CODE_DATA_MAP_MISSING',images=[x['source'] for x in programs['programs']],input_bytes=programs['total_program_bytes'],issues=partition_issues))

flows=list(csv.DictReader((ROOT/'decomp/tentative/all_p1/unresolved.tsv').open(),delimiter='\t'))
resolution_path=ROOT/'decomp/verified-flow-resolutions.json';resolutions=json.loads(resolution_path.read_text()).get('sites',{}) if resolution_path.is_file() else {}
unproven=[]
for row in flows:
    resolution=resolutions.get(row['source']);proof=ROOT/resolution.get('proof','') if resolution else None
    if not resolution or not proof.is_file() or not resolution.get('disposition'):unproven.append(row)
if unproven:blockers.append(dict(kind='FLOW_SITES_WITHOUT_VERIFIED_DISPOSITION',count=len(unproven),scope='Historical P1 frontier; includes imports and computed calls. A site may have partial recovery without an exhaustive domain proof.',sample=unproven[:4]))
root_catalog=ROOT/'decomp/analysis/program-roots.json'
context_roots=set()
try:
    context_model=inspect_context_model(ROOT/'decomp/canonical-model')
    inspect_decoder_evidence(ROOT/'decomp/canonical-model')
    runtime_observations=inspect_runtime_observations(ROOT/'decomp/canonical-model/runtime/euro-start-mission1',ROOT/'decomp/canonical-model')
    modeled_locations={r['location'] for r in csv.DictReader((ROOT/'decomp/canonical-model/M68K_FIXED_P1/instructions.tsv').open(),delimiter='\t')}
    missing_observed_targets=sorted({target for site in runtime_observations.get('indirect_call_observations',[]) for target in site['observed_targets'] if int(target,16)<0x100000 and f'M68K_FIXED_P1:{int(target,16):08x}' not in modeled_locations})
    if missing_observed_targets:blockers.append(dict(kind='OBSERVED_FIXED_ROM_TARGETS_WITHOUT_MODELED_INSTRUCTION',targets=missing_observed_targets))
    lifetime_path=ROOT/'decomp/canonical-model/ram-lifetimes.json'
    if json.loads(lifetime_path.read_text())!=catalog_ram_lifetimes(ROOT/'decomp/canonical-model'):
        raise ValueError('RAM lifetime catalogue is stale')
    context_roots={(r['context'],int(r['pc'],16)) for r in context_model['roots']}
except (AssertionError,KeyError,ValueError,OSError) as error:
    integrity.append(dict(file='decomp/canonical-model',problem='invalid_context_evidence',detail=str(error)))
if root_catalog.is_file():
    roots=json.loads(root_catalog.read_text())
    missing_vectors=[target for target in roots.get('unmodeled_internal_vectors',[]) if ('M68K_FIXED_P1',int(target,16)) not in context_roots]
    if missing_vectors:blockers.append(dict(kind='UNMODELED_CARTRIDGE_VECTOR_ROOTS',targets=missing_vectors))
    # Structural roots do not establish native handlers, imports or exception ABI.
    if roots.get('unmodeled_internal_vectors'):blockers.append(dict(kind='CARTRIDGE_VECTOR_NATIVE_DOMAINS_NOT_CLOSED',targets=roots['unmodeled_internal_vectors'],detail='Historical missing roots are now checked against the context model; native handler equivalence remains open'))
    copies=[x for x in roots.get('runtime_copy_paths',[]) if not x.get('native_domain_closed',False)]
    if copies:blockers.append(dict(kind='RELOCATED_CODE_CONTEXTS_NOT_CLOSED',contexts=[dict(mapping=x['execution_mapping'],pc=x['runtime_pc'],source=x['source_offset'],container_bytes=x['container_bytes']) for x in copies]))
if not (ROOT/'decomp/full-game-validation.json').is_file():blockers.append(dict(kind='FULL_PROGRAM_MIPS_VALIDATION_MISSING',detail='Only module regressions exist; no whole-game equivalence/build validation record'))
if coverage.get('remaining'):blockers.append(dict(kind='RECORDED_OPEN_WORK',items=coverage['remaining']))
if not coverage.get('complete'):blockers.append(dict(kind='RECOVERY_NOT_DECLARED_COMPLETE'))
if integrity:blockers.append(dict(kind='ARTIFACT_INTEGRITY_FAILURE',issues=integrity))
indexed_total=sum(x['addresses'] for x in indices.values());tentative_total=sum(x['tentative_entries'] for x in indices.values())
report=dict(status='BLOCKED_INCOMPLETE' if blockers else 'STRUCTURAL_REQUIREMENTS_PASS_REQUIRES_SEMANTIC_REVIEW',complete=False,structural_checks_pass=not blockers,artifact_integrity='PASS' if not integrity else 'FAIL',indices=indices,project_completion_percent=None,indexed_tentative_entries=dict(numerator=tentative_total,denominator=indexed_total,percent=round(100*tentative_total/indexed_total,2),meaning='Fraction of index entries still tentative; NOT global project completion, workload remaining or percentage of game code bytes'),lexical_risk_signals=dict(signals),signal_samples=samples,blockers=blockers,limits='Lexical signals are review hints, not counts of broken functions. Exhausting roots or passing unit fixtures is not whole-game equivalence. No completion percentage is inferred. Even a structural pass requires review of semantic validation evidence.')
a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n')
print(report['status']+'; integrity='+report['artifact_integrity'])
for item in blockers:print(' - '+item['kind']+((': '+str(item['count'])) if 'count' in item else ''))
raise SystemExit(1 if blockers else 0)
