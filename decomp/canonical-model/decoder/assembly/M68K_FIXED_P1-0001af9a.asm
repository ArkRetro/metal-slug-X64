01AF9A: 4EBA FEF2      jsr     (-$10e,PC) ; ($1ae8e)
01AF9E: 4A00           tst.b   D0
01AFA0: 6700 004E      beq     $1aff0
01AFA4: 41F9 0010 14C4 lea     $1014c4.l, A0
01AFAA: 43F9 0010 1574 lea     $101574.l, A1
01AFB0: 0C00 0001      cmpi.b  #$1, D0
01AFB4: 6700 002C      beq     $1afe2
01AFB8: 0C00 0002      cmpi.b  #$2, D0
01AFBC: 6700 002A      beq     $1afe8
01AFC0: 302E 003C      move.w  ($3c,A6), D0
01AFC4: 3228 003C      move.w  ($3c,A0), D1
01AFC8: 3429 003C      move.w  ($3c,A1), D2
01AFCC: 9240           sub.w   D0, D1
01AFCE: 6C00 0004      bge     $1afd4
01AFD2: 4441           neg.w   D1
01AFD4: 9440           sub.w   D0, D2
01AFD6: 6C00 0004      bge     $1afdc
01AFDA: 4442           neg.w   D2
01AFDC: B441           cmp.w   D1, D2
01AFDE: 6D00 0008      blt     $1afe8
01AFE2: 023C 00FE      andi    #$fe, CCR
01AFE6: 4E75           rts
01AFE8: 2049           movea.l A1, A0
01AFEA: 023C 00FE      andi    #$fe, CCR
01AFEE: 4E75           rts
01AFF0: 41F9 0010 14C4 lea     $1014c4.l, A0
01AFF6: 003C 0001      ori     #$1, CCR
01AFFA: 4E75           rts
