/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09f156.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9f156(void)

{
  short sVar1;
  int extraout_A0;
  int unaff_A6;
  undefined1 in_CF;

  if ((*(byte *)(unaff_A6 + 0x98) & 8) != 0) {
    candidate_9f07a();
    candidate_1b08a();
    if ((!(bool)in_CF) &&
       (0xf < (short)(*(short *)(unaff_A6 + 0x40) - *(short *)(extraout_A0 + 0x40)))) {
      sVar1 = candidate_9f094();
      if (sVar1 < 0x31) {
        return;
      }
    }
  }
  return;
}
