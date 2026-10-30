/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09849c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9849c(void)

{
  ushort uVar1;
  undefined2 uVar2;
  short sVar3;
  int unaff_A6;
  bool bVar4;

  sVar3 = *(short *)(unaff_A6 + 0x7a);
  if (sVar3 == 0) {
    sVar3 = 1;
  }
  uVar2 = *(undefined2 *)(unaff_A6 + 0x7c);
  *(undefined2 *)(unaff_A6 + 0x82) = *(undefined2 *)(unaff_A6 + 0x6e);
  *(undefined2 *)(unaff_A6 + 0x84) = *(undefined2 *)(unaff_A6 + 0x70);
  *(undefined2 *)(unaff_A6 + 0x7c) = *(undefined2 *)(unaff_A6 + 0x72);
  *(undefined2 *)(unaff_A6 + 0x88) = *(undefined2 *)(unaff_A6 + 0x74);
  *(undefined2 *)(unaff_A6 + 0x86) = *(undefined2 *)(unaff_A6 + 0x76);
  *(short *)(unaff_A6 + 0x8a) = sVar3;
  *(undefined2 *)(unaff_A6 + 0x76) = uVar2;
  uVar1 = candidate_29c6();
  *(char *)(unaff_A6 + 0x92) = (char)uVar1;
  *(short *)(unaff_A6 + 0x82) = (uVar1 & 0xf) + *(short *)(unaff_A6 + 0x82);
  if (*(short *)(unaff_A6 + 0x84) != 0) {
    *(short *)(unaff_A6 + 0x84) = (uVar1 >> 4 & 0xf) + *(short *)(unaff_A6 + 0x84);
  }
  bVar4 = false;
  candidate_40bc();
  if (bVar4) {
    candidate_1b220();
  }
  uVar2 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x4e) = uVar2;
  uVar2 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x7a) = uVar2;
  uVar2 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x7e) = uVar2;
  candidate_9ea8();
  candidate_4520();
  *(undefined2 *)(unaff_A6 + 0x66) = 0x8838;
  *(undefined2 *)(unaff_A6 + 0x1e) = 0x8838;
  *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 1;
  *(undefined4 *)(unaff_A6 + 0x50) = 0xe609a;
  candidate_1dcce();
  *(ushort *)(unaff_A6 + 0x78) = *(ushort *)(unaff_A6 + 0x4e) >> 1;
  func_0x00098664();
  return;
}
