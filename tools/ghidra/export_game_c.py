#!/usr/bin/env python3
"""Verify the user's P1, collect prior static roots and export a tentative C frontier."""
import argparse,collections,hashlib,json,os,re,subprocess,zipfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--rom',type=Path,required=True);p.add_argument('--research',type=Path);p.add_argument('--seeds',type=Path,help='Exact prior root list; avoids rescanning research directories');p.add_argument('--output',type=Path,required=True);p.add_argument('--limit',type=int,default=1000);p.add_argument('--whole-candidates',action='store_true',help='Use the batch exporter for a whole-ROM candidate-root scan');a=p.parse_args()
assert 1<=a.limit<=20000
out=a.output.resolve();out.mkdir(parents=True,exist_ok=False)
with zipfile.ZipFile(a.rom) as z:raw=z.read('250-p1.p1')
assert hashlib.sha1(raw).hexdigest()==json.loads((ROOT/'tools/rom_manifest.json').read_text())['250-p1.p1']['sha1']
rom=bytearray(raw);rom[::2]=raw[1::2];rom[1::2]=raw[::2];(out/'program-fixed.bin').write_bytes(rom)
seeds={0xc3078,0xc30f0,0xc315a,0xc31b8,0xc3214,0xc3232,0xc3250,0xc32d0}
if a.seeds:
 for line in a.seeds.read_text().splitlines():
  assert re.fullmatch('[0-9a-fA-F]{3,6}',line) and 0x100<=int(line,16)<0x100000 and int(line,16)%2==0
  seeds.add(int(line,16))
else:
 assert a.research,'Provide --seeds or --research'
 for path in a.research.glob('*/instructions.tsv'):
  with path.open() as f:
   if f.readline().split('\t')[0]!='seed':continue
   for line in f:
    fields=line.split('\t');s=fields[0]
    if len(fields)>=3 and re.fullmatch('[0-9a-f]{3,6}',s) and 0x100<=int(s,16)<0x100000:seeds.add(int(s,16))
header_jumps=[]
for at in range(0x100,0x200,2):
 if rom[at:at+2]==bytes.fromhex('4ef9'):
  target=int.from_bytes(rom[at+2:at+6],'big')
  if 0x100<=target<0x100000 and target%2==0:seeds.add(target);header_jumps.append(dict(site=hex(at),target=hex(target)))
prior=[0xc3078,0xc30f0,0xc315a,0xc31b8,0xc3214,0xc3232,0xc3250,0xc32d0]
(out/'seeds.txt').write_text('\n'.join(f'{s:x}' for s in prior+sorted(seeds-set(prior)))+'\n')
for name in ('projects','profile'):(out/name).mkdir()
jdk=ROOT.parent/'toolchains/jdk-21.0.13+9';ghidra=ROOT.parent/'toolchains/ghidra_11.4.2_PUBLIC'
env=dict(os.environ,JAVA_HOME=str(jdk),PATH=str(jdk/'bin')+os.pathsep+os.environ['PATH'],JAVA_TOOL_OPTIONS='-Duser.home='+str(out/'profile'))
cmd=[str(ghidra/'support/analyzeHeadless'),str(out/'projects'),'game_frontier','-import',str(out/'program-fixed.bin'),'-loader','BinaryLoader','-loader-baseAddr','0','-processor','68000:BE:32:MC68020','-cspec','default','-noanalysis','-scriptPath',str(ROOT/'tools/ghidra'),'-postScript',('ExportProgramFrontier.java' if a.whole_candidates else 'ExportGameCode.java'),str(out/'seeds.txt'),str(out),str(a.limit),'-max-cpu','2']
with (out/'headless.log').open('w') as log:subprocess.run(cmd,env=env,stdout=log,stderr=subprocess.STDOUT,check=True,timeout=3600)
assert (out/'unresolved.tsv').exists(),'Inspect headless.log; exporter did not finish'
rows=[l.split('\t') for l in (out/'functions.tsv').read_text().splitlines()[1:]]
warnings=sum('WARN ' in line for line in (out/'headless.log').read_text().splitlines())
report=dict(cart_header_jumps=header_jumps,constructor_discovery='A0 values loaded immediately before verified generic allocators 0040BC/00410E',analysis_warning_lines=warnings,whole_p1_candidate_mode=a.whole_candidates,status='TENTATIVE_FRONTIER_EXPORTED',seed_file_sha256=hashlib.sha256((out/'seeds.txt').read_bytes()).hexdigest(),initial_seeds=len(seeds),processed=len(rows),statuses=dict(collections.Counter(r[3] for r in rows)),recovered_trampoline_entries=len((out/'trampolines.tsv').read_text().splitlines())-1,pending=len((out/'pending-seeds.txt').read_text().splitlines()),unresolved=len((out/'unresolved.tsv').read_text().splitlines())-1,rom_sha256=hashlib.sha256(rom).hexdigest(),files={n:hashlib.sha256((out/n).read_bytes()).hexdigest() for n in ['functions.tsv','instructions.tsv','TENTATIVE_ROM_C.c','unresolved.tsv','trampolines.tsv']},limits='Cart header JMPs + prior roots + direct calls + allocator callbacks + A6 state-pointer frontier in fixed P1 only. Not all game code; indirect tables, P2 banks and Z80 remain. Exported C is unverified and not native/buildable.')
(out/'validation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
