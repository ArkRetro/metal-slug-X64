/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05785a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5785a(void)

{
  short extraout_var;
  undefined1 extraout_var_00;
  undefined4 *unaff_A6;

  candidate_4256();
  candidate_9ea8();
  unaff_A6[10] = 0;
  unaff_A6[0xb] = 0x1800000;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0;
  *unaff_A6 = 0x578a8;
  candidate_1abc8();
  if (-1 < (short)(*(short *)((int)unaff_A6 + 0x76) + *(short *)(unaff_A6[3] + 0x76))) {
    candidate_2e7c();
    *(undefined1 *)((int)unaff_A6 + 0x12) = extraout_var_00;
    candidate_2e7c();
    *(short *)(unaff_A6 + 10) = extraout_var + 0xa0;
    candidate_427a();
  }
  return;
}
