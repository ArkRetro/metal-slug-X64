/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x027890.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_27890(void)

{
  int unaff_A6;

  *(char *)(unaff_A6 + 0x80) = *(char *)(unaff_A6 + 0x80) + -1;
  if (*(char *)(unaff_A6 + 0x80) < '\x01') {
    *(undefined1 *)(unaff_A6 + 0x80) = *(undefined1 *)(unaff_A6 + 0x81);
    *(char *)(unaff_A6 + 0x84) = *(char *)(unaff_A6 + 0x84) + -1;
    if (*(char *)(unaff_A6 + 0x84) < '\x01') {
      *(undefined1 *)(unaff_A6 + 0x84) = 0;
      return;
    }
  }
  return;
}
