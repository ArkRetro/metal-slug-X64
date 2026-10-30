/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02a27c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2a27c(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  candidate_2a47a();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  candidate_2a5ea();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0f0;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0f0;
  candidate_4256();
  *unaff_A6 = 0x2a2ac;
  unaff_A6[0xd] = *(int *)((int)unaff_A6 + 0x7a) + unaff_A6[0xd];
  func_0x000143e4();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x80) != 0) {
    *unaff_A6 = candidate_2a2fe;
  }
  candidate_427a();
  sVar1 = candidate_49e8();
  if (sVar1 != 0) {
    *unaff_A6 = candidate_2a244;
  }
  bVar2 = false;
  candidate_19504();
  if (bVar2) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
