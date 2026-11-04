0029C6: 4BF9 0010 8000 lea     $108000.l, A5
0029CC: 49ED 8084      lea     (-$7f7c,A5), A4
0029D0: 2654           movea.l (A4), A3
0029D2: 301B           move.w  (A3)+, D0
0029D4: B7CC           cmpa.l  A4, A3
0029D6: 6500 0006      bcs     $29de
0029DA: 96FC 006E      suba.w  #$6e, A3
0029DE: 288B           move.l  A3, (A4)
0029E0: D6FC 002E      adda.w  #$2e, A3
0029E4: B7CC           cmpa.l  A4, A3
0029E6: 6500 0006      bcs     $29ee
0029EA: 96FC 006E      suba.w  #$6e, A3
0029EE: B153           eor.w   D0, (A3)
0029F0: 4E75           rts
0029F2: 3E00           move.w  D0, D7
0029F4: 61D0           bsr     $29c6
0029F6: C0C7           mulu.w  D7, D0
0029F8: 4840           swap    D0
0029FA: 4E75           rts
