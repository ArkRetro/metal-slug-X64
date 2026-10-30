/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01acf2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1acf2(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int in_D0;
  int in_D1;
  int in_A0;
  int unaff_A5;
  int unaff_A6;

  iVar1 = *(int *)(unaff_A6 + 0x44);
  iVar2 = *(int *)(unaff_A6 + 0x48);
  iVar5 = (*(int *)(unaff_A6 + 0x3c) + in_D0 * 0x10000) - *(int *)(unaff_A5 + 0x697c);
  iVar6 = (*(int *)(unaff_A6 + 0x40) + in_D1 * 0x10000) - *(int *)(unaff_A5 + 0x6980);
  iVar3 = *(int *)(unaff_A5 + 0x6984);
  iVar4 = *(int *)(unaff_A5 + 0x6988);
  *(int *)(in_A0 + 0x28) = iVar5;
  *(int *)(in_A0 + 0x2c) = iVar6;
  *(int *)(in_A0 + 0x30) = -(((iVar1 + in_D0 * 0x10000) - iVar3) - iVar5);
  *(int *)(in_A0 + 0x34) = -(((iVar2 + in_D1 * 0x10000) - iVar4) - iVar6);
  *(int *)(in_A0 + 0x38) = unaff_A5 + 0x6968;
  candidate_1abc8();
  return;
}
