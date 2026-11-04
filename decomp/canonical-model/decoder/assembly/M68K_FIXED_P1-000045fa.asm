0045FA: 7000           moveq   #$0, D0
0045FC: 3140 004E      move.w  D0, ($4e,A0)
004600: 2140 0050      move.l  D0, ($50,A0)
004604: 2140 0054      move.l  D0, ($54,A0)
004608: 1140 005A      move.b  D0, ($5a,A0)
00460C: 317C FFFF 0058 move.w  #$ffff, ($58,A0)
004612: 1140 005E      move.b  D0, ($5e,A0)
004616: 117C 00FF 0017 move.b  #$ff, ($17,A0)
00461C: 117C 00FF 006B move.b  #$ff, ($6b,A0)
004622: 4E75           rts
