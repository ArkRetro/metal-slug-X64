03D244: 3600      move.w  D0, D3
03D246: 6100 FF70 bsr     $3d1b8
03D24A: 0C02 0000 cmpi.b  #$0, D2
03D24E: 6600 0004 bne     $3d254
03D252: 4E75      rts
03D254: 6100 FD44 bsr     $3cf9a
03D258: 60EC      bra     $3d246
03D25A: 3600      move.w  D0, D3
03D25C: 6100 FF5A bsr     $3d1b8
03D260: 0C02 0000 cmpi.b  #$0, D2
03D264: 6600 0004 bne     $3d26a
03D268: 4E75      rts
03D26A: 4E91      jsr     (A1)
03D26C: 60EE      bra     $3d25c
