/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07d8a6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7d8a6(void)

{
  short sVar1;
  int unaff_A6;

  sVar1 = *(short *)(*(int *)(unaff_A6 + 0xc) + 0x3c) - *(short *)(unaff_A6 + 0x3c);
  if (sVar1 < 0) {
    sVar1 = -sVar1;
  }
  if (sVar1 < 0xb) {
    sVar1 = *(short *)(*(int *)(unaff_A6 + 0xc) + 0x40) - *(short *)(unaff_A6 + 0x40);
    if (sVar1 < 0) {
      sVar1 = -sVar1;
    }
    if (sVar1 < 0x31) {
      return;
    }
  }
  return;
}
