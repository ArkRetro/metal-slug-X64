4AEB: 21 07 4B ld   hl,$4B07
4AEE: E5       push hl
4AEF: DD 7E 02 ld   a,(ix+$02)
4AF2: 87       add  a,a
4AF3: 6F       ld   l,a
4AF4: 26 00    ld   h,$00
4AF6: 11 FF 4A ld   de,$4AFF
4AF9: 19       add  hl,de
4AFA: 5E       ld   e,(hl)
4AFB: 23       inc  hl
4AFC: 56       ld   d,(hl)
4AFD: EB       ex   de,hl
4AFE: E9       jp   (hl)
