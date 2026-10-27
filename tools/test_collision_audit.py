#!/usr/bin/env python3
"""Guard against treating mutable/unobserved collision as empty static terrain."""
import contextlib
import io
import json
from pathlib import Path
import struct
import tempfile
import unittest
from audit_collision import audit
from extract_collision import extract, observed_cells


class CollisionAuditTests(unittest.TestCase):
    def setUp(self):
        self.temp=tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root=Path(self.temp.name)

    def snapshot(self,run,frame,camera=0,code=0):
        folder=self.root/run;folder.mkdir(exist_ok=True)
        ram=bytearray(65536);struct.pack_into('>H',ram,0x2036,camera)
        ram[0xc7f5]=code  # Column zero, bottom byte: shared ring address at x=0 and x=512, y=0.
        (folder/f'ram{frame}.bin').write_bytes(ram)
        return folder

    def test_observed_empty_and_unknown_are_distinct(self):
        folder=self.snapshot('later',1,camera=512)
        report=audit([folder]);mask=bytes.fromhex(report['valid_mask_hex'])
        self.assertEqual(mask[0],0)
        self.assertEqual(mask[64*32],1)
        self.assertGreater(report['unknown_cells'],0)
        self.assertFalse(report['ready_for_static_export'])

    def test_mutation_has_no_selected_static_value(self):
        folder=self.snapshot('run',1)
        self.snapshot('run',2,code=5)
        report=audit([folder]);self.assertEqual(report['mutable_cells'],1)
        self.assertEqual(bytes.fromhex(report['valid_mask_hex'])[0],0)
        states=report['mutable'][0]['runs'][0]['states']
        self.assertEqual([(s['first_frame'],s['code']) for s in states],[(1,0),(2,5)])
        with self.assertRaisesRegex(AssertionError,'Dynamic collision cell'):
            extract(folder,self.root/'invalid.json')
        self.assertFalse((self.root/'invalid.json').exists())

    def test_independent_runs_do_not_form_one_event_schedule(self):
        a=self.snapshot('a',100,code=5);b=self.snapshot('b',100,code=11)
        runs=audit([a,b])['mutable'][0]['runs']
        self.assertEqual([r['run'] for r in runs],[0,1])
        self.assertEqual([len(r['states']) for r in runs],[1,1])

    def test_static_export_remains_compatible(self):
        folder=self.snapshot('static',1,code=11);self.snapshot('static',2,code=11)
        report=audit([folder]);self.assertTrue(report['ready_for_static_export'])
        with contextlib.redirect_stdout(io.StringIO()):extract(folder,self.root/'static.json')
        original=json.loads((self.root/'static.json').read_text())
        self.assertEqual(original['cells_hex'],report['cells_hex'])

    def test_ring_wrap_preserves_world_coordinates(self):
        folder=self.snapshot('wrap',1,camera=512,code=11)
        values=list(observed_cells((folder/'ram1.bin').read_bytes()))
        matching=[(x,y,value) for camera,x,y,value,address in values if address==0xc7f5]
        self.assertEqual(matching,[(64,0,11)])
        with self.assertRaisesRegex(AssertionError,'64 KiB'):
            list(observed_cells(bytes(100)))


if __name__=='__main__':unittest.main()
