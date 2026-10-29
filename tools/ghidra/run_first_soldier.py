#!/usr/bin/env python3
"""Import the captured logical/physical Neo Geo map and run addressed analysis."""
import argparse,os,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('captures',type=Path);p.add_argument('--output',type=Path,required=True)
p.add_argument('--ghidra',type=Path,required=True);p.add_argument('--jdk',type=Path,required=True)
a=p.parse_args();out=a.output.resolve();out.mkdir(parents=True,exist_ok=True)
properties=(a.ghidra/'Ghidra/application.properties').read_text()
assert 'application.version=11.4.2\n' in properties,'This evidence workflow is pinned to Ghidra 11.4.2'
projects=out/'projects';projects.mkdir(exist_ok=True)
assert not (projects/'mslugx_m1_soldier.gpr').exists(),'Use a new output directory; preserve previous projects'
profile=out/'ghidra-profile';profile.mkdir(exist_ok=True)
env=dict(os.environ,JAVA_HOME=str(a.jdk.resolve()),PATH=str(a.jdk.resolve()/'bin')+os.pathsep+os.environ.get('PATH',''),
    XDG_CONFIG_HOME=str(profile/'config'),XDG_CACHE_HOME=str(profile/'cache'))
env['JAVA_TOOL_OPTIONS']=os.environ.get('JAVA_TOOL_OPTIONS','')+' -Duser.home="'+str(profile)+'"'
cmd=[str(a.ghidra.resolve()/'support/analyzeHeadless'),str(projects),'mslugx_m1_soldier','-import',str(a.captures.resolve()/'program-fixed.bin'),'-loader','BinaryLoader','-loader-baseAddr','0','-processor','68000:BE:32:MC68020','-cspec','default','-noanalysis','-scriptPath',str(ROOT/'tools/ghidra'),'-postScript','AnalyzeNeoGeoSoldier.java',str(a.captures.resolve()),str(out),'-max-cpu','2']
with (out/'headless.log').open('w') as log:result=subprocess.run(cmd,env=env,stdout=log,stderr=subprocess.STDOUT,timeout=240)
assert result.returncode==0 and (out/'functions.tsv').is_file(),'Check headless.log: Ghidra did not complete analysis'
print('Ghidra project, addressed assembly and TENTATIVE decompiler output:',out)
