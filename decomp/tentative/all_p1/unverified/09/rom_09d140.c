/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09d140.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9d140(void)

{
  int unaff_A6;

  if ((*(byte *)(unaff_A6 + 0x15) & 0x40) == 0) {
    *(char *)(unaff_A6 + 0x88) = *(char *)(unaff_A6 + 0x88) + '\x01';
    if ('\x06' < *(char *)(unaff_A6 + 0x88)) {
      *(undefined1 *)(unaff_A6 + 0x88) = 6;
    }
  }
  else {
    *(undefined1 *)(unaff_A6 + 0x88) = 0;
  }
  return;
}
