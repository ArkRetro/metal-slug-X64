/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07c106.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7c106(void)

{
  int unaff_A6;

  if (((*(byte *)(unaff_A6 + 0x80) & 1) == 0) &&
     (*(short *)(unaff_A6 + 0x4e) <= *(short *)(unaff_A6 + 0x7e))) {
    candidate_120f8();
    *(byte *)(unaff_A6 + 0x80) = *(byte *)(unaff_A6 + 0x80) | 1;
    candidate_1da40();
    candidate_1da40();
    return;
  }
  return;
}
