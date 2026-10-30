/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0044f6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_44f6(void)

{
  byte bVar1;
  int unaff_A6;

  bVar1 = *(byte *)(*(int *)(unaff_A6 + 0xc) + 0x17);
  if (bVar1 != 0) {
    if (-1 < (char)bVar1) {
      bVar1 = bVar1 + 1;
    }
    *(byte *)(unaff_A6 + 0x17) = bVar1;
    if ((bVar1 & 1) == 0) {
      return;
    }
  }
  return;
}
