008512: 13C0 0030 0001 move.b  D0, $300001.l
008518: 4A6D 1942      tst.w   ($1942,A5)
00851C: 6600 123C      bne     $975a
008520: 08AD 0007 1945 bclr    #$7, ($1945,A5)
008526: 6700 1212      beq     $973a
00852A: 13C0 003A 000F move.b  D0, $3a000f.l
008530: 41ED F8FA      lea     (-$706,A5), A0
008534: 0250 8000      andi.w  #$8000, (A0)
008538: 43F9 0040 0000 lea     $400000.l, A1
00853E: 7E07           moveq   #$7, D7
008540: 0F90           bclr    D7, (A0)
008542: 6708           beq     $854c
008544: 4CD0 0C3F      movem.l (A0), D0-D5/A2-A3
008548: 48D1 0C3F      movem.l D0-D5/A2-A3, (A1)
00854C: 0FA8 0020      bclr    D7, ($20,A0)
008550: 670C           beq     $855e
008552: 4CE8 0C3F 0020 movem.l ($20,A0), D0-D5/A2-A3
008558: 48E9 0C3F 0020 movem.l D0-D5/A2-A3, ($20,A1)
00855E: 0FA8 0040      bclr    D7, ($40,A0)
008562: 670C           beq     $8570
008564: 4CE8 0C3F 0040 movem.l ($40,A0), D0-D5/A2-A3
00856A: 48E9 0C3F 0040 movem.l D0-D5/A2-A3, ($40,A1)
008570: 0FA8 0060      bclr    D7, ($60,A0)
008574: 670C           beq     $8582
008576: 4CE8 0C3F 0060 movem.l ($60,A0), D0-D5/A2-A3
00857C: 48E9 0C3F 0060 movem.l D0-D5/A2-A3, ($60,A1)
008582: 0FA8 0080      bclr    D7, ($80,A0)
008586: 670C           beq     $8594
008588: 4CE8 0C3F 0080 movem.l ($80,A0), D0-D5/A2-A3
00858E: 48E9 0C3F 0080 movem.l D0-D5/A2-A3, ($80,A1)
008594: 0FA8 00A0      bclr    D7, ($a0,A0)
008598: 670C           beq     $85a6
00859A: 4CE8 0C3F 00A0 movem.l ($a0,A0), D0-D5/A2-A3
0085A0: 48E9 0C3F 00A0 movem.l D0-D5/A2-A3, ($a0,A1)
0085A6: 0FA8 00C0      bclr    D7, ($c0,A0)
0085AA: 670C           beq     $85b8
0085AC: 4CE8 0C3F 00C0 movem.l ($c0,A0), D0-D5/A2-A3
0085B2: 48E9 0C3F 00C0 movem.l D0-D5/A2-A3, ($c0,A1)
0085B8: 0FA8 00E0      bclr    D7, ($e0,A0)
0085BC: 670C           beq     $85ca
0085BE: 4CE8 0C3F 00E0 movem.l ($e0,A0), D0-D5/A2-A3
0085C4: 48E9 0C3F 00E0 movem.l D0-D5/A2-A3, ($e0,A1)
0085CA: 0FA8 0100      bclr    D7, ($100,A0)
0085CE: 670C           beq     $85dc
0085D0: 4CE8 0C3F 0100 movem.l ($100,A0), D0-D5/A2-A3
0085D6: 48E9 0C3F 0100 movem.l D0-D5/A2-A3, ($100,A1)
0085DC: 0FA8 0120      bclr    D7, ($120,A0)
0085E0: 670C           beq     $85ee
0085E2: 4CE8 0C3F 0120 movem.l ($120,A0), D0-D5/A2-A3
0085E8: 48E9 0C3F 0120 movem.l D0-D5/A2-A3, ($120,A1)
0085EE: 0FA8 0140      bclr    D7, ($140,A0)
0085F2: 670C           beq     $8600
0085F4: 4CE8 0C3F 0140 movem.l ($140,A0), D0-D5/A2-A3
0085FA: 48E9 0C3F 0140 movem.l D0-D5/A2-A3, ($140,A1)
008600: 0FA8 0160      bclr    D7, ($160,A0)
008604: 670C           beq     $8612
008606: 4CE8 0C3F 0160 movem.l ($160,A0), D0-D5/A2-A3
00860C: 48E9 0C3F 0160 movem.l D0-D5/A2-A3, ($160,A1)
008612: 0FA8 0180      bclr    D7, ($180,A0)
008616: 670C           beq     $8624
008618: 4CE8 0C3F 0180 movem.l ($180,A0), D0-D5/A2-A3
00861E: 48E9 0C3F 0180 movem.l D0-D5/A2-A3, ($180,A1)
008624: 0FA8 01A0      bclr    D7, ($1a0,A0)
008628: 670C           beq     $8636
00862A: 4CE8 0C3F 01A0 movem.l ($1a0,A0), D0-D5/A2-A3
008630: 48E9 0C3F 01A0 movem.l D0-D5/A2-A3, ($1a0,A1)
008636: 0FA8 01C0      bclr    D7, ($1c0,A0)
00863A: 670C           beq     $8648
00863C: 4CE8 0C3F 01C0 movem.l ($1c0,A0), D0-D5/A2-A3
008642: 48E9 0C3F 01C0 movem.l D0-D5/A2-A3, ($1c0,A1)
008648: 0FA8 01E0      bclr    D7, ($1e0,A0)
00864C: 670C           beq     $865a
00864E: 4CE8 0C3F 01E0 movem.l ($1e0,A0), D0-D5/A2-A3
008654: 48E9 0C3F 01E0 movem.l D0-D5/A2-A3, ($1e0,A1)
00865A: 0FA8 0200      bclr    D7, ($200,A0)
00865E: 670C           beq     $866c
008660: 4CE8 0C3F 0200 movem.l ($200,A0), D0-D5/A2-A3
008666: 48E9 0C3F 0200 movem.l D0-D5/A2-A3, ($200,A1)
00866C: 0FA8 0220      bclr    D7, ($220,A0)
008670: 670C           beq     $867e
008672: 4CE8 0C3F 0220 movem.l ($220,A0), D0-D5/A2-A3
008678: 48E9 0C3F 0220 movem.l D0-D5/A2-A3, ($220,A1)
00867E: 0FA8 0240      bclr    D7, ($240,A0)
008682: 670C           beq     $8690
008684: 4CE8 0C3F 0240 movem.l ($240,A0), D0-D5/A2-A3
00868A: 48E9 0C3F 0240 movem.l D0-D5/A2-A3, ($240,A1)
008690: 0FA8 0260      bclr    D7, ($260,A0)
008694: 670C           beq     $86a2
008696: 4CE8 0C3F 0260 movem.l ($260,A0), D0-D5/A2-A3
00869C: 48E9 0C3F 0260 movem.l D0-D5/A2-A3, ($260,A1)
0086A2: 0FA8 0280      bclr    D7, ($280,A0)
0086A6: 670C           beq     $86b4
0086A8: 4CE8 0C3F 0280 movem.l ($280,A0), D0-D5/A2-A3
0086AE: 48E9 0C3F 0280 movem.l D0-D5/A2-A3, ($280,A1)
0086B4: 0FA8 02A0      bclr    D7, ($2a0,A0)
0086B8: 670C           beq     $86c6
0086BA: 4CE8 0C3F 02A0 movem.l ($2a0,A0), D0-D5/A2-A3
0086C0: 48E9 0C3F 02A0 movem.l D0-D5/A2-A3, ($2a0,A1)
0086C6: 0FA8 02C0      bclr    D7, ($2c0,A0)
0086CA: 670C           beq     $86d8
0086CC: 4CE8 0C3F 02C0 movem.l ($2c0,A0), D0-D5/A2-A3
0086D2: 48E9 0C3F 02C0 movem.l D0-D5/A2-A3, ($2c0,A1)
0086D8: 0FA8 02E0      bclr    D7, ($2e0,A0)
0086DC: 670C           beq     $86ea
0086DE: 4CE8 0C3F 02E0 movem.l ($2e0,A0), D0-D5/A2-A3
0086E4: 48E9 0C3F 02E0 movem.l D0-D5/A2-A3, ($2e0,A1)
0086EA: 0FA8 0300      bclr    D7, ($300,A0)
0086EE: 670C           beq     $86fc
0086F0: 4CE8 0C3F 0300 movem.l ($300,A0), D0-D5/A2-A3
0086F6: 48E9 0C3F 0300 movem.l D0-D5/A2-A3, ($300,A1)
0086FC: 0FA8 0320      bclr    D7, ($320,A0)
008700: 670C           beq     $870e
008702: 4CE8 0C3F 0320 movem.l ($320,A0), D0-D5/A2-A3
008708: 48E9 0C3F 0320 movem.l D0-D5/A2-A3, ($320,A1)
00870E: 0FA8 0340      bclr    D7, ($340,A0)
008712: 670C           beq     $8720
008714: 4CE8 0C3F 0340 movem.l ($340,A0), D0-D5/A2-A3
00871A: 48E9 0C3F 0340 movem.l D0-D5/A2-A3, ($340,A1)
008720: 0FA8 0360      bclr    D7, ($360,A0)
008724: 670C           beq     $8732
008726: 4CE8 0C3F 0360 movem.l ($360,A0), D0-D5/A2-A3
00872C: 48E9 0C3F 0360 movem.l D0-D5/A2-A3, ($360,A1)
008732: 0FA8 0380      bclr    D7, ($380,A0)
008736: 670C           beq     $8744
008738: 4CE8 0C3F 0380 movem.l ($380,A0), D0-D5/A2-A3
00873E: 48E9 0C3F 0380 movem.l D0-D5/A2-A3, ($380,A1)
008744: 0FA8 03A0      bclr    D7, ($3a0,A0)
008748: 670C           beq     $8756
00874A: 4CE8 0C3F 03A0 movem.l ($3a0,A0), D0-D5/A2-A3
008750: 48E9 0C3F 03A0 movem.l D0-D5/A2-A3, ($3a0,A1)
008756: 0FA8 03C0      bclr    D7, ($3c0,A0)
00875A: 670C           beq     $8768
00875C: 4CE8 0C3F 03C0 movem.l ($3c0,A0), D0-D5/A2-A3
008762: 48E9 0C3F 03C0 movem.l D0-D5/A2-A3, ($3c0,A1)
008768: 0FA8 03E0      bclr    D7, ($3e0,A0)
00876C: 670C           beq     $877a
00876E: 4CE8 0C3F 03E0 movem.l ($3e0,A0), D0-D5/A2-A3
008774: 48E9 0C3F 03E0 movem.l D0-D5/A2-A3, ($3e0,A1)
00877A: 0FA8 0400      bclr    D7, ($400,A0)
00877E: 670C           beq     $878c
008780: 4CE8 0C3F 0400 movem.l ($400,A0), D0-D5/A2-A3
008786: 48E9 0C3F 0400 movem.l D0-D5/A2-A3, ($400,A1)
00878C: 0FA8 0420      bclr    D7, ($420,A0)
008790: 670C           beq     $879e
008792: 4CE8 0C3F 0420 movem.l ($420,A0), D0-D5/A2-A3
008798: 48E9 0C3F 0420 movem.l D0-D5/A2-A3, ($420,A1)
00879E: 0FA8 0440      bclr    D7, ($440,A0)
0087A2: 670C           beq     $87b0
0087A4: 4CE8 0C3F 0440 movem.l ($440,A0), D0-D5/A2-A3
0087AA: 48E9 0C3F 0440 movem.l D0-D5/A2-A3, ($440,A1)
0087B0: 0FA8 0460      bclr    D7, ($460,A0)
0087B4: 670C           beq     $87c2
0087B6: 4CE8 0C3F 0460 movem.l ($460,A0), D0-D5/A2-A3
0087BC: 48E9 0C3F 0460 movem.l D0-D5/A2-A3, ($460,A1)
0087C2: 0FA8 0480      bclr    D7, ($480,A0)
0087C6: 670C           beq     $87d4
0087C8: 4CE8 0C3F 0480 movem.l ($480,A0), D0-D5/A2-A3
0087CE: 48E9 0C3F 0480 movem.l D0-D5/A2-A3, ($480,A1)
0087D4: 0FA8 04A0      bclr    D7, ($4a0,A0)
0087D8: 670C           beq     $87e6
0087DA: 4CE8 0C3F 04A0 movem.l ($4a0,A0), D0-D5/A2-A3
0087E0: 48E9 0C3F 04A0 movem.l D0-D5/A2-A3, ($4a0,A1)
0087E6: 0FA8 04C0      bclr    D7, ($4c0,A0)
0087EA: 670C           beq     $87f8
0087EC: 4CE8 0C3F 04C0 movem.l ($4c0,A0), D0-D5/A2-A3
0087F2: 48E9 0C3F 04C0 movem.l D0-D5/A2-A3, ($4c0,A1)
0087F8: 0FA8 04E0      bclr    D7, ($4e0,A0)
0087FC: 670C           beq     $880a
0087FE: 4CE8 0C3F 04E0 movem.l ($4e0,A0), D0-D5/A2-A3
008804: 48E9 0C3F 04E0 movem.l D0-D5/A2-A3, ($4e0,A1)
00880A: 0FA8 0500      bclr    D7, ($500,A0)
00880E: 670C           beq     $881c
008810: 4CE8 0C3F 0500 movem.l ($500,A0), D0-D5/A2-A3
008816: 48E9 0C3F 0500 movem.l D0-D5/A2-A3, ($500,A1)
00881C: 0FA8 0520      bclr    D7, ($520,A0)
008820: 670C           beq     $882e
008822: 4CE8 0C3F 0520 movem.l ($520,A0), D0-D5/A2-A3
008828: 48E9 0C3F 0520 movem.l D0-D5/A2-A3, ($520,A1)
00882E: 0FA8 0540      bclr    D7, ($540,A0)
008832: 670C           beq     $8840
008834: 4CE8 0C3F 0540 movem.l ($540,A0), D0-D5/A2-A3
00883A: 48E9 0C3F 0540 movem.l D0-D5/A2-A3, ($540,A1)
008840: 0FA8 0560      bclr    D7, ($560,A0)
008844: 670C           beq     $8852
008846: 4CE8 0C3F 0560 movem.l ($560,A0), D0-D5/A2-A3
00884C: 48E9 0C3F 0560 movem.l D0-D5/A2-A3, ($560,A1)
008852: 0FA8 0580      bclr    D7, ($580,A0)
008856: 670C           beq     $8864
008858: 4CE8 0C3F 0580 movem.l ($580,A0), D0-D5/A2-A3
00885E: 48E9 0C3F 0580 movem.l D0-D5/A2-A3, ($580,A1)
008864: 0FA8 05A0      bclr    D7, ($5a0,A0)
008868: 670C           beq     $8876
00886A: 4CE8 0C3F 05A0 movem.l ($5a0,A0), D0-D5/A2-A3
008870: 48E9 0C3F 05A0 movem.l D0-D5/A2-A3, ($5a0,A1)
008876: 0FA8 05C0      bclr    D7, ($5c0,A0)
00887A: 670C           beq     $8888
00887C: 4CE8 0C3F 05C0 movem.l ($5c0,A0), D0-D5/A2-A3
008882: 48E9 0C3F 05C0 movem.l D0-D5/A2-A3, ($5c0,A1)
008888: 0FA8 05E0      bclr    D7, ($5e0,A0)
00888C: 670C           beq     $889a
00888E: 4CE8 0C3F 05E0 movem.l ($5e0,A0), D0-D5/A2-A3
008894: 48E9 0C3F 05E0 movem.l D0-D5/A2-A3, ($5e0,A1)
00889A: 0FA8 0600      bclr    D7, ($600,A0)
00889E: 670C           beq     $88ac
0088A0: 4CE8 0C3F 0600 movem.l ($600,A0), D0-D5/A2-A3
0088A6: 48E9 0C3F 0600 movem.l D0-D5/A2-A3, ($600,A1)
0088AC: 0FA8 0620      bclr    D7, ($620,A0)
0088B0: 670C           beq     $88be
0088B2: 4CE8 0C3F 0620 movem.l ($620,A0), D0-D5/A2-A3
0088B8: 48E9 0C3F 0620 movem.l D0-D5/A2-A3, ($620,A1)
0088BE: 0FA8 0640      bclr    D7, ($640,A0)
0088C2: 670C           beq     $88d0
0088C4: 4CE8 0C3F 0640 movem.l ($640,A0), D0-D5/A2-A3
0088CA: 48E9 0C3F 0640 movem.l D0-D5/A2-A3, ($640,A1)
0088D0: 0FA8 0660      bclr    D7, ($660,A0)
0088D4: 670C           beq     $88e2
0088D6: 4CE8 0C3F 0660 movem.l ($660,A0), D0-D5/A2-A3
0088DC: 48E9 0C3F 0660 movem.l D0-D5/A2-A3, ($660,A1)
0088E2: 0FA8 0680      bclr    D7, ($680,A0)
0088E6: 670C           beq     $88f4
0088E8: 4CE8 0C3F 0680 movem.l ($680,A0), D0-D5/A2-A3
0088EE: 48E9 0C3F 0680 movem.l D0-D5/A2-A3, ($680,A1)
0088F4: 0FA8 06A0      bclr    D7, ($6a0,A0)
0088F8: 670C           beq     $8906
0088FA: 4CE8 0C3F 06A0 movem.l ($6a0,A0), D0-D5/A2-A3
008900: 48E9 0C3F 06A0 movem.l D0-D5/A2-A3, ($6a0,A1)
008906: 0FA8 06C0      bclr    D7, ($6c0,A0)
00890A: 670C           beq     $8918
00890C: 4CE8 0C3F 06C0 movem.l ($6c0,A0), D0-D5/A2-A3
008912: 48E9 0C3F 06C0 movem.l D0-D5/A2-A3, ($6c0,A1)
008918: 0FA8 06E0      bclr    D7, ($6e0,A0)
00891C: 670C           beq     $892a
00891E: 4CE8 0C3F 06E0 movem.l ($6e0,A0), D0-D5/A2-A3
008924: 48E9 0C3F 06E0 movem.l D0-D5/A2-A3, ($6e0,A1)
00892A: 0FA8 0700      bclr    D7, ($700,A0)
00892E: 670C           beq     $893c
008930: 4CE8 0C3F 0700 movem.l ($700,A0), D0-D5/A2-A3
008936: 48E9 0C3F 0700 movem.l D0-D5/A2-A3, ($700,A1)
00893C: 0FA8 0720      bclr    D7, ($720,A0)
008940: 670C           beq     $894e
008942: 4CE8 0C3F 0720 movem.l ($720,A0), D0-D5/A2-A3
008948: 48E9 0C3F 0720 movem.l D0-D5/A2-A3, ($720,A1)
00894E: 0FA8 0740      bclr    D7, ($740,A0)
008952: 670C           beq     $8960
008954: 4CE8 0C3F 0740 movem.l ($740,A0), D0-D5/A2-A3
00895A: 48E9 0C3F 0740 movem.l D0-D5/A2-A3, ($740,A1)
008960: 0FA8 0760      bclr    D7, ($760,A0)
008964: 670C           beq     $8972
008966: 4CE8 0C3F 0760 movem.l ($760,A0), D0-D5/A2-A3
00896C: 48E9 0C3F 0760 movem.l D0-D5/A2-A3, ($760,A1)
008972: 0FA8 0780      bclr    D7, ($780,A0)
008976: 670C           beq     $8984
008978: 4CE8 0C3F 0780 movem.l ($780,A0), D0-D5/A2-A3
00897E: 48E9 0C3F 0780 movem.l D0-D5/A2-A3, ($780,A1)
008984: 0FA8 07A0      bclr    D7, ($7a0,A0)
008988: 670C           beq     $8996
00898A: 4CE8 0C3F 07A0 movem.l ($7a0,A0), D0-D5/A2-A3
008990: 48E9 0C3F 07A0 movem.l D0-D5/A2-A3, ($7a0,A1)
008996: 0FA8 07C0      bclr    D7, ($7c0,A0)
00899A: 670C           beq     $89a8
00899C: 4CE8 0C3F 07C0 movem.l ($7c0,A0), D0-D5/A2-A3
0089A2: 48E9 0C3F 07C0 movem.l D0-D5/A2-A3, ($7c0,A1)
0089A8: 0FA8 07E0      bclr    D7, ($7e0,A0)
0089AC: 670C           beq     $89ba
0089AE: 4CE8 0C3F 07E0 movem.l ($7e0,A0), D0-D5/A2-A3
0089B4: 48E9 0C3F 07E0 movem.l D0-D5/A2-A3, ($7e0,A1)
0089BA: 0FA8 0800      bclr    D7, ($800,A0)
0089BE: 670C           beq     $89cc
0089C0: 4CE8 0C3F 0800 movem.l ($800,A0), D0-D5/A2-A3
0089C6: 48E9 0C3F 0800 movem.l D0-D5/A2-A3, ($800,A1)
0089CC: 0FA8 0820      bclr    D7, ($820,A0)
0089D0: 670C           beq     $89de
0089D2: 4CE8 0C3F 0820 movem.l ($820,A0), D0-D5/A2-A3
0089D8: 48E9 0C3F 0820 movem.l D0-D5/A2-A3, ($820,A1)
0089DE: 0FA8 0840      bclr    D7, ($840,A0)
0089E2: 670C           beq     $89f0
0089E4: 4CE8 0C3F 0840 movem.l ($840,A0), D0-D5/A2-A3
0089EA: 48E9 0C3F 0840 movem.l D0-D5/A2-A3, ($840,A1)
0089F0: 0FA8 0860      bclr    D7, ($860,A0)
0089F4: 670C           beq     $8a02
0089F6: 4CE8 0C3F 0860 movem.l ($860,A0), D0-D5/A2-A3
0089FC: 48E9 0C3F 0860 movem.l D0-D5/A2-A3, ($860,A1)
008A02: 0FA8 0880      bclr    D7, ($880,A0)
008A06: 670C           beq     $8a14
008A08: 4CE8 0C3F 0880 movem.l ($880,A0), D0-D5/A2-A3
008A0E: 48E9 0C3F 0880 movem.l D0-D5/A2-A3, ($880,A1)
008A14: 0FA8 08A0      bclr    D7, ($8a0,A0)
008A18: 670C           beq     $8a26
008A1A: 4CE8 0C3F 08A0 movem.l ($8a0,A0), D0-D5/A2-A3
008A20: 48E9 0C3F 08A0 movem.l D0-D5/A2-A3, ($8a0,A1)
008A26: 0FA8 08C0      bclr    D7, ($8c0,A0)
008A2A: 670C           beq     $8a38
008A2C: 4CE8 0C3F 08C0 movem.l ($8c0,A0), D0-D5/A2-A3
008A32: 48E9 0C3F 08C0 movem.l D0-D5/A2-A3, ($8c0,A1)
008A38: 0FA8 08E0      bclr    D7, ($8e0,A0)
008A3C: 670C           beq     $8a4a
008A3E: 4CE8 0C3F 08E0 movem.l ($8e0,A0), D0-D5/A2-A3
008A44: 48E9 0C3F 08E0 movem.l D0-D5/A2-A3, ($8e0,A1)
008A4A: 0FA8 0900      bclr    D7, ($900,A0)
008A4E: 670C           beq     $8a5c
008A50: 4CE8 0C3F 0900 movem.l ($900,A0), D0-D5/A2-A3
008A56: 48E9 0C3F 0900 movem.l D0-D5/A2-A3, ($900,A1)
008A5C: 0FA8 0920      bclr    D7, ($920,A0)
008A60: 670C           beq     $8a6e
008A62: 4CE8 0C3F 0920 movem.l ($920,A0), D0-D5/A2-A3
008A68: 48E9 0C3F 0920 movem.l D0-D5/A2-A3, ($920,A1)
008A6E: 0FA8 0940      bclr    D7, ($940,A0)
008A72: 670C           beq     $8a80
008A74: 4CE8 0C3F 0940 movem.l ($940,A0), D0-D5/A2-A3
008A7A: 48E9 0C3F 0940 movem.l D0-D5/A2-A3, ($940,A1)
008A80: 0FA8 0960      bclr    D7, ($960,A0)
008A84: 670C           beq     $8a92
008A86: 4CE8 0C3F 0960 movem.l ($960,A0), D0-D5/A2-A3
008A8C: 48E9 0C3F 0960 movem.l D0-D5/A2-A3, ($960,A1)
008A92: 0FA8 0980      bclr    D7, ($980,A0)
008A96: 670C           beq     $8aa4
008A98: 4CE8 0C3F 0980 movem.l ($980,A0), D0-D5/A2-A3
008A9E: 48E9 0C3F 0980 movem.l D0-D5/A2-A3, ($980,A1)
008AA4: 0FA8 09A0      bclr    D7, ($9a0,A0)
008AA8: 670C           beq     $8ab6
008AAA: 4CE8 0C3F 09A0 movem.l ($9a0,A0), D0-D5/A2-A3
008AB0: 48E9 0C3F 09A0 movem.l D0-D5/A2-A3, ($9a0,A1)
008AB6: 0FA8 09C0      bclr    D7, ($9c0,A0)
008ABA: 670C           beq     $8ac8
008ABC: 4CE8 0C3F 09C0 movem.l ($9c0,A0), D0-D5/A2-A3
008AC2: 48E9 0C3F 09C0 movem.l D0-D5/A2-A3, ($9c0,A1)
008AC8: 0FA8 09E0      bclr    D7, ($9e0,A0)
008ACC: 670C           beq     $8ada
008ACE: 4CE8 0C3F 09E0 movem.l ($9e0,A0), D0-D5/A2-A3
008AD4: 48E9 0C3F 09E0 movem.l D0-D5/A2-A3, ($9e0,A1)
008ADA: 0FA8 0A00      bclr    D7, ($a00,A0)
008ADE: 670C           beq     $8aec
008AE0: 4CE8 0C3F 0A00 movem.l ($a00,A0), D0-D5/A2-A3
008AE6: 48E9 0C3F 0A00 movem.l D0-D5/A2-A3, ($a00,A1)
008AEC: 0FA8 0A20      bclr    D7, ($a20,A0)
008AF0: 670C           beq     $8afe
008AF2: 4CE8 0C3F 0A20 movem.l ($a20,A0), D0-D5/A2-A3
008AF8: 48E9 0C3F 0A20 movem.l D0-D5/A2-A3, ($a20,A1)
008AFE: 0FA8 0A40      bclr    D7, ($a40,A0)
008B02: 670C           beq     $8b10
008B04: 4CE8 0C3F 0A40 movem.l ($a40,A0), D0-D5/A2-A3
008B0A: 48E9 0C3F 0A40 movem.l D0-D5/A2-A3, ($a40,A1)
008B10: 0FA8 0A60      bclr    D7, ($a60,A0)
008B14: 670C           beq     $8b22
008B16: 4CE8 0C3F 0A60 movem.l ($a60,A0), D0-D5/A2-A3
008B1C: 48E9 0C3F 0A60 movem.l D0-D5/A2-A3, ($a60,A1)
008B22: 0FA8 0A80      bclr    D7, ($a80,A0)
008B26: 670C           beq     $8b34
008B28: 4CE8 0C3F 0A80 movem.l ($a80,A0), D0-D5/A2-A3
008B2E: 48E9 0C3F 0A80 movem.l D0-D5/A2-A3, ($a80,A1)
008B34: 0FA8 0AA0      bclr    D7, ($aa0,A0)
008B38: 670C           beq     $8b46
008B3A: 4CE8 0C3F 0AA0 movem.l ($aa0,A0), D0-D5/A2-A3
008B40: 48E9 0C3F 0AA0 movem.l D0-D5/A2-A3, ($aa0,A1)
008B46: 0FA8 0AC0      bclr    D7, ($ac0,A0)
008B4A: 670C           beq     $8b58
008B4C: 4CE8 0C3F 0AC0 movem.l ($ac0,A0), D0-D5/A2-A3
008B52: 48E9 0C3F 0AC0 movem.l D0-D5/A2-A3, ($ac0,A1)
008B58: 0FA8 0AE0      bclr    D7, ($ae0,A0)
008B5C: 670C           beq     $8b6a
008B5E: 4CE8 0C3F 0AE0 movem.l ($ae0,A0), D0-D5/A2-A3
008B64: 48E9 0C3F 0AE0 movem.l D0-D5/A2-A3, ($ae0,A1)
008B6A: 0FA8 0B00      bclr    D7, ($b00,A0)
008B6E: 670C           beq     $8b7c
008B70: 4CE8 0C3F 0B00 movem.l ($b00,A0), D0-D5/A2-A3
008B76: 48E9 0C3F 0B00 movem.l D0-D5/A2-A3, ($b00,A1)
008B7C: 0FA8 0B20      bclr    D7, ($b20,A0)
008B80: 670C           beq     $8b8e
008B82: 4CE8 0C3F 0B20 movem.l ($b20,A0), D0-D5/A2-A3
008B88: 48E9 0C3F 0B20 movem.l D0-D5/A2-A3, ($b20,A1)
008B8E: 0FA8 0B40      bclr    D7, ($b40,A0)
008B92: 670C           beq     $8ba0
008B94: 4CE8 0C3F 0B40 movem.l ($b40,A0), D0-D5/A2-A3
008B9A: 48E9 0C3F 0B40 movem.l D0-D5/A2-A3, ($b40,A1)
008BA0: 0FA8 0B60      bclr    D7, ($b60,A0)
008BA4: 670C           beq     $8bb2
008BA6: 4CE8 0C3F 0B60 movem.l ($b60,A0), D0-D5/A2-A3
008BAC: 48E9 0C3F 0B60 movem.l D0-D5/A2-A3, ($b60,A1)
008BB2: 0FA8 0B80      bclr    D7, ($b80,A0)
008BB6: 670C           beq     $8bc4
008BB8: 4CE8 0C3F 0B80 movem.l ($b80,A0), D0-D5/A2-A3
008BBE: 48E9 0C3F 0B80 movem.l D0-D5/A2-A3, ($b80,A1)
008BC4: 0FA8 0BA0      bclr    D7, ($ba0,A0)
008BC8: 670C           beq     $8bd6
008BCA: 4CE8 0C3F 0BA0 movem.l ($ba0,A0), D0-D5/A2-A3
008BD0: 48E9 0C3F 0BA0 movem.l D0-D5/A2-A3, ($ba0,A1)
008BD6: 0FA8 0BC0      bclr    D7, ($bc0,A0)
008BDA: 670C           beq     $8be8
008BDC: 4CE8 0C3F 0BC0 movem.l ($bc0,A0), D0-D5/A2-A3
008BE2: 48E9 0C3F 0BC0 movem.l D0-D5/A2-A3, ($bc0,A1)
008BE8: 0FA8 0BE0      bclr    D7, ($be0,A0)
008BEC: 670C           beq     $8bfa
008BEE: 4CE8 0C3F 0BE0 movem.l ($be0,A0), D0-D5/A2-A3
008BF4: 48E9 0C3F 0BE0 movem.l D0-D5/A2-A3, ($be0,A1)
008BFA: 0FA8 0C00      bclr    D7, ($c00,A0)
008BFE: 670C           beq     $8c0c
008C00: 4CE8 0C3F 0C00 movem.l ($c00,A0), D0-D5/A2-A3
008C06: 48E9 0C3F 0C00 movem.l D0-D5/A2-A3, ($c00,A1)
008C0C: 0FA8 0C20      bclr    D7, ($c20,A0)
008C10: 670C           beq     $8c1e
008C12: 4CE8 0C3F 0C20 movem.l ($c20,A0), D0-D5/A2-A3
008C18: 48E9 0C3F 0C20 movem.l D0-D5/A2-A3, ($c20,A1)
008C1E: 0FA8 0C40      bclr    D7, ($c40,A0)
008C22: 670C           beq     $8c30
008C24: 4CE8 0C3F 0C40 movem.l ($c40,A0), D0-D5/A2-A3
008C2A: 48E9 0C3F 0C40 movem.l D0-D5/A2-A3, ($c40,A1)
008C30: 0FA8 0C60      bclr    D7, ($c60,A0)
008C34: 670C           beq     $8c42
008C36: 4CE8 0C3F 0C60 movem.l ($c60,A0), D0-D5/A2-A3
008C3C: 48E9 0C3F 0C60 movem.l D0-D5/A2-A3, ($c60,A1)
008C42: 0FA8 0C80      bclr    D7, ($c80,A0)
008C46: 670C           beq     $8c54
008C48: 4CE8 0C3F 0C80 movem.l ($c80,A0), D0-D5/A2-A3
008C4E: 48E9 0C3F 0C80 movem.l D0-D5/A2-A3, ($c80,A1)
008C54: 0FA8 0CA0      bclr    D7, ($ca0,A0)
008C58: 670C           beq     $8c66
008C5A: 4CE8 0C3F 0CA0 movem.l ($ca0,A0), D0-D5/A2-A3
008C60: 48E9 0C3F 0CA0 movem.l D0-D5/A2-A3, ($ca0,A1)
008C66: 0FA8 0CC0      bclr    D7, ($cc0,A0)
008C6A: 670C           beq     $8c78
008C6C: 4CE8 0C3F 0CC0 movem.l ($cc0,A0), D0-D5/A2-A3
008C72: 48E9 0C3F 0CC0 movem.l D0-D5/A2-A3, ($cc0,A1)
008C78: 0FA8 0CE0      bclr    D7, ($ce0,A0)
008C7C: 670C           beq     $8c8a
008C7E: 4CE8 0C3F 0CE0 movem.l ($ce0,A0), D0-D5/A2-A3
008C84: 48E9 0C3F 0CE0 movem.l D0-D5/A2-A3, ($ce0,A1)
008C8A: 0FA8 0D00      bclr    D7, ($d00,A0)
008C8E: 670C           beq     $8c9c
008C90: 4CE8 0C3F 0D00 movem.l ($d00,A0), D0-D5/A2-A3
008C96: 48E9 0C3F 0D00 movem.l D0-D5/A2-A3, ($d00,A1)
008C9C: 0FA8 0D20      bclr    D7, ($d20,A0)
008CA0: 670C           beq     $8cae
008CA2: 4CE8 0C3F 0D20 movem.l ($d20,A0), D0-D5/A2-A3
008CA8: 48E9 0C3F 0D20 movem.l D0-D5/A2-A3, ($d20,A1)
008CAE: 0FA8 0D40      bclr    D7, ($d40,A0)
008CB2: 670C           beq     $8cc0
008CB4: 4CE8 0C3F 0D40 movem.l ($d40,A0), D0-D5/A2-A3
008CBA: 48E9 0C3F 0D40 movem.l D0-D5/A2-A3, ($d40,A1)
008CC0: 0FA8 0D60      bclr    D7, ($d60,A0)
008CC4: 670C           beq     $8cd2
008CC6: 4CE8 0C3F 0D60 movem.l ($d60,A0), D0-D5/A2-A3
008CCC: 48E9 0C3F 0D60 movem.l D0-D5/A2-A3, ($d60,A1)
008CD2: 0FA8 0D80      bclr    D7, ($d80,A0)
008CD6: 670C           beq     $8ce4
008CD8: 4CE8 0C3F 0D80 movem.l ($d80,A0), D0-D5/A2-A3
008CDE: 48E9 0C3F 0D80 movem.l D0-D5/A2-A3, ($d80,A1)
008CE4: 0FA8 0DA0      bclr    D7, ($da0,A0)
008CE8: 670C           beq     $8cf6
008CEA: 4CE8 0C3F 0DA0 movem.l ($da0,A0), D0-D5/A2-A3
008CF0: 48E9 0C3F 0DA0 movem.l D0-D5/A2-A3, ($da0,A1)
008CF6: 0FA8 0DC0      bclr    D7, ($dc0,A0)
008CFA: 670C           beq     $8d08
008CFC: 4CE8 0C3F 0DC0 movem.l ($dc0,A0), D0-D5/A2-A3
008D02: 48E9 0C3F 0DC0 movem.l D0-D5/A2-A3, ($dc0,A1)
008D08: 0FA8 0DE0      bclr    D7, ($de0,A0)
008D0C: 670C           beq     $8d1a
008D0E: 4CE8 0C3F 0DE0 movem.l ($de0,A0), D0-D5/A2-A3
008D14: 48E9 0C3F 0DE0 movem.l D0-D5/A2-A3, ($de0,A1)
008D1A: 0FA8 0E00      bclr    D7, ($e00,A0)
008D1E: 670C           beq     $8d2c
008D20: 4CE8 0C3F 0E00 movem.l ($e00,A0), D0-D5/A2-A3
008D26: 48E9 0C3F 0E00 movem.l D0-D5/A2-A3, ($e00,A1)
008D2C: 0FA8 0E20      bclr    D7, ($e20,A0)
008D30: 670C           beq     $8d3e
008D32: 4CE8 0C3F 0E20 movem.l ($e20,A0), D0-D5/A2-A3
008D38: 48E9 0C3F 0E20 movem.l D0-D5/A2-A3, ($e20,A1)
008D3E: 0FA8 0E40      bclr    D7, ($e40,A0)
008D42: 670C           beq     $8d50
008D44: 4CE8 0C3F 0E40 movem.l ($e40,A0), D0-D5/A2-A3
008D4A: 48E9 0C3F 0E40 movem.l D0-D5/A2-A3, ($e40,A1)
008D50: 0FA8 0E60      bclr    D7, ($e60,A0)
008D54: 670C           beq     $8d62
008D56: 4CE8 0C3F 0E60 movem.l ($e60,A0), D0-D5/A2-A3
008D5C: 48E9 0C3F 0E60 movem.l D0-D5/A2-A3, ($e60,A1)
008D62: 0FA8 0E80      bclr    D7, ($e80,A0)
008D66: 670C           beq     $8d74
008D68: 4CE8 0C3F 0E80 movem.l ($e80,A0), D0-D5/A2-A3
008D6E: 48E9 0C3F 0E80 movem.l D0-D5/A2-A3, ($e80,A1)
008D74: 0FA8 0EA0      bclr    D7, ($ea0,A0)
008D78: 670C           beq     $8d86
008D7A: 4CE8 0C3F 0EA0 movem.l ($ea0,A0), D0-D5/A2-A3
008D80: 48E9 0C3F 0EA0 movem.l D0-D5/A2-A3, ($ea0,A1)
008D86: 0FA8 0EC0      bclr    D7, ($ec0,A0)
008D8A: 670C           beq     $8d98
008D8C: 4CE8 0C3F 0EC0 movem.l ($ec0,A0), D0-D5/A2-A3
008D92: 48E9 0C3F 0EC0 movem.l D0-D5/A2-A3, ($ec0,A1)
008D98: 0FA8 0EE0      bclr    D7, ($ee0,A0)
008D9C: 670C           beq     $8daa
008D9E: 4CE8 0C3F 0EE0 movem.l ($ee0,A0), D0-D5/A2-A3
008DA4: 48E9 0C3F 0EE0 movem.l D0-D5/A2-A3, ($ee0,A1)
008DAA: 0FA8 0F00      bclr    D7, ($f00,A0)
008DAE: 670C           beq     $8dbc
008DB0: 4CE8 0C3F 0F00 movem.l ($f00,A0), D0-D5/A2-A3
008DB6: 48E9 0C3F 0F00 movem.l D0-D5/A2-A3, ($f00,A1)
008DBC: 0FA8 0F20      bclr    D7, ($f20,A0)
008DC0: 670C           beq     $8dce
008DC2: 4CE8 0C3F 0F20 movem.l ($f20,A0), D0-D5/A2-A3
008DC8: 48E9 0C3F 0F20 movem.l D0-D5/A2-A3, ($f20,A1)
008DCE: 0FA8 0F40      bclr    D7, ($f40,A0)
008DD2: 670C           beq     $8de0
008DD4: 4CE8 0C3F 0F40 movem.l ($f40,A0), D0-D5/A2-A3
008DDA: 48E9 0C3F 0F40 movem.l D0-D5/A2-A3, ($f40,A1)
008DE0: 0FA8 0F60      bclr    D7, ($f60,A0)
008DE4: 670C           beq     $8df2
008DE6: 4CE8 0C3F 0F60 movem.l ($f60,A0), D0-D5/A2-A3
008DEC: 48E9 0C3F 0F60 movem.l D0-D5/A2-A3, ($f60,A1)
008DF2: 0FA8 0F80      bclr    D7, ($f80,A0)
008DF6: 670C           beq     $8e04
008DF8: 4CE8 0C3F 0F80 movem.l ($f80,A0), D0-D5/A2-A3
008DFE: 48E9 0C3F 0F80 movem.l D0-D5/A2-A3, ($f80,A1)
008E04: 0FA8 0FA0      bclr    D7, ($fa0,A0)
008E08: 670C           beq     $8e16
008E0A: 4CE8 0C3F 0FA0 movem.l ($fa0,A0), D0-D5/A2-A3
008E10: 48E9 0C3F 0FA0 movem.l D0-D5/A2-A3, ($fa0,A1)
008E16: 0FA8 0FC0      bclr    D7, ($fc0,A0)
008E1A: 670C           beq     $8e28
008E1C: 4CE8 0C3F 0FC0 movem.l ($fc0,A0), D0-D5/A2-A3
008E22: 48E9 0C3F 0FC0 movem.l D0-D5/A2-A3, ($fc0,A1)
008E28: 0FA8 0FE0      bclr    D7, ($fe0,A0)
008E2C: 670C           beq     $8e3a
008E2E: 4CE8 0C3F 0FE0 movem.l ($fe0,A0), D0-D5/A2-A3
008E34: 48E9 0C3F 0FE0 movem.l D0-D5/A2-A3, ($fe0,A1)
008E3A: 0FA8 1000      bclr    D7, ($1000,A0)
008E3E: 670C           beq     $8e4c
008E40: 4CE8 0C3F 1000 movem.l ($1000,A0), D0-D5/A2-A3
008E46: 48E9 0C3F 1000 movem.l D0-D5/A2-A3, ($1000,A1)
008E4C: 0FA8 1020      bclr    D7, ($1020,A0)
008E50: 670C           beq     $8e5e
008E52: 4CE8 0C3F 1020 movem.l ($1020,A0), D0-D5/A2-A3
008E58: 48E9 0C3F 1020 movem.l D0-D5/A2-A3, ($1020,A1)
008E5E: 0FA8 1040      bclr    D7, ($1040,A0)
008E62: 670C           beq     $8e70
008E64: 4CE8 0C3F 1040 movem.l ($1040,A0), D0-D5/A2-A3
008E6A: 48E9 0C3F 1040 movem.l D0-D5/A2-A3, ($1040,A1)
008E70: 0FA8 1060      bclr    D7, ($1060,A0)
008E74: 670C           beq     $8e82
008E76: 4CE8 0C3F 1060 movem.l ($1060,A0), D0-D5/A2-A3
008E7C: 48E9 0C3F 1060 movem.l D0-D5/A2-A3, ($1060,A1)
008E82: 0FA8 1080      bclr    D7, ($1080,A0)
008E86: 670C           beq     $8e94
008E88: 4CE8 0C3F 1080 movem.l ($1080,A0), D0-D5/A2-A3
008E8E: 48E9 0C3F 1080 movem.l D0-D5/A2-A3, ($1080,A1)
008E94: 0FA8 10A0      bclr    D7, ($10a0,A0)
008E98: 670C           beq     $8ea6
008E9A: 4CE8 0C3F 10A0 movem.l ($10a0,A0), D0-D5/A2-A3
008EA0: 48E9 0C3F 10A0 movem.l D0-D5/A2-A3, ($10a0,A1)
008EA6: 0FA8 10C0      bclr    D7, ($10c0,A0)
008EAA: 670C           beq     $8eb8
008EAC: 4CE8 0C3F 10C0 movem.l ($10c0,A0), D0-D5/A2-A3
008EB2: 48E9 0C3F 10C0 movem.l D0-D5/A2-A3, ($10c0,A1)
008EB8: 0FA8 10E0      bclr    D7, ($10e0,A0)
008EBC: 670C           beq     $8eca
008EBE: 4CE8 0C3F 10E0 movem.l ($10e0,A0), D0-D5/A2-A3
008EC4: 48E9 0C3F 10E0 movem.l D0-D5/A2-A3, ($10e0,A1)
008ECA: 0FA8 1100      bclr    D7, ($1100,A0)
008ECE: 670C           beq     $8edc
008ED0: 4CE8 0C3F 1100 movem.l ($1100,A0), D0-D5/A2-A3
008ED6: 48E9 0C3F 1100 movem.l D0-D5/A2-A3, ($1100,A1)
008EDC: 0FA8 1120      bclr    D7, ($1120,A0)
008EE0: 670C           beq     $8eee
008EE2: 4CE8 0C3F 1120 movem.l ($1120,A0), D0-D5/A2-A3
008EE8: 48E9 0C3F 1120 movem.l D0-D5/A2-A3, ($1120,A1)
008EEE: 0FA8 1140      bclr    D7, ($1140,A0)
008EF2: 670C           beq     $8f00
008EF4: 4CE8 0C3F 1140 movem.l ($1140,A0), D0-D5/A2-A3
008EFA: 48E9 0C3F 1140 movem.l D0-D5/A2-A3, ($1140,A1)
008F00: 0FA8 1160      bclr    D7, ($1160,A0)
008F04: 670C           beq     $8f12
008F06: 4CE8 0C3F 1160 movem.l ($1160,A0), D0-D5/A2-A3
008F0C: 48E9 0C3F 1160 movem.l D0-D5/A2-A3, ($1160,A1)
008F12: 0FA8 1180      bclr    D7, ($1180,A0)
008F16: 670C           beq     $8f24
008F18: 4CE8 0C3F 1180 movem.l ($1180,A0), D0-D5/A2-A3
008F1E: 48E9 0C3F 1180 movem.l D0-D5/A2-A3, ($1180,A1)
008F24: 0FA8 11A0      bclr    D7, ($11a0,A0)
008F28: 670C           beq     $8f36
008F2A: 4CE8 0C3F 11A0 movem.l ($11a0,A0), D0-D5/A2-A3
008F30: 48E9 0C3F 11A0 movem.l D0-D5/A2-A3, ($11a0,A1)
008F36: 0FA8 11C0      bclr    D7, ($11c0,A0)
008F3A: 670C           beq     $8f48
008F3C: 4CE8 0C3F 11C0 movem.l ($11c0,A0), D0-D5/A2-A3
008F42: 48E9 0C3F 11C0 movem.l D0-D5/A2-A3, ($11c0,A1)
008F48: 0FA8 11E0      bclr    D7, ($11e0,A0)
008F4C: 670C           beq     $8f5a
008F4E: 4CE8 0C3F 11E0 movem.l ($11e0,A0), D0-D5/A2-A3
008F54: 48E9 0C3F 11E0 movem.l D0-D5/A2-A3, ($11e0,A1)
008F5A: 0FA8 1200      bclr    D7, ($1200,A0)
008F5E: 670C           beq     $8f6c
008F60: 4CE8 0C3F 1200 movem.l ($1200,A0), D0-D5/A2-A3
008F66: 48E9 0C3F 1200 movem.l D0-D5/A2-A3, ($1200,A1)
008F6C: 0FA8 1220      bclr    D7, ($1220,A0)
008F70: 670C           beq     $8f7e
008F72: 4CE8 0C3F 1220 movem.l ($1220,A0), D0-D5/A2-A3
008F78: 48E9 0C3F 1220 movem.l D0-D5/A2-A3, ($1220,A1)
008F7E: 0FA8 1240      bclr    D7, ($1240,A0)
008F82: 670C           beq     $8f90
008F84: 4CE8 0C3F 1240 movem.l ($1240,A0), D0-D5/A2-A3
008F8A: 48E9 0C3F 1240 movem.l D0-D5/A2-A3, ($1240,A1)
008F90: 0FA8 1260      bclr    D7, ($1260,A0)
008F94: 670C           beq     $8fa2
008F96: 4CE8 0C3F 1260 movem.l ($1260,A0), D0-D5/A2-A3
008F9C: 48E9 0C3F 1260 movem.l D0-D5/A2-A3, ($1260,A1)
008FA2: 0FA8 1280      bclr    D7, ($1280,A0)
008FA6: 670C           beq     $8fb4
008FA8: 4CE8 0C3F 1280 movem.l ($1280,A0), D0-D5/A2-A3
008FAE: 48E9 0C3F 1280 movem.l D0-D5/A2-A3, ($1280,A1)
008FB4: 0FA8 12A0      bclr    D7, ($12a0,A0)
008FB8: 670C           beq     $8fc6
008FBA: 4CE8 0C3F 12A0 movem.l ($12a0,A0), D0-D5/A2-A3
008FC0: 48E9 0C3F 12A0 movem.l D0-D5/A2-A3, ($12a0,A1)
008FC6: 0FA8 12C0      bclr    D7, ($12c0,A0)
008FCA: 670C           beq     $8fd8
008FCC: 4CE8 0C3F 12C0 movem.l ($12c0,A0), D0-D5/A2-A3
008FD2: 48E9 0C3F 12C0 movem.l D0-D5/A2-A3, ($12c0,A1)
008FD8: 0FA8 12E0      bclr    D7, ($12e0,A0)
008FDC: 670C           beq     $8fea
008FDE: 4CE8 0C3F 12E0 movem.l ($12e0,A0), D0-D5/A2-A3
008FE4: 48E9 0C3F 12E0 movem.l D0-D5/A2-A3, ($12e0,A1)
008FEA: 0FA8 1300      bclr    D7, ($1300,A0)
008FEE: 670C           beq     $8ffc
008FF0: 4CE8 0C3F 1300 movem.l ($1300,A0), D0-D5/A2-A3
008FF6: 48E9 0C3F 1300 movem.l D0-D5/A2-A3, ($1300,A1)
008FFC: 0FA8 1320      bclr    D7, ($1320,A0)
009000: 670C           beq     $900e
009002: 4CE8 0C3F 1320 movem.l ($1320,A0), D0-D5/A2-A3
009008: 48E9 0C3F 1320 movem.l D0-D5/A2-A3, ($1320,A1)
00900E: 0FA8 1340      bclr    D7, ($1340,A0)
009012: 670C           beq     $9020
009014: 4CE8 0C3F 1340 movem.l ($1340,A0), D0-D5/A2-A3
00901A: 48E9 0C3F 1340 movem.l D0-D5/A2-A3, ($1340,A1)
009020: 0FA8 1360      bclr    D7, ($1360,A0)
009024: 670C           beq     $9032
009026: 4CE8 0C3F 1360 movem.l ($1360,A0), D0-D5/A2-A3
00902C: 48E9 0C3F 1360 movem.l D0-D5/A2-A3, ($1360,A1)
009032: 0FA8 1380      bclr    D7, ($1380,A0)
009036: 670C           beq     $9044
009038: 4CE8 0C3F 1380 movem.l ($1380,A0), D0-D5/A2-A3
00903E: 48E9 0C3F 1380 movem.l D0-D5/A2-A3, ($1380,A1)
009044: 0FA8 13A0      bclr    D7, ($13a0,A0)
009048: 670C           beq     $9056
00904A: 4CE8 0C3F 13A0 movem.l ($13a0,A0), D0-D5/A2-A3
009050: 48E9 0C3F 13A0 movem.l D0-D5/A2-A3, ($13a0,A1)
009056: 0FA8 13C0      bclr    D7, ($13c0,A0)
00905A: 670C           beq     $9068
00905C: 4CE8 0C3F 13C0 movem.l ($13c0,A0), D0-D5/A2-A3
009062: 48E9 0C3F 13C0 movem.l D0-D5/A2-A3, ($13c0,A1)
009068: 0FA8 13E0      bclr    D7, ($13e0,A0)
00906C: 670C           beq     $907a
00906E: 4CE8 0C3F 13E0 movem.l ($13e0,A0), D0-D5/A2-A3
009074: 48E9 0C3F 13E0 movem.l D0-D5/A2-A3, ($13e0,A1)
00907A: 0FA8 1400      bclr    D7, ($1400,A0)
00907E: 670C           beq     $908c
009080: 4CE8 0C3F 1400 movem.l ($1400,A0), D0-D5/A2-A3
009086: 48E9 0C3F 1400 movem.l D0-D5/A2-A3, ($1400,A1)
00908C: 0FA8 1420      bclr    D7, ($1420,A0)
009090: 670C           beq     $909e
009092: 4CE8 0C3F 1420 movem.l ($1420,A0), D0-D5/A2-A3
009098: 48E9 0C3F 1420 movem.l D0-D5/A2-A3, ($1420,A1)
00909E: 0FA8 1440      bclr    D7, ($1440,A0)
0090A2: 670C           beq     $90b0
0090A4: 4CE8 0C3F 1440 movem.l ($1440,A0), D0-D5/A2-A3
0090AA: 48E9 0C3F 1440 movem.l D0-D5/A2-A3, ($1440,A1)
0090B0: 0FA8 1460      bclr    D7, ($1460,A0)
0090B4: 670C           beq     $90c2
0090B6: 4CE8 0C3F 1460 movem.l ($1460,A0), D0-D5/A2-A3
0090BC: 48E9 0C3F 1460 movem.l D0-D5/A2-A3, ($1460,A1)
0090C2: 0FA8 1480      bclr    D7, ($1480,A0)
0090C6: 670C           beq     $90d4
0090C8: 4CE8 0C3F 1480 movem.l ($1480,A0), D0-D5/A2-A3
0090CE: 48E9 0C3F 1480 movem.l D0-D5/A2-A3, ($1480,A1)
0090D4: 0FA8 14A0      bclr    D7, ($14a0,A0)
0090D8: 670C           beq     $90e6
0090DA: 4CE8 0C3F 14A0 movem.l ($14a0,A0), D0-D5/A2-A3
0090E0: 48E9 0C3F 14A0 movem.l D0-D5/A2-A3, ($14a0,A1)
0090E6: 0FA8 14C0      bclr    D7, ($14c0,A0)
0090EA: 670C           beq     $90f8
0090EC: 4CE8 0C3F 14C0 movem.l ($14c0,A0), D0-D5/A2-A3
0090F2: 48E9 0C3F 14C0 movem.l D0-D5/A2-A3, ($14c0,A1)
0090F8: 0FA8 14E0      bclr    D7, ($14e0,A0)
0090FC: 670C           beq     $910a
0090FE: 4CE8 0C3F 14E0 movem.l ($14e0,A0), D0-D5/A2-A3
009104: 48E9 0C3F 14E0 movem.l D0-D5/A2-A3, ($14e0,A1)
00910A: 0FA8 1500      bclr    D7, ($1500,A0)
00910E: 670C           beq     $911c
009110: 4CE8 0C3F 1500 movem.l ($1500,A0), D0-D5/A2-A3
009116: 48E9 0C3F 1500 movem.l D0-D5/A2-A3, ($1500,A1)
00911C: 0FA8 1520      bclr    D7, ($1520,A0)
009120: 670C           beq     $912e
009122: 4CE8 0C3F 1520 movem.l ($1520,A0), D0-D5/A2-A3
009128: 48E9 0C3F 1520 movem.l D0-D5/A2-A3, ($1520,A1)
00912E: 0FA8 1540      bclr    D7, ($1540,A0)
009132: 670C           beq     $9140
009134: 4CE8 0C3F 1540 movem.l ($1540,A0), D0-D5/A2-A3
00913A: 48E9 0C3F 1540 movem.l D0-D5/A2-A3, ($1540,A1)
009140: 0FA8 1560      bclr    D7, ($1560,A0)
009144: 670C           beq     $9152
009146: 4CE8 0C3F 1560 movem.l ($1560,A0), D0-D5/A2-A3
00914C: 48E9 0C3F 1560 movem.l D0-D5/A2-A3, ($1560,A1)
009152: 0FA8 1580      bclr    D7, ($1580,A0)
009156: 670C           beq     $9164
009158: 4CE8 0C3F 1580 movem.l ($1580,A0), D0-D5/A2-A3
00915E: 48E9 0C3F 1580 movem.l D0-D5/A2-A3, ($1580,A1)
009164: 0FA8 15A0      bclr    D7, ($15a0,A0)
009168: 670C           beq     $9176
00916A: 4CE8 0C3F 15A0 movem.l ($15a0,A0), D0-D5/A2-A3
009170: 48E9 0C3F 15A0 movem.l D0-D5/A2-A3, ($15a0,A1)
009176: 0FA8 15C0      bclr    D7, ($15c0,A0)
00917A: 670C           beq     $9188
00917C: 4CE8 0C3F 15C0 movem.l ($15c0,A0), D0-D5/A2-A3
009182: 48E9 0C3F 15C0 movem.l D0-D5/A2-A3, ($15c0,A1)
009188: 0FA8 15E0      bclr    D7, ($15e0,A0)
00918C: 670C           beq     $919a
00918E: 4CE8 0C3F 15E0 movem.l ($15e0,A0), D0-D5/A2-A3
009194: 48E9 0C3F 15E0 movem.l D0-D5/A2-A3, ($15e0,A1)
00919A: 0FA8 1600      bclr    D7, ($1600,A0)
00919E: 670C           beq     $91ac
0091A0: 4CE8 0C3F 1600 movem.l ($1600,A0), D0-D5/A2-A3
0091A6: 48E9 0C3F 1600 movem.l D0-D5/A2-A3, ($1600,A1)
0091AC: 0FA8 1620      bclr    D7, ($1620,A0)
0091B0: 670C           beq     $91be
0091B2: 4CE8 0C3F 1620 movem.l ($1620,A0), D0-D5/A2-A3
0091B8: 48E9 0C3F 1620 movem.l D0-D5/A2-A3, ($1620,A1)
0091BE: 0FA8 1640      bclr    D7, ($1640,A0)
0091C2: 670C           beq     $91d0
0091C4: 4CE8 0C3F 1640 movem.l ($1640,A0), D0-D5/A2-A3
0091CA: 48E9 0C3F 1640 movem.l D0-D5/A2-A3, ($1640,A1)
0091D0: 0FA8 1660      bclr    D7, ($1660,A0)
0091D4: 670C           beq     $91e2
0091D6: 4CE8 0C3F 1660 movem.l ($1660,A0), D0-D5/A2-A3
0091DC: 48E9 0C3F 1660 movem.l D0-D5/A2-A3, ($1660,A1)
0091E2: 0FA8 1680      bclr    D7, ($1680,A0)
0091E6: 670C           beq     $91f4
0091E8: 4CE8 0C3F 1680 movem.l ($1680,A0), D0-D5/A2-A3
0091EE: 48E9 0C3F 1680 movem.l D0-D5/A2-A3, ($1680,A1)
0091F4: 0FA8 16A0      bclr    D7, ($16a0,A0)
0091F8: 670C           beq     $9206
0091FA: 4CE8 0C3F 16A0 movem.l ($16a0,A0), D0-D5/A2-A3
009200: 48E9 0C3F 16A0 movem.l D0-D5/A2-A3, ($16a0,A1)
009206: 0FA8 16C0      bclr    D7, ($16c0,A0)
00920A: 670C           beq     $9218
00920C: 4CE8 0C3F 16C0 movem.l ($16c0,A0), D0-D5/A2-A3
009212: 48E9 0C3F 16C0 movem.l D0-D5/A2-A3, ($16c0,A1)
009218: 0FA8 16E0      bclr    D7, ($16e0,A0)
00921C: 670C           beq     $922a
00921E: 4CE8 0C3F 16E0 movem.l ($16e0,A0), D0-D5/A2-A3
009224: 48E9 0C3F 16E0 movem.l D0-D5/A2-A3, ($16e0,A1)
00922A: 0FA8 1700      bclr    D7, ($1700,A0)
00922E: 670C           beq     $923c
009230: 4CE8 0C3F 1700 movem.l ($1700,A0), D0-D5/A2-A3
009236: 48E9 0C3F 1700 movem.l D0-D5/A2-A3, ($1700,A1)
00923C: 0FA8 1720      bclr    D7, ($1720,A0)
009240: 670C           beq     $924e
009242: 4CE8 0C3F 1720 movem.l ($1720,A0), D0-D5/A2-A3
009248: 48E9 0C3F 1720 movem.l D0-D5/A2-A3, ($1720,A1)
00924E: 0FA8 1740      bclr    D7, ($1740,A0)
009252: 670C           beq     $9260
009254: 4CE8 0C3F 1740 movem.l ($1740,A0), D0-D5/A2-A3
00925A: 48E9 0C3F 1740 movem.l D0-D5/A2-A3, ($1740,A1)
009260: 0FA8 1760      bclr    D7, ($1760,A0)
009264: 670C           beq     $9272
009266: 4CE8 0C3F 1760 movem.l ($1760,A0), D0-D5/A2-A3
00926C: 48E9 0C3F 1760 movem.l D0-D5/A2-A3, ($1760,A1)
009272: 0FA8 1780      bclr    D7, ($1780,A0)
009276: 670C           beq     $9284
009278: 4CE8 0C3F 1780 movem.l ($1780,A0), D0-D5/A2-A3
00927E: 48E9 0C3F 1780 movem.l D0-D5/A2-A3, ($1780,A1)
009284: 0FA8 17A0      bclr    D7, ($17a0,A0)
009288: 670C           beq     $9296
00928A: 4CE8 0C3F 17A0 movem.l ($17a0,A0), D0-D5/A2-A3
009290: 48E9 0C3F 17A0 movem.l D0-D5/A2-A3, ($17a0,A1)
009296: 0FA8 17C0      bclr    D7, ($17c0,A0)
00929A: 670C           beq     $92a8
00929C: 4CE8 0C3F 17C0 movem.l ($17c0,A0), D0-D5/A2-A3
0092A2: 48E9 0C3F 17C0 movem.l D0-D5/A2-A3, ($17c0,A1)
0092A8: 0FA8 17E0      bclr    D7, ($17e0,A0)
0092AC: 670C           beq     $92ba
0092AE: 4CE8 0C3F 17E0 movem.l ($17e0,A0), D0-D5/A2-A3
0092B4: 48E9 0C3F 17E0 movem.l D0-D5/A2-A3, ($17e0,A1)
0092BA: 0FA8 1800      bclr    D7, ($1800,A0)
0092BE: 670C           beq     $92cc
0092C0: 4CE8 0C3F 1800 movem.l ($1800,A0), D0-D5/A2-A3
0092C6: 48E9 0C3F 1800 movem.l D0-D5/A2-A3, ($1800,A1)
0092CC: 0FA8 1820      bclr    D7, ($1820,A0)
0092D0: 670C           beq     $92de
0092D2: 4CE8 0C3F 1820 movem.l ($1820,A0), D0-D5/A2-A3
0092D8: 48E9 0C3F 1820 movem.l D0-D5/A2-A3, ($1820,A1)
0092DE: 0FA8 1840      bclr    D7, ($1840,A0)
0092E2: 670C           beq     $92f0
0092E4: 4CE8 0C3F 1840 movem.l ($1840,A0), D0-D5/A2-A3
0092EA: 48E9 0C3F 1840 movem.l D0-D5/A2-A3, ($1840,A1)
0092F0: 0FA8 1860      bclr    D7, ($1860,A0)
0092F4: 670C           beq     $9302
0092F6: 4CE8 0C3F 1860 movem.l ($1860,A0), D0-D5/A2-A3
0092FC: 48E9 0C3F 1860 movem.l D0-D5/A2-A3, ($1860,A1)
009302: 0FA8 1880      bclr    D7, ($1880,A0)
009306: 670C           beq     $9314
009308: 4CE8 0C3F 1880 movem.l ($1880,A0), D0-D5/A2-A3
00930E: 48E9 0C3F 1880 movem.l D0-D5/A2-A3, ($1880,A1)
009314: 0FA8 18A0      bclr    D7, ($18a0,A0)
009318: 670C           beq     $9326
00931A: 4CE8 0C3F 18A0 movem.l ($18a0,A0), D0-D5/A2-A3
009320: 48E9 0C3F 18A0 movem.l D0-D5/A2-A3, ($18a0,A1)
009326: 0FA8 18C0      bclr    D7, ($18c0,A0)
00932A: 670C           beq     $9338
00932C: 4CE8 0C3F 18C0 movem.l ($18c0,A0), D0-D5/A2-A3
009332: 48E9 0C3F 18C0 movem.l D0-D5/A2-A3, ($18c0,A1)
009338: 0FA8 18E0      bclr    D7, ($18e0,A0)
00933C: 670C           beq     $934a
00933E: 4CE8 0C3F 18E0 movem.l ($18e0,A0), D0-D5/A2-A3
009344: 48E9 0C3F 18E0 movem.l D0-D5/A2-A3, ($18e0,A1)
00934A: 0FA8 1900      bclr    D7, ($1900,A0)
00934E: 670C           beq     $935c
009350: 4CE8 0C3F 1900 movem.l ($1900,A0), D0-D5/A2-A3
009356: 48E9 0C3F 1900 movem.l D0-D5/A2-A3, ($1900,A1)
00935C: 0FA8 1920      bclr    D7, ($1920,A0)
009360: 670C           beq     $936e
009362: 4CE8 0C3F 1920 movem.l ($1920,A0), D0-D5/A2-A3
009368: 48E9 0C3F 1920 movem.l D0-D5/A2-A3, ($1920,A1)
00936E: 0FA8 1940      bclr    D7, ($1940,A0)
009372: 670C           beq     $9380
009374: 4CE8 0C3F 1940 movem.l ($1940,A0), D0-D5/A2-A3
00937A: 48E9 0C3F 1940 movem.l D0-D5/A2-A3, ($1940,A1)
009380: 0FA8 1960      bclr    D7, ($1960,A0)
009384: 670C           beq     $9392
009386: 4CE8 0C3F 1960 movem.l ($1960,A0), D0-D5/A2-A3
00938C: 48E9 0C3F 1960 movem.l D0-D5/A2-A3, ($1960,A1)
009392: 0FA8 1980      bclr    D7, ($1980,A0)
009396: 670C           beq     $93a4
009398: 4CE8 0C3F 1980 movem.l ($1980,A0), D0-D5/A2-A3
00939E: 48E9 0C3F 1980 movem.l D0-D5/A2-A3, ($1980,A1)
0093A4: 0FA8 19A0      bclr    D7, ($19a0,A0)
0093A8: 670C           beq     $93b6
0093AA: 4CE8 0C3F 19A0 movem.l ($19a0,A0), D0-D5/A2-A3
0093B0: 48E9 0C3F 19A0 movem.l D0-D5/A2-A3, ($19a0,A1)
0093B6: 0FA8 19C0      bclr    D7, ($19c0,A0)
0093BA: 670C           beq     $93c8
0093BC: 4CE8 0C3F 19C0 movem.l ($19c0,A0), D0-D5/A2-A3
0093C2: 48E9 0C3F 19C0 movem.l D0-D5/A2-A3, ($19c0,A1)
0093C8: 0FA8 19E0      bclr    D7, ($19e0,A0)
0093CC: 670C           beq     $93da
0093CE: 4CE8 0C3F 19E0 movem.l ($19e0,A0), D0-D5/A2-A3
0093D4: 48E9 0C3F 19E0 movem.l D0-D5/A2-A3, ($19e0,A1)
0093DA: 0FA8 1A00      bclr    D7, ($1a00,A0)
0093DE: 670C           beq     $93ec
0093E0: 4CE8 0C3F 1A00 movem.l ($1a00,A0), D0-D5/A2-A3
0093E6: 48E9 0C3F 1A00 movem.l D0-D5/A2-A3, ($1a00,A1)
0093EC: 0FA8 1A20      bclr    D7, ($1a20,A0)
0093F0: 670C           beq     $93fe
0093F2: 4CE8 0C3F 1A20 movem.l ($1a20,A0), D0-D5/A2-A3
0093F8: 48E9 0C3F 1A20 movem.l D0-D5/A2-A3, ($1a20,A1)
0093FE: 0FA8 1A40      bclr    D7, ($1a40,A0)
009402: 670C           beq     $9410
009404: 4CE8 0C3F 1A40 movem.l ($1a40,A0), D0-D5/A2-A3
00940A: 48E9 0C3F 1A40 movem.l D0-D5/A2-A3, ($1a40,A1)
009410: 0FA8 1A60      bclr    D7, ($1a60,A0)
009414: 670C           beq     $9422
009416: 4CE8 0C3F 1A60 movem.l ($1a60,A0), D0-D5/A2-A3
00941C: 48E9 0C3F 1A60 movem.l D0-D5/A2-A3, ($1a60,A1)
009422: 0FA8 1A80      bclr    D7, ($1a80,A0)
009426: 670C           beq     $9434
009428: 4CE8 0C3F 1A80 movem.l ($1a80,A0), D0-D5/A2-A3
00942E: 48E9 0C3F 1A80 movem.l D0-D5/A2-A3, ($1a80,A1)
009434: 0FA8 1AA0      bclr    D7, ($1aa0,A0)
009438: 670C           beq     $9446
00943A: 4CE8 0C3F 1AA0 movem.l ($1aa0,A0), D0-D5/A2-A3
009440: 48E9 0C3F 1AA0 movem.l D0-D5/A2-A3, ($1aa0,A1)
009446: 0FA8 1AC0      bclr    D7, ($1ac0,A0)
00944A: 670C           beq     $9458
00944C: 4CE8 0C3F 1AC0 movem.l ($1ac0,A0), D0-D5/A2-A3
009452: 48E9 0C3F 1AC0 movem.l D0-D5/A2-A3, ($1ac0,A1)
009458: 0FA8 1AE0      bclr    D7, ($1ae0,A0)
00945C: 670C           beq     $946a
00945E: 4CE8 0C3F 1AE0 movem.l ($1ae0,A0), D0-D5/A2-A3
009464: 48E9 0C3F 1AE0 movem.l D0-D5/A2-A3, ($1ae0,A1)
00946A: 0FA8 1B00      bclr    D7, ($1b00,A0)
00946E: 670C           beq     $947c
009470: 4CE8 0C3F 1B00 movem.l ($1b00,A0), D0-D5/A2-A3
009476: 48E9 0C3F 1B00 movem.l D0-D5/A2-A3, ($1b00,A1)
00947C: 0FA8 1B20      bclr    D7, ($1b20,A0)
009480: 670C           beq     $948e
009482: 4CE8 0C3F 1B20 movem.l ($1b20,A0), D0-D5/A2-A3
009488: 48E9 0C3F 1B20 movem.l D0-D5/A2-A3, ($1b20,A1)
00948E: 0FA8 1B40      bclr    D7, ($1b40,A0)
009492: 670C           beq     $94a0
009494: 4CE8 0C3F 1B40 movem.l ($1b40,A0), D0-D5/A2-A3
00949A: 48E9 0C3F 1B40 movem.l D0-D5/A2-A3, ($1b40,A1)
0094A0: 0FA8 1B60      bclr    D7, ($1b60,A0)
0094A4: 670C           beq     $94b2
0094A6: 4CE8 0C3F 1B60 movem.l ($1b60,A0), D0-D5/A2-A3
0094AC: 48E9 0C3F 1B60 movem.l D0-D5/A2-A3, ($1b60,A1)
0094B2: 0FA8 1B80      bclr    D7, ($1b80,A0)
0094B6: 670C           beq     $94c4
0094B8: 4CE8 0C3F 1B80 movem.l ($1b80,A0), D0-D5/A2-A3
0094BE: 48E9 0C3F 1B80 movem.l D0-D5/A2-A3, ($1b80,A1)
0094C4: 0FA8 1BA0      bclr    D7, ($1ba0,A0)
0094C8: 670C           beq     $94d6
0094CA: 4CE8 0C3F 1BA0 movem.l ($1ba0,A0), D0-D5/A2-A3
0094D0: 48E9 0C3F 1BA0 movem.l D0-D5/A2-A3, ($1ba0,A1)
0094D6: 0FA8 1BC0      bclr    D7, ($1bc0,A0)
0094DA: 670C           beq     $94e8
0094DC: 4CE8 0C3F 1BC0 movem.l ($1bc0,A0), D0-D5/A2-A3
0094E2: 48E9 0C3F 1BC0 movem.l D0-D5/A2-A3, ($1bc0,A1)
0094E8: 0FA8 1BE0      bclr    D7, ($1be0,A0)
0094EC: 670C           beq     $94fa
0094EE: 4CE8 0C3F 1BE0 movem.l ($1be0,A0), D0-D5/A2-A3
0094F4: 48E9 0C3F 1BE0 movem.l D0-D5/A2-A3, ($1be0,A1)
0094FA: 0FA8 1C00      bclr    D7, ($1c00,A0)
0094FE: 670C           beq     $950c
009500: 4CE8 0C3F 1C00 movem.l ($1c00,A0), D0-D5/A2-A3
009506: 48E9 0C3F 1C00 movem.l D0-D5/A2-A3, ($1c00,A1)
00950C: 0FA8 1C20      bclr    D7, ($1c20,A0)
009510: 670C           beq     $951e
009512: 4CE8 0C3F 1C20 movem.l ($1c20,A0), D0-D5/A2-A3
009518: 48E9 0C3F 1C20 movem.l D0-D5/A2-A3, ($1c20,A1)
00951E: 0FA8 1C40      bclr    D7, ($1c40,A0)
009522: 670C           beq     $9530
009524: 4CE8 0C3F 1C40 movem.l ($1c40,A0), D0-D5/A2-A3
00952A: 48E9 0C3F 1C40 movem.l D0-D5/A2-A3, ($1c40,A1)
009530: 0FA8 1C60      bclr    D7, ($1c60,A0)
009534: 670C           beq     $9542
009536: 4CE8 0C3F 1C60 movem.l ($1c60,A0), D0-D5/A2-A3
00953C: 48E9 0C3F 1C60 movem.l D0-D5/A2-A3, ($1c60,A1)
009542: 0FA8 1C80      bclr    D7, ($1c80,A0)
009546: 670C           beq     $9554
009548: 4CE8 0C3F 1C80 movem.l ($1c80,A0), D0-D5/A2-A3
00954E: 48E9 0C3F 1C80 movem.l D0-D5/A2-A3, ($1c80,A1)
009554: 0FA8 1CA0      bclr    D7, ($1ca0,A0)
009558: 670C           beq     $9566
00955A: 4CE8 0C3F 1CA0 movem.l ($1ca0,A0), D0-D5/A2-A3
009560: 48E9 0C3F 1CA0 movem.l D0-D5/A2-A3, ($1ca0,A1)
009566: 0FA8 1CC0      bclr    D7, ($1cc0,A0)
00956A: 670C           beq     $9578
00956C: 4CE8 0C3F 1CC0 movem.l ($1cc0,A0), D0-D5/A2-A3
009572: 48E9 0C3F 1CC0 movem.l D0-D5/A2-A3, ($1cc0,A1)
009578: 0FA8 1CE0      bclr    D7, ($1ce0,A0)
00957C: 670C           beq     $958a
00957E: 4CE8 0C3F 1CE0 movem.l ($1ce0,A0), D0-D5/A2-A3
009584: 48E9 0C3F 1CE0 movem.l D0-D5/A2-A3, ($1ce0,A1)
00958A: 0FA8 1D00      bclr    D7, ($1d00,A0)
00958E: 670C           beq     $959c
009590: 4CE8 0C3F 1D00 movem.l ($1d00,A0), D0-D5/A2-A3
009596: 48E9 0C3F 1D00 movem.l D0-D5/A2-A3, ($1d00,A1)
00959C: 0FA8 1D20      bclr    D7, ($1d20,A0)
0095A0: 670C           beq     $95ae
0095A2: 4CE8 0C3F 1D20 movem.l ($1d20,A0), D0-D5/A2-A3
0095A8: 48E9 0C3F 1D20 movem.l D0-D5/A2-A3, ($1d20,A1)
0095AE: 0FA8 1D40      bclr    D7, ($1d40,A0)
0095B2: 670C           beq     $95c0
0095B4: 4CE8 0C3F 1D40 movem.l ($1d40,A0), D0-D5/A2-A3
0095BA: 48E9 0C3F 1D40 movem.l D0-D5/A2-A3, ($1d40,A1)
0095C0: 0FA8 1D60      bclr    D7, ($1d60,A0)
0095C4: 670C           beq     $95d2
0095C6: 4CE8 0C3F 1D60 movem.l ($1d60,A0), D0-D5/A2-A3
0095CC: 48E9 0C3F 1D60 movem.l D0-D5/A2-A3, ($1d60,A1)
0095D2: 0FA8 1D80      bclr    D7, ($1d80,A0)
0095D6: 670C           beq     $95e4
0095D8: 4CE8 0C3F 1D80 movem.l ($1d80,A0), D0-D5/A2-A3
0095DE: 48E9 0C3F 1D80 movem.l D0-D5/A2-A3, ($1d80,A1)
0095E4: 0FA8 1DA0      bclr    D7, ($1da0,A0)
0095E8: 670C           beq     $95f6
0095EA: 4CE8 0C3F 1DA0 movem.l ($1da0,A0), D0-D5/A2-A3
0095F0: 48E9 0C3F 1DA0 movem.l D0-D5/A2-A3, ($1da0,A1)
0095F6: 0FA8 1DC0      bclr    D7, ($1dc0,A0)
0095FA: 670C           beq     $9608
0095FC: 4CE8 0C3F 1DC0 movem.l ($1dc0,A0), D0-D5/A2-A3
009602: 48E9 0C3F 1DC0 movem.l D0-D5/A2-A3, ($1dc0,A1)
009608: 0FA8 1DE0      bclr    D7, ($1de0,A0)
00960C: 670C           beq     $961a
00960E: 4CE8 0C3F 1DE0 movem.l ($1de0,A0), D0-D5/A2-A3
009614: 48E9 0C3F 1DE0 movem.l D0-D5/A2-A3, ($1de0,A1)
00961A: 0FA8 1E00      bclr    D7, ($1e00,A0)
00961E: 670C           beq     $962c
009620: 4CE8 0C3F 1E00 movem.l ($1e00,A0), D0-D5/A2-A3
009626: 48E9 0C3F 1E00 movem.l D0-D5/A2-A3, ($1e00,A1)
00962C: 0FA8 1E20      bclr    D7, ($1e20,A0)
009630: 670C           beq     $963e
009632: 4CE8 0C3F 1E20 movem.l ($1e20,A0), D0-D5/A2-A3
009638: 48E9 0C3F 1E20 movem.l D0-D5/A2-A3, ($1e20,A1)
00963E: 0FA8 1E40      bclr    D7, ($1e40,A0)
009642: 670C           beq     $9650
009644: 4CE8 0C3F 1E40 movem.l ($1e40,A0), D0-D5/A2-A3
00964A: 48E9 0C3F 1E40 movem.l D0-D5/A2-A3, ($1e40,A1)
009650: 0FA8 1E60      bclr    D7, ($1e60,A0)
009654: 670C           beq     $9662
009656: 4CE8 0C3F 1E60 movem.l ($1e60,A0), D0-D5/A2-A3
00965C: 48E9 0C3F 1E60 movem.l D0-D5/A2-A3, ($1e60,A1)
009662: 0FA8 1E80      bclr    D7, ($1e80,A0)
009666: 670C           beq     $9674
009668: 4CE8 0C3F 1E80 movem.l ($1e80,A0), D0-D5/A2-A3
00966E: 48E9 0C3F 1E80 movem.l D0-D5/A2-A3, ($1e80,A1)
009674: 0FA8 1EA0      bclr    D7, ($1ea0,A0)
009678: 670C           beq     $9686
00967A: 4CE8 0C3F 1EA0 movem.l ($1ea0,A0), D0-D5/A2-A3
009680: 48E9 0C3F 1EA0 movem.l D0-D5/A2-A3, ($1ea0,A1)
009686: 0FA8 1EC0      bclr    D7, ($1ec0,A0)
00968A: 670C           beq     $9698
00968C: 4CE8 0C3F 1EC0 movem.l ($1ec0,A0), D0-D5/A2-A3
009692: 48E9 0C3F 1EC0 movem.l D0-D5/A2-A3, ($1ec0,A1)
009698: 0FA8 1EE0      bclr    D7, ($1ee0,A0)
00969C: 670C           beq     $96aa
00969E: 4CE8 0C3F 1EE0 movem.l ($1ee0,A0), D0-D5/A2-A3
0096A4: 48E9 0C3F 1EE0 movem.l D0-D5/A2-A3, ($1ee0,A1)
0096AA: 0FA8 1F00      bclr    D7, ($1f00,A0)
0096AE: 670C           beq     $96bc
0096B0: 4CE8 0C3F 1F00 movem.l ($1f00,A0), D0-D5/A2-A3
0096B6: 48E9 0C3F 1F00 movem.l D0-D5/A2-A3, ($1f00,A1)
0096BC: 0FA8 1F20      bclr    D7, ($1f20,A0)
0096C0: 670C           beq     $96ce
0096C2: 4CE8 0C3F 1F20 movem.l ($1f20,A0), D0-D5/A2-A3
0096C8: 48E9 0C3F 1F20 movem.l D0-D5/A2-A3, ($1f20,A1)
0096CE: 0FA8 1F40      bclr    D7, ($1f40,A0)
0096D2: 670C           beq     $96e0
0096D4: 4CE8 0C3F 1F40 movem.l ($1f40,A0), D0-D5/A2-A3
0096DA: 48E9 0C3F 1F40 movem.l D0-D5/A2-A3, ($1f40,A1)
0096E0: 0FA8 1F60      bclr    D7, ($1f60,A0)
0096E4: 670C           beq     $96f2
0096E6: 4CE8 0C3F 1F60 movem.l ($1f60,A0), D0-D5/A2-A3
0096EC: 48E9 0C3F 1F60 movem.l D0-D5/A2-A3, ($1f60,A1)
0096F2: 0FA8 1F80      bclr    D7, ($1f80,A0)
0096F6: 670C           beq     $9704
0096F8: 4CE8 0C3F 1F80 movem.l ($1f80,A0), D0-D5/A2-A3
0096FE: 48E9 0C3F 1F80 movem.l D0-D5/A2-A3, ($1f80,A1)
009704: 0FA8 1FA0      bclr    D7, ($1fa0,A0)
009708: 670C           beq     $9716
00970A: 4CE8 0C3F 1FA0 movem.l ($1fa0,A0), D0-D5/A2-A3
009710: 48E9 0C3F 1FA0 movem.l D0-D5/A2-A3, ($1fa0,A1)
009716: 0FA8 1FC0      bclr    D7, ($1fc0,A0)
00971A: 670C           beq     $9728
00971C: 4CE8 0C3F 1FC0 movem.l ($1fc0,A0), D0-D5/A2-A3
009722: 48E9 0C3F 1FC0 movem.l D0-D5/A2-A3, ($1fc0,A1)
009728: 0FA8 1FE0      bclr    D7, ($1fe0,A0)
00972C: 670C           beq     $973a
00972E: 4CE8 0C3F 1FE0 movem.l ($1fe0,A0), D0-D5/A2-A3
009734: 48E9 0C3F 1FE0 movem.l D0-D5/A2-A3, ($1fe0,A1)
00973A: 13C0 0030 0001 move.b  D0, $300001.l
009740: 4A2D 1944      tst.b   ($1944,A5)
009744: 6600 000C      bne     $9752
009748: 13C0 003A 000F move.b  D0, $3a000f.l
00974E: 6000 0008      bra     $9758
009752: 13C0 003A 001F move.b  D0, $3a001f.l
009758: 4E75           rts
00975A: 13C0 003A 001F move.b  D0, $3a001f.l
009760: 41F9 0040 0000 lea     $400000.l, A0
009766: D0ED 1942      adda.w  ($1942,A5), A0
00976A: 43ED 18FA      lea     ($18fa,A5), A1
00976E: 4CD1 00FF      movem.l (A1), D0-D7
009772: 48E0 FF00      movem.l D0-D7, -(A0)
009776: 48E0 FF00      movem.l D0-D7, -(A0)
00977A: 48E0 FF00      movem.l D0-D7, -(A0)
00977E: 48E0 FF00      movem.l D0-D7, -(A0)
009782: 48E0 FF00      movem.l D0-D7, -(A0)
009786: 48E0 FF00      movem.l D0-D7, -(A0)
00978A: 48E0 FF00      movem.l D0-D7, -(A0)
00978E: 48E0 FF00      movem.l D0-D7, -(A0)
009792: 48E0 FF00      movem.l D0-D7, -(A0)
009796: 48E0 FF00      movem.l D0-D7, -(A0)
00979A: 48E0 FF00      movem.l D0-D7, -(A0)
00979E: 48E0 FF00      movem.l D0-D7, -(A0)
0097A2: 48E0 FF00      movem.l D0-D7, -(A0)
0097A6: 48E0 FF00      movem.l D0-D7, -(A0)
0097AA: 48E0 FF00      movem.l D0-D7, -(A0)
0097AE: 48E0 FF00      movem.l D0-D7, -(A0)
0097B2: 48E0 FF00      movem.l D0-D7, -(A0)
0097B6: 48E0 FF00      movem.l D0-D7, -(A0)
0097BA: 48E0 FF00      movem.l D0-D7, -(A0)
0097BE: 48E0 FF00      movem.l D0-D7, -(A0)
0097C2: 48E0 FF00      movem.l D0-D7, -(A0)
0097C6: 48E0 FF00      movem.l D0-D7, -(A0)
0097CA: 48E0 FF00      movem.l D0-D7, -(A0)
0097CE: 48E0 FF00      movem.l D0-D7, -(A0)
0097D2: 48E0 FF00      movem.l D0-D7, -(A0)
0097D6: 48E0 FF00      movem.l D0-D7, -(A0)
0097DA: 48E0 FF00      movem.l D0-D7, -(A0)
0097DE: 48E0 FF00      movem.l D0-D7, -(A0)
0097E2: 48E0 FF00      movem.l D0-D7, -(A0)
0097E6: 48E0 FF00      movem.l D0-D7, -(A0)
0097EA: 48E0 FF00      movem.l D0-D7, -(A0)
0097EE: 48E0 FF00      movem.l D0-D7, -(A0)
0097F2: 48E0 FF00      movem.l D0-D7, -(A0)
0097F6: 48E0 FF00      movem.l D0-D7, -(A0)
0097FA: 48E0 FF00      movem.l D0-D7, -(A0)
0097FE: 48E0 FF00      movem.l D0-D7, -(A0)
009802: 48E0 FF00      movem.l D0-D7, -(A0)
009806: 48E0 FF00      movem.l D0-D7, -(A0)
00980A: 48E0 FF00      movem.l D0-D7, -(A0)
00980E: 48E0 FF00      movem.l D0-D7, -(A0)
009812: 48E0 FF00      movem.l D0-D7, -(A0)
009816: 48E0 FF00      movem.l D0-D7, -(A0)
00981A: 48E0 FF00      movem.l D0-D7, -(A0)
00981E: 48E0 FF00      movem.l D0-D7, -(A0)
009822: 48E0 FF00      movem.l D0-D7, -(A0)
009826: 48E0 FF00      movem.l D0-D7, -(A0)
00982A: 48E0 FF00      movem.l D0-D7, -(A0)
00982E: 48E0 FF00      movem.l D0-D7, -(A0)
009832: 48E0 FF00      movem.l D0-D7, -(A0)
009836: 48E0 FF00      movem.l D0-D7, -(A0)
00983A: 48E0 FF00      movem.l D0-D7, -(A0)
00983E: 48E0 FF00      movem.l D0-D7, -(A0)
009842: 48E0 FF00      movem.l D0-D7, -(A0)
009846: 48E0 FF00      movem.l D0-D7, -(A0)
00984A: 48E0 FF00      movem.l D0-D7, -(A0)
00984E: 48E0 FF00      movem.l D0-D7, -(A0)
009852: 48E0 FF00      movem.l D0-D7, -(A0)
009856: 48E0 FF00      movem.l D0-D7, -(A0)
00985A: 48E0 FF00      movem.l D0-D7, -(A0)
00985E: 48E0 FF00      movem.l D0-D7, -(A0)
009862: 48E0 FF00      movem.l D0-D7, -(A0)
009866: 48E0 FF00      movem.l D0-D7, -(A0)
00986A: 48E0 FF00      movem.l D0-D7, -(A0)
00986E: 48E0 FF00      movem.l D0-D7, -(A0)
009872: 48E0 FF00      movem.l D0-D7, -(A0)
009876: 48E0 FF00      movem.l D0-D7, -(A0)
00987A: 48E0 FF00      movem.l D0-D7, -(A0)
00987E: 48E0 FF00      movem.l D0-D7, -(A0)
009882: 48E0 FF00      movem.l D0-D7, -(A0)
009886: 48E0 FF00      movem.l D0-D7, -(A0)
00988A: 48E0 FF00      movem.l D0-D7, -(A0)
00988E: 48E0 FF00      movem.l D0-D7, -(A0)
009892: 48E0 FF00      movem.l D0-D7, -(A0)
009896: 48E0 FF00      movem.l D0-D7, -(A0)
00989A: 48E0 FF00      movem.l D0-D7, -(A0)
00989E: 48E0 FF00      movem.l D0-D7, -(A0)
0098A2: 48E0 FF00      movem.l D0-D7, -(A0)
0098A6: 48E0 FF00      movem.l D0-D7, -(A0)
0098AA: 48E0 FF00      movem.l D0-D7, -(A0)
0098AE: 48E0 FF00      movem.l D0-D7, -(A0)
0098B2: 48E0 FF00      movem.l D0-D7, -(A0)
0098B6: 48E0 FF00      movem.l D0-D7, -(A0)
0098BA: 48E0 FF00      movem.l D0-D7, -(A0)
0098BE: 48E0 FF00      movem.l D0-D7, -(A0)
0098C2: 48E0 FF00      movem.l D0-D7, -(A0)
0098C6: 48E0 FF00      movem.l D0-D7, -(A0)
0098CA: 48E0 FF00      movem.l D0-D7, -(A0)
0098CE: 48E0 FF00      movem.l D0-D7, -(A0)
0098D2: 48E0 FF00      movem.l D0-D7, -(A0)
0098D6: 48E0 FF00      movem.l D0-D7, -(A0)
0098DA: 48E0 FF00      movem.l D0-D7, -(A0)
0098DE: 48E0 FF00      movem.l D0-D7, -(A0)
0098E2: 48E0 FF00      movem.l D0-D7, -(A0)
0098E6: 48E0 FF00      movem.l D0-D7, -(A0)
0098EA: 48E0 FF00      movem.l D0-D7, -(A0)
0098EE: 48E0 FF00      movem.l D0-D7, -(A0)
0098F2: 48E0 FF00      movem.l D0-D7, -(A0)
0098F6: 48E0 FF00      movem.l D0-D7, -(A0)
0098FA: 48E0 FF00      movem.l D0-D7, -(A0)
0098FE: 48E0 FF00      movem.l D0-D7, -(A0)
009902: 48E0 FF00      movem.l D0-D7, -(A0)
009906: 48E0 FF00      movem.l D0-D7, -(A0)
00990A: 48E0 FF00      movem.l D0-D7, -(A0)
00990E: 48E0 FF00      movem.l D0-D7, -(A0)
009912: 48E0 FF00      movem.l D0-D7, -(A0)
009916: 48E0 FF00      movem.l D0-D7, -(A0)
00991A: 48E0 FF00      movem.l D0-D7, -(A0)
00991E: 48E0 FF00      movem.l D0-D7, -(A0)
009922: 48E0 FF00      movem.l D0-D7, -(A0)
009926: 48E0 FF00      movem.l D0-D7, -(A0)
00992A: 48E0 FF00      movem.l D0-D7, -(A0)
00992E: 48E0 FF00      movem.l D0-D7, -(A0)
009932: 48E0 FF00      movem.l D0-D7, -(A0)
009936: 48E0 FF00      movem.l D0-D7, -(A0)
00993A: 48E0 FF00      movem.l D0-D7, -(A0)
00993E: 48E0 FF00      movem.l D0-D7, -(A0)
009942: 48E0 FF00      movem.l D0-D7, -(A0)
009946: 48E0 FF00      movem.l D0-D7, -(A0)
00994A: 48E0 FF00      movem.l D0-D7, -(A0)
00994E: 48E0 FF00      movem.l D0-D7, -(A0)
009952: 48E0 FF00      movem.l D0-D7, -(A0)
009956: 48E0 FF00      movem.l D0-D7, -(A0)
00995A: 48E0 FF00      movem.l D0-D7, -(A0)
00995E: 48E0 FF00      movem.l D0-D7, -(A0)
009962: 48E0 FF00      movem.l D0-D7, -(A0)
009966: 48E0 FF00      movem.l D0-D7, -(A0)
00996A: 48E0 FF00      movem.l D0-D7, -(A0)
00996E: 48E0 FF00      movem.l D0-D7, -(A0)
009972: 48E0 FF00      movem.l D0-D7, -(A0)
009976: 48E0 FF00      movem.l D0-D7, -(A0)
00997A: 48E0 FF00      movem.l D0-D7, -(A0)
00997E: 48E0 FF00      movem.l D0-D7, -(A0)
009982: 48E0 FF00      movem.l D0-D7, -(A0)
009986: 48E0 FF00      movem.l D0-D7, -(A0)
00998A: 48E0 FF00      movem.l D0-D7, -(A0)
00998E: 48E0 FF00      movem.l D0-D7, -(A0)
009992: 48E0 FF00      movem.l D0-D7, -(A0)
009996: 48E0 FF00      movem.l D0-D7, -(A0)
00999A: 48E0 FF00      movem.l D0-D7, -(A0)
00999E: 48E0 FF00      movem.l D0-D7, -(A0)
0099A2: 48E0 FF00      movem.l D0-D7, -(A0)
0099A6: 48E0 FF00      movem.l D0-D7, -(A0)
0099AA: 48E0 FF00      movem.l D0-D7, -(A0)
0099AE: 48E0 FF00      movem.l D0-D7, -(A0)
0099B2: 48E0 FF00      movem.l D0-D7, -(A0)
0099B6: 48E0 FF00      movem.l D0-D7, -(A0)
0099BA: 48E0 FF00      movem.l D0-D7, -(A0)
0099BE: 48E0 FF00      movem.l D0-D7, -(A0)
0099C2: 48E0 FF00      movem.l D0-D7, -(A0)
0099C6: 48E0 FF00      movem.l D0-D7, -(A0)
0099CA: 48E0 FF00      movem.l D0-D7, -(A0)
0099CE: 48E0 FF00      movem.l D0-D7, -(A0)
0099D2: 48E0 FF00      movem.l D0-D7, -(A0)
0099D6: 48E0 FF00      movem.l D0-D7, -(A0)
0099DA: 48E0 FF00      movem.l D0-D7, -(A0)
0099DE: 48E0 FF00      movem.l D0-D7, -(A0)
0099E2: 48E0 FF00      movem.l D0-D7, -(A0)
0099E6: 48E0 FF00      movem.l D0-D7, -(A0)
0099EA: 48E0 FF00      movem.l D0-D7, -(A0)
0099EE: 48E0 FF00      movem.l D0-D7, -(A0)
0099F2: 48E0 FF00      movem.l D0-D7, -(A0)
0099F6: 48E0 FF00      movem.l D0-D7, -(A0)
0099FA: 48E0 FF00      movem.l D0-D7, -(A0)
0099FE: 48E0 FF00      movem.l D0-D7, -(A0)
009A02: 48E0 FF00      movem.l D0-D7, -(A0)
009A06: 48E0 FF00      movem.l D0-D7, -(A0)
009A0A: 48E0 FF00      movem.l D0-D7, -(A0)
009A0E: 48E0 FF00      movem.l D0-D7, -(A0)
009A12: 48E0 FF00      movem.l D0-D7, -(A0)
009A16: 48E0 FF00      movem.l D0-D7, -(A0)
009A1A: 48E0 FF00      movem.l D0-D7, -(A0)
009A1E: 48E0 FF00      movem.l D0-D7, -(A0)
009A22: 48E0 FF00      movem.l D0-D7, -(A0)
009A26: 48E0 FF00      movem.l D0-D7, -(A0)
009A2A: 48E0 FF00      movem.l D0-D7, -(A0)
009A2E: 48E0 FF00      movem.l D0-D7, -(A0)
009A32: 48E0 FF00      movem.l D0-D7, -(A0)
009A36: 48E0 FF00      movem.l D0-D7, -(A0)
009A3A: 48E0 FF00      movem.l D0-D7, -(A0)
009A3E: 48E0 FF00      movem.l D0-D7, -(A0)
009A42: 48E0 FF00      movem.l D0-D7, -(A0)
009A46: 48E0 FF00      movem.l D0-D7, -(A0)
009A4A: 48E0 FF00      movem.l D0-D7, -(A0)
009A4E: 48E0 FF00      movem.l D0-D7, -(A0)
009A52: 48E0 FF00      movem.l D0-D7, -(A0)
009A56: 48E0 FF00      movem.l D0-D7, -(A0)
009A5A: 48E0 FF00      movem.l D0-D7, -(A0)
009A5E: 48E0 FF00      movem.l D0-D7, -(A0)
009A62: 48E0 FF00      movem.l D0-D7, -(A0)
009A66: 48E0 FF00      movem.l D0-D7, -(A0)
009A6A: 48E0 FF00      movem.l D0-D7, -(A0)
009A6E: 48E0 FF00      movem.l D0-D7, -(A0)
009A72: 48E0 FF00      movem.l D0-D7, -(A0)
009A76: 48E0 FF00      movem.l D0-D7, -(A0)
009A7A: 48E0 FF00      movem.l D0-D7, -(A0)
009A7E: 48E0 FF00      movem.l D0-D7, -(A0)
009A82: 48E0 FF00      movem.l D0-D7, -(A0)
009A86: 48E0 FF00      movem.l D0-D7, -(A0)
009A8A: 48E0 FF00      movem.l D0-D7, -(A0)
009A8E: 48E0 FF00      movem.l D0-D7, -(A0)
009A92: 48E0 FF00      movem.l D0-D7, -(A0)
009A96: 48E0 FF00      movem.l D0-D7, -(A0)
009A9A: 48E0 FF00      movem.l D0-D7, -(A0)
009A9E: 48E0 FF00      movem.l D0-D7, -(A0)
009AA2: 48E0 FF00      movem.l D0-D7, -(A0)
009AA6: 48E0 FF00      movem.l D0-D7, -(A0)
009AAA: 48E0 FF00      movem.l D0-D7, -(A0)
009AAE: 48E0 FF00      movem.l D0-D7, -(A0)
009AB2: 48E0 FF00      movem.l D0-D7, -(A0)
009AB6: 48E0 FF00      movem.l D0-D7, -(A0)
009ABA: 48E0 FF00      movem.l D0-D7, -(A0)
009ABE: 48E0 FF00      movem.l D0-D7, -(A0)
009AC2: 48E0 FF00      movem.l D0-D7, -(A0)
009AC6: 48E0 FF00      movem.l D0-D7, -(A0)
009ACA: 48E0 FF00      movem.l D0-D7, -(A0)
009ACE: 48E0 FF00      movem.l D0-D7, -(A0)
009AD2: 48E0 FF00      movem.l D0-D7, -(A0)
009AD6: 48E0 FF00      movem.l D0-D7, -(A0)
009ADA: 48E0 FF00      movem.l D0-D7, -(A0)
009ADE: 48E0 FF00      movem.l D0-D7, -(A0)
009AE2: 48E0 FF00      movem.l D0-D7, -(A0)
009AE6: 48E0 FF00      movem.l D0-D7, -(A0)
009AEA: 48E0 FF00      movem.l D0-D7, -(A0)
009AEE: 48E0 FF00      movem.l D0-D7, -(A0)
009AF2: 48E0 FF00      movem.l D0-D7, -(A0)
009AF6: 48E0 FF00      movem.l D0-D7, -(A0)
009AFA: 48E0 FF00      movem.l D0-D7, -(A0)
009AFE: 48E0 FF00      movem.l D0-D7, -(A0)
009B02: 48E0 FF00      movem.l D0-D7, -(A0)
009B06: 48E0 FF00      movem.l D0-D7, -(A0)
009B0A: 48E0 FF00      movem.l D0-D7, -(A0)
009B0E: 48E0 FF00      movem.l D0-D7, -(A0)
009B12: 48E0 FF00      movem.l D0-D7, -(A0)
009B16: 48E0 FF00      movem.l D0-D7, -(A0)
009B1A: 48E0 FF00      movem.l D0-D7, -(A0)
009B1E: 48E0 FF00      movem.l D0-D7, -(A0)
009B22: 48E0 FF00      movem.l D0-D7, -(A0)
009B26: 48E0 FF00      movem.l D0-D7, -(A0)
009B2A: 48E0 FF00      movem.l D0-D7, -(A0)
009B2E: 48E0 FF00      movem.l D0-D7, -(A0)
009B32: 48E0 FF00      movem.l D0-D7, -(A0)
009B36: 48E0 FF00      movem.l D0-D7, -(A0)
009B3A: 48E0 FF00      movem.l D0-D7, -(A0)
009B3E: 48E0 FF00      movem.l D0-D7, -(A0)
009B42: 48E0 FF00      movem.l D0-D7, -(A0)
009B46: 48E0 FF00      movem.l D0-D7, -(A0)
009B4A: 48E0 FF00      movem.l D0-D7, -(A0)
009B4E: 48E0 FF00      movem.l D0-D7, -(A0)
009B52: 48E0 FF00      movem.l D0-D7, -(A0)
009B56: 48E0 FF00      movem.l D0-D7, -(A0)
009B5A: 48E0 FF00      movem.l D0-D7, -(A0)
009B5E: 48E0 FF00      movem.l D0-D7, -(A0)
009B62: 48E0 FF00      movem.l D0-D7, -(A0)
009B66: 48E0 FF00      movem.l D0-D7, -(A0)
009B6A: 48E0 FF00      movem.l D0-D7, -(A0)
009B6E: 48E0 FF00      movem.l D0-D7, -(A0)
009B72: 046D 2000 1942 subi.w  #$2000, ($1942,A5)
009B78: 6600 0006      bne     $9b80
009B7C: 30BC 0000      move.w  #$0, (A0)
009B80: 6000 FBBE      bra     $9740
