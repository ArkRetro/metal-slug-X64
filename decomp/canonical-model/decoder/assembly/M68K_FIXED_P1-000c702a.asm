0C702A: 43E8 0088      lea     ($88,A0), A1
0C702E: 3E3C 0400      move.w  #$400, D7
0C7032: 7000           moveq   #$0, D0
0C7034: 22C0           move.l  D0, (A1)+
0C7036: 5347           subq.w  #1, D7
0C7038: 66FA           bne     $c7034
0C703A: 4E75           rts
0C703C: 3E28 002C      move.w  ($2c,A0), D7
0C7040: 6B00 002E      bmi     $c7070
0C7044: 43F9 003C 0000 lea     $3c0000.l, A1
0C704A: 3C3C 03FF      move.w  #$3ff, D6
0C704E: 5247           addq.w  #1, D7
0C7050: ED4F           lsl.w   #6, D7
0C7052: 4847           swap    D7
0C7054: 4247           clr.w   D7
0C7056: 2287           move.l  D7, (A1)
0C7058: 0687 0001 0000 addi.l  #$10000, D7
0C705E: 2287           move.l  D7, (A1)
0C7060: 0687 0001 0000 addi.l  #$10000, D7
0C7066: 51CE FFEE      dbra    D6, $c7056
0C706A: 13C0 0030 0001 move.b  D0, $300001.l
0C7070: 4E75           rts
0C7072: 43E8 0042      lea     ($42,A0), A1
0C7076: 303C FF00      move.w  #$ff00, D0
0C707A: C159           and.w   D0, (A1)+
0C707C: C159           and.w   D0, (A1)+
0C707E: C159           and.w   D0, (A1)+
0C7080: C159           and.w   D0, (A1)+
0C7082: C159           and.w   D0, (A1)+
0C7084: C159           and.w   D0, (A1)+
0C7086: C159           and.w   D0, (A1)+
0C7088: C159           and.w   D0, (A1)+
0C708A: C159           and.w   D0, (A1)+
0C708C: C159           and.w   D0, (A1)+
0C708E: C159           and.w   D0, (A1)+
0C7090: C159           and.w   D0, (A1)+
0C7092: C159           and.w   D0, (A1)+
0C7094: C159           and.w   D0, (A1)+
0C7096: C159           and.w   D0, (A1)+
0C7098: C159           and.w   D0, (A1)+
0C709A: C159           and.w   D0, (A1)+
0C709C: C159           and.w   D0, (A1)+
0C709E: C159           and.w   D0, (A1)+
0C70A0: C159           and.w   D0, (A1)+
0C70A2: C159           and.w   D0, (A1)+
0C70A4: C159           and.w   D0, (A1)+
0C70A6: C159           and.w   D0, (A1)+
0C70A8: C159           and.w   D0, (A1)+
0C70AA: C159           and.w   D0, (A1)+
0C70AC: C159           and.w   D0, (A1)+
0C70AE: C159           and.w   D0, (A1)+
0C70B0: C159           and.w   D0, (A1)+
0C70B2: C159           and.w   D0, (A1)+
0C70B4: C159           and.w   D0, (A1)+
0C70B6: C159           and.w   D0, (A1)+
0C70B8: C159           and.w   D0, (A1)+
0C70BA: 4E75           rts
