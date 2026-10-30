/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x039eec.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_39eec(void)

{
  int iVar1;
  int unaff_A6;

  iVar1 = (uint)*(ushort *)(unaff_A6 + 0x86) * 0x100;
  if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
    iVar1 = (uint)*(ushort *)(unaff_A6 + 0x86) * -0x100;
  }
  *(int *)(unaff_A6 + 0x62) = iVar1;
  return;
}
