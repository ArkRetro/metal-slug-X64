/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0767dc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_767dc(void)

{
  int iVar1;
  ushort uVar2;
  int unaff_A6;

  iVar1 = *(int *)(unaff_A6 + 0xc);
  *(short *)(iVar1 + 0x70) = *(short *)(iVar1 + 0x70) + -1;
  uVar2 = *(ushort *)(unaff_A6 + 0x7e);
  *(byte *)(iVar1 + 0x9c) = *(byte *)(iVar1 + 0x9c) & ~('\x01' << (uVar2 & 7));
  *(byte *)(iVar1 + 0x9d) = *(byte *)(iVar1 + 0x9d) & ~('\x01' << (uVar2 & 7));
  *(undefined2 *)(iVar1 + (short)(uVar2 * 2 + 0x86)) = 0xffff;
  *(short *)(iVar1 + 0x9a) = *(short *)(iVar1 + 0x9a) + -1;
  return;
}
