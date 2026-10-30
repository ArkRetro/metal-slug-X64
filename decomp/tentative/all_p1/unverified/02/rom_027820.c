/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x027820.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_27820(void)

{
  short sVar1;
  short sVar2;
  short in_D0w;
  ushort in_D1w;
  int unaff_A6;

  sVar2 = (in_D1w & 0xff) * 2;
  sVar1 = *(short *)(sVar2 + 0x29fc);
  *(int *)(unaff_A6 + 0x30) = (int)*(short *)(sVar2 + 0x2a7c) * (int)in_D0w;
  *(int *)(unaff_A6 + 0x34) = (int)sVar1 * (int)in_D0w;
  return;
}
