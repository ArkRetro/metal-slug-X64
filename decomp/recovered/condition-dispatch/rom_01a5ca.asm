; Canonical MAME M68000 decoder, selected bytes match supplied P1.
01A5CA: 7000           moveq   #$0, D0
01A5CC: 1011           move.b  (A1), D0
01A5CE: 3200           move.w  D0, D1
01A5D0: D040           add.w   D0, D0
01A5D2: D040           add.w   D0, D0
01A5D4: 2F08           move.l  A0, -(A7)
01A5D6: 4EBB 0006      jsr     ($6,PC,D0.w)
01A5DA: 205F           movea.l (A7)+, A0
01A5DC: 4E75           rts
01A5DE: 6000 0056      bra     $1a636
01A5E2: 6000 0052      bra     $1a636
01A5E6: 6000 004E      bra     $1a636
01A5EA: 6000 004A      bra     $1a636
01A5EE: 6000 0046      bra     $1a636
01A5F2: 6000 0042      bra     $1a636
01A5F6: 6000 003E      bra     $1a636
01A5FA: 6000 003A      bra     $1a636
01A5FE: 6000 0036      bra     $1a636
01A602: 6000 0040      bra     $1a644
01A606: 6000 0090      bra     $1a698
01A60A: 6000 0092      bra     $1a69e
01A60E: 6000 00AE      bra     $1a6be
01A612: 6000 00CA      bra     $1a6de
01A616: 6000 00EA      bra     $1a702
01A61A: 6000 0106      bra     $1a722
01A61E: 6000 0122      bra     $1a742
01A622: 6000 0142      bra     $1a766
01A626: 6000 0150      bra     $1a778
01A62A: 6000 0164      bra     $1a790
01A62E: 6000 0172      bra     $1a7a2
01A632: 6000 0194      bra     $1a7c8
01A636: 45ED 4326      lea     ($4326,A5), A2
01A63A: 7000           moveq   #$0, D0
01A63C: 1032 1000      move.b  (A2,D1.w), D0
01A640: 6000 002A      bra     $1a66c
01A644: 7000           moveq   #$0, D0
01A646: 7200           moveq   #$0, D1
01A648: 45ED 4326      lea     ($4326,A5), A2
01A64C: 121A           move.b  (A2)+, D1
01A64E: D041           add.w   D1, D0
01A650: 121A           move.b  (A2)+, D1
01A652: D041           add.w   D1, D0
01A654: 121A           move.b  (A2)+, D1
01A656: D041           add.w   D1, D0
01A658: 121A           move.b  (A2)+, D1
01A65A: D041           add.w   D1, D0
01A65C: 121A           move.b  (A2)+, D1
01A65E: D041           add.w   D1, D0
01A660: 121A           move.b  (A2)+, D1
01A662: D041           add.w   D1, D0
01A664: 121A           move.b  (A2)+, D1
01A666: D041           add.w   D1, D0
01A668: 121A           move.b  (A2)+, D1
01A66A: D041           add.w   D1, D0
01A66C: 2F3C 0000 0000 move.l  #$0, -(A7)
01A672: B069 0002      cmp.w   ($2,A1), D0
01A676: 57D7           seq     (A7)
01A678: 52EF 0001      shi     ($1,A7)
01A67C: 55EF 0002      scs     ($2,A7)
01A680: 56EF 0003      sne     ($3,A7)
01A684: 1029 0001      move.b  ($1,A1), D0
01A688: 4A37 0000      tst.b   (A7,D0.w)
01A68C: 6700 0006      beq     $1a694
01A690: 003C 0001      ori     #$1, CCR
01A694: 584F           addq.w  #4, A7
01A696: 4E75           rts
01A698: 003C 0001      ori     #$1, CCR
01A69C: 4E75           rts
01A69E: 4EB9 0001 903A jsr     $1903a.l
01A6A4: 4A47           tst.w   D7
01A6A6: 6700 0146      beq     $1a7ee
01A6AA: D042           add.w   D2, D0
01A6AC: E250           roxr.w  #1, D0
01A6AE: 3A2D 4330      move.w  ($4330,A5), D5
01A6B2: B045           cmp.w   D5, D0
01A6B4: 6A00 0138      bpl     $1a7ee
01A6B8: 003C 0001      ori     #$1, CCR
01A6BC: 4E75           rts
01A6BE: 4EB9 0001 903A jsr     $1903a.l
01A6C4: 4A47           tst.w   D7
01A6C6: 6700 0126      beq     $1a7ee
01A6CA: D042           add.w   D2, D0
01A6CC: E250           roxr.w  #1, D0
01A6CE: 3A2D 4330      move.w  ($4330,A5), D5
01A6D2: B045           cmp.w   D5, D0
01A6D4: 6B00 0118      bmi     $1a7ee
01A6D8: 003C 0001      ori     #$1, CCR
01A6DC: 4E75           rts
01A6DE: 4EB9 0001 903A jsr     $1903a.l
01A6E4: 4A47           tst.w   D7
01A6E6: 6700 0106      beq     $1a7ee
01A6EA: 3A2D 4330      move.w  ($4330,A5), D5
01A6EE: B045           cmp.w   D5, D0
01A6F0: 5AC6           spl     D6
01A6F2: B445           cmp.w   D5, D2
01A6F4: 5AC7           spl     D7
01A6F6: BD07           eor.b   D6, D7
01A6F8: 6A00 00F4      bpl     $1a7ee
01A6FC: 003C 0001      ori     #$1, CCR
01A700: 4E75           rts
01A702: 4EB9 0001 903A jsr     $1903a.l
01A708: 4A47           tst.w   D7
01A70A: 6700 00E2      beq     $1a7ee
01A70E: D243           add.w   D3, D1
01A710: E251           roxr.w  #1, D1
01A712: 3A2D 4332      move.w  ($4332,A5), D5
01A716: B245           cmp.w   D5, D1
01A718: 6B00 00D4      bmi     $1a7ee
01A71C: 003C 0001      ori     #$1, CCR
01A720: 4E75           rts
01A722: 4EB9 0001 903A jsr     $1903a.l
01A728: 4A47           tst.w   D7
01A72A: 6700 00C2      beq     $1a7ee
01A72E: D243           add.w   D3, D1
01A730: E251           roxr.w  #1, D1
01A732: 3A2D 4332      move.w  ($4332,A5), D5
01A736: B245           cmp.w   D5, D1
01A738: 6A00 00B4      bpl     $1a7ee
01A73C: 003C 0001      ori     #$1, CCR
01A740: 4E75           rts
01A742: 4EB9 0001 903A jsr     $1903a.l
01A748: 4A47           tst.w   D7
01A74A: 6700 00A2      beq     $1a7ee
01A74E: 3A2D 4332      move.w  ($4332,A5), D5
01A752: B245           cmp.w   D5, D1
01A754: 5AC6           spl     D6
01A756: B645           cmp.w   D5, D3
01A758: 5AC7           spl     D7
01A75A: BD07           eor.b   D6, D7
01A75C: 6700 0090      beq     $1a7ee
01A760: 003C 0001      ori     #$1, CCR
01A764: 4E75           rts
01A766: 4EB9 0001 903A jsr     $1903a.l
01A76C: 4A47           tst.w   D7
01A76E: 6600 007E      bne     $1a7ee
01A772: 003C 0001      ori     #$1, CCR
01A776: 4E75           rts
01A778: 4EB9 0001 903A jsr     $1903a.l
01A77E: 4A47           tst.w   D7
01A780: 6700 006C      beq     $1a7ee
01A784: 5247           addq.w  #1, D7
01A786: 6700 0066      beq     $1a7ee
01A78A: 003C 0001      ori     #$1, CCR
01A78E: 4E75           rts
01A790: 4EB9 0001 903A jsr     $1903a.l
01A796: 5247           addq.w  #1, D7
01A798: 6600 0054      bne     $1a7ee
01A79C: 003C 0001      ori     #$1, CCR
01A7A0: 4E75           rts
01A7A2: 2F08           move.l  A0, -(A7)
01A7A4: 7000           moveq   #$0, D0
01A7A6: 4EB9 000A AA88 jsr     $aaa88.l
01A7AC: 4A00           tst.b   D0
01A7AE: 6600 000A      bne     $1a7ba
01A7B2: 7001           moveq   #$1, D0
01A7B4: 4EB9 000A AA88 jsr     $aaa88.l
01A7BA: 205F           movea.l (A7)+, A0
01A7BC: 4A00           tst.b   D0
01A7BE: 6700 002E      beq     $1a7ee
01A7C2: 003C 0001      ori     #$1, CCR
01A7C6: 4E75           rts
01A7C8: 2F08           move.l  A0, -(A7)
01A7CA: 7000           moveq   #$0, D0
01A7CC: 4EB9 000A AA88 jsr     $aaa88.l
01A7D2: 4A00           tst.b   D0
01A7D4: 6600 000A      bne     $1a7e0
01A7D8: 7001           moveq   #$1, D0
01A7DA: 4EB9 000A AA88 jsr     $aaa88.l
01A7E0: 205F           movea.l (A7)+, A0
01A7E2: 4A00           tst.b   D0
01A7E4: 6600 0008      bne     $1a7ee
01A7E8: 003C 0001      ori     #$1, CCR
01A7EC: 4E75           rts
01A7EE: 023C 00FE      andi    #$fe, CCR
01A7F2: 4E75           rts
