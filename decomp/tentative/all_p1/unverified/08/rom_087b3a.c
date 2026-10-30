/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x087b3a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_87b3a(void)

{
  short sVar1;
  int unaff_A6;

  sVar1 = *(short *)(unaff_A6 + 0x3c) - *(short *)(unaff_A6 + 0x7e);
  if (sVar1 < 0) {
    sVar1 = -sVar1;
  }
  if (8 < sVar1) {
    return;
  }
  *(byte *)(unaff_A6 + 0x8b) = *(byte *)(unaff_A6 + 0x8b) | 4;
  return;
}
