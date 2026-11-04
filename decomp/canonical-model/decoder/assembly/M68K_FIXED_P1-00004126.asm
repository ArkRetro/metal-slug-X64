004126: 4BF9 0010 8000 lea     $108000.l, A5
00412C: 13C0 0030 0001 move.b  D0, $300001.l
004132: 6100 FE5C      bsr     $3f90
004136: 2CBC 0000 3F1E move.l  #$3f1e, (A6)
00413C: 4A6E 0010      tst.w   ($10,A6)
004140: 6600 0008      bne     $414a
004144: 204E           movea.l A6, A0
004146: 6000 FDDC      bra     $3f24
00414A: 6100 FF40      bsr     $408c
00414E: 6500 0036      bcs     $4186
004152: 204E           movea.l A6, A0
004154: 6100 FDCE      bsr     $3f24
004158: 2CBC 0000 3F1E move.l  #$3f1e, (A6)
00415E: 206E 0004      movea.l ($4,A6), A0
004162: 226E 0008      movea.l ($8,A6), A1
004166: 2348 0004      move.l  A0, ($4,A1)
00416A: 2149 0008      move.l  A1, ($8,A0)
00416E: 2D6D E3D4 0008 move.l  (-$1c2c,A5), ($8,A6)
004174: 2B4E E3D4      move.l  A6, (-$1c2c,A5)
004178: 526D E3D8      addq.w  #1, (-$1c28,A5)
00417C: 2C48           movea.l A0, A6
00417E: 0C96 0000 3F1E cmpi.l  #$3f1e, (A6)
004184: 67B6           beq     $413c
004186: 4E75           rts
004188: 619C           bsr     $4126
00418A: 2E6D E3DC      movea.l (-$1c24,A5), A7
00418E: 6000 00BA      bra     $424a
