/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04976a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4976a(void)

{
  short sVar1;
  int in_A0;
  int unaff_A6;

  if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
    sVar1 = *(short *)(in_A0 + 0x3c);
    if (sVar1 < *(short *)(unaff_A6 + 0x3c)) {
      *(short *)(unaff_A6 + 0x78) = sVar1 + 0x50;
    }
    else {
      *(short *)(unaff_A6 + 0x78) = sVar1 + -0x50;
    }
  }
  else {
    sVar1 = *(short *)(in_A0 + 0x3c);
    if (*(short *)(unaff_A6 + 0x3c) < sVar1) {
      *(short *)(unaff_A6 + 0x78) = sVar1 + -0x50;
    }
    else {
      *(short *)(unaff_A6 + 0x78) = sVar1 + 0x50;
    }
  }
  if (*(short *)(unaff_A6 + 0x78) < 0x20) {
    *(short *)(unaff_A6 + 0x78) = *(short *)(in_A0 + 0x3c) + 0x50;
    return;
  }
  if (0x120 < *(short *)(unaff_A6 + 0x78)) {
    *(short *)(unaff_A6 + 0x78) = *(short *)(in_A0 + 0x3c) + -0x50;
    return;
  }
  return;
}
