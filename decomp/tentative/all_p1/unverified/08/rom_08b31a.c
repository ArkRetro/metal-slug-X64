/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08b31a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8b31a(void)

{
  short sVar1;
  int unaff_A6;

  sVar1 = *(short *)((short)((ushort)*(byte *)(unaff_A6 + 0x76) * 2) + 0x29fc);
  *(short *)(unaff_A6 + 0x76) = *(short *)(unaff_A6 + 0x74) + *(short *)(unaff_A6 + 0x76);
  *(int *)(unaff_A6 + 0x30) =
       (int)(short)(*(short *)((short)((ushort)*(byte *)(unaff_A6 + 0x76) * 2) + 0x29fc) - sVar1) *
       (int)*(short *)(unaff_A6 + 0x7a);
  if ((*(byte *)(unaff_A6 + 0x7f) & 8) == 0) {
    func_0x000143e4();
  }
  else {
    candidate_1ac38();
  }
  return;
}
