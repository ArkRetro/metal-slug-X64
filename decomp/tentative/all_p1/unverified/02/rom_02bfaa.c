/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02bfaa.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2bfaa(void)

{
  short sVar1;
  int unaff_A6;

  if ((0x1f < *(short *)(unaff_A6 + 0x3c)) && (*(short *)(unaff_A6 + 0x3c) < 0x121)) {
    sVar1 = *(short *)(unaff_A6 + 0x84);
    *(short *)(unaff_A6 + 0x84) = sVar1 + -1;
    if (sVar1 == 0) {
      *(undefined2 *)(unaff_A6 + 0x84) = 0;
    }
    if (((*(byte *)(unaff_A6 + 0x91) & 0x80) != 0) && (*(short *)(unaff_A6 + 0x84) < 1)) {
      return;
    }
  }
  return;
}
