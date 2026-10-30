/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a2134.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a2134(void)

{
  ushort uVar1;
  undefined2 uVar2;
  undefined4 *unaff_A6;

  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
  candidate_1abc8();
  uVar1 = candidate_29c6();
  uVar2 = 0x100;
  if (*(short *)(unaff_A6 + 0xf) < *(short *)(unaff_A6[3] + 0x3c)) {
    uVar2 = 0xff00;
  }
  *(undefined2 *)((int)unaff_A6 + 0x6e) = 0xffff;
  *(undefined2 *)(unaff_A6 + 0x1c) = uVar2;
  *(undefined2 *)((int)unaff_A6 + 0x72) = 0xffff;
  *(undefined2 *)(unaff_A6 + 0x1d) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x76) = 1;
  *(ushort *)(unaff_A6 + 0x1e) = (uVar1 & 0x1f) - 4;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8800;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8800;
  candidate_9ea8();
  *unaff_A6 = candidate_99090;
  return;
}
