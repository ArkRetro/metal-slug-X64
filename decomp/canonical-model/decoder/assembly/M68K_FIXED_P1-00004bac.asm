004BAC: 4A2D E7E2           tst.b   (-$181e,A5)
004BB0: 6600 0026           bne     $4bd8
004BB4: 0839 0000 0010 F001 btst    #$0, $10f001.l
004BBC: 6700 0016           beq     $4bd4
004BC0: 1039 0010 C35D      move.b  $10c35d.l, D0
004BC6: 8039 0010 C363      or.b    $10c363.l, D0
004BCC: 0200 0080           andi.b  #$80, D0
004BD0: 6600 0006           bne     $4bd8
004BD4: 6000 0022           bra     $4bf8
004BD8: 0C2B 0001 0001      cmpi.b  #$1, ($1,A3)
004BDE: 6700 0018           beq     $4bf8
004BE2: 426E 004E           clr.w   ($4e,A6)
004BE6: 08EE 0007 005F      bset    #$7, ($5f,A6)
004BEC: 1D7C 0006 005A      move.b  #$6, ($5a,A6)
004BF2: 003C 0001           ori     #$1, CCR
004BF6: 4E75                rts
004BF8: 023C 00FE           andi    #$fe, CCR
004BFC: 4E75                rts
