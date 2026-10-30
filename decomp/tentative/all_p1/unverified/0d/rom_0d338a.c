/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d338a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

byte candidate_d338a(void)

{
  byte bVar1;
  int unaff_A6;
  bool bVar2;

  bVar1 = *(byte *)(unaff_A6 + 0x70) & 7;
  if ((*(byte *)(unaff_A6 + 0x70) & 7) != 0) {
    return bVar1;
  }
  if (((*(byte *)(unaff_A6 + 0x70) & 0x10) == 0) || (*(char *)(unaff_A6 + 0x74) == '\0')) {
    bVar2 = false;
    if ((*(byte *)(unaff_A6 + 0x70) & 8) == 0) {
      return bVar1;
    }
    bVar1 = candidate_d33ee();
    if (!bVar2) {
      bVar1 = candidate_29c6();
      bVar1 = bVar1 & 7;
      if (bVar1 == 0) {
        *(byte *)(unaff_A6 + 0x70) = *(byte *)(unaff_A6 + 0x70) | 2;
        return 0;
      }
    }
    *(byte *)(unaff_A6 + 0x70) = *(byte *)(unaff_A6 + 0x70) | 1;
  }
  else {
    *(byte *)(unaff_A6 + 0x70) = *(byte *)(unaff_A6 + 0x70) | 4;
  }
  return bVar1;
}
