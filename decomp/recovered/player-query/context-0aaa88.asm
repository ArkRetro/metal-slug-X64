0AAA88: 4A00           tst.b   D0
0AAA8A: 6600 000E      bne     $aaa9a
0AAA8E: 49ED 4608      lea     ($4608,A5), A4
0AAA92: 41ED 94C4      lea     (-$6b3c,A5), A0
0AAA96: 6000 000A      bra     $aaaa2
0AAA9A: 49ED 462C      lea     ($462c,A5), A4
0AAA9E: 41ED 9574      lea     (-$6a8c,A5), A0
0AAAA2: 4EB9 0000 40AC jsr     $40ac.l
0AAAA8: 6400 0008      bcc     $aaab2
0AAAAC: 4A54           tst.w   (A4)
0AAAAE: 6600 0010      bne     $aaac0
0AAAB2: 303C 0000      move.w  #$0, D0
0AAAB6: 7200           moveq   #$0, D1
0AAAB8: 7400           moveq   #$0, D2
0AAABA: 7600           moveq   #$0, D3
0AAABC: 7800           moveq   #$0, D4
0AAABE: 4E75           rts
0AAAC0: 102C 0021      move.b  ($21,A4), D0
0AAAC4: 0C00 0000      cmpi.b  #$0, D0
0AAAC8: 6600 0004      bne     $aaace
0AAACC: 4E4F           trap    #$f
0AAACE: 122C 001F      move.b  ($1f,A4), D1
0AAAD2: 142C 0020      move.b  ($20,A4), D2
0AAAD6: 362C 0018      move.w  ($18,A4), D3
0AAADA: 382C 0014      move.w  ($14,A4), D4
0AAADE: 4E75           rts
0AAAE0: 4A00           tst.b   D0
0AAAE2: 6600 000A      bne     $aaaee
0AAAE6: 49ED 4608      lea     ($4608,A5), A4
0AAAEA: 6000 0006      bra     $aaaf2
0AAAEE: 49ED 462C      lea     ($462c,A5), A4
0AAAF2: 4A54           tst.w   (A4)
0AAAF4: 6700 003E      beq     $aab34
0AAAF8: 102C 0021      move.b  ($21,A4), D0
0AAAFC: 0C00 0001      cmpi.b  #$1, D0
0AAB00: 6700 0040      beq     $aab42
0AAB04: 0C00 0004      cmpi.b  #$4, D0
0AAB08: 6700 0038      beq     $aab42
0AAB0C: 0C00 0003      cmpi.b  #$3, D0
0AAB10: 6700 0030      beq     $aab42
0AAB14: 0C00 0008      cmpi.b  #$8, D0
0AAB18: 6700 0028      beq     $aab42
0AAB1C: 0C00 0009      cmpi.b  #$9, D0
0AAB20: 6700 0020      beq     $aab42
0AAB24: 0C00 000A      cmpi.b  #$a, D0
0AAB28: 6700 0018      beq     $aab42
0AAB2C: 0C00 000B      cmpi.b  #$b, D0
0AAB30: 6700 0010      beq     $aab42
0AAB34: 3E2C 0018      move.w  ($18,A4), D7
0AAB38: 3C2C 0014      move.w  ($14,A4), D6
0AAB3C: 023C 00FE      andi    #$fe, CCR
0AAB40: 4E75           rts
0AAB42: 0C41 0000      cmpi.w  #$0, D1
0AAB46: 6700 0010      beq     $aab58
0AAB4A: 302C 0018      move.w  ($18,A4), D0
0AAB4E: B06C 0014      cmp.w   ($14,A4), D0
0AAB52: 64E0           bcc     $aab34
0AAB54: D36C 0016      add.w   D1, ($16,A4)
0AAB58: 3E2C 0018      move.w  ($18,A4), D7
0AAB5C: 3C2C 0014      move.w  ($14,A4), D6
0AAB60: 003C 0001      ori     #$1, CCR
0AAB64: 4E75           rts
0AAB66: 200E           move.l  A6, D0
0AAB68: B06D 4608      cmp.w   ($4608,A5), D0
0AAB6C: 6600 0006      bne     $aab74
0AAB70: 7000           moveq   #$0, D0
0AAB72: 4E75           rts
0AAB74: B06D 462C      cmp.w   ($462c,A5), D0
0AAB78: 6600 0006      bne     $aab80
0AAB7C: 7001           moveq   #$1, D0
0AAB7E: 4E75           rts
0AAB80: 70FF           moveq   #-$1, D0
0AAB82: 4E75           rts
0AAB84: 302E 000E      move.w  ($e,A6), D0
0AAB88: 41ED 94C4      lea     (-$6b3c,A5), A0
0AAB8C: 4EB9 0000 40AC jsr     $40ac.l
0AAB92: 6400 001A      bcc     $aabae
0AAB96: B06D 4608      cmp.w   ($4608,A5), D0
0AAB9A: 6600 0012      bne     $aabae
0AAB9E: 4A2D 4624      tst.b   ($4624,A5)
0AABA2: 6700 000A      beq     $aabae
0AABA6: 7000           moveq   #$0, D0
0AABA8: 003C 0001      ori     #$1, CCR
0AABAC: 4E75           rts
0AABAE: 41ED 9574      lea     (-$6a8c,A5), A0
0AABB2: 4EB9 0000 40AC jsr     $40ac.l
0AABB8: 6400 001A      bcc     $aabd4
0AABBC: B06D 462C      cmp.w   ($462c,A5), D0
0AABC0: 6600 0012      bne     $aabd4
0AABC4: 4A2D 4648      tst.b   ($4648,A5)
0AABC8: 6700 000A      beq     $aabd4
0AABCC: 7001           moveq   #$1, D0
0AABCE: 003C 0001      ori     #$1, CCR
0AABD2: 4E75           rts
0AABD4: 70FF           moveq   #-$1, D0
0AABD6: 023C 00FE      andi    #$fe, CCR
0AABDA: 4E75           rts
0AABDC: 4A00           tst.b   D0
0AABDE: 6600 000A      bne     $aabea
0AABE2: 49ED 4608      lea     ($4608,A5), A4
0AABE6: 6000 0006      bra     $aabee
