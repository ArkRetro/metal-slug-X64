00A03E: 206D 3F64      movea.l ($3f64,A5), A0
00A042: 4E90           jsr     (A0)
00A044: 4A87           tst.l   D7
00A046: 6700 003C      beq     $a084
00A04A: 41ED 1948      lea     ($1948,A5), A0
00A04E: 4A2D 1946      tst.b   ($1946,A5)
00A052: 6A00 0006      bpl     $a05a
00A056: 41ED 294E      lea     ($294e,A5), A0
00A05A: 49E8 0400      lea     ($400,A0), A4
00A05E: 2654           movea.l (A4), A3
00A060: B7CC           cmpa.l  A4, A3
00A062: 6400 0020      bcc     $a084
00A066: 4845           swap    D5
00A068: 3A28 1004      move.w  ($1004,A0), D5
00A06C: 0445 000C      subi.w  #$c, D5
00A070: 3145 1004      move.w  D5, ($1004,A0)
00A074: 0201 0007      andi.b  #$7, D1
00A078: 48F0 0083 5000 movem.l D0-D1/D7, (A0,D5.w)
00A07E: 4845           swap    D5
00A080: 26C5           move.l  D5, (A3)+
00A082: 288B           move.l  A3, (A4)
00A084: 4E75           rts
