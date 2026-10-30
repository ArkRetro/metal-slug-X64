/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x024aec.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_24aec(void)

{
  int unaff_A6;

  if (*(char *)(unaff_A6 + 0x9c) == *(char *)(unaff_A6 + 0x9d)) {
    return;
  }
  if (*(char *)(unaff_A6 + 0x9c) < *(char *)(unaff_A6 + 0x9d)) {
    *(char *)(unaff_A6 + 0x9c) = *(char *)(unaff_A6 + 0x9c) + '\x01';
  }
  else {
    *(char *)(unaff_A6 + 0x9c) = *(char *)(unaff_A6 + 0x9c) + -1;
  }
  return;
}
