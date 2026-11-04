03CAD2: 4BF9 0010 8000 lea     $108000.l, A5
03CAD8: 41ED 435A      lea     ($435a,A5), A0
03CADC: 4298           clr.l   (A0)+
03CADE: 4250           clr.w   (A0)
03CAE0: 41ED 4360      lea     ($4360,A5), A0
03CAE4: 4298           clr.l   (A0)+
03CAE6: 4250           clr.w   (A0)
03CAE8: 0C40 0003      cmpi.w  #$3, D0
03CAEC: 6500 000A      bcs     $3caf8
03CAF0: 1B7C 0000 4382 move.b  #$0, ($4382,A5)
03CAF6: 4E75           rts
03CAF8: 1B7C 0001 4382 move.b  #$1, ($4382,A5)
03CAFE: D040           add.w   D0, D0
03CB00: D040           add.w   D0, D0
03CB02: 41F9 0003 CF06 lea     $3cf06.l, A0
03CB08: 2B70 0000 4376 move.l  (A0,D0.w), ($4376,A5)
03CB0E: 4240           clr.w   D0
03CB10: 3B40 437A      move.w  D0, ($437a,A5)
03CB14: 3B40 437C      move.w  D0, ($437c,A5)
03CB18: 3B40 4374      move.w  D0, ($4374,A5)
03CB1C: 1B40 437E      move.b  D0, ($437e,A5)
03CB20: 3038 0180      move.w  $180.w, D0
03CB24: 6700 0012      beq     $3cb38
03CB28: 206D 4376      movea.l ($4376,A5), A0
03CB2C: 72FF           moveq   #-$1, D1
03CB2E: 3E3C 01FF      move.w  #$1ff, D7
03CB32: 10C1           move.b  D1, (A0)+
03CB34: 51CF FFFC      dbra    D7, $3cb32
03CB38: 4E75           rts
