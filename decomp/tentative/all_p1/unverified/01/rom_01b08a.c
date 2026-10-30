/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01b08a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1b08a(void)

{
  short sVar1;
  int in_A0;
  int unaff_A6;

  sVar1 = *(short *)(unaff_A6 + 0x3c) - *(short *)(in_A0 + 0x3c);
  if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
    if (-1 < sVar1) {
      return;
    }
  }
  else if (sVar1 < 1) {
    return;
  }
  return;
}
