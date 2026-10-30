/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05e60c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5e60c(void)

{
  int unaff_A6;
  bool bVar1;

  *(short *)(unaff_A6 + 0x2c) = *(short *)(unaff_A6 + 0x2c) + 0x18;
  *(short *)(unaff_A6 + 0x40) = *(short *)(unaff_A6 + 0x40) + 0x18;
  bVar1 = false;
  candidate_14906();
  if (bVar1) {
    *(short *)(unaff_A6 + 0x2c) = *(short *)(unaff_A6 + 0x2c) + -0x18;
    *(short *)(unaff_A6 + 0x40) = *(short *)(unaff_A6 + 0x40) + -0x18;
  }
  else {
    *(short *)(unaff_A6 + 0x2c) = *(short *)(unaff_A6 + 0x2c) + -0x18;
    *(short *)(unaff_A6 + 0x40) = *(short *)(unaff_A6 + 0x40) + -0x18;
  }
  return;
}
