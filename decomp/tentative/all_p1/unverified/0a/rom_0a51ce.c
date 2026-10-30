/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a51ce.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a51ce(void)

{
  uint uVar1;
  undefined2 uVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_120f8();
  candidate_4256();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfb;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8800;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8800;
  *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) + -0x20;
  candidate_1ad52();
  candidate_1b1dc();
  *(undefined4 *)((int)unaff_A6 + 0x82) = 0xffffe000;
  unaff_A6[0x14] = 0x2a600a;
  uVar2 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar2;
  *unaff_A6 = 0xa5232;
  uVar1 = unaff_A6[0xd];
  uVar3 = CARRY4(*(uint *)((int)unaff_A6 + 0x82),uVar1);
  unaff_A6[0xd] = *(uint *)((int)unaff_A6 + 0x82) + uVar1;
  func_0x000143e4();
  candidate_427a();
  candidate_4c24();
  if ((bool)uVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_a52a4;
  }
  candidate_49e8();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = thunk_FUN_000a52d6;
  }
  if ((*(byte *)(*(int *)((int)unaff_A6 + 0x6e) + 0x5f) & 0x80) != 0) {
    *unaff_A6 = FUN_000a52d6;
  }
  return;
}
