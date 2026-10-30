/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03fb10.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3fb10(void)

{
  int unaff_A6;

  *(int *)(unaff_A6 + 0x30) = (int)*(short *)(unaff_A6 + 0x70) << 8;
  if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
    *(int *)(unaff_A6 + 0x30) = -*(int *)(unaff_A6 + 0x30);
  }
  return;
}
