#!/usr/bin/env python3
"""Synthetic graphics decoding cases; no original ROM data required."""
import unittest
from array import array
from export_marco import reconstruct,top_sprite_pixel
class MarcoSpriteEdges(unittest.TestCase):
 def test_sprite_wraps_across_right_counter_edge(self):
  v=[0]*34304;p=[0]*4096;p[0x631]=0x7fff
  # One 16x16 fully opaque pen-1 tile at raw x=504, raw y=32.
  c=bytearray(128)
  for half in (0,64):
   for y in range(16):c[half+y*4]=255
  v[0x8001]=0xfff;v[0x8201]=((512-32)<<7)|1;v[0x8401]=504<<7
  v[65]=0x6300
  im,records=reconstruct(v,p,c)
  self.assertEqual(len(records),1)
  for x in range(8):
   self.assertEqual(im.info['neo_indices'][32*320+x],1)
   self.assertEqual(top_sprite_pixel(v,p,c,x,32)[0],0x63)
  self.assertEqual(im.info['neo_indices'][32*320+8],0)
 def test_remapped_actor_palettes_keep_their_identity(self):
  v=[0]*34304;p=[0]*4096;p[0x631]=0x7fff;p[0xc11]=0x00ff;c=bytearray(128)
  for half in (0,64):
   for y in range(16):c[half+y*4]=255
  for slot,x,bank in ((1,24,0x63),(2,40,0xc1)):
   v[0x8000+slot]=0xfff;v[0x8200+slot]=((512-32)<<7)|1;v[0x8400+slot]=x<<7;v[slot*64+1]=bank<<8
  im,_=reconstruct(v,p,c,selected_palette={0x63,0xc1})
  indices=array('H');indices.frombytes(im.info['neo_palette_indices'])
  self.assertEqual(indices[32*320+24],0x631)
  self.assertEqual(indices[32*320+40],0xc11)
  self.assertNotEqual(im.getpixel((24,32)),im.getpixel((40,32)))
 def test_vertical_counter_wraps_before_clipping(self):
  v=[0]*34304;p=[0]*4096;p[0x631]=0x7fff;c=bytearray(128)
  for half in (0,64):
   for y in range(16):c[half+y*4]=255
  v[0x8001]=0xfff;v[0x8201]=(8<<7)|1;v[0x8401]=32<<7;v[65]=0x6300
  im,_=reconstruct(v,p,c)
  for y in range(8):self.assertEqual(im.info['neo_indices'][y*320+32],1)
  self.assertEqual(im.info['neo_indices'][8*320+32],0)
if __name__=='__main__':unittest.main()
