/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08d1d0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8d1d0(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  byte in_D0b;
  ushort unaff_D3w;
  int extraout_A0;
  int unaff_A5;
  int unaff_A6;
  undefined1 uStack_d;
  undefined4 uStack_c;

  if ((in_D0b & 3) == 0) {
    return;
  }
  uStack_c = CONCAT22(unaff_D3w,uStack_c._2_2_);
  candidate_57948();
  candidate_120f8();
  candidate_411c();
  *(undefined1 *)(extraout_A0 + 0x87) = 0;
  *(byte *)(extraout_A0 + 0x71) = (in_D0b & 3) - 1 & 1;
  *(undefined1 *)(extraout_A0 + 0x70) = uStack_d;
  iVar1 = *(int *)(unaff_A6 + 0x44);
  iVar2 = *(int *)(unaff_A6 + 0x48);
  iVar5 = (*(int *)(unaff_A6 + 0x3c) + (uint)unaff_D3w * 0x10000) - *(int *)(unaff_A5 + 0x697c);
  iVar6 = (*(int *)(unaff_A6 + 0x40) + uStack_c * 0x10000) - *(int *)(unaff_A5 + 0x6980);
  iVar3 = *(int *)(unaff_A5 + 0x6984);
  iVar4 = *(int *)(unaff_A5 + 0x6988);
  *(int *)(extraout_A0 + 0x28) = iVar5;
  *(int *)(extraout_A0 + 0x2c) = iVar6;
  *(uint *)(extraout_A0 + 0x30) = -(((iVar1 + (uint)unaff_D3w * 0x10000) - iVar3) - iVar5);
  *(int *)(extraout_A0 + 0x34) = -(((iVar2 + uStack_c * 0x10000) - iVar4) - iVar6);
  *(int *)(extraout_A0 + 0x38) = unaff_A5 + 0x6968;
  candidate_1abc8();
  return;
}
