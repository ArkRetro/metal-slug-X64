21CA: 7E       ld   a,(hl)
21CB: A7       and  a
21CC: C8       ret  z
21CD: 36 04    ld   (hl),$04
21CF: 11 08 00 ld   de,$0008
21D2: 19       add  hl,de
21D3: EB       ex   de,hl
21D4: 21 02 00 ld   hl,$0002
21D7: 19       add  hl,de
21D8: EB       ex   de,hl
21D9: ED A0    ldi
21DB: 7E       ld   a,(hl)
21DC: 12       ld   (de),a
21DD: C9       ret
21DE: 7E       ld   a,(hl)
21DF: A7       and  a
21E0: C8       ret  z
21E1: EB       ex   de,hl
21E2: 21 0A 00 ld   hl,$000A
21E5: 19       add  hl,de
21E6: 4E       ld   c,(hl)
21E7: 23       inc  hl
21E8: 46       ld   b,(hl)
21E9: 23       inc  hl
21EA: 3D       dec  a
21EB: 28 17    jr   z,$2204
21ED: 3D       dec  a
21EE: 28 2D    jr   z,$221D
21F0: 79       ld   a,c
21F1: B0       or   b
21F2: C2 F8 21 jp   nz,$21F8
21F5: 23       inc  hl
21F6: 7E       ld   a,(hl)
21F7: C9       ret
21F8: 7E       ld   a,(hl)
21F9: 91       sub  c
21FA: 77       ld   (hl),a
21FB: 23       inc  hl
21FC: 7E       ld   a,(hl)
21FD: 98       sbc  a,b
21FE: 77       ld   (hl),a
21FF: D0       ret  nc
2200: AF       xor  a
2201: 77       ld   (hl),a
2202: 12       ld   (de),a
2203: C9       ret
2204: 7E       ld   a,(hl)
2205: 81       add  a,c
2206: 77       ld   (hl),a
2207: 23       inc  hl
2208: 7E       ld   a,(hl)
2209: 88       adc  a,b
220A: 77       ld   (hl),a
220B: D0       ret  nc
220C: 9F       sbc  a,a
220D: 77       ld   (hl),a
220E: 2B       dec  hl
220F: 2B       dec  hl
2210: EB       ex   de,hl
2211: 34       inc  (hl)
2212: 01 05 00 ld   bc,$0005
2215: 09       add  hl,bc
2216: ED A8    ldd
2218: 7E       ld   a,(hl)
2219: 12       ld   (de),a
221A: 3E FF    ld   a,$FF
221C: C9       ret
221D: 79       ld   a,c
221E: B0       or   b
221F: C2 25 22 jp   nz,$2225
2222: 23       inc  hl
2223: 7E       ld   a,(hl)
2224: C9       ret
2225: 7E       ld   a,(hl)
2226: 91       sub  c
2227: 77       ld   (hl),a
2228: 23       inc  hl
2229: 7E       ld   a,(hl)
222A: 98       sbc  a,b
222B: EB       ex   de,hl
222C: 23       inc  hl
222D: 4E       ld   c,(hl)
222E: 38 05    jr   c,$2235
2230: B9       cp   c
2231: 38 02    jr   c,$2235
2233: 12       ld   (de),a
2234: C9       ret
2235: EB       ex   de,hl
2236: 1B       dec  de
2237: 71       ld   (hl),c
2238: 79       ld   a,c
2239: A7       and  a
223A: C2 3F 22 jp   nz,$223F
223D: 12       ld   (de),a
223E: C9       ret
223F: 08       ex   af,af'
2240: EB       ex   de,hl
2241: 34       inc  (hl)
2242: 01 07 00 ld   bc,$0007
2245: 09       add  hl,bc
2246: 1B       dec  de
2247: 1B       dec  de
2248: ED A8    ldd
224A: 7E       ld   a,(hl)
224B: 12       ld   (de),a
224C: 08       ex   af,af'
224D: C9       ret
224E: AF       xor  a
224F: DD 77 00 ld   (ix+$00),a
2252: DD 77 01 ld   (ix+$01),a
2255: DD 77 02 ld   (ix+$02),a
2258: C9       ret
