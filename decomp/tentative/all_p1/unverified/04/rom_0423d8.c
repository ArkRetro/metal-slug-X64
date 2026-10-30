/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0423d8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_423d8(void)

{
  char extraout_D1b;
  int unaff_A6;

  *(undefined1 *)(unaff_A6 + 0x9f) = 0;
  if (*(char *)(unaff_A6 + 0x80) == '\b') {
    *(undefined1 *)(unaff_A6 + 0x80) = 7;
    return;
  }
  candidate_431b6();
  if (extraout_D1b == '\0') {
    *(undefined1 *)(unaff_A6 + 0x80) = 7;
  }
  else {
    *(undefined1 *)(unaff_A6 + 0x80) = 8;
  }
  return;
}
