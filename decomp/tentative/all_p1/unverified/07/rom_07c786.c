/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07c786.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7c786(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8800;
  candidate_1b1dc();
  *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) + -0x20;
  *(undefined4 *)((int)unaff_A6 + 0x72) = 0;
  *(undefined4 *)((int)unaff_A6 + 0x76) = 0;
  candidate_4256();
  *unaff_A6 = 0x7c7be;
  func_0x0007cce0();
  unaff_A6[0xd] = *(int *)((int)unaff_A6 + 0x76) + unaff_A6[0xd];
  func_0x000143e4();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_7c816;
  }
  candidate_427a();
  sVar1 = candidate_49e8();
  if (sVar1 != 0) {
    *unaff_A6 = candidate_7c85a;
  }
  bVar2 = false;
  candidate_19504();
  if (bVar2) {
    *unaff_A6 = 0x7c946;
  }
  return;
}
