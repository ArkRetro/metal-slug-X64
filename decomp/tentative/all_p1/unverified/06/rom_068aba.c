/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x068aba.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_68aba(void)

{
  int extraout_A0;
  int unaff_A6;
  bool bVar1;

  bVar1 = *(ushort *)(unaff_A6 + 0x6e) < 2;
  if ((*(ushort *)(unaff_A6 + 0x6e) != 2) && (candidate_1af9a(), !bVar1)) {
    *(int *)(unaff_A6 + 0x86) = extraout_A0;
    if (*(short *)(extraout_A0 + 0x3c) < *(short *)(unaff_A6 + 0x3c)) {
      if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
        return;
      }
    }
    else if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
      return;
    }
  }
  return;
}
