/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x087a1e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_87a1e(void)

{
  short sVar1;
  int unaff_A6;
  undefined1 in_CF;

  if (((*(byte *)(unaff_A6 + 0x8b) & 0x10) != 0) && ((*(byte *)(unaff_A6 + 0x5f) & 1) == 0)) {
    candidate_87964();
    if (!(bool)in_CF) {
      sVar1 = *(short *)(unaff_A6 + 0x3c) - *(short *)(*(int *)(unaff_A6 + 0x6e) + 0x3c);
      if (sVar1 < 0) {
        sVar1 = -sVar1;
      }
      if (sVar1 < 0x39) {
        return;
      }
    }
  }
  return;
}
