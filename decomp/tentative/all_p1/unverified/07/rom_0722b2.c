/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0722b2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_722b2(void)

{
  int unaff_A6;
  bool bVar1;

  bVar1 = false;
  if (*(short *)(unaff_A6 + 0x7a) == 0) {
    candidate_723c6();
    if (bVar1) {
      bVar1 = false;
      candidate_40bc();
      if (bVar1) {
        candidate_1b220();
      }
      *(undefined2 *)(unaff_A6 + 0x7a) = 3;
    }
  }
  else {
    *(short *)(unaff_A6 + 0x7a) = *(short *)(unaff_A6 + 0x7a) + -1;
  }
  return;
}
