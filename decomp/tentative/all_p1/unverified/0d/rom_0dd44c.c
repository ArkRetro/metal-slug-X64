/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0dd44c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_dd44c(void)

{
  int unaff_A6;

  *(undefined1 *)(unaff_A6 + 0x9b) = *(undefined1 *)(*(int *)(unaff_A6 + 0xc) + 0x9b);
  *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) & 0xfe;
  if ((*(byte *)(*(int *)(unaff_A6 + 0xc) + 0x15) & 1) != 0) {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 1;
  }
  return;
}
