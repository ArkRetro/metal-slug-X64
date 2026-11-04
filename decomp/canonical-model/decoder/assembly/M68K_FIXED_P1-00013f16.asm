013F16: 117C 00FF 006A      move.b  #$ff, ($6a,A0)
013F1C: 4E75                rts
013F1E: 7000                moveq   #$0, D0
013F20: 102E 006A           move.b  ($6a,A6), D0
013F24: 0C00 00FF           cmpi.b  #-$1, D0
013F28: 6700 0030           beq     $13f5a
013F2C: 0C40 0008           cmpi.w  #$8, D0
013F30: 6500 0004           bcs     $13f36
013F34: 4E4F                trap    #$f
013F36: ED48                lsl.w   #6, D0
013F38: 41ED 406E           lea     ($406e,A5), A0
013F3C: 41F0 0000           lea     (A0,D0.w), A0
013F40: 217C 0000 0000 0026 move.l  #$0, ($26,A0)
013F48: 317C 0002 002A      move.w  #$2, ($2a,A0)
013F4E: 1D7C 00FF 006A      move.b  #$ff, ($6a,A6)
013F54: 4EB9 0001 ABCC      jsr     $1abcc.l
013F5A: 4E75                rts
