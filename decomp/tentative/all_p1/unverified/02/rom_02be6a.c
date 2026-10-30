/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02be6a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2be6a(void)

{
  int unaff_A6;

  if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
    if (*(short *)(unaff_A6 + 0x7e) < *(short *)(unaff_A6 + 0x3c)) {
      return;
    }
  }
  else if (*(short *)(unaff_A6 + 0x3c) < *(short *)(unaff_A6 + 0x7e)) {
    return;
  }
  return;
}
