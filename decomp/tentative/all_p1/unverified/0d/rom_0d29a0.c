/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d29a0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d29a0(void)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  short sVar4;
  ushort uVar5;
  undefined4 *unaff_A6;
  undefined1 uVar6;

  *(short *)(unaff_A6 + 10) =
       *(short *)((short)(*(short *)((int)unaff_A6 + 0x92) * 2 +
                         *(short *)((int)unaff_A6 + 0x8e) * 8) + 0xe8794) +
       *(short *)(unaff_A6 + 10);
  *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) + -0x18;
  uVar5 = candidate_29c6();
  sVar4 = (uVar5 & 0xf) * 6;
  iVar2 = *(int *)((short)(*(short *)((int)unaff_A6 + 0x8e) << 2) + 0xe87cc);
  unaff_A6[0xc] = (int)((uint)*(ushort *)(iVar2 + sVar4) << 0x10) >> 8;
  *(undefined2 *)(unaff_A6 + 0x22) = *(undefined2 *)(iVar2 + 2 + (int)sVar4);
  unaff_A6[0xd] = (int)((uint)*(ushort *)(iVar2 + 4 + (int)sVar4) << 0x10) >> 8;
  unaff_A6[0xd] = -unaff_A6[0xd];
  *(short *)(unaff_A6 + 0x22) = -*(short *)(unaff_A6 + 0x22);
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ed;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ed;
  candidate_4256();
  *unaff_A6 = 0xd2a3e;
  uVar3 = (int)((uint)*(ushort *)(unaff_A6 + 0x22) << 0x10) >> 8;
  uVar1 = unaff_A6[0xd];
  uVar6 = CARRY4(uVar3,uVar1);
  unaff_A6[0xd] = uVar3 + uVar1;
  func_0x000143e4();
  candidate_427a();
  if ((bool)uVar6) {
    *unaff_A6 = candidate_41ac;
    uVar6 = 0;
  }
  candidate_19504();
  if ((bool)uVar6) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
