/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x059ee0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_59ee0(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  ushort in_D0w;
  int extraout_A0;
  int unaff_A5;
  int unaff_A6;
  undefined4 uStack_8;

  uStack_8 = CONCAT22(in_D0w,uStack_8._2_2_);
  candidate_411c();
  iVar4 = *(int *)(unaff_A6 + 0x30);
  *(int *)(extraout_A0 + 0x6e) = iVar4 >> 5;
  *(int *)(extraout_A0 + 0x30) = -iVar4;
  iVar4 = *(int *)(unaff_A6 + 0x34);
  *(int *)(extraout_A0 + 0x72) = iVar4 >> 5;
  *(int *)(extraout_A0 + 0x34) = -iVar4;
  iVar4 = *(int *)(unaff_A6 + 0x44);
  iVar1 = *(int *)(unaff_A6 + 0x48);
  iVar5 = (*(int *)(unaff_A6 + 0x3c) + (uint)in_D0w * 0x10000) - *(int *)(unaff_A5 + 0x697c);
  iVar6 = (*(int *)(unaff_A6 + 0x40) + uStack_8 * 0x10000) - *(int *)(unaff_A5 + 0x6980);
  iVar2 = *(int *)(unaff_A5 + 0x6984);
  iVar3 = *(int *)(unaff_A5 + 0x6988);
  *(int *)(extraout_A0 + 0x28) = iVar5;
  *(int *)(extraout_A0 + 0x2c) = iVar6;
  *(uint *)(extraout_A0 + 0x30) = -(((iVar4 + (uint)in_D0w * 0x10000) - iVar2) - iVar5);
  *(int *)(extraout_A0 + 0x34) = -(((iVar1 + uStack_8 * 0x10000) - iVar3) - iVar6);
  *(int *)(extraout_A0 + 0x38) = unaff_A5 + 0x6968;
  candidate_1abc8();
  return;
}
