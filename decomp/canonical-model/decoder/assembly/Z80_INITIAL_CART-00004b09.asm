4B09: FD 5E 00 ld   e,(iy+$00)
4B0C: FD 56 01 ld   d,(iy+$01)
4B0F: 7B       ld   a,e
4B10: B2       or   d
4B11: C8       ret  z
4B12: DD 6B    ld   ixl,e
4B14: DD 62    ld   ixh,d
4B16: DD 6E 00 ld   l,(ix+$00)
4B19: DD 66 01 ld   h,(ix+$01)
4B1C: FD 5D    ld   e,iyl
4B1E: FD 54    ld   d,iyh
4B20: A7       and  a
4B21: ED 52    sbc  hl,de
4B23: CA 28 4B jp   z,$4B28
4B26: A7       and  a
4B27: C9       ret
4B28: 37       scf
4B29: C9       ret
4B2A: DD 7E 02 ld   a,(ix+$02)
4B2D: EE 01    xor  $01
4B2F: C0       ret  nz
4B30: 37       scf
4B31: C9       ret
4B32: CD 09 4B call $4B09
4B35: D0       ret  nc
4B36: CD 2A 4B call $4B2A
4B39: 30 05    jr   nc,$4B40
4B3B: CD 9D 37 call $379D
4B3E: 37       scf
4B3F: C9       ret
4B40: FD 7E 12 ld   a,(iy+$12)
4B43: C6 FE    add  a,$FE
4B45: 47       ld   b,a
4B46: 9F       sbc  a,a
4B47: A0       and  b
4B48: FD 77 12 ld   (iy+$12),a
4B4B: 37       scf
4B4C: C9       ret
4B4D: CD 09 4B call $4B09
4B50: D0       ret  nc
4B51: CD 2A 4B call $4B2A
4B54: 30 05    jr   nc,$4B5B
4B56: CD D5 37 call $37D5
4B59: 37       scf
4B5A: C9       ret
4B5B: FD 7E 12 ld   a,(iy+$12)
4B5E: C6 02    add  a,$02
4B60: 47       ld   b,a
4B61: 9F       sbc  a,a
4B62: B0       or   b
4B63: FD 77 12 ld   (iy+$12),a
4B66: 37       scf
4B67: C9       ret
4B68: CD 09 4B call $4B09
4B6B: D0       ret  nc
4B6C: CD 2A 4B call $4B2A
4B6F: 3F       ccf
4B70: D8       ret  c
4B71: CD 0C 38 call $380C
4B74: 37       scf
4B75: C9       ret
4B76: CD 09 4B call $4B09
4B79: D0       ret  nc
4B7A: CD 2A 4B call $4B2A
4B7D: 3F       ccf
4B7E: D8       ret  c
4B7F: CD 3C 38 call $383C
4B82: 37       scf
4B83: C9       ret
4B84: FD 6E 0A ld   l,(iy+$0a)
4B87: 26 00    ld   h,$00
4B89: 29       add  hl,hl
4B8A: 29       add  hl,hl
4B8B: EB       ex   de,hl
4B8C: FD 66 0B ld   h,(iy+$0b)
4B8F: 7C       ld   a,h
4B90: 3C       inc  a
4B91: 20 06    jr   nz,$4B99
4B93: 21 BA 24 ld   hl,$24BA
4B96: C3 A6 4B jp   $4BA6
4B99: 2E 00    ld   l,$00
4B9B: 29       add  hl,hl
4B9C: 19       add  hl,de
4B9D: EB       ex   de,hl
4B9E: D5       push de
4B9F: 21 80 00 ld   hl,$0080
4BA2: CD BE 57 call $57BE
4BA5: D1       pop  de
4BA6: 19       add  hl,de
4BA7: CD BE 57 call $57BE
4BAA: C9       ret
