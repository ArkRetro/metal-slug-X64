/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d32be.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d32be(void)

{
  int unaff_A6;

  if ((*(byte *)(unaff_A6 + 0x70) & 0x20) != 0) {
    return;
  }
  if (0x1f < *(short *)(unaff_A6 + 0x3c)) {
    if (*(short *)(unaff_A6 + 0x3c) < 0x121) {
      if (*(char *)(unaff_A6 + 0x74) != '\0') {
        return;
      }
      if (*(short *)(unaff_A6 + 0x3c) < *(short *)(*(int *)(unaff_A6 + 0x7e) + 0x3c))
      goto LAB_000d32fe;
    }
    *(undefined1 *)(unaff_A6 + 0x72) = 1;
    return;
  }
LAB_000d32fe:
  *(undefined1 *)(unaff_A6 + 0x72) = 0;
  return;
}
