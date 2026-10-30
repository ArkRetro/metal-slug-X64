/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0893d0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_893d0(void)

{
  int extraout_A0;
  int unaff_A6;
  undefined1 in_CF;

  candidate_1affc();
  if (!(bool)in_CF) {
    if (*(short *)(unaff_A6 + 0x3c) < *(short *)(extraout_A0 + 0x3c)) {
      if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
        return;
      }
    }
    else if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
      return;
    }
  }
  return;
}
