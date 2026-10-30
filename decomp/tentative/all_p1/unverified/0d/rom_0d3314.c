/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d3314.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d3314(void)

{
  int unaff_A6;

  if (((*(byte *)(unaff_A6 + 0x70) & 0x20) == 0) &&
     (((*(short *)(unaff_A6 + 0x3c) < 0x20 || (0x120 < *(short *)(unaff_A6 + 0x3c))) ||
      (*(char *)(unaff_A6 + 0x74) == '\0')))) {
    if (*(char *)(unaff_A6 + 0x72) == '\0') {
      if ((*(short *)(unaff_A6 + 0x3c) < 0x121) &&
         (*(short *)(unaff_A6 + 0x3c) <= *(short *)(*(int *)(unaff_A6 + 0x7e) + 0x3c))) {
        return;
      }
    }
    else if ((0x2f < *(short *)(unaff_A6 + 0x3c)) &&
            (*(short *)(*(int *)(unaff_A6 + 0x7e) + 0x3c) <= *(short *)(unaff_A6 + 0x3c))) {
      return;
    }
  }
  return;
}
