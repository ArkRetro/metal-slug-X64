/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0878a2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_878a2(void)

{
  short sVar1;
  short sVar2;
  int unaff_A6;

  sVar2 = (ushort)*(byte *)(unaff_A6 + 0x8a) * 2;
  sVar1 = *(short *)(sVar2 + 0x29fc);
  *(int *)(unaff_A6 + 0x30) = (int)*(short *)(sVar2 + 0x2a7c) * (int)*(short *)(unaff_A6 + 0x76);
  *(int *)(unaff_A6 + 0x34) = (int)sVar1 * (int)*(short *)(unaff_A6 + 0x76);
  return;
}
