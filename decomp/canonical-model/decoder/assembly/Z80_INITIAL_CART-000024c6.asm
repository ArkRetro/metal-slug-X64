24C6: 11 00 08    ld   de,$0800
24C9: 0E 04       ld   c,$04
24CB: CF          rst  $08
24CC: 11 00 09    ld   de,$0900
24CF: 0E 04       ld   c,$04
24D1: CF          rst  $08
24D2: 11 00 0A    ld   de,$0A00
24D5: 0E 04       ld   c,$04
24D7: CF          rst  $08
24D8: 3E 38       ld   a,$38
24DA: 32 05 F9    ld   ($F905),a
24DD: 32 06 F9    ld   ($F906),a
24E0: 5F          ld   e,a
24E1: 16 07       ld   d,$07
24E3: 0E 04       ld   c,$04
24E5: CF          rst  $08
24E6: DD 21 1C F8 ld   ix,$F81C
24EA: DD 36 14 09 ld   (ix+$14),$09
24EE: CD 0E 25    call $250E
24F1: DD 21 69 F8 ld   ix,$F869
24F5: DD 36 14 12 ld   (ix+$14),$12
24F9: CD 0E 25    call $250E
24FC: DD 21 B6 F8 ld   ix,$F8B6
2500: DD 36 14 24 ld   (ix+$14),$24
2504: CD 0E 25    call $250E
2507: 21 00 00    ld   hl,$0000
250A: 22 03 F9    ld   ($F903),hl
250D: C9          ret
250E: CD 96 1F    call $1F96
2511: DD 36 02 00 ld   (ix+$02),$00
2515: AF          xor  a
2516: DD 77 15    ld   (ix+$15),a
2519: DD E5       push ix
251B: 11 16 00    ld   de,$0016
251E: DD 19       add  ix,de
2520: F7          rst  $30
2521: DD E1       pop  ix
2523: DD E5       push ix
2525: 11 32 00    ld   de,$0032
2528: DD 19       add  ix,de
252A: CD 4E 22    call $224E
252D: DD E1       pop  ix
252F: DD E5       push ix
2531: 11 24 00    ld   de,$0024
2534: DD 19       add  ix,de
2536: CD 4E 22    call $224E
2539: DD E1       pop  ix
253B: DD E5       push ix
253D: 11 40 00    ld   de,$0040
2540: DD 19       add  ix,de
2542: CD 9F 23    call $239F
2545: DD E1       pop  ix
2547: C9          ret
2548: DD 21 1C F8 ld   ix,$F81C
254C: FD 2A 1C F8 ld   iy,($F81C)
2550: FD CB 09 5E bit  3,(iy+$09)
2554: C4 CC 1F    call nz,$1FCC
2557: 21 32 F8    ld   hl,$F832
255A: CD DE 21    call $21DE
255D: 32 11 F8    ld   ($F811),a
2560: A7          and  a
2561: CA C4 26    jp   z,$26C4
2564: 21 40 F8    ld   hl,$F840
2567: FD 56 0D    ld   d,(iy+$0d)
256A: CD CF 22    call $22CF
256D: 91          sub  c
256E: 3D          dec  a
256F: 6F          ld   l,a
2570: 3A 11 F8    ld   a,($F811)
2573: A7          and  a
2574: 28 15       jr   z,$258B
2576: 2C          inc  l
2577: 28 12       jr   z,$258B
2579: 08          ex   af,af'
257A: 2D          dec  l
257B: 7D          ld   a,l
257C: 28 0D       jr   z,$258B
257E: 26 06       ld   h,$06
2580: 7E          ld   a,(hl)
2581: 08          ex   af,af'
2582: 6F          ld   l,a
2583: 08          ex   af,af'
2584: 86          add  a,(hl)
2585: 6F          ld   l,a
2586: 9F          sbc  a,a
2587: 30 02       jr   nc,$258B
2589: 24          inc  h
258A: 7E          ld   a,(hl)
258B: 32 11 F8    ld   ($F811),a
258E: A7          and  a
258F: CA C4 26    jp   z,$26C4
2592: 3A 27 F8    ld   a,($F827)
2595: A7          and  a
2596: CA C4 26    jp   z,$26C4
2599: 3A 2F F8    ld   a,($F82F)
259C: A7          and  a
259D: CA D3 25    jp   z,$25D3
25A0: 21 4E F8    ld   hl,$F84E
25A3: FD 56 0C    ld   d,(iy+$0c)
25A6: CD CF 22    call $22CF
25A9: 6F          ld   l,a
25AA: 26 00       ld   h,$00
25AC: 44          ld   b,h
25AD: 29          add  hl,hl
25AE: A7          and  a
25AF: ED 42       sbc  hl,bc
25B1: 7D          ld   a,l
25B2: CB 2C       sra  h
25B4: 1F          rra
25B5: CB 2C       sra  h
25B7: 1F          rra
25B8: CB 2C       sra  h
25BA: 1F          rra
25BB: 21 24 F8    ld   hl,$F824
25BE: 47          ld   b,a
25BF: 7E          ld   a,(hl)
25C0: E6 1F       and  $1F
25C2: 80          add  a,b
25C3: F2 C7 25    jp   p,$25C7
25C6: AF          xor  a
25C7: FE 20       cp   $20
25C9: 38 02       jr   c,$25CD
25CB: 3E 1F       ld   a,$1F
25CD: 32 03 F9    ld   ($F903),a
25D0: C3 C4 26    jp   $26C4
25D3: 2A 25 F8    ld   hl,($F825)
25D6: ED 5B 2D F8 ld   de,($F82D)
25DA: 19          add  hl,de
25DB: FD 5E 10    ld   e,(iy+$10)
25DE: FD 56 11    ld   d,(iy+$11)
25E1: 19          add  hl,de
25E2: 22 0F F8    ld   ($F80F),hl
25E5: 21 4E F8    ld   hl,$F84E
25E8: FD 56 0C    ld   d,(iy+$0c)
25EB: CD CF 22    call $22CF
25EE: 6F          ld   l,a
25EF: 26 00       ld   h,$00
25F1: 44          ld   b,h
25F2: 29          add  hl,hl
25F3: A7          and  a
25F4: ED 42       sbc  hl,bc
25F6: ED 5B 0F F8 ld   de,($F80F)
25FA: 19          add  hl,de
25FB: 22 0F F8    ld   ($F80F),hl
25FE: 21 5C F8    ld   hl,$F85C
2601: CD 3F 24    call $243F
2604: ED 5B 0F F8 ld   de,($F80F)
2608: 19          add  hl,de
2609: EB          ex   de,hl
260A: 2A 2A F8    ld   hl,($F82A)
260D: A7          and  a
260E: ED 52       sbc  hl,de
2610: CA C4 26    jp   z,$26C4
2613: ED 53 2A F8 ld   ($F82A),de
2617: 7A          ld   a,d
2618: FE 10       cp   $10
261A: DA 1F 26    jp   c,$261F
261D: 3E 0F       ld   a,$0F
261F: 87          add  a,a
2620: 87          add  a,a
2621: 4F          ld   c,a
2622: 06 00       ld   b,$00
2624: 7B          ld   a,e
2625: D9          exx
2626: 6F          ld   l,a
2627: 26 02       ld   h,$02
2629: 7E          ld   a,(hl)
262A: 24          inc  h
262B: 6E          ld   l,(hl)
262C: 26 00       ld   h,$00
262E: D9          exx
262F: 21 34 26    ld   hl,$2634
2632: 09          add  hl,bc
2633: E9          jp   (hl)
