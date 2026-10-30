/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a13b6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a13b6(void)

{
  undefined4 *unaff_A6;

  candidate_9bb0();
  candidate_c8022();
  *unaff_A6 = 0xa13cc;
  *(short *)((int)unaff_A6 + 0x82) = *(short *)((int)unaff_A6 + 0x82) + -1;
  if (*(short *)((int)unaff_A6 + 0x82) < 1) {
    candidate_120f8();
    candidate_c8022();
    unaff_A6[0xd] = 0xffff0000;
    *unaff_A6 = 0xa13f8;
  }
  candidate_1abc8();
  candidate_1ac38();
  candidate_427a();
  if (*(short *)(unaff_A6 + 0x10) < 0x71) {
    *(undefined1 *)((int)unaff_A6 + 0x91) = 1;
    *unaff_A6 = candidate_a142a;
  }
  candidate_a2680();
  return;
}
