/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a2b08.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a2b08(void)

{
  short sVar1;
  ushort in_D0w;
  ushort uVar2;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int extraout_A0_02;
  int iVar3;
  bool bVar4;

  uVar2 = in_D0w & 0x7f;
  bVar4 = false;
  candidate_40bc();
  iVar3 = extraout_A0;
  if (bVar4) {
    candidate_1b220();
    iVar3 = extraout_A0_00;
  }
  sVar1 = uVar2 * -4 + -0x10;
  *(short *)(iVar3 + 0x28) = uVar2 * 0x20 + *(short *)(iVar3 + 0x28);
  *(short *)(iVar3 + 0x2c) = sVar1;
  if (uVar2 != 0) {
    bVar4 = false;
    candidate_40bc();
    iVar3 = extraout_A0_01;
    if (bVar4) {
      candidate_1b220();
      iVar3 = extraout_A0_02;
    }
    *(short *)(iVar3 + 0x28) = *(short *)(iVar3 + 0x28) + uVar2 * -0x20;
    *(short *)(iVar3 + 0x2c) = sVar1;
  }
  return;
}
