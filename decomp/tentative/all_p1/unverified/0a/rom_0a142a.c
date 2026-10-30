/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a142a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a142a(void)

{
  undefined4 *unaff_A6;

  *(undefined1 *)((int)unaff_A6 + 0x91) = 0;
  candidate_a29ce();
  *(undefined2 *)((int)unaff_A6 + 0x82) = 0x1e;
  *unaff_A6 = 0xa143e;
  candidate_1abc8();
  *(short *)((int)unaff_A6 + 0x82) = *(short *)((int)unaff_A6 + 0x82) + -1;
  if (*(short *)((int)unaff_A6 + 0x82) < 1) {
    *(undefined1 *)((int)unaff_A6 + 0x91) = 1;
    candidate_18f40();
    *unaff_A6 = candidate_a1466;
  }
  return;
}
