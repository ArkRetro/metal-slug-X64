03CF9A: 4840                swap    D0
03CF9C: 3001                move.w  D1, D0
03CF9E: E908                lsl.b   #4, D0
03CFA0: E948                lsl.w   #4, D0
03CFA2: 1002                move.b  D2, D0
03CFA4: 23C0 003C 0000      move.l  D0, $3c0000.l
03CFAA: 4840                swap    D0
03CFAC: 0640 0020           addi.w  #$20, D0
03CFB0: 33FC 8200 003C 0000 move.w  #$8200, $3c0000.l
03CFB8: 4E75                rts
