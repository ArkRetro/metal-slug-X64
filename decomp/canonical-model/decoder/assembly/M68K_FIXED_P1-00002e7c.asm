002E7C: 41FA FB7E lea     (-$482,PC) ; ($29fc), A0
002E80: 7400      moveq   #$0, D2
002E82: 1401      move.b  D1, D2
002E84: E080      asr.l   #8, D0
002E86: 2200      move.l  D0, D1
002E88: D442      add.w   D2, D2
002E8A: C3F0 2000 muls.w  (A0,D2.w), D1
002E8E: D0FC 0080 adda.w  #$80, A0
002E92: C1F0 2000 muls.w  (A0,D2.w), D0
002E96: 4E75      rts
