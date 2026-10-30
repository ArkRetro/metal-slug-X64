/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x049c52.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_49c52(void)

{
  char in_D0b;
  int unaff_A6;

  *(char *)(unaff_A6 + 0x96) = *(char *)(unaff_A6 + 0x96) + -3;
  if (in_D0b == *(char *)(unaff_A6 + 0x96)) {
    return;
  }
  if (in_D0b < *(char *)(unaff_A6 + 0x96)) {
    *(char *)(unaff_A6 + 0x96) = *(char *)(unaff_A6 + 0x96) + -1;
  }
  else {
    *(char *)(unaff_A6 + 0x96) = *(char *)(unaff_A6 + 0x96) + '\x01';
  }
  return;
}
