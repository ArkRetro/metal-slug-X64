#!/usr/bin/env python3
import struct
import unittest
from vehicle_wreckage import stamp_destroyed_vehicle


class VehicleWreckageTests(unittest.TestCase):
    def ram(self,origin_x=-872):
        ram=bytearray(65536)
        struct.pack_into('>i',ram,0xc742,origin_x*65536)
        struct.pack_into('>i',ram,0xc746,256*65536)
        return ram

    def test_surviving_or_ineligible_vehicle_never_creates_a_ramp(self):
        ram=self.ram()
        for health,flags in ((300,1),(1,1),(0,0),(0,33)):
            output,writes=stamp_destroyed_vehicle(ram,937*65536,64*65536,health,flags)
            self.assertEqual(output,ram);self.assertFalse(writes)

    def test_wreckage_moves_with_vehicle_and_is_idempotent(self):
        ram=self.ram()
        a,first=stamp_destroyed_vehicle(ram,937*65536,64*65536,0,1)
        b,second=stamp_destroyed_vehicle(ram,945*65536,64*65536,0,1)
        self.assertEqual(len(first),13);self.assertEqual(len(second),13)
        self.assertEqual([w['world_x']+8 for w in first],[w['world_x'] for w in second])
        same,writes=stamp_destroyed_vehicle(a,937*65536,64*65536,0,1)
        self.assertEqual(same,a);self.assertFalse(writes)
        self.assertNotEqual(a,b)

    def test_solid_cells_and_upper_metadata_are_preserved(self):
        ram=self.ram();ram[0xd36d]=0xab;ram[0xd3ac]=0xa0
        output,writes=stamp_destroyed_vehicle(ram,int(937.5*65536),64*65536,0,1)
        self.assertEqual(output[0xd36d],0xab)
        self.assertEqual(output[0xd3ac],0xa5)
        self.assertEqual(len(writes),12)

    def test_outside_ring_window_is_not_aliased_into_live_terrain(self):
        ram=self.ram();output,writes=stamp_destroyed_vehicle(ram,100*65536,64*65536,0,1)
        self.assertEqual(output,ram);self.assertFalse(writes)


if __name__=='__main__':unittest.main()
