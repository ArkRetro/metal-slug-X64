/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01dea2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

byte candidate_1dea2(void)

{
  byte bVar1;
  int unaff_A6;

  bVar1 = candidate_29c6();
  if ((bVar1 & 2) == 0) {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) ^ 1;
  }
  return bVar1 & 2;
}
