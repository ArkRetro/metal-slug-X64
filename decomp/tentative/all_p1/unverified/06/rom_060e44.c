/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x060e44.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_60e44(void)

{
  short sVar1;
  short sVar2;
  int unaff_A6;

  sVar1 = *(short *)(unaff_A6 + 0x6e) - *(short *)(unaff_A6 + 0x3c);
  sVar2 = *(short *)(unaff_A6 + 0x70) - *(short *)(unaff_A6 + 0x40);
  if ((sVar1 != 0) || (sVar2 != 0)) {
    *(int *)(unaff_A6 + 0x30) = (int)(short)(sVar1 * 0x40) << 8;
    *(int *)(unaff_A6 + 0x34) = (int)(short)(sVar2 * 0x40) << 8;
  }
  return;
}
