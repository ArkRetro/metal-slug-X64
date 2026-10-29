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
from extract_region_collision import extract as extract_region
from vehicle_wreckage import stamp_destroyed_vehicle


class CollisionAuditTests(unittest.TestCase):
    def setUp(self):
        self.temp=tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root=Path(self.temp.name)

    def snapshot(self,run,frame,camera=0,code=0):
        folder=self.root/run;folder.mkdir(exist_ok=True)
        ram=bytearray(65536);struct.pack_into('>H',ram,0x2036,camera)
        # Before the middle bank scrolls with the foreground (measured Y 264 vs 256).
        struct.pack_into('>i',ram,0xd7b6+0x18,264*65536)
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

    def test_full_speed_middle_bank_owns_collision(self):
        ram=bytearray(65536)
        struct.pack_into('>2i',ram,0xc72e+0x14,-1023*65536,240*65536)
        struct.pack_into('>2i',ram,0xd7b6+0x14,-1023*65536,240*65536)
        struct.pack_into('>H',ram,0x2036,1023)
        x=1024;ram[0xc72e+0x88+((x&504)<<3)+63]=11;ram[0xd7b6+0x88+((x&504)<<3)+63]=5
        values={(cx,cy):code for camera,cx,cy,code,address in observed_cells(bytes(ram))}
        self.assertEqual(values[128,0],5)  # Stale foreground ring is no longer read.


class RegionCollisionTests(unittest.TestCase):
    def setUp(self):
        self.temp=tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root=Path(self.temp.name)

    def ram(self):
        ram=bytearray(65536);struct.pack_into('>H',ram,0x2036,872)
        struct.pack_into('>2i',ram,0xc72e+0x14,-872*65536,256*65536)
        struct.pack_into('>2i',ram,0xd7b6+0x14,-654*65536,264*65536)
        for x in range(0,1280,8):ram[0xc7b6+(x//8&63)*64+(~7&63)]=11  # Floor, Y=56..64.
        return ram

    def write(self,frames):
        folder=self.root/'run';folder.mkdir(exist_ok=True)
        # Earlier views cover columns left of camera 872 with the same floor.
        for frame,camera in enumerate((0,320,640)):
            ram=self.ram();struct.pack_into('>H',ram,0x2036,camera)
            (folder/f'ram{frame}.bin').write_bytes(bytes(ram))
        for frame,ram in frames:(folder/f'ram{frame+10}.bin').write_bytes(bytes(ram))
        return folder

    def test_wreck_cells_keep_pre_event_terrain(self):
        before=self.ram();after,writes=stamp_destroyed_vehicle(before,1174*65536,64*65536,0,1)
        ahead=bytearray(after);struct.pack_into('>H',ahead,0x2036,1000)  # Reveals columns up to 1327.
        report=extract_region([self.write([(1,before),(2,after),(3,ahead)])])
        self.assertEqual(len(writes),13);self.assertEqual(report['events'][0]['anchor_cell'],[146,8])
        data=bytes.fromhex(report['cells_hex'])
        self.assertEqual(data[139*32+8],0)  # Ramp foot: static empty, event value 5.
        self.assertEqual(data[151*32+8],0)  # First observed after the event: preloaded pre-event byte.
        self.assertEqual(data[151*32+7],11)

    def test_unexplained_change_still_fails(self):
        before=self.ram();after=bytearray(before);after[0xc7b6+(140&63)*64+(~9&63)]=5
        with self.assertRaisesRegex(AssertionError,'Unexplained or ambiguous'):
            extract_region([self.write([(1,before),(2,after)])])

    def test_wreck_cell_changing_again_fails(self):
        before=self.ram();after,_=stamp_destroyed_vehicle(before,1174*65536,64*65536,0,1)
        later=bytearray(after);later[0xc7b6+(139&63)*64+(~8&63)]=0
        with self.assertRaisesRegex(AssertionError,'Wreck cell changed again'):
            extract_region([self.write([(1,before),(2,after),(3,later)])])


if __name__=='__main__':unittest.main()
