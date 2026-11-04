28F2: 3A 11 F8    ld   a,($F811)
28F5: 6F          ld   l,a
28F6: 3A 74 F8    ld   a,($F874)
28F9: A7          and  a
28FA: 28 15       jr   z,$2911
28FC: 2C          inc  l
28FD: 28 12       jr   z,$2911
28FF: 08          ex   af,af'
2900: 2D          dec  l
2901: 7D          ld   a,l
2902: 28 0D       jr   z,$2911
2904: 26 06       ld   h,$06
2906: 7E          ld   a,(hl)
2907: 08          ex   af,af'
2908: 6F          ld   l,a
2909: 08          ex   af,af'
290A: 86          add  a,(hl)
290B: 6F          ld   l,a
290C: 9F          sbc  a,a
290D: 30 02       jr   nc,$2911
290F: 24          inc  h
2910: 7E          ld   a,(hl)
2911: FD 6E 12    ld   l,(iy+$12)
2914: A7          and  a
2915: 28 15       jr   z,$292C
2917: 2C          inc  l
2918: 28 12       jr   z,$292C
291A: 08          ex   af,af'
291B: 2D          dec  l
291C: 7D          ld   a,l
291D: 28 0D       jr   z,$292C
291F: 26 06       ld   h,$06
2921: 7E          ld   a,(hl)
2922: 08          ex   af,af'
2923: 6F          ld   l,a
2924: 08          ex   af,af'
2925: 86          add  a,(hl)
2926: 6F          ld   l,a
2927: 9F          sbc  a,a
2928: 30 02       jr   nc,$292C
292A: 24          inc  h
292B: 7E          ld   a,(hl)
292C: 6F          ld   l,a
292D: 3E FF       ld   a,$FF
292F: FD CB 09 4E bit  1,(iy+$09)
2933: CA 39 29    jp   z,$2939
2936: 3A 05 F8    ld   a,($F805)
2939: A7          and  a
293A: 28 15       jr   z,$2951
293C: 2C          inc  l
293D: 28 12       jr   z,$2951
293F: 08          ex   af,af'
2940: 2D          dec  l
2941: 7D          ld   a,l
2942: 28 0D       jr   z,$2951
2944: 26 06       ld   h,$06
2946: 7E          ld   a,(hl)
2947: 08          ex   af,af'
2948: 6F          ld   l,a
2949: 08          ex   af,af'
294A: 86          add  a,(hl)
294B: 6F          ld   l,a
294C: 9F          sbc  a,a
294D: 30 02       jr   nc,$2951
294F: 24          inc  h
2950: 7E          ld   a,(hl)
2951: 21 79 F8    ld   hl,$F879
2954: BE          cp   (hl)
2955: 77          ld   (hl),a
2956: CA 62 29    jp   z,$2962
2959: 6F          ld   l,a
295A: 26 0A       ld   h,$0A
295C: 5E          ld   e,(hl)
295D: 16 09       ld   d,$09
295F: 0E 04       ld   c,$04
2961: CF          rst  $08
2962: 0E 01       ld   c,$01
2964: FD CB 09 4E bit  1,(iy+$09)
2968: 28 04       jr   z,$296E
296A: 3A 09 F8    ld   a,($F809)
296D: 4F          ld   c,a
296E: 21 6F F8    ld   hl,$F86F
2971: 7E          ld   a,(hl)
2972: 81          add  a,c
2973: DE 00       sbc  a,$00
2975: 77          ld   (hl),a
2976: 21 6E F8    ld   hl,$F86E
2979: 7E          ld   a,(hl)
297A: A7          and  a
297B: CA 8F 29    jp   z,$298F
297E: 91          sub  c
297F: 77          ld   (hl),a
2980: 20 22       jr   nz,$29A4
2982: FD CB 09 66 bit  4,(iy+$09)
2986: 28 16       jr   z,$299E
2988: DD CB 03 E6 set  4,(ix+$03)
298C: C3 A4 29    jp   $29A4
298F: FD CB 09 66 bit  4,(iy+$09)
2993: 20 0F       jr   nz,$29A4
2995: 21 6C F8    ld   hl,$F86C
2998: CB 66       bit  4,(hl)
299A: 28 08       jr   z,$29A4
299C: CB A6       res  4,(hl)
299E: 21 7F F8    ld   hl,$F87F
29A1: CD CA 21    call $21CA
29A4: DD 21 B6 F8 ld   ix,$F8B6
29A8: FD 2A B6 F8 ld   iy,($F8B6)
29AC: FD CB 09 5E bit  3,(iy+$09)
29B0: C4 CC 1F    call nz,$1FCC
29B3: 21 CC F8    ld   hl,$F8CC
29B6: CD DE 21    call $21DE
29B9: 32 11 F8    ld   ($F811),a
29BC: A7          and  a
29BD: CA 20 2B    jp   z,$2B20
29C0: 21 DA F8    ld   hl,$F8DA
29C3: FD 56 0D    ld   d,(iy+$0d)
29C6: CD CF 22    call $22CF
29C9: 91          sub  c
29CA: 3D          dec  a
29CB: 6F          ld   l,a
29CC: 3A 11 F8    ld   a,($F811)
29CF: A7          and  a
29D0: 28 15       jr   z,$29E7
29D2: 2C          inc  l
29D3: 28 12       jr   z,$29E7
29D5: 08          ex   af,af'
29D6: 2D          dec  l
29D7: 7D          ld   a,l
29D8: 28 0D       jr   z,$29E7
29DA: 26 06       ld   h,$06
29DC: 7E          ld   a,(hl)
29DD: 08          ex   af,af'
29DE: 6F          ld   l,a
29DF: 08          ex   af,af'
29E0: 86          add  a,(hl)
29E1: 6F          ld   l,a
29E2: 9F          sbc  a,a
29E3: 30 02       jr   nc,$29E7
29E5: 24          inc  h
29E6: 7E          ld   a,(hl)
29E7: 32 11 F8    ld   ($F811),a
29EA: A7          and  a
29EB: CA 20 2B    jp   z,$2B20
29EE: 3A C1 F8    ld   a,($F8C1)
29F1: A7          and  a
29F2: CA 20 2B    jp   z,$2B20
29F5: 3A C9 F8    ld   a,($F8C9)
29F8: A7          and  a
29F9: CA 2F 2A    jp   z,$2A2F
29FC: 21 E8 F8    ld   hl,$F8E8
29FF: FD 56 0C    ld   d,(iy+$0c)
2A02: CD CF 22    call $22CF
2A05: 6F          ld   l,a
2A06: 26 00       ld   h,$00
2A08: 44          ld   b,h
2A09: 29          add  hl,hl
2A0A: A7          and  a
2A0B: ED 42       sbc  hl,bc
2A0D: 7D          ld   a,l
2A0E: CB 2C       sra  h
2A10: 1F          rra
2A11: CB 2C       sra  h
2A13: 1F          rra
2A14: CB 2C       sra  h
2A16: 1F          rra
2A17: 21 BE F8    ld   hl,$F8BE
2A1A: 47          ld   b,a
2A1B: 7E          ld   a,(hl)
2A1C: E6 1F       and  $1F
2A1E: 80          add  a,b
2A1F: F2 23 2A    jp   p,$2A23
2A22: AF          xor  a
2A23: FE 20       cp   $20
2A25: 38 02       jr   c,$2A29
2A27: 3E 1F       ld   a,$1F
2A29: 32 03 F9    ld   ($F903),a
2A2C: C3 20 2B    jp   $2B20
2A2F: 2A BF F8    ld   hl,($F8BF)
2A32: ED 5B C7 F8 ld   de,($F8C7)
2A36: 19          add  hl,de
2A37: FD 5E 10    ld   e,(iy+$10)
2A3A: FD 56 11    ld   d,(iy+$11)
2A3D: 19          add  hl,de
2A3E: 22 0F F8    ld   ($F80F),hl
2A41: 21 E8 F8    ld   hl,$F8E8
2A44: FD 56 0C    ld   d,(iy+$0c)
2A47: CD CF 22    call $22CF
2A4A: 6F          ld   l,a
2A4B: 26 00       ld   h,$00
2A4D: 44          ld   b,h
2A4E: 29          add  hl,hl
2A4F: A7          and  a
2A50: ED 42       sbc  hl,bc
2A52: ED 5B 0F F8 ld   de,($F80F)
2A56: 19          add  hl,de
2A57: 22 0F F8    ld   ($F80F),hl
2A5A: 21 F6 F8    ld   hl,$F8F6
2A5D: CD 3F 24    call $243F
2A60: ED 5B 0F F8 ld   de,($F80F)
2A64: 19          add  hl,de
2A65: EB          ex   de,hl
2A66: 2A C4 F8    ld   hl,($F8C4)
2A69: A7          and  a
2A6A: ED 52       sbc  hl,de
2A6C: CA 20 2B    jp   z,$2B20
2A6F: ED 53 C4 F8 ld   ($F8C4),de
2A73: 7A          ld   a,d
2A74: FE 10       cp   $10
2A76: DA 7B 2A    jp   c,$2A7B
2A79: 3E 0F       ld   a,$0F
2A7B: 87          add  a,a
2A7C: 87          add  a,a
2A7D: 4F          ld   c,a
2A7E: 06 00       ld   b,$00
2A80: 7B          ld   a,e
2A81: D9          exx
2A82: 6F          ld   l,a
2A83: 26 02       ld   h,$02
2A85: 7E          ld   a,(hl)
2A86: 24          inc  h
2A87: 6E          ld   l,(hl)
2A88: 26 00       ld   h,$00
2A8A: D9          exx
2A8B: 21 90 2A    ld   hl,$2A90
2A8E: 09          add  hl,bc
2A8F: E9          jp   (hl)
