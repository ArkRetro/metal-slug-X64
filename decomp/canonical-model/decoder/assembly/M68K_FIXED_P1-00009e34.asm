009E34: 3412           move.w  (A2), D2
009E36: 362A 0002      move.w  ($2,A2), D3
009E3A: 3382 3000      move.w  D2, (A1,D3.w)
009E3E: 3383 2002      move.w  D3, ($2,A1,D2.w)
009E42: 343C 1000      move.w  #$1000, D2
009E46: 3631 2000      move.w  (A1,D2.w), D3
009E4A: 3542 0002      move.w  D2, ($2,A2)
009E4E: 3483           move.w  D3, (A2)
009E50: 3380 3002      move.w  D0, ($2,A1,D3.w)
009E54: 3380 2000      move.w  D0, (A1,D2.w)
009E58: 357C FFFF 000A move.w  #$ffff, ($a,A2)
009E5E: 357C FFFF 000C move.w  #$ffff, ($c,A2)
009E64: 157C 0002 000F move.b  #$2, ($f,A2)
009E6A: 4E75           rts
