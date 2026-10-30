/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03faf2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3faf2(void)

{
  int unaff_A6;

  if ((*(ushort *)(unaff_A6 + 0x6e) & 1) == 0) {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) & 0xfe;
  }
  else {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 1;
  }
  return;
}
