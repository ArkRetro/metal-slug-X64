0008: DB 04       in   a,($04)
000A: A7          and  a
000B: FA 08 00    jp   m,$0008
000E: 7B          ld   a,e
000F: 59          ld   e,c
0010: ED 53 0B F8 ld   ($F80B),de
0014: ED 51       out  (c),d
0016: 00          nop
0017: 00          nop
0018: 5F          ld   e,a
0019: 0C          inc  c
001A: ED 59       out  (c),e
001C: 0D          dec  c
001D: C9          ret
