/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d8bd8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d8bd8(void)

{
  uint uVar1;
  uint uVar2;
  undefined2 uVar3;
  short sVar4;
  undefined4 *unaff_A6;
  bool bVar5;
  undefined1 uVar6;

  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  candidate_9ea8();
  candidate_4520();
  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0xfffe0000;
  *(undefined2 *)(unaff_A6 + 0x1c) = 0xffc0;
  uVar3 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar3;
  candidate_4256();
  *unaff_A6 = 0xd8c2e;
  uVar2 = (int)((uint)*(ushort *)(unaff_A6 + 0x1c) << 0x10) >> 8;
  uVar1 = unaff_A6[0xd];
  bVar5 = CARRY4(uVar2,uVar1);
  unaff_A6[0xd] = uVar2 + uVar1;
  func_0x000143e4();
  if (bVar5) {
    *unaff_A6 = candidate_d888c;
  }
  candidate_427a();
  sVar4 = candidate_49e8();
  if (sVar4 != 0) {
    *unaff_A6 = candidate_d888c;
  }
  uVar6 = false;
  candidate_4c24();
  if ((bool)uVar6) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar6 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_d888c;
    uVar6 = 0;
  }
  candidate_19504();
  if ((bool)uVar6) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
