22CF: 7E          ld   a,(hl)
22D0: 23          inc  hl
22D1: 46          ld   b,(hl)
22D2: 23          inc  hl
22D3: 4E          ld   c,(hl)
22D4: 23          inc  hl
22D5: 5E          ld   e,(hl)
22D6: 23          inc  hl
22D7: A7          and  a
22D8: 28 2E       jr   z,$2308
22DA: 78          ld   a,b
22DB: A7          and  a
22DC: CA 1A 23    jp   z,$231A
22DF: 7A          ld   a,d
22E0: 87          add  a,a
22E1: EB          ex   de,hl
22E2: A7          and  a
22E3: 28 15       jr   z,$22FA
22E5: 2C          inc  l
22E6: 28 12       jr   z,$22FA
22E8: 08          ex   af,af'
22E9: 2D          dec  l
22EA: 7D          ld   a,l
22EB: 28 0D       jr   z,$22FA
22ED: 26 06       ld   h,$06
22EF: 7E          ld   a,(hl)
22F0: 08          ex   af,af'
22F1: 6F          ld   l,a
22F2: 08          ex   af,af'
22F3: 86          add  a,(hl)
22F4: 6F          ld   l,a
22F5: 9F          sbc  a,a
22F6: 30 02       jr   nc,$22FA
22F8: 24          inc  h
22F9: 7E          ld   a,(hl)
22FA: EB          ex   de,hl
22FB: 81          add  a,c
22FC: 4F          ld   c,a
22FD: CA 1A 23    jp   z,$231A
2300: D2 13 23    jp   nc,$2313
2303: 0E FF       ld   c,$FF
2305: C3 13 23    jp   $2313
2308: 79          ld   a,c
2309: A7          and  a
230A: CA 1A 23    jp   z,$231A
230D: 7A          ld   a,d
230E: 80          add  a,b
230F: 47          ld   b,a
2310: CA 1A 23    jp   z,$231A
2313: 7E          ld   a,(hl)
2314: A7          and  a
2315: CA 1D 23    jp   z,$231D
2318: 3D          dec  a
2319: 77          ld   (hl),a
231A: AF          xor  a
231B: 4F          ld   c,a
231C: C9          ret
231D: 23          inc  hl
231E: 7E          ld   a,(hl)
231F: 37          scf
2320: 88          adc  a,b
2321: 5F          ld   e,a
2322: 77          ld   (hl),a
2323: 23          inc  hl
2324: 9F          sbc  a,a
2325: 47          ld   b,a
2326: 3E 00       ld   a,$00
2328: 8E          adc  a,(hl)
2329: 77          ld   (hl),a
232A: 23          inc  hl
232B: 56          ld   d,(hl)
232C: 23          inc  hl
232D: 15          dec  d
232E: 28 2E       jr   z,$235E
2330: 15          dec  d
2331: 28 0C       jr   z,$233F
2333: 15          dec  d
2334: 28 0E       jr   z,$2344
2336: 15          dec  d
2337: 28 11       jr   z,$234A
2339: 1F          rra
233A: 9F          sbc  a,a
233B: AB          xor  e
233C: C3 5D 23    jp   $235D
233F: 7B          ld   a,e
2340: 2F          cpl
2341: C3 5D 23    jp   $235D
2344: 1F          rra
2345: 3F          ccf
2346: 9F          sbc  a,a
2347: C3 5D 23    jp   $235D
234A: 04          inc  b
234B: 20 0F       jr   nz,$235C
234D: ED 5B 1A F8 ld   de,($F81A)
2351: ED 5F       ld   a,r
2353: 83          add  a,e
2354: 5A          ld   e,d
2355: AA          xor  d
2356: 57          ld   d,a
2357: ED 53 1A F8 ld   ($F81A),de
235B: 77          ld   (hl),a
235C: 7E          ld   a,(hl)
235D: 5F          ld   e,a
235E: 23          inc  hl
235F: 7E          ld   a,(hl)
2360: 23          inc  hl
2361: 86          add  a,(hl)
2362: 77          ld   (hl),a
2363: 23          inc  hl
2364: 7E          ld   a,(hl)
2365: 23          inc  hl
2366: 8E          adc  a,(hl)
2367: 47          ld   b,a
2368: 9F          sbc  a,a
2369: B0          or   b
236A: 77          ld   (hl),a
236B: 69          ld   l,c
236C: A7          and  a
236D: 28 15       jr   z,$2384
236F: 2C          inc  l
2370: 28 12       jr   z,$2384
2372: 08          ex   af,af'
2373: 2D          dec  l
2374: 7D          ld   a,l
2375: 28 0D       jr   z,$2384
2377: 26 06       ld   h,$06
2379: 7E          ld   a,(hl)
237A: 08          ex   af,af'
237B: 6F          ld   l,a
237C: 08          ex   af,af'
237D: 86          add  a,(hl)
237E: 6F          ld   l,a
237F: 9F          sbc  a,a
2380: 30 02       jr   nc,$2384
2382: 24          inc  h
2383: 7E          ld   a,(hl)
2384: 4F          ld   c,a
2385: 6B          ld   l,e
2386: A7          and  a
2387: 28 15       jr   z,$239E
2389: 2C          inc  l
238A: 28 12       jr   z,$239E
238C: 08          ex   af,af'
238D: 2D          dec  l
238E: 7D          ld   a,l
238F: 28 0D       jr   z,$239E
2391: 26 06       ld   h,$06
2393: 7E          ld   a,(hl)
2394: 08          ex   af,af'
2395: 6F          ld   l,a
2396: 08          ex   af,af'
2397: 86          add  a,(hl)
2398: 6F          ld   l,a
2399: 9F          sbc  a,a
239A: 30 02       jr   nc,$239E
239C: 24          inc  h
239D: 7E          ld   a,(hl)
239E: C9          ret
239F: AF          xor  a
23A0: DD 77 00    ld   (ix+$00),a
23A3: DD 77 01    ld   (ix+$01),a
23A6: C9          ret
