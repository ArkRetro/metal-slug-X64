01205E: 4239 0010 BFED      clr.b   $10bfed.l
012064: 42B9 0010 BFE8      clr.l   $10bfe8.l
01206A: 4239 0010 BFF0      clr.b   $10bff0.l
012070: 33FC 0000 0010 BFF2 move.w  #$0, $10bff2.l
012078: 4239 0010 BFEE      clr.b   $10bfee.l
01207E: 4239 0010 BFEF      clr.b   $10bfef.l
012084: 13FC 00FF 0010 BFEC move.b  #$ff, $10bfec.l
01208C: 103C 0003           move.b  #$3, D0
012090: 6100 000A           bsr     $1209c
012094: 4BF9 0010 8000      lea     $108000.l, A5
01209A: 4E75                rts
01209C: 0C00 0020           cmpi.b  #$20, D0
0120A0: 6400 0012           bcc     $120b4
0120A4: B039 0010 BFEC      cmp.b   $10bfec.l, D0
0120AA: 6700 0040           beq     $120ec
0120AE: 13C0 0010 BFEC      move.b  D0, $10bfec.l
0120B4: 0C00 0003           cmpi.b  #$3, D0
0120B8: 6600 000C           bne     $120c6
0120BC: 13FC 00FF 0010 BFED move.b  #$ff, $10bfed.l
0120C4: 4E75                rts
0120C6: 41F9 0010 BF68      lea     $10bf68.l, A0
0120CC: 3239 0010 BFEA      move.w  $10bfea.l, D1
0120D2: 1180 1000           move.b  D0, (A0,D1.w)
0120D6: 5241                addq.w  #1, D1
0120D8: 0241 007F           andi.w  #$7f, D1
0120DC: B279 0010 BFE8      cmp.w   $10bfe8.l, D1
0120E2: 6700 000A           beq     $120ee
0120E6: 33C1 0010 BFEA      move.w  D1, $10bfea.l
0120EC: 4E75                rts
0120EE: 13FC 00FF 0010 BFEC move.b  #$ff, $10bfec.l
0120F6: 4E75                rts
0120F8: 0C40 2000           cmpi.w  #$2000, D0
0120FC: 6408                bcc     $12106
0120FE: 0C00 0020           cmpi.b  #$20, D0
012102: 6400 0004           bcc     $12108
012106: 4E75                rts
012108: 3200                move.w  D0, D1
01210A: 4201                clr.b   D1
01210C: 0C41 0100           cmpi.w  #$100, D1
012110: 6600 0004           bne     $12116
012114: 4E75                rts
012116: 0C41 0300           cmpi.w  #$300, D1
01211A: 6600 0004           bne     $12120
01211E: 4E75                rts
012120: 0C41 0200           cmpi.w  #$200, D1
012124: 6600 0004           bne     $1212a
012128: 4E75                rts
01212A: 0C39 0001 0010 0088 cmpi.b  #$1, $100088.l
012132: 6600 003A           bne     $1216e
012136: 0C41 0000           cmpi.w  #$0, D1
01213A: 6600 0004           bne     $12140
01213E: 4E75                rts
012140: 4A39 0010 FD82      tst.b   $10fd82.l
012146: 6700 0018           beq     $12160
01214A: 4A39 0010 FE80      tst.b   $10fe80.l
012150: 6600 000E           bne     $12160
012154: 4A39 00D0 0046      tst.b   $d00046.l
01215A: 6700 0004           beq     $12160
01215E: 4E75                rts
012160: 0C39 0000 0010 FD8D cmpi.b  #$0, $10fd8d.l
012168: 6600 0004           bne     $1216e
01216C: 4E75                rts
01216E: 4A39 0010 BFF0      tst.b   $10bff0.l
012174: 6700 000C           beq     $12182
012178: 0C40 1000           cmpi.w  #$1000, D0
01217C: 6500 0004           bcs     $12182
012180: 4E75                rts
012182: 3F00                move.w  D0, -(A7)
012184: E048                lsr.w   #8, D0
012186: 6100 FF14           bsr     $1209c
01218A: 301F                move.w  (A7)+, D0
01218C: 6000 FF0E           bra     $1209c
