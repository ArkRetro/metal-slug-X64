/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d35d2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d35d2(void)

{
  short sVar1;
  ushort uVar2;
  short sVar3;
  short sVar4;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int extraout_A0_02;
  int iVar5;
  int unaff_A6;
  bool bVar6;

  bVar6 = false;
  candidate_40bc();
  iVar5 = extraout_A0;
  if (bVar6) {
    candidate_1b220();
    iVar5 = extraout_A0_00;
  }
  sVar3 = *(short *)(unaff_A6 + 0x92);
  sVar1 = sVar3 << 2;
  sVar4 = *(short *)(sVar1 + 0xe8a6c);
  *(short *)(iVar5 + 0x2c) = *(short *)(sVar1 + 0xe8a6e) + *(short *)(iVar5 + 0x2c);
  sVar3 = sVar3 * 6;
  *(undefined2 *)(iVar5 + 0x88) = *(undefined2 *)(sVar3 + 0xe8a86);
  *(int *)(iVar5 + 0x34) = (int)((uint)*(ushort *)(sVar3 + 0xe8a88) << 0x10) >> 8;
  uVar2 = *(ushort *)(sVar3 + 0xe8a84);
  if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
    sVar4 = -sVar4;
    uVar2 = -uVar2;
  }
  *(short *)(iVar5 + 0x28) = sVar4 + *(short *)(iVar5 + 0x28);
  *(int *)(iVar5 + 0x30) = ((int)((uint)uVar2 << 0x10) >> 8) + *(int *)(iVar5 + 0x30);
  bVar6 = false;
  candidate_40bc();
  iVar5 = extraout_A0_01;
  if (bVar6) {
    candidate_1b220();
    iVar5 = extraout_A0_02;
  }
  sVar3 = *(short *)(unaff_A6 + 0x92);
  *(short *)(iVar5 + 0x8e) = sVar3;
  uVar2 = sVar3 * 8 + 0x20;
  if (0x7f < uVar2) {
    uVar2 = 0x80;
  }
  *(ushort *)(iVar5 + 0x8c) = uVar2;
  *(short *)(iVar5 + 0x2c) = *(short *)((short)(sVar3 << 2) + 0xe8a5a) + *(short *)(iVar5 + 0x2c);
  sVar3 = *(short *)((short)(sVar3 << 2) + 0xe8a58);
  if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
    sVar3 = -sVar3;
  }
  *(short *)(iVar5 + 0x28) = sVar3 + *(short *)(iVar5 + 0x28);
  return;
}
