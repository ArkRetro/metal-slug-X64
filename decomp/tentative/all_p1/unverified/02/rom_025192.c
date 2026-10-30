/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x025192.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_25192(void)

{
  int unaff_A6;

  if (*(short *)(unaff_A6 + 0x98) == 0) {
    *(undefined1 *)(unaff_A6 + 0x14) = *(undefined1 *)(unaff_A6 + 0x19);
  }
  if (*(short *)(unaff_A6 + 0x98) == 1) {
    *(undefined1 *)(unaff_A6 + 0x14) = *(undefined1 *)(unaff_A6 + 0x1a);
  }
  if (*(short *)(unaff_A6 + 0x98) == 2) {
    *(undefined1 *)(unaff_A6 + 0x14) = *(undefined1 *)(unaff_A6 + 0x1b);
  }
  *(short *)(unaff_A6 + 0x98) = *(short *)(unaff_A6 + 0x98) + 1;
  if (2 < *(short *)(unaff_A6 + 0x98)) {
    *(undefined2 *)(unaff_A6 + 0x98) = 0;
  }
  return;
}
