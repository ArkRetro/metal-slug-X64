/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x020dd2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_20dd2(void)

{
  int unaff_A6;

  candidate_20994();
  if (*(short *)(unaff_A6 + 0x70) < 0x10) {
    *(short *)(unaff_A6 + 0x2c) =
         *(short *)(unaff_A6 + 0x78) +
         *(short *)((short)(*(short *)(unaff_A6 + 0x70) * 2) + 0x207a6);
    *(short *)(unaff_A6 + 0x70) = *(short *)(unaff_A6 + 0x70) + 1;
  }
  else {
    *(undefined2 *)(unaff_A6 + 0x2c) = *(undefined2 *)(unaff_A6 + 0x78);
  }
  return;
}
