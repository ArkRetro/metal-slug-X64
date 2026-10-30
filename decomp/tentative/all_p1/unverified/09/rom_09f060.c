/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09f060.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9f060(void)

{
  int unaff_A6;

  if (*(char *)(unaff_A6 + 0x5b) < '\0') {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 1;
  }
  else {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) & 0xfe;
  }
  return;
}
