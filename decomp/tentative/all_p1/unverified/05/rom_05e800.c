/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05e800.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5e800(void)

{
  char unaff_D6b;
  int iVar1;
  undefined4 *extraout_A0;
  int unaff_A6;

  if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
    iVar1 = 0x5ea80;
  }
  else {
    iVar1 = 0x5eac0;
  }
  *(uint *)(unaff_A6 + 0x72) = iVar1 + (uint)(ushort)((*(byte *)(unaff_A6 + 0x89) & 0xf) << 2);
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
