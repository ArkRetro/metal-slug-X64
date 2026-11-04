243F: 46       ld   b,(hl)
2440: 23       inc  hl
2441: 4E       ld   c,(hl)
2442: 78       ld   a,b
2443: B1       or   c
2444: 20 03    jr   nz,$2449
2446: 60       ld   h,b
2447: 69       ld   l,c
2448: C9       ret
2449: 23       inc  hl
244A: 7E       ld   a,(hl)
244B: A7       and  a
244C: 28 0B    jr   z,$2459
244E: 3D       dec  a
244F: 77       ld   (hl),a
2450: 23       inc  hl
2451: 23       inc  hl
2452: 5E       ld   e,(hl)
2453: 23       inc  hl
2454: 56       ld   d,(hl)
2455: 23       inc  hl
2456: C3 72 24 jp   $2472
2459: 08       ex   af,af'
245A: 23       inc  hl
245B: 7E       ld   a,(hl)
245C: 90       sub  b
245D: 77       ld   (hl),a
245E: 23       inc  hl
245F: 5E       ld   e,(hl)
2460: 23       inc  hl
2461: 56       ld   d,(hl)
2462: EB       ex   de,hl
2463: 06 00    ld   b,$00
2465: ED 42    sbc  hl,bc
2467: EB       ex   de,hl
2468: 30 03    jr   nc,$246D
246A: 11 00 00 ld   de,$0000
246D: 72       ld   (hl),d
246E: 2B       dec  hl
246F: 73       ld   (hl),e
2470: 23       inc  hl
2471: 23       inc  hl
2472: 7E       ld   a,(hl)
2473: 87       add  a,a
2474: 38 0D    jr   c,$2483
2476: 23       inc  hl
2477: 4E       ld   c,(hl)
2478: 23       inc  hl
2479: 66       ld   h,(hl)
247A: 69       ld   l,c
247B: 87       add  a,a
247C: 30 02    jr   nc,$2480
247E: EB       ex   de,hl
247F: A7       and  a
2480: ED 52    sbc  hl,de
2482: C9       ret
2483: 87       add  a,a
2484: 30 07    jr   nc,$248D
2486: 21 00 00 ld   hl,$0000
2489: A7       and  a
248A: ED 52    sbc  hl,de
248C: C9       ret
248D: EB       ex   de,hl
248E: C9       ret
