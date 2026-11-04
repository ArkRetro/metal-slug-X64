01A9EA: 3F01           move.w  D1, -(A7)
01A9EC: 3E28 0004      move.w  ($4,A0), D7
01A9F0: 2068 0006      movea.l ($6,A0), A0
01A9F4: 4A47           tst.w   D7
01A9F6: 6600 0008      bne     $1aa00
01A9FA: 4EB9 000C 6C8A jsr     $c6c8a.l
01AA00: 301F           move.w  (A7)+, D0
01AA02: 4EB9 000C 6CE4 jsr     $c6ce4.l
01AA08: 205F           movea.l (A7)+, A0
01AA0A: 4E75           rts
01AA0C: 4E4F           trap    #$f
