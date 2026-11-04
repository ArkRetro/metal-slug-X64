2C93: 11 00 21    ld   de,$2100
2C96: 0E 04       ld   c,$04
2C98: CF          rst  $08
2C99: 11 01 28    ld   de,$2801
2C9C: CF          rst  $08
2C9D: 11 02 28    ld   de,$2802
2CA0: CF          rst  $08
2CA1: 11 05 28    ld   de,$2805
2CA4: CF          rst  $08
2CA5: 11 06 28    ld   de,$2806
2CA8: CF          rst  $08
2CA9: 11 7F 41    ld   de,$417F
2CAC: CF          rst  $08
2CAD: 11 7F 42    ld   de,$427F
2CB0: CF          rst  $08
2CB1: 11 7F 45    ld   de,$457F
2CB4: CF          rst  $08
2CB5: 11 7F 46    ld   de,$467F
2CB8: CF          rst  $08
2CB9: 11 7F 49    ld   de,$497F
2CBC: CF          rst  $08
2CBD: 11 7F 4A    ld   de,$4A7F
2CC0: CF          rst  $08
2CC1: 11 7F 4D    ld   de,$4D7F
2CC4: CF          rst  $08
2CC5: 11 7F 4E    ld   de,$4E7F
2CC8: CF          rst  $08
2CC9: 79          ld   a,c
2CCA: FE 04       cp   $04
2CCC: 20 05       jr   nz,$2CD3
2CCE: 0E 06       ld   c,$06
2CD0: C3 A9 2C    jp   $2CA9
2CD3: DD 21 07 F9 ld   ix,$F907
2CD7: DD 36 12 01 ld   (ix+$12),$01
2CDB: CD 07 2D    call $2D07
2CDE: DD 21 6F F9 ld   ix,$F96F
2CE2: DD 36 12 02 ld   (ix+$12),$02
2CE6: CD 07 2D    call $2D07
2CE9: DD 21 D7 F9 ld   ix,$F9D7
2CED: DD 36 12 05 ld   (ix+$12),$05
2CF1: CD 07 2D    call $2D07
2CF4: DD 21 3F FA ld   ix,$FA3F
2CF8: DD 36 12 06 ld   (ix+$12),$06
2CFC: CD 07 2D    call $2D07
2CFF: AF          xor  a
2D00: 32 A7 FA    ld   ($FAA7),a
2D03: 32 A8 FA    ld   ($FAA8),a
2D06: C9          ret
2D07: CD 96 1F    call $1F96
2D0A: DD 36 02 01 ld   (ix+$02),$01
2D0E: AF          xor  a
2D0F: DD 77 1B    ld   (ix+$1b),a
2D12: DD 77 24    ld   (ix+$24),a
2D15: DD 77 2D    ld   (ix+$2d),a
2D18: DD 77 36    ld   (ix+$36),a
2D1B: DD 77 38    ld   (ix+$38),a
2D1E: DD 77 3C    ld   (ix+$3c),a
2D21: DD 77 3D    ld   (ix+$3d),a
2D24: DD 77 3E    ld   (ix+$3e),a
2D27: 3D          dec  a
2D28: DD 77 39    ld   (ix+$39),a
2D2B: DD 77 3A    ld   (ix+$3a),a
2D2E: DD E5       push ix
2D30: 11 4D 00    ld   de,$004D
2D33: DD 19       add  ix,de
2D35: CD 4E 22    call $224E
2D38: DD E1       pop  ix
2D3A: DD E5       push ix
2D3C: 11 3F 00    ld   de,$003F
2D3F: DD 19       add  ix,de
2D41: CD 4E 22    call $224E
2D44: DD E1       pop  ix
2D46: DD E5       push ix
2D48: 11 5B 00    ld   de,$005B
2D4B: DD 19       add  ix,de
2D4D: CD 9F 23    call $239F
2D50: DD E1       pop  ix
2D52: C9          ret
2D53: 47          ld   b,a
2D54: 5E          ld   e,(hl)
2D55: CB 38       srl  b
2D57: DC 08 00    call c,$0008
2D5A: 78          ld   a,b
2D5B: A7          and  a
2D5C: C8          ret  z
2D5D: 7A          ld   a,d
2D5E: C6 10       add  a,$10
2D60: 57          ld   d,a
2D61: 23          inc  hl
2D62: 5E          ld   e,(hl)
2D63: CB 38       srl  b
2D65: DC 08 00    call c,$0008
2D68: 78          ld   a,b
2D69: A7          and  a
2D6A: C8          ret  z
2D6B: 7A          ld   a,d
2D6C: C6 10       add  a,$10
2D6E: 57          ld   d,a
2D6F: 23          inc  hl
2D70: 5E          ld   e,(hl)
2D71: CB 38       srl  b
2D73: DC 08 00    call c,$0008
2D76: 78          ld   a,b
2D77: A7          and  a
2D78: C8          ret  z
2D79: 7A          ld   a,d
2D7A: C6 10       add  a,$10
2D7C: 57          ld   d,a
2D7D: 23          inc  hl
2D7E: 5E          ld   e,(hl)
2D7F: CB 38       srl  b
2D81: DC 08 00    call c,$0008
2D84: 78          ld   a,b
2D85: A7          and  a
2D86: C8          ret  z
2D87: 7A          ld   a,d
2D88: C6 10       add  a,$10
2D8A: 57          ld   d,a
2D8B: 23          inc  hl
2D8C: 5E          ld   e,(hl)
2D8D: CB 38       srl  b
2D8F: DC 08 00    call c,$0008
2D92: 78          ld   a,b
2D93: A7          and  a
2D94: C8          ret  z
2D95: 7A          ld   a,d
2D96: C6 10       add  a,$10
2D98: 57          ld   d,a
2D99: 23          inc  hl
2D9A: 5E          ld   e,(hl)
2D9B: CB 38       srl  b
2D9D: DC 08 00    call c,$0008
2DA0: 78          ld   a,b
2DA1: A7          and  a
2DA2: C8          ret  z
2DA3: 7A          ld   a,d
2DA4: C6 10       add  a,$10
2DA6: 57          ld   d,a
2DA7: 23          inc  hl
2DA8: 5E          ld   e,(hl)
2DA9: CB 38       srl  b
2DAB: DA 08 00    jp   c,$0008
2DAE: C9          ret
2DAF: 21 A7 FA    ld   hl,$FAA7
2DB2: 5E          ld   e,(hl)
2DB3: 23          inc  hl
2DB4: 7E          ld   a,(hl)
2DB5: 73          ld   (hl),e
2DB6: BB          cp   e
2DB7: CA BF 2D    jp   z,$2DBF
2DBA: 16 22       ld   d,$22
2DBC: 0E 04       ld   c,$04
2DBE: CF          rst  $08
2DBF: DD 21 07 F9 ld   ix,$F907
2DC3: FD 2A 07 F9 ld   iy,($F907)
2DC7: FD CB 09 5E bit  3,(iy+$09)
2DCB: C4 CC 1F    call nz,$1FCC
2DCE: 3A 40 F9    ld   a,($F940)
2DD1: A7          and  a
2DD2: F2 E5 2D    jp   p,$2DE5
2DD5: 21 43 F9    ld   hl,$F943
2DD8: AF          xor  a
2DD9: 77          ld   (hl),a
2DDA: 5F          ld   e,a
2DDB: 16 B5       ld   d,$B5
2DDD: 0E 04       ld   c,$04
2DDF: CF          rst  $08
2DE0: 3E FF       ld   a,$FF
2DE2: 32 17 F9    ld   ($F917),a
2DE5: 3A 18 F9    ld   a,($F918)
2DE8: 2A 10 F9    ld   hl,($F910)
2DEB: 84          add  a,h
2DEC: 67          ld   h,a
2DED: FD 5E 10    ld   e,(iy+$10)
2DF0: FD 56 11    ld   d,(iy+$11)
2DF3: 19          add  hl,de
2DF4: 22 0F F8    ld   ($F80F),hl
2DF7: 21 54 F9    ld   hl,$F954
2DFA: FD 56 0C    ld   d,(iy+$0c)
2DFD: CD CF 22    call $22CF
2E00: 6F          ld   l,a
2E01: 26 00       ld   h,$00
2E03: 44          ld   b,h
2E04: 29          add  hl,hl
2E05: A7          and  a
2E06: ED 42       sbc  hl,bc
2E08: ED 5B 0F F8 ld   de,($F80F)
2E0C: 19          add  hl,de
2E0D: 22 0F F8    ld   ($F80F),hl
2E10: 21 62 F9    ld   hl,$F962
2E13: CD 3F 24    call $243F
2E16: ED 5B 0F F8 ld   de,($F80F)
2E1A: 19          add  hl,de
2E1B: ED 5B 15 F9 ld   de,($F915)
2E1F: A7          and  a
2E20: EB          ex   de,hl
2E21: ED 52       sbc  hl,de
2E23: CA 55 2E    jp   z,$2E55
2E26: ED 53 15 F9 ld   ($F915),de
2E2A: EB          ex   de,hl
2E2B: 7C          ld   a,h
2E2C: A7          and  a
2E2D: F2 32 2E    jp   p,$2E32
2E30: AF          xor  a
2E31: 6F          ld   l,a
2E32: 26 04       ld   h,$04
2E34: 56          ld   d,(hl)
2E35: 24          inc  h
2E36: 5E          ld   e,(hl)
2E37: EB          ex   de,hl
2E38: FE 08       cp   $08
2E3A: 38 0B       jr   c,$2E47
2E3C: 28 06       jr   z,$2E44
2E3E: 21 FF FF    ld   hl,$FFFF
2E41: C3 47 2E    jp   $2E47
2E44: 3E 07       ld   a,$07
2E46: 29          add  hl,hl
2E47: 87          add  a,a
2E48: 87          add  a,a
2E49: 87          add  a,a
2E4A: B4          or   h
2E4B: 5F          ld   e,a
2E4C: 0E 04       ld   c,$04
2E4E: 16 A5       ld   d,$A5
2E50: CF          rst  $08
2E51: 5D          ld   e,l
2E52: 16 A1       ld   d,$A1
2E54: CF          rst  $08
2E55: 21 46 F9    ld   hl,$F946
2E58: FD 56 0D    ld   d,(iy+$0d)
2E5B: CD CF 22    call $22CF
2E5E: 91          sub  c
2E5F: 3D          dec  a
2E60: 47          ld   b,a
2E61: 3A 40 F9    ld   a,($F940)
2E64: A7          and  a
2E65: F2 6A 2E    jp   p,$2E6A
2E68: 06 00       ld   b,$00
2E6A: 78          ld   a,b
2E6B: 6F          ld   l,a
2E6C: 3A 12 F9    ld   a,($F912)
2E6F: A7          and  a
2E70: 28 15       jr   z,$2E87
2E72: 2C          inc  l
2E73: 28 12       jr   z,$2E87
2E75: 08          ex   af,af'
2E76: 2D          dec  l
2E77: 7D          ld   a,l
2E78: 28 0D       jr   z,$2E87
2E7A: 26 06       ld   h,$06
2E7C: 7E          ld   a,(hl)
2E7D: 08          ex   af,af'
2E7E: 6F          ld   l,a
2E7F: 08          ex   af,af'
2E80: 86          add  a,(hl)
2E81: 6F          ld   l,a
2E82: 9F          sbc  a,a
2E83: 30 02       jr   nc,$2E87
2E85: 24          inc  h
2E86: 7E          ld   a,(hl)
2E87: FD 6E 12    ld   l,(iy+$12)
2E8A: A7          and  a
2E8B: 28 15       jr   z,$2EA2
2E8D: 2C          inc  l
2E8E: 28 12       jr   z,$2EA2
2E90: 08          ex   af,af'
2E91: 2D          dec  l
2E92: 7D          ld   a,l
2E93: 28 0D       jr   z,$2EA2
2E95: 26 06       ld   h,$06
2E97: 7E          ld   a,(hl)
2E98: 08          ex   af,af'
2E99: 6F          ld   l,a
2E9A: 08          ex   af,af'
2E9B: 86          add  a,(hl)
2E9C: 6F          ld   l,a
2E9D: 9F          sbc  a,a
2E9E: 30 02       jr   nc,$2EA2
2EA0: 24          inc  h
2EA1: 7E          ld   a,(hl)
2EA2: 6F          ld   l,a
2EA3: 3E FF       ld   a,$FF
2EA5: FD CB 09 4E bit  1,(iy+$09)
2EA9: CA AF 2E    jp   z,$2EAF
2EAC: 3A 05 F8    ld   a,($F805)
2EAF: A7          and  a
2EB0: 28 15       jr   z,$2EC7
2EB2: 2C          inc  l
2EB3: 28 12       jr   z,$2EC7
2EB5: 08          ex   af,af'
2EB6: 2D          dec  l
2EB7: 7D          ld   a,l
2EB8: 28 0D       jr   z,$2EC7
2EBA: 26 06       ld   h,$06
2EBC: 7E          ld   a,(hl)
2EBD: 08          ex   af,af'
2EBE: 6F          ld   l,a
2EBF: 08          ex   af,af'
2EC0: 86          add  a,(hl)
2EC1: 6F          ld   l,a
2EC2: 9F          sbc  a,a
2EC3: 30 02       jr   nc,$2EC7
2EC5: 24          inc  h
2EC6: 7E          ld   a,(hl)
2EC7: 21 17 F9    ld   hl,$F917
2ECA: BE          cp   (hl)
2ECB: 77          ld   (hl),a
2ECC: CA 36 2F    jp   z,$2F36
2ECF: 6F          ld   l,a
2ED0: CB FD       set  7,l
2ED2: 26 0A       ld   h,$0A
2ED4: 46          ld   b,(hl)
2ED5: DD 7E 39    ld   a,(ix+$39)
2ED8: E6 07       and  $07
2EDA: FE 04       cp   $04
2EDC: 38 44       jr   c,$2F22
2EDE: FE 05       cp   $05
2EE0: 38 2C       jr   c,$2F0E
2EE2: FE 07       cp   $07
2EE4: 38 14       jr   c,$2EFA
2EE6: 3A 23 F9    ld   a,($F923)
2EE9: 80          add  a,b
2EEA: 4F          ld   c,a
2EEB: 17          rla
2EEC: 9F          sbc  a,a
2EED: B1          or   c
2EEE: 21 1C F9    ld   hl,$F91C
2EF1: BE          cp   (hl)
2EF2: 28 06       jr   z,$2EFA
2EF4: 77          ld   (hl),a
2EF5: 21 22 F9    ld   hl,$F922
2EF8: CB CE       set  1,(hl)
2EFA: 3A 35 F9    ld   a,($F935)
2EFD: 80          add  a,b
2EFE: 4F          ld   c,a
2EFF: 17          rla
2F00: 9F          sbc  a,a
2F01: B1          or   c
2F02: 21 2E F9    ld   hl,$F92E
2F05: BE          cp   (hl)
2F06: 28 06       jr   z,$2F0E
2F08: 77          ld   (hl),a
2F09: 21 34 F9    ld   hl,$F934
2F0C: CB CE       set  1,(hl)
2F0E: 3A 2C F9    ld   a,($F92C)
2F11: 80          add  a,b
2F12: 4F          ld   c,a
2F13: 17          rla
2F14: 9F          sbc  a,a
2F15: B1          or   c
2F16: 21 25 F9    ld   hl,$F925
2F19: BE          cp   (hl)
2F1A: 28 06       jr   z,$2F22
2F1C: 77          ld   (hl),a
2F1D: 21 2B F9    ld   hl,$F92B
2F20: CB CE       set  1,(hl)
2F22: 3A 3E F9    ld   a,($F93E)
2F25: 80          add  a,b
2F26: 4F          ld   c,a
2F27: 17          rla
2F28: 9F          sbc  a,a
2F29: B1          or   c
2F2A: 21 37 F9    ld   hl,$F937
2F2D: BE          cp   (hl)
2F2E: 28 06       jr   z,$2F36
2F30: 77          ld   (hl),a
2F31: 21 3D F9    ld   hl,$F93D
2F34: CB CE       set  1,(hl)
2F36: 0E 04       ld   c,$04
2F38: 3A 22 F9    ld   a,($F922)
2F3B: A7          and  a
2F3C: 28 0C       jr   z,$2F4A
2F3E: 21 1B F9    ld   hl,$F91B
2F41: 16 31       ld   d,$31
2F43: CD 53 2D    call $2D53
2F46: AF          xor  a
2F47: 32 22 F9    ld   ($F922),a
2F4A: 3A 2B F9    ld   a,($F92B)
2F4D: A7          and  a
2F4E: 28 0C       jr   z,$2F5C
2F50: 21 24 F9    ld   hl,$F924
2F53: 16 39       ld   d,$39
2F55: CD 53 2D    call $2D53
2F58: AF          xor  a
2F59: 32 2B F9    ld   ($F92B),a
2F5C: 3A 34 F9    ld   a,($F934)
2F5F: A7          and  a
2F60: 28 0C       jr   z,$2F6E
2F62: 21 2D F9    ld   hl,$F92D
2F65: 16 35       ld   d,$35
2F67: CD 53 2D    call $2D53
2F6A: AF          xor  a
2F6B: 32 34 F9    ld   ($F934),a
2F6E: 3A 3D F9    ld   a,($F93D)
2F71: A7          and  a
2F72: 28 0C       jr   z,$2F80
2F74: 21 36 F9    ld   hl,$F936
2F77: 16 3D       ld   d,$3D
2F79: CD 53 2D    call $2D53
2F7C: AF          xor  a
2F7D: 32 3D F9    ld   ($F93D),a
2F80: 21 40 F9    ld   hl,$F940
2F83: 7E          ld   a,(hl)
2F84: E6 7F       and  $7F
2F86: 77          ld   (hl),a
2F87: 23          inc  hl
2F88: BE          cp   (hl)
2F89: CA 91 2F    jp   z,$2F91
2F8C: 77          ld   (hl),a
2F8D: 5F          ld   e,a
2F8E: 16 B1       ld   d,$B1
2F90: CF          rst  $08
2F91: 23          inc  hl
2F92: 7E          ld   a,(hl)
2F93: 23          inc  hl
2F94: BE          cp   (hl)
2F95: 28 07       jr   z,$2F9E
2F97: 77          ld   (hl),a
2F98: 5F          ld   e,a
2F99: 16 B5       ld   d,$B5
2F9B: 0E 04       ld   c,$04
2F9D: CF          rst  $08
2F9E: 23          inc  hl
2F9F: 7E          ld   a,(hl)
2FA0: 23          inc  hl
2FA1: BE          cp   (hl)
2FA2: CA AC 2F    jp   z,$2FAC
2FA5: 77          ld   (hl),a
2FA6: 0E 04       ld   c,$04
2FA8: 16 28       ld   d,$28
2FAA: 5F          ld   e,a
2FAB: CF          rst  $08
2FAC: 0E 01       ld   c,$01
2FAE: FD CB 09 4E bit  1,(iy+$09)
2FB2: 28 04       jr   z,$2FB8
2FB4: 3A 09 F8    ld   a,($F809)
2FB7: 4F          ld   c,a
2FB8: 21 0D F9    ld   hl,$F90D
2FBB: 7E          ld   a,(hl)
2FBC: 81          add  a,c
2FBD: DE 00       sbc  a,$00
2FBF: 77          ld   (hl),a
2FC0: 21 0C F9    ld   hl,$F90C
2FC3: 7E          ld   a,(hl)
2FC4: A7          and  a
2FC5: CA D9 2F    jp   z,$2FD9
2FC8: 91          sub  c
2FC9: 77          ld   (hl),a
2FCA: 20 22       jr   nz,$2FEE
2FCC: FD CB 09 66 bit  4,(iy+$09)
2FD0: 28 16       jr   z,$2FE8
2FD2: DD CB 03 E6 set  4,(ix+$03)
2FD6: C3 EE 2F    jp   $2FEE
2FD9: FD CB 09 66 bit  4,(iy+$09)
2FDD: 20 0F       jr   nz,$2FEE
2FDF: 21 0A F9    ld   hl,$F90A
2FE2: CB 66       bit  4,(hl)
2FE4: 28 08       jr   z,$2FEE
2FE6: CB A6       res  4,(hl)
2FE8: 3A 19 F9    ld   a,($F919)
2FEB: 32 44 F9    ld   ($F944),a
2FEE: DD 21 6F F9 ld   ix,$F96F
2FF2: FD 2A 6F F9 ld   iy,($F96F)
2FF6: FD CB 09 5E bit  3,(iy+$09)
2FFA: C4 CC 1F    call nz,$1FCC
2FFD: 3A A8 F9    ld   a,($F9A8)
3000: A7          and  a
3001: F2 14 30    jp   p,$3014
3004: 21 AB F9    ld   hl,$F9AB
3007: AF          xor  a
3008: 77          ld   (hl),a
3009: 5F          ld   e,a
300A: 16 B6       ld   d,$B6
300C: 0E 04       ld   c,$04
300E: CF          rst  $08
300F: 3E FF       ld   a,$FF
3011: 32 7F F9    ld   ($F97F),a
3014: 3A 80 F9    ld   a,($F980)
3017: 2A 78 F9    ld   hl,($F978)
301A: 84          add  a,h
301B: 67          ld   h,a
301C: FD 5E 10    ld   e,(iy+$10)
301F: FD 56 11    ld   d,(iy+$11)
3022: 19          add  hl,de
3023: 22 0F F8    ld   ($F80F),hl
3026: 21 BC F9    ld   hl,$F9BC
3029: FD 56 0C    ld   d,(iy+$0c)
302C: CD CF 22    call $22CF
302F: 6F          ld   l,a
3030: 26 00       ld   h,$00
3032: 44          ld   b,h
3033: 29          add  hl,hl
3034: A7          and  a
3035: ED 42       sbc  hl,bc
3037: ED 5B 0F F8 ld   de,($F80F)
303B: 19          add  hl,de
303C: 22 0F F8    ld   ($F80F),hl
303F: 21 CA F9    ld   hl,$F9CA
3042: CD 3F 24    call $243F
3045: ED 5B 0F F8 ld   de,($F80F)
3049: 19          add  hl,de
304A: ED 5B 7D F9 ld   de,($F97D)
304E: A7          and  a
304F: EB          ex   de,hl
3050: ED 52       sbc  hl,de
3052: CA 84 30    jp   z,$3084
3055: ED 53 7D F9 ld   ($F97D),de
3059: EB          ex   de,hl
305A: 7C          ld   a,h
305B: A7          and  a
305C: F2 61 30    jp   p,$3061
305F: AF          xor  a
3060: 6F          ld   l,a
3061: 26 04       ld   h,$04
3063: 56          ld   d,(hl)
3064: 24          inc  h
3065: 5E          ld   e,(hl)
3066: EB          ex   de,hl
3067: FE 08       cp   $08
3069: 38 0B       jr   c,$3076
306B: 28 06       jr   z,$3073
306D: 21 FF FF    ld   hl,$FFFF
3070: C3 76 30    jp   $3076
3073: 3E 07       ld   a,$07
3075: 29          add  hl,hl
3076: 87          add  a,a
3077: 87          add  a,a
3078: 87          add  a,a
3079: B4          or   h
307A: 5F          ld   e,a
307B: 0E 04       ld   c,$04
307D: 16 A6       ld   d,$A6
307F: CF          rst  $08
3080: 5D          ld   e,l
3081: 16 A2       ld   d,$A2
3083: CF          rst  $08
3084: 21 AE F9    ld   hl,$F9AE
3087: FD 56 0D    ld   d,(iy+$0d)
308A: CD CF 22    call $22CF
308D: 91          sub  c
308E: 3D          dec  a
308F: 47          ld   b,a
3090: 3A A8 F9    ld   a,($F9A8)
3093: A7          and  a
3094: F2 99 30    jp   p,$3099
3097: 06 00       ld   b,$00
3099: 78          ld   a,b
309A: 6F          ld   l,a
309B: 3A 7A F9    ld   a,($F97A)
309E: A7          and  a
309F: 28 15       jr   z,$30B6
30A1: 2C          inc  l
30A2: 28 12       jr   z,$30B6
30A4: 08          ex   af,af'
30A5: 2D          dec  l
30A6: 7D          ld   a,l
30A7: 28 0D       jr   z,$30B6
30A9: 26 06       ld   h,$06
30AB: 7E          ld   a,(hl)
30AC: 08          ex   af,af'
30AD: 6F          ld   l,a
30AE: 08          ex   af,af'
30AF: 86          add  a,(hl)
30B0: 6F          ld   l,a
30B1: 9F          sbc  a,a
30B2: 30 02       jr   nc,$30B6
30B4: 24          inc  h
30B5: 7E          ld   a,(hl)
30B6: FD 6E 12    ld   l,(iy+$12)
30B9: A7          and  a
30BA: 28 15       jr   z,$30D1
30BC: 2C          inc  l
30BD: 28 12       jr   z,$30D1
30BF: 08          ex   af,af'
30C0: 2D          dec  l
30C1: 7D          ld   a,l
30C2: 28 0D       jr   z,$30D1
30C4: 26 06       ld   h,$06
30C6: 7E          ld   a,(hl)
30C7: 08          ex   af,af'
30C8: 6F          ld   l,a
30C9: 08          ex   af,af'
30CA: 86          add  a,(hl)
30CB: 6F          ld   l,a
30CC: 9F          sbc  a,a
30CD: 30 02       jr   nc,$30D1
30CF: 24          inc  h
30D0: 7E          ld   a,(hl)
30D1: 6F          ld   l,a
30D2: 3E FF       ld   a,$FF
30D4: FD CB 09 4E bit  1,(iy+$09)
30D8: CA DE 30    jp   z,$30DE
30DB: 3A 05 F8    ld   a,($F805)
30DE: A7          and  a
30DF: 28 15       jr   z,$30F6
30E1: 2C          inc  l
30E2: 28 12       jr   z,$30F6
30E4: 08          ex   af,af'
30E5: 2D          dec  l
30E6: 7D          ld   a,l
30E7: 28 0D       jr   z,$30F6
30E9: 26 06       ld   h,$06
30EB: 7E          ld   a,(hl)
30EC: 08          ex   af,af'
30ED: 6F          ld   l,a
30EE: 08          ex   af,af'
30EF: 86          add  a,(hl)
30F0: 6F          ld   l,a
30F1: 9F          sbc  a,a
30F2: 30 02       jr   nc,$30F6
30F4: 24          inc  h
30F5: 7E          ld   a,(hl)
30F6: 21 7F F9    ld   hl,$F97F
30F9: BE          cp   (hl)
30FA: 77          ld   (hl),a
30FB: CA 65 31    jp   z,$3165
30FE: 6F          ld   l,a
30FF: CB FD       set  7,l
3101: 26 0A       ld   h,$0A
3103: 46          ld   b,(hl)
3104: DD 7E 39    ld   a,(ix+$39)
3107: E6 07       and  $07
3109: FE 04       cp   $04
310B: 38 44       jr   c,$3151
310D: FE 05       cp   $05
310F: 38 2C       jr   c,$313D
3111: FE 07       cp   $07
3113: 38 14       jr   c,$3129
3115: 3A 8B F9    ld   a,($F98B)
3118: 80          add  a,b
3119: 4F          ld   c,a
311A: 17          rla
311B: 9F          sbc  a,a
311C: B1          or   c
311D: 21 84 F9    ld   hl,$F984
3120: BE          cp   (hl)
3121: 28 06       jr   z,$3129
3123: 77          ld   (hl),a
3124: 21 8A F9    ld   hl,$F98A
3127: CB CE       set  1,(hl)
3129: 3A 9D F9    ld   a,($F99D)
312C: 80          add  a,b
312D: 4F          ld   c,a
312E: 17          rla
312F: 9F          sbc  a,a
3130: B1          or   c
3131: 21 96 F9    ld   hl,$F996
3134: BE          cp   (hl)
3135: 28 06       jr   z,$313D
3137: 77          ld   (hl),a
3138: 21 9C F9    ld   hl,$F99C
313B: CB CE       set  1,(hl)
313D: 3A 94 F9    ld   a,($F994)
3140: 80          add  a,b
3141: 4F          ld   c,a
3142: 17          rla
3143: 9F          sbc  a,a
3144: B1          or   c
3145: 21 8D F9    ld   hl,$F98D
3148: BE          cp   (hl)
3149: 28 06       jr   z,$3151
314B: 77          ld   (hl),a
314C: 21 93 F9    ld   hl,$F993
314F: CB CE       set  1,(hl)
3151: 3A A6 F9    ld   a,($F9A6)
3154: 80          add  a,b
3155: 4F          ld   c,a
3156: 17          rla
3157: 9F          sbc  a,a
3158: B1          or   c
3159: 21 9F F9    ld   hl,$F99F
315C: BE          cp   (hl)
315D: 28 06       jr   z,$3165
315F: 77          ld   (hl),a
3160: 21 A5 F9    ld   hl,$F9A5
3163: CB CE       set  1,(hl)
3165: 0E 04       ld   c,$04
3167: 3A 8A F9    ld   a,($F98A)
316A: A7          and  a
316B: 28 0C       jr   z,$3179
316D: 21 83 F9    ld   hl,$F983
3170: 16 32       ld   d,$32
3172: CD 53 2D    call $2D53
3175: AF          xor  a
3176: 32 8A F9    ld   ($F98A),a
3179: 3A 93 F9    ld   a,($F993)
317C: A7          and  a
317D: 28 0C       jr   z,$318B
317F: 21 8C F9    ld   hl,$F98C
3182: 16 3A       ld   d,$3A
3184: CD 53 2D    call $2D53
3187: AF          xor  a
3188: 32 93 F9    ld   ($F993),a
318B: 3A 9C F9    ld   a,($F99C)
318E: A7          and  a
318F: 28 0C       jr   z,$319D
3191: 21 95 F9    ld   hl,$F995
3194: 16 36       ld   d,$36
3196: CD 53 2D    call $2D53
3199: AF          xor  a
319A: 32 9C F9    ld   ($F99C),a
319D: 3A A5 F9    ld   a,($F9A5)
31A0: A7          and  a
31A1: 28 0C       jr   z,$31AF
31A3: 21 9E F9    ld   hl,$F99E
31A6: 16 3E       ld   d,$3E
31A8: CD 53 2D    call $2D53
31AB: AF          xor  a
31AC: 32 A5 F9    ld   ($F9A5),a
31AF: 21 A8 F9    ld   hl,$F9A8
31B2: 7E          ld   a,(hl)
31B3: E6 7F       and  $7F
31B5: 77          ld   (hl),a
31B6: 23          inc  hl
31B7: BE          cp   (hl)
31B8: CA C0 31    jp   z,$31C0
31BB: 77          ld   (hl),a
31BC: 5F          ld   e,a
31BD: 16 B2       ld   d,$B2
31BF: CF          rst  $08
31C0: 23          inc  hl
31C1: 7E          ld   a,(hl)
31C2: 23          inc  hl
31C3: BE          cp   (hl)
31C4: 28 07       jr   z,$31CD
31C6: 77          ld   (hl),a
31C7: 5F          ld   e,a
31C8: 16 B6       ld   d,$B6
31CA: 0E 04       ld   c,$04
31CC: CF          rst  $08
31CD: 23          inc  hl
31CE: 7E          ld   a,(hl)
31CF: 23          inc  hl
31D0: BE          cp   (hl)
31D1: CA DB 31    jp   z,$31DB
31D4: 77          ld   (hl),a
31D5: 0E 04       ld   c,$04
31D7: 16 28       ld   d,$28
31D9: 5F          ld   e,a
31DA: CF          rst  $08
31DB: 0E 01       ld   c,$01
31DD: FD CB 09 4E bit  1,(iy+$09)
31E1: 28 04       jr   z,$31E7
31E3: 3A 09 F8    ld   a,($F809)
31E6: 4F          ld   c,a
31E7: 21 75 F9    ld   hl,$F975
31EA: 7E          ld   a,(hl)
31EB: 81          add  a,c
31EC: DE 00       sbc  a,$00
31EE: 77          ld   (hl),a
31EF: 21 74 F9    ld   hl,$F974
31F2: 7E          ld   a,(hl)
31F3: A7          and  a
31F4: CA 08 32    jp   z,$3208
31F7: 91          sub  c
31F8: 77          ld   (hl),a
31F9: 20 22       jr   nz,$321D
31FB: FD CB 09 66 bit  4,(iy+$09)
31FF: 28 16       jr   z,$3217
3201: DD CB 03 E6 set  4,(ix+$03)
3205: C3 1D 32    jp   $321D
3208: FD CB 09 66 bit  4,(iy+$09)
320C: 20 0F       jr   nz,$321D
320E: 21 72 F9    ld   hl,$F972
3211: CB 66       bit  4,(hl)
3213: 28 08       jr   z,$321D
3215: CB A6       res  4,(hl)
3217: 3A 81 F9    ld   a,($F981)
321A: 32 AC F9    ld   ($F9AC),a
321D: DD 21 D7 F9 ld   ix,$F9D7
3221: FD 2A D7 F9 ld   iy,($F9D7)
3225: FD CB 09 5E bit  3,(iy+$09)
3229: C4 CC 1F    call nz,$1FCC
322C: 3A 10 FA    ld   a,($FA10)
322F: A7          and  a
3230: F2 43 32    jp   p,$3243
3233: 21 13 FA    ld   hl,$FA13
3236: AF          xor  a
3237: 77          ld   (hl),a
3238: 5F          ld   e,a
3239: 16 B5       ld   d,$B5
323B: 0E 06       ld   c,$06
323D: CF          rst  $08
323E: 3E FF       ld   a,$FF
3240: 32 E7 F9    ld   ($F9E7),a
3243: 3A E8 F9    ld   a,($F9E8)
3246: 2A E0 F9    ld   hl,($F9E0)
3249: 84          add  a,h
324A: 67          ld   h,a
324B: FD 5E 10    ld   e,(iy+$10)
324E: FD 56 11    ld   d,(iy+$11)
3251: 19          add  hl,de
3252: 22 0F F8    ld   ($F80F),hl
3255: 21 24 FA    ld   hl,$FA24
3258: FD 56 0C    ld   d,(iy+$0c)
325B: CD CF 22    call $22CF
325E: 6F          ld   l,a
325F: 26 00       ld   h,$00
3261: 44          ld   b,h
3262: 29          add  hl,hl
3263: A7          and  a
3264: ED 42       sbc  hl,bc
3266: ED 5B 0F F8 ld   de,($F80F)
326A: 19          add  hl,de
326B: 22 0F F8    ld   ($F80F),hl
326E: 21 32 FA    ld   hl,$FA32
3271: CD 3F 24    call $243F
3274: ED 5B 0F F8 ld   de,($F80F)
3278: 19          add  hl,de
3279: ED 5B E5 F9 ld   de,($F9E5)
327D: A7          and  a
327E: EB          ex   de,hl
327F: ED 52       sbc  hl,de
3281: CA B3 32    jp   z,$32B3
3284: ED 53 E5 F9 ld   ($F9E5),de
3288: EB          ex   de,hl
3289: 7C          ld   a,h
328A: A7          and  a
328B: F2 90 32    jp   p,$3290
328E: AF          xor  a
328F: 6F          ld   l,a
3290: 26 04       ld   h,$04
3292: 56          ld   d,(hl)
3293: 24          inc  h
3294: 5E          ld   e,(hl)
3295: EB          ex   de,hl
3296: FE 08       cp   $08
3298: 38 0B       jr   c,$32A5
329A: 28 06       jr   z,$32A2
329C: 21 FF FF    ld   hl,$FFFF
329F: C3 A5 32    jp   $32A5
32A2: 3E 07       ld   a,$07
32A4: 29          add  hl,hl
32A5: 87          add  a,a
32A6: 87          add  a,a
32A7: 87          add  a,a
32A8: B4          or   h
32A9: 5F          ld   e,a
32AA: 0E 06       ld   c,$06
32AC: 16 A5       ld   d,$A5
32AE: CF          rst  $08
32AF: 5D          ld   e,l
32B0: 16 A1       ld   d,$A1
32B2: CF          rst  $08
32B3: 21 16 FA    ld   hl,$FA16
32B6: FD 56 0D    ld   d,(iy+$0d)
32B9: CD CF 22    call $22CF
32BC: 91          sub  c
32BD: 3D          dec  a
32BE: 47          ld   b,a
32BF: 3A 10 FA    ld   a,($FA10)
32C2: A7          and  a
32C3: F2 C8 32    jp   p,$32C8
32C6: 06 00       ld   b,$00
32C8: 78          ld   a,b
32C9: 6F          ld   l,a
32CA: 3A E2 F9    ld   a,($F9E2)
32CD: A7          and  a
32CE: 28 15       jr   z,$32E5
32D0: 2C          inc  l
32D1: 28 12       jr   z,$32E5
32D3: 08          ex   af,af'
32D4: 2D          dec  l
32D5: 7D          ld   a,l
32D6: 28 0D       jr   z,$32E5
32D8: 26 06       ld   h,$06
32DA: 7E          ld   a,(hl)
32DB: 08          ex   af,af'
32DC: 6F          ld   l,a
32DD: 08          ex   af,af'
32DE: 86          add  a,(hl)
32DF: 6F          ld   l,a
32E0: 9F          sbc  a,a
32E1: 30 02       jr   nc,$32E5
32E3: 24          inc  h
32E4: 7E          ld   a,(hl)
32E5: FD 6E 12    ld   l,(iy+$12)
32E8: A7          and  a
32E9: 28 15       jr   z,$3300
32EB: 2C          inc  l
32EC: 28 12       jr   z,$3300
32EE: 08          ex   af,af'
32EF: 2D          dec  l
32F0: 7D          ld   a,l
32F1: 28 0D       jr   z,$3300
32F3: 26 06       ld   h,$06
32F5: 7E          ld   a,(hl)
32F6: 08          ex   af,af'
32F7: 6F          ld   l,a
32F8: 08          ex   af,af'
32F9: 86          add  a,(hl)
32FA: 6F          ld   l,a
32FB: 9F          sbc  a,a
32FC: 30 02       jr   nc,$3300
32FE: 24          inc  h
32FF: 7E          ld   a,(hl)
3300: 6F          ld   l,a
3301: 3E FF       ld   a,$FF
3303: FD CB 09 4E bit  1,(iy+$09)
3307: CA 0D 33    jp   z,$330D
330A: 3A 05 F8    ld   a,($F805)
330D: A7          and  a
330E: 28 15       jr   z,$3325
3310: 2C          inc  l
3311: 28 12       jr   z,$3325
3313: 08          ex   af,af'
3314: 2D          dec  l
3315: 7D          ld   a,l
3316: 28 0D       jr   z,$3325
3318: 26 06       ld   h,$06
331A: 7E          ld   a,(hl)
331B: 08          ex   af,af'
331C: 6F          ld   l,a
331D: 08          ex   af,af'
331E: 86          add  a,(hl)
331F: 6F          ld   l,a
3320: 9F          sbc  a,a
3321: 30 02       jr   nc,$3325
3323: 24          inc  h
3324: 7E          ld   a,(hl)
3325: 21 E7 F9    ld   hl,$F9E7
3328: BE          cp   (hl)
3329: 77          ld   (hl),a
332A: CA 94 33    jp   z,$3394
332D: 6F          ld   l,a
332E: CB FD       set  7,l
3330: 26 0A       ld   h,$0A
3332: 46          ld   b,(hl)
3333: DD 7E 39    ld   a,(ix+$39)
3336: E6 07       and  $07
3338: FE 04       cp   $04
333A: 38 44       jr   c,$3380
333C: FE 05       cp   $05
333E: 38 2C       jr   c,$336C
3340: FE 07       cp   $07
3342: 38 14       jr   c,$3358
3344: 3A F3 F9    ld   a,($F9F3)
3347: 80          add  a,b
3348: 4F          ld   c,a
3349: 17          rla
334A: 9F          sbc  a,a
334B: B1          or   c
334C: 21 EC F9    ld   hl,$F9EC
334F: BE          cp   (hl)
3350: 28 06       jr   z,$3358
3352: 77          ld   (hl),a
3353: 21 F2 F9    ld   hl,$F9F2
3356: CB CE       set  1,(hl)
3358: 3A 05 FA    ld   a,($FA05)
335B: 80          add  a,b
335C: 4F          ld   c,a
335D: 17          rla
335E: 9F          sbc  a,a
335F: B1          or   c
3360: 21 FE F9    ld   hl,$F9FE
3363: BE          cp   (hl)
3364: 28 06       jr   z,$336C
3366: 77          ld   (hl),a
3367: 21 04 FA    ld   hl,$FA04
336A: CB CE       set  1,(hl)
336C: 3A FC F9    ld   a,($F9FC)
336F: 80          add  a,b
3370: 4F          ld   c,a
3371: 17          rla
3372: 9F          sbc  a,a
3373: B1          or   c
3374: 21 F5 F9    ld   hl,$F9F5
3377: BE          cp   (hl)
3378: 28 06       jr   z,$3380
337A: 77          ld   (hl),a
337B: 21 FB F9    ld   hl,$F9FB
337E: CB CE       set  1,(hl)
3380: 3A 0E FA    ld   a,($FA0E)
3383: 80          add  a,b
3384: 4F          ld   c,a
3385: 17          rla
3386: 9F          sbc  a,a
3387: B1          or   c
3388: 21 07 FA    ld   hl,$FA07
338B: BE          cp   (hl)
338C: 28 06       jr   z,$3394
338E: 77          ld   (hl),a
338F: 21 0D FA    ld   hl,$FA0D
3392: CB CE       set  1,(hl)
3394: 0E 06       ld   c,$06
3396: 3A F2 F9    ld   a,($F9F2)
3399: A7          and  a
339A: 28 0C       jr   z,$33A8
339C: 21 EB F9    ld   hl,$F9EB
339F: 16 31       ld   d,$31
33A1: CD 53 2D    call $2D53
33A4: AF          xor  a
33A5: 32 F2 F9    ld   ($F9F2),a
33A8: 3A FB F9    ld   a,($F9FB)
33AB: A7          and  a
33AC: 28 0C       jr   z,$33BA
33AE: 21 F4 F9    ld   hl,$F9F4
33B1: 16 39       ld   d,$39
33B3: CD 53 2D    call $2D53
33B6: AF          xor  a
33B7: 32 FB F9    ld   ($F9FB),a
33BA: 3A 04 FA    ld   a,($FA04)
33BD: A7          and  a
33BE: 28 0C       jr   z,$33CC
33C0: 21 FD F9    ld   hl,$F9FD
33C3: 16 35       ld   d,$35
33C5: CD 53 2D    call $2D53
33C8: AF          xor  a
33C9: 32 04 FA    ld   ($FA04),a
33CC: 3A 0D FA    ld   a,($FA0D)
33CF: A7          and  a
33D0: 28 0C       jr   z,$33DE
33D2: 21 06 FA    ld   hl,$FA06
33D5: 16 3D       ld   d,$3D
33D7: CD 53 2D    call $2D53
33DA: AF          xor  a
33DB: 32 0D FA    ld   ($FA0D),a
33DE: 21 10 FA    ld   hl,$FA10
33E1: 7E          ld   a,(hl)
33E2: E6 7F       and  $7F
33E4: 77          ld   (hl),a
33E5: 23          inc  hl
33E6: BE          cp   (hl)
33E7: CA EF 33    jp   z,$33EF
33EA: 77          ld   (hl),a
33EB: 5F          ld   e,a
33EC: 16 B1       ld   d,$B1
33EE: CF          rst  $08
33EF: 23          inc  hl
33F0: 7E          ld   a,(hl)
33F1: 23          inc  hl
33F2: BE          cp   (hl)
33F3: 28 07       jr   z,$33FC
33F5: 77          ld   (hl),a
33F6: 5F          ld   e,a
33F7: 16 B5       ld   d,$B5
33F9: 0E 06       ld   c,$06
33FB: CF          rst  $08
33FC: 23          inc  hl
33FD: 7E          ld   a,(hl)
33FE: 23          inc  hl
33FF: BE          cp   (hl)
3400: CA 0A 34    jp   z,$340A
3403: 77          ld   (hl),a
3404: 0E 04       ld   c,$04
3406: 16 28       ld   d,$28
3408: 5F          ld   e,a
3409: CF          rst  $08
340A: 0E 01       ld   c,$01
340C: FD CB 09 4E bit  1,(iy+$09)
3410: 28 04       jr   z,$3416
3412: 3A 09 F8    ld   a,($F809)
3415: 4F          ld   c,a
3416: 21 DD F9    ld   hl,$F9DD
3419: 7E          ld   a,(hl)
341A: 81          add  a,c
341B: DE 00       sbc  a,$00
341D: 77          ld   (hl),a
341E: 21 DC F9    ld   hl,$F9DC
3421: 7E          ld   a,(hl)
3422: A7          and  a
3423: CA 37 34    jp   z,$3437
3426: 91          sub  c
3427: 77          ld   (hl),a
3428: 20 22       jr   nz,$344C
342A: FD CB 09 66 bit  4,(iy+$09)
342E: 28 16       jr   z,$3446
3430: DD CB 03 E6 set  4,(ix+$03)
3434: C3 4C 34    jp   $344C
3437: FD CB 09 66 bit  4,(iy+$09)
343B: 20 0F       jr   nz,$344C
343D: 21 DA F9    ld   hl,$F9DA
3440: CB 66       bit  4,(hl)
3442: 28 08       jr   z,$344C
3444: CB A6       res  4,(hl)
3446: 3A E9 F9    ld   a,($F9E9)
3449: 32 14 FA    ld   ($FA14),a
344C: DD 21 3F FA ld   ix,$FA3F
3450: FD 2A 3F FA ld   iy,($FA3F)
3454: FD CB 09 5E bit  3,(iy+$09)
3458: C4 CC 1F    call nz,$1FCC
345B: 3A 78 FA    ld   a,($FA78)
345E: A7          and  a
345F: F2 72 34    jp   p,$3472
3462: 21 7B FA    ld   hl,$FA7B
3465: AF          xor  a
3466: 77          ld   (hl),a
3467: 5F          ld   e,a
3468: 16 B6       ld   d,$B6
346A: 0E 06       ld   c,$06
346C: CF          rst  $08
346D: 3E FF       ld   a,$FF
346F: 32 4F FA    ld   ($FA4F),a
3472: 3A 50 FA    ld   a,($FA50)
3475: 2A 48 FA    ld   hl,($FA48)
3478: 84          add  a,h
3479: 67          ld   h,a
347A: FD 5E 10    ld   e,(iy+$10)
347D: FD 56 11    ld   d,(iy+$11)
3480: 19          add  hl,de
3481: 22 0F F8    ld   ($F80F),hl
3484: 21 8C FA    ld   hl,$FA8C
3487: FD 56 0C    ld   d,(iy+$0c)
348A: CD CF 22    call $22CF
348D: 6F          ld   l,a
348E: 26 00       ld   h,$00
3490: 44          ld   b,h
3491: 29          add  hl,hl
3492: A7          and  a
3493: ED 42       sbc  hl,bc
3495: ED 5B 0F F8 ld   de,($F80F)
3499: 19          add  hl,de
349A: 22 0F F8    ld   ($F80F),hl
349D: 21 9A FA    ld   hl,$FA9A
34A0: CD 3F 24    call $243F
34A3: ED 5B 0F F8 ld   de,($F80F)
34A7: 19          add  hl,de
34A8: ED 5B 4D FA ld   de,($FA4D)
34AC: A7          and  a
34AD: EB          ex   de,hl
34AE: ED 52       sbc  hl,de
34B0: CA E2 34    jp   z,$34E2
34B3: ED 53 4D FA ld   ($FA4D),de
34B7: EB          ex   de,hl
34B8: 7C          ld   a,h
34B9: A7          and  a
34BA: F2 BF 34    jp   p,$34BF
34BD: AF          xor  a
34BE: 6F          ld   l,a
34BF: 26 04       ld   h,$04
34C1: 56          ld   d,(hl)
34C2: 24          inc  h
34C3: 5E          ld   e,(hl)
34C4: EB          ex   de,hl
34C5: FE 08       cp   $08
34C7: 38 0B       jr   c,$34D4
34C9: 28 06       jr   z,$34D1
34CB: 21 FF FF    ld   hl,$FFFF
34CE: C3 D4 34    jp   $34D4
34D1: 3E 07       ld   a,$07
34D3: 29          add  hl,hl
34D4: 87          add  a,a
34D5: 87          add  a,a
34D6: 87          add  a,a
34D7: B4          or   h
34D8: 5F          ld   e,a
34D9: 0E 06       ld   c,$06
34DB: 16 A6       ld   d,$A6
34DD: CF          rst  $08
34DE: 5D          ld   e,l
34DF: 16 A2       ld   d,$A2
34E1: CF          rst  $08
34E2: 21 7E FA    ld   hl,$FA7E
34E5: FD 56 0D    ld   d,(iy+$0d)
34E8: CD CF 22    call $22CF
34EB: 91          sub  c
34EC: 3D          dec  a
34ED: 47          ld   b,a
34EE: 3A 78 FA    ld   a,($FA78)
34F1: A7          and  a
34F2: F2 F7 34    jp   p,$34F7
34F5: 06 00       ld   b,$00
34F7: 78          ld   a,b
34F8: 6F          ld   l,a
34F9: 3A 4A FA    ld   a,($FA4A)
34FC: A7          and  a
34FD: 28 15       jr   z,$3514
34FF: 2C          inc  l
3500: 28 12       jr   z,$3514
3502: 08          ex   af,af'
3503: 2D          dec  l
3504: 7D          ld   a,l
3505: 28 0D       jr   z,$3514
3507: 26 06       ld   h,$06
3509: 7E          ld   a,(hl)
350A: 08          ex   af,af'
350B: 6F          ld   l,a
350C: 08          ex   af,af'
350D: 86          add  a,(hl)
350E: 6F          ld   l,a
350F: 9F          sbc  a,a
3510: 30 02       jr   nc,$3514
3512: 24          inc  h
3513: 7E          ld   a,(hl)
3514: FD 6E 12    ld   l,(iy+$12)
3517: A7          and  a
3518: 28 15       jr   z,$352F
351A: 2C          inc  l
351B: 28 12       jr   z,$352F
351D: 08          ex   af,af'
351E: 2D          dec  l
351F: 7D          ld   a,l
3520: 28 0D       jr   z,$352F
3522: 26 06       ld   h,$06
3524: 7E          ld   a,(hl)
3525: 08          ex   af,af'
3526: 6F          ld   l,a
3527: 08          ex   af,af'
3528: 86          add  a,(hl)
3529: 6F          ld   l,a
352A: 9F          sbc  a,a
352B: 30 02       jr   nc,$352F
352D: 24          inc  h
352E: 7E          ld   a,(hl)
352F: 6F          ld   l,a
3530: 3E FF       ld   a,$FF
3532: FD CB 09 4E bit  1,(iy+$09)
3536: CA 3C 35    jp   z,$353C
3539: 3A 05 F8    ld   a,($F805)
353C: A7          and  a
353D: 28 15       jr   z,$3554
353F: 2C          inc  l
3540: 28 12       jr   z,$3554
3542: 08          ex   af,af'
3543: 2D          dec  l
3544: 7D          ld   a,l
3545: 28 0D       jr   z,$3554
3547: 26 06       ld   h,$06
3549: 7E          ld   a,(hl)
354A: 08          ex   af,af'
354B: 6F          ld   l,a
354C: 08          ex   af,af'
354D: 86          add  a,(hl)
354E: 6F          ld   l,a
354F: 9F          sbc  a,a
3550: 30 02       jr   nc,$3554
3552: 24          inc  h
3553: 7E          ld   a,(hl)
3554: 21 4F FA    ld   hl,$FA4F
3557: BE          cp   (hl)
3558: 77          ld   (hl),a
3559: CA C3 35    jp   z,$35C3
355C: 6F          ld   l,a
355D: CB FD       set  7,l
355F: 26 0A       ld   h,$0A
3561: 46          ld   b,(hl)
3562: DD 7E 39    ld   a,(ix+$39)
3565: E6 07       and  $07
3567: FE 04       cp   $04
3569: 38 44       jr   c,$35AF
356B: FE 05       cp   $05
356D: 38 2C       jr   c,$359B
356F: FE 07       cp   $07
3571: 38 14       jr   c,$3587
3573: 3A 5B FA    ld   a,($FA5B)
3576: 80          add  a,b
3577: 4F          ld   c,a
3578: 17          rla
3579: 9F          sbc  a,a
357A: B1          or   c
357B: 21 54 FA    ld   hl,$FA54
357E: BE          cp   (hl)
357F: 28 06       jr   z,$3587
3581: 77          ld   (hl),a
3582: 21 5A FA    ld   hl,$FA5A
3585: CB CE       set  1,(hl)
3587: 3A 6D FA    ld   a,($FA6D)
358A: 80          add  a,b
358B: 4F          ld   c,a
358C: 17          rla
358D: 9F          sbc  a,a
358E: B1          or   c
358F: 21 66 FA    ld   hl,$FA66
3592: BE          cp   (hl)
3593: 28 06       jr   z,$359B
3595: 77          ld   (hl),a
3596: 21 6C FA    ld   hl,$FA6C
3599: CB CE       set  1,(hl)
359B: 3A 64 FA    ld   a,($FA64)
359E: 80          add  a,b
359F: 4F          ld   c,a
35A0: 17          rla
35A1: 9F          sbc  a,a
35A2: B1          or   c
35A3: 21 5D FA    ld   hl,$FA5D
35A6: BE          cp   (hl)
35A7: 28 06       jr   z,$35AF
35A9: 77          ld   (hl),a
35AA: 21 63 FA    ld   hl,$FA63
35AD: CB CE       set  1,(hl)
35AF: 3A 76 FA    ld   a,($FA76)
35B2: 80          add  a,b
35B3: 4F          ld   c,a
35B4: 17          rla
35B5: 9F          sbc  a,a
35B6: B1          or   c
35B7: 21 6F FA    ld   hl,$FA6F
35BA: BE          cp   (hl)
35BB: 28 06       jr   z,$35C3
35BD: 77          ld   (hl),a
35BE: 21 75 FA    ld   hl,$FA75
35C1: CB CE       set  1,(hl)
35C3: 0E 06       ld   c,$06
35C5: 3A 5A FA    ld   a,($FA5A)
35C8: A7          and  a
35C9: 28 0C       jr   z,$35D7
35CB: 21 53 FA    ld   hl,$FA53
35CE: 16 32       ld   d,$32
35D0: CD 53 2D    call $2D53
35D3: AF          xor  a
35D4: 32 5A FA    ld   ($FA5A),a
35D7: 3A 63 FA    ld   a,($FA63)
35DA: A7          and  a
35DB: 28 0C       jr   z,$35E9
35DD: 21 5C FA    ld   hl,$FA5C
35E0: 16 3A       ld   d,$3A
35E2: CD 53 2D    call $2D53
35E5: AF          xor  a
35E6: 32 63 FA    ld   ($FA63),a
35E9: 3A 6C FA    ld   a,($FA6C)
35EC: A7          and  a
35ED: 28 0C       jr   z,$35FB
35EF: 21 65 FA    ld   hl,$FA65
35F2: 16 36       ld   d,$36
35F4: CD 53 2D    call $2D53
35F7: AF          xor  a
35F8: 32 6C FA    ld   ($FA6C),a
35FB: 3A 75 FA    ld   a,($FA75)
35FE: A7          and  a
35FF: 28 0C       jr   z,$360D
3601: 21 6E FA    ld   hl,$FA6E
3604: 16 3E       ld   d,$3E
3606: CD 53 2D    call $2D53
3609: AF          xor  a
360A: 32 75 FA    ld   ($FA75),a
360D: 21 78 FA    ld   hl,$FA78
3610: 7E          ld   a,(hl)
3611: E6 7F       and  $7F
3613: 77          ld   (hl),a
3614: 23          inc  hl
3615: BE          cp   (hl)
3616: CA 1E 36    jp   z,$361E
3619: 77          ld   (hl),a
361A: 5F          ld   e,a
361B: 16 B2       ld   d,$B2
361D: CF          rst  $08
361E: 23          inc  hl
361F: 7E          ld   a,(hl)
3620: 23          inc  hl
3621: BE          cp   (hl)
3622: 28 07       jr   z,$362B
3624: 77          ld   (hl),a
3625: 5F          ld   e,a
3626: 16 B6       ld   d,$B6
3628: 0E 06       ld   c,$06
362A: CF          rst  $08
362B: 23          inc  hl
362C: 7E          ld   a,(hl)
362D: 23          inc  hl
362E: BE          cp   (hl)
362F: CA 39 36    jp   z,$3639
3632: 77          ld   (hl),a
3633: 0E 04       ld   c,$04
3635: 16 28       ld   d,$28
3637: 5F          ld   e,a
3638: CF          rst  $08
3639: 0E 01       ld   c,$01
363B: FD CB 09 4E bit  1,(iy+$09)
363F: 28 04       jr   z,$3645
3641: 3A 09 F8    ld   a,($F809)
3644: 4F          ld   c,a
3645: 21 45 FA    ld   hl,$FA45
3648: 7E          ld   a,(hl)
3649: 81          add  a,c
364A: DE 00       sbc  a,$00
364C: 77          ld   (hl),a
364D: 21 44 FA    ld   hl,$FA44
3650: 7E          ld   a,(hl)
3651: A7          and  a
3652: CA 66 36    jp   z,$3666
3655: 91          sub  c
3656: 77          ld   (hl),a
3657: 20 22       jr   nz,$367B
3659: FD CB 09 66 bit  4,(iy+$09)
365D: 28 16       jr   z,$3675
365F: DD CB 03 E6 set  4,(ix+$03)
3663: C3 7B 36    jp   $367B
3666: FD CB 09 66 bit  4,(iy+$09)
366A: 20 0F       jr   nz,$367B
366C: 21 42 FA    ld   hl,$FA42
366F: CB 66       bit  4,(hl)
3671: 28 08       jr   z,$367B
3673: CB A6       res  4,(hl)
3675: 3A 51 FA    ld   a,($FA51)
3678: 32 7C FA    ld   ($FA7C),a
367B: C9          ret
