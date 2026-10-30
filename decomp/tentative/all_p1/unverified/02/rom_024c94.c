/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x024c94.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_24c94(void)

{
  int iVar1;
  short sVar2;
  int unaff_A6;

  if (*(short *)(unaff_A6 + 0x7a) != *(short *)(unaff_A6 + 0x4e)) {
    iVar1 = *(int *)(unaff_A6 + 0xc);
    sVar2 = *(short *)(iVar1 + 0x4e) - (*(short *)(unaff_A6 + 0x7a) - *(short *)(unaff_A6 + 0x4e));
    *(short *)(iVar1 + 0x4e) = sVar2;
    if (sVar2 < 0) {
      *(undefined2 *)(iVar1 + 0x4e) = 0;
      *(byte *)(iVar1 + 0x5f) = *(byte *)(iVar1 + 0x5f) | 0x80;
      *(undefined1 *)(iVar1 + 0x5e) = *(undefined1 *)(unaff_A6 + 0x5e);
    }
    *(undefined2 *)(unaff_A6 + 0x7a) = *(undefined2 *)(unaff_A6 + 0x4e);
  }
  return;
}
