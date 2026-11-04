#!/usr/bin/env python3
"""Build fresh Ghidra contexts from the prepared canonical mapping inputs."""
import argparse,csv,hashlib,json,os,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--inputs',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
inputs=a.inputs.resolve();out=a.output.resolve();out.mkdir(parents=True,exist_ok=False)
layout=json.loads((inputs/'layout.json').read_text());jobs=list(csv.DictReader((inputs/'contexts.tsv').open(),delimiter='\t'));job_map={r['context']:r for r in jobs}
for c in layout['contexts']:
 r=job_map[c['id']];data=(inputs/r['file']).read_bytes();assert len(data)==c['size'] and hashlib.sha256(data).hexdigest()==c['sha256']
(out/'projects').mkdir();(out/'profile').mkdir()
jdk=ROOT.parent/'toolchains/jdk-21.0.13+9';ghidra=ROOT.parent/'toolchains/ghidra_11.4.2_PUBLIC'
env=dict(os.environ,JAVA_HOME=str(jdk),PATH=str(jdk/'bin')+os.pathsep+os.environ['PATH'],JAVA_TOOL_OPTIONS='-Duser.home='+str(out/'profile'))
reports=[]
for context,language in [('M68K_FIXED_P1','68000:BE:32:MC68020'),('Z80_INITIAL_CART','z80:LE:16:default'),('Z80_INITIAL_SYSTEM','z80:LE:16:default')]:
 r=job_map[context];destination=out/context;destination.mkdir()
 cmd=[str(ghidra/'support/analyzeHeadless'),str(out/'projects'),context,'-import',str(inputs/r['file']),'-loader','BinaryLoader','-loader-baseAddr','0','-processor',language,'-cspec','default','-noanalysis','-scriptPath',str(ROOT/'tools/ghidra'),'-postScript','BuildCanonicalContexts.java',str(inputs),str(destination),context,'-max-cpu','2']
 with (destination/'headless.log').open('w') as log:subprocess.run(cmd,env=env,stdout=log,stderr=subprocess.STDOUT,check=True,timeout=600)
 assert (destination/'roots.tsv').is_file(),'Inspect Ghidra log; script did not finish'
 roots=list(csv.DictReader((destination/'roots.tsv').open(),delimiter='\t'));assert all(r['instruction_present']=='true' and r['owner']!='-' for r in roots),roots
 reports.append(dict(program=context,processor=language,roots=len(roots),models=len((destination/'functions.tsv').read_text().splitlines())-1,files={n:hashlib.sha256((destination/n).read_bytes()).hexdigest() for n in ['memory.tsv','roots.tsv','functions.tsv','edges.tsv','instructions.tsv','descriptor-seeds.tsv']}))
 print(context,':',reports[-1]['roots'],'seeded roots,',reports[-1]['models'],'candidate functions',flush=True)
report=dict(status='CONTEXT_MODELS_BUILT_NOT_NATIVE_RECOVERY',layout_sha256=hashlib.sha256((inputs/'layout.json').read_bytes()).hexdigest(),programs=reports,limits='Ghidra candidate CFG only. M68000 model uses MC68020 superset; execution/liveness/code-data/native equivalence remains unverified. Initial Z80 maps do not model all selector tuples. Fixed-system audio BIOS is imported as a context, not decompiled as game code. P2 blocks are data-only analysis containers, not proven non-code.')
(out/'validation.json').write_text(json.dumps(report,indent=2)+'\n')
