01B08A: 302E 003C      move.w  ($3c,A6), D0
01B08E: 9068 003C      sub.w   ($3c,A0), D0
01B092: 082E 0000 0015 btst    #$0, ($15,A6)
01B098: 6600 0010      bne     $1b0aa
01B09C: 0C40 0000      cmpi.w  #$0, D0
01B0A0: 6D00 0016      blt     $1b0b8
01B0A4: 023C 00FE      andi    #$fe, CCR
01B0A8: 4E75           rts
01B0AA: 0C40 0000      cmpi.w  #$0, D0
01B0AE: 6E00 0008      bgt     $1b0b8
01B0B2: 023C 00FE      andi    #$fe, CCR
01B0B6: 4E75           rts
01B0B8: 003C 0001      ori     #$1, CCR
01B0BC: 4E75           rts
