100630: 28BC 0010 0636      move.l  #$100636, (A4)
100636: 4DF9 003C 0002      lea     $3c0002.l, A6
10063C: 203C 8401 0000      move.l  #$84010000, D0
100642: 302C 0018           move.w  ($18,A4), D0
100646: 6100 0172           bsr     $1007ba
10064A: 2D40 FFFE           move.l  D0, (-$2,A6)
10064E: 3D7C 8001 FFFE      move.w  #$8001, (-$2,A6)
100654: 3D7C 0001 0002      move.w  #$1, ($2,A6)
10065A: 3E3C 0011           move.w  #$11, D7
10065E: 3CAC 0014           move.w  ($14,A4), (A6)
100662: 4E71                nop
100664: 51CF FFF8           dbra    D7, $10065e
100668: 197C 0001 0010      move.b  #$1, ($10,A4)
10066E: 28BC 0010 0674      move.l  #$100674, (A4)
100674: 532C 0010           subq.b  #1, ($10,A4)
100678: 641E                bcc     $100698
10067A: 28BC 0010 0636      move.l  #$100636, (A4)
100680: 102C 0014           move.b  ($14,A4), D0
100684: 0600 0001           addi.b  #$1, D0
100688: 0C00 0010           cmpi.b  #$10, D0
10068C: 640C                bcc     $10069a
10068E: 1940 0014           move.b  D0, ($14,A4)
100692: 046C 0009 0018      subi.w  #$9, ($18,A4)
100698: 4E75                rts
10069A: 197C 000C 0010      move.b  #$c, ($10,A4)
1006A0: 28BC 0010 06A6      move.l  #$1006a6, (A4)
1006A6: 042C 0001 0010      subi.b  #$1, ($10,A4)
1006AC: 6502                bcs     $1006b0
1006AE: 4E75                rts
1006B0: 206C 0008           movea.l ($8,A4), A0
1006B4: 117C 00FF 0015      move.b  #$ff, ($15,A0)
1006BA: 4DF9 003C 0002      lea     $3c0002.l, A6
1006C0: 203C 8201 0000      move.l  #$82010000, D0
1006C6: 302C 001A           move.w  ($1a,A4), D0
1006CA: 6100 00F2           bsr     $1007be
1006CE: 2D40 FFFE           move.l  D0, (-$2,A6)
1006D2: 203C 8001 0000      move.l  #$80010000, D0
1006D8: 302C 0014           move.w  ($14,A4), D0
1006DC: 2D40 FFFE           move.l  D0, (-$2,A6)
1006E0: 197C 0000 0010      move.b  #$0, ($10,A4)
1006E6: 28BC 0010 06EC      move.l  #$1006ec, (A4)
1006EC: 532C 0010           subq.b  #1, ($10,A4)
1006F0: 6416                bcc     $100708
1006F2: 066C 0001 001A      addi.w  #$1, ($1a,A4)
1006F8: 042C 0008 0015      subi.b  #$8, ($15,A4)
1006FE: 6500 000A           bcs     $10070a
100702: 28BC 0010 06B0      move.l  #$1006b0, (A4)
100708: 4E75                rts
10070A: 062C 0008 0015      addi.b  #$8, ($15,A4)
100710: 4DF9 003C 0002      lea     $3c0002.l, A6
100716: 2D7C 8201 0000 FFFE move.l  #$82010000, (-$2,A6)
10071E: 203C 8013 0000      move.l  #$80130000, D0
100724: 302C 0014           move.w  ($14,A4), D0
100728: 2D40 FFFE           move.l  D0, (-$2,A6)
10072C: 203C 8213 0000      move.l  #$82130000, D0
100732: 302C 001A           move.w  ($1a,A4), D0
100736: 6100 0086           bsr     $1007be
10073A: 2D40 FFFE           move.l  D0, (-$2,A6)
10073E: 203C 8413 0000      move.l  #$84130000, D0
100744: 302C 0018           move.w  ($18,A4), D0
100748: 6100 0070           bsr     $1007ba
10074C: 2D40 FFFE           move.l  D0, (-$2,A6)
100750: 28BC 0010 0756      move.l  #$100756, (A4)
100756: 4DF9 003C 0002      lea     $3c0002.l, A6
10075C: 203C 8213 0000      move.l  #$82130000, D0
100762: 302C 001A           move.w  ($1a,A4), D0
100766: 6100 0056           bsr     $1007be
10076A: 2D40 FFFE           move.l  D0, (-$2,A6)
10076E: 203C 8013 0000      move.l  #$80130000, D0
100774: 302C 0014           move.w  ($14,A4), D0
100778: 2D40 FFFE           move.l  D0, (-$2,A6)
10077C: 197C 0000 0010      move.b  #$0, ($10,A4)
100782: 28BC 0010 0788      move.l  #$100788, (A4)
100788: 532C 0010           subq.b  #1, ($10,A4)
10078C: 641C                bcc     $1007aa
10078E: 046C 0001 001A      subi.w  #$1, ($1a,A4)
100794: 102C 0015           move.b  ($15,A4), D0
100798: 0600 0008           addi.b  #$8, D0
10079C: 6500 000E           bcs     $1007ac
1007A0: 1940 0015           move.b  D0, ($15,A4)
1007A4: 28BC 0010 0756      move.l  #$100756, (A4)
1007AA: 4E75                rts
1007AC: 206C 0008           movea.l ($8,A4), A0
1007B0: 117C 00FF 0014      move.b  #$ff, ($14,A0)
1007B6: 6000 0120           bra     $1008d8
1007BA: EF40                asl.w   #7, D0
1007BC: 4E75                rts
1007BE: 3F01                move.w  D1, -(A7)
1007C0: 323C 01FF           move.w  #$1ff, D1
1007C4: 9240                sub.w   D0, D1
1007C6: EF41                asl.w   #7, D1
1007C8: 3001                move.w  D1, D0
1007CA: 0040 0004           ori.w   #$4, D0
1007CE: 321F                move.w  (A7)+, D1
1007D0: 4E75                rts
1007D2: 43F9 003C 0002      lea     $3c0002.l, A1
1007D8: 337C 0001 0002      move.w  #$1, ($2,A1)
1007DE: 7E11                moveq   #$11, D7
1007E0: 3340 FFFE           move.w  D0, (-$2,A1)
1007E4: 7C03                moveq   #$3, D6
1007E6: 1418                move.b  (A0)+, D2
1007E8: 3282                move.w  D2, (A1)
1007EA: 4E71                nop
1007EC: 3281                move.w  D1, (A1)
1007EE: 51CE FFF6           dbra    D6, $1007e6
1007F2: 7C1B                moveq   #$1b, D6
1007F4: 143C 00FF           move.b  #$ff, D2
1007F8: 3282                move.w  D2, (A1)
1007FA: 4E71                nop
1007FC: 32BC 0000           move.w  #$0, (A1)
100800: 51CE FFF6           dbra    D6, $1007f8
100804: 0640 0040           addi.w  #$40, D0
100808: 51CF FFD6           dbra    D7, $1007e0
10080C: 4E75                rts
10080E: 49ED 7C00           lea     ($7c00,A5), A4
100812: 4BF9 0010 8000      lea     $108000.l, A5
100818: 2054                movea.l (A4), A0
10081A: 4E90                jsr     (A0)
10081C: 102C 0005           move.b  ($5,A4), D0
100820: 6700 000A           beq     $10082c
100824: 220C                move.l  A4, D1
100826: 1200                move.b  D0, D1
100828: 2841                movea.l D1, A4
10082A: 60EC                bra     $100818
10082C: 4E75                rts
10082E: 4A2D 7CE9           tst.b   ($7ce9,A5)
100832: 6602                bne     $100836
100834: 4E75                rts
100836: 48E7 7020           movem.l D1-D3/A2, -(A7)
10083A: 223C 0010 FC00      move.l  #$10fc00, D1
100840: 244C                movea.l A4, A2
100842: 220C                move.l  A4, D1
100844: B02A 0006           cmp.b   ($6,A2), D0
100848: 6400 003A           bcc     $100884
10084C: 122A 0004           move.b  ($4,A2), D1
100850: 2441                movea.l D1, A2
100852: B02A 0006           cmp.b   ($6,A2), D0
100856: 65F4                bcs     $10084c
100858: 43ED 7CE0           lea     ($7ce0,A5), A1
10085C: 7400                moveq   #$0, D2
10085E: 142D 7CE7           move.b  ($7ce7,A5), D2
100862: 1231 2000           move.b  (A1,D2.w), D1
100866: 2241                movea.l D1, A1
100868: 360A                move.w  A2, D3
10086A: 1343 0004           move.b  D3, ($4,A1)
10086E: 122A 0005           move.b  ($5,A2), D1
100872: 1341 0005           move.b  D1, ($5,A1)
100876: 3609                move.w  A1, D3
100878: 1543 0005           move.b  D3, ($5,A2)
10087C: 2441                movea.l D1, A2
10087E: 1543 0004           move.b  D3, ($4,A2)
100882: 6036                bra     $1008ba
100884: 122A 0005           move.b  ($5,A2), D1
100888: 2441                movea.l D1, A2
10088A: B02A 0006           cmp.b   ($6,A2), D0
10088E: 64F4                bcc     $100884
100890: 43ED 7CE0           lea     ($7ce0,A5), A1
100894: 7400                moveq   #$0, D2
100896: 142D 7CE7           move.b  ($7ce7,A5), D2
10089A: 1231 2000           move.b  (A1,D2.w), D1
10089E: 2241                movea.l D1, A1
1008A0: 360A                move.w  A2, D3
1008A2: 1343 0005           move.b  D3, ($5,A1)
1008A6: 122A 0004           move.b  ($4,A2), D1
1008AA: 1341 0004           move.b  D1, ($4,A1)
1008AE: 3609                move.w  A1, D3
1008B0: 1543 0004           move.b  D3, ($4,A2)
1008B4: 2441                movea.l D1, A2
1008B6: 1543 0005           move.b  D3, ($5,A2)
1008BA: 1340 0006           move.b  D0, ($6,A1)
1008BE: 2288                move.l  A0, (A1)
1008C0: 5202                addq.b  #1, D2
1008C2: 0C42 0007           cmpi.w  #$7, D2
1008C6: 6502                bcs     $1008ca
1008C8: 4202                clr.b   D2
1008CA: 1B42 7CE7           move.b  D2, ($7ce7,A5)
1008CE: 532D 7CE9           subq.b  #1, ($7ce9,A5)
1008D2: 4CDF 040E           movem.l (A7)+, D1-D3/A2
1008D6: 4E75                rts
1008D8: 48E7 80C0           movem.l D0/A0-A1, -(A7)
1008DC: 203C 0010 FC00      move.l  #$10fc00, D0
1008E2: 102C 0004           move.b  ($4,A4), D0
1008E6: 2040                movea.l D0, A0
1008E8: 102C 0005           move.b  ($5,A4), D0
1008EC: 2240                movea.l D0, A1
1008EE: 116C 0005 0005      move.b  ($5,A4), ($5,A0)
1008F4: 136C 0004 0004      move.b  ($4,A4), ($4,A1)
1008FA: 522D 7CE9           addq.b  #1, ($7ce9,A5)
1008FE: 102D 7CE8           move.b  ($7ce8,A5), D0
100902: 5200                addq.b  #1, D0
100904: 0C00 0007           cmpi.b  #$7, D0
100908: 6602                bne     $10090c
10090A: 7000                moveq   #$0, D0
10090C: 1B40 7CE8           move.b  D0, ($7ce8,A5)
100910: 4CDF 0301           movem.l (A7)+, D0/A0-A1
100914: 4E75                rts
100916: 1B7C 0001 7E8C      move.b  #$1, ($7e8c,A5)
10091C: 4A2D 7E8C           tst.b   ($7e8c,A5)
100920: 66FA                bne     $10091c
100922: 4E75                rts
100924: 206C 0018           movea.l ($18,A4), A0
100928: 4A6C 001C           tst.w   ($1c,A4)
10092C: 6602                bne     $100930
10092E: 4E75                rts
100930: 426C 001C           clr.w   ($1c,A4)
100934: 4A2C 0016           tst.b   ($16,A4)
100938: 6600 0016           bne     $100950
10093C: 43F9 0040 01E2      lea     $4001e2.l, A1
100942: 22D8                move.l  (A0)+, (A1)+
100944: 22D8                move.l  (A0)+, (A1)+
100946: 22D8                move.l  (A0)+, (A1)+
100948: 33D0 0040 1FFE      move.w  (A0), $401ffe.l
10094E: 4E75                rts
100950: 43F9 0040 01CE      lea     $4001ce.l, A1
100956: 22D8                move.l  (A0)+, (A1)+
100958: 22D8                move.l  (A0)+, (A1)+
10095A: 22D8                move.l  (A0)+, (A1)+
10095C: 2290                move.l  (A0), (A1)
10095E: 4E75                rts
100960: 43ED 7C00           lea     ($7c00,A5), A1
100964: 7200                moveq   #$0, D1
100966: 1229 001E           move.b  ($1e,A1), D1
10096A: 41F9 0010 0D72      lea     $100d72.l, A0
100970: 7000                moveq   #$0, D0
100972: 1030 1000           move.b  (A0,D1.w), D0
100976: 323C 000E           move.w  #$e, D1
10097A: C0C1                mulu.w  D1, D0
10097C: 0680 0010 0DA2      addi.l  #$100da2, D0
100982: 2340 0018           move.l  D0, ($18,A1)
100986: 337C 0001 001C      move.w  #$1, ($1c,A1)
10098C: 4E75                rts
10098E: 43ED 7C00           lea     ($7c00,A5), A1
100992: 7200                moveq   #$0, D1
100994: 1229 001E           move.b  ($1e,A1), D1
100998: 41F9 0010 0D72      lea     $100d72.l, A0
10099E: 7000                moveq   #$0, D0
1009A0: 1030 1000           move.b  (A0,D1.w), D0
1009A4: 323C 0010           move.w  #$10, D1
1009A8: C0C1                mulu.w  D1, D0
1009AA: 0680 0010 1042      addi.l  #$101042, D0
1009B0: 2340 0018           move.l  D0, ($18,A1)
1009B4: 337C 0001 001C      move.w  #$1, ($1c,A1)
1009BA: 4E75                rts
1009BC: 4A2D 7D82           tst.b   ($7d82,A5)
1009C0: 6B02                bmi     $1009c4
1009C2: 4E75                rts
1009C4: 1039 0032 0000      move.b  $320000.l, D0
1009CA: 5300                subq.b  #1, D0
1009CC: 6700 0052           beq     $100a20
1009D0: 40E7                move    SR, -(A7)
1009D2: 2F01                move.l  D1, -(A7)
1009D4: 13FC 0001 0010 FEDB move.b  #$1, $10fedb.l
1009DC: 46FC 2000           move    #$2000, SR
1009E0: 323C 0007           move.w  #$7, D1
1009E4: 4841                swap    D1
1009E6: 3238 7E8A           move.w  $7e8a.w, D1
1009EA: 5841                addq.w  #4, D1
1009EC: 13FC 0001 0032 0000 move.b  #$1, $320000.l
1009F4: 13C0 0030 0001      move.b  D0, $300001.l
1009FA: 1039 0032 0000      move.b  $320000.l, D0
100A00: 5300                subq.b  #1, D0
100A02: 670C                beq     $100a10
100A04: B278 7E8A           cmp.w   $7e8a.w, D1
100A08: 66EA                bne     $1009f4
100A0A: 4841                swap    D1
100A0C: 51C9 FFD6           dbra    D1, $1009e4
100A10: 221F                move.l  (A7)+, D1
100A12: 46DF                move    (A7)+, SR
100A14: 4EB9 0010 0A54      jsr     $100a54.l
100A1A: 4239 0010 FEDB      clr.b   $10fedb.l
100A20: 4E75                rts
