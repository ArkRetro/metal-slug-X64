/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x090a1e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_90a1e(void)

{
  int unaff_A6;

  if ((*(byte *)(unaff_A6 + 0x15) & 0x40) == 0) {
    if (*(short *)(unaff_A6 + 0x86) != 0) {
      *(short *)(unaff_A6 + 0x86) = *(short *)(unaff_A6 + 0x86) + -1;
    }
  }
  else {
    if (((*(byte *)(unaff_A6 + 0x97) & 0x40) == 0) && (*(short *)(unaff_A6 + 0x86) == 0)) {
      candidate_120f8();
    }
    *(undefined2 *)(unaff_A6 + 0x86) = 5;
  }
  *(undefined1 *)(unaff_A6 + 0x97) = *(undefined1 *)(unaff_A6 + 0x15);
  return;
}
