/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08f90a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8f90a(void)

{
  int unaff_A6;

  if ((0x148 < *(short *)(unaff_A6 + 0x28)) && (*(short *)(unaff_A6 + 0x28) < 0x1b0)) {
    return;
  }
  if ((*(short *)(unaff_A6 + 0x3c) < 0x31) && ((*(byte *)(unaff_A6 + 0x93) & 0x80) == 0)) {
    *(byte *)(unaff_A6 + 0x93) = *(byte *)(unaff_A6 + 0x93) | 0x80;
  }
  else {
    if (*(short *)(unaff_A6 + 0x3c) < 0x110) {
      return;
    }
    if ((*(byte *)(unaff_A6 + 0x93) & 0x80) == 0) {
      return;
    }
    *(byte *)(unaff_A6 + 0x93) = *(byte *)(unaff_A6 + 0x93) & 0x7f;
  }
  return;
}
