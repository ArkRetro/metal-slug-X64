03D1B8: 1418      move.b  (A0)+, D2
03D1BA: 0C02 005C cmpi.b  #$5c, D2
03D1BE: 6700 0004 beq     $3d1c4
03D1C2: 4E75      rts
03D1C4: 1418      move.b  (A0)+, D2
03D1C6: 0C02 006E cmpi.b  #$6e, D2
03D1CA: 6700 0034 beq     $3d200
03D1CE: 0C02 0063 cmpi.b  #$63, D2
03D1D2: 6700 0024 beq     $3d1f8
03D1D6: 0C02 0070 cmpi.b  #$70, D2
03D1DA: 6700 000C beq     $3d1e8
03D1DE: 0C02 006C cmpi.b  #$6c, D2
03D1E2: 6700 002C beq     $3d210
03D1E6: 4E75      rts
03D1E8: 0241 00FF andi.w  #$ff, D1
03D1EC: 1E18      move.b  (A0)+, D7
03D1EE: 0207 000F andi.b  #$f, D7
03D1F2: E14F      lsl.w   #8, D7
03D1F4: 8247      or.w    D7, D1
03D1F6: 60C0      bra     $3d1b8
03D1F8: 0241 FF00 andi.w  #$ff00, D1
03D1FC: 1218      move.b  (A0)+, D1
03D1FE: 60B8      bra     $3d1b8
03D200: 5243      addq.w  #1, D3
03D202: 0C43 7500 cmpi.w  #$7500, D3
03D206: 6500 0004 bcs     $3d20c
03D20A: 4E4F      trap    #$f
03D20C: 3003      move.w  D3, D0
03D20E: 60A8      bra     $3d1b8
03D210: 7600      moveq   #$0, D3
03D212: 1618      move.b  (A0)+, D3
03D214: EB4B      lsl.w   #5, D3
03D216: D618      add.b   (A0)+, D3
03D218: 0643 7000 addi.w  #$7000, D3
03D21C: 0C43 7500 cmpi.w  #$7500, D3
03D220: 6500 0004 bcs     $3d226
03D224: 4E4F      trap    #$f
03D226: 3003      move.w  D3, D0
03D228: 608E      bra     $3d1b8
