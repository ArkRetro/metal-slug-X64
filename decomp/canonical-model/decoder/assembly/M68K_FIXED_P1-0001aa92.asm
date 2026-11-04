01AA92: 2F08           move.l  A0, -(A7)
01AA94: 5247           addq.w  #1, D7
01AA96: D0C0           adda.w  D0, A0
01AA98: 4C90 0003      movem.w (A0), D0-D1
01AA9C: 60CE           bra     $1aa6c
01AA9E: 4A47           tst.w   D7
01AAA0: 6700 0010      beq     $1aab2
01AAA4: 205F           movea.l (A7)+, A0
01AAA6: 5347           subq.w  #1, D7
01AAA8: 3F07           move.w  D7, -(A7)
01AAAA: 6100 FECE      bsr     $1a97a
01AAAE: 3E1F           move.w  (A7)+, D7
01AAB0: 60EE           bra     $1aaa0
01AAB2: 6100 0084      bsr     $1ab38
01AAB6: 303C 0000      move.w  #$0, D0
01AABA: 4EB9 0000 A002 jsr     $a002.l
01AAC0: 4E75           rts
01AAC2: 4BF9 0010 8000 lea     $108000.l, A5
01AAC8: 7E00           moveq   #$0, D7
01AACA: 49F9 003C 0000 lea     $3c0000.l, A4
01AAD0: 0C87 8000 0000 cmpi.l  #-$80000000, D7
01AAD6: 6400 000C      bcc     $1aae4
01AADA: 2887           move.l  D7, (A4)
01AADC: 0687 0001 0000 addi.l  #$10000, D7
01AAE2: 60EC           bra     $1aad0
01AAE4: 4EB9 000C 7A24 jsr     $c7a24.l
01AAEA: 207C 0000 0000 movea.l #$0, A0
01AAF0: 6000 FF36      bra     $1aa28
01AAF4: 4A2D 4338      tst.b   ($4338,A5)
01AAF8: 6600 0020      bne     $1ab1a
01AAFC: 322D 4334      move.w  ($4334,A5), D1
01AB00: 362D 4336      move.w  ($4336,A5), D3
01AB04: 3401           move.w  D1, D2
01AB06: D440           add.w   D0, D2
01AB08: B443           cmp.w   D3, D2
01AB0A: 6500 0004      bcs     $1ab10
01AB0E: 3403           move.w  D3, D2
01AB10: 3B42 4334      move.w  D2, ($4334,A5)
01AB14: 9042           sub.w   D2, D0
01AB16: D041           add.w   D1, D0
01AB18: 4E75           rts
01AB1A: 342D 4336      move.w  ($4336,A5), D2
01AB1E: 362D 4334      move.w  ($4334,A5), D3
01AB22: 3202           move.w  D2, D1
01AB24: 9240           sub.w   D0, D1
01AB26: B243           cmp.w   D3, D1
01AB28: 6400 0004      bcc     $1ab2e
01AB2C: 3203           move.w  D3, D1
01AB2E: 3B41 4336      move.w  D1, ($4336,A5)
01AB32: 9042           sub.w   D2, D0
01AB34: D041           add.w   D1, D0
01AB36: 4E75           rts
01AB38: 322D 4334      move.w  ($4334,A5), D1
01AB3C: 342D 4336      move.w  ($4336,A5), D2
01AB40: 3B42 4334      move.w  D2, ($4334,A5)
01AB44: 3002           move.w  D2, D0
01AB46: 9041           sub.w   D1, D0
01AB48: 4E75           rts
