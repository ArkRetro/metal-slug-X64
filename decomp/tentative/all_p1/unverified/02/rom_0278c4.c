/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0278c4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_278c4(void)

{
  byte bVar1;
  char cVar2;
  int unaff_A6;

  if (*(char *)(unaff_A6 + 0x82) == '\0') {
    *(undefined1 *)(unaff_A6 + 0x82) = 1;
  }
  *(char *)(unaff_A6 + 0x80) = *(char *)(unaff_A6 + 0x80) + -1;
  if (*(char *)(unaff_A6 + 0x80) < '\x01') {
    *(undefined1 *)(unaff_A6 + 0x80) = *(undefined1 *)(unaff_A6 + 0x81);
    while( true ) {
      bVar1 = *(byte *)(unaff_A6 + 0x82);
      *(char *)(unaff_A6 + 0x82) = *(char *)(unaff_A6 + 0x82) + '\x01';
      cVar2 = *(char *)((short)(bVar1 & 0x1f) + 0x27926);
      if (-1 < cVar2) break;
      if (cVar2 == -1) {
        *(undefined1 *)(unaff_A6 + 0x82) = 0;
        return;
      }
      candidate_27bae();
    }
    *(char *)(unaff_A6 + 0x84) = cVar2;
  }
  return;
}
