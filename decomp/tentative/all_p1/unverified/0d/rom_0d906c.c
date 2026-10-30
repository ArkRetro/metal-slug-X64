/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d906c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d906c(void)

{
  short in_D0w;
  short in_D1w;
  int unaff_A6;

  if (in_D1w == 0) {
    __m68k_trap(0xf);
  }
  *(short *)(unaff_A6 + 0xa8) = in_D1w + *(short *)(unaff_A6 + 0xa8);
  if (in_D1w < 1) {
    if (*(short *)(unaff_A6 + 0xa8) <= in_D0w) {
      *(short *)(unaff_A6 + 0xa8) = in_D0w;
      return;
    }
  }
  else if (in_D0w <= *(short *)(unaff_A6 + 0xa8)) {
    *(short *)(unaff_A6 + 0xa8) = in_D0w;
    return;
  }
  return;
}
