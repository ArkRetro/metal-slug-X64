/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x079b14.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_79b14(void)

{
  short sVar1;
  undefined4 *unaff_A6;

  candidate_13f1e();
  candidate_7a13e();
  candidate_120f8();
  candidate_1da40();
  candidate_1da40();
  *(undefined2 *)((int)unaff_A6 + 0x82) = 10;
  *unaff_A6 = 0x79b4c;
  candidate_1abc8();
  candidate_427a();
  sVar1 = *(short *)((int)unaff_A6 + 0x82) + -1;
  *(short *)((int)unaff_A6 + 0x82) = sVar1;
  if (sVar1 == 0) {
    *(undefined1 *)(unaff_A6 + 0x18) = 0xff;
    *unaff_A6 = 0x79b92;
  }
  return;
}
