/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03f4ee.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3f4ee(void)

{
  undefined4 *unaff_A6;
  undefined1 in_CF;

  candidate_1abc8();
  candidate_1ad8c();
  candidate_427a();
  if ((bool)in_CF) {
    if (*(short *)((int)unaff_A6 + 0x6e) == 0) {
      *(short *)(unaff_A6 + 10) = *(short *)(unaff_A6 + 10) + -0x48;
    }
    else {
      *(short *)(unaff_A6 + 10) = *(short *)(unaff_A6 + 10) + 0x48;
    }
    *(undefined2 *)(unaff_A6 + 0x1d) = *(undefined2 *)((int)unaff_A6 + 0x72);
    *unaff_A6 = candidate_3f530;
  }
  candidate_3fada();
  return;
}
