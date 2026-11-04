0CA598: 41F9 0010 F068 lea     $10f068.l, A0
0CA59E: 3E3C 0022      move.w  #$22, D7
0CA5A2: 43FA FF91      lea     (-$6f,PC) ; ($ca535), A1
0CA5A6: 61A2           bsr     $ca54a
0CA5A8: 4EBA 0006      jsr     ($6,PC) ; ($ca5b0)
0CA5AC: 6000 0284      bra     $ca832
0CA5B0: 41F9 000C A1B0 lea     $ca1b0.l, A0
0CA5B6: 43F9 0010 F07C lea     $10f07c.l, A1
0CA5BC: 45F9 0010 F080 lea     $10f080.l, A2
0CA5C2: 3E3C 0004      move.w  #$4, D7
0CA5C6: 12D0           move.b  (A0), (A1)+
0CA5C8: 14D8           move.b  (A0)+, (A2)+
0CA5CA: 5347           subq.w  #1, D7
0CA5CC: 66F8           bne     $ca5c6
0CA5CE: 7000           moveq   #$0, D0
0CA5D0: 4EBA 001C      jsr     ($1c,PC) ; ($ca5ee)
0CA5D4: 7001           moveq   #$1, D0
0CA5D6: 4EBA 0016      jsr     ($16,PC) ; ($ca5ee)
0CA5DA: 4E75           rts
