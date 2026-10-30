/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06cb24.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

short candidate_6cb24(void)

{
  short sVar1;
  int extraout_A0;
  int unaff_A6;

  *(byte *)(unaff_A6 + 0x8d) = *(byte *)(unaff_A6 + 0x8d) & 0xfb;
  candidate_1b04a();
  if (-1 < (short)(*(short *)(extraout_A0 + 0x40) - (*(short *)(unaff_A6 + 0x40) + 0x14))) {
    *(byte *)(unaff_A6 + 0x8d) = *(byte *)(unaff_A6 + 0x8d) | 4;
  }
  sVar1 = candidate_260c8();
  return sVar1 + -1;
}
