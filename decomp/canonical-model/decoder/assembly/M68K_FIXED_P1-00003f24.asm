003F24: 13C0 0030 0001      move.b  D0, $300001.l
003F2A: 4EB9 0000 9EA0      jsr     $9ea0.l
003F30: 4EB9 0001 AB60      jsr     $1ab60.l
003F36: 4EB9 0001 64B8      jsr     $164b8.l
003F3C: 7000                moveq   #$0, D0
003F3E: 72FF                moveq   #-$1, D1
003F40: 1140 0015           move.b  D0, ($15,A0)
003F44: 1140 005F           move.b  D0, ($5f,A0)
003F48: 1140 0060           move.b  D0, ($60,A0)
003F4C: 1140 0061           move.b  D0, ($61,A0)
003F50: 1141 0012           move.b  D1, ($12,A0)
003F54: 1141 0013           move.b  D1, ($13,A0)
003F58: 3141 001E           move.w  D1, ($1e,A0)
003F5C: 41E8 006E           lea     ($6e,A0), A0
003F60: 13C0 0030 0001      move.b  D0, $300001.l
003F66: 7000                moveq   #$0, D0
003F68: 20C0                move.l  D0, (A0)+
003F6A: 20C0                move.l  D0, (A0)+
003F6C: 20C0                move.l  D0, (A0)+
003F6E: 20C0                move.l  D0, (A0)+
003F70: 20C0                move.l  D0, (A0)+
003F72: 20C0                move.l  D0, (A0)+
003F74: 20C0                move.l  D0, (A0)+
003F76: 20C0                move.l  D0, (A0)+
003F78: 20C0                move.l  D0, (A0)+
003F7A: 20C0                move.l  D0, (A0)+
003F7C: 20C0                move.l  D0, (A0)+
003F7E: 20C0                move.l  D0, (A0)+
003F80: 20C0                move.l  D0, (A0)+
003F82: 20C0                move.l  D0, (A0)+
003F84: 20C0                move.l  D0, (A0)+
003F86: 20C0                move.l  D0, (A0)+
003F88: 3080                move.w  D0, (A0)
003F8A: 90FC 00AE           suba.w  #$ae, A0
003F8E: 4E75                rts
003F90: 4EB9 0000 9F88      jsr     $9f88.l
003F96: 4EB9 0001 64D0      jsr     $164d0.l
003F9C: 4E75                rts
003F9E: 4BF9 0010 8000      lea     $108000.l, A5
003FA4: 4DFA FEF4           lea     (-$10c,PC) ; ($3e9a), A6
003FA8: 205E                movea.l (A6)+, A0
003FAA: 6100 FF78           bsr     $3f24
003FAE: 209E                move.l  (A6)+, (A0)
003FB0: 315E 006E           move.w  (A6)+, ($6e,A0)
003FB4: 315E 0070           move.w  (A6)+, ($70,A0)
003FB8: 315E 0072           move.w  (A6)+, ($72,A0)
003FBC: 315E 0074           move.w  (A6)+, ($74,A0)
003FC0: 42A8 000C           clr.l   ($c,A0)
003FC4: 4268 0010           clr.w   ($10,A0)
003FC8: 2148 0004           move.l  A0, ($4,A0)
003FCC: 2148 0008           move.l  A0, ($8,A0)
003FD0: 2E1E                move.l  (A6)+, D7
003FD2: 6700 003A           beq     $400e
003FD6: 2248                movea.l A0, A1
003FD8: 2047                movea.l D7, A0
003FDA: 6100 FF48           bsr     $3f24
003FDE: 42A8 000C           clr.l   ($c,A0)
003FE2: 4268 0010           clr.w   ($10,A0)
003FE6: 209E                move.l  (A6)+, (A0)
003FE8: 315E 006E           move.w  (A6)+, ($6e,A0)
003FEC: 315E 0070           move.w  (A6)+, ($70,A0)
003FF0: 315E 0072           move.w  (A6)+, ($72,A0)
003FF4: 315E 0074           move.w  (A6)+, ($74,A0)
003FF8: 2469 0008           movea.l ($8,A1), A2
003FFC: 2548 0004           move.l  A0, ($4,A2)
004000: 2348 0008           move.l  A0, ($8,A1)
004004: 2149 0004           move.l  A1, ($4,A0)
004008: 214A 0008           move.l  A2, ($8,A0)
00400C: 60C2                bra     $3fd0
00400E: 41F9 0010 16D4      lea     $1016d4.l, A0
004014: 3B7C 0000 E3D8      move.w  #$0, (-$1c28,A5)
00401A: 2B48 E3D4           move.l  A0, (-$1c2c,A5)
00401E: 4EBA FF04           jsr     (-$fc,PC) ; ($3f24)
004022: 20BC 0000 3F1E      move.l  #$3f1e, (A0)
004028: 526D E3D8           addq.w  #1, (-$1c28,A5)
00402C: 43E8 00B0           lea     ($b0,A0), A1
004030: B3FC 0010 63D4      cmpa.l  #$1063d4, A1
004036: 6400 000A           bcc     $4042
00403A: 2149 0008           move.l  A1, ($8,A0)
00403E: 2049                movea.l A1, A0
004040: 60DC                bra     $401e
004042: 0C6D 0070 E3D8      cmpi.w  #$70, (-$1c28,A5)
004048: 6700 0004           beq     $404e
00404C: 4E4F                trap    #$f
00404E: 217C 0000 0000 0008 move.l  #$0, ($8,A0)
004056: 426D E3DA           clr.w   (-$1c26,A5)
00405A: 41F9 0010 16D4      lea     $1016d4.l, A0
004060: 3E3C 0057           move.w  #$57, D7
004064: 313C 0000           move.w  #$0, -(A0)
004068: 51CF FFFA           dbra    D7, $4064
00406C: 20BC 0000 3F1E      move.l  #$3f1e, (A0)
004072: 217C 0010 1624 0004 move.l  #$101624, ($4,A0)
00407A: 217C 0010 1624 0008 move.l  #$101624, ($8,A0)
004082: 217C 0010 1624 000C move.l  #$101624, ($c,A0)
00408A: 4E75                rts
00408C: 206E 0008           movea.l ($8,A6), A0
004090: 302E 0010           move.w  ($10,A6), D0
004094: B068 0010           cmp.w   ($10,A0), D0
004098: 4E75                rts
