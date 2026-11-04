53E8: 1A          ld   a,(de)
53E9: 13          inc  de
53EA: ED 53 11 FF ld   ($FF11),de
53EE: E6 3F       and  $3F
53F0: 87          add  a,a
53F1: C6 80       add  a,$80
53F3: 47          ld   b,a
53F4: 3A 10 FF    ld   a,($FF10)
53F7: 3C          inc  a
53F8: 32 10 FF    ld   ($FF10),a
53FB: E6 01       and  $01
53FD: B0          or   b
53FE: D3 0C       out  ($0C),a
5400: C3 32 53    jp   $5332
