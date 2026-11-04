009FD2: 6100 005A           bsr     $a02e
009FD6: 1B6D 1946 1947      move.b  ($1946,A5), ($1947,A5)
009FDC: 41ED 1948           lea     ($1948,A5), A0
009FE0: 4EBA 7E5A           jsr     ($7e5a,PC) ; ($11e3c)
009FE4: 41ED 294E           lea     ($294e,A5), A0
009FE8: 4EBA 7E52           jsr     ($7e52,PC) ; ($11e3c)
009FEC: 7000                moveq   #$0, D0
009FEE: 72FF                moveq   #-$1, D1
009FF0: 74FF                moveq   #-$1, D2
009FF2: 4EBA 000E           jsr     ($e,PC) ; ($a002)
009FF6: 7001                moveq   #$1, D0
009FF8: 72FF                moveq   #-$1, D1
009FFA: 74FF                moveq   #-$1, D2
009FFC: 4EBA 0004           jsr     ($4,PC) ; ($a002)
00A000: 4E75                rts
00A002: 49ED 3954           lea     ($3954,A5), A4
00A006: 4A40                tst.w   D0
00A008: 6700 0006           beq     $a010
00A00C: 49ED 395C           lea     ($395c,A5), A4
00A010: 0C41 017C           cmpi.w  #$17c, D1
00A014: 6500 0004           bcs     $a01a
00A018: 72FF                moveq   #-$1, D1
00A01A: B242                cmp.w   D2, D1
00A01C: 6500 0004           bcs     $a022
00A020: 74FF                moveq   #-$1, D2
00A022: 4841                swap    D1
00A024: 3202                move.w  D2, D1
00A026: 2881                move.l  D1, (A4)
00A028: 2941 0004           move.l  D1, ($4,A4)
00A02C: 4E75                rts
00A02E: 2B7C 0000 A036 3F64 move.l  #$a036, ($3f64,A5)
00A036: 4E75                rts
