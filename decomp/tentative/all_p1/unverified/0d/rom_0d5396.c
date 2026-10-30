/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d5396.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d5396(void)

{
  short sVar1;
  int unaff_A6;

  if (*(char *)(unaff_A6 + 0x85) == '\0') {
    sVar1 = candidate_260c8();
    *(short *)(unaff_A6 + 0x78) = (sVar1 >> 1) + 0x14;
    *(undefined1 *)(unaff_A6 + 0x85) = 1;
    *(undefined1 *)(unaff_A6 + 0x7f) = 0;
    *(undefined1 *)(unaff_A6 + 0x80) = 0;
    *(undefined1 *)(unaff_A6 + 0x81) = 0;
    *(undefined1 *)(unaff_A6 + 0x82) = 2;
    *(undefined1 *)(unaff_A6 + 0x7e) = 0;
    return;
  }
  if (*(char *)(unaff_A6 + 0x85) == '\x01') {
    *(undefined1 *)(unaff_A6 + 0x7f) = 0;
    *(undefined1 *)(unaff_A6 + 0x80) = 0;
    *(undefined1 *)(unaff_A6 + 0x81) = 0;
    *(undefined1 *)(unaff_A6 + 0x82) = 3;
    *(undefined1 *)(unaff_A6 + 0x7e) = 0;
    sVar1 = candidate_260c8();
    *(short *)(unaff_A6 + 0x78) = (sVar1 >> 1) + 4;
    *(undefined1 *)(unaff_A6 + 0x85) = 2;
    return;
  }
  sVar1 = candidate_260c8();
  *(short *)(unaff_A6 + 0x78) = (sVar1 >> 1) + 10;
  *(undefined1 *)(unaff_A6 + 0x85) = 1;
  *(undefined1 *)(unaff_A6 + 0x7f) = 0;
  *(undefined1 *)(unaff_A6 + 0x80) = 0;
  *(undefined1 *)(unaff_A6 + 0x81) = 0;
  *(undefined1 *)(unaff_A6 + 0x82) = 6;
  *(undefined1 *)(unaff_A6 + 0x7e) = 0;
  return;
}
