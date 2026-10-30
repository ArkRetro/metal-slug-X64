/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x028d24.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_28d24(void)

{
  int unaff_A6;

  if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
    if ((*(byte *)(unaff_A6 + 0x7d) & 0x80) != 0) {
      return;
    }
  }
  else if ((*(byte *)(unaff_A6 + 0x7d) & 0x80) == 0) {
    return;
  }
  return;
}
