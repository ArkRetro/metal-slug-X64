#!/usr/bin/env python3
"""Inventory the original 00427A dispatch table and current converter syntax.

Table bytes and destinations are static evidence; parser syntax is not proof
that native semantics or all original animation programs are complete.
"""
import argparse,ast,hashlib,json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--input-rom',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
b=a.input_rom.read_bytes();expected=json.loads((ROOT/'decomp/program-set.json').read_text())['programs'][0]['cpu_order_sha256'];assert hashlib.sha256(b).hexdigest()==expected
assert b[0x42a0:0x42a4]==bytes.fromhex('0c470034') and b[0x42bc:0x42c0]==bytes.fromhex('4efb7002')
parser=ROOT/'tools/convert_marco_programs.py';syntax=ast.parse(parser.read_text());recognized=set()
for n in ast.walk(syntax):
 if isinstance(n,ast.Compare) and isinstance(n.left,ast.Name) and n.left.id=='op' and len(n.ops)==1 and isinstance(n.ops[0],ast.Eq) and len(n.comparators)==1 and isinstance(n.comparators[0],ast.Constant) and isinstance(n.comparators[0].value,int):recognized.add(n.comparators[0].value)
rows=[]
for opcode in range(0,0x34,4):
 stub=0x42c0+opcode;assert b[stub:stub+2]==bytes.fromhex('6000');target=stub+2+int.from_bytes(b[stub+2:stub+4],'big',signed=True)
 rows.append(dict(opcode=f'0x{opcode:02X}',stub=f'0x{stub:06X}',target=f'0x{target:06X}',current_marco_converter_has_branch=opcode in recognized,semantic_coverage='NOT_PROVEN_COMPLETE',callback_service_domain='partially_recovered' if opcode in (0x14,0x30) else 'not_classified_here'))
report=dict(status='STATIC_ANIMATION_VM_DISPATCH_AND_CONVERTER_RECONCILIATION',entry='0x00427A',guard='0x0042A0',guard_upper_exclusive=0x34,dispatch='0x0042BC',table='0x0042C0',table_slots=len(rows),entries=rows,missing_marco_converter_opcodes=[r['opcode'] for r in rows if not r['current_marco_converter_has_branch']],runtime_domain_proven=False,original_input_sha256=expected,converter_sha256=hashlib.sha256(parser.read_bytes()).hexdigest(),limits='13 four-byte BRA.W slots, not 52 distinct commands. Guard checks byte <0x34, not an explicit alignment proof. A recognized converter branch may still assert on unproven data or produce UNRECOVERED callbacks. Neither 12/13 syntax branches nor table bytes measure whole-game recovery.')
a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('Original table slots:',len(rows),'; missing current Marco parser branches:',report['missing_marco_converter_opcodes'])
