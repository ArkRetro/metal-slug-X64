/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08d940.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

short candidate_8d940(void)

{
  short sVar1;
  int extraout_A0;
  int unaff_A6;
  undefined1 in_CF;

  sVar1 = candidate_1af9a();
  if ((bool)in_CF) {
    return sVar1;
  }
  sVar1 = *(short *)(extraout_A0 + 0x3c) - *(short *)(unaff_A6 + 0x3c);
  if ((ushort)(*(short *)(extraout_A0 + 0x40) - *(short *)(unaff_A6 + 0x40)) < 0x40) {
    sVar1 = sVar1 + 0x20;
  }
  return sVar1;
}
