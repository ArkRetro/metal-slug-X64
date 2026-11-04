001AE6: 43ED 7C00      lea     ($7c00,A5), A1
001AEA: 7200           moveq   #$0, D1
001AEC: 1229 001E      move.b  ($1e,A1), D1
001AF0: 41F9 0010 0D72 lea     $100d72.l, A0
001AF6: 7000           moveq   #$0, D0
001AF8: 1030 1000      move.b  (A0,D1.w), D0
001AFC: 323C 0010      move.w  #$10, D1
001B00: C0C1           mulu.w  D1, D0
001B02: 0680 0010 1042 addi.l  #$101042, D0
001B08: 2340 0018      move.l  D0, ($18,A1)
001B0C: 337C 0001 001C move.w  #$1, ($1c,A1)
001B12: 4E75           rts
