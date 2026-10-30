/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08086a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8086a(void)

{
  int unaff_A6;

  if (*(short *)(unaff_A6 + 0x30) == 0) {
    return;
  }
  if (*(short *)(unaff_A6 + 0x30) < 0) {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) & 0xfe;
  }
  else {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 1;
  }
  return;
}
