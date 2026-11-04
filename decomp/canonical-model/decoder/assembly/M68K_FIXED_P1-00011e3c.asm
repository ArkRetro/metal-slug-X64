011E3C: 2148 0400           move.l  A0, ($400,A0)
011E40: 317C 1004 1004      move.w  #$1004, ($1004,A0)
011E46: 4E75                rts
011E48: 102D 1946           move.b  ($1946,A5), D0
011E4C: B02D 1947           cmp.b   ($1947,A5), D0
011E50: 6700 0008           beq     $11e5a
011E54: 003C 0001           ori     #$1, CCR
011E58: 4E75                rts
011E5A: 023C 00FE           andi    #$fe, CCR
011E5E: 4E75                rts
011E60: 4BF9 0010 8000      lea     $108000.l, A5
011E66: 102D 1946           move.b  ($1946,A5), D0
011E6A: B02D 1947           cmp.b   ($1947,A5), D0
011E6E: 6700 0180           beq     $11ff0
011E72: 1B40 1947           move.b  D0, ($1947,A5)
011E76: 6A00 00BA           bpl     $11f32
011E7A: 41ED 1948           lea     ($1948,A5), A0
011E7E: 2F0E                move.l  A6, -(A7)
011E80: 4DF9 003C 0000      lea     $3c0000.l, A6
011E86: 49ED 3954           lea     ($3954,A5), A4
011E8A: 3954 0004           move.w  (A4), ($4,A4)
011E8E: 49ED 395C           lea     ($395c,A5), A4
011E92: 3954 0004           move.w  (A4), ($4,A4)
011E96: 43D0                lea     (A0), A1
011E98: 6000 0038           bra     $11ed2
011E9C: 3E11                move.w  (A1), D7
011E9E: 5889                addq.l  #4, A1
011EA0: 45F0 7000           lea     (A0,D7.w), A2
011EA4: 322A 0006           move.w  ($6,A2), D1
011EA8: 49ED 3954           lea     ($3954,A5), A4
011EAC: 0881 0002           bclr    #$2, D1
011EB0: 6700 0006           beq     $11eb8
011EB4: 49ED 395C           lea     ($395c,A5), A4
011EB8: 0C6A FFFF 0004      cmpi.w  #-$1, ($4,A2)
011EBE: 56C7                sne     D7
011EC0: 0247 0004           andi.w  #$4, D7
011EC4: 8E01                or.b    D1, D7
011EC6: DE47                add.w   D7, D7
011EC8: DE47                add.w   D7, D7
011ECA: 47FA 81DA           lea     (-$7e26,PC) ; ($a0a6), A3
011ECE: 4EB3 7000           jsr     (A3,D7.w)
011ED2: B3E8 0400           cmpa.l  ($400,A0), A1
011ED6: 65C4                bcs     $11e9c
011ED8: 49ED 3954           lea     ($3954,A5), A4
011EDC: 302C 0004           move.w  ($4,A4), D0
011EE0: 322C 0006           move.w  ($6,A4), D1
011EE4: B041                cmp.w   D1, D0
011EE6: 6E00 0004           bgt     $11eec
011EEA: 3001                move.w  D1, D0
011EEC: 322C 0002           move.w  ($2,A4), D1
011EF0: 9240                sub.w   D0, D1
011EF2: 6100 E4B2           bsr     $103a6
011EF6: 49ED 395C           lea     ($395c,A5), A4
011EFA: 302C 0004           move.w  ($4,A4), D0
011EFE: 322C 0006           move.w  ($6,A4), D1
011F02: B041                cmp.w   D1, D0
011F04: 6E00 0004           bgt     $11f0a
011F08: 3001                move.w  D1, D0
011F0A: 322C 0002           move.w  ($2,A4), D1
011F0E: 9240                sub.w   D0, D1
011F10: 6100 E494           bsr     $103a6
011F14: 2C5F                movea.l (A7)+, A6
011F16: 3B7C 0001 8014      move.w  #$1, (-$7fec,A5)
011F1C: 33FC 0001 002F FFF0 move.w  #$1, $2ffff0.l
011F24: 0C39 0001 002F FFFF cmpi.b  #$1, $2fffff.l
011F2C: 66F6                bne     $11f24
011F2E: 6000 00BA           bra     $11fea
011F32: 41ED 294E           lea     ($294e,A5), A0
011F36: 2F0E                move.l  A6, -(A7)
011F38: 4DF9 003C 0000      lea     $3c0000.l, A6
011F3E: 49ED 3954           lea     ($3954,A5), A4
011F42: 396C 0002 0006      move.w  ($2,A4), ($6,A4)
011F48: 49ED 395C           lea     ($395c,A5), A4
011F4C: 396C 0002 0006      move.w  ($2,A4), ($6,A4)
011F52: 2268 0400           movea.l ($400,A0), A1
011F56: 6000 0038           bra     $11f90
011F5A: 5989                subq.l  #4, A1
011F5C: 3E11                move.w  (A1), D7
011F5E: 45F0 7000           lea     (A0,D7.w), A2
011F62: 322A 0006           move.w  ($6,A2), D1
011F66: 49ED 3954           lea     ($3954,A5), A4
011F6A: 0881 0002           bclr    #$2, D1
011F6E: 6700 0006           beq     $11f76
011F72: 49ED 395C           lea     ($395c,A5), A4
011F76: 0C6A FFFF 0004      cmpi.w  #-$1, ($4,A2)
011F7C: 56C7                sne     D7
011F7E: 0247 0004           andi.w  #$4, D7
011F82: 8E01                or.b    D1, D7
011F84: DE47                add.w   D7, D7
011F86: DE47                add.w   D7, D7
011F88: 47FA 815C           lea     (-$7ea4,PC) ; ($a0e6), A3
011F8C: 4EB3 7000           jsr     (A3,D7.w)
011F90: B1C9                cmpa.l  A1, A0
011F92: 65C6                bcs     $11f5a
011F94: 49ED 3954           lea     ($3954,A5), A4
011F98: 302C 0004           move.w  ($4,A4), D0
011F9C: 322C 0006           move.w  ($6,A4), D1
011FA0: B041                cmp.w   D1, D0
011FA2: 6D00 0004           blt     $11fa8
011FA6: 3001                move.w  D1, D0
011FA8: 3214                move.w  (A4), D1
011FAA: 9240                sub.w   D0, D1
011FAC: 4441                neg.w   D1
011FAE: 6100 EC18           bsr     $10bc8
011FB2: 49ED 395C           lea     ($395c,A5), A4
011FB6: 302C 0004           move.w  ($4,A4), D0
011FBA: 322C 0006           move.w  ($6,A4), D1
011FBE: B041                cmp.w   D1, D0
011FC0: 6D00 0004           blt     $11fc6
011FC4: 3001                move.w  D1, D0
011FC6: 3214                move.w  (A4), D1
011FC8: 9240                sub.w   D0, D1
011FCA: 4441                neg.w   D1
011FCC: 6100 EBFA           bsr     $10bc8
011FD0: 2C5F                movea.l (A7)+, A6
011FD2: 3B7C 0001 8014      move.w  #$1, (-$7fec,A5)
011FD8: 33FC 0001 002F FFF0 move.w  #$1, $2ffff0.l
011FE0: 0C39 0001 002F FFFF cmpi.b  #$1, $2fffff.l
011FE8: 66F6                bne     $11fe0
011FEA: 003C 0001           ori     #$1, CCR
011FEE: 4E75                rts
011FF0: 023C 00FE           andi    #$fe, CCR
011FF4: 4E75                rts
