/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0286f8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_286f8(void)

{
  int iVar1;
  uint uVar2;
  short sVar3;
  undefined4 *unaff_A6;
  bool bVar4;

  iVar1 = candidate_260c8();
  uVar2 = (uint)(iVar1 << 0x10) >> 8;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    uVar2 = -uVar2;
  }
  unaff_A6[0xc] = uVar2;
  unaff_A6[0xd] = 0;
  if ((*(byte *)((int)unaff_A6 + 0x7d) & 8) != 0) {
    *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) + 8;
  }
  candidate_4256();
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  *unaff_A6 = 0x28762;
  func_0x000143e4();
  candidate_427a();
  sVar3 = candidate_49e8();
  if (sVar3 != 0) {
    *unaff_A6 = candidate_28b8c;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x80) != 0) {
    *unaff_A6 = candidate_28b8c;
  }
  bVar4 = false;
  candidate_19504();
  if (bVar4) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
