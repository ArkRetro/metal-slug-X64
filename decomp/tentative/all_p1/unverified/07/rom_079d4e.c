/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x079d4e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_79d4e(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  candidate_9f88();
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  candidate_4256();
  candidate_1b1dc();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  sVar1 = candidate_260c8();
  unaff_A6[0xc] = sVar1 * -0x100;
  *unaff_A6 = 0x79da6;
  func_0x000143e4();
  candidate_427a();
  sVar1 = candidate_49e8();
  if (sVar1 != 0) {
    *unaff_A6 = candidate_79dec;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_79dec;
  }
  bVar2 = false;
  candidate_7a032();
  if (bVar2) {
    *unaff_A6 = candidate_4188;
  }
  return;
}
