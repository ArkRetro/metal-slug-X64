/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c7b4e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c7b4e(void)

{
  short sVar1;
  int unaff_A5;

  if (*(char *)(unaff_A5 + 0x695a) != '\0') {
    return;
  }
  *(undefined1 *)(unaff_A5 + 0x695b) = 0xff;
  sVar1 = *(short *)(unaff_A5 + 0x69b6);
  if (sVar1 == 1) {
    *(undefined4 *)(unaff_A5 + 0x695c) = 0xfffd0000;
    *(undefined1 *)(unaff_A5 + 0x695a) = 1;
    return;
  }
  if (sVar1 == 2) {
    *(undefined4 *)(unaff_A5 + 0x6960) = 0x30000;
    *(undefined1 *)(unaff_A5 + 0x695a) = 2;
    return;
  }
  if (sVar1 == 3) {
    *(undefined4 *)(unaff_A5 + 0x6960) = 0xfffd0000;
    *(undefined1 *)(unaff_A5 + 0x695a) = 2;
    return;
  }
  *(undefined4 *)(unaff_A5 + 0x695c) = 0x30000;
  *(undefined1 *)(unaff_A5 + 0x695a) = 1;
  return;
}
