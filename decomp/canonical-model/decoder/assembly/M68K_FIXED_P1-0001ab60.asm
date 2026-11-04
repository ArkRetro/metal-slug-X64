01AB60: D0FC 0028 adda.w  #$28, A0
01AB64: 6100 0008 bsr     $1ab6e
01AB68: 90FC 0028 suba.w  #$28, A0
01AB6C: 4E75      rts
01AB6E: 7000      moveq   #$0, D0
01AB70: 2140 0010 move.l  D0, ($10,A0)
01AB74: 2080      move.l  D0, (A0)
01AB76: 2140 0004 move.l  D0, ($4,A0)
01AB7A: 2140 0008 move.l  D0, ($8,A0)
01AB7E: 2140 000C move.l  D0, ($c,A0)
01AB82: 2140 0014 move.l  D0, ($14,A0)
01AB86: 2140 0018 move.l  D0, ($18,A0)
01AB8A: 2140 001C move.l  D0, ($1c,A0)
01AB8E: 2140 0020 move.l  D0, ($20,A0)
01AB92: 3140 0024 move.w  D0, ($24,A0)
01AB96: 4E75      rts
