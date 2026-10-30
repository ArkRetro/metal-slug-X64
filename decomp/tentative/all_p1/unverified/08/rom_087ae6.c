/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x087ae6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_87ae6(void)

{
  int unaff_A6;
  bool bVar1;

  if (((((*(byte *)(unaff_A6 + 0x8b) & 0x20) != 0) && (*(short *)(unaff_A6 + 0x80) < 1)) &&
      (0x17 < *(short *)(unaff_A6 + 0x3c))) &&
     ((*(short *)(unaff_A6 + 0x3c) < 0x129 &&
      (bVar1 = *(ushort *)(unaff_A6 + 0x40) < 0x1e0, (short)*(ushort *)(unaff_A6 + 0x40) < 0x1e1))))
  {
    candidate_1af9a();
    if (!bVar1) {
      return;
    }
  }
  return;
}
