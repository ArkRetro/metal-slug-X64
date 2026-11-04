0035B2: 4840                     swap    D0
0035B4: 223C 0020 0000           move.l  #$200000, D1
0035BA: 303C 0300                move.w  #$300, D0
0035BE: 8EFC 8CA0                divu.w  #$8ca0, D7
0035C2: 1C07                     move.b  D7, D6
0035C4: 1007                     move.b  D7, D0
0035C6: 0600 0030                addi.b  #$30, D0
0035CA: 23C0 003C 0000           move.l  D0, $3c0000.l
0035D0: D081                     add.l   D1, D0
0035D2: 4247                     clr.w   D7
0035D4: 4847                     swap    D7
0035D6: 8EFC 0E10                divu.w  #$e10, D7
0035DA: 8C07                     or.b    D7, D6
0035DC: 1007                     move.b  D7, D0
0035DE: 0600 0030                addi.b  #$30, D0
0035E2: 23C0 003C 0000           move.l  D0, $3c0000.l
0035E8: D081                     add.l   D1, D0
0035EA: 4A06                     tst.b   D6
0035EC: 103C 0027                move.b  #$27, D0
0035F0: 23C0 003C 0000           move.l  D0, $3c0000.l
0035F6: D081                     add.l   D1, D0
0035F8: 4247                     clr.w   D7
0035FA: 4847                     swap    D7
0035FC: 8EFC 0258                divu.w  #$258, D7
003600: 8C07                     or.b    D7, D6
003602: 1007                     move.b  D7, D0
003604: 0600 0030                addi.b  #$30, D0
003608: 23C0 003C 0000           move.l  D0, $3c0000.l
00360E: D081                     add.l   D1, D0
003610: 4247                     clr.w   D7
003612: 4847                     swap    D7
003614: 8EFC 003C                divu.w  #$3c, D7
003618: 1007                     move.b  D7, D0
00361A: 0600 0030                addi.b  #$30, D0
00361E: 23C0 003C 0000           move.l  D0, $3c0000.l
003624: D081                     add.l   D1, D0
003626: 103C 0022                move.b  #$22, D0
00362A: 23C0 003C 0000           move.l  D0, $3c0000.l
003630: D081                     add.l   D1, D0
003632: 4247                     clr.w   D7
003634: 4847                     swap    D7
003636: 8EFC 0006                divu.w  #$6, D7
00363A: 1007                     move.b  D7, D0
00363C: 0600 0030                addi.b  #$30, D0
003640: 23C0 003C 0000           move.l  D0, $3c0000.l
003646: D081                     add.l   D1, D0
003648: 4247                     clr.w   D7
00364A: 4847                     swap    D7
00364C: 5847                     addq.w  #4, D7
00364E: 0C47 000A                cmpi.w  #$a, D7
003652: 6500 0006                bcs     $365a
003656: 0447 000A                subi.w  #$a, D7
00365A: 1007                     move.b  D7, D0
00365C: 0600 0030                addi.b  #$30, D0
003660: 23C0 003C 0000           move.l  D0, $3c0000.l
003666: 4E75                     rts
003668: 4239 0010 F009           clr.b   $10f009.l
00366E: 4239 0010 008C           clr.b   $10008c.l
003674: 4239 0010 008A           clr.b   $10008a.l
00367A: 4239 0010 F008           clr.b   $10f008.l
003680: 4239 0010 008B           clr.b   $10008b.l
003686: 42B9 0010 10A0           clr.l   $1010a0.l
00368C: 4239 0010 109F           clr.b   $10109f.l
003692: 4239 0010 109E           clr.b   $10109e.l
003698: 23FC 0010 109A 0010 109A move.l  #$10109a, $10109a.l
0036A2: 4E75                     rts
