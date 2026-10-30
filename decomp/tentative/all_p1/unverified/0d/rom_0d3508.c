/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d3508.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d3508(void)

{
  ushort uVar1;
  short sVar2;
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
  sVar2 = *(short *)(unaff_A6 + 0x8e) << 2;
  sVar4 = *(short *)(sVar2 + 0xe8a10);
  *(short *)(iVar5 + 0x2c) = *(short *)(sVar2 + 0xe8a12) + *(short *)(iVar5 + 0x2c);
  if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
    sVar4 = -sVar4;
  }
  *(short *)(iVar5 + 0x28) = sVar4 + *(short *)(iVar5 + 0x28);
  bVar6 = false;
  candidate_40bc();
  iVar5 = extraout_A0_01;
  if (bVar6) {
    candidate_1b220();
    iVar5 = extraout_A0_02;
  }
  sVar4 = *(short *)(unaff_A6 + 0x8e);
  sVar3 = sVar4 << 2;
  sVar2 = *(short *)(sVar3 + 0xe8a10);
  *(short *)(iVar5 + 0x2c) = *(short *)(sVar3 + 0xe8a12) + *(short *)(iVar5 + 0x2c);
  sVar4 = sVar4 * 6;
  *(undefined2 *)(iVar5 + 0x88) = *(undefined2 *)(sVar4 + 0xe8a2a);
  *(int *)(iVar5 + 0x34) = (int)((uint)*(ushort *)(sVar4 + 0xe8a2c) << 0x10) >> 8;
  uVar1 = *(ushort *)(sVar4 + 0xe8a28);
  if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
    sVar2 = -sVar2;
    uVar1 = -uVar1;
  }
  *(short *)(iVar5 + 0x28) = sVar2 + *(short *)(iVar5 + 0x28);
  *(int *)(iVar5 + 0x30) = (int)((uint)uVar1 << 0x10) >> 8;
  return;
}
