0C6C8A: 4EB9 0001 AB6E      jsr     $1ab6e.l
0C6C90: 117C 0000 002A      move.b  #$0, ($2a,A0)
0C6C96: 117C 0000 002B      move.b  #$0, ($2b,A0)
0C6C9C: 217C 0000 0000 002E move.l  #$0, ($2e,A0)
0C6CA4: 317C 0100 0026      move.w  #$100, ($26,A0)
0C6CAA: 317C 0100 0028      move.w  #$100, ($28,A0)
0C6CB0: 317C 0000 0032      move.w  #$0, ($32,A0)
0C6CB6: 317C 0000 0034      move.w  #$0, ($34,A0)
0C6CBC: 317C 0000 0036      move.w  #$0, ($36,A0)
0C6CC2: 317C 0000 0038      move.w  #$0, ($38,A0)
0C6CC8: 317C 0000 003A      move.w  #$0, ($3a,A0)
0C6CCE: 317C 0000 003C      move.w  #$0, ($3c,A0)
0C6CD4: 117C 0006 0082      move.b  #$6, ($82,A0)
0C6CDA: 217C 0000 0000 003E move.l  #$0, ($3e,A0)
0C6CE2: 4E75                rts
0C6CE4: 3140 002C           move.w  D0, ($2c,A0)
0C6CE8: 6100 0352           bsr     $c703c
0C6CEC: 6100 0016           bsr     $c6d04
0C6CF0: 6100 0380           bsr     $c7072
0C6CF4: 13C0 0030 0001      move.b  D0, $300001.l
0C6CFA: 4E75                rts
