/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d8f14.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d8f14(void)

{
  short sVar1;
  short sVar2;
  int unaff_A6;

  sVar2 = (*(byte *)(unaff_A6 + 0x8d) & 0xf) << 2;
  sVar1 = *(short *)(sVar2 + 0xe992a);
  sVar2 = *(short *)(sVar2 + 0xe9928) + -0xee0;
  if (0x110 < sVar2) {
    sVar2 = 0x110;
  }
  *(short *)(unaff_A6 + 0x8e) = sVar2;
  *(short *)(unaff_A6 + 0x90) = sVar1 + -0x308;
  return;
}
