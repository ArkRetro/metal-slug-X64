/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x4318; tentative_c. */

void z80_candidate_4318(undefined2 param_1,short param_2)

{
  undefined2 uVar1;
  short sVar2;
  undefined2 unaff_BC_;
  short unaff_HL_;
  short sVar3;

  *(char *)(param_2 + 0x11) = (char)((ushort)unaff_BC_ >> 8);
  sVar2 = unaff_HL_ + 1;
  uVar1 = 0x23;
  sVar3 = param_2 + 0x23;
  z80_candidate_214c(sVar3);
  sVar3 = sVar3 + 0xe;
  z80_candidate_2259(uVar1,sVar3,param_1);
  sVar2 = sVar2 + 6;
  sVar3 = sVar3 + 0xe;
  z80_candidate_2259(sVar3);
  sVar2 = sVar2 + 6;
  z80_candidate_23df(sVar3 + 0xe);
  *(short *)(param_2 + 0x12) = sVar2 + 4;
  return;
}
