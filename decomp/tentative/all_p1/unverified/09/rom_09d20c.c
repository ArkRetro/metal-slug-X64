/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09d20c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9d20c(void)

{
  int unaff_A6;
  undefined1 in_CF;

  if ((*(byte *)(unaff_A6 + 0x90) & 4) != 0) {
    candidate_1affc();
    if (!(bool)in_CF) {
      *(byte *)(unaff_A6 + 0x90) = *(byte *)(unaff_A6 + 0x90) | 8;
      return;
    }
    candidate_1affc();
    if (!(bool)in_CF) {
      *(byte *)(unaff_A6 + 0x90) = *(byte *)(unaff_A6 + 0x90) & 0xf7;
      return;
    }
  }
  return;
}
