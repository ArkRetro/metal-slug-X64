003B6A: 49F9 002F FFE0      lea     $2fffe0.l, A4
003B70: 397C 0050 000A      move.w  #$50, ($a,A4)
003B76: 397C 00A0 000A      move.w  #$a0, ($a,A4)
003B7C: 3B7C 0001 8014      move.w  #$1, (-$7fec,A5)
003B82: 33FC 0001 002F FFF0 move.w  #$1, $2ffff0.l
003B8A: 0C39 0001 002F FFFF cmpi.b  #$1, $2fffff.l
003B92: 66F6                bne     $3b8a
003B94: 7000                moveq   #$0, D0
003B96: 3880                move.w  D0, (A4)
003B98: 3940 0002           move.w  D0, ($2,A4)
003B9C: 3940 0004           move.w  D0, ($4,A4)
003BA0: 3940 0006           move.w  D0, ($6,A4)
003BA4: 41F9 000D EDD2      lea     $dedd2.l, A0
003BAA: 43D4                lea     (A4), A1
003BAC: 303C 0040           move.w  #$40, D0
003BB0: 5340                subq.w  #1, D0
003BB2: 6500 0034           bcs     $3be8
003BB6: 2E18                move.l  (A0)+, D7
003BB8: 721F                moveq   #$1f, D1
003BBA: 0307                btst    D1, D7
003BBC: 56C6                sne     D6
003BBE: 3886                move.w  D6, (A4)
003BC0: 397C 0001 0002      move.w  #$1, ($2,A4)
003BC6: 397C 0000 0002      move.w  #$0, ($2,A4)
003BCC: 397C 0001 0004      move.w  #$1, ($4,A4)
003BD2: 397C 0000 0004      move.w  #$0, ($4,A4)
003BD8: 3A2C 0008           move.w  ($8,A4), D5
003BDC: DB79 0010 F00A      add.w   D5, $10f00a.l
003BE2: 5341                subq.w  #1, D1
003BE4: 64D4                bcc     $3bba
003BE6: 60C8                bra     $3bb0
003BE8: 43F9 000D EDD2      lea     $dedd2.l, A1
003BEE: 397C 0001 0006      move.w  #$1, ($6,A4)
003BF4: 397C 0000 0006      move.w  #$0, ($6,A4)
003BFA: 7000                moveq   #$0, D0
003BFC: 2E18                move.l  (A0)+, D7
003BFE: 2C19                move.l  (A1)+, D6
003C00: E39E                rol.l   #1, D6
003C02: E39F                rol.l   #1, D7
003C04: 3A39 0010 F00A      move.w  $10f00a.l, D5
003C0A: BD45                eor.w   D6, D5
003C0C: DB79 0010 F00A      add.w   D5, $10f00a.l
003C12: 3885                move.w  D5, (A4)
003C14: 397C 0001 0002      move.w  #$1, ($2,A4)
003C1A: 397C 0000 0002      move.w  #$0, ($2,A4)
003C20: 397C 0001 0004      move.w  #$1, ($4,A4)
003C26: 397C 0000 0004      move.w  #$0, ($4,A4)
003C2C: 3A2C 0008           move.w  ($8,A4), D5
003C30: BF45                eor.w   D7, D5
003C32: 0245 0001           andi.w  #$1, D5
003C36: 6600 002C           bne     $3c64
003C3A: 5240                addq.w  #1, D0
003C3C: 0C40 0421           cmpi.w  #$421, D0
003C40: 6400 000C           bcc     $3c4e
003C44: 3200                move.w  D0, D1
003C46: 0241 001F           andi.w  #$1f, D1
003C4A: 67B0                beq     $3bfc
003C4C: 60B2                bra     $3c00
003C4E: 397C 0001 0006      move.w  #$1, ($6,A4)
003C54: 397C 0000 0006      move.w  #$0, ($6,A4)
003C5A: 0279 F000 0010 F00A andi.w  #$f000, $10f00a.l
003C62: 4E75                rts
003C64: 3B7C 0001 8014      move.w  #$1, (-$7fec,A5)
003C6A: 33FC 0001 002F FFF0 move.w  #$1, $2ffff0.l
003C72: 0C39 0001 002F FFFF cmpi.b  #$1, $2fffff.l
003C7A: 66F6                bne     $3c72
003C7C: 46FC 2700           move    #$2700, SR
003C80: 49F9 0040 0000      lea     $400000.l, A4
003C86: 47F9 0022 4000      lea     $224000.l, A3
003C8C: 41F9 0021 12E2      lea     $2112e2.l, A0
003C92: 38FC 0000           move.w  #$0, (A4)+
003C96: 3018                move.w  (A0)+, D0
003C98: D040                add.w   D0, D0
003C9A: 38F3 0000           move.w  (A3,D0.w), (A4)+
003C9E: 3018                move.w  (A0)+, D0
003CA0: D040                add.w   D0, D0
003CA2: 38F3 0000           move.w  (A3,D0.w), (A4)+
003CA6: 3018                move.w  (A0)+, D0
003CA8: D040                add.w   D0, D0
003CAA: 38F3 0000           move.w  (A3,D0.w), (A4)+
003CAE: 3018                move.w  (A0)+, D0
003CB0: D040                add.w   D0, D0
003CB2: 38F3 0000           move.w  (A3,D0.w), (A4)+
003CB6: 3018                move.w  (A0)+, D0
003CB8: D040                add.w   D0, D0
003CBA: 38F3 0000           move.w  (A3,D0.w), (A4)+
003CBE: 3018                move.w  (A0)+, D0
003CC0: D040                add.w   D0, D0
003CC2: 38F3 0000           move.w  (A3,D0.w), (A4)+
003CC6: 3018                move.w  (A0)+, D0
003CC8: D040                add.w   D0, D0
003CCA: 38F3 0000           move.w  (A3,D0.w), (A4)+
003CCE: 3018                move.w  (A0)+, D0
003CD0: D040                add.w   D0, D0
003CD2: 38F3 0000           move.w  (A3,D0.w), (A4)+
003CD6: 3018                move.w  (A0)+, D0
003CD8: D040                add.w   D0, D0
003CDA: 38F3 0000           move.w  (A3,D0.w), (A4)+
003CDE: 3018                move.w  (A0)+, D0
003CE0: D040                add.w   D0, D0
003CE2: 38F3 0000           move.w  (A3,D0.w), (A4)+
003CE6: 3018                move.w  (A0)+, D0
003CE8: D040                add.w   D0, D0
003CEA: 38F3 0000           move.w  (A3,D0.w), (A4)+
003CEE: 3018                move.w  (A0)+, D0
003CF0: D040                add.w   D0, D0
003CF2: 38F3 0000           move.w  (A3,D0.w), (A4)+
003CF6: 3018                move.w  (A0)+, D0
003CF8: D040                add.w   D0, D0
003CFA: 38F3 0000           move.w  (A3,D0.w), (A4)+
003CFE: 3018                move.w  (A0)+, D0
003D00: D040                add.w   D0, D0
003D02: 38F3 0000           move.w  (A3,D0.w), (A4)+
003D06: 3018                move.w  (A0)+, D0
003D08: D040                add.w   D0, D0
003D0A: 38F3 0000           move.w  (A3,D0.w), (A4)+
003D0E: 33FC 0000 0040 1FFE move.w  #$0, $401ffe.l
003D16: 303C 7000           move.w  #$7000, D0
003D1A: 323C 00FF           move.w  #$ff, D1
003D1E: 343C 0028           move.w  #$28, D2
003D22: 363C 0020           move.w  #$20, D3
003D26: 4EB9 0003 CF12      jsr     $3cf12.l
003D2C: 41FA 0010           lea     ($10,PC) ; ($3d3e), A0
003D30: 4EB9 0003 E2A8      jsr     $3e2a8.l
003D36: 13C0 0030 0001      move.b  D0, $300001.l
003D3C: 60F8                bra     $3d36
