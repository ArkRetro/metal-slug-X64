081E08: 4EB9 0001 AF9A jsr     $1af9a.l
081E0E: 6500 001E      bcs     $81e2e
081E12: 3028 003C      move.w  ($3c,A0), D0
081E16: 906E 003C      sub.w   ($3c,A6), D0
081E1A: 6A00 0004      bpl     $81e20
081E1E: 4440           neg.w   D0
081E20: B06E 008A      cmp.w   ($8a,A6), D0
081E24: 6E00 0008      bgt     $81e2e
081E28: 003C 0001      ori     #$1, CCR
081E2C: 4E75           rts
081E2E: 023C 00FE      andi    #$fe, CCR
081E32: 4E75           rts
