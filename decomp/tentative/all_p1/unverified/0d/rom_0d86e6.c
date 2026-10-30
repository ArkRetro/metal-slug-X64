/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d86e6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d86e6(void)

{
  uint uVar1;
  uint uVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  *(short *)(unaff_A6 + 0xf) = *(short *)(unaff_A6 + 0xf) + -0x20;
  *(short *)(unaff_A6 + 0x10) = *(short *)(unaff_A6 + 0x10) + -8;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
  candidate_40f04();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
  candidate_40f04();
  *(short *)(unaff_A6 + 0xf) = *(short *)(unaff_A6 + 0xf) + 0x20;
  *(short *)(unaff_A6 + 0x10) = *(short *)(unaff_A6 + 0x10) + 8;
  candidate_1b1dc();
  unaff_A6[0xd] = 0x21c00;
  *(undefined2 *)(unaff_A6 + 0x1c) = 0xffdc;
  candidate_4256();
  *unaff_A6 = 0xd8740;
  uVar2 = (int)((uint)*(ushort *)(unaff_A6 + 0x1c) << 0x10) >> 8;
  uVar1 = unaff_A6[0xd];
  uVar3 = CARRY4(uVar2,uVar1);
  unaff_A6[0xd] = uVar2 + uVar1;
  func_0x000143e4();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    unaff_A6[0x14] = 0xe987e;
    candidate_4256();
    *unaff_A6 = 0xd877c;
    uVar3 = 0;
    candidate_1abc8();
  }
  candidate_427a();
  candidate_4c24();
  if ((bool)uVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    *(short *)(unaff_A6 + 10) = *(short *)(unaff_A6 + 10) + -0x20;
    *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) + -8;
    candidate_40ed0();
    *(short *)(unaff_A6 + 10) = *(short *)(unaff_A6 + 10) + 0x20;
    *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) + 8;
    candidate_4256();
  }
  return;
}
