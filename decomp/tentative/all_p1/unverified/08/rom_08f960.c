/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08f960.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8f960(void)

{
  int unaff_A6;

  if (*(short *)(unaff_A6 + 0x3c) < 0x31) {
    *(byte *)(unaff_A6 + 0x93) = *(byte *)(unaff_A6 + 0x93) | 0x80;
  }
  else {
    if (*(short *)(unaff_A6 + 0x3c) < 0x110) {
      return;
    }
    *(byte *)(unaff_A6 + 0x93) = *(byte *)(unaff_A6 + 0x93) & 0x7f;
  }
  return;
}
