/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07c0ac.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7c0ac(void)

{
  int unaff_A6;
  undefined1 in_CF;

  if ((*(byte *)(unaff_A6 + 0x81) & 1) == 0) {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) & 0xfe;
  }
  else {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 1;
  }
  candidate_1b0be();
  if ((bool)in_CF) {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) & 0xfe;
  }
  else {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) & 0xfe;
  }
  return;
}
