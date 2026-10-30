/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d4e2a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d4e2a(void)

{
  int iVar1;
  undefined2 uVar2;
  short sVar3;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  candidate_260c8();
  iVar1 = candidate_d5090();
  unaff_A6[0xc] = (iVar1 << 0x10) >> 8;
  candidate_260c8();
  uVar2 = candidate_d5090();
  *(undefined2 *)(unaff_A6 + 0x29) = uVar2;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  candidate_4256();
  *unaff_A6 = 0xd4e74;
  unaff_A6[0xc] = ((int)((uint)*(ushort *)(unaff_A6 + 0x29) << 0x10) >> 8) + unaff_A6[0xc];
  unaff_A6[0xd] = ((int)((uint)*(ushort *)((int)unaff_A6 + 0xa6) << 0x10) >> 8) + unaff_A6[0xd];
  func_0x000143e4();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x80) != 0) {
    *unaff_A6 = 0xd4fde;
  }
  candidate_427a();
  sVar3 = (short)*(int *)((int)unaff_A6 + 0x9a);
  if (*(int *)((int)unaff_A6 + 0x9a) != 0) {
    sVar3 = candidate_49e8();
  }
  if (sVar3 != 0) {
    *unaff_A6 = 0xd4fde;
  }
  uVar4 = false;
  candidate_409a();
  if (!(bool)uVar4) {
    *unaff_A6 = 0xd4fde;
    uVar4 = 0;
  }
  candidate_19504();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
