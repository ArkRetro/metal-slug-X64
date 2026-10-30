/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0b21a8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_b21a8(void)

{
  short sVar1;
  int iVar2;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar3;

  *(undefined2 *)((int)unaff_A6 + 0x6e) = *(undefined2 *)(unaff_A5 + 0x57ec);
  unaff_A6[0x1c] = 0xfffff800;
  *unaff_A6 = 0xb21c0;
  iVar2 = unaff_A6[0x1c] + *(int *)(unaff_A5 + 0x694e);
  if (iVar2 < -0x7ffff) {
    iVar2 = -0x80000;
  }
  *(int *)(unaff_A5 + 0x694e) = iVar2;
  bVar3 = *(short *)((int)unaff_A6 + 0x6e) < 0;
  candidate_c6f7a();
  if (bVar3) {
    *(short *)((int)unaff_A6 + 0x6e) = *(short *)((int)unaff_A6 + 0x6e) + 0x14;
    *(short *)(extraout_A0 + 0x32) = (short)((*(short *)(extraout_A0 + 0x32) + 0x14) * 0x10) >> 4;
    *(short *)(extraout_A0 + 0x34) = (short)(*(short *)(extraout_A0 + 0x34) << 4) >> 4;
    if (*(int *)(extraout_A0 + 0x2e) == 0) {
      *(short *)(extraout_A0 + 0x36) = (short)(-0x400000 - *(int *)(extraout_A0 + 0x14) >> 0x14);
      *(short *)(extraout_A0 + 0x38) = (short)(0x800000 - *(int *)(extraout_A0 + 0x18) >> 0x14);
      return;
    }
    sVar1 = (short)(-0x400000 - *(int *)(extraout_A0 + 0x14) >> 0x14);
    *(short *)(extraout_A0 + 0x36) = sVar1;
    iVar2 = extraout_A0;
    if ((*(short *)(extraout_A0 + 0x3a) != sVar1) &&
       (candidate_c6bde(), iVar2 = extraout_A0_00, (*(byte *)(extraout_A0_00 + 0x82) & 2) != 0)) {
      func_0x000c714e();
      iVar2 = extraout_A0_01;
    }
    if (((*(byte *)(iVar2 + 0x82) & 4) != 0) &&
       (sVar1 = (short)(0x800000 - *(int *)(iVar2 + 0x18) >> 0x14), *(short *)(iVar2 + 0x38) = sVar1
       , *(short *)(iVar2 + 0x3c) != sVar1)) {
      func_0x000c714e();
    }
    return;
  }
  return;
}
