/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01dde8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1dde8(void)

{
  ushort uVar1;
  short sVar2;
  int extraout_A0;
  int extraout_A0_00;
  int iVar3;
  int unaff_A6;
  bool bVar4;

  bVar4 = false;
  candidate_40bc();
  iVar3 = extraout_A0;
  if (bVar4) {
    candidate_1b220();
    iVar3 = extraout_A0_00;
  }
  *(undefined2 *)(iVar3 + 0x8c) = 0x1e;
  uVar1 = candidate_29c6();
  sVar2 = (*(ushort *)(unaff_A6 + 0x72) & uVar1) - (*(ushort *)(unaff_A6 + 0x72) >> 1);
  if (*(char *)(unaff_A6 + 0x12) != -1) {
    sVar2 = (short)(sVar2 * (*(byte *)(unaff_A6 + 0x12) + 1)) >> 8;
  }
  *(short *)(iVar3 + 0x28) = sVar2 + *(short *)(iVar3 + 0x28);
  uVar1 = candidate_29c6();
  sVar2 = (*(ushort *)(unaff_A6 + 0x74) & uVar1) + 8;
  if (*(char *)(unaff_A6 + 0x13) != -1) {
    sVar2 = (short)(sVar2 * (*(byte *)(unaff_A6 + 0x13) + 1)) >> 8;
  }
  *(short *)(iVar3 + 0x2c) = sVar2 + *(short *)(iVar3 + 0x2c);
  *(undefined1 *)(iVar3 + 0x12) = *(undefined1 *)(unaff_A6 + 0x12);
  *(undefined1 *)(iVar3 + 0x13) = *(undefined1 *)(unaff_A6 + 0x13);
  return;
}
