10FC00: 1340 0003           move.b  D0, ($3,A1)
10FC04: 1E39 00D0 0108      move.b  $d00108.l, D7
10FC0A: 1C39 00D0 0047      move.b  $d00047.l, D6
10FC10: 5306                subq.b  #1, D6
10FC12: 7000                moveq   #$0, D0
10FC14: 1200                move.b  D0, D1
10FC16: 1340 001D           move.b  D0, ($1d,A1)
10FC1A: 13C0 0038 0021      move.b  D0, $380021.l
10FC20: 0CB8 4E45 4F2D 0100 cmpi.l  #$4e454f2d, $100.w
10FC28: 6622                bne     $10fc4c
10FC2A: 0C78 0240 0108      cmpi.w  #$240, $108.w
10FC30: 6722                beq     $10fc54
10FC32: 4A01                tst.b   D1
10FC34: 6616                bne     $10fc4c
10FC36: 0C38 0001 0114      cmpi.b  #$1, $114.w
10FC3C: 660E                bne     $10fc4c
10FC3E: 0C78 0001 0108      cmpi.w  #$1, $108.w
10FC44: 6706                beq     $10fc4c
10FC46: 123C 00FF           move.b  #$ff, D1
10FC4A: 1400                move.b  D0, D2
10FC4C: 5200                addq.b  #1, D0
10FC4E: 51CE FFC6           dbra    D6, $10fc16
10FC52: 1002                move.b  D2, D0
10FC54: 13C0 00D0 0058      move.b  D0, $d00058.l
10FC5A: 13C7 0038 0021      move.b  D7, $380021.l
10FC60: 1340 000D           move.b  D0, ($d,A1)
10FC64: 1340 0013           move.b  D0, ($13,A1)
10FC68: 4E75                rts
