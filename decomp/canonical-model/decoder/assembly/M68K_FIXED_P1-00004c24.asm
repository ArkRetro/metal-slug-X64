004C24: 266E 0050           movea.l ($50,A6), A3
004C28: B6FC 0000           cmpa.w  #$0, A3
004C2C: 6700 0178           beq     $4da6
004C30: 6100 FF7A           bsr     $4bac
004C34: 6500 0170           bcs     $4da6
004C38: 49ED E3E4           lea     (-$1c1c,A5), A4
004C3C: 4A2D E3E2           tst.b   (-$1c1e,A5)
004C40: 6A00 0006           bpl     $4c48
004C44: 49ED E596           lea     (-$1a6a,A5), A4
004C48: 3E2C 0100           move.w  ($100,A4), D7
004C4C: 0C47 0040           cmpi.w  #$40, D7
004C50: 6400 0154           bcc     $4da6
004C54: 3C07                move.w  D7, D6
004C56: DC46                add.w   D6, D6
004C58: DC46                add.w   D6, D6
004C5A: 45F4 6000           lea     (A4,D6.w), A2
004C5E: 3C07                move.w  D7, D6
004C60: 4647                not.w   D7
004C62: 0247 0007           andi.w  #$7, D7
004C66: E64E                lsr.w   #3, D6
004C68: 0246 0007           andi.w  #$7, D6
004C6C: 7A00                moveq   #$0, D5
004C6E: 1A2B 0001           move.b  ($1,A3), D5
004C72: 47EB 0002           lea     ($2,A3), A3
004C76: 301B                move.w  (A3)+, D0
004C78: 0C40 8000           cmpi.w  #-$8000, D0
004C7C: 6700 00E6           beq     $4d64
004C80: 321B                move.w  (A3)+, D1
004C82: 341B                move.w  (A3)+, D2
004C84: 361B                move.w  (A3)+, D3
004C86: 082E 0000 0015      btst    #$0, ($15,A6)
004C8C: 6700 0006           beq     $4c94
004C90: D042                add.w   D2, D0
004C92: 4440                neg.w   D0
004C94: D06E 003C           add.w   ($3c,A6), D0
004C98: 082E 0001 0015      btst    #$1, ($15,A6)
004C9E: 6700 0006           beq     $4ca6
004CA2: D243                add.w   D3, D1
004CA4: 4441                neg.w   D1
004CA6: D26E 0040           add.w   ($40,A6), D1
004CAA: 0440 FFE0           subi.w  #-$20, D0
004CAE: 6C00 000A           bge     $4cba
004CB2: D440                add.w   D0, D2
004CB4: 6D00 00A4           blt     $4d5a
004CB8: 4240                clr.w   D0
004CBA: 383C 017F           move.w  #$17f, D4
004CBE: B044                cmp.w   D4, D0
004CC0: 6E00 0098           bgt     $4d5a
004CC4: D440                add.w   D0, D2
004CC6: B444                cmp.w   D4, D2
004CC8: 6F00 0004           ble     $4cce
004CCC: 3404                move.w  D4, D2
004CCE: 9440                sub.w   D0, D2
004CD0: 6700 0088           beq     $4d5a
004CD4: 0441 0108           subi.w  #$108, D1
004CD8: 6C00 000A           bge     $4ce4
004CDC: D641                add.w   D1, D3
004CDE: 6D00 007A           blt     $4d5a
004CE2: 4241                clr.w   D1
004CE4: 383C 00EF           move.w  #$ef, D4
004CE8: B244                cmp.w   D4, D1
004CEA: 6E00 006E           bgt     $4d5a
004CEE: D641                add.w   D1, D3
004CF0: B644                cmp.w   D4, D3
004CF2: 6F00 0004           ble     $4cf8
004CF6: 3604                move.w  D4, D3
004CF8: 9641                sub.w   D1, D3
004CFA: 6700 005E           beq     $4d5a
004CFE: D440                add.w   D0, D2
004D00: D641                add.w   D1, D3
004D02: EA48                lsr.w   #5, D0
004D04: EA49                lsr.w   #5, D1
004D06: EA4A                lsr.w   #5, D2
004D08: EA4B                lsr.w   #5, D3
004D0A: 9440                sub.w   D0, D2
004D0C: 9641                sub.w   D1, D3
004D0E: E748                lsl.w   #3, D0
004D10: E749                lsl.w   #3, D1
004D12: D046                add.w   D6, D0
004D14: D246                add.w   D6, D1
004D16: 43EC 0102           lea     ($102,A4), A1
004D1A: 43F1 0000           lea     (A1,D0.w), A1
004D1E: 0FD1                bset    D7, (A1)
004D20: 43E9 0008           lea     ($8,A1), A1
004D24: 51CA FFF8           dbra    D2, $4d1e
004D28: 43EC 0162           lea     ($162,A4), A1
004D2C: 43F1 1000           lea     (A1,D1.w), A1
004D30: 0FD1                bset    D7, (A1)
004D32: 43E9 0008           lea     ($8,A1), A1
004D36: 51CB FFF8           dbra    D3, $4d30
004D3A: 0045 8000           ori.w   #$8000, D5
004D3E: 0839 0006 0010 F000 btst    #$6, $10f000.l
004D46: 6700 0012           beq     $4d5a
004D4A: 48E7 FFF8           movem.l D0-D7/A0-A4, -(A7)
004D4E: 41EB FFF8           lea     (-$8,A3), A0
004D52: 6100 0080           bsr     $4dd4
004D56: 4CDF 1FFF           movem.l (A7)+, D0-D7/A0-A4
004D5A: 301B                move.w  (A3)+, D0
004D5C: 0C40 8000           cmpi.w  #-$8000, D0
004D60: 6600 FF1E           bne     $4c80
004D64: 47EE 0058           lea     ($58,A6), A3
004D68: 4A2D E3E2           tst.b   (-$1c1e,A5)
004D6C: 6A00 0004           bpl     $4d72
004D70: 528B                addq.l  #1, A3
004D72: 70FF                moveq   #-$1, D0
004D74: 4A45                tst.w   D5
004D76: 6A00 002C           bpl     $4da4
004D7A: 0805 0000           btst    #$0, D5
004D7E: 6700 000A           beq     $4d8a
004D82: 43EC 01A2           lea     ($1a2,A4), A1
004D86: 0FF1 6000           bset    D7, (A1,D6.w)
004D8A: 0805 0001           btst    #$1, D5
004D8E: 6700 000A           beq     $4d9a
004D92: 43EC 01AA           lea     ($1aa,A4), A1
004D96: 0FF1 6000           bset    D7, (A1,D6.w)
004D9A: 248E                move.l  A6, (A2)
004D9C: 302C 0100           move.w  ($100,A4), D0
004DA0: 526C 0100           addq.w  #1, ($100,A4)
004DA4: 1680                move.b  D0, (A3)
004DA6: 7400                moveq   #$0, D2
004DA8: 7600                moveq   #$0, D3
004DAA: 102E 005A           move.b  ($5a,A6), D0
004DAE: 4A00                tst.b   D0
004DB0: 6700 0016           beq     $4dc8
004DB4: 142E 005C           move.b  ($5c,A6), D2
004DB8: 4882                ext.w   D2
004DBA: D442                add.w   D2, D2
004DBC: 162E 005D           move.b  ($5d,A6), D3
004DC0: 4883                ext.w   D3
004DC2: D643                add.w   D3, D3
004DC4: 003C 0001           ori     #$1, CCR
004DC8: 4E75                rts
