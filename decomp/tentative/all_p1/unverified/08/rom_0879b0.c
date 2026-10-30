/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0879b0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_879b0(void)

{
  int iVar1;
  short sVar2;
  int unaff_A6;
  bool bVar3;

  iVar1 = -*(int *)(unaff_A6 + 0x62);
  *(int *)(unaff_A6 + 0x62) = iVar1;
  bVar3 = iVar1 != 0;
  candidate_87964();
  if (bVar3) {
    *(int *)(unaff_A6 + 0x62) = -*(int *)(unaff_A6 + 0x62);
    return;
  }
  *(int *)(unaff_A6 + 0x62) = -*(int *)(unaff_A6 + 0x62);
  if ((*(byte *)(unaff_A6 + 0x8b) & 0x20) != 0) {
    sVar2 = *(short *)(unaff_A6 + 0x3c) - *(short *)(*(int *)(unaff_A6 + 0x6e) + 0x3c);
    if (sVar2 < 0) {
      sVar2 = -sVar2;
    }
    if (sVar2 < 0x28) {
      return;
    }
  }
  return;
}
