/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09f3b0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9f3b0(void)

{
  short sVar1;
  int extraout_A0;
  int unaff_A6;
  undefined1 in_CF;

  candidate_9f07a();
  candidate_1b08a();
  if ((bool)in_CF) {
    sVar1 = 0x30;
    if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
      sVar1 = -0x30;
    }
  }
  else {
    sVar1 = *(short *)(extraout_A0 + 0x3c) - *(short *)(unaff_A6 + 0x3c);
    if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
      sVar1 = -sVar1;
    }
    if (sVar1 < 0x30) {
      sVar1 = 0x30;
    }
    if (*(short *)(unaff_A6 + 0x82) < sVar1) {
      sVar1 = *(short *)(unaff_A6 + 0x82);
    }
    if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
      sVar1 = -sVar1;
    }
  }
  *(short *)(unaff_A6 + 0x86) = sVar1;
  return;
}
