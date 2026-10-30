/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x081ff8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_81ff8(void)

{
  int unaff_A6;

  if ((*(char *)(unaff_A6 + 0x9d) == '\x01') && (*(char *)(unaff_A6 + 0x98) == '\0')) {
    candidate_4256();
    *(undefined1 *)(unaff_A6 + 0x9d) = 0;
  }
  if ((*(char *)(unaff_A6 + 0x9d) == '\x02') && (*(char *)(unaff_A6 + 0x98) == '\0')) {
    candidate_4256();
    *(undefined1 *)(unaff_A6 + 0x9d) = 0;
  }
  if (*(char *)(unaff_A6 + 0x9d) == '\x03') {
    if (*(char *)(unaff_A6 + 0x98) == '\x06') {
      candidate_4256();
      *(undefined1 *)(unaff_A6 + 0x9d) = 0;
    }
    if (*(char *)(unaff_A6 + 0x98) == '\x03') {
      candidate_4256();
      *(undefined1 *)(unaff_A6 + 0x9d) = 0;
    }
  }
  return;
}
