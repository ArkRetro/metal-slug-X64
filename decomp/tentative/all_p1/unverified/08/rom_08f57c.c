/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08f57c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8f57c(void)

{
  int unaff_A6;
  undefined1 in_CF;
  bool bVar1;

  candidate_8f636();
  if ((bool)in_CF) {
    *(byte *)(unaff_A6 + 0x93) = *(byte *)(unaff_A6 + 0x93) ^ 0x80;
  }
  else if ((*(short *)(unaff_A6 + 0x3c) < 0x31) && ((*(byte *)(unaff_A6 + 0x93) & 0x80) == 0)) {
    *(byte *)(unaff_A6 + 0x93) = *(byte *)(unaff_A6 + 0x93) | 0x80;
  }
  else {
    bVar1 = *(ushort *)(unaff_A6 + 0x3c) < 0x110;
    if (((short)*(ushort *)(unaff_A6 + 0x3c) < 0x110) || ((*(byte *)(unaff_A6 + 0x93) & 0x80) == 0))
    {
      candidate_8f5de();
      if (!bVar1) {
        return;
      }
    }
    else {
      *(byte *)(unaff_A6 + 0x93) = *(byte *)(unaff_A6 + 0x93) & 0x7f;
    }
  }
  return;
}
