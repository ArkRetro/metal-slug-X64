/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07ef24.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

short candidate_7ef24(void)

{
  short sVar1;
  int extraout_A0;
  int unaff_A6;

  candidate_1af9a();
  sVar1 = *(short *)(unaff_A6 + 0x3c) - *(short *)(extraout_A0 + 0x3c);
  if (sVar1 < 0) {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 1;
  }
  else {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) & 0xfe;
  }
  return sVar1;
}
