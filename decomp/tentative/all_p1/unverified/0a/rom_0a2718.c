/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a2718.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a2718(void)

{
  short sVar1;
  short sVar2;
  int unaff_A6;
  bool bVar3;

  sVar2 = (*(byte *)(unaff_A6 + 0x85) & 7) << 2;
  sVar1 = *(short *)(sVar2 + 0xa2daa);
  sVar2 = *(short *)(sVar2 + 0xa2dac);
  *(short *)(unaff_A6 + 0x28) = sVar1 + *(short *)(unaff_A6 + 0x28);
  *(short *)(unaff_A6 + 0x2c) = sVar2 + *(short *)(unaff_A6 + 0x2c);
  *(undefined2 *)(unaff_A6 + 0x66) = 0xe0ed;
  *(undefined2 *)(unaff_A6 + 0x1e) = 0xe0ed;
  candidate_1abc8();
  candidate_1dbaa();
  bVar3 = false;
  candidate_40bc();
  if (bVar3) {
    candidate_1b220();
  }
  *(short *)(unaff_A6 + 0x28) = *(short *)(unaff_A6 + 0x28) - sVar1;
  *(short *)(unaff_A6 + 0x2c) = *(short *)(unaff_A6 + 0x2c) - sVar2;
  *(undefined2 *)(unaff_A6 + 0x66) = 0x8820;
  *(undefined2 *)(unaff_A6 + 0x1e) = 0x8820;
  candidate_1abc8();
  return;
}
