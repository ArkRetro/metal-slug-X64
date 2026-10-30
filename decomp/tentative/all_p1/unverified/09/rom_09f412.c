/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09f412.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9f412(void)

{
  byte bVar1;
  int unaff_A6;

  if ((*(byte *)(unaff_A6 + 0x98) & 0x10) != 0) {
    bVar1 = candidate_29c6();
    if ((bVar1 & 1) == 0) {
      *(byte *)(unaff_A6 + 0x98) = *(byte *)(unaff_A6 + 0x98) ^ 0x20;
    }
  }
  return;
}
