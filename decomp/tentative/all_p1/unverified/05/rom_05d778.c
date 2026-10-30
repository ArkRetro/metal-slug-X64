/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05d778.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5d778(void)

{
  short sVar1;
  ushort uVar3;
  short sVar4;
  int iVar2;
  undefined4 *unaff_A6;
  bool bVar5;

  uVar3 = candidate_260c8();
  sVar4 = (uVar3 & 3) * 0xc;
  unaff_A6[0xd] = *(undefined4 *)(sVar4 + 0x2cfeaa);
  *(undefined4 *)((int)unaff_A6 + 0x76) = *(undefined4 *)(sVar4 + 0x2cfeae);
  iVar2 = *(int *)(sVar4 + 0x2cfeb2);
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    iVar2 = -iVar2;
  }
  unaff_A6[0xc] = iVar2;
  candidate_4256();
  *unaff_A6 = 0x5d7ce;
  func_0x000143e4();
  unaff_A6[0xd] = *(int *)((int)unaff_A6 + 0x76) + unaff_A6[0xd];
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_5d7f6;
  }
  candidate_5e6d2();
  sVar4 = *(short *)(unaff_A6 + 0x1f);
  sVar1 = sVar4 + -1;
  *(short *)(unaff_A6 + 0x1f) = sVar1;
  if (-1 >= sVar1) {
    *(undefined2 *)(unaff_A6 + 0x1f) = 0;
  }
  bVar5 = (*(byte *)((int)unaff_A6 + 0x5f) & 1) == 0;
  if (!bVar5) {
    *(undefined1 *)((int)unaff_A6 + 0x91) = 0x1e;
  }
  bVar5 = bVar5 && (-1 < sVar1 && sVar4 == 0);
  candidate_5e7a8();
  if (bVar5) {
    *unaff_A6 = candidate_5d732;
  }
  candidate_4c24();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  else {
    *unaff_A6 = candidate_5db18;
  }
  bVar5 = false;
  candidate_1945c();
  if (bVar5) {
    *unaff_A6 = candidate_5dc7a;
  }
  return;
}
