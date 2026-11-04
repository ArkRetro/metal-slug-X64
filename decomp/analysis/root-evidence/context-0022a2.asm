0022A2: 46FC 2700           move    #$2700, SR
0022A6: 2078 0010           movea.l $10.w, A0
0022AA: 4ED0                jmp     (A0)
0022AC: 33FC 0006 003C 000C move.w  #$6, $3c000c.l
0022B4: 4A39 0010 FD80      tst.b   $10fd80.l
0022BA: 6B00 0008           bmi     $22c4
0022BE: 4EF9 00C0 0438      jmp     $c00438.l
0022C4: 48E7 FFFE           movem.l D0-D7/A0-A6, -(A7)
0022C8: 4BF9 0010 8000      lea     $108000.l, A5
0022CE: 3F2D 8010           move.w  (-$7ff0,A5), -(A7)
0022D2: 3F2D 8014           move.w  (-$7fec,A5), -(A7)
0022D6: 206D 8006           movea.l (-$7ffa,A5), A0
0022DA: 4E90                jsr     (A0)
0022DC: 4EB9 0000 36CC      jsr     $36cc.l
0022E2: 4EB9 00C0 044A      jsr     $c0044a.l
0022E8: 33FC 0000 003C 0004 move.w  #$0, $3c0004.l
0022F0: 4BF9 0010 8000      lea     $108000.l, A5
0022F6: 3039 003C 0006      move.w  $3c0006.l, D0
0022FC: 0200 0008           andi.b  #$8, D0
002300: 56ED 8002           sne     (-$7ffe,A5)
002304: 4C9F 0003           movem.w (A7)+, D0-D1
002308: 3B40 8014           move.w  D0, (-$7fec,A5)
00230C: 33C0 002F FFF0      move.w  D0, $2ffff0.l
002312: 3B41 8010           move.w  D1, (-$7ff0,A5)
002316: 33C1 003C 0000      move.w  D1, $3c0000.l
00231C: 4CDF 7FFF           movem.l (A7)+, D0-D7/A0-A6
002320: 4E73                rte
002322: 4A39 0010 FD80      tst.b   $10fd80.l
002328: 6B00 000C           bmi     $2336
00232C: 33FC 0002 003C 000C move.w  #$2, $3c000c.l
002334: 4E73                rte
002336: 48E7 0084           movem.l A0/A5, -(A7)
00233A: 4BF9 0010 8000      lea     $108000.l, A5
002340: 3F2D 8010           move.w  (-$7ff0,A5), -(A7)
002344: 3F2D 8014           move.w  (-$7fec,A5), -(A7)
002348: 33FC 0002 003C 000C move.w  #$2, $3c000c.l
002350: 206D 800A           movea.l (-$7ff6,A5), A0
002354: 4E90                jsr     (A0)
002356: 33FC 0000 003C 0004 move.w  #$0, $3c0004.l
00235E: 33D7 0010 0014      move.w  (A7), $100014.l
002364: 33DF 002F FFF0      move.w  (A7)+, $2ffff0.l
00236A: 33D7 0010 0010      move.w  (A7), $100010.l
002370: 33DF 003C 0000      move.w  (A7)+, $3c0000.l
002376: 4CDF 2100           movem.l (A7)+, A0/A5
00237A: 4E73                rte
00237C: 4EF9 0000 3C64      jmp     $3c64.l
