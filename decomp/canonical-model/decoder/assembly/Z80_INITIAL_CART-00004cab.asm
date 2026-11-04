4CAB: 21 00 00    ld   hl,$0000
4CAE: 22 F4 FD    ld   ($FDF4),hl
4CB1: FD 21 00 FC ld   iy,$FC00
4CB5: 06 0E       ld   b,$0E
4CB7: 11 23 00    ld   de,$0023
4CBA: AF          xor  a
4CBB: FD 77 09    ld   (iy+$09),a
4CBE: FD 77 00    ld   (iy+$00),a
4CC1: FD 77 01    ld   (iy+$01),a
4CC4: FD CB 09 DE set  3,(iy+$09)
4CC8: FD 19       add  iy,de
4CCA: 10 EF       djnz $4CBB
4CCC: C9          ret
4CCD: FD 21 00 FC ld   iy,$FC00
4CD1: FD CB 09 76 bit  6,(iy+$09)
4CD5: C4 DE 52    call nz,$52DE
4CD8: FD 21 23 FC ld   iy,$FC23
4CDC: FD CB 09 76 bit  6,(iy+$09)
4CE0: C4 DE 52    call nz,$52DE
4CE3: FD 21 46 FC ld   iy,$FC46
4CE7: FD CB 09 76 bit  6,(iy+$09)
4CEB: C4 DE 52    call nz,$52DE
4CEE: FD 21 69 FC ld   iy,$FC69
4CF2: FD CB 09 76 bit  6,(iy+$09)
4CF6: C4 DE 52    call nz,$52DE
4CF9: FD 21 8C FC ld   iy,$FC8C
4CFD: FD CB 09 76 bit  6,(iy+$09)
4D01: C4 DE 52    call nz,$52DE
4D04: FD 21 AF FC ld   iy,$FCAF
4D08: FD CB 09 76 bit  6,(iy+$09)
4D0C: C4 DE 52    call nz,$52DE
4D0F: FD 21 D2 FC ld   iy,$FCD2
4D13: FD CB 09 76 bit  6,(iy+$09)
4D17: C4 DE 52    call nz,$52DE
4D1A: FD 21 F5 FC ld   iy,$FCF5
4D1E: FD CB 09 76 bit  6,(iy+$09)
4D22: C4 DE 52    call nz,$52DE
4D25: FD 21 18 FD ld   iy,$FD18
4D29: FD CB 09 76 bit  6,(iy+$09)
4D2D: C4 DE 52    call nz,$52DE
4D30: FD 21 3B FD ld   iy,$FD3B
4D34: FD CB 09 76 bit  6,(iy+$09)
4D38: C4 DE 52    call nz,$52DE
4D3B: FD 21 5E FD ld   iy,$FD5E
4D3F: FD CB 09 76 bit  6,(iy+$09)
4D43: C4 DE 52    call nz,$52DE
4D46: FD 21 81 FD ld   iy,$FD81
4D4A: FD CB 09 76 bit  6,(iy+$09)
4D4E: C4 DE 52    call nz,$52DE
4D51: FD 21 A4 FD ld   iy,$FDA4
4D55: FD CB 09 76 bit  6,(iy+$09)
4D59: C4 DE 52    call nz,$52DE
4D5C: FD 21 C7 FD ld   iy,$FDC7
4D60: FD CB 09 76 bit  6,(iy+$09)
4D64: C4 DE 52    call nz,$52DE
4D67: 2A 04 F8    ld   hl,($F804)
4D6A: ED 4B 06 F8 ld   bc,($F806)
4D6E: 3A 08 F8    ld   a,($F808)
4D71: B7          or   a
4D72: 20 18       jr   nz,$4D8C
4D74: 7C          ld   a,h
4D75: FE FF       cp   $FF
4D77: CA 97 4D    jp   z,$4D97
4D7A: 09          add  hl,bc
4D7B: 38 08       jr   c,$4D85
4D7D: 7C          ld   a,h
4D7E: FE FF       cp   $FF
4D80: 38 03       jr   c,$4D85
4D82: C3 97 4D    jp   $4D97
4D85: 26 FF       ld   h,$FF
4D87: 2E FF       ld   l,$FF
4D89: C3 97 4D    jp   $4D97
4D8C: 7D          ld   a,l
4D8D: B4          or   h
4D8E: 28 07       jr   z,$4D97
4D90: ED 42       sbc  hl,bc
4D92: 30 03       jr   nc,$4D97
4D94: 21 00 00    ld   hl,$0000
4D97: 22 04 F8    ld   ($F804),hl
4D9A: AF          xor  a
4D9B: 32 09 F8    ld   ($F809),a
4D9E: 3A EB FD    ld   a,($FDEB)
4DA1: F5          push af
4DA2: 21 EB FD    ld   hl,$FDEB
4DA5: 7E          ld   a,(hl)
4DA6: 23          inc  hl
4DA7: 96          sub  (hl)
4DA8: C2 AE 4D    jp   nz,$4DAE
4DAB: C3 F0 4D    jp   $4DF0
4DAE: 23          inc  hl
4DAF: 7E          ld   a,(hl)
4DB0: A7          and  a
4DB1: C2 BB 4D    jp   nz,$4DBB
4DB4: 2B          dec  hl
4DB5: 7E          ld   a,(hl)
4DB6: 2B          dec  hl
4DB7: 77          ld   (hl),a
4DB8: C3 F0 4D    jp   $4DF0
4DBB: 2B          dec  hl
4DBC: 7E          ld   a,(hl)
4DBD: CB 2F       sra  a
4DBF: 2B          dec  hl
4DC0: 46          ld   b,(hl)
4DC1: CB 28       sra  b
4DC3: 90          sub  b
4DC4: 20 06       jr   nz,$4DCC
4DC6: 23          inc  hl
4DC7: 23          inc  hl
4DC8: 77          ld   (hl),a
4DC9: C3 F0 4D    jp   $4DF0
4DCC: 23          inc  hl
4DCD: 23          inc  hl
4DCE: 4F          ld   c,a
4DCF: 87          add  a,a
4DD0: 9F          sbc  a,a
4DD1: 47          ld   b,a
4DD2: A9          xor  c
4DD3: 16 06       ld   d,$06
4DD5: 5F          ld   e,a
4DD6: 1A          ld   a,(de)
4DD7: 5E          ld   e,(hl)
4DD8: EB          ex   de,hl
4DD9: 86          add  a,(hl)
4DDA: DA DE 4D    jp   c,$4DDE
4DDD: AF          xor  a
4DDE: 24          inc  h
4DDF: 6F          ld   l,a
4DE0: 7E          ld   a,(hl)
4DE1: A8          xor  b
4DE2: 60          ld   h,b
4DE3: 6F          ld   l,a
4DE4: 29          add  hl,hl
4DE5: 29          add  hl,hl
4DE6: 29          add  hl,hl
4DE7: 29          add  hl,hl
4DE8: EB          ex   de,hl
4DE9: 2A EA FD    ld   hl,($FDEA)
4DEC: 19          add  hl,de
4DED: 22 EA FD    ld   ($FDEA),hl
4DF0: C1          pop  bc
4DF1: 3A EB FD    ld   a,($FDEB)
4DF4: B8          cp   b
4DF5: C4 67 4F    call nz,$4F67
4DF8: 21 EF FD    ld   hl,$FDEF
4DFB: 7E          ld   a,(hl)
4DFC: 23          inc  hl
4DFD: 96          sub  (hl)
4DFE: C2 04 4E    jp   nz,$4E04
4E01: C3 46 4E    jp   $4E46
4E04: 23          inc  hl
4E05: 7E          ld   a,(hl)
4E06: A7          and  a
4E07: C2 11 4E    jp   nz,$4E11
4E0A: 2B          dec  hl
4E0B: 7E          ld   a,(hl)
4E0C: 2B          dec  hl
4E0D: 77          ld   (hl),a
4E0E: C3 46 4E    jp   $4E46
4E11: 2B          dec  hl
4E12: 7E          ld   a,(hl)
4E13: CB 2F       sra  a
4E15: 2B          dec  hl
4E16: 46          ld   b,(hl)
4E17: CB 28       sra  b
4E19: 90          sub  b
4E1A: 20 06       jr   nz,$4E22
4E1C: 23          inc  hl
4E1D: 23          inc  hl
4E1E: 77          ld   (hl),a
4E1F: C3 46 4E    jp   $4E46
4E22: 23          inc  hl
4E23: 23          inc  hl
4E24: 4F          ld   c,a
4E25: 87          add  a,a
4E26: 9F          sbc  a,a
4E27: 47          ld   b,a
4E28: A9          xor  c
4E29: 16 06       ld   d,$06
4E2B: 5F          ld   e,a
4E2C: 1A          ld   a,(de)
4E2D: 5E          ld   e,(hl)
4E2E: EB          ex   de,hl
4E2F: 86          add  a,(hl)
4E30: DA 34 4E    jp   c,$4E34
4E33: AF          xor  a
4E34: 24          inc  h
4E35: 6F          ld   l,a
4E36: 7E          ld   a,(hl)
4E37: A8          xor  b
4E38: 60          ld   h,b
4E39: 6F          ld   l,a
4E3A: 29          add  hl,hl
4E3B: 29          add  hl,hl
4E3C: 29          add  hl,hl
4E3D: 29          add  hl,hl
4E3E: EB          ex   de,hl
4E3F: 2A EE FD    ld   hl,($FDEE)
4E42: 19          add  hl,de
4E43: 22 EE FD    ld   ($FDEE),hl
4E46: 3A EF FD    ld   a,($FDEF)
4E49: 47          ld   b,a
4E4A: 3A EB FD    ld   a,($FDEB)
4E4D: 80          add  a,b
4E4E: E2 54 4E    jp   po,$4E54
4E51: 9F          sbc  a,a
4E52: EE 7F       xor  $7F
4E54: C6 80       add  a,$80
4E56: 6F          ld   l,a
4E57: 26 00       ld   h,$00
4E59: 23          inc  hl
4E5A: 3A 03 F8    ld   a,($F803)
4E5D: A7          and  a
4E5E: C8          ret  z
4E5F: EB          ex   de,hl
4E60: 2A F4 FD    ld   hl,($FDF4)
4E63: 3D          dec  a
4E64: CA 6D 4E    jp   z,$4E6D
4E67: 3D          dec  a
4E68: CA 6C 4E    jp   z,$4E6C
4E6B: 19          add  hl,de
4E6C: 19          add  hl,de
4E6D: 19          add  hl,de
4E6E: 22 F4 FD    ld   ($FDF4),hl
4E71: 7C          ld   a,h
4E72: A7          and  a
4E73: C8          ret  z
4E74: 3D          dec  a
4E75: E6 03       and  $03
4E77: 32 F5 FD    ld   ($FDF5),a
4E7A: 3E 01       ld   a,$01
4E7C: 32 09 F8    ld   ($F809),a
4E7F: FD 21 00 FC ld   iy,$FC00
4E83: 21 09 FC    ld   hl,$FC09
4E86: 7E          ld   a,(hl)
4E87: E6 F7       and  $F7
4E89: 77          ld   (hl),a
4E8A: E6 04       and  $04
4E8C: C4 1F 53    call nz,$531F
4E8F: FD 21 23 FC ld   iy,$FC23
4E93: 21 2C FC    ld   hl,$FC2C
4E96: 7E          ld   a,(hl)
4E97: E6 F7       and  $F7
4E99: 77          ld   (hl),a
4E9A: E6 04       and  $04
4E9C: C4 1F 53    call nz,$531F
4E9F: FD 21 46 FC ld   iy,$FC46
4EA3: 21 4F FC    ld   hl,$FC4F
4EA6: 7E          ld   a,(hl)
4EA7: E6 F7       and  $F7
4EA9: 77          ld   (hl),a
4EAA: E6 04       and  $04
4EAC: C4 1F 53    call nz,$531F
4EAF: FD 21 69 FC ld   iy,$FC69
4EB3: 21 72 FC    ld   hl,$FC72
4EB6: 7E          ld   a,(hl)
4EB7: E6 F7       and  $F7
4EB9: 77          ld   (hl),a
4EBA: E6 04       and  $04
4EBC: C4 1F 53    call nz,$531F
4EBF: FD 21 8C FC ld   iy,$FC8C
4EC3: 21 95 FC    ld   hl,$FC95
4EC6: 7E          ld   a,(hl)
4EC7: E6 F7       and  $F7
4EC9: 77          ld   (hl),a
4ECA: E6 04       and  $04
4ECC: C4 1F 53    call nz,$531F
4ECF: FD 21 AF FC ld   iy,$FCAF
4ED3: 21 B8 FC    ld   hl,$FCB8
4ED6: 7E          ld   a,(hl)
4ED7: E6 F7       and  $F7
4ED9: 77          ld   (hl),a
4EDA: E6 04       and  $04
4EDC: C4 1F 53    call nz,$531F
4EDF: FD 21 D2 FC ld   iy,$FCD2
4EE3: 21 DB FC    ld   hl,$FCDB
4EE6: 7E          ld   a,(hl)
4EE7: E6 F7       and  $F7
4EE9: 77          ld   (hl),a
4EEA: E6 04       and  $04
4EEC: C4 1F 53    call nz,$531F
4EEF: FD 21 F5 FC ld   iy,$FCF5
4EF3: 21 FE FC    ld   hl,$FCFE
4EF6: 7E          ld   a,(hl)
4EF7: E6 F7       and  $F7
4EF9: 77          ld   (hl),a
4EFA: E6 04       and  $04
4EFC: C4 1F 53    call nz,$531F
4EFF: FD 21 18 FD ld   iy,$FD18
4F03: 21 21 FD    ld   hl,$FD21
4F06: 7E          ld   a,(hl)
4F07: E6 F7       and  $F7
4F09: 77          ld   (hl),a
4F0A: E6 04       and  $04
4F0C: C4 1F 53    call nz,$531F
4F0F: FD 21 3B FD ld   iy,$FD3B
4F13: 21 44 FD    ld   hl,$FD44
4F16: 7E          ld   a,(hl)
4F17: E6 F7       and  $F7
4F19: 77          ld   (hl),a
4F1A: E6 04       and  $04
4F1C: C4 1F 53    call nz,$531F
4F1F: FD 21 5E FD ld   iy,$FD5E
4F23: 21 67 FD    ld   hl,$FD67
4F26: 7E          ld   a,(hl)
4F27: E6 F7       and  $F7
4F29: 77          ld   (hl),a
4F2A: E6 04       and  $04
4F2C: C4 1F 53    call nz,$531F
4F2F: FD 21 81 FD ld   iy,$FD81
4F33: 21 8A FD    ld   hl,$FD8A
4F36: 7E          ld   a,(hl)
4F37: E6 F7       and  $F7
4F39: 77          ld   (hl),a
4F3A: E6 04       and  $04
4F3C: C4 1F 53    call nz,$531F
4F3F: FD 21 A4 FD ld   iy,$FDA4
4F43: 21 AD FD    ld   hl,$FDAD
4F46: 7E          ld   a,(hl)
4F47: E6 F7       and  $F7
4F49: 77          ld   (hl),a
4F4A: E6 04       and  $04
4F4C: C4 1F 53    call nz,$531F
4F4F: FD 21 C7 FD ld   iy,$FDC7
4F53: 21 D0 FD    ld   hl,$FDD0
4F56: 7E          ld   a,(hl)
4F57: E6 F7       and  $F7
4F59: 77          ld   (hl),a
4F5A: E6 04       and  $04
4F5C: C4 1F 53    call nz,$531F
4F5F: C9          ret
4F60: 32 F1 FD    ld   ($FDF1),a
4F63: 78          ld   a,b
4F64: 32 F2 FD    ld   ($FDF2),a
4F67: 3A F2 FD    ld   a,($FDF2)
4F6A: FE 40       cp   $40
4F6C: 20 05       jr   nz,$4F73
4F6E: AF          xor  a
4F6F: 32 F0 FD    ld   ($FDF0),a
4F72: C9          ret
4F73: 6F          ld   l,a
4F74: 3A EB FD    ld   a,($FDEB)
4F77: CD 90 4F    call $4F90
4F7A: 21 EB FD    ld   hl,$FDEB
4F7D: 96          sub  (hl)
4F7E: 32 F0 FD    ld   ($FDF0),a
4F81: C9          ret
4F82: 32 ED FD    ld   ($FDED),a
4F85: 68          ld   l,b
4F86: 3A F3 FD    ld   a,($FDF3)
4F89: CD 90 4F    call $4F90
4F8C: 32 EC FD    ld   ($FDEC),a
4F8F: C9          ret
4F90: C6 80       add  a,$80
4F92: 26 06       ld   h,$06
4F94: 5E          ld   e,(hl)
4F95: 6F          ld   l,a
4F96: 7E          ld   a,(hl)
4F97: 83          add  a,e
4F98: 06 C0       ld   b,$C0
4F9A: DA A4 4F    jp   c,$4FA4
4F9D: 90          sub  b
4F9E: 30 09       jr   nc,$4FA9
4FA0: AF          xor  a
4FA1: C3 A9 4F    jp   $4FA9
4FA4: 90          sub  b
4FA5: 38 02       jr   c,$4FA9
4FA7: 3E FF       ld   a,$FF
4FA9: 6F          ld   l,a
4FAA: 24          inc  h
4FAB: 7E          ld   a,(hl)
4FAC: D6 80       sub  $80
4FAE: C9          ret
4FAF: A7          and  a
4FB0: 20 07       jr   nz,$4FB9
4FB2: 21 FF FF    ld   hl,$FFFF
4FB5: 22 06 F8    ld   ($F806),hl
4FB8: C9          ret
4FB9: 6F          ld   l,a
4FBA: 26 06       ld   h,$06
4FBC: 7E          ld   a,(hl)
4FBD: 2F          cpl
4FBE: 24          inc  h
4FBF: 6E          ld   l,(hl)
4FC0: 26 00       ld   h,$00
4FC2: 29          add  hl,hl
4FC3: 29          add  hl,hl
4FC4: 29          add  hl,hl
4FC5: 29          add  hl,hl
4FC6: 22 06 F8    ld   ($F806),hl
4FC9: C9          ret
4FCA: AF          xor  a
4FCB: D3 0C       out  ($0C),a
4FCD: DD 6E 00    ld   l,(ix+$00)
4FD0: 26 00       ld   h,$00
4FD2: 11 AB 4B    ld   de,$4BAB
4FD5: 19          add  hl,de
4FD6: 7E          ld   a,(hl)
4FD7: 32 F3 FD    ld   ($FDF3),a
4FDA: 32 EB FD    ld   ($FDEB),a
4FDD: 32 EC FD    ld   ($FDEC),a
4FE0: AF          xor  a
4FE1: 32 ED FD    ld   ($FDED),a
4FE4: 32 EA FD    ld   ($FDEA),a
4FE7: 3E 40       ld   a,$40
4FE9: 32 F2 FD    ld   ($FDF2),a
4FEC: AF          xor  a
4FED: 32 F4 FD    ld   ($FDF4),a
4FF0: 32 EF FD    ld   ($FDEF),a
4FF3: 32 F0 FD    ld   ($FDF0),a
4FF6: AF          xor  a
4FF7: 32 F1 FD    ld   ($FDF1),a
4FFA: 32 EE FD    ld   ($FDEE),a
4FFD: DD 23       inc  ix
4FFF: AF          xor  a
5000: 32 05 F8    ld   ($F805),a
5003: 32 08 F8    ld   ($F808),a
5006: FD 21 00 FC ld   iy,$FC00
500A: 06 0E       ld   b,$0E
500C: DD 6E 00    ld   l,(ix+$00)
500F: DD 66 01    ld   h,(ix+$01)
5012: DD 7E 02    ld   a,(ix+$02)
5015: B4          or   h
5016: B5          or   l
5017: 28 1B       jr   z,$5034
5019: C5          push bc
501A: CD 45 52    call $5245
501D: FD CB 09 CE set  1,(iy+$09)
5021: DD 7E 04    ld   a,(ix+$04)
5024: FD 77 02    ld   (iy+$02),a
5027: 11 23 00    ld   de,$0023
502A: FD 19       add  iy,de
502C: 11 05 00    ld   de,$0005
502F: DD 19       add  ix,de
5031: C1          pop  bc
5032: 10 D8       djnz $500C
5034: 78          ld   a,b
5035: A7          and  a
5036: CA 4B 50    jp   z,$504B
5039: AF          xor  a
503A: FD 36 09 08 ld   (iy+$09),$08
503E: FD 77 00    ld   (iy+$00),a
5041: FD 77 01    ld   (iy+$01),a
5044: 11 23 00    ld   de,$0023
5047: FD 19       add  iy,de
5049: 10 EF       djnz $503A
504B: C9          ret
504C: FD 21 F6 FD ld   iy,$FDF6
5050: 06 08       ld   b,$08
5052: 11 23 00    ld   de,$0023
5055: AF          xor  a
5056: FD 77 09    ld   (iy+$09),a
5059: FD CB 09 DE set  3,(iy+$09)
505D: FD 36 02 FF ld   (iy+$02),$FF
5061: FD 77 03    ld   (iy+$03),a
5064: FD 77 00    ld   (iy+$00),a
5067: FD 77 01    ld   (iy+$01),a
506A: FD 19       add  iy,de
506C: 10 E8       djnz $5056
506E: C9          ret
506F: DD 4E 00    ld   c,(ix+$00)
5072: DD 46 01    ld   b,(ix+$01)
5075: CD 04 52    call $5204
5078: DD 4E 02    ld   c,(ix+$02)
507B: DD 46 03    ld   b,(ix+$03)
507E: CD 04 52    call $5204
5081: DD 4E 04    ld   c,(ix+$04)
5084: DD 46 05    ld   b,(ix+$05)
5087: CD 04 52    call $5204
508A: DD 4E 06    ld   c,(ix+$06)
508D: DD 46 07    ld   b,(ix+$07)
5090: CD 04 52    call $5204
5093: DD 56 08    ld   d,(ix+$08)
5096: 01 09 00    ld   bc,$0009
5099: DD 09       add  ix,bc
509B: DD 6E 00    ld   l,(ix+$00)
509E: DD 66 01    ld   h,(ix+$01)
50A1: DD 7E 02    ld   a,(ix+$02)
50A4: B5          or   l
50A5: B4          or   h
50A6: 28 26       jr   z,$50CE
50A8: DD 5E 04    ld   e,(ix+$04)
50AB: D5          push de
50AC: E5          push hl
50AD: CD CF 50    call $50CF
50B0: 7C          ld   a,h
50B1: E1          pop  hl
50B2: D1          pop  de
50B3: A7          and  a
50B4: 28 0F       jr   z,$50C5
50B6: D5          push de
50B7: CD 45 52    call $5245
50BA: D1          pop  de
50BB: FD 73 02    ld   (iy+$02),e
50BE: FD 72 03    ld   (iy+$03),d
50C1: FD CB 09 C6 set  0,(iy+$09)
50C5: 01 05 00    ld   bc,$0005
50C8: DD 09       add  ix,bc
50CA: C3 9B 50    jp   $509B
