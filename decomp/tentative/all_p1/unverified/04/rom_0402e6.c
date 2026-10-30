/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0402e6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

uint candidate_402e6(void)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  int *extraout_A1;
  undefined4 extraout_A1_00;
  int unaff_A5;
  int unaff_A6;
  undefined8 uVar6;

  candidate_1ad8c();
  candidate_401ae();
  if (extraout_A1 == (int *)0x0) {
    candidate_1436a();
    return 0x8000;
  }
  iVar5 = *(int *)(unaff_A6 + 0x34);
  uVar6 = candidate_1ac4c();
  iVar1 = *extraout_A1;
  iVar2 = extraout_A1[1];
  *(undefined8 *)(unaff_A6 + 0x28) = uVar6;
  *(int *)(unaff_A6 + 0x30) =
       (iVar1 - (int)((ulonglong)uVar6 >> 0x20)) - ((iVar5 + (int)uVar6) - iVar2 >> 3);
  *(int *)(unaff_A6 + 0x34) = iVar5;
  *(undefined4 *)(unaff_A6 + 0x38) = extraout_A1_00;
  func_0x000143e4();
  uVar3 = (uint)*(byte *)(extraout_A1 + 2) * 0x10000;
  uVar4 = (*(int *)(unaff_A5 + 0x6980) + extraout_A1[1] + uVar3) - *(int *)(unaff_A6 + 0x40);
  if (uVar4 != uVar3 &&
      SBORROW4(uVar4,uVar3) == (int)(uVar4 + (uint)*(byte *)(extraout_A1 + 2) * -0x10000) < 0) {
    uVar4 = 0xffffffff;
  }
  return uVar4 >> 0x10;
}
