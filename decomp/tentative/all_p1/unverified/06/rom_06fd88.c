/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06fd88.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6fd88(void)

{
  short sVar1;
  int unaff_A6;
  bool bVar2;

  sVar1 = *(short *)(unaff_A6 + 0x30);
  bVar2 = sVar1 < 0;
  if (bVar2) {
    sVar1 = sVar1 + 1;
    bVar2 = sVar1 < 0;
  }
  if (sVar1 == 0) {
    *(short *)(unaff_A6 + 0x7c) = *(short *)(unaff_A6 + 0x7c) - (*(short *)(unaff_A6 + 0x7c) >> 2);
  }
  else if (bVar2) {
    sVar1 = *(short *)(unaff_A6 + 0x7c) + -0x55;
    if (-0x280 < sVar1) {
      *(short *)(unaff_A6 + 0x7c) = sVar1;
    }
  }
  else {
    sVar1 = *(short *)(unaff_A6 + 0x7c) + 0x55;
    if (sVar1 < 0x280) {
      *(short *)(unaff_A6 + 0x7c) = sVar1;
    }
  }
  sVar1 = *(short *)(unaff_A6 + 0x34);
  bVar2 = sVar1 < 0;
  if (bVar2) {
    sVar1 = sVar1 + 1;
    bVar2 = sVar1 < 0;
  }
  if (sVar1 == 0) {
    *(short *)(unaff_A6 + 0x7e) = *(short *)(unaff_A6 + 0x7e) - (*(short *)(unaff_A6 + 0x7e) >> 2);
  }
  else if (bVar2) {
    sVar1 = *(short *)(unaff_A6 + 0x7e) + 0x55;
    if (sVar1 < 0x280) {
      *(short *)(unaff_A6 + 0x7e) = sVar1;
    }
  }
  else {
    sVar1 = *(short *)(unaff_A6 + 0x7e) + -0x55;
    if (-0x280 < sVar1) {
      *(short *)(unaff_A6 + 0x7e) = sVar1;
    }
  }
  return;
}
