/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d8ea4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d8ea4(void)

{
  uint uVar1;
  uint uVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_9ea8();
  candidate_4256();
  *unaff_A6 = 0xd8ec8;
  uVar2 = (int)((uint)*(ushort *)(unaff_A6 + 0x1c) << 0x10) >> 8;
  uVar1 = unaff_A6[0xd];
  uVar3 = CARRY4(uVar2,uVar1);
  unaff_A6[0xd] = uVar2 + uVar1;
  func_0x000143e4();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_d888c;
    uVar3 = 0;
  }
  candidate_427a();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_d888c;
    uVar3 = 0;
  }
  candidate_19504();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
