#!/usr/bin/env python3
"""Explicit capture bounds must prevent later scenes from contaminating a map."""
import contextlib
import io
import json
from pathlib import Path
import struct
import tempfile
import unittest

from extract_mission_map import extract
from validate_mission_map import animation_phase,animated_tile_code


class MissionMapRangeTests(unittest.TestCase):
    def setUp(self):
        self.temporary=tempfile.TemporaryDirectory()
        self.root=Path(self.temporary.name)
        self.capture=self.root/'capture';self.capture.mkdir()
        for frame,backdrop in ((10,1),(20,2),(30,2),(40,3)):
            words=[0]*34304
            words[0x8000+105]=0x0fff
            words[0x8200+105]=((512-16)<<7)|1
            words[0x8400+105]=0
            words[105*64+1]=0x0040
            (self.capture/f'vram{frame}.bin').write_bytes(struct.pack('>34304H',*words))
            ram=bytearray(65536)
            for address,y in ((0xc72e,256),(0xd7b6,264),(0xe83e,264)):
                struct.pack_into('>2i',ram,address+0x14,0,y*65536)
                struct.pack_into('>2h',ram,address+0x84,0,y)
            (self.capture/f'ram{frame}.bin').write_bytes(ram)
            palette=[0]*4096;palette[-1]=backdrop
            (self.capture/f'palette{frame}.bin').write_bytes(struct.pack('>4096H',*palette))

    def tearDown(self):
        self.temporary.cleanup()

    def run_extract(self,**arguments):
        output=self.root/'output'
        with contextlib.redirect_stdout(io.StringIO()):
            extract(self.capture,output,**arguments)
        return json.loads((output/'map.json').read_text())

    def test_bounds_are_inclusive_and_retained(self):
        model=self.run_extract(first_frame=20,last_frame=30)
        self.assertEqual([o['frame'] for o in model['observations']],[20,30])
        self.assertEqual(model['backdrop_values'],[2])
        self.assertEqual(model['capture_selection']['out_of_range_snapshots'],2)
        self.assertEqual(model['capture_selection']['first_frame'],20)
        self.assertEqual(model['capture_selection']['last_frame'],30)

    def test_unbounded_backdrop_changes_still_require_opt_in(self):
        with self.assertRaisesRegex(AssertionError,'Multiple backdrops'):
            self.run_extract()
        model=self.run_extract(allow_backdrop_timeline=True)
        self.assertEqual(model['backdrop_values'],[1,2,3])
        self.assertEqual(len(model['observations']),4)

    def test_empty_or_reversed_range_rejected(self):
        with self.assertRaisesRegex(ValueError,'No RAM/VRAM'):
            self.run_extract(first_frame=50)
        with self.assertRaisesRegex(ValueError,'must not exceed'):
            self.run_extract(first_frame=30,last_frame=20)
        with self.assertRaisesRegex(ValueError,'nonnegative'):
            self.run_extract(first_frame=-1)

    def test_measured_device_phase_is_kept_not_frame_phase(self):
        (self.capture/'frames.jsonl').write_text(json.dumps(dict(frame=20,animation_counter=9,animation_disabled=False))+'\n')
        model=self.run_extract(first_frame=20,last_frame=20)
        observation=model['observations'][0]
        self.assertEqual(animation_phase(observation),1)
        self.assertNotEqual(animation_phase(observation),observation['frame']&7)
        self.assertIn(str(self.capture),model['animation_metadata_sha256'])

    def test_missing_phase_stops_audit(self):
        model=self.run_extract(first_frame=20,last_frame=20)
        with self.assertRaisesRegex(ValueError,'Missing measured'):
            animation_phase(model['observations'][0])

    def test_disabled_animation_preserves_raw_low_bits(self):
        self.assertEqual(animated_tile_code(0xbdd,0x4b44,0,True),0x40bdd)
        self.assertEqual(animated_tile_code(0xbdd,0x4b44,0,False),0x40bdc)
        self.assertEqual(animated_tile_code(0xbdd,0x4b44,1,False),0x40bdd)
        self.assertEqual(animation_phase(dict(frame=20,animation_counter=9,animation_disabled=True)),1)


if __name__=='__main__':unittest.main()
