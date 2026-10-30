/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03a776.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3a776(void)

{
  undefined4 *unaff_A6;
  bool bVar1;

  candidate_b7350();
  bVar1 = false;
  candidate_40bc();
  if (bVar1) {
    candidate_1b220();
  }
  *(undefined2 *)((int)unaff_A6 + 0x6e) = *(undefined2 *)(unaff_A6 + 0xb);
  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0x40000;
  *unaff_A6 = 0x3a7ba;
  candidate_1ac38();
  candidate_427a();
  if (0x4f < (short)(*(short *)(unaff_A6 + 0xb) - *(short *)((int)unaff_A6 + 0x6e))) {
    *unaff_A6 = candidate_3a7de;
  }
  return;
}
