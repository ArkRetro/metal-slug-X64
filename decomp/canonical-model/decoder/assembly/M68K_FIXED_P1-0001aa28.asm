01AA28: 4BF9 0010 8000 lea     $108000.l, A5
01AA2E: 2F08           move.l  A0, -(A7)
01AA30: 6100 FF2A      bsr     $1a95c
01AA34: 205F           movea.l (A7)+, A0
01AA36: B0FC 0000      cmpa.w  #$0, A0
01AA3A: 6700 0076      beq     $1aab2
01AA3E: 1B7C 00FF 4338 move.b  #$ff, ($4338,A5)
01AA44: 3028 0002      move.w  ($2,A0), D0
01AA48: 0C40 0000      cmpi.w  #$0, D0
01AA4C: 6700 0010      beq     $1aa5e
01AA50: 0C40 0003      cmpi.w  #$3, D0
01AA54: 6700 005C      beq     $1aab2
01AA58: 6100 FF20      bsr     $1a97a
01AA5C: 60E0           bra     $1aa3e
01AA5E: D0D0           adda.w  (A0), A0
01AA60: 7E00           moveq   #$0, D7
01AA62: 1B7C 0000 4338 move.b  #$0, ($4338,A5)
01AA68: 4C90 0003      movem.w (A0), D0-D1
01AA6C: 0C41 0003      cmpi.w  #$3, D1
01AA70: 6700 002C      beq     $1aa9e
01AA74: 6500 001C      bcs     $1aa92
01AA78: 4E4F           trap    #$f
