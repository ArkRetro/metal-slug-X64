/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09cf50.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9cf50(void)

{
  short sVar1;
  int unaff_A6;
  undefined1 in_CF;

  candidate_9cec4();
  if (!(bool)in_CF) {
    candidate_1af9a();
    if ((!(bool)in_CF) && ((*(byte *)(unaff_A6 + 0x5f) & 1) == 0)) {
      candidate_9ce96();
      if (!(bool)in_CF) {
        sVar1 = candidate_9ce82();
        if (sVar1 <= *(short *)(unaff_A6 + 0x7c)) {
          return;
        }
      }
    }
  }
  return;
}
