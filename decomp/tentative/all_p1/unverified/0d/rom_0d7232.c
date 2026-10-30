/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d7232.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

short candidate_d7232(void)

{
  short sVar1;
  short sVar2;
  short sVar3;
  int unaff_A6;

  sVar3 = *(short *)(unaff_A6 + 0x82) << 4;
  sVar2 = *(short *)(sVar3 + 0x2a7c);
  sVar1 = *(short *)(unaff_A6 + 0xa8);
  *(int *)(unaff_A6 + 0x34) = (((int)*(short *)(sVar3 + 0x29fc) * (int)sVar1 >> 8) << 0x10) >> 8;
  *(int *)(unaff_A6 + 0x30) = (((int)sVar2 * (int)sVar1 >> 8) << 0x10) >> 8;
  return sVar1;
}
