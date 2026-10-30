/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x047a6c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_47a6c(void)

{
  undefined4 *unaff_A6;

  *(undefined2 *)(unaff_A6 + 0x20) = *(undefined2 *)(unaff_A6[3] + 0x80);
  *(undefined1 *)((int)unaff_A6 + 0x96) = *(undefined1 *)(unaff_A6[3] + 0x96);
  if ('\a' < *(char *)((int)unaff_A6 + 0x96)) {
    *(char *)((int)unaff_A6 + 0x96) = -(*(char *)((int)unaff_A6 + 0x96) + -0xe);
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  candidate_4256();
  candidate_9f88();
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8600;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8600;
  candidate_1ad6e();
  *unaff_A6 = candidate_47ae0;
  return;
}
