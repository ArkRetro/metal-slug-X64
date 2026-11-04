01A9B0: 7001           moveq   #$1, D0
01A9B2: 4EB9 0000 A002 jsr     $a002.l
01A9B8: 205F           movea.l (A7)+, A0
01A9BA: 4E75           rts
01A9BC: 0C41 0002      cmpi.w  #$2, D1
01A9C0: 6600 004A      bne     $1aa0c
01A9C4: 303C 0020      move.w  #$20, D0
01A9C8: 6100 012A      bsr     $1aaf4
01A9CC: 0C40 0000      cmpi.w  #$0, D0
01A9D0: 6700 0018      beq     $1a9ea
01A9D4: 4E4F           trap    #$f
01A9D6: 7472           moveq   #$72, D2
01A9D8: 6170           bsr     $1aa4a
01A9DA: 2073 7072      movea.l ($72,A3,D7.w), A0
01A9DE: 6974           bvs     $1aa54
01A9E0: 6520           bcs     $1aa02
01A9E2: 6173           bsr     $1aa57
