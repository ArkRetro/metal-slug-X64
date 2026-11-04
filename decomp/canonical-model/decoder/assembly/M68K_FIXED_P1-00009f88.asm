009F88: 7E00           moveq   #$0, D7
009F8A: 1E2E 001D      move.b  ($1d,A6), D7
009F8E: 5347           subq.w  #1, D7
009F90: 6500 0032      bcs     $9fc4
009F94: 43ED E8EA      lea     (-$1716,A5), A1
009F98: 7000           moveq   #$0, D0
009F9A: 1036 7019      move.b  ($19,A6,D7.w), D0
009F9E: E948           lsl.w   #4, D0
009FA0: 45F1 0000      lea     (A1,D0.w), A2
009FA4: 322A 0008      move.w  ($8,A2), D1
009FA8: 6700 0016      beq     $9fc0
009FAC: 0C41 FFFF      cmpi.w  #-$1, D1
009FB0: 6700 000E      beq     $9fc0
009FB4: 536A 0008      subq.w  #1, ($8,A2)
009FB8: 6600 0006      bne     $9fc0
009FBC: 6100 FE76      bsr     $9e34
009FC0: 5347           subq.w  #1, D7
009FC2: 64D4           bcc     $9f98
009FC4: 1D7C 0000 001D move.b  #$0, ($1d,A6)
009FCA: 4E75           rts
