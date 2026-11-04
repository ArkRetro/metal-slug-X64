01945C: 226D 42F0      movea.l ($42f0,A5), A1
019460: 2451           movea.l (A1), A2
019462: 4CEE 000F 003C movem.l ($3c,A6), D0-D3
019468: B4FC 0000      cmpa.w  #$0, A2
01946C: 6700 0012      beq     $19480
019470: 90AA 0014      sub.l   ($14,A2), D0
019474: 92AA 0018      sub.l   ($18,A2), D1
019478: 94AA 001C      sub.l   ($1c,A2), D2
01947C: 96AA 0020      sub.l   ($20,A2), D3
019480: 9480           sub.l   D0, D2
019482: 9681           sub.l   D1, D3
019484: 4840           swap    D0
019486: 4841           swap    D1
019488: 3818           move.w  (A0)+, D4
01948A: 3A18           move.w  (A0)+, D5
01948C: 3C18           move.w  (A0)+, D6
01948E: 3E18           move.w  (A0)+, D7
019490: DC44           add.w   D4, D6
019492: 082E 0000 0015 btst    #$0, ($15,A6)
019498: 6700 0008      beq     $194a2
01949C: C946           exg     D4, D6
01949E: 4444           neg.w   D4
0194A0: 4446           neg.w   D6
0194A2: 4A82           tst.l   D2
0194A4: 6700 001E      beq     $194c4
0194A8: 6B00 0010      bmi     $194ba
0194AC: DC40           add.w   D0, D6
0194AE: BC69 0004      cmp.w   ($4,A1), D6
0194B2: 6B00 004A      bmi     $194fe
0194B6: 6000 000C      bra     $194c4
0194BA: D840           add.w   D0, D4
0194BC: B869 0008      cmp.w   ($8,A1), D4
0194C0: 6A00 003C      bpl     $194fe
0194C4: DE45           add.w   D5, D7
0194C6: 082E 0001 0015 btst    #$1, ($15,A6)
0194CC: 6700 0008      beq     $194d6
0194D0: CB47           exg     D5, D7
0194D2: 4445           neg.w   D5
0194D4: 4447           neg.w   D7
0194D6: 4A83           tst.l   D3
0194D8: 6700 001E      beq     $194f8
0194DC: 6B00 0010      bmi     $194ee
0194E0: DE41           add.w   D1, D7
0194E2: BE69 0006      cmp.w   ($6,A1), D7
0194E6: 6B00 0016      bmi     $194fe
0194EA: 6000 000C      bra     $194f8
0194EE: DA41           add.w   D1, D5
0194F0: BA69 000A      cmp.w   ($a,A1), D5
0194F4: 6A00 0008      bpl     $194fe
0194F8: 023C 00FE      andi    #$fe, CCR
0194FC: 4E75           rts
0194FE: 003C 0001      ori     #$1, CCR
019502: 4E75           rts
