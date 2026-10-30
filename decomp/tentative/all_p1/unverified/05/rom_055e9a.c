/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x055e9a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_55e9a(void)

{
  short sVar1;
  int unaff_A6;

  sVar1 = 8;
  if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
    sVar1 = -8;
  }
  *(short *)(unaff_A6 + 0x28) = sVar1 + *(short *)(unaff_A6 + 0x28);
  if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
    *(ushort *)(unaff_A6 + 0x28) = *(ushort *)(unaff_A6 + 0x28) & 0xfff8;
  }
  else {
    *(ushort *)(unaff_A6 + 0x28) = *(ushort *)(unaff_A6 + 0x28) | 7;
  }
  return;
}
