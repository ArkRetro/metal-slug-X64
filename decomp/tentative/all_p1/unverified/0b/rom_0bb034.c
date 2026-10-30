/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0bb034.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

ushort candidate_bb034(void)

{
  uint uVar1;
  ushort uVar2;
  int unaff_D2;
  int iVar3;
  short sVar4;
  int extraout_A0;
  int extraout_A0_00;
  int in_A1;
  int extraout_A1;
  int extraout_A1_00;
  uint6 uVar5;
  undefined6 uVar6;

  sVar4 = 7;
  do {
    candidate_29c6(sVar4,in_A1);
    candidate_29c6();
    candidate_8cd58();
    uVar5 = candidate_29c6();
    uVar1 = (uint)uVar5 & CONCAT22((short)((uint)unaff_D2 >> 0x10),(short)(uVar5 >> 0x20));
    iVar3 = uVar1 * 2;
    if ((uVar5 & 0x100000000) == 0) {
      iVar3 = uVar1 * -2;
    }
    iVar3 = *(int *)(extraout_A1 + 2) + iVar3;
    *(int *)(extraout_A0 + 0x30) = iVar3;
    uVar6 = candidate_29c6();
    uVar2 = (ushort)((uint6)uVar6 >> 0x20);
    unaff_D2 = *(int *)(extraout_A1_00 + 6) +
               ((uint)uVar6 & CONCAT22((short)((uint)iVar3 >> 0x10),uVar2)) * 2;
    *(int *)(extraout_A0_00 + 0x34) = unaff_D2;
    in_A1 = extraout_A1_00 + 0x12;
    sVar4 = sVar4 + -1;
  } while (sVar4 != -1);
  return uVar2 & 1;
}
