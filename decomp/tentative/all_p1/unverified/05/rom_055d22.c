/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x055d22.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_55d22(void)

{
  short sVar1;
  int extraout_A0;
  int unaff_A6;
  undefined1 in_CF;

  candidate_55cde();
  if ((bool)in_CF) {
    sVar1 = *(short *)(extraout_A0 + 0x3c) - *(short *)(unaff_A6 + 0x3c);
    if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
      sVar1 = -sVar1;
    }
    if (sVar1 < 0) {
      return;
    }
  }
  return;
}
