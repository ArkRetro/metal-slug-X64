26C4: 3A 11 F8    ld   a,($F811)
26C7: 6F          ld   l,a
26C8: 3A 27 F8    ld   a,($F827)
26CB: A7          and  a
26CC: 28 15       jr   z,$26E3
26CE: 2C          inc  l
26CF: 28 12       jr   z,$26E3
26D1: 08          ex   af,af'
26D2: 2D          dec  l
26D3: 7D          ld   a,l
26D4: 28 0D       jr   z,$26E3
26D6: 26 06       ld   h,$06
26D8: 7E          ld   a,(hl)
26D9: 08          ex   af,af'
26DA: 6F          ld   l,a
26DB: 08          ex   af,af'
26DC: 86          add  a,(hl)
26DD: 6F          ld   l,a
26DE: 9F          sbc  a,a
26DF: 30 02       jr   nc,$26E3
26E1: 24          inc  h
26E2: 7E          ld   a,(hl)
26E3: FD 6E 12    ld   l,(iy+$12)
26E6: A7          and  a
26E7: 28 15       jr   z,$26FE
26E9: 2C          inc  l
26EA: 28 12       jr   z,$26FE
26EC: 08          ex   af,af'
26ED: 2D          dec  l
26EE: 7D          ld   a,l
26EF: 28 0D       jr   z,$26FE
26F1: 26 06       ld   h,$06
26F3: 7E          ld   a,(hl)
26F4: 08          ex   af,af'
26F5: 6F          ld   l,a
26F6: 08          ex   af,af'
26F7: 86          add  a,(hl)
26F8: 6F          ld   l,a
26F9: 9F          sbc  a,a
26FA: 30 02       jr   nc,$26FE
26FC: 24          inc  h
26FD: 7E          ld   a,(hl)
26FE: 6F          ld   l,a
26FF: 3E FF       ld   a,$FF
2701: FD CB 09 4E bit  1,(iy+$09)
2705: CA 0B 27    jp   z,$270B
2708: 3A 05 F8    ld   a,($F805)
270B: A7          and  a
270C: 28 15       jr   z,$2723
270E: 2C          inc  l
270F: 28 12       jr   z,$2723
2711: 08          ex   af,af'
2712: 2D          dec  l
2713: 7D          ld   a,l
2714: 28 0D       jr   z,$2723
2716: 26 06       ld   h,$06
2718: 7E          ld   a,(hl)
2719: 08          ex   af,af'
271A: 6F          ld   l,a
271B: 08          ex   af,af'
271C: 86          add  a,(hl)
271D: 6F          ld   l,a
271E: 9F          sbc  a,a
271F: 30 02       jr   nc,$2723
2721: 24          inc  h
2722: 7E          ld   a,(hl)
2723: 21 2C F8    ld   hl,$F82C
2726: BE          cp   (hl)
2727: 77          ld   (hl),a
2728: CA 34 27    jp   z,$2734
272B: 6F          ld   l,a
272C: 26 0A       ld   h,$0A
272E: 5E          ld   e,(hl)
272F: 16 08       ld   d,$08
2731: 0E 04       ld   c,$04
2733: CF          rst  $08
2734: 0E 01       ld   c,$01
2736: FD CB 09 4E bit  1,(iy+$09)
273A: 28 04       jr   z,$2740
273C: 3A 09 F8    ld   a,($F809)
273F: 4F          ld   c,a
2740: 21 22 F8    ld   hl,$F822
2743: 7E          ld   a,(hl)
2744: 81          add  a,c
2745: DE 00       sbc  a,$00
2747: 77          ld   (hl),a
2748: 21 21 F8    ld   hl,$F821
274B: 7E          ld   a,(hl)
274C: A7          and  a
274D: CA 61 27    jp   z,$2761
2750: 91          sub  c
2751: 77          ld   (hl),a
2752: 20 22       jr   nz,$2776
2754: FD CB 09 66 bit  4,(iy+$09)
2758: 28 16       jr   z,$2770
275A: DD CB 03 E6 set  4,(ix+$03)
275E: C3 76 27    jp   $2776
2761: FD CB 09 66 bit  4,(iy+$09)
2765: 20 0F       jr   nz,$2776
2767: 21 1F F8    ld   hl,$F81F
276A: CB 66       bit  4,(hl)
276C: 28 08       jr   z,$2776
276E: CB A6       res  4,(hl)
2770: 21 32 F8    ld   hl,$F832
2773: CD CA 21    call $21CA
2776: DD 21 69 F8 ld   ix,$F869
277A: FD 2A 69 F8 ld   iy,($F869)
277E: FD CB 09 5E bit  3,(iy+$09)
2782: C4 CC 1F    call nz,$1FCC
2785: 21 7F F8    ld   hl,$F87F
2788: CD DE 21    call $21DE
278B: 32 11 F8    ld   ($F811),a
278E: A7          and  a
278F: CA F2 28    jp   z,$28F2
2792: 21 8D F8    ld   hl,$F88D
2795: FD 56 0D    ld   d,(iy+$0d)
2798: CD CF 22    call $22CF
279B: 91          sub  c
279C: 3D          dec  a
279D: 6F          ld   l,a
279E: 3A 11 F8    ld   a,($F811)
27A1: A7          and  a
27A2: 28 15       jr   z,$27B9
27A4: 2C          inc  l
27A5: 28 12       jr   z,$27B9
27A7: 08          ex   af,af'
27A8: 2D          dec  l
27A9: 7D          ld   a,l
27AA: 28 0D       jr   z,$27B9
27AC: 26 06       ld   h,$06
27AE: 7E          ld   a,(hl)
27AF: 08          ex   af,af'
27B0: 6F          ld   l,a
27B1: 08          ex   af,af'
27B2: 86          add  a,(hl)
27B3: 6F          ld   l,a
27B4: 9F          sbc  a,a
27B5: 30 02       jr   nc,$27B9
27B7: 24          inc  h
27B8: 7E          ld   a,(hl)
27B9: 32 11 F8    ld   ($F811),a
27BC: A7          and  a
27BD: CA F2 28    jp   z,$28F2
27C0: 3A 74 F8    ld   a,($F874)
27C3: A7          and  a
27C4: CA F2 28    jp   z,$28F2
27C7: 3A 7C F8    ld   a,($F87C)
27CA: A7          and  a
27CB: CA 01 28    jp   z,$2801
27CE: 21 9B F8    ld   hl,$F89B
27D1: FD 56 0C    ld   d,(iy+$0c)
27D4: CD CF 22    call $22CF
27D7: 6F          ld   l,a
27D8: 26 00       ld   h,$00
27DA: 44          ld   b,h
27DB: 29          add  hl,hl
27DC: A7          and  a
27DD: ED 42       sbc  hl,bc
27DF: 7D          ld   a,l
27E0: CB 2C       sra  h
27E2: 1F          rra
27E3: CB 2C       sra  h
27E5: 1F          rra
27E6: CB 2C       sra  h
27E8: 1F          rra
27E9: 21 71 F8    ld   hl,$F871
27EC: 47          ld   b,a
27ED: 7E          ld   a,(hl)
27EE: E6 1F       and  $1F
27F0: 80          add  a,b
27F1: F2 F5 27    jp   p,$27F5
27F4: AF          xor  a
27F5: FE 20       cp   $20
27F7: 38 02       jr   c,$27FB
27F9: 3E 1F       ld   a,$1F
27FB: 32 03 F9    ld   ($F903),a
27FE: C3 F2 28    jp   $28F2
2801: 2A 72 F8    ld   hl,($F872)
2804: ED 5B 7A F8 ld   de,($F87A)
2808: 19          add  hl,de
2809: FD 5E 10    ld   e,(iy+$10)
280C: FD 56 11    ld   d,(iy+$11)
280F: 19          add  hl,de
2810: 22 0F F8    ld   ($F80F),hl
2813: 21 9B F8    ld   hl,$F89B
2816: FD 56 0C    ld   d,(iy+$0c)
2819: CD CF 22    call $22CF
281C: 6F          ld   l,a
281D: 26 00       ld   h,$00
281F: 44          ld   b,h
2820: 29          add  hl,hl
2821: A7          and  a
2822: ED 42       sbc  hl,bc
2824: ED 5B 0F F8 ld   de,($F80F)
2828: 19          add  hl,de
2829: 22 0F F8    ld   ($F80F),hl
282C: 21 A9 F8    ld   hl,$F8A9
282F: CD 3F 24    call $243F
2832: ED 5B 0F F8 ld   de,($F80F)
2836: 19          add  hl,de
2837: EB          ex   de,hl
2838: 2A 77 F8    ld   hl,($F877)
283B: A7          and  a
283C: ED 52       sbc  hl,de
283E: CA F2 28    jp   z,$28F2
2841: ED 53 77 F8 ld   ($F877),de
2845: 7A          ld   a,d
2846: FE 10       cp   $10
2848: DA 4D 28    jp   c,$284D
284B: 3E 0F       ld   a,$0F
284D: 87          add  a,a
284E: 87          add  a,a
284F: 4F          ld   c,a
2850: 06 00       ld   b,$00
2852: 7B          ld   a,e
2853: D9          exx
2854: 6F          ld   l,a
2855: 26 02       ld   h,$02
2857: 7E          ld   a,(hl)
2858: 24          inc  h
2859: 6E          ld   l,(hl)
285A: 26 00       ld   h,$00
285C: D9          exx
285D: 21 62 28    ld   hl,$2862
2860: 09          add  hl,bc
2861: E9          jp   (hl)
