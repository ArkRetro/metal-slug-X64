405A: DD 21 9F FB ld   ix,$FB9F
405E: CD 70 40    call $4070
4061: 0E 04       ld   c,$04
4063: 11 00 1B    ld   de,$1B00
4066: CF          rst  $08
4067: 11 01 10    ld   de,$1001
406A: CF          rst  $08
406B: 11 00 11    ld   de,$1100
406E: CF          rst  $08
406F: C9          ret
4070: DD 21 9F FB ld   ix,$FB9F
4074: CD 96 1F    call $1F96
4077: DD 36 02 03 ld   (ix+$02),$03
407B: AF          xor  a
407C: 32 C0 FB    ld   ($FBC0),a
407F: 32 C1 FB    ld   ($FBC1),a
4082: 32 BE FB    ld   ($FBBE),a
4085: DD E5       push ix
4087: 11 23 00    ld   de,$0023
408A: DD 19       add  ix,de
408C: F7          rst  $30
408D: DD E1       pop  ix
408F: DD E5       push ix
4091: 11 3F 00    ld   de,$003F
4094: DD 19       add  ix,de
4096: CD 4E 22    call $224E
4099: DD E1       pop  ix
409B: DD E5       push ix
409D: 11 31 00    ld   de,$0031
40A0: DD 19       add  ix,de
40A2: CD 4E 22    call $224E
40A5: DD E1       pop  ix
40A7: DD E5       push ix
40A9: 11 4D 00    ld   de,$004D
40AC: DD 19       add  ix,de
40AE: CD 9F 23    call $239F
40B1: DD E1       pop  ix
40B3: C9          ret
40B4: DD 21 9F FB ld   ix,$FB9F
40B8: FD 2A 9F FB ld   iy,($FB9F)
40BC: FD CB 09 5E bit  3,(iy+$09)
40C0: C4 CC 1F    call nz,$1FCC
40C3: 3A BE FB    ld   a,($FBBE)
40C6: A7          and  a
40C7: CA D5 42    jp   z,$42D5
40CA: FE 04       cp   $04
40CC: CA E4 40    jp   z,$40E4
40CF: 08          ex   af,af'
40D0: 3A 0A F8    ld   a,($F80A)
40D3: E6 80       and  $80
40D5: CA A3 41    jp   z,$41A3
40D8: 08          ex   af,af'
40D9: 3D          dec  a
40DA: CA 93 41    jp   z,$4193
40DD: 3D          dec  a
40DE: CA 13 41    jp   z,$4113
40E1: C3 F9 40    jp   $40F9
40E4: 2A B6 FB    ld   hl,($FBB6)
40E7: ED 4B B8 FB ld   bc,($FBB8)
40EB: 78          ld   a,b
40EC: B1          or   c
40ED: CA 01 41    jp   z,$4101
40F0: 0B          dec  bc
40F1: 3E 03       ld   a,$03
40F3: 32 BE FB    ld   ($FBBE),a
40F6: C3 49 41    jp   $4149
40F9: 2A B6 FB    ld   hl,($FBB6)
40FC: ED 4B B8 FB ld   bc,($FBB8)
4100: 09          add  hl,bc
4101: ED 4B BA FB ld   bc,($FBBA)
4105: 78          ld   a,b
4106: B1          or   c
4107: CA 3B 41    jp   z,$413B
410A: 0B          dec  bc
410B: 3E 02       ld   a,$02
410D: 32 BE FB    ld   ($FBBE),a
4110: C3 49 41    jp   $4149
4113: 3A BF FB    ld   a,($FBBF)
4116: FE FF       cp   $FF
4118: CA A3 41    jp   z,$41A3
411B: A7          and  a
411C: 28 10       jr   z,$412E
411E: 3D          dec  a
411F: 32 BF FB    ld   ($FBBF),a
4122: 0E 04       ld   c,$04
4124: 11 80 1C    ld   de,$1C80
4127: CF          rst  $08
4128: 1E 00       ld   e,$00
412A: CF          rst  $08
412B: C3 A3 41    jp   $41A3
412E: 2A B6 FB    ld   hl,($FBB6)
4131: ED 4B B8 FB ld   bc,($FBB8)
4135: 09          add  hl,bc
4136: ED 4B BA FB ld   bc,($FBBA)
413A: 09          add  hl,bc
413B: ED 4B BC FB ld   bc,($FBBC)
413F: 78          ld   a,b
4140: B1          or   c
4141: CA 93 41    jp   z,$4193
4144: 3E 01       ld   a,$01
4146: 32 BE FB    ld   ($FBBE),a
4149: C5          push bc
414A: 11 01 10    ld   de,$1001
414D: 0E 04       ld   c,$04
414F: CF          rst  $08
4150: 11 00 11    ld   de,$1100
4153: CF          rst  $08
4154: 11 00 1B    ld   de,$1B00
4157: CF          rst  $08
4158: AF          xor  a
4159: 32 AF FB    ld   ($FBAF),a
415C: 32 C1 FB    ld   ($FBC1),a
415F: 5D          ld   e,l
4160: 16 12       ld   d,$12
4162: CF          rst  $08
4163: 5C          ld   e,h
4164: 14          inc  d
4165: CF          rst  $08
4166: C1          pop  bc
4167: 09          add  hl,bc
4168: 5D          ld   e,l
4169: 14          inc  d
416A: 0E 04       ld   c,$04
416C: CF          rst  $08
416D: 5C          ld   e,h
416E: 14          inc  d
416F: CF          rst  $08
4170: 11 80 1C    ld   de,$1C80
4173: CF          rst  $08
4174: 3A 0A F8    ld   a,($F80A)
4177: E6 7F       and  $7F
4179: 32 0A F8    ld   ($F80A),a
417C: 11 00 1C    ld   de,$1C00
417F: CF          rst  $08
4180: 11 00 10    ld   de,$1000
4183: CF          rst  $08
4184: 1E 80       ld   e,$80
4186: 3A BE FB    ld   a,($FBBE)
4189: FE 02       cp   $02
418B: 20 02       jr   nz,$418F
418D: 1E 90       ld   e,$90
418F: CF          rst  $08
4190: C3 A3 41    jp   $41A3
4193: 11 00 1B    ld   de,$1B00
4196: 0E 04       ld   c,$04
4198: CF          rst  $08
4199: AF          xor  a
419A: 32 BE FB    ld   ($FBBE),a
419D: 32 A4 FB    ld   ($FBA4),a
41A0: C3 D5 42    jp   $42D5
41A3: 3A C0 FB    ld   a,($FBC0)
41A6: A7          and  a
41A7: 20 03       jr   nz,$41AC
41A9: FD 7E 0F    ld   a,(iy+$0f)
41AC: 0F          rrca
41AD: 0F          rrca
41AE: 21 C1 FB    ld   hl,$FBC1
41B1: BE          cp   (hl)
41B2: 28 07       jr   z,$41BB
41B4: 77          ld   (hl),a
41B5: 5F          ld   e,a
41B6: 16 11       ld   d,$11
41B8: 0E 04       ld   c,$04
41BA: CF          rst  $08
41BB: FD 6E 10    ld   l,(iy+$10)
41BE: FD 66 11    ld   h,(iy+$11)
41C1: ED 5B A8 FB ld   de,($FBA8)
41C5: 19          add  hl,de
41C6: 22 0F F8    ld   ($F80F),hl
41C9: 21 DE FB    ld   hl,$FBDE
41CC: FD 56 0C    ld   d,(iy+$0c)
41CF: CD CF 22    call $22CF
41D2: 6F          ld   l,a
41D3: 26 00       ld   h,$00
41D5: 44          ld   b,h
41D6: 29          add  hl,hl
41D7: A7          and  a
41D8: ED 42       sbc  hl,bc
41DA: ED 5B 0F F8 ld   de,($F80F)
41DE: 19          add  hl,de
41DF: 22 0F F8    ld   ($F80F),hl
41E2: 21 EC FB    ld   hl,$FBEC
41E5: CD 3F 24    call $243F
41E8: ED 5B 0F F8 ld   de,($F80F)
41EC: 19          add  hl,de
41ED: 4D          ld   c,l
41EE: 44          ld   b,h
41EF: 2A AD FB    ld   hl,($FBAD)
41F2: A7          and  a
41F3: ED 42       sbc  hl,bc
41F5: 28 3E       jr   z,$4235
41F7: ED 43 AD FB ld   ($FBAD),bc
41FB: ED 5B B3 FB ld   de,($FBB3)
41FF: 26 09       ld   h,$09
4201: 69          ld   l,c
4202: 4E          ld   c,(hl)
4203: D5          push de
4204: CD 6D 1F    call $1F6D
4207: D1          pop  de
4208: 19          add  hl,de
4209: 30 05       jr   nc,$4210
420B: CB 1C       rr   h
420D: CB 1D       rr   l
420F: 04          inc  b
4210: 78          ld   a,b
4211: A7          and  a
4212: CA 2C 42    jp   z,$422C
4215: F2 23 42    jp   p,$4223
4218: CB 3C       srl  h
421A: CB 1D       rr   l
421C: 3C          inc  a
421D: C2 18 42    jp   nz,$4218
4220: C3 2C 42    jp   $422C
4223: 29          add  hl,hl
4224: 30 02       jr   nc,$4228
4226: ED 62       sbc  hl,hl
4228: 3D          dec  a
4229: C2 23 42    jp   nz,$4223
422C: 5D          ld   e,l
422D: 16 19       ld   d,$19
422F: 0E 04       ld   c,$04
4231: CF          rst  $08
4232: 14          inc  d
4233: 5C          ld   e,h
4234: CF          rst  $08
4235: 21 C2 FB    ld   hl,$FBC2
4238: CD DE 21    call $21DE
423B: 32 11 F8    ld   ($F811),a
423E: FD 56 0D    ld   d,(iy+$0d)
4241: 21 D0 FB    ld   hl,$FBD0
4244: CD CF 22    call $22CF
4247: 91          sub  c
4248: 3D          dec  a
4249: 6F          ld   l,a
424A: 3A 11 F8    ld   a,($F811)
424D: A7          and  a
424E: 28 15       jr   z,$4265
4250: 2C          inc  l
4251: 28 12       jr   z,$4265
4253: 08          ex   af,af'
4254: 2D          dec  l
4255: 7D          ld   a,l
4256: 28 0D       jr   z,$4265
4258: 26 06       ld   h,$06
425A: 7E          ld   a,(hl)
425B: 08          ex   af,af'
425C: 6F          ld   l,a
425D: 08          ex   af,af'
425E: 86          add  a,(hl)
425F: 6F          ld   l,a
4260: 9F          sbc  a,a
4261: 30 02       jr   nc,$4265
4263: 24          inc  h
4264: 7E          ld   a,(hl)
4265: 6F          ld   l,a
4266: 3A AA FB    ld   a,($FBAA)
4269: A7          and  a
426A: 28 15       jr   z,$4281
426C: 2C          inc  l
426D: 28 12       jr   z,$4281
426F: 08          ex   af,af'
4270: 2D          dec  l
4271: 7D          ld   a,l
4272: 28 0D       jr   z,$4281
4274: 26 06       ld   h,$06
4276: 7E          ld   a,(hl)
4277: 08          ex   af,af'
4278: 6F          ld   l,a
4279: 08          ex   af,af'
427A: 86          add  a,(hl)
427B: 6F          ld   l,a
427C: 9F          sbc  a,a
427D: 30 02       jr   nc,$4281
427F: 24          inc  h
4280: 7E          ld   a,(hl)
4281: FD 6E 12    ld   l,(iy+$12)
4284: A7          and  a
4285: 28 15       jr   z,$429C
4287: 2C          inc  l
4288: 28 12       jr   z,$429C
428A: 08          ex   af,af'
428B: 2D          dec  l
428C: 7D          ld   a,l
428D: 28 0D       jr   z,$429C
428F: 26 06       ld   h,$06
4291: 7E          ld   a,(hl)
4292: 08          ex   af,af'
4293: 6F          ld   l,a
4294: 08          ex   af,af'
4295: 86          add  a,(hl)
4296: 6F          ld   l,a
4297: 9F          sbc  a,a
4298: 30 02       jr   nc,$429C
429A: 24          inc  h
429B: 7E          ld   a,(hl)
429C: 6F          ld   l,a
429D: 3E FF       ld   a,$FF
429F: FD CB 09 4E bit  1,(iy+$09)
42A3: CA A9 42    jp   z,$42A9
42A6: 3A 05 F8    ld   a,($F805)
42A9: A7          and  a
42AA: 28 15       jr   z,$42C1
42AC: 2C          inc  l
42AD: 28 12       jr   z,$42C1
42AF: 08          ex   af,af'
42B0: 2D          dec  l
42B1: 7D          ld   a,l
42B2: 28 0D       jr   z,$42C1
42B4: 26 06       ld   h,$06
42B6: 7E          ld   a,(hl)
42B7: 08          ex   af,af'
42B8: 6F          ld   l,a
42B9: 08          ex   af,af'
42BA: 86          add  a,(hl)
42BB: 6F          ld   l,a
42BC: 9F          sbc  a,a
42BD: 30 02       jr   nc,$42C1
42BF: 24          inc  h
42C0: 7E          ld   a,(hl)
42C1: 21 AF FB    ld   hl,$FBAF
42C4: BE          cp   (hl)
42C5: 77          ld   (hl),a
42C6: 28 0D       jr   z,$42D5
42C8: 87          add  a,a
42C9: 1E 80       ld   e,$80
42CB: 83          add  a,e
42CC: 8B          adc  a,e
42CD: 5F          ld   e,a
42CE: 16 1B       ld   d,$1B
42D0: 0E 04       ld   c,$04
42D2: 0E 04       ld   c,$04
42D4: CF          rst  $08
42D5: 0E 01       ld   c,$01
42D7: FD CB 09 4E bit  1,(iy+$09)
42DB: 28 04       jr   z,$42E1
42DD: 3A 09 F8    ld   a,($F809)
42E0: 4F          ld   c,a
42E1: 21 A5 FB    ld   hl,$FBA5
42E4: 7E          ld   a,(hl)
42E5: 81          add  a,c
42E6: DE 00       sbc  a,$00
42E8: 77          ld   (hl),a
42E9: 21 A4 FB    ld   hl,$FBA4
42EC: 7E          ld   a,(hl)
42ED: A7          and  a
42EE: CA 02 43    jp   z,$4302
42F1: 91          sub  c
42F2: 77          ld   (hl),a
42F3: 20 22       jr   nz,$4317
42F5: FD CB 09 66 bit  4,(iy+$09)
42F9: 28 16       jr   z,$4311
42FB: DD CB 03 E6 set  4,(ix+$03)
42FF: C3 17 43    jp   $4317
4302: FD CB 09 66 bit  4,(iy+$09)
4306: 20 0F       jr   nz,$4317
4308: 21 A2 FB    ld   hl,$FBA2
430B: CB 66       bit  4,(hl)
430D: 28 08       jr   z,$4317
430F: CB A6       res  4,(hl)
4311: 21 C2 FB    ld   hl,$FBC2
4314: CD CA 21    call $21CA
4317: C9          ret
