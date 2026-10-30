/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04d768.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4d768(void)

{
  int unaff_A6;

  if (*(char *)(unaff_A6 + 0x9a) == *(char *)(unaff_A6 + 0x9c)) {
    return;
  }
  if (*(char *)(unaff_A6 + 0x9a) < *(char *)(unaff_A6 + 0x9c)) {
    *(char *)(unaff_A6 + 0x9a) = *(char *)(unaff_A6 + 0x9a) + '\x01';
  }
  else {
    *(char *)(unaff_A6 + 0x9a) = *(char *)(unaff_A6 + 0x9a) + -1;
  }
  return;
}
