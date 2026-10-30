/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06b946.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6b946(void)

{
  short sVar1;
  int unaff_A6;
  undefined1 in_CF;

  if ((*(byte *)(unaff_A6 + 0x92) & 1) == 0) {
    candidate_6b3fc();
    candidate_1b08a();
    if (!(bool)in_CF) {
      sVar1 = candidate_6b874();
      if (sVar1 < 0x21) {
        sVar1 = candidate_6b860();
        if (sVar1 < *(short *)(unaff_A6 + 0x8e)) {
          return;
        }
      }
    }
  }
  return;
}
