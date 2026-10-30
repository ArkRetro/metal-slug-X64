/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a4006.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a4006(void)

{
  short sVar1;
  short sVar2;
  int unaff_A6;

  sVar2 = (*(byte *)(unaff_A6 + 0x8d) & 0x1f) << 4;
  sVar1 = *(short *)(sVar2 + 0x29fc);
  *(int *)(unaff_A6 + 0x30) = (int)*(short *)(sVar2 + 0x2a7c) * (int)*(short *)(unaff_A6 + 0x62);
  *(int *)(unaff_A6 + 0x34) = (int)sVar1 * (int)*(short *)(unaff_A6 + 0x62);
  return;
}
