/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01ef2e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1ef2e(void)

{
  ushort uVar2;
  short sVar3;
  int iVar1;
  int unaff_A6;

  candidate_1b1dc();
  uVar2 = candidate_29c6();
  sVar3 = (uVar2 & 7) * 0xc;
  *(undefined4 *)(unaff_A6 + 0x34) = *(undefined4 *)(sVar3 + 0x1f16e);
  *(undefined4 *)(unaff_A6 + 0x72) = *(undefined4 *)(sVar3 + 0x1f172);
  iVar1 = *(int *)(sVar3 + 0x1f176);
  if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
    iVar1 = -iVar1;
  }
  *(int *)(unaff_A6 + 0x30) = iVar1;
  return;
}
