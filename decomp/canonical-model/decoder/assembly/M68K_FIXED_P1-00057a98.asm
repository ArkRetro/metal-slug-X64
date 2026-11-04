057A98: 5340      subq.w  #1, D0
057A9A: 6500 0008 bcs     $57aa4
057A9E: 12D8      move.b  (A0)+, (A1)+
057AA0: 51C8 FFFC dbra    D0, $57a9e
057AA4: 4E75      rts
