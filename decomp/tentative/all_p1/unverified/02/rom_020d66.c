/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x020d66.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_20d66(void)

{
  int extraout_A0;
  undefined4 *unaff_A6;

  candidate_20994();
  if (*(short *)((int)unaff_A6 + 0x6e) == 0) {
    if (*(char *)(unaff_A6 + 0x1f) == '\0') {
      candidate_120f8();
      *(undefined1 *)(unaff_A6 + 0x1f) = 0xff;
    }
    *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) + -0x10;
    if (*(short *)(unaff_A6 + 0xb) < 0x12d) {
      *(undefined2 *)(unaff_A6 + 0x1c) = 0;
      *(undefined2 *)(unaff_A6 + 0xb) = 300;
      *(undefined2 *)(unaff_A6 + 0x1e) = *(undefined2 *)(unaff_A6 + 0xb);
      candidate_40bc();
      *(undefined2 *)(extraout_A0 + 0x72) = *(undefined2 *)((int)unaff_A6 + 0x72);
      *unaff_A6 = candidate_20dd2;
    }
  }
  else {
    *(short *)((int)unaff_A6 + 0x6e) = *(short *)((int)unaff_A6 + 0x6e) + -1;
  }
  return;
}
