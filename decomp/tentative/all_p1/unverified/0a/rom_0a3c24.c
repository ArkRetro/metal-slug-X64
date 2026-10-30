/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a3c24.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a3c24(void)

{
  ushort uVar1;
  short sVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  *(undefined1 *)((int)unaff_A6 + 0x8d) = 0xc;
  uVar1 = candidate_260c8();
  *(ushort *)((int)unaff_A6 + 0x62) = uVar1;
  *(undefined1 *)(unaff_A6 + 0x23) = *(undefined1 *)((short)(uVar1 >> 8) + 0xa4308);
  *(undefined2 *)((int)unaff_A6 + 0x86) = *(undefined2 *)((short)((uVar1 >> 8) * 2) + 0xa4318);
  *(undefined2 *)(unaff_A6 + 0x21) = 0x5a;
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 5;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  candidate_4c1c();
  candidate_4256();
  candidate_a4006();
  *unaff_A6 = 0xa3c9e;
  *(short *)((int)unaff_A6 + 0x86) = *(short *)((int)unaff_A6 + 0x86) + -1;
  if (*(short *)((int)unaff_A6 + 0x86) < 1) {
    *unaff_A6 = candidate_a3d26;
  }
  *(short *)(unaff_A6 + 0x21) = *(short *)(unaff_A6 + 0x21) + -1;
  if (*(short *)(unaff_A6 + 0x21) < 1) {
    *unaff_A6 = 0xa3cb2;
  }
  func_0x000143e4();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x80) != 0) {
    *unaff_A6 = candidate_a3d86;
  }
  candidate_427a();
  sVar2 = candidate_49e8();
  if (sVar2 != 0) {
    *unaff_A6 = candidate_a3d86;
  }
  uVar3 = 0;
  candidate_4c24();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_a3d86;
    uVar3 = 0;
  }
  candidate_19504();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
