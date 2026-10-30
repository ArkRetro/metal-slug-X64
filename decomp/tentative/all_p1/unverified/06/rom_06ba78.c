/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06ba78.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6ba78(void)

{
  short sVar1;
  int unaff_A6;
  undefined1 in_CF;

  if ((*(byte *)(unaff_A6 + 0x92) & 1) == 0) {
    if ((*(byte *)(unaff_A6 + 0x92) & 0x10) != 0) {
      return;
    }
    candidate_6b3fc();
    candidate_1b08a();
    if ((!(bool)in_CF) &&
       (sVar1 = candidate_6b860(),
       (short)(sVar1 - *(ushort *)(unaff_A6 + 0x88)) <= (short)(*(ushort *)(unaff_A6 + 0x88) >> 2)))
    {
      return;
    }
  }
  return;
}
