/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d2ddc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d2ddc(void)

{
  uint uVar1;
  uint uVar2;
  ushort uVar3;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ed;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ed;
  uVar3 = candidate_29c6();
  unaff_A6[0xc] = ((int)((uint)(ushort)((uVar3 & 0x7f) - 0x40) << 0x10) >> 8) + unaff_A6[0xc];
  uVar3 = candidate_29c6();
  unaff_A6[0xd] = ((int)((uint)(uVar3 & 0x3f) << 0x10) >> 8) + unaff_A6[0xd];
  candidate_4256();
  *unaff_A6 = 0xd2e38;
  uVar2 = (int)((uint)*(ushort *)(unaff_A6 + 0x22) << 0x10) >> 8;
  uVar1 = unaff_A6[0xd];
  uVar4 = CARRY4(uVar2,uVar1);
  unaff_A6[0xd] = uVar2 + uVar1;
  func_0x000143e4();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_41ac;
    uVar4 = 0;
  }
  candidate_427a();
  candidate_19504();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
