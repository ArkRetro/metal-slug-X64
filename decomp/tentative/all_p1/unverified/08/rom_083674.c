/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x083674.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_83674(void)

{
  ushort uVar1;
  short sVar2;
  int unaff_A6;

  uVar1 = candidate_29c6();
  if (uVar1 < 0x2667) {
    sVar2 = *(short *)(unaff_A6 + 0x70) + (*(short *)(unaff_A6 + 0x70) >> 1);
  }
  else if (uVar1 < 0x4ccd) {
    sVar2 = *(short *)(unaff_A6 + 0x70) + (*(short *)(unaff_A6 + 0x70) >> 2);
  }
  else if (uVar1 < 0xa666) {
    sVar2 = *(short *)(unaff_A6 + 0x70);
  }
  else if (uVar1 < 0xcccd) {
    sVar2 = *(short *)(unaff_A6 + 0x70) >> 1;
  }
  else {
    sVar2 = *(short *)(unaff_A6 + 0x70) >> 3;
  }
  *(short *)(unaff_A6 + 0x6e) = sVar2;
  return;
}
