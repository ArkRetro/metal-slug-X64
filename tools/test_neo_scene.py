#!/usr/bin/env python3
"""Regression cases for culled backgrounds and a reused sprite bank.

These register-only fixtures exercise observed failure modes; no ROM graphics
or original program bytes are included in the test data.
"""
import struct,unittest
from neo_scene import state

class SceneTests(unittest.TestCase):
    def scene(self,camera=10,full_speed=False):
        v=[0]*34304;ram=bytearray(65536)
        for addr,y,x in ((0xc72e,256,-camera),(0xd7b6,256 if full_speed else 264,-camera if full_speed else -camera*3//4),(0xe83e,264,-camera//2)):
            struct.pack_into('>2i',ram,addr+0x14,x*65536,y*65536)
            struct.pack_into('>2h',ram,addr+0x84,x,y)
        return v,ram
    def column(self,v,slot,x,y=0,attr=0x1140):
        v[0x8000+slot]=0xfff
        v[0x8200+slot]=(((512-y)&511)<<7)|1
        v[0x8400+slot]=x<<7
        v[slot*64+1]=attr
    def test_culled_first_column_uses_surviving_columns(self):
        v,r=self.scene();v[0x8400+105]=500<<7
        self.column(v,106,7)
        result=state(v,r)
        self.assertEqual(result['camera'],9)
        self.assertEqual(result['vertical_offsets'][3],0)
    def test_empty_foreground_uses_new_full_speed_bank(self):
        v,r=self.scene(camera=1035,full_speed=True)
        struct.pack_into('>h',r,0xd7b6+0x86,240)
        self.column(v,34,7,y=16)
        result=state(v,r)
        self.assertEqual(result['camera'],1033)
        self.assertEqual(result['vertical_offsets'][2],16)
        self.assertEqual(result['active'],[False,False,True,False])
    def test_enemy_bank_is_not_background(self):
        v,r=self.scene();self.column(v,106,7)
        self.column(v,65,40,y=25,attr=0xbb10)
        self.assertEqual([c['slot'] for c in state(v,r)['columns']],[106])
    def test_inconsistent_live_columns_fail(self):
        v,r=self.scene();self.column(v,106,7);self.column(v,107,24)
        with self.assertRaisesRegex(AssertionError,'Inconsistent background X'):state(v,r)
    def test_ambiguous_vertical_wrap_fails(self):
        v,r=self.scene();self.column(v,106,7,y=24)
        with self.assertRaisesRegex(AssertionError,'Ambiguous frame transform'):state(v,r)
    def test_no_full_speed_reference_fails(self):
        v,r=self.scene();self.column(v,1,0,y=8)
        with self.assertRaisesRegex(ValueError,'No unambiguous'):state(v,r)

if __name__=='__main__':unittest.main()
