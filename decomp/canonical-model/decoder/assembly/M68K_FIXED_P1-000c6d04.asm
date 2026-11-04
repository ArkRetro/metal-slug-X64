0C6D04: 0228 00FE 0082 andi.b  #$fe, ($82,A0)
0C6D0A: 7E00           moveq   #$0, D7
0C6D0C: 3E28 002C      move.w  ($2c,A0), D7
0C6D10: 6B00 0220      bmi     $c6f32
0C6D14: 13C0 0030 0001 move.b  D0, $300001.l
0C6D1A: 3C07           move.w  D7, D6
0C6D1C: 0647 8201      addi.w  #-$7dff, D7
0C6D20: 0646 8001      addi.w  #-$7fff, D6
0C6D24: 4847           swap    D7
0C6D26: 4846           swap    D6
0C6D28: 3C3C 0FFF      move.w  #$fff, D6
0C6D2C: 2A3C 0001 0000 move.l  #$10000, D5
0C6D32: 23C7 003C 0000 move.l  D7, $3c0000.l
0C6D38: DE85           add.l   D5, D7
0C6D3A: 23C6 003C 0000 move.l  D6, $3c0000.l
0C6D40: DC85           add.l   D5, D6
0C6D42: 23C7 003C 0000 move.l  D7, $3c0000.l
0C6D48: DE85           add.l   D5, D7
0C6D4A: 23C6 003C 0000 move.l  D6, $3c0000.l
0C6D50: DC85           add.l   D5, D6
0C6D52: 23C7 003C 0000 move.l  D7, $3c0000.l
0C6D58: DE85           add.l   D5, D7
0C6D5A: 23C6 003C 0000 move.l  D6, $3c0000.l
0C6D60: DC85           add.l   D5, D6
0C6D62: 23C7 003C 0000 move.l  D7, $3c0000.l
0C6D68: DE85           add.l   D5, D7
0C6D6A: 23C6 003C 0000 move.l  D6, $3c0000.l
0C6D70: DC85           add.l   D5, D6
0C6D72: 23C7 003C 0000 move.l  D7, $3c0000.l
0C6D78: DE85           add.l   D5, D7
0C6D7A: 23C6 003C 0000 move.l  D6, $3c0000.l
0C6D80: DC85           add.l   D5, D6
0C6D82: 23C7 003C 0000 move.l  D7, $3c0000.l
0C6D88: DE85           add.l   D5, D7
0C6D8A: 23C6 003C 0000 move.l  D6, $3c0000.l
0C6D90: DC85           add.l   D5, D6
0C6D92: 23C7 003C 0000 move.l  D7, $3c0000.l
0C6D98: DE85           add.l   D5, D7
0C6D9A: 23C6 003C 0000 move.l  D6, $3c0000.l
0C6DA0: DC85           add.l   D5, D6
0C6DA2: 23C7 003C 0000 move.l  D7, $3c0000.l
0C6DA8: DE85           add.l   D5, D7
0C6DAA: 23C6 003C 0000 move.l  D6, $3c0000.l
0C6DB0: DC85           add.l   D5, D6
0C6DB2: 23C7 003C 0000 move.l  D7, $3c0000.l
0C6DB8: DE85           add.l   D5, D7
0C6DBA: 23C6 003C 0000 move.l  D6, $3c0000.l
0C6DC0: DC85           add.l   D5, D6
0C6DC2: 23C7 003C 0000 move.l  D7, $3c0000.l
0C6DC8: DE85           add.l   D5, D7
0C6DCA: 23C6 003C 0000 move.l  D6, $3c0000.l
0C6DD0: DC85           add.l   D5, D6
0C6DD2: 23C7 003C 0000 move.l  D7, $3c0000.l
0C6DD8: DE85           add.l   D5, D7
0C6DDA: 23C6 003C 0000 move.l  D6, $3c0000.l
0C6DE0: DC85           add.l   D5, D6
0C6DE2: 23C7 003C 0000 move.l  D7, $3c0000.l
0C6DE8: DE85           add.l   D5, D7
0C6DEA: 23C6 003C 0000 move.l  D6, $3c0000.l
0C6DF0: DC85           add.l   D5, D6
0C6DF2: 23C7 003C 0000 move.l  D7, $3c0000.l
0C6DF8: DE85           add.l   D5, D7
0C6DFA: 23C6 003C 0000 move.l  D6, $3c0000.l
0C6E00: DC85           add.l   D5, D6
0C6E02: 23C7 003C 0000 move.l  D7, $3c0000.l
0C6E08: DE85           add.l   D5, D7
0C6E0A: 23C6 003C 0000 move.l  D6, $3c0000.l
0C6E10: DC85           add.l   D5, D6
0C6E12: 23C7 003C 0000 move.l  D7, $3c0000.l
0C6E18: DE85           add.l   D5, D7
0C6E1A: 23C6 003C 0000 move.l  D6, $3c0000.l
0C6E20: DC85           add.l   D5, D6
0C6E22: 23C7 003C 0000 move.l  D7, $3c0000.l
0C6E28: DE85           add.l   D5, D7
0C6E2A: 23C6 003C 0000 move.l  D6, $3c0000.l
0C6E30: DC85           add.l   D5, D6
0C6E32: 23C7 003C 0000 move.l  D7, $3c0000.l
0C6E38: DE85           add.l   D5, D7
0C6E3A: 23C6 003C 0000 move.l  D6, $3c0000.l
0C6E40: DC85           add.l   D5, D6
0C6E42: 23C7 003C 0000 move.l  D7, $3c0000.l
0C6E48: DE85           add.l   D5, D7
0C6E4A: 23C6 003C 0000 move.l  D6, $3c0000.l
0C6E50: DC85           add.l   D5, D6
0C6E52: 23C7 003C 0000 move.l  D7, $3c0000.l
0C6E58: DE85           add.l   D5, D7
0C6E5A: 23C6 003C 0000 move.l  D6, $3c0000.l
0C6E60: DC85           add.l   D5, D6
0C6E62: 23C7 003C 0000 move.l  D7, $3c0000.l
0C6E68: DE85           add.l   D5, D7
0C6E6A: 23C6 003C 0000 move.l  D6, $3c0000.l
0C6E70: DC85           add.l   D5, D6
0C6E72: 23C7 003C 0000 move.l  D7, $3c0000.l
0C6E78: DE85           add.l   D5, D7
0C6E7A: 23C6 003C 0000 move.l  D6, $3c0000.l
0C6E80: DC85           add.l   D5, D6
0C6E82: 23C7 003C 0000 move.l  D7, $3c0000.l
0C6E88: DE85           add.l   D5, D7
0C6E8A: 23C6 003C 0000 move.l  D6, $3c0000.l
0C6E90: DC85           add.l   D5, D6
0C6E92: 23C7 003C 0000 move.l  D7, $3c0000.l
0C6E98: DE85           add.l   D5, D7
0C6E9A: 23C6 003C 0000 move.l  D6, $3c0000.l
0C6EA0: DC85           add.l   D5, D6
0C6EA2: 23C7 003C 0000 move.l  D7, $3c0000.l
0C6EA8: DE85           add.l   D5, D7
0C6EAA: 23C6 003C 0000 move.l  D6, $3c0000.l
0C6EB0: DC85           add.l   D5, D6
0C6EB2: 23C7 003C 0000 move.l  D7, $3c0000.l
0C6EB8: DE85           add.l   D5, D7
0C6EBA: 23C6 003C 0000 move.l  D6, $3c0000.l
0C6EC0: DC85           add.l   D5, D6
0C6EC2: 23C7 003C 0000 move.l  D7, $3c0000.l
0C6EC8: DE85           add.l   D5, D7
0C6ECA: 23C6 003C 0000 move.l  D6, $3c0000.l
0C6ED0: DC85           add.l   D5, D6
0C6ED2: 23C7 003C 0000 move.l  D7, $3c0000.l
0C6ED8: DE85           add.l   D5, D7
0C6EDA: 23C6 003C 0000 move.l  D6, $3c0000.l
0C6EE0: DC85           add.l   D5, D6
0C6EE2: 23C7 003C 0000 move.l  D7, $3c0000.l
0C6EE8: DE85           add.l   D5, D7
0C6EEA: 23C6 003C 0000 move.l  D6, $3c0000.l
0C6EF0: DC85           add.l   D5, D6
0C6EF2: 23C7 003C 0000 move.l  D7, $3c0000.l
0C6EF8: DE85           add.l   D5, D7
0C6EFA: 23C6 003C 0000 move.l  D6, $3c0000.l
0C6F00: DC85           add.l   D5, D6
0C6F02: 23C7 003C 0000 move.l  D7, $3c0000.l
0C6F08: DE85           add.l   D5, D7
0C6F0A: 23C6 003C 0000 move.l  D6, $3c0000.l
0C6F10: DC85           add.l   D5, D6
0C6F12: 23C7 003C 0000 move.l  D7, $3c0000.l
0C6F18: DE85           add.l   D5, D7
0C6F1A: 23C6 003C 0000 move.l  D6, $3c0000.l
0C6F20: DC85           add.l   D5, D6
0C6F22: 23C7 003C 0000 move.l  D7, $3c0000.l
0C6F28: DE85           add.l   D5, D7
0C6F2A: 23C6 003C 0000 move.l  D6, $3c0000.l
0C6F30: DC85           add.l   D5, D6
0C6F32: 4E75           rts
