/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x367c; tentative_c. */

void z80_candidate_367c(undefined2 param_1,short param_2)

{
  short sVar1;
  char cVar2;
  undefined1 *puVar3;
  undefined1 *puVar4;
  undefined1 *puVar5;
  short unaff_HL_;
  short sVar6;

  puVar4 = (undefined1 *)(unaff_HL_ + 6);
  puVar5 = (undefined1 *)(param_2 + 0x14);
  cVar2 = '\x04';
  do {
    sVar1 = 7;
    do {
      puVar3 = puVar5;
      *puVar3 = *puVar4;
      puVar4 = puVar4 + 1;
      sVar1 = sVar1 + -1;
      puVar5 = puVar3 + 1;
    } while (sVar1 != 0);
    puVar3[1] = 0x7f;
    puVar5 = puVar3 + 3;
    cVar2 = cVar2 + -1;
  } while (cVar2 != '\0');
  *(undefined1 *)(param_2 + 0x1c) = *(undefined1 *)(param_2 + 0x15);
  *(undefined1 *)(param_2 + 0x25) = *(undefined1 *)(param_2 + 0x1e);
  *(undefined1 *)(param_2 + 0x2e) = *(undefined1 *)(param_2 + 0x27);
  *(undefined1 *)(param_2 + 0x37) = *(undefined1 *)(param_2 + 0x30);
  *(byte *)(param_2 + 0x13) = *(byte *)(unaff_HL_ + 1) & 0xf0;
  *(byte *)(param_2 + 0x39) = *(byte *)(unaff_HL_ + 2) | 0x80;
  if ((*(byte *)(unaff_HL_ + 3) & 8) == 0) {
    *(byte *)(param_2 + 0x38) = *(byte *)(unaff_HL_ + 4) & 0xc0;
  }
  else {
    DAT_ram_faa7 = *(byte *)(unaff_HL_ + 3);
    *(undefined1 *)(param_2 + 0x38) = *(undefined1 *)(unaff_HL_ + 4);
  }
  *(undefined1 *)(param_2 + 0x3c) = 0;
  *(undefined1 *)(param_2 + 0x11) = *(undefined1 *)(unaff_HL_ + 5);
  sVar6 = unaff_HL_ + 0x22;
  sVar1 = param_2 + 0x3f;
  z80_candidate_2259(sVar6,sVar1,param_2,param_1);
  sVar6 = sVar6 + 6;
  sVar1 = sVar1 + 0xe;
  z80_candidate_2259(sVar6,sVar1);
  z80_candidate_23df(sVar6 + 6,sVar1 + 0xe);
  return;
}
