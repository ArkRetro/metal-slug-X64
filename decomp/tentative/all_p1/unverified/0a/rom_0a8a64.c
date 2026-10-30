/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a8a64.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a8a64(void)

{
  short sVar1;
  short sVar2;
  ushort uVar3;
  undefined4 *unaff_A6;

  if ((byte)*(undefined2 *)((int)unaff_A6 + 0x6e) < *(byte *)(unaff_A6[3] + 0x6e)) {
    candidate_9ea8();
    candidate_9ea8();
    candidate_4256();
  }
  else {
    candidate_9ea8();
    candidate_9ea8();
    candidate_4256();
  }
  *unaff_A6 = 0xa8aea;
  uVar3 = *(short *)((int)unaff_A6 + 0x6e) * 0x100 - *(short *)(unaff_A6[3] + 0x74);
  sVar2 = 0x600;
  if (0x5ff < uVar3) {
    if (-1 < (short)(uVar3 - 0x600)) {
      sVar2 = -0x600;
    }
    uVar3 = sVar2 + uVar3;
  }
  sVar1 = (short)((uint)uVar3 * 0x2aaa >> 0x10) * 2;
  sVar2 = *(short *)(sVar1 + 0x2a7c);
  sVar1 = *(short *)(sVar1 + 0x29fc);
  *(short *)((int)unaff_A6 + 0x1e) = sVar2 + -0x8000;
  uVar3 = (ushort)(sVar2 + 0x700U) >> 3;
  if (0x7f < uVar3) {
    uVar3 = uVar3 - 1;
  }
  *(char *)((int)unaff_A6 + 0x12) = (char)uVar3;
  *(char *)((int)unaff_A6 + 0x13) = (char)uVar3;
  func_0x00009cce();
  func_0x00009cce();
  *(short *)(unaff_A6 + 0x10) = 0x36c - (short)(0xd2000 / (ushort)(0x700U - sVar2));
  *(short *)(unaff_A6 + 0xf) = (short)((sVar1 * 0x460) / (int)(short)(0x700U - sVar2)) + 0xa0;
  candidate_427a();
  return;
}
