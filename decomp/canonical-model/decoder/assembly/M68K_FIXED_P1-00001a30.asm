001A30: 48E7 80C0      movem.l D0/A0-A1, -(A7)
001A34: 203C 0010 FC00 move.l  #$10fc00, D0
001A3A: 102C 0004      move.b  ($4,A4), D0
001A3E: 2040           movea.l D0, A0
001A40: 102C 0005      move.b  ($5,A4), D0
001A44: 2240           movea.l D0, A1
001A46: 116C 0005 0005 move.b  ($5,A4), ($5,A0)
001A4C: 136C 0004 0004 move.b  ($4,A4), ($4,A1)
001A52: 522D 7CE9      addq.b  #1, ($7ce9,A5)
001A56: 102D 7CE8      move.b  ($7ce8,A5), D0
001A5A: 5200           addq.b  #1, D0
001A5C: 0C00 0007      cmpi.b  #$7, D0
001A60: 6602           bne     $1a64
001A62: 7000           moveq   #$0, D0
001A64: 1B40 7CE8      move.b  D0, ($7ce8,A5)
001A68: 4CDF 0301      movem.l (A7)+, D0/A0-A1
001A6C: 4E75           rts
001A6E: 1B7C 0001 7E8C move.b  #$1, ($7e8c,A5)
001A74: 4A2D 7E8C      tst.b   ($7e8c,A5)
001A78: 66FA           bne     $1a74
001A7A: 4E75           rts
