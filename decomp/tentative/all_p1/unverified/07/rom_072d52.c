/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x072d52.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_72d52(void)

{
  byte bVar1;
  short sVar2;
  int unaff_A6;

  sVar2 = -0x8000 - *(short *)(unaff_A6 + 0x74);
  if (sVar2 < 0) {
    sVar2 = -sVar2;
  }
  bVar1 = (byte)((ushort)sVar2 >> 8);
  *(byte *)(unaff_A6 + 0x78) = bVar1 >> 3;
  *(byte *)(unaff_A6 + 0x79) = bVar1 >> 4;
  return;
}
