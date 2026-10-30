/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x091898.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_91898(void)

{
  char cVar1;
  int unaff_A6;

  if ((*(byte *)(unaff_A6 + 0x5f) & 1) == 0) {
    *(undefined1 *)(unaff_A6 + 0x8b) = 0;
  }
  else {
    *(char *)(unaff_A6 + 0x8b) = *(char *)(unaff_A6 + 0x8b) + '\x01';
    cVar1 = '\n';
    if ((0x30 < *(short *)(unaff_A6 + 0x3c)) && (*(short *)(unaff_A6 + 0x3c) < 0x110)) {
      cVar1 = '\x1e';
    }
    if (cVar1 < *(char *)(unaff_A6 + 0x8b)) {
      *(byte *)(unaff_A6 + 0x5f) = *(byte *)(unaff_A6 + 0x5f) & 0xfe;
      return;
    }
  }
  return;
}
