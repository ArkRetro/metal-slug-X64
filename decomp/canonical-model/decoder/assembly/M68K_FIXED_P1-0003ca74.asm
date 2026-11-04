03CA74: 4BF9 0010 8000 lea     $108000.l, A5
03CA7A: 70FF           moveq   #-$1, D0
03CA7C: 6100 0054      bsr     $3cad2
03CA80: 6100 002A      bsr     $3caac
03CA84: 41ED 435A      lea     ($435a,A5), A0
03CA88: 4298           clr.l   (A0)+
03CA8A: 4250           clr.w   (A0)
03CA8C: 41ED 4360      lea     ($4360,A5), A0
03CA90: 4298           clr.l   (A0)+
03CA92: 4250           clr.w   (A0)
03CA94: 41ED 4368      lea     ($4368,A5), A0
03CA98: 4298           clr.l   (A0)+
03CA9A: 4250           clr.w   (A0)
03CA9C: 41ED 436E      lea     ($436e,A5), A0
03CAA0: 4298           clr.l   (A0)+
03CAA2: 4250           clr.w   (A0)
03CAA4: 3B7C 0000 4366 move.w  #$0, ($4366,A5)
03CAAA: 4E75           rts
03CAAC: 3B7C 1002 4380 move.w  #$1002, ($4380,A5)
03CAB2: 4E75           rts
