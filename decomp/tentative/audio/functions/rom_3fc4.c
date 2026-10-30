/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x3fc4; tentative_c. */

void z80_candidate_3fc4(short param_1)

{
  short sVar1;
  undefined2 unaff_BC_;
  short unaff_HL_;

  *(char *)(param_1 + 0x11) = (char)((ushort)unaff_BC_ >> 8);
  sVar1 = unaff_HL_ + 1;
  z80_candidate_214c(param_1 + 0x1b);
  *(short *)(param_1 + 0x12) = sVar1 + 6;
  return;
}
