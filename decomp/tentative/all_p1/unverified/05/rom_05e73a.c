/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05e73a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5e73a(void)

{
  int iVar1;
  int unaff_A6;

  iVar1 = *(int *)(unaff_A6 + 0x62);
  if ((*(byte *)(unaff_A6 + 0x93) & 1) == 0) {
    if (iVar1 < 0) {
      iVar1 = -iVar1;
    }
  }
  else if (-1 < iVar1) {
    iVar1 = -iVar1;
  }
  *(int *)(unaff_A6 + 0x62) = iVar1;
  iVar1 = iVar1 >> 1;
  if (iVar1 < 0) {
    iVar1 = -iVar1;
  }
  *(short *)(unaff_A6 + 0x26) = (short)((uint)iVar1 >> 8);
  return;
}
