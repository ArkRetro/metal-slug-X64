/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a047e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a047e(void)

{
  int iVar1;
  short sVar2;
  int unaff_A6;

  iVar1 = *(int *)(unaff_A6 + 0x62);
  if (iVar1 < 0) {
    iVar1 = -iVar1;
  }
  sVar2 = (short)((uint)iVar1 >> 8);
  if (sVar2 < 0x100) {
    sVar2 = 0x100;
  }
  if (0x400 < sVar2) {
    sVar2 = 0x400;
  }
  *(short *)(unaff_A6 + 0x26) = sVar2;
  return;
}
