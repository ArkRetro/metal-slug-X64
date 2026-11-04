01A918: 49F9 003C 0000      lea     $3c0000.l, A4
01A91E: 2E3C 8201 0000      move.l  #$82010000, D7
01A924: 2C3C 8401 F000      move.l  #$8401f000, D6
01A92A: 2A3C 8001 FFFF      move.l  #$8001ffff, D5
01A930: 7801                moveq   #$1, D4
01A932: 4844                swap    D4
01A934: 303C 017B           move.w  #$17b, D0
01A938: 2887                move.l  D7, (A4)
01A93A: DE84                add.l   D4, D7
01A93C: 13C0 0030 0001      move.b  D0, $300001.l
01A942: 2886                move.l  D6, (A4)
01A944: DC84                add.l   D4, D6
01A946: 13C0 0030 0001      move.b  D0, $300001.l
01A94C: 2885                move.l  D5, (A4)
01A94E: DA84                add.l   D4, D5
01A950: 13C0 0030 0001      move.b  D0, $300001.l
01A956: 51C8 FFE0           dbra    D0, $1a938
01A95A: 4E75                rts
01A95C: 4EB9 0000 9FD2      jsr     $9fd2.l
01A962: 4EB9 000C 7A56      jsr     $c7a56.l
01A968: 61AE                bsr     $1a918
01A96A: 4279 0010 C334      clr.w   $10c334.l
01A970: 33FC 017C 0010 C336 move.w  #$17c, $10c336.l
01A978: 4E75                rts
01A97A: 4C90 0003           movem.w (A0), D0-D1
01A97E: 4870 0000           pea     (A0,D0.w)
01A982: 0C41 0001           cmpi.w  #$1, D1
01A986: 6600 0034           bne     $1a9bc
01A98A: 3028 0004           move.w  ($4,A0), D0
01A98E: 6100 0164           bsr     $1aaf4
01A992: 0C40 0000           cmpi.w  #$0, D0
01A996: 6700 0018           beq     $1a9b0
01A99A: 4E4F                trap    #$f
01A99C: 7472                moveq   #$72, D2
01A99E: 6170                bsr     $1aa10
01A9A0: 2073 7072           movea.l ($72,A3,D7.w), A0
01A9A4: 6974                bvs     $1aa1a
01A9A6: 6520                bcs     $1a9c8
01A9A8: 6173                bsr     $1aa1d
