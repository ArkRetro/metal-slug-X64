/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09f250.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9f250(void)

{
  short sVar1;
  int unaff_A6;
  undefined1 in_CF;

  if ((*(byte *)(unaff_A6 + 0x96) & 0x80) == 0) {
    candidate_9f07a();
    candidate_1b08a();
    if (!(bool)in_CF) {
      sVar1 = candidate_9f094();
      if (*(short *)(unaff_A6 + 0x82) <= sVar1) {
        return;
      }
    }
  }
  return;
}
