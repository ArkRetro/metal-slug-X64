/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x020f02.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_20f02(void)

{
  undefined4 *unaff_A6;

  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xa0;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xa0;
  candidate_9f88();
  candidate_9ea8();
  candidate_4256();
  *(undefined2 *)(unaff_A6 + 10) = *(undefined2 *)(unaff_A6[3] + 0x28);
  *(undefined2 *)(unaff_A6 + 0xb) = *(undefined2 *)(unaff_A6[3] + 0x2c);
  *(short *)(unaff_A6 + 10) = *(short *)(unaff_A6 + 10) + -0x20;
  *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) + 0x20;
  *unaff_A6 = candidate_20f66;
  return;
}
