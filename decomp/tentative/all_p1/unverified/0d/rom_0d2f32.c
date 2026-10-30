/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d2f32.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d2f32(void)

{
  short sVar1;
  int iVar2;
  uint uVar3;
  undefined4 *unaff_A6;
  bool bVar4;

  sVar1 = *(short *)((short)(*(short *)(unaff_A6 + 0x23) * 2) + 0x29fc);
  iVar2 = *(short *)((short)(*(short *)(unaff_A6 + 0x23) * 2) + 0x2a7c) * 0x2000;
  uVar3 = iVar2 >> 8;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
    uVar3 = (uint)(ushort)-(short)((uint)iVar2 >> 8);
  }
  unaff_A6[0xc] = (int)(uVar3 << 0x10) >> 8;
  unaff_A6[0xd] = ((sVar1 * 0x2000 >> 8) << 0x10) >> 8;
  *(undefined2 *)(unaff_A6 + 0x23) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x92) = *(undefined2 *)(unaff_A6 + 10);
  candidate_9ea8();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  candidate_4256();
  *unaff_A6 = 0xd2fb8;
  func_0x000143e4();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x80) != 0) {
    *unaff_A6 = candidate_d301a;
  }
  candidate_427a();
  if (0x4d7 < *(short *)(unaff_A6 + 0xb)) {
    *unaff_A6 = candidate_d2e7a;
  }
  sVar1 = (short)*(int *)((int)unaff_A6 + 0x82);
  if (*(int *)((int)unaff_A6 + 0x82) != 0) {
    sVar1 = candidate_49e8();
  }
  if (sVar1 != 0) {
    *unaff_A6 = candidate_d301a;
  }
  bVar4 = false;
  candidate_19504();
  if (bVar4) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
