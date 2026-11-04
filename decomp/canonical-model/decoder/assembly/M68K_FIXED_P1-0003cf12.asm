03CF12: 13C0 0030 0001      move.b  D0, $300001.l
03CF18: 49F9 003C 0000      lea     $3c0000.l, A4
03CF1E: 4A42                tst.w   D2
03CF20: 6700 002E           beq     $3cf50
03CF24: 3E00                move.w  D0, D7
03CF26: 4847                swap    D7
03CF28: 3E01                move.w  D1, D7
03CF2A: 3C03                move.w  D3, D6
03CF2C: 6700 001A           beq     $3cf48
03CF30: 0C87 7500 0000      cmpi.l  #$75000000, D7
03CF36: 6500 0004           bcs     $3cf3c
03CF3A: 4E4F                trap    #$f
03CF3C: 2887                move.l  D7, (A4)
03CF3E: 0687 0001 0000      addi.l  #$10000, D7
03CF44: 5346                subq.w  #1, D6
03CF46: 60E4                bra     $3cf2c
03CF48: 0640 0020           addi.w  #$20, D0
03CF4C: 5342                subq.w  #1, D2
03CF4E: 60D0                bra     $3cf20
03CF50: 33FC 0000 003C 0004 move.w  #$0, $3c0004.l
03CF58: 28BC 8200 0000      move.l  #$82000000, (A4)
03CF5E: 4E75                rts
