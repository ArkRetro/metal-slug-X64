0103A6: 0640 8201 addi.w  #-$7dff, D0
0103AA: 4840      swap    D0
0103AC: 303C 8000 move.w  #$8000, D0
0103B0: 7401      moveq   #$1, D2
0103B2: 4842      swap    D2
0103B4: 0241 01FF andi.w  #$1ff, D1
0103B8: D241      add.w   D1, D1
0103BA: D241      add.w   D1, D1
0103BC: 4441      neg.w   D1
0103BE: 43FA 0806 lea     ($806,PC) ; ($10bc6), A1
0103C2: 4EF1 1000 jmp     (A1,D1.w)
