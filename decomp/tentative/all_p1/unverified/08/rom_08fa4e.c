/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08fa4e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8fa4e(void)

{
  byte bVar2;
  short sVar1;
  short sVar3;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int extraout_A0_02;
  int extraout_A0_03;
  int extraout_A0_04;
  int iVar4;
  int unaff_A6;
  bool bVar5;

  candidate_120f8();
  bVar2 = *(byte *)(unaff_A6 + 0x8a);
  if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
    bVar2 = *(byte *)((short)(bVar2 & 0xf) + 0x8fee0);
  }
  *(byte *)(unaff_A6 + 0x6e) = bVar2 & 0xf;
  bVar5 = false;
  candidate_40bc();
  iVar4 = extraout_A0;
  if (bVar5) {
    candidate_1b220();
    iVar4 = extraout_A0_00;
  }
  bVar2 = *(byte *)(unaff_A6 + 0x8a);
  *(byte *)(iVar4 + 0x8a) = bVar2;
  sVar1 = (bVar2 & 0xf) << 2;
  sVar3 = *(short *)(sVar1 + 0x8fd1c);
  sVar1 = *(short *)(sVar1 + 0x8fd1e);
  if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
    sVar3 = -sVar3;
  }
  *(char *)(iVar4 + 0x8b) = (char)sVar3;
  *(char *)(iVar4 + 0x8c) = (char)sVar1;
  if ((*(byte *)(unaff_A6 + 0x93) & 0x20) == 0) {
    bVar5 = false;
    candidate_40bc();
    iVar4 = extraout_A0_01;
    if (bVar5) {
      candidate_1b220();
      iVar4 = extraout_A0_02;
    }
  }
  else {
    bVar5 = false;
    candidate_40bc();
    iVar4 = extraout_A0_03;
    if (bVar5) {
      candidate_1b220();
      iVar4 = extraout_A0_04;
    }
  }
  *(undefined1 *)(iVar4 + 0x8a) = *(undefined1 *)(unaff_A6 + 0x6e);
  *(short *)(iVar4 + 0x28) = sVar3 + *(short *)(iVar4 + 0x28);
  *(short *)(iVar4 + 0x2c) = sVar1 + *(short *)(iVar4 + 0x2c);
  *(undefined1 *)(iVar4 + 0x93) = 0;
  *(undefined1 *)(iVar4 + 0x94) = 0;
  if ((*(byte *)(unaff_A6 + 0x94) & 1) != 0) {
    *(byte *)(iVar4 + 0x94) = *(byte *)(iVar4 + 0x94) | 1;
  }
  *(undefined1 *)(iVar4 + 0x88) = *(undefined1 *)(unaff_A6 + 0x88);
  return;
}
