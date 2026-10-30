/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0365f0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_365f0(void)

{
  int iVar1;
  char *pcVar2;
  ushort uVar3;
  int unaff_A6;

  iVar1 = *(int *)(*(int *)(unaff_A6 + 0xc) + 0x82);
  if (iVar1 != 0) {
    uVar3 = *(short *)(iVar1 + 0x3c) - *(short *)(unaff_A6 + 0x3c);
    if ((short)uVar3 < 0) {
      uVar3 = -uVar3;
    }
    if ((uVar3 < 0xb) && ((ushort)(*(short *)(unaff_A6 + 0x40) - *(short *)(iVar1 + 0x40)) < 0x33))
    {
      candidate_36464();
      if (*(char *)(*(int *)(unaff_A6 + 0xc) + 0x9e) == '\0') {
        pcVar2 = (char *)(*(int *)(unaff_A6 + 0xc) + 0x9e);
        *pcVar2 = *pcVar2 + '\x01';
      }
    }
  }
  return;
}
