/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05e7d8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5e7d8(void)

{
  byte bVar1;
  char unaff_D6b;
  undefined4 *extraout_A0;
  int unaff_A6;

  bVar1 = (*(byte *)(unaff_A6 + 0x88) >> 1) + 8;
  *(byte *)(unaff_A6 + 0x89) = bVar1;
  *(uint *)(unaff_A6 + 0x72) = (ushort)((bVar1 & 0xf) << 2) + 0x5eb00;
  candidate_40bc();
  candidate_1b242();
  candidate_1acf2();
  candidate_40bc();
  *(short *)(unaff_A6 + 0x2c) = *(short *)(unaff_A6 + 0x2c) + 0xf;
  *(short *)(unaff_A6 + 0x40) = *(short *)(unaff_A6 + 0x40) + 0xf;
  candidate_14a02();
  *(short *)(unaff_A6 + 0x2c) = *(short *)(unaff_A6 + 0x2c) + -0xf;
  *(short *)(unaff_A6 + 0x40) = *(short *)(unaff_A6 + 0x40) + -0xf;
  if (unaff_D6b != '\0') {
    *extraout_A0 = candidate_5dd12;
  }
  *(undefined1 *)((int)extraout_A0 + 0x89) = *(undefined1 *)(unaff_A6 + 0x89);
  candidate_1b242();
  candidate_1acf2();
  return;
}
