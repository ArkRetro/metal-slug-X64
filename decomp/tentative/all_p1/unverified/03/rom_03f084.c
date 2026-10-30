/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03f084.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3f084(void)

{
  undefined4 *unaff_A6;

  candidate_1abc8();
  candidate_1ad8c();
  candidate_3fa46();
  candidate_427a();
  *(short *)(unaff_A6 + 0x1d) = *(short *)(unaff_A6 + 0x1d) + -1;
  if (*(short *)(unaff_A6 + 0x1d) == 0) {
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8838;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8838;
    unaff_A6[0xc] = (int)*(short *)(unaff_A6 + 0x20) << 8;
    unaff_A6[0xd] = 0;
    *unaff_A6 = candidate_3ed8e;
  }
  candidate_3fada();
  return;
}
