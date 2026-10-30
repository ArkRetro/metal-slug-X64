/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x052d36.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

byte candidate_52d36(void)

{
  byte in_D0b;
  byte bVar1;
  int unaff_A6;

  if (((-1 < *(int *)(unaff_A6 + 0x6e)) && (*(short *)(unaff_A6 + 0x2c) < 0x138)) &&
     (*(char *)(unaff_A6 + 0x90) == '\0')) {
    if ((*(char *)(unaff_A6 + 0x8b) == '\0') ||
       (bVar1 = *(byte *)(unaff_A6 + 0x8f) | *(byte *)(unaff_A6 + 0x8e), bVar1 == 0)) {
      bVar1 = candidate_29c6();
      bVar1 = bVar1 & 0xf;
      if (*(byte *)(unaff_A6 + 0x8c) <= bVar1) {
        return bVar1;
      }
    }
    return bVar1;
  }
  return in_D0b;
}
