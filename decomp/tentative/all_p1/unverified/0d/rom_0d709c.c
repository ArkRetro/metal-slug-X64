/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d709c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d709c(void)

{
  int extraout_A0;
  int unaff_A6;
  undefined1 in_CF;

  if ((*(byte *)(unaff_A6 + 0x77) & 1) == 0) {
    candidate_1af9a();
    if ((!(bool)in_CF) &&
       ((short)(*(short *)(unaff_A6 + 0x3c) - *(short *)(extraout_A0 + 0x3c)) < 200)) {
      return;
    }
  }
  return;
}
