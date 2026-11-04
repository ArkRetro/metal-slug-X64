01ABC8: 41EE 0028      lea     ($28,A6), A0
01ABCC: 4BF9 0010 8000 lea     $108000.l, A5
01ABD2: 4CE8 000C 0014 movem.l ($14,A0), D2-D3
01ABD8: 2268 0010      movea.l ($10,A0), A1
01ABDC: 4CD0 0003      movem.l (A0), D0-D1
01ABE0: 7E00           moveq   #$0, D7
01ABE2: B2FC 0000      cmpa.w  #$0, A1
01ABE6: 6700 001C      beq     $1ac04
01ABEA: 4CE9 0030 0014 movem.l ($14,A1), D4-D5
01ABF0: 3C01           move.w  D1, D6
01ABF2: DC45           add.w   D5, D6
01ABF4: DF47           addx.w  D7, D7
01ABF6: 3C00           move.w  D0, D6
01ABF8: DC44           add.w   D4, D6
01ABFA: DF47           addx.w  D7, D7
01ABFC: D084           add.l   D4, D0
01ABFE: D285           add.l   D5, D1
01AC00: 8E29 0025      or.b    ($25,A1), D7
01AC04: 1147 0025      move.b  D7, ($25,A0)
01AC08: 48E8 0003 0014 movem.l D0-D1, ($14,A0)
01AC0E: 1A2D 433A      move.b  ($433a,A5), D5
01AC12: 1828 0024      move.b  ($24,A0), D4
01AC16: 6600 000E      bne     $1ac26
01AC1A: 1145 0024      move.b  D5, ($24,A0)
01AC1E: 48E8 0003 001C movem.l D0-D1, ($1c,A0)
01AC24: 4E75           rts
01AC26: BA04           cmp.b   D4, D5
01AC28: 6700 000C      beq     $1ac36
01AC2C: 1145 0024      move.b  D5, ($24,A0)
01AC30: 48E8 000C 001C movem.l D2-D3, ($1c,A0)
01AC36: 4E75           rts
01AC38: 41EE 0028      lea     ($28,A6), A0
01AC3C: 2028 0008      move.l  ($8,A0), D0
01AC40: D190           add.l   D0, (A0)
01AC42: 2028 000C      move.l  ($c,A0), D0
01AC46: D1A8 0004      add.l   D0, ($4,A0)
01AC4A: 6080           bra     $1abcc
