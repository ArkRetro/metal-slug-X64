/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09f4ac.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9f4ac(void)

{
  short sVar1;
  short sVar2;
  int unaff_A6;
  undefined1 in_CF;

  if (((*(byte *)(unaff_A6 + 0x98) & 0x40) != 0) && ((*(byte *)(unaff_A6 + 0x98) & 0x80) != 0)) {
    candidate_9f07a();
    candidate_1b08a();
    if (!(bool)in_CF) {
      sVar1 = candidate_9f094();
      sVar2 = candidate_260c8();
      if (sVar2 <= sVar1) {
        *(byte *)(unaff_A6 + 0x98) = *(byte *)(unaff_A6 + 0x98) & 0x7f;
        candidate_9f432();
      }
    }
  }
  return;
}
