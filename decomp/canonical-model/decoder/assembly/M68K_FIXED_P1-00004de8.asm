004DE8: 223C FFFF 0000 move.l  #$ffff0000, D1
004DEE: 082E 0002 0015 btst    #$2, ($15,A6)
004DF4: 6700 0006      beq     $4dfc
004DF8: 323C 0004      move.w  #$4, D1
004DFC: 3018           move.w  (A0)+, D0
004DFE: 3418           move.w  (A0)+, D2
004E00: 082E 0000 0015 btst    #$0, ($15,A6)
004E06: 6700 0008      beq     $4e10
004E0A: 4440           neg.w   D0
004E0C: 08C1 0000      bset    #$0, D1
004E10: 082E 0001 0015 btst    #$1, ($15,A6)
004E16: 6700 0008      beq     $4e20
004E1A: 4442           neg.w   D2
004E1C: 08C1 0001      bset    #$1, D1
004E20: D06E 003C      add.w   ($3c,A6), D0
004E24: D46E 0040      add.w   ($40,A6), D2
004E28: 3F00           move.w  D0, -(A7)
004E2A: 3F02           move.w  D2, -(A7)
004E2C: 2F07           move.l  D7, -(A7)
004E2E: 2F08           move.l  A0, -(A7)
004E30: 3F01           move.w  D1, -(A7)
004E32: 4840           swap    D0
004E34: 3002           move.w  D2, D0
004E36: 7AFF           moveq   #-$1, D5
004E38: 4EB9 0000 A03E jsr     $a03e.l
004E3E: 321F           move.w  (A7)+, D1
004E40: 205F           movea.l (A7)+, A0
004E42: 2E1F           move.l  (A7)+, D7
004E44: 3018           move.w  (A0)+, D0
004E46: 3418           move.w  (A0)+, D2
004E48: 0A41 0003      eori.w  #$3, D1
004E4C: 082E 0000 0015 btst    #$0, ($15,A6)
004E52: 6700 0004      beq     $4e58
004E56: 4440           neg.w   D0
004E58: 082E 0001 0015 btst    #$1, ($15,A6)
004E5E: 6700 0004      beq     $4e64
004E62: 4442           neg.w   D2
004E64: D45F           add.w   (A7)+, D2
004E66: D05F           add.w   (A7)+, D0
004E68: 4840           swap    D0
004E6A: 3002           move.w  D2, D0
004E6C: 7AFF           moveq   #-$1, D5
004E6E: 4EB9 0000 A03E jsr     $a03e.l
004E74: 4E75           rts
