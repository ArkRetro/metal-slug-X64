/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0cda32.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_cda32(void)

{
  short sVar1;
  short sVar2;
  undefined4 *unaff_A6;

  candidate_1ad6e();
  sVar2 = *(short *)((int)unaff_A6 + 0x6e) << 4;
  sVar1 = *(short *)(sVar2 + 0x29fc);
  unaff_A6[0xc] = (int)((uint)(ushort)(*(short *)(sVar2 + 0x2a7c) * -4) << 0x10) >> 8;
  unaff_A6[0xd] = (int)((uint)(ushort)(sVar1 * -4) << 0x10) >> 8;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  candidate_9ea8();
  *(undefined2 *)(unaff_A6 + 0x26) = 8;
  candidate_4256();
  *unaff_A6 = 0xcdab8;
  candidate_1ac38();
  candidate_427a();
  *(short *)(unaff_A6 + 0x26) = *(short *)(unaff_A6 + 0x26) + -1;
  if (*(short *)(unaff_A6 + 0x26) < 1) {
    *unaff_A6 = candidate_cdada;
  }
  return;
}
