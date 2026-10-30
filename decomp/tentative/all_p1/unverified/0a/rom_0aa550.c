/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0aa550.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_aa550(void)

{
  undefined4 uVar1;
  int iVar2;
  ushort uVar3;
  byte bVar4;
  short *psVar5;
  int unaff_A5;
  int unaff_A6;

  if (*(short *)(unaff_A6 + 0x70) == 0) {
    psVar5 = (short *)(unaff_A5 + 0x4608);
  }
  else {
    psVar5 = (short *)(unaff_A5 + 0x462c);
  }
  if (*psVar5 != 0) {
    bVar4 = *(byte *)(psVar5 + 0x11);
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) & 0xbf;
    *(byte *)(unaff_A6 + 0x15) = bVar4 & 0x40 | *(byte *)(unaff_A6 + 0x15);
    uVar1 = *(undefined4 *)(psVar5 + 3);
    uVar3 = psVar5[2];
    iVar2 = *(int *)(psVar5 + 7);
    *(uint *)(unaff_A6 + 0x28) = *(int *)(psVar5 + 5) + (uint)(ushort)psVar5[1] * 0x10000;
    *(uint *)(unaff_A6 + 0x2c) = iVar2 + (uint)uVar3 * 0x10000;
    *(undefined4 *)(unaff_A6 + 0x30) = 0;
    *(undefined4 *)(unaff_A6 + 0x34) = 0;
    *(undefined4 *)(unaff_A6 + 0x38) = uVar1;
    candidate_1abc8();
  }
  return;
}
