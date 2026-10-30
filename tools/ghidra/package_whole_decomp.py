#!/usr/bin/env python3
"""Publish the global candidate C in reviewable files, never as verified native C."""
import argparse,csv,hashlib,json,re
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
p=argparse.ArgumentParser(description=__doc__);p.add_argument('export',type=Path);a=p.parse_args()
manifest=json.loads((a.export/'validation.json').read_text());assert manifest['whole_p1_candidate_mode']
out=ROOT/'decomp/tentative/all_p1';out.mkdir(parents=True,exist_ok=False)
rows=list(csv.DictReader((a.export/'functions.tsv').open(),delimiter='\t'))
by_owner={r['owner']:r for r in rows if r['status']!='shared_owner' and r['owner']!='-'}
text=(a.export/'TENTATIVE_ROM_C.c').read_text()
parts=re.split(r'(?m)^/\* Candidate owner 0x([0-9a-f]+); ([^\n]*) \*/\n',text)
index=[]
for i in range(1,len(parts),3):
 owner,label,body=parts[i:i+3];row=by_owner[owner];status=row['status'];hexpc=f'{int(owner,16):06x}'
 if status=='decompiler_failed':index.append((hexpc,status,'NOT_EXPORTED'));continue
 folder=out/('with_diagnostics' if status=='tentative_c_with_diagnostics' else 'unverified')/hexpc[:2];folder.mkdir(parents=True,exist_ok=True)
 path=folder/f'rom_{hexpc}.c'
 header=f'/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x{hexpc}.\n * Status: {status}. Not native N64 logic; inferred types/registers and call boundaries require recovery. */\n'
 path.write_text(header+'\n'+'\n'.join(line.rstrip() for line in body.strip().splitlines())+'\n');index.append((hexpc,status,str(path.relative_to(ROOT/'decomp'))))
assert len(index)==len(by_owner),(len(index),len(by_owner))
(ROOT/'decomp/full-p1-index.tsv').write_text('owner\tstatus\tfile\n'+''.join('\t'.join(row)+'\n' for row in index))
for name in ['functions.tsv','unresolved.tsv','trampolines.tsv','seeds.txt']:(out/name).write_text((a.export/name).read_text().rstrip()+'\n')
manifest['published_c_files']=sum(status!='decompiler_failed' for _,status,_ in index)
manifest['native_c_port']=False
manifest['index']='full-p1-index.tsv'
manifest['candidate_roots_may_be_data']=True
manifest['index_sha256']=hashlib.sha256((ROOT/'decomp/full-p1-index.tsv').read_bytes()).hexdigest()
(ROOT/'decomp/full-p1-inventory.json').write_text(json.dumps(manifest,indent=2)+'\n')
print('Published',manifest['published_c_files'],'candidate C files; statuses remain unverified')
