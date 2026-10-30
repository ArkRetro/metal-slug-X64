/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x078d96.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_78d96(void)

{
  int iVar1;
  int unaff_A6;

  *(short *)(unaff_A6 + 0x7e) = *(short *)(unaff_A6 + 0x7e) + 2;
  *(ushort *)(unaff_A6 + 0x7e) = *(ushort *)(unaff_A6 + 0x7e) & 0x7f;
  *(int *)(unaff_A6 + 0x2c) = *(int *)(unaff_A6 + 0x2c) - *(int *)(unaff_A6 + 0x80);
  iVar1 = *(short *)((short)(*(short *)(unaff_A6 + 0x7e) * 2) + 0x29fc) * 0x400;
  *(int *)(unaff_A6 + 0x2c) = iVar1 + *(int *)(unaff_A6 + 0x2c);
  *(int *)(unaff_A6 + 0x80) = iVar1;
  return;
}
