/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d2cb6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d2cb6(void)

{
  uint uVar1;
  uint uVar2;
  short sVar3;
  undefined4 *unaff_A6;
  bool bVar4;
  undefined1 uVar5;

  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  *(undefined2 *)(unaff_A6 + 0x22) = 0xffc0;
  candidate_4256();
  *unaff_A6 = 0xd2cd8;
  uVar2 = (int)((uint)*(ushort *)(unaff_A6 + 0x22) << 0x10) >> 8;
  uVar1 = unaff_A6[0xd];
  uVar5 = CARRY4(uVar2,uVar1);
  unaff_A6[0xd] = uVar2 + uVar1;
  func_0x000143e4();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_d2d14;
    uVar5 = 0;
  }
  candidate_427a();
  candidate_4c24();
  if ((bool)uVar5) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  sVar3 = (short)*(int *)((int)unaff_A6 + 0x82);
  if (*(int *)((int)unaff_A6 + 0x82) != 0) {
    sVar3 = candidate_49e8();
  }
  if (sVar3 != 0) {
    *unaff_A6 = candidate_d289a;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_d289a;
  }
  bVar4 = false;
  candidate_19504();
  if (bVar4) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
