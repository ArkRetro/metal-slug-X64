/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d7062.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d7062(void)

{
  int unaff_A6;

  if ((*(byte *)(unaff_A6 + 0x77) & 1) == 0) {
    if (*(short *)(unaff_A6 + 0x3c) < 0xa0) {
      return;
    }
  }
  else {
    if (0x118 < *(short *)(unaff_A6 + 0x3c)) {
      return;
    }
    if (*(short *)(unaff_A6 + 0x70) < *(short *)(unaff_A6 + 0x28)) {
      return;
    }
  }
  return;
}
