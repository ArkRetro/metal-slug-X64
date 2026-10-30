/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08a528.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8a528(void)

{
  short sVar1;
  int unaff_A6;

  sVar1 = candidate_8a502();
  sVar1 = *(short *)(unaff_A6 + 0x7e) + sVar1;
  if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
    sVar1 = -sVar1;
  }
  *(int *)(unaff_A6 + 0x6e) = (int)sVar1 << 8;
  return;
}
