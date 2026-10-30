/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d7198.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d7198(void)

{
  byte bVar1;
  short sVar2;
  int extraout_A0;
  int unaff_A6;

  candidate_d70f2();
  bVar1 = *(byte *)(extraout_A0 + 0x7e);
  sVar2 = 2;
  if ((*(byte *)(unaff_A6 + 0x7d) & 1) != 0) {
    bVar1 = *(byte *)(extraout_A0 + 0x7f);
    sVar2 = -10;
  }
  *(short *)(unaff_A6 + 0x2c) = (ushort)bVar1 + *(short *)(unaff_A6 + 0x2c);
  *(short *)(unaff_A6 + 0x28) = sVar2 + *(short *)(unaff_A6 + 0x28);
  candidate_1abc8();
  return;
}
