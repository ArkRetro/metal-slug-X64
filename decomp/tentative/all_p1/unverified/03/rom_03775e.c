/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03775e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3775e(void)

{
  int iVar1;
  ushort uVar2;
  ushort uVar3;
  ushort uVar4;
  short sVar5;
  int *piVar6;
  int unaff_A6;
  bool bVar7;

  bVar7 = false;
  if (((-1 < *(int *)(unaff_A6 + 0x72)) && (func_0x000040ac(), bVar7)) &&
     ((*(byte *)(*(int *)(unaff_A6 + 0x72) + 0x5f) & 0x80) == 0)) {
    return;
  }
  *(undefined4 *)(unaff_A6 + 0x72) = 0xffffffff;
  uVar4 = 0xffff;
  sVar5 = 1;
  bVar7 = false;
  piVar6 = (int *)(*(int *)(unaff_A6 + 0xc) + 0x76);
  while (!bVar7) {
    iVar1 = *piVar6;
    if (iVar1 != 0) {
      uVar2 = *(short *)(iVar1 + 0x28) - *(short *)(unaff_A6 + 0x28);
      uVar3 = uVar2;
      if ((short)uVar2 < 0) {
        uVar3 = -uVar2;
      }
      if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
        uVar2 = -uVar2;
      }
      if (((short)uVar2 < 0) && (uVar3 <= uVar4)) {
        *(int *)(unaff_A6 + 0x72) = iVar1;
        uVar4 = uVar3;
      }
    }
    sVar5 = sVar5 + -1;
    piVar6 = piVar6 + 1;
    bVar7 = sVar5 == 0;
  }
  return;
}
