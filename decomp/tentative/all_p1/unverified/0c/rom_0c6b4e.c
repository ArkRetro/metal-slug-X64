/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c6b4e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c6b4e(void)

{
  short sVar1;
  int extraout_A0;
  int unaff_A6;

  if ((*(char *)(unaff_A6 + 0x97) != '\x01') && (*(char *)(unaff_A6 + 0x97) != '\x02')) {
    *(undefined4 *)(unaff_A6 + 0x84) = 0;
    candidate_1af9a();
    sVar1 = *(short *)(unaff_A6 + 0x3c) - *(short *)(extraout_A0 + 0x3c);
    if (sVar1 < 0) {
      sVar1 = -sVar1;
    }
    if (sVar1 <= *(short *)(unaff_A6 + 0x6e)) {
      *(undefined1 *)(unaff_A6 + 0x97) = 1;
    }
  }
  return;
}
