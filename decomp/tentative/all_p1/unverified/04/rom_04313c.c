/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04313c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4313c(void)

{
  short sVar1;
  int extraout_A0;
  int unaff_A6;

  sVar1 = *(short *)(unaff_A6 + 0x86);
  *(short *)(unaff_A6 + 0x86) = sVar1 + -1;
  if (sVar1 == 0) {
    if (*(byte *)(unaff_A6 + 0x89) <= *(byte *)(unaff_A6 + 0x88)) {
      return;
    }
    *(byte *)(unaff_A6 + 0x88) = *(byte *)(unaff_A6 + 0x88) + 1;
    *(undefined2 *)(unaff_A6 + 0x86) = 0x3c;
    candidate_411c();
    *(undefined4 *)(extraout_A0 + 0x6e) = *(undefined4 *)(unaff_A6 + 0x6e);
    *(undefined4 *)(extraout_A0 + 0x72) = *(undefined4 *)(unaff_A6 + 0x72);
    *(undefined1 *)(extraout_A0 + 0x80) = *(undefined1 *)(unaff_A6 + 0x80);
    *(char *)(extraout_A0 + 0x69) = *(char *)(unaff_A6 + 0x80) + -3;
    *(undefined1 *)(extraout_A0 + 0xa0) = *(undefined1 *)(unaff_A6 + 0x88);
    candidate_1ab98();
  }
  return;
}
