/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d1f2e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d1f2e(void)

{
  int extraout_A0;
  undefined4 *unaff_A6;
  bool bVar1;

  *(undefined1 *)(unaff_A6 + 0x1c) = 0;
  candidate_18f92();
  *(undefined1 *)(unaff_A6 + 0x18) = 1;
  *(undefined2 *)((int)unaff_A6 + 0x6e) = 0xb4;
  unaff_A6[0x14] = 0;
  candidate_40bc();
  *(undefined1 *)(extraout_A0 + 0x73) = 1;
  bVar1 = false;
  candidate_40bc();
  if (bVar1) {
    candidate_1b220();
  }
  candidate_4256();
  *unaff_A6 = 0xd1fa6;
  candidate_1abc8();
  candidate_d31ee();
  candidate_427a();
  *(short *)((int)unaff_A6 + 0x6e) = *(short *)((int)unaff_A6 + 0x6e) + -1;
  if (*(short *)((int)unaff_A6 + 0x6e) == 0x3c) {
    candidate_1da40();
  }
  if (*(short *)((int)unaff_A6 + 0x6e) == 0) {
    candidate_18f40();
  }
  return;
}
