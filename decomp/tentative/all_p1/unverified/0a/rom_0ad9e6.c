/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ad9e6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_ad9e6(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int extraout_A0;
  int unaff_A5;
  int unaff_A6;

  candidate_411c();
  iVar1 = *(int *)(unaff_A6 + 0x44);
  iVar2 = *(int *)(unaff_A6 + 0x48);
  iVar5 = *(int *)(unaff_A6 + 0x3c) - *(int *)(unaff_A5 + 0x697c);
  iVar6 = *(int *)(unaff_A6 + 0x40) - *(int *)(unaff_A5 + 0x6980);
  iVar3 = *(int *)(unaff_A5 + 0x6984);
  iVar4 = *(int *)(unaff_A5 + 0x6988);
  *(int *)(extraout_A0 + 0x28) = iVar5;
  *(int *)(extraout_A0 + 0x2c) = iVar6;
  *(int *)(extraout_A0 + 0x30) = -((iVar1 - iVar3) - iVar5);
  *(int *)(extraout_A0 + 0x34) = -((iVar2 - iVar4) - iVar6);
  *(int *)(extraout_A0 + 0x38) = unaff_A5 + 0x6968;
  candidate_1abc8();
  return;
}
