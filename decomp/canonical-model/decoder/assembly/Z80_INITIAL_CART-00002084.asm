2084: C9          ret
2085: 3A 15 F8    ld   a,($F815)
2088: CD AF 4F    call $4FAF
208B: AF          xor  a
208C: 32 15 F8    ld   ($F815),a
208F: 21 84 00    ld   hl,$0084
2092: CD BE 57    call $57BE
2095: EB          ex   de,hl
2096: 3A 12 F8    ld   a,($F812)
2099: 6F          ld   l,a
209A: 26 00       ld   h,$00
209C: 29          add  hl,hl
209D: 29          add  hl,hl
209E: 19          add  hl,de
209F: 7E          ld   a,(hl)
20A0: 23          inc  hl
20A1: B6          or   (hl)
20A2: 23          inc  hl
20A3: B6          or   (hl)
20A4: C8          ret  z
20A5: 2B          dec  hl
20A6: 2B          dec  hl
20A7: CD BE 57    call $57BE
20AA: EB          ex   de,hl
20AB: DD 6B       ld   ixl,e
20AD: DD 62       ld   ixh,d
20AF: C3 CA 4F    jp   $4FCA
20B2: AF          xor  a
20B3: 32 15 F8    ld   ($F815),a
20B6: CD AF 4F    call $4FAF
20B9: DD 21 8F 24 ld   ix,$248F
20BD: C3 CA 4F    jp   $4FCA
20C0: 3E FF       ld   a,$FF
20C2: 32 13 F8    ld   ($F813),a
20C5: C9          ret
20C6: 3A 13 F8    ld   a,($F813)
20C9: A7          and  a
20CA: 28 0B       jr   z,$20D7
20CC: AF          xor  a
20CD: 32 13 F8    ld   ($F813),a
20D0: ED 4B 18 F8 ld   bc,($F818)
20D4: C3 04 52    jp   $5204
20D7: 3A 19 F8    ld   a,($F819)
20DA: D6 10       sub  $10
20DC: E6 0F       and  $0F
20DE: 87          add  a,a
20DF: 5F          ld   e,a
20E0: 16 00       ld   d,$00
20E2: 21 88 00    ld   hl,$0088
20E5: 19          add  hl,de
20E6: 19          add  hl,de
20E7: CD BE 57    call $57BE
20EA: EB          ex   de,hl
20EB: 3A 12 F8    ld   a,($F812)
20EE: 6F          ld   l,a
20EF: 26 00       ld   h,$00
20F1: 29          add  hl,hl
20F2: 29          add  hl,hl
20F3: 19          add  hl,de
20F4: 7E          ld   a,(hl)
20F5: 23          inc  hl
20F6: B6          or   (hl)
20F7: 23          inc  hl
20F8: B6          or   (hl)
20F9: C8          ret  z
20FA: 2B          dec  hl
20FB: 2B          dec  hl
20FC: CD BE 57    call $57BE
20FF: EB          ex   de,hl
2100: DD 6B       ld   ixl,e
2102: DD 62       ld   ixh,d
2104: C3 6F 50    jp   $506F
2107: DD 21 A8 24 ld   ix,$24A8
210B: C3 6F 50    jp   $506F
210E: CD AB 4C    call $4CAB
2111: C3 4C 50    jp   $504C
2114: C3 AB 4C    jp   $4CAB
2117: C3 4C 50    jp   $504C
211A: 3A 12 F8    ld   a,($F812)
211D: 32 14 F8    ld   ($F814),a
2120: C9          ret
2121: 3A 12 F8    ld   a,($F812)
2124: C6 20       add  a,$20
2126: CB 3F       srl  a
2128: 47          ld   b,a
2129: 3A 14 F8    ld   a,($F814)
212C: C3 82 4F    jp   $4F82
212F: 3A 12 F8    ld   a,($F812)
2132: 32 15 F8    ld   ($F815),a
2135: C9          ret
2136: 3E 01       ld   a,$01
2138: 32 08 F8    ld   ($F808),a
213B: 3A 12 F8    ld   a,($F812)
213E: C3 AF 4F    jp   $4FAF
2141: 3E 00       ld   a,$00
2143: 32 08 F8    ld   ($F808),a
2146: 3A 12 F8    ld   a,($F812)
2149: C3 AF 4F    jp   $4FAF
