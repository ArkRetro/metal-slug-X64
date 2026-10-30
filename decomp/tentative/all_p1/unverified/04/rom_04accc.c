/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04accc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4accc(void)

{
  undefined4 *unaff_A6;

  *(undefined2 *)(unaff_A6 + 0x21) = 0x10;
  unaff_A6[0xc] = 0;
  *unaff_A6 = 0x4acdc;
  *(int *)((int)unaff_A6 + 0x7a) = -*(int *)((int)unaff_A6 + 0x7a);
  unaff_A6[0xd] = *(undefined4 *)((int)unaff_A6 + 0x7a);
  candidate_1ac38();
  candidate_427a();
  if (*(short *)(unaff_A6 + 0x21) == 0) {
    *unaff_A6 = 0x4ab22;
  }
  else {
    *(short *)(unaff_A6 + 0x21) = *(short *)(unaff_A6 + 0x21) + -1;
  }
  return;
}
