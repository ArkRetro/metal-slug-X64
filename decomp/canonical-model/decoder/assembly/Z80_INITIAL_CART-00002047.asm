2047: 32 18 F8 ld   ($F818),a
204A: D6 20    sub  $20
204C: 32 12 F8 ld   ($F812),a
204F: 3A 19 F8 ld   a,($F819)
2052: FE 10    cp   $10
2054: D2 C6 20 jp   nc,$20C6
2057: 87       add  a,a
2058: 5F       ld   e,a
2059: 16 00    ld   d,$00
205B: 21 64 20 ld   hl,$2064
205E: 19       add  hl,de
205F: 7E       ld   a,(hl)
2060: 23       inc  hl
2061: 66       ld   h,(hl)
2062: 6F       ld   l,a
2063: E9       jp   (hl)
