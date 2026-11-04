081E36: 4EB9 0001 AF9A jsr     $1af9a.l
081E3C: 6500 0028      bcs     $81e66
081E40: 4EB9 0001 B162 jsr     $1b162.l
081E46: 3001           move.w  D1, D0
081E48: 906E 0040      sub.w   ($40,A6), D0
081E4C: 7200           moveq   #$0, D1
081E4E: 0640 0010      addi.w  #$10, D0
081E52: 6B00 0012      bmi     $81e66
081E56: 7201           moveq   #$1, D1
081E58: 0C40 0028      cmpi.w  #$28, D0
081E5C: 6E00 0008      bgt     $81e66
081E60: 003C 0001      ori     #$1, CCR
081E64: 4E75           rts
081E66: 023C 00FE      andi    #$fe, CCR
081E6A: 4E75           rts
081E6C: 60D2           bra     $81e40
081E6E: 7000           moveq   #$0, D0
081E70: 4EBA 000A      jsr     ($a,PC) ; ($81e7c)
081E74: 6400 0004      bcc     $81e7a
081E78: 4E75           rts
081E7A: 7001           moveq   #$1, D0
081E7C: 4EB9 0001 AEB0 jsr     $1aeb0.l
081E82: 6400 001A      bcc     $81e9e
081E86: 4EB9 0001 B08A jsr     $1b08a.l
081E8C: 6500 0010      bcs     $81e9e
081E90: 4EBA FFDA      jsr     (-$26,PC) ; ($81e6c)
081E94: 6400 0008      bcc     $81e9e
081E98: 003C 0001      ori     #$1, CCR
081E9C: 4E75           rts
081E9E: 023C 00FE      andi    #$fe, CCR
081EA2: 4E75           rts
081EA4: 4A2E 009A      tst.b   ($9a,A6)
081EA8: 6600 0090      bne     $81f3a
081EAC: 4EB9 0001 AF9A jsr     $1af9a.l
081EB2: 6500 0086      bcs     $81f3a
081EB6: 202E 007A      move.l  ($7a,A6), D0
081EBA: 6600 0028      bne     $81ee4
081EBE: 4EBA FF76      jsr     (-$8a,PC) ; ($81e36)
081EC2: 6500 0076      bcs     $81f3a
081EC6: 2D48 007A      move.l  A0, ($7a,A6)
081ECA: 4A01           tst.b   D1
081ECC: 6600 000C      bne     $81eda
081ED0: 1D7C 0002 009D move.b  #$2, ($9d,A6)
081ED6: 6000 0008      bra     $81ee0
081EDA: 1D7C 0001 009D move.b  #$1, ($9d,A6)
081EE0: 6000 002A      bra     $81f0c
081EE4: 2040           movea.l D0, A0
081EE6: 4EBA FF84      jsr     (-$7c,PC) ; ($81e6c)
081EEA: 6400 0020      bcc     $81f0c
081EEE: 7000           moveq   #$0, D0
081EF0: 2D40 007A      move.l  D0, ($7a,A6)
081EF4: 1D40 0099      move.b  D0, ($99,A6)
081EF8: 2D40 0076      move.l  D0, ($76,A6)
081EFC: 1D7C 0003 009D move.b  #$3, ($9d,A6)
081F02: 1D7C 001E 009A move.b  #$1e, ($9a,A6)
081F08: 6000 0030      bra     $81f3a
081F0C: 202E 006E      move.l  ($6e,A6), D0
081F10: 4A01           tst.b   D1
081F12: 6600 000E      bne     $81f22
081F16: 4480           neg.l   D0
081F18: 1D7C 0002 009C move.b  #$2, ($9c,A6)
081F1E: 6000 0008      bra     $81f28
081F22: 1D7C 0001 009C move.b  #$1, ($9c,A6)
081F28: 90AE 0072      sub.l   ($72,A6), D0
081F2C: E880           asr.l   #4, D0
081F2E: D1AE 0072      add.l   D0, ($72,A6)
081F32: 2D6E 0072 0034 move.l  ($72,A6), ($34,A6)
081F38: 4E75           rts
081F3A: 7000           moveq   #$0, D0
081F3C: 2D40 007A      move.l  D0, ($7a,A6)
081F40: 202E 0072      move.l  ($72,A6), D0
081F44: E880           asr.l   #4, D0
081F46: 91AE 0072      sub.l   D0, ($72,A6)
081F4A: 122E 0099      move.b  ($99,A6), D1
081F4E: 0601 0003      addi.b  #$3, D1
081F52: 1D41 0099      move.b  D1, ($99,A6)
081F56: 203C 0006 0000 move.l  #$60000, D0
081F5C: 4EB9 0000 2E7C jsr     $2e7c.l
081F62: 202E 0076      move.l  ($76,A6), D0
081F66: 2D41 0076      move.l  D1, ($76,A6)
081F6A: 9280           sub.l   D0, D1
081F6C: D2AE 0072      add.l   ($72,A6), D1
081F70: 2D41 0034      move.l  D1, ($34,A6)
081F74: 1D7C 0000 009C move.b  #$0, ($9c,A6)
081F7A: 4E75           rts
081F7C: 536E 0092      subq.w  #1, ($92,A6)
081F80: 6A00 0020      bpl     $81fa2
081F84: 4EBA 0050      jsr     ($50,PC) ; ($81fd6)
081F88: 3D40 0090      move.w  D0, ($90,A6)
081F8C: 303C 003C      move.w  #$3c, D0
081F90: 4EB9 0000 29F2 jsr     $29f2.l
081F96: 0640 003C      addi.w  #$3c, D0
081F9A: 3D40 0092      move.w  D0, ($92,A6)
081F9E: 6000 0034      bra     $81fd4
081FA2: 302E 0090      move.w  ($90,A6), D0
081FA6: 906E 003C      sub.w   ($3c,A6), D0
081FAA: 5BC1           smi     D1
081FAC: 4881           ext.w   D1
081FAE: B340           eor.w   D1, D0
081FB0: 9041           sub.w   D1, D0
081FB2: 4840           swap    D0
081FB4: 4240           clr.w   D0
081FB6: E680           asr.l   #3, D0
081FB8: B0AE 006E      cmp.l   ($6e,A6), D0
081FBC: 6F00 0006      ble     $81fc4
081FC0: 202E 006E      move.l  ($6e,A6), D0
081FC4: 48C1           ext.l   D1
081FC6: B380           eor.l   D1, D0
081FC8: 9081           sub.l   D1, D0
081FCA: 90AE 0030      sub.l   ($30,A6), D0
081FCE: E880           asr.l   #4, D0
081FD0: D1AE 0030      add.l   D0, ($30,A6)
081FD4: 4E75           rts
081FD6: 303C 0060      move.w  #$60, D0
081FDA: 4EB9 0000 29F2 jsr     $29f2.l
081FE0: 082E 0000 0015 btst    #$0, ($15,A6)
081FE6: 6600 000A      bne     $81ff2
081FEA: 0640 00C0      addi.w  #$c0, D0
081FEE: 6000 0006      bra     $81ff6
081FF2: 0640 0020      addi.w  #$20, D0
081FF6: 4E75           rts
081FF8: 0C2E 0001 009D cmpi.b  #$1, ($9d,A6)
081FFE: 6600 0022      bne     $82022
082002: 0C2E 0000 0098 cmpi.b  #$0, ($98,A6)
082008: 6600 0018      bne     $82022
08200C: 227C 0028 074C movea.l #$28074c, A1
082012: 303C 0100      move.w  #$100, D0
082016: 4EB9 0000 4256 jsr     $4256.l
08201C: 1D7C 0000 009D move.b  #$0, ($9d,A6)
082022: 0C2E 0002 009D cmpi.b  #$2, ($9d,A6)
082028: 6600 0022      bne     $8204c
08202C: 0C2E 0000 0098 cmpi.b  #$0, ($98,A6)
082032: 6600 0018      bne     $8204c
082036: 227C 0028 07BC movea.l #$2807bc, A1
08203C: 303C 0100      move.w  #$100, D0
082040: 4EB9 0000 4256 jsr     $4256.l
082046: 1D7C 0000 009D move.b  #$0, ($9d,A6)
08204C: 0C2E 0003 009D cmpi.b  #$3, ($9d,A6)
082052: 6600 0042      bne     $82096
082056: 0C2E 0006 0098 cmpi.b  #$6, ($98,A6)
08205C: 6600 0018      bne     $82076
082060: 227C 0028 0784 movea.l #$280784, A1
082066: 303C 0100      move.w  #$100, D0
08206A: 4EB9 0000 4256 jsr     $4256.l
082070: 1D7C 0000 009D move.b  #$0, ($9d,A6)
082076: 0C2E 0003 0098 cmpi.b  #$3, ($98,A6)
08207C: 6600 0018      bne     $82096
082080: 227C 0028 07F4 movea.l #$2807f4, A1
082086: 303C 0100      move.w  #$100, D0
08208A: 4EB9 0000 4256 jsr     $4256.l
082090: 1D7C 0000 009D move.b  #$0, ($9d,A6)
082096: 4E75           rts
