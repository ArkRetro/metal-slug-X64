0CA54A: 5347      subq.w  #1, D7
0CA54C: 10D9      move.b  (A1)+, (A0)+
0CA54E: 57CF FFFC dbeq    D7, $ca54c
0CA552: 4A47      tst.w   D7
0CA554: 6700 000A beq     $ca560
0CA558: 10FC 0000 move.b  #$0, (A0)+
0CA55C: 51CF FFFA dbra    D7, $ca558
0CA560: 4E75      rts
