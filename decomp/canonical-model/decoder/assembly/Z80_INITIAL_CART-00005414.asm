5414: EB          ex   de,hl
5415: 4E          ld   c,(hl)
5416: 23          inc  hl
5417: 7E          ld   a,(hl)
5418: FD 77 16    ld   (iy+$16),a
541B: 23          inc  hl
541C: 22 11 FF    ld   ($FF11),hl
541F: CD 32 4B    call $4B32
5422: D2 7A 57    jp   nc,$577A
5425: C3 32 53    jp   $5332
5428: EB          ex   de,hl
5429: 4E          ld   c,(hl)
542A: 23          inc  hl
542B: 7E          ld   a,(hl)
542C: FD 77 16    ld   (iy+$16),a
542F: 23          inc  hl
5430: 22 11 FF    ld   ($FF11),hl
5433: CD 4D 4B    call $4B4D
5436: D2 7A 57    jp   nc,$577A
5439: C3 32 53    jp   $5332
543C: EB          ex   de,hl
543D: 4E          ld   c,(hl)
543E: 23          inc  hl
543F: 7E          ld   a,(hl)
5440: FD 77 16    ld   (iy+$16),a
5443: 23          inc  hl
5444: 22 11 FF    ld   ($FF11),hl
5447: CD 68 4B    call $4B68
544A: D2 7A 57    jp   nc,$577A
544D: C3 32 53    jp   $5332
5450: EB          ex   de,hl
5451: 4E          ld   c,(hl)
5452: 23          inc  hl
5453: 7E          ld   a,(hl)
5454: FD 77 16    ld   (iy+$16),a
5457: 23          inc  hl
5458: 22 11 FF    ld   ($FF11),hl
545B: CD 76 4B    call $4B76
545E: D2 7A 57    jp   nc,$577A
5461: C3 32 53    jp   $5332
5464: EB          ex   de,hl
5465: 4E          ld   c,(hl)
5466: 23          inc  hl
5467: 7E          ld   a,(hl)
5468: FD 77 16    ld   (iy+$16),a
546B: 23          inc  hl
546C: 22 11 FF    ld   ($FF11),hl
546F: 06 00       ld   b,$00
5471: FD 6E 10    ld   l,(iy+$10)
5474: FD 66 11    ld   h,(iy+$11)
5477: 11 00 10    ld   de,$1000
547A: A7          and  a
547B: ED 52       sbc  hl,de
547D: 09          add  hl,bc
547E: 30 03       jr   nc,$5483
5480: 21 FF FF    ld   hl,$FFFF
5483: 19          add  hl,de
5484: FD 75 10    ld   (iy+$10),l
5487: FD 74 11    ld   (iy+$11),h
548A: C3 32 53    jp   $5332
548D: EB          ex   de,hl
548E: 4E          ld   c,(hl)
548F: 23          inc  hl
5490: 7E          ld   a,(hl)
5491: FD 77 16    ld   (iy+$16),a
5494: 23          inc  hl
5495: 22 11 FF    ld   ($FF11),hl
5498: 06 00       ld   b,$00
549A: FD 6E 10    ld   l,(iy+$10)
549D: FD 66 11    ld   h,(iy+$11)
54A0: 11 00 10    ld   de,$1000
54A3: 19          add  hl,de
54A4: A7          and  a
54A5: ED 42       sbc  hl,bc
54A7: D2 AD 54    jp   nc,$54AD
54AA: 21 00 00    ld   hl,$0000
54AD: A7          and  a
54AE: ED 52       sbc  hl,de
54B0: FD 75 10    ld   (iy+$10),l
54B3: FD 74 11    ld   (iy+$11),h
54B6: C3 32 53    jp   $5332
54B9: EB          ex   de,hl
54BA: 7E          ld   a,(hl)
54BB: 23          inc  hl
54BC: 22 11 FF    ld   ($FF11),hl
54BF: FE 40       cp   $40
54C1: 38 13       jr   c,$54D6
54C3: FD 4E 09    ld   c,(iy+$09)
54C6: FD CB 09 F6 set  6,(iy+$09)
54CA: CB 71       bit  6,c
54CC: C2 32 53    jp   nz,$5332
54CF: FD 36 1B FF ld   (iy+$1b),$FF
54D3: C3 32 53    jp   $5332
54D6: FD CB 09 B6 res  6,(iy+$09)
54DA: AF          xor  a
54DB: FD 77 10    ld   (iy+$10),a
54DE: FD 77 11    ld   (iy+$11),a
54E1: C3 32 53    jp   $5332
54E4: EB          ex   de,hl
54E5: 7E          ld   a,(hl)
54E6: 23          inc  hl
54E7: 22 11 FF    ld   ($FF11),hl
54EA: FD 77 13    ld   (iy+$13),a
54ED: C3 32 53    jp   $5332
54F0: EB          ex   de,hl
54F1: 7E          ld   a,(hl)
54F2: E6 7F       and  $7F
54F4: 87          add  a,a
54F5: C6 80       add  a,$80
54F7: CE 80       adc  a,$80
54F9: FD 77 12    ld   (iy+$12),a
54FC: 23          inc  hl
54FD: 22 11 FF    ld   ($FF11),hl
5500: C3 32 53    jp   $5332
5503: EB          ex   de,hl
5504: 46          ld   b,(hl)
5505: 23          inc  hl
5506: 7E          ld   a,(hl)
5507: 23          inc  hl
5508: 22 11 FF    ld   ($FF11),hl
550B: CD 60 4F    call $4F60
550E: C3 32 53    jp   $5332
5511: EB          ex   de,hl
5512: 7E          ld   a,(hl)
5513: 23          inc  hl
5514: 22 11 FF    ld   ($FF11),hl
5517: 06 03       ld   b,$03
5519: FE 40       cp   $40
551B: CA 23 55    jp   z,$5523
551E: 05          dec  b
551F: DA 23 55    jp   c,$5523
5522: 05          dec  b
5523: FD 70 0F    ld   (iy+$0f),b
5526: C3 32 53    jp   $5332
5529: EB          ex   de,hl
552A: 7E          ld   a,(hl)
552B: 23          inc  hl
552C: 22 11 FF    ld   ($FF11),hl
552F: FD 77 16    ld   (iy+$16),a
5532: C3 32 53    jp   $5332
5535: EB          ex   de,hl
5536: 5E          ld   e,(hl)
5537: 23          inc  hl
5538: 56          ld   d,(hl)
5539: 23          inc  hl
553A: 7E          ld   a,(hl)
553B: FD 77 16    ld   (iy+$16),a
553E: 23          inc  hl
553F: 22 11 FF    ld   ($FF11),hl
5542: EB          ex   de,hl
5543: 29          add  hl,hl
5544: 29          add  hl,hl
5545: 29          add  hl,hl
5546: 9F          sbc  a,a
5547: FD 74 10    ld   (iy+$10),h
554A: FD 77 11    ld   (iy+$11),a
554D: C3 32 53    jp   $5332
5550: EB          ex   de,hl
5551: 7E          ld   a,(hl)
5552: 23          inc  hl
5553: 22 11 FF    ld   ($FF11),hl
5556: A7          and  a
5557: CA 61 55    jp   z,$5561
555A: FD CB 09 E6 set  4,(iy+$09)
555E: C3 32 53    jp   $5332
5561: FD CB 09 A6 res  4,(iy+$09)
5565: C3 32 53    jp   $5332
5568: EB          ex   de,hl
5569: 7E          ld   a,(hl)
556A: 23          inc  hl
556B: 22 11 FF    ld   ($FF11),hl
556E: E6 7F       and  $7F
5570: FD 77 0C    ld   (iy+$0c),a
5573: C3 32 53    jp   $5332
5576: EB          ex   de,hl
5577: 7E          ld   a,(hl)
5578: 23          inc  hl
5579: 22 11 FF    ld   ($FF11),hl
557C: E6 7F       and  $7F
557E: FD 77 0D    ld   (iy+$0d),a
5581: C3 32 53    jp   $5332
5584: EB          ex   de,hl
5585: 7E          ld   a,(hl)
5586: 23          inc  hl
5587: 22 11 FF    ld   ($FF11),hl
558A: E6 7F       and  $7F
558C: FD 77 0E    ld   (iy+$0e),a
558F: C3 32 53    jp   $5332
5592: FD 7E 22    ld   a,(iy+$22)
5595: FE 06       cp   $06
5597: DA 9D 55    jp   c,$559D
559A: C3 85 57    jp   $5785
559D: 5F          ld   e,a
559E: C6 02       add  a,$02
55A0: FD 77 22    ld   (iy+$22),a
55A3: 16 00       ld   d,$00
55A5: 21 1C 00    ld   hl,$001C
55A8: 19          add  hl,de
55A9: FD 5D       ld   e,iyl
55AB: FD 54       ld   d,iyh
55AD: 19          add  hl,de
55AE: 5E          ld   e,(hl)
55AF: 23          inc  hl
55B0: 56          ld   d,(hl)
55B1: ED 53 11 FF ld   ($FF11),de
55B5: FD 36 17 40 ld   (iy+$17),$40
55B9: FD 36 18 C0 ld   (iy+$18),$C0
55BD: FD 36 19 C0 ld   (iy+$19),$C0
55C1: FD 36 1A 7F ld   (iy+$1a),$7F
55C5: C3 32 53    jp   $5332
55C8: ED 53 11 FF ld   ($FF11),de
55CC: FD 7E 22    ld   a,(iy+$22)
55CF: 3D          dec  a
55D0: FE 06       cp   $06
55D2: D2 81 57    jp   nc,$5781
55D5: FD 77 22    ld   (iy+$22),a
55D8: 6F          ld   l,a
55D9: 26 00       ld   h,$00
55DB: 01 1C 00    ld   bc,$001C
55DE: 09          add  hl,bc
55DF: FD 4D       ld   c,iyl
55E1: FD 44       ld   b,iyh
55E3: 09          add  hl,bc
55E4: 36 00       ld   (hl),$00
55E6: FD 36 17 40 ld   (iy+$17),$40
55EA: FD 36 18 C0 ld   (iy+$18),$C0
55EE: FD 36 19 C0 ld   (iy+$19),$C0
55F2: FD 36 1A 7F ld   (iy+$1a),$7F
55F6: C3 32 53    jp   $5332
55F9: FD 7E 22    ld   a,(iy+$22)
55FC: FE 06       cp   $06
55FE: D2 81 57    jp   nc,$5781
5601: 6F          ld   l,a
5602: 26 00       ld   h,$00
5604: 01 1C 00    ld   bc,$001C
5607: 09          add  hl,bc
5608: FD 4D       ld   c,iyl
560A: FD 44       ld   b,iyh
560C: 09          add  hl,bc
560D: EB          ex   de,hl
560E: 1A          ld   a,(de)
560F: A7          and  a
5610: 28 06       jr   z,$5618
5612: 3D          dec  a
5613: 28 27       jr   z,$563C
5615: 12          ld   (de),a
5616: 18 08       jr   $5620
5618: 7E          ld   a,(hl)
5619: A7          and  a
561A: 28 04       jr   z,$5620
561C: 3D          dec  a
561D: 28 1D       jr   z,$563C
561F: 12          ld   (de),a
5620: 23          inc  hl
5621: 5E          ld   e,(hl)
5622: 23          inc  hl
5623: 56          ld   d,(hl)
5624: 23          inc  hl
5625: 19          add  hl,de
5626: 22 11 FF    ld   ($FF11),hl
5629: FD 36 17 40 ld   (iy+$17),$40
562D: FD 36 18 C0 ld   (iy+$18),$C0
5631: FD 36 19 C0 ld   (iy+$19),$C0
5635: FD 36 1A 7F ld   (iy+$1a),$7F
5639: C3 32 53    jp   $5332
563C: FD 7E 22    ld   a,(iy+$22)
563F: FE 06       cp   $06
5641: D2 81 57    jp   nc,$5781
5644: 3C          inc  a
5645: FD 77 22    ld   (iy+$22),a
5648: 23          inc  hl
5649: 23          inc  hl
564A: 23          inc  hl
564B: 22 11 FF    ld   ($FF11),hl
564E: C3 32 53    jp   $5332
5651: EB          ex   de,hl
5652: 5E          ld   e,(hl)
5653: 23          inc  hl
5654: 56          ld   d,(hl)
5655: 23          inc  hl
5656: EB          ex   de,hl
5657: 19          add  hl,de
5658: 22 11 FF    ld   ($FF11),hl
565B: FD 7E 22    ld   a,(iy+$22)
565E: A7          and  a
565F: CA 32 53    jp   z,$5332
5662: D6 02       sub  $02
5664: FE 06       cp   $06
5666: D2 81 57    jp   nc,$5781
5669: FD 77 22    ld   (iy+$22),a
566C: 6F          ld   l,a
566D: 26 00       ld   h,$00
566F: 01 1C 00    ld   bc,$001C
5672: 09          add  hl,bc
5673: FD 4D       ld   c,iyl
5675: FD 44       ld   b,iyh
5677: 09          add  hl,bc
5678: 73          ld   (hl),e
5679: 23          inc  hl
567A: 72          ld   (hl),d
567B: FD 36 17 40 ld   (iy+$17),$40
567F: FD 36 18 C0 ld   (iy+$18),$C0
5683: FD 36 19 C0 ld   (iy+$19),$C0
5687: FD 36 1A 7F ld   (iy+$1a),$7F
568B: C3 32 53    jp   $5332
568E: EB          ex   de,hl
568F: 7E          ld   a,(hl)
5690: B7          or   a
5691: 28 07       jr   z,$569A
5693: FD CB 09 C6 set  0,(iy+$09)
5697: C3 9E 56    jp   $569E
569A: FD CB 09 86 res  0,(iy+$09)
569E: 23          inc  hl
569F: 22 11 FF    ld   ($FF11),hl
56A2: C3 32 53    jp   $5332
56A5: EB          ex   de,hl
56A6: 7E          ld   a,(hl)
56A7: FD 77 0A    ld   (iy+$0a),a
56AA: 23          inc  hl
56AB: 22 11 FF    ld   ($FF11),hl
56AE: C3 32 53    jp   $5332
56B1: EB          ex   de,hl
56B2: 7E          ld   a,(hl)
56B3: FD 77 0A    ld   (iy+$0a),a
56B6: 23          inc  hl
56B7: 7E          ld   a,(hl)
56B8: FD 77 0B    ld   (iy+$0b),a
56BB: 23          inc  hl
56BC: 22 11 FF    ld   ($FF11),hl
56BF: C3 32 53    jp   $5332
56C2: EB          ex   de,hl
56C3: 5E          ld   e,(hl)
56C4: 23          inc  hl
56C5: 56          ld   d,(hl)
56C6: 23          inc  hl
56C7: 19          add  hl,de
56C8: 22 11 FF    ld   ($FF11),hl
56CB: FD 36 17 40 ld   (iy+$17),$40
56CF: FD 36 18 C0 ld   (iy+$18),$C0
56D3: FD 36 19 C0 ld   (iy+$19),$C0
56D7: FD 36 1A 7F ld   (iy+$1a),$7F
56DB: C3 32 53    jp   $5332
56DE: 47          ld   b,a
56DF: E6 F8       and  $F8
56E1: FE B8       cp   $B8
56E3: 28 0B       jr   z,$56F0
56E5: 0F          rrca
56E6: 0F          rrca
56E7: 0F          rrca
56E8: D6 0B       sub  $0B
56EA: FD 86 17    add  a,(iy+$17)
56ED: C3 F2 56    jp   $56F2
56F0: 1A          ld   a,(de)
56F1: 13          inc  de
56F2: FD 77 17    ld   (iy+$17),a
56F5: 32 F9 FB    ld   ($FBF9),a
56F8: FD 7E 18    ld   a,(iy+$18)
56FB: CB 38       srl  b
56FD: 38 05       jr   c,$5704
56FF: 1A          ld   a,(de)
5700: 13          inc  de
5701: FD 77 18    ld   (iy+$18),a
5704: FD 77 16    ld   (iy+$16),a
5707: 32 FA FB    ld   ($FBFA),a
570A: FD 7E 19    ld   a,(iy+$19)
570D: CB 38       srl  b
570F: 38 05       jr   c,$5716
5711: 1A          ld   a,(de)
5712: 13          inc  de
5713: FD 77 19    ld   (iy+$19),a
5716: 32 FB FB    ld   ($FBFB),a
5719: FD 7E 1A    ld   a,(iy+$1a)
571C: CB 38       srl  b
571E: 38 05       jr   c,$5725
5720: 1A          ld   a,(de)
5721: 13          inc  de
5722: FD 77 1A    ld   (iy+$1a),a
5725: 32 FC FB    ld   ($FBFC),a
5728: ED 53 11 FF ld   ($FF11),de
572C: FD CB 09 76 bit  6,(iy+$09)
5730: CA 5E 57    jp   z,$575E
5733: FD 7E 1B    ld   a,(iy+$1b)
5736: FE FF       cp   $FF
5738: CA 5E 57    jp   z,$575E
573B: 6F          ld   l,a
573C: 26 01       ld   h,$01
573E: 4E          ld   c,(hl)
573F: CB FD       set  7,l
5741: 46          ld   b,(hl)
5742: 59          ld   e,c
5743: 50          ld   d,b
5744: FD 6E 17    ld   l,(iy+$17)
5747: 26 01       ld   h,$01
5749: 4E          ld   c,(hl)
574A: CB FD       set  7,l
574C: 46          ld   b,(hl)
574D: EB          ex   de,hl
574E: A7          and  a
574F: ED 42       sbc  hl,bc
5751: FD 5E 10    ld   e,(iy+$10)
5754: FD 56 11    ld   d,(iy+$11)
5757: 19          add  hl,de
5758: FD 75 10    ld   (iy+$10),l
575B: FD 74 11    ld   (iy+$11),h
575E: FD 7E 17    ld   a,(iy+$17)
5761: FD 77 1B    ld   (iy+$1b),a
5764: 3A FB FB    ld   a,($FBFB)
5767: A7          and  a
5768: CA 32 53    jp   z,$5332
576B: 3A FC FB    ld   a,($FBFC)
576E: CA 32 53    jp   z,$5332
5771: CD 2A 4A    call $4A2A
5774: D2 7A 57    jp   nc,$577A
5777: C3 32 53    jp   $5332
577A: FD CB 09 4E bit  1,(iy+$09)
577E: C2 32 53    jp   nz,$5332
5781: AF          xor  a
5782: FD 77 12    ld   (iy+$12),a
5785: FD 36 09 08 ld   (iy+$09),$08
5789: AF          xor  a
578A: FD 77 00    ld   (iy+$00),a
578D: FD 77 01    ld   (iy+$01),a
5790: 3D          dec  a
5791: FD 77 02    ld   (iy+$02),a
5794: C9          ret
5795: 3E FF       ld   a,$FF
5797: 32 13 FF    ld   ($FF13),a
579A: 06 02       ld   b,$02
579C: 3A 13 FF    ld   a,($FF13)
579F: B8          cp   b
57A0: CA BD 57    jp   z,$57BD
57A3: 78          ld   a,b
57A4: 32 13 FF    ld   ($FF13),a
57A7: 0E 0B       ld   c,$0B
57A9: ED 78       in   a,(c)
57AB: 04          inc  b
57AC: CB 20       sla  b
57AE: 0D          dec  c
57AF: ED 78       in   a,(c)
57B1: 04          inc  b
57B2: CB 20       sla  b
57B4: 0D          dec  c
57B5: ED 78       in   a,(c)
57B7: 04          inc  b
57B8: CB 20       sla  b
57BA: 0D          dec  c
57BB: ED 78       in   a,(c)
57BD: C9          ret
57BE: 5E          ld   e,(hl)
57BF: 23          inc  hl
57C0: 56          ld   d,(hl)
57C1: 23          inc  hl
57C2: 7E          ld   a,(hl)
57C3: 6B          ld   l,e
57C4: 62          ld   h,d
57C5: 29          add  hl,hl
57C6: 8F          adc  a,a
57C7: 29          add  hl,hl
57C8: 8F          adc  a,a
57C9: 47          ld   b,a
57CA: C5          push bc
57CB: 3A 13 FF    ld   a,($FF13)
57CE: B8          cp   b
57CF: CA EC 57    jp   z,$57EC
57D2: 78          ld   a,b
57D3: 32 13 FF    ld   ($FF13),a
57D6: 0E 0B       ld   c,$0B
57D8: ED 78       in   a,(c)
57DA: 04          inc  b
57DB: CB 20       sla  b
57DD: 0D          dec  c
57DE: ED 78       in   a,(c)
57E0: 04          inc  b
57E1: CB 20       sla  b
57E3: 0D          dec  c
57E4: ED 78       in   a,(c)
57E6: 04          inc  b
57E7: CB 20       sla  b
57E9: 0D          dec  c
57EA: ED 78       in   a,(c)
57EC: C1          pop  bc
57ED: 7A          ld   a,d
57EE: E6 3F       and  $3F
57F0: 57          ld   d,a
57F1: 21 00 80    ld   hl,$8000
57F4: 19          add  hl,de
57F5: C9          ret
