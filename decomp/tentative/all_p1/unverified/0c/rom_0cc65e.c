/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0cc65e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_cc65e(void)

{
  undefined4 *unaff_A6;

  candidate_9ea8();
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x6e) = 0xd000;
  candidate_4256();
  candidate_1ad6e();
  *(undefined2 *)(unaff_A6 + 0x26) = 5;
  *unaff_A6 = 0xcc6aa;
  candidate_1abc8();
  *(short *)(unaff_A6 + 0x26) = *(short *)(unaff_A6 + 0x26) + -1;
  if (*(short *)(unaff_A6 + 0x26) < 1) {
    *unaff_A6 = candidate_cc6c6;
  }
  return;
}
