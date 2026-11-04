01A4F2: 43E8 000A      lea     ($a,A0), A1
01A4F6: 6100 00D2      bsr     $1a5ca
01A4FA: 6500 0008      bcs     $1a504
01A4FE: 4EF9 0000 4188 jmp     $4188.l
01A504: 4A6E 0072      tst.w   ($72,A6)
01A508: 6700 0008      beq     $1a512
01A50C: 536E 0072      subq.w  #1, ($72,A6)
01A510: 4E75           rts
01A512: 3D68 0004 0072 move.w  ($4,A0), ($72,A6)
01A518: 7000           moveq   #$0, D0
01A51A: 1028 0001      move.b  ($1,A0), D0
01A51E: D0C0           adda.w  D0, A0
01A520: 1D7C 0000 0074 move.b  #$0, ($74,A6)
01A526: 6100 FB7E      bsr     $1a0a6
01A52A: 4A2E 0074      tst.b   ($74,A6)
