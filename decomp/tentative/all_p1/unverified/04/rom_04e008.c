/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04e008.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4e008(void)

{
  short sVar2;
  int iVar1;
  int unaff_A6;

  for (sVar2 = 0; *(short *)(sVar2 + 0xe108a) < *(short *)(unaff_A6 + 0x98); sVar2 = sVar2 + 4) {
  }
  iVar1 = *(int *)((short)(*(short *)(sVar2 + 0xe108c) << 2) + 0xe103a);
  if (iVar1 == 0xe0e8a) {
    iVar1 = *(int *)((short)((ushort)*(byte *)(unaff_A6 + 0xa3) << 2) + 0xe0e8a);
  }
  *(int *)(unaff_A6 + 0x50) = iVar1;
  return;
}
