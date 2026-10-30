/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05e244.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5e244(void)

{
  int unaff_A6;

  if ((*(byte *)(unaff_A6 + 0x15) & 0x40) == 0) {
    *(char *)(unaff_A6 + 0x8f) = *(char *)(unaff_A6 + 0x8f) + '\x01';
    if ('\x05' < *(char *)(unaff_A6 + 0x8f)) {
      return;
    }
  }
  else {
    *(undefined1 *)(unaff_A6 + 0x8f) = 0;
  }
  return;
}
