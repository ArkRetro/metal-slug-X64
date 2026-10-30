/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01ad8c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1ad8c(void)

{
  int unaff_A6;

  if ((*(ushort *)(unaff_A6 + 0x66) & 0xf000) == 0x8000) {
    *(ushort *)(unaff_A6 + 0x1e) = *(ushort *)(unaff_A6 + 0x66) - *(short *)(unaff_A6 + 0x40);
  }
  return;
}
