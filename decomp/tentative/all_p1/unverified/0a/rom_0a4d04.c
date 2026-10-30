/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a4d04.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a4d04(void)

{
  short sVar1;
  undefined4 *unaff_A6;

  candidate_4192();
  candidate_1abc8();
  candidate_120f8();
  candidate_a5634();
  candidate_a5604();
  candidate_c801a();
  candidate_a565e();
  *(undefined2 *)((int)unaff_A6 + 0x9e) = 0x1e;
  *unaff_A6 = 0xa4d3c;
  candidate_1abc8();
  sVar1 = *(short *)((int)unaff_A6 + 0x9e) + -1;
  *(short *)((int)unaff_A6 + 0x9e) = sVar1;
  if (sVar1 < 0) {
    *unaff_A6 = candidate_a4d52;
  }
  return;
}
