1E00: AF          xor  a
1E01: D3 0C       out  ($0C),a
1E03: 31 EE FF    ld   sp,$FFEE
1E06: 01 04 00    ld   bc,$0004
1E09: ED 43 0B F8 ld   ($F80B),bc
1E0D: CD 95 57    call $5795
1E10: CD B9 1E    call $1EB9
1E13: CD F0 1F    call $1FF0
1E16: CD C6 24    call $24C6
1E19: CD 93 2C    call $2C93
1E1C: CD 6B 38    call $386B
1E1F: CD 5A 40    call $405A
1E22: CD AB 4C    call $4CAB
1E25: CD 4C 50    call $504C
1E28: AF          xor  a
1E29: 32 01 F8    ld   ($F801),a
1E2C: 32 0A F8    ld   ($F80A),a
1E2F: 32 02 F8    ld   ($F802),a
1E32: 3E 01       ld   a,$01
1E34: 32 03 F8    ld   ($F803),a
1E37: CD 71 1E    call $1E71
1E3A: ED 56       im   1
1E3C: D3 08       out  ($08),a
1E3E: FB          ei
1E3F: CD 6A 1E    call $1E6A
1E42: CD 01 20    call $2001
1E45: CD 48 25    call $2548
1E48: CD AF 2D    call $2DAF
1E4B: CD C7 38    call $38C7
1E4E: CD B4 40    call $40B4
1E51: CD 43 51    call $5143
1E54: CD CD 4C    call $4CCD
1E57: 21 01 F8    ld   hl,$F801
1E5A: 7E          ld   a,(hl)
1E5B: A7          and  a
1E5C: CA 5A 1E    jp   z,$1E5A
1E5F: 3D          dec  a
1E60: 77          ld   (hl),a
1E61: 23          inc  hl
1E62: 7E          ld   a,(hl)
1E63: 36 00       ld   (hl),$00
1E65: 23          inc  hl
1E66: 77          ld   (hl),a
1E67: C3 3F 1E    jp   $1E3F
1E6A: 21 00 F8    ld   hl,$F800
1E6D: 7E          ld   a,(hl)
1E6E: 3C          inc  a
1E6F: 77          ld   (hl),a
1E70: C9          ret
1E71: 11 AC 24    ld   de,$24AC
1E74: 0E 04       ld   c,$04
1E76: CF          rst  $08
1E77: 11 03 25    ld   de,$2503
1E7A: CF          rst  $08
1E7B: 11 35 27    ld   de,$2735
1E7E: CF          rst  $08
1E7F: C9          ret
1E80: F5          push af
1E81: C5          push bc
1E82: D5          push de
1E83: E5          push hl
1E84: DD E5       push ix
1E86: FD E5       push iy
1E88: ED 4B 0B F8 ld   bc,($F80B)
1E8C: C5          push bc
1E8D: 0E 04       ld   c,$04
1E8F: 11 35 27    ld   de,$2735
1E92: CF          rst  $08
1E93: 21 01 F8    ld   hl,$F801
1E96: 7E          ld   a,(hl)
1E97: FE 02       cp   $02
1E99: 30 02       jr   nc,$1E9D
1E9B: 3C          inc  a
1E9C: 77          ld   (hl),a
1E9D: 23          inc  hl
1E9E: 34          inc  (hl)
1E9F: DB 06       in   a,($06)
1EA1: 32 0A F8    ld   ($F80A),a
1EA4: 0E 04       ld   c,$04
1EA6: ED 78       in   a,(c)
1EA8: FA A6 1E    jp   m,$1EA6
1EAB: C1          pop  bc
1EAC: ED 41       out  (c),b
1EAE: FD E1       pop  iy
1EB0: DD E1       pop  ix
1EB2: E1          pop  hl
1EB3: D1          pop  de
1EB4: C1          pop  bc
1EB5: F1          pop  af
1EB6: FB          ei
1EB7: ED 4D       reti
1EB9: AF          xor  a
1EBA: 32 EE FF    ld   ($FFEE),a
1EBD: 32 EF FF    ld   ($FFEF),a
1EC0: C9          ret
1EC1: 21 EF FF    ld   hl,$FFEF
1EC4: 4E          ld   c,(hl)
1EC5: 2B          dec  hl
1EC6: 7E          ld   a,(hl)
1EC7: B9          cp   c
1EC8: 28 10       jr   z,$1EDA
1ECA: E6 0F       and  $0F
1ECC: 5F          ld   e,a
1ECD: 16 00       ld   d,$00
1ECF: 3C          inc  a
1ED0: E6 0F       and  $0F
1ED2: 77          ld   (hl),a
1ED3: 21 F0 FF    ld   hl,$FFF0
1ED6: 19          add  hl,de
1ED7: 7E          ld   a,(hl)
1ED8: 37          scf
1ED9: C9          ret
1EDA: A7          and  a
1EDB: C9          ret
1EDC: F5          push af
1EDD: C5          push bc
1EDE: D5          push de
1EDF: E5          push hl
1EE0: DD E5       push ix
1EE2: FD E5       push iy
1EE4: DB 00       in   a,($00)
1EE6: FE 01       cp   $01
1EE8: CA 10 1F    jp   z,$1F10
1EEB: FE 03       cp   $03
1EED: CA 61 1F    jp   z,$1F61
1EF0: 4F          ld   c,a
1EF1: 3A EF FF    ld   a,($FFEF)
1EF4: E6 0F       and  $0F
1EF6: 21 F0 FF    ld   hl,$FFF0
1EF9: 5F          ld   e,a
1EFA: 16 00       ld   d,$00
1EFC: 19          add  hl,de
1EFD: 71          ld   (hl),c
1EFE: 3C          inc  a
1EFF: E6 0F       and  $0F
1F01: 32 EF FF    ld   ($FFEF),a
1F04: FD E1       pop  iy
1F06: DD E1       pop  ix
1F08: E1          pop  hl
1F09: D1          pop  de
1F0A: C1          pop  bc
1F0B: F1          pop  af
1F0C: D3 08       out  ($08),a
1F0E: ED 45       retn
1F10: F3          di
1F11: 31 EE FF    ld   sp,$FFEE
1F14: 0E 04       ld   c,$04
1F16: 11 FF 07    ld   de,$07FF
1F19: CF          rst  $08
1F1A: 11 00 11    ld   de,$1100
1F1D: CF          rst  $08
1F1E: 11 01 10    ld   de,$1001
1F21: CF          rst  $08
1F22: 11 00 B5    ld   de,$B500
1F25: CF          rst  $08
1F26: 14          inc  d
1F27: CF          rst  $08
1F28: 0C          inc  c
1F29: 0C          inc  c
1F2A: CF          rst  $08
1F2B: 15          dec  d
1F2C: CF          rst  $08
1F2D: 11 FF 00    ld   de,$00FF
1F30: CF          rst  $08
1F31: 11 00 08    ld   de,$0800
1F34: CF          rst  $08
1F35: 14          inc  d
1F36: CF          rst  $08
1F37: 14          inc  d
1F38: CF          rst  $08
1F39: 14          inc  d
1F3A: CF          rst  $08
1F3B: 14          inc  d
1F3C: CF          rst  $08
1F3D: 14          inc  d
1F3E: CF          rst  $08
1F3F: 21 00 F8    ld   hl,$F800
1F42: 01 80 08    ld   bc,$0880
1F45: AF          xor  a
1F46: 77          ld   (hl),a
1F47: 23          inc  hl
1F48: 0D          dec  c
1F49: 20 FB       jr   nz,$1F46
1F4B: 05          dec  b
1F4C: 20 F8       jr   nz,$1F46
1F4E: F9          ld   sp,hl
1F4F: E5          push hl
1F50: 36 D3       ld   (hl),$D3
1F52: 23          inc  hl
1F53: 36 0C       ld   (hl),$0C
1F55: 23          inc  hl
1F56: 36 18       ld   (hl),$18
1F58: 23          inc  hl
1F59: 36 FE       ld   (hl),$FE
1F5B: 3E 01       ld   a,$01
1F5D: D3 08       out  ($08),a
1F5F: ED 45       retn
1F61: F3          di
1F62: 31 EE FF    ld   sp,$FFEE
1F65: 21 00 00    ld   hl,$0000
1F68: E5          push hl
1F69: D3 08       out  ($08),a
1F6B: ED 45       retn
1F6D: 21 00 00    ld   hl,$0000
1F70: 79          ld   a,c
1F71: A7          and  a
1F72: C8          ret  z
1F73: 26 06       ld   h,$06
1F75: 69          ld   l,c
1F76: 7E          ld   a,(hl)
1F77: 6B          ld   l,e
1F78: 1E 00       ld   e,$00
1F7A: 86          add  a,(hl)
1F7B: 30 04       jr   nc,$1F81
1F7D: 6F          ld   l,a
1F7E: 24          inc  h
1F7F: 5E          ld   e,(hl)
1F80: 25          dec  h
1F81: 69          ld   l,c
1F82: 7E          ld   a,(hl)
1F83: 6A          ld   l,d
1F84: 16 00       ld   d,$00
1F86: 86          add  a,(hl)
1F87: 6F          ld   l,a
1F88: 24          inc  h
1F89: 38 05       jr   c,$1F90
1F8B: 6E          ld   l,(hl)
1F8C: 26 00       ld   h,$00
1F8E: 19          add  hl,de
1F8F: C9          ret
1F90: 7E          ld   a,(hl)
1F91: 24          inc  h
1F92: 6E          ld   l,(hl)
1F93: 67          ld   h,a
1F94: 19          add  hl,de
1F95: C9          ret
1F96: 01 40 00    ld   bc,$0040
1F99: DD 71 00    ld   (ix+$00),c
1F9C: DD 70 01    ld   (ix+$01),b
1F9F: AF          xor  a
1FA0: DD 77 0B    ld   (ix+$0b),a
1FA3: DD 77 05    ld   (ix+$05),a
1FA6: DD 77 07    ld   (ix+$07),a
1FA9: DD 77 10    ld   (ix+$10),a
1FAC: DD 77 08    ld   (ix+$08),a
1FAF: DD 77 09    ld   (ix+$09),a
1FB2: DD 77 0A    ld   (ix+$0a),a
1FB5: 3D          dec  a
1FB6: DD 77 03    ld   (ix+$03),a
1FB9: DD 77 04    ld   (ix+$04),a
1FBC: DD 77 06    ld   (ix+$06),a
1FBF: DD 77 0E    ld   (ix+$0e),a
1FC2: DD 77 0F    ld   (ix+$0f),a
1FC5: DD 77 0C    ld   (ix+$0c),a
1FC8: DD 77 0D    ld   (ix+$0d),a
1FCB: C9          ret
1FCC: AF          xor  a
1FCD: DD 77 0B    ld   (ix+$0b),a
1FD0: DD 77 05    ld   (ix+$05),a
1FD3: DD 77 07    ld   (ix+$07),a
1FD6: 3D          dec  a
1FD7: DD 77 03    ld   (ix+$03),a
1FDA: DD 77 04    ld   (ix+$04),a
1FDD: DD 77 06    ld   (ix+$06),a
1FE0: DD 77 0E    ld   (ix+$0e),a
1FE3: DD 77 0F    ld   (ix+$0f),a
1FE6: 21 40 00    ld   hl,$0040
1FE9: DD 75 00    ld   (ix+$00),l
1FEC: DD 74 01    ld   (ix+$01),h
1FEF: C9          ret
1FF0: AF          xor  a
1FF1: 32 13 F8    ld   ($F813),a
1FF4: 32 18 F8    ld   ($F818),a
1FF7: 32 19 F8    ld   ($F819),a
1FFA: 32 14 F8    ld   ($F814),a
1FFD: 32 15 F8    ld   ($F815),a
2000: C9          ret
2001: CD C1 1E    call $1EC1
2004: D0          ret  nc
2005: 2A 16 F8    ld   hl,($F816)
2008: 23          inc  hl
2009: 22 16 F8    ld   ($F816),hl
200C: FE 20       cp   $20
200E: 30 37       jr   nc,$2047
2010: FE 0F       cp   $0F
2012: 28 03       jr   z,$2017
2014: 32 19 F8    ld   ($F819),a
2017: FE 10       cp   $10
2019: D0          ret  nc
201A: 87          add  a,a
201B: 5F          ld   e,a
201C: 16 00       ld   d,$00
201E: 21 27 20    ld   hl,$2027
2021: 19          add  hl,de
2022: 7E          ld   a,(hl)
2023: 23          inc  hl
2024: 66          ld   h,(hl)
2025: 6F          ld   l,a
2026: E9          jp   (hl)
