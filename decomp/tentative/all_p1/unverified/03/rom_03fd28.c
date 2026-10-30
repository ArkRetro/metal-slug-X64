/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03fd28.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3fd28(void)

{
  undefined4 *unaff_A6;

  candidate_9f88();
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0;
  candidate_4256();
  *(undefined2 *)((int)unaff_A6 + 0x72) = 1;
  candidate_1ad6e();
  *unaff_A6 = candidate_3fcfa;
  candidate_1abc8();
  candidate_1ad8c();
  candidate_427a();
  if (*(short *)(unaff_A6 + 0x1c) == 0) {
    *(undefined2 *)(unaff_A6 + 0x1c) = 0x20;
  }
  else {
    *(short *)(unaff_A6 + 0x1c) = *(short *)(unaff_A6 + 0x1c) + -1;
  }
  candidate_3fada();
  return;
}
