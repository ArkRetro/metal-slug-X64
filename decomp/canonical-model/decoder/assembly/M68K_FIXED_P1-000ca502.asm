0CA502: 2F08      move.l  A0, -(A7)
0CA504: 7000      moveq   #$0, D0
0CA506: 1218      move.b  (A0)+, D1
0CA508: 4881      ext.w   D1
0CA50A: D041      add.w   D1, D0
0CA50C: D040      add.w   D0, D0
0CA50E: 6400 0004 bcc     $ca514
0CA512: 4640      not.w   D0
0CA514: 0640 3C5B addi.w  #$3c5b, D0
0CA518: 5347      subq.w  #1, D7
0CA51A: 66EA      bne     $ca506
0CA51C: 205F      movea.l (A7)+, A0
0CA51E: 4E75      rts
