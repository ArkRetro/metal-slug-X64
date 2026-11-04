0CA812: 4A39 0010 FD82           tst.b   $10fd82.l
0CA818: 6600 0016                bne     $ca830
0CA81C: 13F9 0010 F086 0010 FD88 move.b  $10f086.l, $10fd88.l
0CA826: 13F9 0010 F087 0010 FD8B move.b  $10f087.l, $10fd8b.l
0CA830: 4E75                     rts
0CA832: 41F9 0010 F068           lea     $10f068.l, A0
0CA838: 1179 0010 FD88 001E      move.b  $10fd88.l, ($1e,A0)
0CA840: 1179 0010 FD8B 001F      move.b  $10fd8b.l, ($1f,A0)
0CA848: 4E75                     rts
