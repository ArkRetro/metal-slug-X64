/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0146e8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_146e8(void)

{
  undefined4 unaff_D7;
  undefined4 extraout_A0;
  int unaff_A5;
  int unaff_A6;
  undefined8 uVar1;

  candidate_128da();
  *(undefined1 *)(unaff_A5 + 0x4009) = 0;
  *(undefined4 *)(unaff_A5 + 0x4004) = 0;
  *(char *)(unaff_A5 + 0x4008) = -((*(byte *)(unaff_A6 + 0x15) & 0x20) == 0);
  uVar1 = candidate_13db2();
  *(short *)(unaff_A6 + 0x6c) = (short)((uint)unaff_D7 >> 8);
  *(undefined8 *)(unaff_A6 + 0x28) = uVar1;
  *(undefined4 *)(unaff_A6 + 0x30) = 0;
  *(undefined4 *)(unaff_A6 + 0x34) = 0;
  *(undefined4 *)(unaff_A6 + 0x38) = extraout_A0;
  candidate_1abc8();
  *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 0x40;
  return;
}
