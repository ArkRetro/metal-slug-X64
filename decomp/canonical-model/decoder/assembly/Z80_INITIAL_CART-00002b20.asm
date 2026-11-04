2B20: 3A 11 F8    ld   a,($F811)
2B23: 6F          ld   l,a
2B24: 3A C1 F8    ld   a,($F8C1)
2B27: A7          and  a
2B28: 28 15       jr   z,$2B3F
2B2A: 2C          inc  l
2B2B: 28 12       jr   z,$2B3F
2B2D: 08          ex   af,af'
2B2E: 2D          dec  l
2B2F: 7D          ld   a,l
2B30: 28 0D       jr   z,$2B3F
2B32: 26 06       ld   h,$06
2B34: 7E          ld   a,(hl)
2B35: 08          ex   af,af'
2B36: 6F          ld   l,a
2B37: 08          ex   af,af'
2B38: 86          add  a,(hl)
2B39: 6F          ld   l,a
2B3A: 9F          sbc  a,a
2B3B: 30 02       jr   nc,$2B3F
2B3D: 24          inc  h
2B3E: 7E          ld   a,(hl)
2B3F: FD 6E 12    ld   l,(iy+$12)
2B42: A7          and  a
2B43: 28 15       jr   z,$2B5A
2B45: 2C          inc  l
2B46: 28 12       jr   z,$2B5A
2B48: 08          ex   af,af'
2B49: 2D          dec  l
2B4A: 7D          ld   a,l
2B4B: 28 0D       jr   z,$2B5A
2B4D: 26 06       ld   h,$06
2B4F: 7E          ld   a,(hl)
2B50: 08          ex   af,af'
2B51: 6F          ld   l,a
2B52: 08          ex   af,af'
2B53: 86          add  a,(hl)
2B54: 6F          ld   l,a
2B55: 9F          sbc  a,a
2B56: 30 02       jr   nc,$2B5A
2B58: 24          inc  h
2B59: 7E          ld   a,(hl)
2B5A: 6F          ld   l,a
2B5B: 3E FF       ld   a,$FF
2B5D: FD CB 09 4E bit  1,(iy+$09)
2B61: CA 67 2B    jp   z,$2B67
2B64: 3A 05 F8    ld   a,($F805)
2B67: A7          and  a
2B68: 28 15       jr   z,$2B7F
2B6A: 2C          inc  l
2B6B: 28 12       jr   z,$2B7F
2B6D: 08          ex   af,af'
2B6E: 2D          dec  l
2B6F: 7D          ld   a,l
2B70: 28 0D       jr   z,$2B7F
2B72: 26 06       ld   h,$06
2B74: 7E          ld   a,(hl)
2B75: 08          ex   af,af'
2B76: 6F          ld   l,a
2B77: 08          ex   af,af'
2B78: 86          add  a,(hl)
2B79: 6F          ld   l,a
2B7A: 9F          sbc  a,a
2B7B: 30 02       jr   nc,$2B7F
2B7D: 24          inc  h
2B7E: 7E          ld   a,(hl)
2B7F: 21 C6 F8    ld   hl,$F8C6
2B82: BE          cp   (hl)
2B83: 77          ld   (hl),a
2B84: CA 90 2B    jp   z,$2B90
2B87: 6F          ld   l,a
2B88: 26 0A       ld   h,$0A
2B8A: 5E          ld   e,(hl)
2B8B: 16 0A       ld   d,$0A
2B8D: 0E 04       ld   c,$04
2B8F: CF          rst  $08
2B90: 0E 01       ld   c,$01
2B92: FD CB 09 4E bit  1,(iy+$09)
2B96: 28 04       jr   z,$2B9C
2B98: 3A 09 F8    ld   a,($F809)
2B9B: 4F          ld   c,a
2B9C: 21 BC F8    ld   hl,$F8BC
2B9F: 7E          ld   a,(hl)
2BA0: 81          add  a,c
2BA1: DE 00       sbc  a,$00
2BA3: 77          ld   (hl),a
2BA4: 21 BB F8    ld   hl,$F8BB
2BA7: 7E          ld   a,(hl)
2BA8: A7          and  a
2BA9: CA BD 2B    jp   z,$2BBD
2BAC: 91          sub  c
2BAD: 77          ld   (hl),a
2BAE: 20 22       jr   nz,$2BD2
2BB0: FD CB 09 66 bit  4,(iy+$09)
2BB4: 28 16       jr   z,$2BCC
2BB6: DD CB 03 E6 set  4,(ix+$03)
2BBA: C3 D2 2B    jp   $2BD2
2BBD: FD CB 09 66 bit  4,(iy+$09)
2BC1: 20 0F       jr   nz,$2BD2
2BC3: 21 B9 F8    ld   hl,$F8B9
2BC6: CB 66       bit  4,(hl)
2BC8: 28 08       jr   z,$2BD2
2BCA: CB A6       res  4,(hl)
2BCC: 21 CC F8    ld   hl,$F8CC
2BCF: CD CA 21    call $21CA
2BD2: 21 03 F9    ld   hl,$F903
2BD5: 5E          ld   e,(hl)
2BD6: 23          inc  hl
2BD7: 7E          ld   a,(hl)
2BD8: 73          ld   (hl),e
2BD9: 23          inc  hl
2BDA: BB          cp   e
2BDB: CA E3 2B    jp   z,$2BE3
2BDE: 16 06       ld   d,$06
2BE0: 0E 04       ld   c,$04
2BE2: CF          rst  $08
2BE3: 5E          ld   e,(hl)
2BE4: 23          inc  hl
2BE5: 7E          ld   a,(hl)
2BE6: 73          ld   (hl),e
2BE7: BB          cp   e
2BE8: C8          ret  z
2BE9: 7B          ld   a,e
2BEA: 2F          cpl
2BEB: E6 07       and  $07
2BED: 87          add  a,a
2BEE: 87          add  a,a
2BEF: 87          add  a,a
2BF0: B3          or   e
2BF1: 5F          ld   e,a
2BF2: 16 07       ld   d,$07
2BF4: 0E 04       ld   c,$04
2BF6: CF          rst  $08
2BF7: C9          ret
