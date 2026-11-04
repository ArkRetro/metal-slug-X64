01A2D8: 43E8 0002 lea     ($2,A0), A1
01A2DC: 7000      moveq   #$0, D0
01A2DE: 1028 0001 move.b  ($1,A0), D0
01A2E2: D0C0      adda.w  D0, A0
01A2E4: 6100 02E4 bsr     $1a5ca
01A2E8: 6500 FDBC bcs     $1a0a6
01A2EC: 6000 0084 bra     $1a372
