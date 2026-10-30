/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d6f90.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d6f90(void)

{
  short sVar1;
  int iVar2;
  undefined4 *unaff_A6;
  bool bVar3;

  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  iVar2 = 0x2caf92;
  if ('\x03' < *(char *)(*(int *)(unaff_A6[3] + 0xc) + 0x79)) {
    iVar2 = 0x2cafa2;
  }
  iVar2 = *(int *)(iVar2 + (short)((ushort)*(byte *)(unaff_A6[3] + 0xa3) << 2));
  sVar1 = (*(byte *)((int)unaff_A6 + 0x7d) & 7) << 2;
  unaff_A6[0xc] = (int)((uint)*(ushort *)(iVar2 + sVar1) << 0x10) >> 8;
  unaff_A6[0xd] = 0x10000;
  *(int *)((int)unaff_A6 + 0x7a) = (int)((uint)*(ushort *)(iVar2 + 2 + (int)sVar1) << 0x10) >> 8;
  *(undefined1 *)((int)unaff_A6 + 0x86) = 0;
  *(byte *)((int)unaff_A6 + 0x86) = *(byte *)((int)unaff_A6 + 0x86) | 0x40;
  *(byte *)((int)unaff_A6 + 0x86) = *(byte *)((int)unaff_A6 + 0x86) | 0x10;
  candidate_2a50a();
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
  bVar3 = false;
  candidate_19504();
  if (bVar3) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
