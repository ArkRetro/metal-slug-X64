/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0597de.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_597de(void)

{
  int unaff_A6;

  if (*(char *)(unaff_A6 + 0x83) != '\0') {
    return;
  }
  *(undefined1 *)(unaff_A6 + 0x83) = 0xff;
  if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
    *(undefined4 *)(unaff_A6 + 0x72) = 0xffff0000;
  }
  else {
    *(undefined4 *)(unaff_A6 + 0x72) = 0x10000;
  }
  return;
}
