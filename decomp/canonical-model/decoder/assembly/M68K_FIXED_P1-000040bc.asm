0040BC: 4BF9 0010 8000 lea     $108000.l, A5
0040C2: B079 0010 63D8 cmp.w   $1063d8.l, D0
0040C8: 6500 000A      bcs     $40d4
0040CC: 41ED 9624      lea     (-$69dc,A5), A0
0040D0: 4207           clr.b   D7
0040D2: 4E75           rts
0040D4: 2F08           move.l  A0, -(A7)
0040D6: 206D E3D4      movea.l (-$1c2c,A5), A0
0040DA: 2B68 0008 E3D4 move.l  ($8,A0), (-$1c2c,A5)
0040E0: B0FC 0000      cmpa.w  #$0, A0
0040E4: 6700 0006      beq     $40ec
0040E8: 536D E3D8      subq.w  #1, (-$1c28,A5)
0040EC: 214E 000C      move.l  A6, ($c,A0)
0040F0: 302E 0010      move.w  ($10,A6), D0
0040F4: 5240           addq.w  #1, D0
0040F6: 3140 0010      move.w  D0, ($10,A0)
0040FA: 226E 0008      movea.l ($8,A6), A1
0040FE: 214E 0004      move.l  A6, ($4,A0)
004102: 2149 0008      move.l  A1, ($8,A0)
004106: 2348 0004      move.l  A0, ($4,A1)
00410A: 2D48 0008      move.l  A0, ($8,A6)
00410E: 209F           move.l  (A7)+, (A0)
004110: 116E 0068 0068 move.b  ($68,A6), ($68,A0)
004116: 003C 0001      ori     #$1, CCR
00411A: 4E75           rts
