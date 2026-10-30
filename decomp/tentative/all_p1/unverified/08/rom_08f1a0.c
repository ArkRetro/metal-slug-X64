/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08f1a0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8f1a0(void)

{
  ushort uVar1;
  short sVar2;
  int iVar3;
  undefined4 *unaff_A6;
  bool bVar4;

  sVar2 = (*(byte *)((int)unaff_A6 + 0x8a) & 0xf) << 5;
  iVar3 = *(short *)(sVar2 + 0x29fc) * 0xc00;
  unaff_A6[0xc] = *(short *)(sVar2 + 0x2a7c) * 0xc00;
  unaff_A6[0xd] = iVar3;
  *(int *)((int)unaff_A6 + 0x76) = -(iVar3 >> 5);
  *(byte *)((int)unaff_A6 + 0x93) = *(byte *)((int)unaff_A6 + 0x93) | 0x20;
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  candidate_4256();
  *unaff_A6 = 0x8f114;
  func_0x000143e4();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_8f1e0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
    *unaff_A6 = candidate_8f1e0;
  }
  uVar1 = candidate_2eb4();
  *(char *)((int)unaff_A6 + 0x8a) = (char)(uVar1 >> 3);
  unaff_A6[0xd] = *(int *)((int)unaff_A6 + 0x76) + unaff_A6[0xd];
  candidate_427a();
  sVar2 = candidate_49e8();
  if (sVar2 != 0) {
    *unaff_A6 = candidate_8efde;
  }
  bVar4 = false;
  candidate_19504();
  if (bVar4) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
