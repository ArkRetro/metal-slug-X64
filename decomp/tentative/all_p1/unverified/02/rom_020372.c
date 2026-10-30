/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x020372.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_20372(void)

{
  undefined4 *unaff_A6;

  candidate_9f88();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8801;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8801;
  candidate_9ea8();
  candidate_4256();
  candidate_1ad6e();
  *unaff_A6 = 0x203b6;
  if ((*(byte *)(unaff_A6[3] + 0x15) & 1) != 0) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  candidate_1abc8();
  candidate_1ad8c();
  candidate_427a();
  return;
}
