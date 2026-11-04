004352: 117C 0000 0004 move.b  #$0, ($4,A0)
004358: 7EFF           moveq   #-$1, D7
00435A: 60F2           bra     $434e
00435C: 6100 0006      bsr     $4364
004360: 6000 FF3A      bra     $429c
004364: 7A00           moveq   #$0, D5
004366: 1A19           move.b  (A1)+, D5
004368: 1019           move.b  (A1)+, D0
00436A: 1C19           move.b  (A1)+, D6
00436C: 2E19           move.l  (A1)+, D7
00436E: 082E 0003 005F btst    #$3, ($5f,A6)
004374: 6700 000E      beq     $4384
004378: 082D 0000 800F btst    #$0, (-$7ff1,A5)
00437E: 6700 0004      beq     $4384
004382: 4E75           rts
004384: 4A28 0004      tst.b   ($4,A0)
004388: 6A00 0004      bpl     $438e
00438C: 4E75           rts
00438E: 222E 0012      move.l  ($12,A6), D1
004392: 4880           ext.w   D0
004394: 4886           ext.w   D6
004396: E20D           lsr.b   #1, D5
004398: DD46           addx.w  D6, D6
00439A: E20D           lsr.b   #1, D5
00439C: D140           addx.w  D0, D0
00439E: 0801 0000      btst    #$0, D1
0043A2: 6700 0004      beq     $43a8
0043A6: 4480           neg.l   D0
0043A8: 0801 0001      btst    #$1, D1
0043AC: 6700 0004      beq     $43b2
0043B0: 4446           neg.w   D6
0043B2: D06E 003C      add.w   ($3c,A6), D0
0043B6: DC6E 0040      add.w   ($40,A6), D6
0043BA: 7600           moveq   #$0, D3
0043BC: 142E 004D      move.b  ($4d,A6), D2
0043C0: E20A           lsr.b   #1, D2
0043C2: 9143           subx.w  D3, D0
0043C4: E20A           lsr.b   #1, D2
0043C6: 9D43           subx.w  D3, D6
0043C8: 4840           swap    D0
0043CA: 3006           move.w  D6, D0
0043CC: 1805           move.b  D5, D4
0043CE: 0204 0003      andi.b  #$3, D4
0043D2: B901           eor.b   D4, D1
0043D4: 382E 0016      move.w  ($16,A6), D4
0043D8: E204           asr.b   #1, D4
0043DA: 6B00 000E      bmi     $43ea
0043DE: 6400 000A      bcc     $43ea
0043E2: 1801           move.b  D1, D4
0043E4: 3204           move.w  D4, D1
0043E6: 6000 0016      bra     $43fe
0043EA: E44D           lsr.w   #2, D5
0043EC: 0C45 0004      cmpi.w  #$4, D5
0043F0: 6400 000C      bcc     $43fe
0043F4: 1801           move.b  D1, D4
0043F6: 1236 5019      move.b  ($19,A6,D5.w), D1
0043FA: E149           lsl.w   #8, D1
0043FC: 1204           move.b  D4, D1
0043FE: 3A2E 001E      move.w  ($1e,A6), D5
004402: 48E7 00C0      movem.l A0-A1, -(A7)
004406: 4EB9 0000 A03E jsr     $a03e.l
00440C: 4CDF 0300      movem.l (A7)+, A0-A1
004410: 4E75           rts
004412: 7000           moveq   #$0, D0
004414: 1019           move.b  (A1)+, D0
004416: 3219           move.w  (A1)+, D1
004418: 4871 1000      pea     (A1,D1.w)
00441C: 1036 0000      move.b  (A6,D0.w), D0
004420: E748           lsl.w   #3, D0
004422: B041           cmp.w   D1, D0
004424: 6500 0006      bcs     $442c
004428: 3001           move.w  D1, D0
00442A: 5140           subq.w  #8, D0
00442C: 0C31 0004 0000 cmpi.b  #$4, (A1,D0.w)
004432: 6700 0012      beq     $4446
004436: 4E4F           trap    #$f
004438: 7061           moveq   #$61, D0
00443A: 7420           moveq   #$20, D2
00443C: 696E           bvs     $44ac
00443E: 6465           bcc     $44a5
004440: 7820           moveq   #$20, D4
004442: 6572           bcs     $44b6
004444: 7200           moveq   #$0, D1
004446: 43F1 0001      lea     ($1,A1,D0.w), A1
00444A: 6100 FF18      bsr     $4364
00444E: 225F           movea.l (A7)+, A1
004450: 6000 FE4A      bra     $429c
004454: 2089           move.l  A1, (A0)
004456: 6000 FE44      bra     $429c
00445A: 1019           move.b  (A1)+, D0
00445C: 3219           move.w  (A1)+, D1
00445E: 1D80 1000      move.b  D0, (A6,D1.w)
004462: 60F0           bra     $4454
004464: 7000           moveq   #$0, D0
004466: 1019           move.b  (A1)+, D0
004468: 3219           move.w  (A1)+, D1
00446A: 3D81 0000      move.w  D1, (A6,D0.w)
00446E: 60E4           bra     $4454
004470: 7000           moveq   #$0, D0
004472: 1019           move.b  (A1)+, D0
004474: 2219           move.l  (A1)+, D1
004476: 2D81 0000      move.l  D1, (A6,D0.w)
00447A: 60D8           bra     $4454
00447C: 7000           moveq   #$0, D0
00447E: 1019           move.b  (A1)+, D0
004480: 5336 0000      subq.b  #1, (A6,D0.w)
004484: 60CE           bra     $4454
004486: 43E9 0001      lea     ($1,A1), A1
00448A: 7000           moveq   #$0, D0
00448C: 3019           move.w  (A1)+, D0
00448E: 48E7 00C0      movem.l A0-A1, -(A7)
004492: 4EB9 0001 20F8 jsr     $120f8.l
004498: 4CDF 0300      movem.l (A7)+, A0-A1
00449C: 60B6           bra     $4454
00449E: 43E9 0001      lea     ($1,A1), A1
0044A2: 2251           movea.l (A1), A1
0044A4: 60AE           bra     $4454
0044A6: 1219           move.b  (A1)+, D1
0044A8: 56C1           sne     D1
0044AA: 3019           move.w  (A1)+, D0
0044AC: 2E19           move.l  (A1)+, D7
0044AE: 4A36 0000      tst.b   (A6,D0.w)
0044B2: 56C0           sne     D0
0044B4: B001           cmp.b   D1, D0
0044B6: 669C           bne     $4454
0044B8: 2247           movea.l D7, A1
0044BA: 6098           bra     $4454
0044BC: 7000           moveq   #$0, D0
0044BE: 1019           move.b  (A1)+, D0
0044C0: 3419           move.w  (A1)+, D2
0044C2: 1036 0000      move.b  (A6,D0.w), D0
0044C6: D040           add.w   D0, D0
0044C8: B042           cmp.w   D2, D0
0044CA: 6400 0006      bcc     $44d2
0044CE: D2F1 0000      adda.w  (A1,D0.w), A1
0044D2: 43F1 2000      lea     (A1,D2.w), A1
0044D6: 6000 FF7C      bra     $4454
0044DA: 2F08           move.l  A0, -(A7)
0044DC: 7000           moveq   #$0, D0
0044DE: 1019           move.b  (A1)+, D0
0044E0: 4871 0000      pea     (A1,D0.w)
0044E4: 2E19           move.l  (A1)+, D7
0044E6: 5940           subq.w  #4, D0
0044E8: 2049           movea.l A1, A0
0044EA: 2247           movea.l D7, A1
0044EC: 4E91           jsr     (A1)
0044EE: 225F           movea.l (A7)+, A1
0044F0: 205F           movea.l (A7)+, A0
0044F2: 6000 FF60      bra     $4454
0044F6: 206E 000C      movea.l ($c,A6), A0
0044FA: 1028 0017      move.b  ($17,A0), D0
0044FE: 6700 001A      beq     $451a
004502: 6B00 0004      bmi     $4508
004506: 5200           addq.b  #1, D0
004508: 1D40 0017      move.b  D0, ($17,A6)
00450C: 0800 0000      btst    #$0, D0
004510: 6600 0008      bne     $451a
004514: 003C 0001      ori     #$1, CCR
004518: 4E75           rts
00451A: 023C 00FE      andi    #$fe, CCR
00451E: 4E75           rts
004520: 1F2E 0014      move.b  ($14,A6), -(A7)
004524: 7200           moveq   #$0, D1
004526: 4EB9 0000 9EA8 jsr     $9ea8.l
00452C: 1D6E 0014 0016 move.b  ($14,A6), ($16,A6)
004532: 1D7C 0000 0017 move.b  #$0, ($17,A6)
004538: 1D5F 0014      move.b  (A7)+, ($14,A6)
00453C: 4E75           rts
00453E: 1D6E 0014 0016 move.b  ($14,A6), ($16,A6)
004544: 1D7C 0000 0017 move.b  #$0, ($17,A6)
00454A: 4E75           rts
00454C: 1E2E 0017      move.b  ($17,A6), D7
004550: E207           asr.b   #1, D7
004552: 6B00 0016      bmi     $456a
004556: 6500 0004      bcs     $455c
00455A: 3200           move.w  D0, D1
00455C: 102E 0016      move.b  ($16,A6), D0
004560: 143C 0020      move.b  #$20, D2
