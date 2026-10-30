/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x068bb8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_68bb8(void)

{
  short sVar1;
  int extraout_A0;
  int unaff_A6;
  undefined1 in_CF;

  candidate_1af9a();
  if (!(bool)in_CF) {
    if (*(short *)(unaff_A6 + 0x6e) == 1) {
      if (*(short *)(unaff_A6 + 0x96) != 0) {
        *(short *)(unaff_A6 + 0x96) = *(short *)(unaff_A6 + 0x96) + -1;
      }
      return;
    }
    candidate_68f66();
    *(int *)(unaff_A6 + 0x86) = extraout_A0;
    sVar1 = *(short *)(unaff_A6 + 0x3c) - *(short *)(extraout_A0 + 0x3c);
    if (sVar1 < 0) {
      if (((*(byte *)(unaff_A6 + 0x15) & 1) != 0) &&
         (-*(short *)(unaff_A6 + 0x94) == sVar1 || -sVar1 < *(short *)(unaff_A6 + 0x94))) {
        return;
      }
    }
    else if (((*(byte *)(unaff_A6 + 0x15) & 1) == 0) && (sVar1 <= *(short *)(unaff_A6 + 0x94))) {
      return;
    }
  }
  return;
}
