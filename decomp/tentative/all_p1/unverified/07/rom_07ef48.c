/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07ef48.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7ef48(void)

{
  short sVar1;
  int extraout_A0;
  int unaff_A6;
  undefined1 in_CF;

  candidate_1af9a();
  if (!(bool)in_CF) {
    sVar1 = *(short *)(unaff_A6 + 0x3c) - *(short *)(extraout_A0 + 0x3c);
    if (sVar1 < 0) {
      sVar1 = -sVar1;
    }
    if ((*(short *)(unaff_A6 + 0x72) != 0) && (sVar1 <= *(short *)(unaff_A6 + 0x72))) {
      return;
    }
  }
  return;
}
