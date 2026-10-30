#!/usr/bin/env python3
"""Export tentative C for the Z80 initial map, without flattening banked code."""
import argparse,collections,hashlib,json,os,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--inputs',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
source=a.inputs/'z80-initial-map.bin';assert source.stat().st_size==0xf800
out=a.output.resolve();out.mkdir(parents=True,exist_ok=False)
for name in ('projects','profile'):(out/name).mkdir()
jdk=ROOT.parent/'toolchains/jdk-21.0.13+9';ghidra=ROOT.parent/'toolchains/ghidra_11.4.2_PUBLIC'
env=dict(os.environ,JAVA_HOME=str(jdk),PATH=str(jdk/'bin')+os.pathsep+os.environ['PATH'],JAVA_TOOL_OPTIONS='-Duser.home='+str(out/'profile'))
cmd=[str(ghidra/'support/analyzeHeadless'),str(out/'projects'),'audio_initial','-import',str(source.resolve()),'-loader','BinaryLoader','-loader-baseAddr','0','-processor','z80:LE:16:default','-cspec','default','-noanalysis','-scriptPath',str(ROOT/'tools/ghidra'),'-postScript','ExportZ80Code.java',str(out),'-max-cpu','2']
with (out/'headless.log').open('w') as log:subprocess.run(cmd,env=env,stdout=log,stderr=subprocess.STDOUT,check=True,timeout=600)
assert (out/'unresolved.tsv').exists(),'Check Ghidra log; export not complete'
rows=[line.split('\t') for line in (out/'functions.tsv').read_text().splitlines()[1:]]
report=dict(status='TENTATIVE_Z80_INITIAL_MAP',processed=len(rows),statuses=dict(collections.Counter(r[-1] for r in rows)),unresolved=len((out/'unresolved.tsv').read_text().splitlines())-1,analysis_warning_lines=sum('WARN ' in line for line in (out/'headless.log').read_text().splitlines()),initial_map_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),limits='Initial driver map only; bank changes, RAM execution, I/O, inferred types and indirect flow remain unverified. This is not a native sound driver or all M1 execution states.')
(out/'validation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
