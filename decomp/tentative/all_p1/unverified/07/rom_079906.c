/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x079906.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_79906(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  candidate_9f88();
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  candidate_120f8();
  candidate_1b1dc();
  sVar1 = candidate_260c8();
  unaff_A6[0xc] = sVar1 * -0x100;
  candidate_4256();
  *unaff_A6 = 0x79968;
  func_0x000143e4();
  candidate_427a();
  sVar1 = candidate_49e8();
  if (sVar1 != 0) {
    *unaff_A6 = candidate_79dec;
  }
  bVar2 = false;
  candidate_19504();
  if (bVar2) {
    *unaff_A6 = candidate_4188;
  }
  return;
}
