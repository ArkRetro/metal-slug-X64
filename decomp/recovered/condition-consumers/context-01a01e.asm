01A01E: 43E8 0002      lea     ($2,A0), A1
01A022: 6100 05A6      bsr     $1a5ca
01A026: 6500 001E      bcs     $1a046
01A02A: 4E75           rts
01A02C: 08EE 0007 005F bset    #$7, ($5f,A6)
01A032: 226E 000C      movea.l ($c,A6), A1
01A036: B2FC 0000      cmpa.w  #$0, A1
01A03A: 6700 0008      beq     $1a044
01A03E: 08E9 0007 0060 bset    #$7, ($60,A1)
01A044: 4E75           rts
01A046: 7000           moveq   #$0, D0
01A048: 1028 0001      move.b  ($1,A0), D0
01A04C: D0C0           adda.w  D0, A0
01A04E: 6000 0006      bra     $1a056
01A052: 4E4F           trap    #$f
01A054: 4E75           rts
01A056: 7000           moveq   #$0, D0
