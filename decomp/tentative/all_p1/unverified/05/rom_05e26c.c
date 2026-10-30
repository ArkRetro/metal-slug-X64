/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05e26c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5e26c(void)

{
  int unaff_A6;

  if ((*(byte *)(unaff_A6 + 0x5f) & 1) == 0) {
    *(undefined1 *)(unaff_A6 + 0x90) = 0;
  }
  else {
    *(undefined1 *)(unaff_A6 + 0x91) = 0x1e;
    *(char *)(unaff_A6 + 0x90) = *(char *)(unaff_A6 + 0x90) + '\x01';
    if ('\x0e' < *(char *)(unaff_A6 + 0x90)) {
      *(undefined1 *)(unaff_A6 + 0x90) = 0xf;
      return;
    }
  }
  return;
}
