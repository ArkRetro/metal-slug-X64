#!/usr/bin/env python3
"""Apply only an independently audited disabled-animation mask correction."""
import argparse
import hashlib
import json
from pathlib import Path
from audit_map_phase_correction import corrected_cells

def apply(old_path,candidate_path,proof_path,output):
    sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
    old=json.loads(old_path.read_text());candidate=json.loads(candidate_path.read_text())
    proof=json.loads(proof_path.read_text())
    if proof['status']!='TARGETED_CORRECTION_PASS' or proof['failures']:
        raise ValueError('Correction requires a passing targeted source-art audit')
    if sha(old_path)!=proof['old_map_sha256'] or sha(candidate_path)!=proof['candidate_map_sha256']:
        raise ValueError('Map hashes do not match the audited inputs')
    changed=corrected_cells(old,candidate)
    if len(changed)!=proof['corrected_cells'] or proof['source_opaque_corrected_cells']!=len(changed):
        raise ValueError('Every corrected cell requires direct source provenance')
    for cell in old['cells']:
        if (cell['layer'],cell['x'],cell['y']) in changed:cell['animation_mask']=0
    old.setdefault('corrections',[]).append(dict(
        kind='measured_disabled_autoanimation',cells=len(changed),proof_sha256=sha(proof_path),
        old_map_sha256=proof['old_map_sha256'],candidate_map_sha256=proof['candidate_map_sha256'],
        animation_metadata_sha256=proof['animation_metadata_sha256'],
        visible_pixel_comparison=proof['visible_source_pixels'],
        limits=proof['limits']))
    output.write_text(json.dumps(old,indent=2)+'\n')
    print(f'Applied {len(changed)} mask-only corrections to {output}')

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--map',type=Path,required=True);p.add_argument('--candidate',type=Path,required=True)
    p.add_argument('--proof',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();apply(a.map,a.candidate,a.proof,a.output)
