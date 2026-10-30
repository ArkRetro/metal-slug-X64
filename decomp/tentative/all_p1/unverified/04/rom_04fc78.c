/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04fc78.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4fc78(void)

{
  int unaff_A6;

  if (*(char *)(unaff_A6 + 0x60) == '\0') {
    return;
  }
  *(int *)(unaff_A6 + 0x30) = *(int *)(unaff_A6 + 0x30) + -0x12000;
  if (*(int *)(unaff_A6 + 0x30) < -0xa0000) {
    *(undefined4 *)(unaff_A6 + 0x30) = 0xa0000;
  }
  return;
}
