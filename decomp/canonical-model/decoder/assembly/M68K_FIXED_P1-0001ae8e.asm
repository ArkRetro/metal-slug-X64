01AE8E: 7000           moveq   #$0, D0
01AE90: BBFC 0010 8000 cmpa.l  #$108000, A5
01AE96: 6700 0004      beq     $1ae9c
01AE9A: 4E4F           trap    #$f
01AE9C: 102D 433C      move.b  ($433c,A5), D0
01AEA0: 6600 0008      bne     $1aeaa
01AEA4: 003C 0001      ori     #$1, CCR
01AEA8: 4E75           rts
01AEAA: 023C 00FE      andi    #$fe, CCR
01AEAE: 4E75           rts
01AEB0: BBFC 0010 8000 cmpa.l  #$108000, A5
01AEB6: 6700 0004      beq     $1aebc
01AEBA: 4E4F           trap    #$f
01AEBC: 41ED 94C4      lea     (-$6b3c,A5), A0
01AEC0: 0240 0001      andi.w  #$1, D0
01AEC4: 6700 0006      beq     $1aecc
01AEC8: 41ED 9574      lea     (-$6a8c,A5), A0
01AECC: 012D 433C      btst    D0, ($433c,A5)
01AED0: 6700 0006      beq     $1aed8
01AED4: 003C 0001      ori     #$1, CCR
01AED8: 4E75           rts
