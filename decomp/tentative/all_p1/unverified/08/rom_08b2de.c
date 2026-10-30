/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08b2de.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8b2de(void)

{
  short sVar1;
  int unaff_A6;

  sVar1 = *(short *)((short)((ushort)*(byte *)(unaff_A6 + 0x78) * 2) + 0x29fc);
  *(short *)(unaff_A6 + 0x78) = *(short *)(unaff_A6 + 0x78) + 0x400;
  *(int *)(unaff_A6 + 0x34) =
       (short)(*(short *)((short)((ushort)*(byte *)(unaff_A6 + 0x78) * 2) + 0x29fc) - sVar1) * 0x200
  ;
  return;
}
