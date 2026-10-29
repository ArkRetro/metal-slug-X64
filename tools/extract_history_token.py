#!/usr/bin/env python3
"""Extract executed history-token writes and their addressed instruction proof."""
import argparse,csv,hashlib,json,re
from pathlib import Path
from extract_soldier_rng import values
p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('ghidra',type=Path);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
log=(a.capture/'debug.log').read_text();assert 'Error in command' not in log
instructions=list(csv.DictReader((a.ghidra/'instructions.tsv').open(),delimiter='\t'));pcs={int(r['address'],16) for r in instructions};assert {0x1ab50,0x1ab54,0x1ab58,0x1ab5a,0x1ab5e}<=pcs
rows=[];duplicates=0
for line in log.splitlines():
 if not line.startswith('HISTORY_TOKEN_WRITE '):continue
 r=values(line)
 if rows and r==rows[-1]:duplicates+=1;continue
 assert r['pc']==0x1ab5e and r['address']==0x10c33a and r['data']==(r['before']&1)+1
 rows.append(r)
assert len(rows)==180 and duplicates==1 and all(r['data'] in (1,2) for r in rows)
trace=(a.capture/'history-token.log').read_text().splitlines();proof=[]
for line in trace:
 m=re.search(r'^frame=(\d+) D0=([0-9A-F]{8}) A5=([0-9A-F]{8}) (01AB(?:50|54|58|5A|5E)):',line)
 if m and int(m[1])==2103:proof.append(dict(pc=int(m[4],16),d0=int(m[2],16),a5=int(m[3],16)))
assert [r['pc'] for r in proof]==[0x1ab50,0x1ab54,0x1ab58,0x1ab5a,0x1ab5e]
assert [r['d0']&255 for r in proof[1:]]==[2,0,1,1] and all(r['a5']==0x108000 for r in proof)
sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
f=dict(source='MAME 0.276 watchpoint on RAM 0x10C33A plus executed instructions; Ghidra addressed cross-check',writer_rom='0x01AB5A',routine_rom='0x01AB50',watch_pc_after_write='0x01AB5E',rows=rows,executed_proof=proof,duplicate_watch_log_rows=duplicates,provenance={n:sha(a.capture/n) for n in ('debug.log','history-token.log','program-fixed.bin','run-command.json')},ghidra_sha256=sha(a.ghidra/'instructions.tsv'),language='68000:BE:32:MC68020',limits=['Observed active-gameplay update passes, not arbitrary startup RAM.','Watch PC reports the following RTS; the actual store is 01AB5A.','Native callers must use this token convention; raw uint8_t increment is not the recovered scheduler.'])
a.output.write_text(json.dumps(f,indent=2)+'\n');print('Verified 180 writes, values 1/2; one duplicate watcher log row explained')
