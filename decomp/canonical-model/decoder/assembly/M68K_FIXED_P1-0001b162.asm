01B162: B1FC 0000 0000 cmpa.l  #$0, A0
01B168: 6600 000C      bne     $1b176
01B16C: 303C 00A0      move.w  #$a0, D0
01B170: 323C 0180      move.w  #$180, D1
01B174: 4E75           rts
01B176: 3028 003C      move.w  ($3c,A0), D0
01B17A: 3228 0040      move.w  ($40,A0), D1
01B17E: 2268 0050      movea.l ($50,A0), A1
01B182: B3FC 0000 0000 cmpa.l  #$0, A1
01B188: 6700 0034      beq     $1b1be
01B18C: 43E9 0002      lea     ($2,A1), A1
01B190: 3429 0004      move.w  ($4,A1), D2
01B194: 3629 0006      move.w  ($6,A1), D3
01B198: E242           asr.w   #1, D2
01B19A: E243           asr.w   #1, D3
01B19C: D451           add.w   (A1), D2
01B19E: D669 0002      add.w   ($2,A1), D3
01B1A2: 082E 0000 0015 btst    #$0, ($15,A6)
01B1A8: 6700 0004      beq     $1b1ae
01B1AC: 4442           neg.w   D2
01B1AE: 082E 0001 0015 btst    #$1, ($15,A6)
01B1B4: 6700 0004      beq     $1b1ba
01B1B8: 4443           neg.w   D3
01B1BA: D042           add.w   D2, D0
01B1BC: D243           add.w   D3, D1
01B1BE: 4E75           rts
