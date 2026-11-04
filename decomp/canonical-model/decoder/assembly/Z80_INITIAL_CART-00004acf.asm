4ACF: 21 EB 4A ld   hl,$4AEB
4AD2: E5       push hl
4AD3: DD 7E 02 ld   a,(ix+$02)
4AD6: 87       add  a,a
4AD7: 6F       ld   l,a
4AD8: 26 00    ld   h,$00
4ADA: 11 E3 4A ld   de,$4AE3
4ADD: 19       add  hl,de
4ADE: 5E       ld   e,(hl)
4ADF: 23       inc  hl
4AE0: 56       ld   d,(hl)
4AE1: EB       ex   de,hl
4AE2: E9       jp   (hl)
