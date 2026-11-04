0CA6A0: 2F0E                     move.l  A6, -(A7)
0CA6A2: 13FC 0002 0010 FDC4      move.b  #$2, $10fdc4.l
0CA6AA: 33FC 0250 0010 FDCE      move.w  #$250, $10fdce.l
0CA6B2: 13FC 0001 0010 FDD0      move.b  #$1, $10fdd0.l
0CA6BA: 23FC 0010 F068 0010 FDC8 move.l  #$10f068, $10fdc8.l
0CA6C4: 33FC 0022 0010 FDCC      move.w  #$22, $10fdcc.l
0CA6CC: 4EB9 00C0 0468           jsr     $c00468.l
0CA6D2: 2C5F                     movea.l (A7)+, A6
0CA6D4: 4BF9 0010 8000           lea     $108000.l, A5
0CA6DA: 4A39 0010 FDC6           tst.b   $10fdc6.l
0CA6E0: 6600 001C                bne     $ca6fe
0CA6E4: 41ED 7068                lea     ($7068,A5), A0
0CA6E8: 3E3C 0020                move.w  #$20, D7
0CA6EC: 6100 FE14                bsr     $ca502
0CA6F0: B068 0020                cmp.w   ($20,A0), D0
0CA6F4: 6600 0008                bne     $ca6fe
0CA6F8: 003C 0001                ori     #$1, CCR
0CA6FC: 4E75                     rts
0CA6FE: 6100 FE98                bsr     $ca598
0CA702: 023C 00FE                andi    #$fe, CCR
0CA706: 4E75                     rts
