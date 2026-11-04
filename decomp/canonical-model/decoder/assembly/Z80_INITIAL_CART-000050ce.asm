50CE: C9          ret
50CF: FD 21 F6 FD ld   iy,$FDF6
50D3: 06 08       ld   b,$08
50D5: 4B          ld   c,e
50D6: 26 00       ld   h,$00
50D8: FD CB 09 56 bit  2,(iy+$09)
50DC: CA 27 51    jp   z,$5127
50DF: 7A          ld   a,d
50E0: A7          and  a
50E1: 28 2C       jr   z,$510F
50E3: FD BE 03    cp   (iy+$03)
50E6: C2 0F 51    jp   nz,$510F
50E9: D9          exx
50EA: FD 5E 07    ld   e,(iy+$07)
50ED: FD 56 08    ld   d,(iy+$08)
50F0: 2A 16 F8    ld   hl,($F816)
50F3: A7          and  a
50F4: ED 52       sbc  hl,de
50F6: D9          exx
50F7: 28 16       jr   z,$510F
50F9: FD 7E 02    ld   a,(iy+$02)
50FC: BB          cp   e
50FD: 30 04       jr   nc,$5103
50FF: 26 00       ld   h,$00
5101: 18 3B       jr   $513E
5103: C5          push bc
5104: D5          push de
5105: E5          push hl
5106: CD 81 57    call $5781
5109: E1          pop  hl
510A: D1          pop  de
510B: C1          pop  bc
510C: C3 27 51    jp   $5127
510F: 7C          ld   a,h
5110: FE 02       cp   $02
5112: CA 35 51    jp   z,$5135
5115: FD 7E 02    ld   a,(iy+$02)
5118: B9          cp   c
5119: 38 1A       jr   c,$5135
511B: FD 4E 02    ld   c,(iy+$02)
511E: 26 01       ld   h,$01
5120: FD 22 0E FF ld   ($FF0E),iy
5124: C3 35 51    jp   $5135
5127: 26 02       ld   h,$02
5129: FD 22 0E FF ld   ($FF0E),iy
512D: 7A          ld   a,d
512E: A7          and  a
512F: CA 3E 51    jp   z,$513E
5132: C3 35 51    jp   $5135
5135: D9          exx
5136: 11 23 00    ld   de,$0023
5139: FD 19       add  iy,de
513B: D9          exx
513C: 10 9A       djnz $50D8
513E: FD 2A 0E FF ld   iy,($FF0E)
5142: C9          ret
5143: 21 FF FD    ld   hl,$FDFF
5146: 7E          ld   a,(hl)
5147: E6 F7       and  $F7
5149: 77          ld   (hl),a
514A: CB 57       bit  2,a
514C: CA 5B 51    jp   z,$515B
514F: FD 21 F6 FD ld   iy,$FDF6
5153: CB 77       bit  6,a
5155: C4 DE 52    call nz,$52DE
5158: CD 1F 53    call $531F
515B: 21 22 FE    ld   hl,$FE22
515E: 7E          ld   a,(hl)
515F: E6 F7       and  $F7
5161: 77          ld   (hl),a
5162: CB 57       bit  2,a
5164: CA 73 51    jp   z,$5173
5167: FD 21 19 FE ld   iy,$FE19
516B: CB 77       bit  6,a
516D: C4 DE 52    call nz,$52DE
5170: CD 1F 53    call $531F
5173: 21 45 FE    ld   hl,$FE45
5176: 7E          ld   a,(hl)
5177: E6 F7       and  $F7
5179: 77          ld   (hl),a
517A: CB 57       bit  2,a
517C: CA 8B 51    jp   z,$518B
517F: FD 21 3C FE ld   iy,$FE3C
5183: CB 77       bit  6,a
5185: C4 DE 52    call nz,$52DE
5188: CD 1F 53    call $531F
518B: 21 68 FE    ld   hl,$FE68
518E: 7E          ld   a,(hl)
518F: E6 F7       and  $F7
5191: 77          ld   (hl),a
5192: CB 57       bit  2,a
5194: CA A3 51    jp   z,$51A3
5197: FD 21 5F FE ld   iy,$FE5F
519B: CB 77       bit  6,a
519D: C4 DE 52    call nz,$52DE
51A0: CD 1F 53    call $531F
51A3: 21 8B FE    ld   hl,$FE8B
51A6: 7E          ld   a,(hl)
51A7: E6 F7       and  $F7
51A9: 77          ld   (hl),a
51AA: CB 57       bit  2,a
51AC: CA BB 51    jp   z,$51BB
51AF: FD 21 82 FE ld   iy,$FE82
51B3: CB 77       bit  6,a
51B5: C4 DE 52    call nz,$52DE
51B8: CD 1F 53    call $531F
51BB: 21 AE FE    ld   hl,$FEAE
51BE: 7E          ld   a,(hl)
51BF: E6 F7       and  $F7
51C1: 77          ld   (hl),a
51C2: CB 57       bit  2,a
51C4: CA D3 51    jp   z,$51D3
51C7: FD 21 A5 FE ld   iy,$FEA5
51CB: CB 77       bit  6,a
51CD: C4 DE 52    call nz,$52DE
51D0: CD 1F 53    call $531F
51D3: 21 D1 FE    ld   hl,$FED1
51D6: 7E          ld   a,(hl)
51D7: E6 F7       and  $F7
51D9: 77          ld   (hl),a
51DA: CB 57       bit  2,a
51DC: CA EB 51    jp   z,$51EB
51DF: FD 21 C8 FE ld   iy,$FEC8
51E3: CB 77       bit  6,a
51E5: C4 DE 52    call nz,$52DE
51E8: CD 1F 53    call $531F
51EB: 21 F4 FE    ld   hl,$FEF4
51EE: 7E          ld   a,(hl)
51EF: E6 F7       and  $F7
51F1: 77          ld   (hl),a
51F2: CB 57       bit  2,a
51F4: CA 03 52    jp   z,$5203
51F7: FD 21 EB FE ld   iy,$FEEB
51FB: CB 77       bit  6,a
51FD: C4 DE 52    call nz,$52DE
5200: CD 1F 53    call $531F
5203: C9          ret
5204: 79          ld   a,c
5205: B0          or   b
5206: C8          ret  z
5207: DD E5       push ix
5209: DD 21 F6 FD ld   ix,$FDF6
520D: 3E 08       ld   a,$08
520F: DD 6E 05    ld   l,(ix+$05)
5212: DD 66 06    ld   h,(ix+$06)
5215: A7          and  a
5216: ED 42       sbc  hl,bc
5218: C2 3A 52    jp   nz,$523A
521B: 2A 16 F8    ld   hl,($F816)
521E: DD 5E 07    ld   e,(ix+$07)
5221: DD 56 08    ld   d,(ix+$08)
5224: A7          and  a
5225: ED 52       sbc  hl,de
5227: CA 3A 52    jp   z,$523A
522A: DD 36 09 08 ld   (ix+$09),$08
522E: DD 36 00 00 ld   (ix+$00),$00
5232: DD 36 01 00 ld   (ix+$01),$00
5236: DD 36 02 FF ld   (ix+$02),$FF
523A: 11 23 00    ld   de,$0023
523D: DD 19       add  ix,de
523F: 3D          dec  a
5240: 20 CD       jr   nz,$520F
5242: DD E1       pop  ix
5244: C9          ret
5245: DD 5D       ld   e,ixl
5247: DD 54       ld   d,ixh
5249: EB          ex   de,hl
524A: 3A 13 FF    ld   a,($FF13)
524D: 47          ld   b,a
524E: C5          push bc
524F: CD BE 57    call $57BE
5252: FD 75 14    ld   (iy+$14),l
5255: FD 74 15    ld   (iy+$15),h
5258: FD 70 04    ld   (iy+$04),b
525B: C1          pop  bc
525C: 3A 13 FF    ld   a,($FF13)
525F: B8          cp   b
5260: CA 7D 52    jp   z,$527D
5263: 78          ld   a,b
5264: 32 13 FF    ld   ($FF13),a
5267: 0E 0B       ld   c,$0B
5269: ED 78       in   a,(c)
526B: 04          inc  b
526C: CB 20       sla  b
526E: 0D          dec  c
526F: ED 78       in   a,(c)
5271: 04          inc  b
5272: CB 20       sla  b
5274: 0D          dec  c
5275: ED 78       in   a,(c)
5277: 04          inc  b
5278: CB 20       sla  b
527A: 0D          dec  c
527B: ED 78       in   a,(c)
527D: AF          xor  a
527E: FD 77 03    ld   (iy+$03),a
5281: FD 77 09    ld   (iy+$09),a
5284: FD CB 09 D6 set  2,(iy+$09)
5288: FD CB 09 DE set  3,(iy+$09)
528C: FD 77 00    ld   (iy+$00),a
528F: FD 77 01    ld   (iy+$01),a
5292: FD 36 0A FF ld   (iy+$0a),$FF
5296: FD 36 0B FF ld   (iy+$0b),$FF
529A: FD 77 16    ld   (iy+$16),a
529D: FD 77 0C    ld   (iy+$0c),a
52A0: FD 77 0D    ld   (iy+$0d),a
52A3: FD 77 0E    ld   (iy+$0e),a
52A6: FD 77 10    ld   (iy+$10),a
52A9: FD 77 11    ld   (iy+$11),a
52AC: FD 36 12 FF ld   (iy+$12),$FF
52B0: FD 36 0F 03 ld   (iy+$0f),$03
52B4: FD 77 13    ld   (iy+$13),a
52B7: FD 36 22 06 ld   (iy+$22),$06
52BB: 2A 16 F8    ld   hl,($F816)
52BE: FD 75 07    ld   (iy+$07),l
52C1: FD 74 08    ld   (iy+$08),h
52C4: 2A 18 F8    ld   hl,($F818)
52C7: FD 75 05    ld   (iy+$05),l
52CA: FD 74 06    ld   (iy+$06),h
52CD: FD 36 17 40 ld   (iy+$17),$40
52D1: FD 36 18 C0 ld   (iy+$18),$C0
52D5: FD 36 19 C0 ld   (iy+$19),$C0
52D9: FD 36 1A 7F ld   (iy+$1a),$7F
52DD: C9          ret
52DE: FD CB 09 76 bit  6,(iy+$09)
52E2: C8          ret  z
52E3: FD 7E 10    ld   a,(iy+$10)
52E6: FD B6 11    or   (iy+$11)
52E9: C8          ret  z
52EA: FD 7E 13    ld   a,(iy+$13)
52ED: E6 7F       and  $7F
52EF: 87          add  a,a
52F0: 26 0C       ld   h,$0C
52F2: 6F          ld   l,a
52F3: 5E          ld   e,(hl)
52F4: 23          inc  hl
52F5: 56          ld   d,(hl)
52F6: FD 6E 10    ld   l,(iy+$10)
52F9: FD 66 11    ld   h,(iy+$11)
52FC: 7C          ld   a,h
52FD: A7          and  a
52FE: F2 0C 53    jp   p,$530C
5301: ED 5A       adc  hl,de
5303: 38 12       jr   c,$5317
5305: FD 75 10    ld   (iy+$10),l
5308: FD 74 11    ld   (iy+$11),h
530B: C9          ret
530C: ED 52       sbc  hl,de
530E: 38 07       jr   c,$5317
5310: FD 75 10    ld   (iy+$10),l
5313: FD 74 11    ld   (iy+$11),h
5316: C9          ret
5317: AF          xor  a
5318: FD 77 10    ld   (iy+$10),a
531B: FD 77 11    ld   (iy+$11),a
531E: C9          ret
531F: FD 7E 16    ld   a,(iy+$16)
5322: A7          and  a
5323: C2 41 53    jp   nz,$5341
5326: FD 6E 14    ld   l,(iy+$14)
5329: FD 66 15    ld   h,(iy+$15)
532C: 22 11 FF    ld   ($FF11),hl
532F: C3 56 53    jp   $5356
5332: FD 7E 16    ld   a,(iy+$16)
5335: A7          and  a
5336: 28 1E       jr   z,$5356
5338: 2A 11 FF    ld   hl,($FF11)
533B: FD 75 14    ld   (iy+$14),l
533E: FD 74 15    ld   (iy+$15),h
5341: 3D          dec  a
5342: FD 77 16    ld   (iy+$16),a
5345: FD CB 09 4E bit  1,(iy+$09)
5349: C0          ret  nz
534A: FD CB 09 6E bit  5,(iy+$09)
534E: C8          ret  z
534F: CD 09 4B    call $4B09
5352: D8          ret  c
5353: C3 7A 57    jp   $577A
5356: FD 46 04    ld   b,(iy+$04)
5359: 3A 13 FF    ld   a,($FF13)
535C: B8          cp   b
535D: CA 7A 53    jp   z,$537A
5360: 78          ld   a,b
5361: 32 13 FF    ld   ($FF13),a
5364: 0E 0B       ld   c,$0B
5366: ED 78       in   a,(c)
5368: 04          inc  b
5369: CB 20       sla  b
536B: 0D          dec  c
536C: ED 78       in   a,(c)
536E: 04          inc  b
536F: CB 20       sla  b
5371: 0D          dec  c
5372: ED 78       in   a,(c)
5374: 04          inc  b
5375: CB 20       sla  b
5377: 0D          dec  c
5378: ED 78       in   a,(c)
537A: ED 5B 11 FF ld   de,($FF11)
537E: 1A          ld   a,(de)
537F: 13          inc  de
5380: FE DA       cp   $DA
5382: D2 81 57    jp   nc,$5781
5385: FE C0       cp   $C0
5387: DA DE 56    jp   c,$56DE
538A: D6 C0       sub  $C0
538C: 6F          ld   l,a
538D: 87          add  a,a
538E: 85          add  a,l
538F: 6F          ld   l,a
5390: 26 00       ld   h,$00
5392: 01 97 53    ld   bc,$5397
5395: 09          add  hl,bc
5396: E9          jp   (hl)
5397: C3 A5 56    jp   $56A5
539A: C3 B1 56    jp   $56B1
539D: C3 C2 56    jp   $56C2
53A0: C3 8E 56    jp   $568E
53A3: C3 51 56    jp   $5651
53A6: C3 92 55    jp   $5592
53A9: C3 68 55    jp   $5568
53AC: C3 76 55    jp   $5576
53AF: C3 84 55    jp   $5584
53B2: C3 50 55    jp   $5550
53B5: C3 35 55    jp   $5535
53B8: C3 29 55    jp   $5529
53BB: C3 11 55    jp   $5511
53BE: C3 03 55    jp   $5503
53C1: C3 F0 54    jp   $54F0
53C4: C3 C8 55    jp   $55C8
53C7: C3 F9 55    jp   $55F9
53CA: C3 B9 54    jp   $54B9
53CD: C3 E4 54    jp   $54E4
53D0: C3 14 54    jp   $5414
53D3: C3 28 54    jp   $5428
53D6: C3 3C 54    jp   $543C
53D9: C3 50 54    jp   $5450
53DC: C3 64 54    jp   $5464
53DF: C3 8D 54    jp   $548D
53E2: C3 E8 53    jp   $53E8
