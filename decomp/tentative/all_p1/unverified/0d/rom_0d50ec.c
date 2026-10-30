/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d50ec.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d50ec(void)

{
  int unaff_A6;

  *(short *)(unaff_A6 + 0x76) = *(short *)(unaff_A6 + 0x76) + -1;
  if (*(short *)(unaff_A6 + 0x76) < 1) {
    candidate_d512e();
  }
  *(short *)(unaff_A6 + 0x78) = *(short *)(unaff_A6 + 0x78) + -1;
  if (*(short *)(unaff_A6 + 0x78) < 1) {
    if (*(char *)(unaff_A6 + 0x60) == '\x01') {
      candidate_d5396();
    }
    else {
      candidate_d5298();
    }
  }
  return;
}
