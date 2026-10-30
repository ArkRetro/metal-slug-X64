/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d4814.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d4814(void)

{
  ushort uVar1;
  short sVar2;
  int iVar3;
  undefined4 *unaff_A6;
  bool bVar4;
  undefined1 uVar5;

  iVar3 = 0x2ca75e;
  if (*(char *)(unaff_A6 + 0x18) == '\x01') {
    iVar3 = 0x2ca7be;
  }
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfd;
  sVar2 = (*(byte *)((int)unaff_A6 + 0x6e) & 0xf) * 6;
  *(undefined2 *)((int)unaff_A6 + 0xa6) = *(undefined2 *)(iVar3 + 2 + (int)sVar2);
  unaff_A6[0xd] = (int)((uint)*(ushort *)(iVar3 + 4 + (int)sVar2) << 0x10) >> 8;
  iVar3 = candidate_d5090();
  unaff_A6[0xc] = (iVar3 << 0x10) >> 8;
  bVar4 = false;
  candidate_1b0f6();
  if (bVar4) {
    unaff_A6[0xc] = -unaff_A6[0xc];
  }
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 0x32;
  unaff_A6[0x14] = 0xe8ce6;
  candidate_4256();
  *unaff_A6 = 0xd489e;
  if ((int)unaff_A6[0xd] < 1) {
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
    *unaff_A6 = 0xd48bc;
  }
  unaff_A6[0xd] = ((int)((uint)*(ushort *)((int)unaff_A6 + 0xa6) << 0x10) >> 8) + unaff_A6[0xd];
  func_0x000143e4();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x80) != 0) {
    *unaff_A6 = candidate_d497e;
  }
  if (-1 < (int)unaff_A6[0xd]) {
    *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(unaff_A6[3] + 0x1e);
  }
  uVar1 = candidate_2eb4();
  *(ushort *)((int)unaff_A6 + 0xa2) = (uVar1 & 0xff) >> 3;
  candidate_427a();
  sVar2 = (short)*(int *)((int)unaff_A6 + 0x9a);
  if (*(int *)((int)unaff_A6 + 0x9a) != 0) {
    sVar2 = candidate_49e8();
  }
  if (sVar2 != 0) {
    *unaff_A6 = 0xd4ffe;
  }
  uVar5 = false;
  candidate_4c24();
  if ((bool)uVar5) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar5 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_d497e;
    uVar5 = 0;
  }
  candidate_409a();
  if (!(bool)uVar5) {
    *unaff_A6 = candidate_d497e;
    uVar5 = 0;
  }
  candidate_19504();
  if ((bool)uVar5) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
