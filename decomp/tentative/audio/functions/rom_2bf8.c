/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x2bf8; tentative_c. */

void z80_candidate_2bf8(undefined2 param_1,short param_2)

{
  byte bVar1;
  char cVar2;
  undefined2 uVar3;
  short sVar4;
  short unaff_HL_;
  short sVar5;

  bVar1 = *(byte *)(unaff_HL_ + 1);
  *(byte *)(param_2 + 0x11) = bVar1;
  *(char *)(param_2 + 0x12) = -CARRY1(bVar1,bVar1);
  cVar2 = *(char *)(unaff_HL_ + 2);
  *(char *)(param_2 + 0x13) = cVar2;
  bVar1 = -(cVar2 == '\0') ^ 0x38;
  DAT_ram_f905 = (bVar1 ^ DAT_ram_f905 | *(byte *)(param_2 + 0x14)) ^ bVar1;
  sVar4 = unaff_HL_ + 3;
  uVar3 = 0x16;
  sVar5 = param_2 + 0x16;
  z80_candidate_214c(sVar4,sVar5,param_2);
  sVar5 = sVar5 + 0xe;
  z80_candidate_2259(uVar3,sVar4,sVar5,param_1);
  sVar4 = sVar4 + 6;
  sVar5 = sVar5 + 0xe;
  z80_candidate_2259(sVar4,sVar5);
  z80_candidate_23df(sVar4 + 6,sVar5 + 0xe);
  return;
}
