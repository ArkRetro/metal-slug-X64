/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09d19e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9d19e(void)

{
  short sVar1;
  int unaff_A6;
  undefined1 in_CF;
  bool bVar2;

  candidate_9d178();
  if ((((bool)in_CF) && ((*(byte *)(unaff_A6 + 0x90) & 0x20) == 0)) &&
     (bVar2 = false, *(short *)(unaff_A6 + 0x7a) < 1)) {
    candidate_1affc();
    if (!bVar2) {
      sVar1 = candidate_9ce82();
      if ((short)(sVar1 + -0x10) <= *(short *)(unaff_A6 + 0x7c)) {
        return;
      }
    }
  }
  return;
}
