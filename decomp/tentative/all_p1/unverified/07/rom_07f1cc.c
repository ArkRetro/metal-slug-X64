/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07f1cc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7f1cc(void)

{
  undefined4 *unaff_A6;

  candidate_19dfc();
  unaff_A6[0xd] = 0;
  candidate_40bc();
  candidate_1acf2();
  candidate_1ac94();
  candidate_4256();
  *unaff_A6 = 0x7f20e;
  unaff_A6[0xd] = (-0x80000 - unaff_A6[0xd] >> 5) + unaff_A6[0xd];
  unaff_A6[0xc] = unaff_A6[0xc] - ((int)unaff_A6[0xc] >> 5);
  func_0x000143e4();
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_7f30c;
    return;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
    *unaff_A6 = candidate_7f26c;
  }
  candidate_7f7a0();
  return;
}
