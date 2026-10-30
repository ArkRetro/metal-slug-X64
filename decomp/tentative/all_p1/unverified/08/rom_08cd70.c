/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08cd70.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8cd70(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  ushort in_D0w;
  undefined2 in_D1w;
  ushort unaff_D2w;
  int extraout_A0;
  int unaff_A5;
  int unaff_A6;
  undefined2 uStack_16;
  undefined2 uStack_12;
  undefined2 uStack_e;

  if (0x59 < unaff_D2w) {
    return;
  }
  candidate_40bc();
  *(char *)(extraout_A0 + 0x8b) = (char)unaff_D2w;
  *(undefined2 *)(extraout_A0 + 0x70) = uStack_12;
  *(undefined2 *)(extraout_A0 + 0x6e) = in_D1w;
  *(undefined2 *)(extraout_A0 + 0x72) = uStack_e;
  *(undefined1 *)(extraout_A0 + 0x7f) = 0;
  iVar1 = *(int *)(unaff_A6 + 0x44);
  iVar2 = *(int *)(unaff_A6 + 0x48);
  iVar5 = (*(int *)(unaff_A6 + 0x3c) + (uint)in_D0w * 0x10000) - *(int *)(unaff_A5 + 0x697c);
  iVar6 = (*(int *)(unaff_A6 + 0x40) + (uint)uStack_16 * 0x10000) - *(int *)(unaff_A5 + 0x6980);
  iVar3 = *(int *)(unaff_A5 + 0x6984);
  iVar4 = *(int *)(unaff_A5 + 0x6988);
  *(int *)(extraout_A0 + 0x28) = iVar5;
  *(int *)(extraout_A0 + 0x2c) = iVar6;
  *(uint *)(extraout_A0 + 0x30) = -(((iVar1 + (uint)in_D0w * 0x10000) - iVar3) - iVar5);
  *(uint *)(extraout_A0 + 0x34) = -(((iVar2 + (uint)uStack_16 * 0x10000) - iVar4) - iVar6);
  *(int *)(extraout_A0 + 0x38) = unaff_A5 + 0x6968;
  candidate_1abc8();
  return;
}
