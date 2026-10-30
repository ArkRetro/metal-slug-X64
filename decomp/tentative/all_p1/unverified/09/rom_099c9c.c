/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x099c9c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_99c9c(void)

{
  int iVar1;
  int extraout_A0;
  int unaff_A6;

  candidate_99d12();
  iVar1 = (int)((*(ushort *)(unaff_A6 + 0x72) & 7) << 0x10) >> 1;
  *(undefined4 *)(extraout_A0 + 0x34) = 0x30000;
  *(int *)(extraout_A0 + 0x34) = iVar1 + *(int *)(extraout_A0 + 0x34);
  if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
    iVar1 = -iVar1;
  }
  *(int *)(extraout_A0 + 0x30) = iVar1;
  return;
}
