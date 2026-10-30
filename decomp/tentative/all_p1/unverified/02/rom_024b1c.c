/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x024b1c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_24b1c(void)

{
  short sVar1;
  int unaff_A6;

  sVar1 = candidate_1b14a();
  if (sVar1 < 0x80) {
    sVar1 = 0x100 - sVar1;
  }
  if (sVar1 < 0xa9) {
    *(undefined1 *)(unaff_A6 + 0x9d) = 0;
  }
  else if (sVar1 < 0xd8) {
    *(undefined1 *)(unaff_A6 + 0x9d) = 4;
  }
  else {
    *(undefined1 *)(unaff_A6 + 0x9d) = 8;
  }
  return;
}
