/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x087460.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_87460(void)

{
  undefined2 uVar1;
  short sVar2;
  int iVar3;
  undefined4 *unaff_A6;
  bool bVar4;

  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    iVar3 = 0x87d0e;
  }
  else {
    iVar3 = 0x87d16;
  }
  *(undefined1 *)((int)unaff_A6 + 0x8a) =
       *(undefined1 *)(iVar3 + (short)(*(byte *)((int)unaff_A6 + 0x89) & 7));
  uVar1 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar1;
  uVar1 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x76) = uVar1;
  unaff_A6[0x14] = 0x87caa;
  unaff_A6[0x15] = 0x87cb6;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  if (*(char *)((int)unaff_A6 + 0x85) == '\0') {
    candidate_9ea8();
  }
  else {
    candidate_9ea8();
  }
  candidate_4256();
  candidate_4c1c();
  uVar1 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x20) = uVar1;
  candidate_878a2();
  *unaff_A6 = 0x87520;
  func_0x000143e4();
  candidate_427a();
  *(short *)(unaff_A6 + 0x20) = *(short *)(unaff_A6 + 0x20) + -1;
  if (*(short *)(unaff_A6 + 0x20) < 1) {
    *unaff_A6 = candidate_87598;
  }
  bVar4 = false;
  candidate_1af9a();
  if (bVar4) {
    *unaff_A6 = candidate_87640;
  }
  sVar2 = candidate_49e8();
  if (sVar2 != 0) {
    *unaff_A6 = candidate_87674;
  }
  candidate_4c24();
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_87674;
  }
  bVar4 = false;
  candidate_19504();
  if (bVar4) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
