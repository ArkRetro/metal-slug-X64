/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d4f1c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d4f1c(void)

{
  uint uVar1;
  uint uVar2;
  undefined4 uVar3;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  uVar3 = 0x11100;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
    uVar3 = 0xfffeef00;
  }
  unaff_A6[0xc] = uVar3;
  unaff_A6[0xd] = 0x87f00;
  *(undefined2 *)((int)unaff_A6 + 0xa6) = 0xff6f;
  *(undefined2 *)(unaff_A6 + 0x29) = 0;
  candidate_9ea8();
  candidate_4256();
  *unaff_A6 = 0xd4f68;
  uVar2 = (int)((uint)*(ushort *)((int)unaff_A6 + 0xa6) << 0x10) >> 8;
  uVar1 = unaff_A6[0xd];
  uVar4 = CARRY4(uVar2,uVar1);
  unaff_A6[0xd] = uVar2 + uVar1;
  func_0x000143e4();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x80) != 0) {
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
