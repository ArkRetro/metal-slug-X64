/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06b416.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6b416(void)

{
  int iVar1;
  int unaff_A6;

  iVar1 = *(int *)(unaff_A6 + 0x62);
  if (iVar1 == 0) {
    return;
  }
  if (iVar1 < 0) {
    iVar1 = -iVar1;
  }
  *(short *)(unaff_A6 + 0x26) = (short)(iVar1 / 0x100);
  return;
}
