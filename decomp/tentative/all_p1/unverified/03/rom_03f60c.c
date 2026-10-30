/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03f60c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3f60c(void)

{
  undefined4 *unaff_A6;

  candidate_4256();
  candidate_3faf2();
  unaff_A6[0xc] = 0;
  if (*(short *)((int)unaff_A6 + 0x6e) == 0) {
    *(undefined2 *)(unaff_A6 + 0x21) = 0xfff8;
  }
  else {
    *(undefined2 *)(unaff_A6 + 0x21) = 8;
  }
  *unaff_A6 = 0x3f644;
  candidate_3faba();
  candidate_1abc8();
  candidate_1ad8c();
  unaff_A6[0xc] = *(short *)(unaff_A6 + 0x21) * 0x100 + unaff_A6[0xc];
  if (*(short *)((int)unaff_A6 + 0x6e) == 0) {
    if ((int)unaff_A6[0xc] <= *(int *)((int)unaff_A6 + 0x7a)) {
      *unaff_A6 = candidate_3f69a;
    }
  }
  else if (*(int *)((int)unaff_A6 + 0x7a) <= (int)unaff_A6[0xc]) {
    *unaff_A6 = candidate_3f69a;
  }
  candidate_427a();
  candidate_3fada();
  return;
}
