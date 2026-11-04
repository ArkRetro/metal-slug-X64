379D: DD 36 0C FF ld   (ix+$0c),$FF
37A1: DD 36 0D FF ld   (ix+$0d),$FF
37A5: DD E5       push ix
37A7: 11 14 00    ld   de,$0014
37AA: DD 19       add  ix,de
37AC: 11 09 00    ld   de,$0009
37AF: 06 04       ld   b,$04
37B1: CB 39       srl  c
37B3: 30 14       jr   nc,$37C9
37B5: C5          push bc
37B6: DD 7E 08    ld   a,(ix+$08)
37B9: FE 7F       cp   $7F
37BB: 30 0B       jr   nc,$37C8
37BD: 3C          inc  a
37BE: DD 77 08    ld   (ix+$08),a
37C1: DD 77 01    ld   (ix+$01),a
37C4: DD CB 07 CE set  1,(ix+$07)
37C8: C1          pop  bc
37C9: DD 19       add  ix,de
37CB: 10 E4       djnz $37B1
37CD: DD E1       pop  ix
37CF: 3E FF       ld   a,$FF
37D1: 32 4F FA    ld   ($FA4F),a
37D4: C9          ret
37D5: DD 36 0C FF ld   (ix+$0c),$FF
37D9: DD 36 0D FF ld   (ix+$0d),$FF
37DD: DD E5       push ix
37DF: 11 14 00    ld   de,$0014
37E2: DD 19       add  ix,de
37E4: 11 09 00    ld   de,$0009
37E7: 06 04       ld   b,$04
37E9: CB 39       srl  c
37EB: 30 13       jr   nc,$3800
37ED: C5          push bc
37EE: DD 7E 08    ld   a,(ix+$08)
37F1: A7          and  a
37F2: 28 0B       jr   z,$37FF
37F4: 3D          dec  a
37F5: DD 77 08    ld   (ix+$08),a
37F8: DD 77 01    ld   (ix+$01),a
37FB: DD CB 07 CE set  1,(ix+$07)
37FF: C1          pop  bc
3800: DD 19       add  ix,de
3802: 10 E5       djnz $37E9
3804: DD E1       pop  ix
3806: 3E FF       ld   a,$FF
3808: 32 4F FA    ld   ($FA4F),a
380B: C9          ret
380C: DD 36 0C FF ld   (ix+$0c),$FF
3810: DD 36 0D FF ld   (ix+$0d),$FF
3814: DD E5       push ix
3816: 11 14 00    ld   de,$0014
3819: DD 19       add  ix,de
381B: 11 09 00    ld   de,$0009
381E: 06 04       ld   b,$04
3820: CB 39       srl  c
3822: 30 11       jr   nc,$3835
3824: C5          push bc
3825: DD 7E 00    ld   a,(ix+$00)
3828: FE 0F       cp   $0F
382A: 30 08       jr   nc,$3834
382C: 3C          inc  a
382D: DD 77 00    ld   (ix+$00),a
3830: DD CB 07 C6 set  0,(ix+$07)
3834: C1          pop  bc
3835: DD 19       add  ix,de
3837: 10 E7       djnz $3820
3839: DD E1       pop  ix
383B: C9          ret
383C: DD 36 0C FF ld   (ix+$0c),$FF
3840: DD 36 0D FF ld   (ix+$0d),$FF
3844: DD E5       push ix
3846: 11 14 00    ld   de,$0014
3849: DD 19       add  ix,de
384B: 11 09 00    ld   de,$0009
384E: 06 04       ld   b,$04
3850: CB 39       srl  c
3852: 30 10       jr   nc,$3864
3854: C5          push bc
3855: DD 7E 00    ld   a,(ix+$00)
3858: A7          and  a
3859: 28 08       jr   z,$3863
385B: 3D          dec  a
385C: DD 77 00    ld   (ix+$00),a
385F: DD CB 07 C6 set  0,(ix+$07)
3863: C1          pop  bc
3864: DD 19       add  ix,de
3866: 10 E8       djnz $3850
3868: DD E1       pop  ix
386A: C9          ret
386B: 11 00 02    ld   de,$0200
386E: 0E 06       ld   c,$06
3870: CF          rst  $08
3871: 11 FF 00    ld   de,$00FF
3874: 0E 06       ld   c,$06
3876: CF          rst  $08
3877: 11 FF 1C    ld   de,$1CFF
387A: 0E 04       ld   c,$04
387C: CF          rst  $08
387D: 11 3E 01    ld   de,$013E
3880: 0E 06       ld   c,$06
3882: CF          rst  $08
3883: DD 21 A9 FA ld   ix,$FAA9
3887: CD AE 38    call $38AE
388A: DD 21 D2 FA ld   ix,$FAD2
388E: CD AE 38    call $38AE
3891: DD 21 FB FA ld   ix,$FAFB
3895: CD AE 38    call $38AE
3898: DD 21 24 FB ld   ix,$FB24
389C: CD AE 38    call $38AE
389F: DD 21 4D FB ld   ix,$FB4D
38A3: CD AE 38    call $38AE
38A6: DD 21 76 FB ld   ix,$FB76
38AA: CD AE 38    call $38AE
38AD: C9          ret
38AE: CD 96 1F    call $1F96
38B1: DD 36 02 02 ld   (ix+$02),$02
38B5: AF          xor  a
38B6: DD 77 1A    ld   (ix+$1a),a
38B9: DD 77 14    ld   (ix+$14),a
38BC: DD E5       push ix
38BE: 11 1B 00    ld   de,$001B
38C1: DD 19       add  ix,de
38C3: F7          rst  $30
38C4: DD E1       pop  ix
38C6: C9          ret
38C7: DD 21 A9 FA ld   ix,$FAA9
38CB: FD 2A A9 FA ld   iy,($FAA9)
38CF: FD CB 09 5E bit  3,(iy+$09)
38D3: C4 CC 1F    call nz,$1FCC
38D6: 21 C3 FA    ld   hl,$FAC3
38D9: 7E          ld   a,(hl)
38DA: A7          and  a
38DB: CA AF 39    jp   z,$39AF
38DE: FE 02       cp   $02
38E0: 28 12       jr   z,$38F4
38E2: 3A 0A F8    ld   a,($F80A)
38E5: E6 01       and  $01
38E7: CA 30 39    jp   z,$3930
38EA: AF          xor  a
38EB: 32 C3 FA    ld   ($FAC3),a
38EE: 32 B4 FA    ld   ($FAB4),a
38F1: C3 AF 39    jp   $39AF
38F4: 2A BF FA    ld   hl,($FABF)
38F7: ED 4B C1 FA ld   bc,($FAC1)
38FB: 3E 01       ld   a,$01
38FD: 32 C3 FA    ld   ($FAC3),a
3900: C5          push bc
3901: 11 C0 08    ld   de,$08C0
3904: 0E 06       ld   c,$06
3906: CF          rst  $08
3907: AF          xor  a
3908: 32 B9 FA    ld   ($FAB9),a
390B: 11 81 00    ld   de,$0081
390E: CF          rst  $08
390F: 11 01 1C    ld   de,$1C01
3912: 0E 04       ld   c,$04
3914: CF          rst  $08
3915: 1E 00       ld   e,$00
3917: CF          rst  $08
3918: 0E 06       ld   c,$06
391A: 5D          ld   e,l
391B: 16 10       ld   d,$10
391D: CF          rst  $08
391E: 5C          ld   e,h
391F: 16 18       ld   d,$18
3921: E1          pop  hl
3922: CF          rst  $08
3923: 5D          ld   e,l
3924: 16 20       ld   d,$20
3926: CF          rst  $08
3927: 5C          ld   e,h
3928: 16 28       ld   d,$28
392A: CF          rst  $08
392B: 1E 01       ld   e,$01
392D: 16 00       ld   d,$00
392F: CF          rst  $08
3930: 21 C4 FA    ld   hl,$FAC4
3933: CD DE 21    call $21DE
3936: 6F          ld   l,a
3937: 3A B4 FA    ld   a,($FAB4)
393A: A7          and  a
393B: 28 15       jr   z,$3952
393D: 2C          inc  l
393E: 28 12       jr   z,$3952
3940: 08          ex   af,af'
3941: 2D          dec  l
3942: 7D          ld   a,l
3943: 28 0D       jr   z,$3952
3945: 26 06       ld   h,$06
3947: 7E          ld   a,(hl)
3948: 08          ex   af,af'
3949: 6F          ld   l,a
394A: 08          ex   af,af'
394B: 86          add  a,(hl)
394C: 6F          ld   l,a
394D: 9F          sbc  a,a
394E: 30 02       jr   nc,$3952
3950: 24          inc  h
3951: 7E          ld   a,(hl)
3952: FD 6E 12    ld   l,(iy+$12)
3955: A7          and  a
3956: 28 15       jr   z,$396D
3958: 2C          inc  l
3959: 28 12       jr   z,$396D
395B: 08          ex   af,af'
395C: 2D          dec  l
395D: 7D          ld   a,l
395E: 28 0D       jr   z,$396D
3960: 26 06       ld   h,$06
3962: 7E          ld   a,(hl)
3963: 08          ex   af,af'
3964: 6F          ld   l,a
3965: 08          ex   af,af'
3966: 86          add  a,(hl)
3967: 6F          ld   l,a
3968: 9F          sbc  a,a
3969: 30 02       jr   nc,$396D
396B: 24          inc  h
396C: 7E          ld   a,(hl)
396D: 6F          ld   l,a
396E: 3E FF       ld   a,$FF
3970: FD CB 09 4E bit  1,(iy+$09)
3974: CA 7A 39    jp   z,$397A
3977: 3A 05 F8    ld   a,($F805)
397A: A7          and  a
397B: 28 15       jr   z,$3992
397D: 2C          inc  l
397E: 28 12       jr   z,$3992
3980: 08          ex   af,af'
3981: 2D          dec  l
3982: 7D          ld   a,l
3983: 28 0D       jr   z,$3992
3985: 26 06       ld   h,$06
3987: 7E          ld   a,(hl)
3988: 08          ex   af,af'
3989: 6F          ld   l,a
398A: 08          ex   af,af'
398B: 86          add  a,(hl)
398C: 6F          ld   l,a
398D: 9F          sbc  a,a
398E: 30 02       jr   nc,$3992
3990: 24          inc  h
3991: 7E          ld   a,(hl)
3992: 21 B9 FA    ld   hl,$FAB9
3995: BE          cp   (hl)
3996: 77          ld   (hl),a
3997: 26 0B       ld   h,$0B
3999: 6F          ld   l,a
399A: 7E          ld   a,(hl)
399B: A7          and  a
399C: 28 0B       jr   z,$39A9
399E: D6 01       sub  $01
39A0: 5F          ld   e,a
39A1: 3A BE FA    ld   a,($FABE)
39A4: E6 03       and  $03
39A6: 0F          rrca
39A7: 0F          rrca
39A8: B3          or   e
39A9: 5F          ld   e,a
39AA: 16 08       ld   d,$08
39AC: 0E 06       ld   c,$06
39AE: CF          rst  $08
39AF: 0E 01       ld   c,$01
39B1: FD CB 09 4E bit  1,(iy+$09)
39B5: 28 04       jr   z,$39BB
39B7: 3A 09 F8    ld   a,($F809)
39BA: 4F          ld   c,a
39BB: 21 AF FA    ld   hl,$FAAF
39BE: 7E          ld   a,(hl)
39BF: 81          add  a,c
39C0: DE 00       sbc  a,$00
39C2: 77          ld   (hl),a
39C3: 21 AE FA    ld   hl,$FAAE
39C6: 7E          ld   a,(hl)
39C7: A7          and  a
39C8: CA DC 39    jp   z,$39DC
39CB: 91          sub  c
39CC: 77          ld   (hl),a
39CD: 20 22       jr   nz,$39F1
39CF: FD CB 09 66 bit  4,(iy+$09)
39D3: 28 16       jr   z,$39EB
39D5: DD CB 03 E6 set  4,(ix+$03)
39D9: C3 F1 39    jp   $39F1
39DC: FD CB 09 66 bit  4,(iy+$09)
39E0: 20 0F       jr   nz,$39F1
39E2: 21 AC FA    ld   hl,$FAAC
39E5: CB 66       bit  4,(hl)
39E7: 28 08       jr   z,$39F1
39E9: CB A6       res  4,(hl)
39EB: 21 C4 FA    ld   hl,$FAC4
39EE: CD CA 21    call $21CA
39F1: DD 21 D2 FA ld   ix,$FAD2
39F5: FD 2A D2 FA ld   iy,($FAD2)
39F9: FD CB 09 5E bit  3,(iy+$09)
39FD: C4 CC 1F    call nz,$1FCC
3A00: 21 EC FA    ld   hl,$FAEC
3A03: 7E          ld   a,(hl)
3A04: A7          and  a
3A05: CA D9 3A    jp   z,$3AD9
3A08: FE 02       cp   $02
3A0A: 28 12       jr   z,$3A1E
3A0C: 3A 0A F8    ld   a,($F80A)
3A0F: E6 02       and  $02
3A11: CA 5A 3A    jp   z,$3A5A
3A14: AF          xor  a
3A15: 32 EC FA    ld   ($FAEC),a
3A18: 32 DD FA    ld   ($FADD),a
3A1B: C3 D9 3A    jp   $3AD9
3A1E: 2A E8 FA    ld   hl,($FAE8)
3A21: ED 4B EA FA ld   bc,($FAEA)
3A25: 3E 01       ld   a,$01
3A27: 32 EC FA    ld   ($FAEC),a
3A2A: C5          push bc
3A2B: 11 C0 09    ld   de,$09C0
3A2E: 0E 06       ld   c,$06
3A30: CF          rst  $08
3A31: AF          xor  a
3A32: 32 E2 FA    ld   ($FAE2),a
3A35: 11 82 00    ld   de,$0082
3A38: CF          rst  $08
3A39: 11 02 1C    ld   de,$1C02
3A3C: 0E 04       ld   c,$04
3A3E: CF          rst  $08
3A3F: 1E 00       ld   e,$00
3A41: CF          rst  $08
3A42: 0E 06       ld   c,$06
3A44: 5D          ld   e,l
3A45: 16 11       ld   d,$11
3A47: CF          rst  $08
3A48: 5C          ld   e,h
3A49: 16 19       ld   d,$19
3A4B: E1          pop  hl
3A4C: CF          rst  $08
3A4D: 5D          ld   e,l
3A4E: 16 21       ld   d,$21
3A50: CF          rst  $08
3A51: 5C          ld   e,h
3A52: 16 29       ld   d,$29
3A54: CF          rst  $08
3A55: 1E 02       ld   e,$02
3A57: 16 00       ld   d,$00
3A59: CF          rst  $08
3A5A: 21 ED FA    ld   hl,$FAED
3A5D: CD DE 21    call $21DE
3A60: 6F          ld   l,a
3A61: 3A DD FA    ld   a,($FADD)
3A64: A7          and  a
3A65: 28 15       jr   z,$3A7C
3A67: 2C          inc  l
3A68: 28 12       jr   z,$3A7C
3A6A: 08          ex   af,af'
3A6B: 2D          dec  l
3A6C: 7D          ld   a,l
3A6D: 28 0D       jr   z,$3A7C
3A6F: 26 06       ld   h,$06
3A71: 7E          ld   a,(hl)
3A72: 08          ex   af,af'
3A73: 6F          ld   l,a
3A74: 08          ex   af,af'
3A75: 86          add  a,(hl)
3A76: 6F          ld   l,a
3A77: 9F          sbc  a,a
3A78: 30 02       jr   nc,$3A7C
3A7A: 24          inc  h
3A7B: 7E          ld   a,(hl)
3A7C: FD 6E 12    ld   l,(iy+$12)
3A7F: A7          and  a
3A80: 28 15       jr   z,$3A97
3A82: 2C          inc  l
3A83: 28 12       jr   z,$3A97
3A85: 08          ex   af,af'
3A86: 2D          dec  l
3A87: 7D          ld   a,l
3A88: 28 0D       jr   z,$3A97
3A8A: 26 06       ld   h,$06
3A8C: 7E          ld   a,(hl)
3A8D: 08          ex   af,af'
3A8E: 6F          ld   l,a
3A8F: 08          ex   af,af'
3A90: 86          add  a,(hl)
3A91: 6F          ld   l,a
3A92: 9F          sbc  a,a
3A93: 30 02       jr   nc,$3A97
3A95: 24          inc  h
3A96: 7E          ld   a,(hl)
3A97: 6F          ld   l,a
3A98: 3E FF       ld   a,$FF
3A9A: FD CB 09 4E bit  1,(iy+$09)
3A9E: CA A4 3A    jp   z,$3AA4
3AA1: 3A 05 F8    ld   a,($F805)
3AA4: A7          and  a
3AA5: 28 15       jr   z,$3ABC
3AA7: 2C          inc  l
3AA8: 28 12       jr   z,$3ABC
3AAA: 08          ex   af,af'
3AAB: 2D          dec  l
3AAC: 7D          ld   a,l
3AAD: 28 0D       jr   z,$3ABC
3AAF: 26 06       ld   h,$06
3AB1: 7E          ld   a,(hl)
3AB2: 08          ex   af,af'
3AB3: 6F          ld   l,a
3AB4: 08          ex   af,af'
3AB5: 86          add  a,(hl)
3AB6: 6F          ld   l,a
3AB7: 9F          sbc  a,a
3AB8: 30 02       jr   nc,$3ABC
3ABA: 24          inc  h
3ABB: 7E          ld   a,(hl)
3ABC: 21 E2 FA    ld   hl,$FAE2
3ABF: BE          cp   (hl)
3AC0: 77          ld   (hl),a
3AC1: 26 0B       ld   h,$0B
3AC3: 6F          ld   l,a
3AC4: 7E          ld   a,(hl)
3AC5: A7          and  a
3AC6: 28 0B       jr   z,$3AD3
3AC8: D6 01       sub  $01
3ACA: 5F          ld   e,a
3ACB: 3A E7 FA    ld   a,($FAE7)
3ACE: E6 03       and  $03
3AD0: 0F          rrca
3AD1: 0F          rrca
3AD2: B3          or   e
3AD3: 5F          ld   e,a
3AD4: 16 09       ld   d,$09
3AD6: 0E 06       ld   c,$06
3AD8: CF          rst  $08
3AD9: 0E 01       ld   c,$01
3ADB: FD CB 09 4E bit  1,(iy+$09)
3ADF: 28 04       jr   z,$3AE5
3AE1: 3A 09 F8    ld   a,($F809)
3AE4: 4F          ld   c,a
3AE5: 21 D8 FA    ld   hl,$FAD8
3AE8: 7E          ld   a,(hl)
3AE9: 81          add  a,c
3AEA: DE 00       sbc  a,$00
3AEC: 77          ld   (hl),a
3AED: 21 D7 FA    ld   hl,$FAD7
3AF0: 7E          ld   a,(hl)
3AF1: A7          and  a
3AF2: CA 06 3B    jp   z,$3B06
3AF5: 91          sub  c
3AF6: 77          ld   (hl),a
3AF7: 20 22       jr   nz,$3B1B
3AF9: FD CB 09 66 bit  4,(iy+$09)
3AFD: 28 16       jr   z,$3B15
3AFF: DD CB 03 E6 set  4,(ix+$03)
3B03: C3 1B 3B    jp   $3B1B
3B06: FD CB 09 66 bit  4,(iy+$09)
3B0A: 20 0F       jr   nz,$3B1B
3B0C: 21 D5 FA    ld   hl,$FAD5
3B0F: CB 66       bit  4,(hl)
3B11: 28 08       jr   z,$3B1B
3B13: CB A6       res  4,(hl)
3B15: 21 ED FA    ld   hl,$FAED
3B18: CD CA 21    call $21CA
3B1B: DD 21 FB FA ld   ix,$FAFB
3B1F: FD 2A FB FA ld   iy,($FAFB)
3B23: FD CB 09 5E bit  3,(iy+$09)
3B27: C4 CC 1F    call nz,$1FCC
3B2A: 21 15 FB    ld   hl,$FB15
3B2D: 7E          ld   a,(hl)
3B2E: A7          and  a
3B2F: CA 03 3C    jp   z,$3C03
3B32: FE 02       cp   $02
3B34: 28 12       jr   z,$3B48
3B36: 3A 0A F8    ld   a,($F80A)
3B39: E6 04       and  $04
3B3B: CA 84 3B    jp   z,$3B84
3B3E: AF          xor  a
3B3F: 32 15 FB    ld   ($FB15),a
3B42: 32 06 FB    ld   ($FB06),a
3B45: C3 03 3C    jp   $3C03
3B48: 2A 11 FB    ld   hl,($FB11)
3B4B: ED 4B 13 FB ld   bc,($FB13)
3B4F: 3E 01       ld   a,$01
3B51: 32 15 FB    ld   ($FB15),a
3B54: C5          push bc
3B55: 11 C0 0A    ld   de,$0AC0
3B58: 0E 06       ld   c,$06
3B5A: CF          rst  $08
3B5B: AF          xor  a
3B5C: 32 0B FB    ld   ($FB0B),a
3B5F: 11 84 00    ld   de,$0084
3B62: CF          rst  $08
3B63: 11 04 1C    ld   de,$1C04
3B66: 0E 04       ld   c,$04
3B68: CF          rst  $08
3B69: 1E 00       ld   e,$00
3B6B: CF          rst  $08
3B6C: 0E 06       ld   c,$06
3B6E: 5D          ld   e,l
3B6F: 16 12       ld   d,$12
3B71: CF          rst  $08
3B72: 5C          ld   e,h
3B73: 16 1A       ld   d,$1A
3B75: E1          pop  hl
3B76: CF          rst  $08
3B77: 5D          ld   e,l
3B78: 16 22       ld   d,$22
3B7A: CF          rst  $08
3B7B: 5C          ld   e,h
3B7C: 16 2A       ld   d,$2A
3B7E: CF          rst  $08
3B7F: 1E 04       ld   e,$04
3B81: 16 00       ld   d,$00
3B83: CF          rst  $08
3B84: 21 16 FB    ld   hl,$FB16
3B87: CD DE 21    call $21DE
3B8A: 6F          ld   l,a
3B8B: 3A 06 FB    ld   a,($FB06)
3B8E: A7          and  a
3B8F: 28 15       jr   z,$3BA6
3B91: 2C          inc  l
3B92: 28 12       jr   z,$3BA6
3B94: 08          ex   af,af'
3B95: 2D          dec  l
3B96: 7D          ld   a,l
3B97: 28 0D       jr   z,$3BA6
3B99: 26 06       ld   h,$06
3B9B: 7E          ld   a,(hl)
3B9C: 08          ex   af,af'
3B9D: 6F          ld   l,a
3B9E: 08          ex   af,af'
3B9F: 86          add  a,(hl)
3BA0: 6F          ld   l,a
3BA1: 9F          sbc  a,a
3BA2: 30 02       jr   nc,$3BA6
3BA4: 24          inc  h
3BA5: 7E          ld   a,(hl)
3BA6: FD 6E 12    ld   l,(iy+$12)
3BA9: A7          and  a
3BAA: 28 15       jr   z,$3BC1
3BAC: 2C          inc  l
3BAD: 28 12       jr   z,$3BC1
3BAF: 08          ex   af,af'
3BB0: 2D          dec  l
3BB1: 7D          ld   a,l
3BB2: 28 0D       jr   z,$3BC1
3BB4: 26 06       ld   h,$06
3BB6: 7E          ld   a,(hl)
3BB7: 08          ex   af,af'
3BB8: 6F          ld   l,a
3BB9: 08          ex   af,af'
3BBA: 86          add  a,(hl)
3BBB: 6F          ld   l,a
3BBC: 9F          sbc  a,a
3BBD: 30 02       jr   nc,$3BC1
3BBF: 24          inc  h
3BC0: 7E          ld   a,(hl)
3BC1: 6F          ld   l,a
3BC2: 3E FF       ld   a,$FF
3BC4: FD CB 09 4E bit  1,(iy+$09)
3BC8: CA CE 3B    jp   z,$3BCE
3BCB: 3A 05 F8    ld   a,($F805)
3BCE: A7          and  a
3BCF: 28 15       jr   z,$3BE6
3BD1: 2C          inc  l
3BD2: 28 12       jr   z,$3BE6
3BD4: 08          ex   af,af'
3BD5: 2D          dec  l
3BD6: 7D          ld   a,l
3BD7: 28 0D       jr   z,$3BE6
3BD9: 26 06       ld   h,$06
3BDB: 7E          ld   a,(hl)
3BDC: 08          ex   af,af'
3BDD: 6F          ld   l,a
3BDE: 08          ex   af,af'
3BDF: 86          add  a,(hl)
3BE0: 6F          ld   l,a
3BE1: 9F          sbc  a,a
3BE2: 30 02       jr   nc,$3BE6
3BE4: 24          inc  h
3BE5: 7E          ld   a,(hl)
3BE6: 21 0B FB    ld   hl,$FB0B
3BE9: BE          cp   (hl)
3BEA: 77          ld   (hl),a
3BEB: 26 0B       ld   h,$0B
3BED: 6F          ld   l,a
3BEE: 7E          ld   a,(hl)
3BEF: A7          and  a
3BF0: 28 0B       jr   z,$3BFD
3BF2: D6 01       sub  $01
3BF4: 5F          ld   e,a
3BF5: 3A 10 FB    ld   a,($FB10)
3BF8: E6 03       and  $03
3BFA: 0F          rrca
3BFB: 0F          rrca
3BFC: B3          or   e
3BFD: 5F          ld   e,a
3BFE: 16 0A       ld   d,$0A
3C00: 0E 06       ld   c,$06
3C02: CF          rst  $08
3C03: 0E 01       ld   c,$01
3C05: FD CB 09 4E bit  1,(iy+$09)
3C09: 28 04       jr   z,$3C0F
3C0B: 3A 09 F8    ld   a,($F809)
3C0E: 4F          ld   c,a
3C0F: 21 01 FB    ld   hl,$FB01
3C12: 7E          ld   a,(hl)
3C13: 81          add  a,c
3C14: DE 00       sbc  a,$00
3C16: 77          ld   (hl),a
3C17: 21 00 FB    ld   hl,$FB00
3C1A: 7E          ld   a,(hl)
3C1B: A7          and  a
3C1C: CA 30 3C    jp   z,$3C30
3C1F: 91          sub  c
3C20: 77          ld   (hl),a
3C21: 20 22       jr   nz,$3C45
3C23: FD CB 09 66 bit  4,(iy+$09)
3C27: 28 16       jr   z,$3C3F
3C29: DD CB 03 E6 set  4,(ix+$03)
3C2D: C3 45 3C    jp   $3C45
3C30: FD CB 09 66 bit  4,(iy+$09)
3C34: 20 0F       jr   nz,$3C45
3C36: 21 FE FA    ld   hl,$FAFE
3C39: CB 66       bit  4,(hl)
3C3B: 28 08       jr   z,$3C45
3C3D: CB A6       res  4,(hl)
3C3F: 21 16 FB    ld   hl,$FB16
3C42: CD CA 21    call $21CA
3C45: DD 21 24 FB ld   ix,$FB24
3C49: FD 2A 24 FB ld   iy,($FB24)
3C4D: FD CB 09 5E bit  3,(iy+$09)
3C51: C4 CC 1F    call nz,$1FCC
3C54: 21 3E FB    ld   hl,$FB3E
3C57: 7E          ld   a,(hl)
3C58: A7          and  a
3C59: CA 2D 3D    jp   z,$3D2D
3C5C: FE 02       cp   $02
3C5E: 28 12       jr   z,$3C72
3C60: 3A 0A F8    ld   a,($F80A)
3C63: E6 08       and  $08
3C65: CA AE 3C    jp   z,$3CAE
3C68: AF          xor  a
3C69: 32 3E FB    ld   ($FB3E),a
3C6C: 32 2F FB    ld   ($FB2F),a
3C6F: C3 2D 3D    jp   $3D2D
3C72: 2A 3A FB    ld   hl,($FB3A)
3C75: ED 4B 3C FB ld   bc,($FB3C)
3C79: 3E 01       ld   a,$01
3C7B: 32 3E FB    ld   ($FB3E),a
3C7E: C5          push bc
3C7F: 11 C0 0B    ld   de,$0BC0
3C82: 0E 06       ld   c,$06
3C84: CF          rst  $08
3C85: AF          xor  a
3C86: 32 34 FB    ld   ($FB34),a
3C89: 11 88 00    ld   de,$0088
3C8C: CF          rst  $08
3C8D: 11 08 1C    ld   de,$1C08
3C90: 0E 04       ld   c,$04
3C92: CF          rst  $08
3C93: 1E 00       ld   e,$00
3C95: CF          rst  $08
3C96: 0E 06       ld   c,$06
3C98: 5D          ld   e,l
3C99: 16 13       ld   d,$13
3C9B: CF          rst  $08
3C9C: 5C          ld   e,h
3C9D: 16 1B       ld   d,$1B
3C9F: E1          pop  hl
3CA0: CF          rst  $08
3CA1: 5D          ld   e,l
3CA2: 16 23       ld   d,$23
3CA4: CF          rst  $08
3CA5: 5C          ld   e,h
3CA6: 16 2B       ld   d,$2B
3CA8: CF          rst  $08
3CA9: 1E 08       ld   e,$08
3CAB: 16 00       ld   d,$00
3CAD: CF          rst  $08
3CAE: 21 3F FB    ld   hl,$FB3F
3CB1: CD DE 21    call $21DE
3CB4: 6F          ld   l,a
3CB5: 3A 2F FB    ld   a,($FB2F)
3CB8: A7          and  a
3CB9: 28 15       jr   z,$3CD0
3CBB: 2C          inc  l
3CBC: 28 12       jr   z,$3CD0
3CBE: 08          ex   af,af'
3CBF: 2D          dec  l
3CC0: 7D          ld   a,l
3CC1: 28 0D       jr   z,$3CD0
3CC3: 26 06       ld   h,$06
3CC5: 7E          ld   a,(hl)
3CC6: 08          ex   af,af'
3CC7: 6F          ld   l,a
3CC8: 08          ex   af,af'
3CC9: 86          add  a,(hl)
3CCA: 6F          ld   l,a
3CCB: 9F          sbc  a,a
3CCC: 30 02       jr   nc,$3CD0
3CCE: 24          inc  h
3CCF: 7E          ld   a,(hl)
3CD0: FD 6E 12    ld   l,(iy+$12)
3CD3: A7          and  a
3CD4: 28 15       jr   z,$3CEB
3CD6: 2C          inc  l
3CD7: 28 12       jr   z,$3CEB
3CD9: 08          ex   af,af'
3CDA: 2D          dec  l
3CDB: 7D          ld   a,l
3CDC: 28 0D       jr   z,$3CEB
3CDE: 26 06       ld   h,$06
3CE0: 7E          ld   a,(hl)
3CE1: 08          ex   af,af'
3CE2: 6F          ld   l,a
3CE3: 08          ex   af,af'
3CE4: 86          add  a,(hl)
3CE5: 6F          ld   l,a
3CE6: 9F          sbc  a,a
3CE7: 30 02       jr   nc,$3CEB
3CE9: 24          inc  h
3CEA: 7E          ld   a,(hl)
3CEB: 6F          ld   l,a
3CEC: 3E FF       ld   a,$FF
3CEE: FD CB 09 4E bit  1,(iy+$09)
3CF2: CA F8 3C    jp   z,$3CF8
3CF5: 3A 05 F8    ld   a,($F805)
3CF8: A7          and  a
3CF9: 28 15       jr   z,$3D10
3CFB: 2C          inc  l
3CFC: 28 12       jr   z,$3D10
3CFE: 08          ex   af,af'
3CFF: 2D          dec  l
3D00: 7D          ld   a,l
3D01: 28 0D       jr   z,$3D10
3D03: 26 06       ld   h,$06
3D05: 7E          ld   a,(hl)
3D06: 08          ex   af,af'
3D07: 6F          ld   l,a
3D08: 08          ex   af,af'
3D09: 86          add  a,(hl)
3D0A: 6F          ld   l,a
3D0B: 9F          sbc  a,a
3D0C: 30 02       jr   nc,$3D10
3D0E: 24          inc  h
3D0F: 7E          ld   a,(hl)
3D10: 21 34 FB    ld   hl,$FB34
3D13: BE          cp   (hl)
3D14: 77          ld   (hl),a
3D15: 26 0B       ld   h,$0B
3D17: 6F          ld   l,a
3D18: 7E          ld   a,(hl)
3D19: A7          and  a
3D1A: 28 0B       jr   z,$3D27
3D1C: D6 01       sub  $01
3D1E: 5F          ld   e,a
3D1F: 3A 39 FB    ld   a,($FB39)
3D22: E6 03       and  $03
3D24: 0F          rrca
3D25: 0F          rrca
3D26: B3          or   e
3D27: 5F          ld   e,a
3D28: 16 0B       ld   d,$0B
3D2A: 0E 06       ld   c,$06
3D2C: CF          rst  $08
3D2D: 0E 01       ld   c,$01
3D2F: FD CB 09 4E bit  1,(iy+$09)
3D33: 28 04       jr   z,$3D39
3D35: 3A 09 F8    ld   a,($F809)
3D38: 4F          ld   c,a
3D39: 21 2A FB    ld   hl,$FB2A
3D3C: 7E          ld   a,(hl)
3D3D: 81          add  a,c
3D3E: DE 00       sbc  a,$00
3D40: 77          ld   (hl),a
3D41: 21 29 FB    ld   hl,$FB29
3D44: 7E          ld   a,(hl)
3D45: A7          and  a
3D46: CA 5A 3D    jp   z,$3D5A
3D49: 91          sub  c
3D4A: 77          ld   (hl),a
3D4B: 20 22       jr   nz,$3D6F
3D4D: FD CB 09 66 bit  4,(iy+$09)
3D51: 28 16       jr   z,$3D69
3D53: DD CB 03 E6 set  4,(ix+$03)
3D57: C3 6F 3D    jp   $3D6F
3D5A: FD CB 09 66 bit  4,(iy+$09)
3D5E: 20 0F       jr   nz,$3D6F
3D60: 21 27 FB    ld   hl,$FB27
3D63: CB 66       bit  4,(hl)
3D65: 28 08       jr   z,$3D6F
3D67: CB A6       res  4,(hl)
3D69: 21 3F FB    ld   hl,$FB3F
3D6C: CD CA 21    call $21CA
3D6F: DD 21 4D FB ld   ix,$FB4D
3D73: FD 2A 4D FB ld   iy,($FB4D)
3D77: FD CB 09 5E bit  3,(iy+$09)
3D7B: C4 CC 1F    call nz,$1FCC
3D7E: 21 67 FB    ld   hl,$FB67
3D81: 7E          ld   a,(hl)
3D82: A7          and  a
3D83: CA 57 3E    jp   z,$3E57
3D86: FE 02       cp   $02
3D88: 28 12       jr   z,$3D9C
3D8A: 3A 0A F8    ld   a,($F80A)
3D8D: E6 10       and  $10
3D8F: CA D8 3D    jp   z,$3DD8
3D92: AF          xor  a
3D93: 32 67 FB    ld   ($FB67),a
3D96: 32 58 FB    ld   ($FB58),a
3D99: C3 57 3E    jp   $3E57
3D9C: 2A 63 FB    ld   hl,($FB63)
3D9F: ED 4B 65 FB ld   bc,($FB65)
3DA3: 3E 01       ld   a,$01
3DA5: 32 67 FB    ld   ($FB67),a
3DA8: C5          push bc
3DA9: 11 C0 0C    ld   de,$0CC0
3DAC: 0E 06       ld   c,$06
3DAE: CF          rst  $08
3DAF: AF          xor  a
3DB0: 32 5D FB    ld   ($FB5D),a
3DB3: 11 90 00    ld   de,$0090
3DB6: CF          rst  $08
3DB7: 11 10 1C    ld   de,$1C10
3DBA: 0E 04       ld   c,$04
3DBC: CF          rst  $08
3DBD: 1E 00       ld   e,$00
3DBF: CF          rst  $08
3DC0: 0E 06       ld   c,$06
3DC2: 5D          ld   e,l
3DC3: 16 14       ld   d,$14
3DC5: CF          rst  $08
3DC6: 5C          ld   e,h
3DC7: 16 1C       ld   d,$1C
3DC9: E1          pop  hl
3DCA: CF          rst  $08
3DCB: 5D          ld   e,l
3DCC: 16 24       ld   d,$24
3DCE: CF          rst  $08
3DCF: 5C          ld   e,h
3DD0: 16 2C       ld   d,$2C
3DD2: CF          rst  $08
3DD3: 1E 10       ld   e,$10
3DD5: 16 00       ld   d,$00
3DD7: CF          rst  $08
3DD8: 21 68 FB    ld   hl,$FB68
3DDB: CD DE 21    call $21DE
3DDE: 6F          ld   l,a
3DDF: 3A 58 FB    ld   a,($FB58)
3DE2: A7          and  a
3DE3: 28 15       jr   z,$3DFA
3DE5: 2C          inc  l
3DE6: 28 12       jr   z,$3DFA
3DE8: 08          ex   af,af'
3DE9: 2D          dec  l
3DEA: 7D          ld   a,l
3DEB: 28 0D       jr   z,$3DFA
3DED: 26 06       ld   h,$06
3DEF: 7E          ld   a,(hl)
3DF0: 08          ex   af,af'
3DF1: 6F          ld   l,a
3DF2: 08          ex   af,af'
3DF3: 86          add  a,(hl)
3DF4: 6F          ld   l,a
3DF5: 9F          sbc  a,a
3DF6: 30 02       jr   nc,$3DFA
3DF8: 24          inc  h
3DF9: 7E          ld   a,(hl)
3DFA: FD 6E 12    ld   l,(iy+$12)
3DFD: A7          and  a
3DFE: 28 15       jr   z,$3E15
3E00: 2C          inc  l
3E01: 28 12       jr   z,$3E15
3E03: 08          ex   af,af'
3E04: 2D          dec  l
3E05: 7D          ld   a,l
3E06: 28 0D       jr   z,$3E15
3E08: 26 06       ld   h,$06
3E0A: 7E          ld   a,(hl)
3E0B: 08          ex   af,af'
3E0C: 6F          ld   l,a
3E0D: 08          ex   af,af'
3E0E: 86          add  a,(hl)
3E0F: 6F          ld   l,a
3E10: 9F          sbc  a,a
3E11: 30 02       jr   nc,$3E15
3E13: 24          inc  h
3E14: 7E          ld   a,(hl)
3E15: 6F          ld   l,a
3E16: 3E FF       ld   a,$FF
3E18: FD CB 09 4E bit  1,(iy+$09)
3E1C: CA 22 3E    jp   z,$3E22
3E1F: 3A 05 F8    ld   a,($F805)
3E22: A7          and  a
3E23: 28 15       jr   z,$3E3A
3E25: 2C          inc  l
3E26: 28 12       jr   z,$3E3A
3E28: 08          ex   af,af'
3E29: 2D          dec  l
3E2A: 7D          ld   a,l
3E2B: 28 0D       jr   z,$3E3A
3E2D: 26 06       ld   h,$06
3E2F: 7E          ld   a,(hl)
3E30: 08          ex   af,af'
3E31: 6F          ld   l,a
3E32: 08          ex   af,af'
3E33: 86          add  a,(hl)
3E34: 6F          ld   l,a
3E35: 9F          sbc  a,a
3E36: 30 02       jr   nc,$3E3A
3E38: 24          inc  h
3E39: 7E          ld   a,(hl)
3E3A: 21 5D FB    ld   hl,$FB5D
3E3D: BE          cp   (hl)
3E3E: 77          ld   (hl),a
3E3F: 26 0B       ld   h,$0B
3E41: 6F          ld   l,a
3E42: 7E          ld   a,(hl)
3E43: A7          and  a
3E44: 28 0B       jr   z,$3E51
3E46: D6 01       sub  $01
3E48: 5F          ld   e,a
3E49: 3A 62 FB    ld   a,($FB62)
3E4C: E6 03       and  $03
3E4E: 0F          rrca
3E4F: 0F          rrca
3E50: B3          or   e
3E51: 5F          ld   e,a
3E52: 16 0C       ld   d,$0C
3E54: 0E 06       ld   c,$06
3E56: CF          rst  $08
3E57: 0E 01       ld   c,$01
3E59: FD CB 09 4E bit  1,(iy+$09)
3E5D: 28 04       jr   z,$3E63
3E5F: 3A 09 F8    ld   a,($F809)
3E62: 4F          ld   c,a
3E63: 21 53 FB    ld   hl,$FB53
3E66: 7E          ld   a,(hl)
3E67: 81          add  a,c
3E68: DE 00       sbc  a,$00
3E6A: 77          ld   (hl),a
3E6B: 21 52 FB    ld   hl,$FB52
3E6E: 7E          ld   a,(hl)
3E6F: A7          and  a
3E70: CA 84 3E    jp   z,$3E84
3E73: 91          sub  c
3E74: 77          ld   (hl),a
3E75: 20 22       jr   nz,$3E99
3E77: FD CB 09 66 bit  4,(iy+$09)
3E7B: 28 16       jr   z,$3E93
3E7D: DD CB 03 E6 set  4,(ix+$03)
3E81: C3 99 3E    jp   $3E99
3E84: FD CB 09 66 bit  4,(iy+$09)
3E88: 20 0F       jr   nz,$3E99
3E8A: 21 50 FB    ld   hl,$FB50
3E8D: CB 66       bit  4,(hl)
3E8F: 28 08       jr   z,$3E99
3E91: CB A6       res  4,(hl)
3E93: 21 68 FB    ld   hl,$FB68
3E96: CD CA 21    call $21CA
3E99: DD 21 76 FB ld   ix,$FB76
3E9D: FD 2A 76 FB ld   iy,($FB76)
3EA1: FD CB 09 5E bit  3,(iy+$09)
3EA5: C4 CC 1F    call nz,$1FCC
3EA8: 21 90 FB    ld   hl,$FB90
3EAB: 7E          ld   a,(hl)
3EAC: A7          and  a
3EAD: CA 81 3F    jp   z,$3F81
3EB0: FE 02       cp   $02
3EB2: 28 12       jr   z,$3EC6
3EB4: 3A 0A F8    ld   a,($F80A)
3EB7: E6 20       and  $20
3EB9: CA 02 3F    jp   z,$3F02
3EBC: AF          xor  a
3EBD: 32 90 FB    ld   ($FB90),a
3EC0: 32 81 FB    ld   ($FB81),a
3EC3: C3 81 3F    jp   $3F81
3EC6: 2A 8C FB    ld   hl,($FB8C)
3EC9: ED 4B 8E FB ld   bc,($FB8E)
3ECD: 3E 01       ld   a,$01
3ECF: 32 90 FB    ld   ($FB90),a
3ED2: C5          push bc
3ED3: 11 C0 0D    ld   de,$0DC0
3ED6: 0E 06       ld   c,$06
3ED8: CF          rst  $08
3ED9: AF          xor  a
3EDA: 32 86 FB    ld   ($FB86),a
3EDD: 11 A0 00    ld   de,$00A0
3EE0: CF          rst  $08
3EE1: 11 20 1C    ld   de,$1C20
3EE4: 0E 04       ld   c,$04
3EE6: CF          rst  $08
3EE7: 1E 00       ld   e,$00
3EE9: CF          rst  $08
3EEA: 0E 06       ld   c,$06
3EEC: 5D          ld   e,l
3EED: 16 15       ld   d,$15
3EEF: CF          rst  $08
3EF0: 5C          ld   e,h
3EF1: 16 1D       ld   d,$1D
3EF3: E1          pop  hl
3EF4: CF          rst  $08
3EF5: 5D          ld   e,l
3EF6: 16 25       ld   d,$25
3EF8: CF          rst  $08
3EF9: 5C          ld   e,h
3EFA: 16 2D       ld   d,$2D
3EFC: CF          rst  $08
3EFD: 1E 20       ld   e,$20
3EFF: 16 00       ld   d,$00
3F01: CF          rst  $08
3F02: 21 91 FB    ld   hl,$FB91
3F05: CD DE 21    call $21DE
3F08: 6F          ld   l,a
3F09: 3A 81 FB    ld   a,($FB81)
3F0C: A7          and  a
3F0D: 28 15       jr   z,$3F24
3F0F: 2C          inc  l
3F10: 28 12       jr   z,$3F24
3F12: 08          ex   af,af'
3F13: 2D          dec  l
3F14: 7D          ld   a,l
3F15: 28 0D       jr   z,$3F24
3F17: 26 06       ld   h,$06
3F19: 7E          ld   a,(hl)
3F1A: 08          ex   af,af'
3F1B: 6F          ld   l,a
3F1C: 08          ex   af,af'
3F1D: 86          add  a,(hl)
3F1E: 6F          ld   l,a
3F1F: 9F          sbc  a,a
3F20: 30 02       jr   nc,$3F24
3F22: 24          inc  h
3F23: 7E          ld   a,(hl)
3F24: FD 6E 12    ld   l,(iy+$12)
3F27: A7          and  a
3F28: 28 15       jr   z,$3F3F
3F2A: 2C          inc  l
3F2B: 28 12       jr   z,$3F3F
3F2D: 08          ex   af,af'
3F2E: 2D          dec  l
3F2F: 7D          ld   a,l
3F30: 28 0D       jr   z,$3F3F
3F32: 26 06       ld   h,$06
3F34: 7E          ld   a,(hl)
3F35: 08          ex   af,af'
3F36: 6F          ld   l,a
3F37: 08          ex   af,af'
3F38: 86          add  a,(hl)
3F39: 6F          ld   l,a
3F3A: 9F          sbc  a,a
3F3B: 30 02       jr   nc,$3F3F
3F3D: 24          inc  h
3F3E: 7E          ld   a,(hl)
3F3F: 6F          ld   l,a
3F40: 3E FF       ld   a,$FF
3F42: FD CB 09 4E bit  1,(iy+$09)
3F46: CA 4C 3F    jp   z,$3F4C
3F49: 3A 05 F8    ld   a,($F805)
3F4C: A7          and  a
3F4D: 28 15       jr   z,$3F64
3F4F: 2C          inc  l
3F50: 28 12       jr   z,$3F64
3F52: 08          ex   af,af'
3F53: 2D          dec  l
3F54: 7D          ld   a,l
3F55: 28 0D       jr   z,$3F64
3F57: 26 06       ld   h,$06
3F59: 7E          ld   a,(hl)
3F5A: 08          ex   af,af'
3F5B: 6F          ld   l,a
3F5C: 08          ex   af,af'
3F5D: 86          add  a,(hl)
3F5E: 6F          ld   l,a
3F5F: 9F          sbc  a,a
3F60: 30 02       jr   nc,$3F64
3F62: 24          inc  h
3F63: 7E          ld   a,(hl)
3F64: 21 86 FB    ld   hl,$FB86
3F67: BE          cp   (hl)
3F68: 77          ld   (hl),a
3F69: 26 0B       ld   h,$0B
3F6B: 6F          ld   l,a
3F6C: 7E          ld   a,(hl)
3F6D: A7          and  a
3F6E: 28 0B       jr   z,$3F7B
3F70: D6 01       sub  $01
3F72: 5F          ld   e,a
3F73: 3A 8B FB    ld   a,($FB8B)
3F76: E6 03       and  $03
3F78: 0F          rrca
3F79: 0F          rrca
3F7A: B3          or   e
3F7B: 5F          ld   e,a
3F7C: 16 0D       ld   d,$0D
3F7E: 0E 06       ld   c,$06
3F80: CF          rst  $08
3F81: 0E 01       ld   c,$01
3F83: FD CB 09 4E bit  1,(iy+$09)
3F87: 28 04       jr   z,$3F8D
3F89: 3A 09 F8    ld   a,($F809)
3F8C: 4F          ld   c,a
3F8D: 21 7C FB    ld   hl,$FB7C
3F90: 7E          ld   a,(hl)
3F91: 81          add  a,c
3F92: DE 00       sbc  a,$00
3F94: 77          ld   (hl),a
3F95: 21 7B FB    ld   hl,$FB7B
3F98: 7E          ld   a,(hl)
3F99: A7          and  a
3F9A: CA AE 3F    jp   z,$3FAE
3F9D: 91          sub  c
3F9E: 77          ld   (hl),a
3F9F: 20 22       jr   nz,$3FC3
3FA1: FD CB 09 66 bit  4,(iy+$09)
3FA5: 28 16       jr   z,$3FBD
3FA7: DD CB 03 E6 set  4,(ix+$03)
3FAB: C3 C3 3F    jp   $3FC3
3FAE: FD CB 09 66 bit  4,(iy+$09)
3FB2: 20 0F       jr   nz,$3FC3
3FB4: 21 79 FB    ld   hl,$FB79
3FB7: CB 66       bit  4,(hl)
3FB9: 28 08       jr   z,$3FC3
3FBB: CB A6       res  4,(hl)
3FBD: 21 91 FB    ld   hl,$FB91
3FC0: CD CA 21    call $21CA
3FC3: C9          ret
