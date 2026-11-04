005152: 4BF9 0010 8000 lea     $108000.l, A5
005158: 43F9 0010 68EA lea     $1068ea.l, A1
00515E: 2449           movea.l A1, A2
005160: 49F9 0010 78FA lea     $1078fa.l, A4
005166: 3E3C 0100      move.w  #$100, D7
00516A: 3C3C 0004      move.w  #$4, D6
00516E: 3546 0004      move.w  D6, ($4,A2)
005172: 3546 0006      move.w  D6, ($6,A2)
005176: 0646 0010      addi.w  #$10, D6
00517A: 357C 0000 0008 move.w  #$0, ($8,A2)
005180: 357C FFFF 000A move.w  #$ffff, ($a,A2)
005186: 357C FFFF 000C move.w  #$ffff, ($c,A2)
00518C: 157C 0000 000E move.b  #$0, ($e,A2)
005192: 157C 0002 000F move.b  #$2, ($f,A2)
005198: 45EA 0010      lea     ($10,A2), A2
00519C: 4254           clr.w   (A4)
00519E: 49EC 0020      lea     ($20,A4), A4
0051A2: 5347           subq.w  #1, D7
0051A4: 66C8           bne     $516e
0051A6: 303C 1000      move.w  #$1000, D0
0051AA: 3480           move.w  D0, (A2)
0051AC: 3540 0002      move.w  D0, ($2,A2)
0051B0: 3200           move.w  D0, D1
0051B2: 343C 0FF0      move.w  #$ff0, D2
0051B6: 3382 1000      move.w  D2, (A1,D1.w)
0051BA: 3381 2002      move.w  D1, ($2,A1,D2.w)
0051BE: 3202           move.w  D2, D1
0051C0: 0442 0010      subi.w  #$10, D2
0051C4: 64F0           bcc     $51b6
0051C6: 3381 0002      move.w  D1, ($2,A1,D0.w)
0051CA: 3280           move.w  D0, (A1)
0051CC: 7000           moveq   #$0, D0
0051CE: 3B40 191A      move.w  D0, ($191a,A5)
0051D2: 3B40 191C      move.w  D0, ($191c,A5)
0051D6: 3B40 191E      move.w  D0, ($191e,A5)
0051DA: 3B40 1920      move.w  D0, ($1920,A5)
0051DE: 3B40 1922      move.w  D0, ($1922,A5)
0051E2: 3B40 1924      move.w  D0, ($1924,A5)
0051E6: 1B40 192C      move.b  D0, ($192c,A5)
0051EA: 3B40 1942      move.w  D0, ($1942,A5)
0051EE: 1B40 1944      move.b  D0, ($1944,A5)
0051F2: 1B40 1945      move.b  D0, ($1945,A5)
0051F6: 4E75           rts
