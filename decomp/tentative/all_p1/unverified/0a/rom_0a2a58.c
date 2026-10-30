/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a2a58.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a2a58(void)

{
  ushort in_D0w;
  ushort uVar1;
  undefined2 in_D1w;
  undefined2 uVar2;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int extraout_A0_02;
  int iVar3;
  bool bVar4;

  uVar1 = in_D0w & 0x7f;
  bVar4 = false;
  candidate_40bc();
  iVar3 = extraout_A0;
  if (bVar4) {
    candidate_1b220();
    iVar3 = extraout_A0_00;
  }
  if ((in_D0w & 1) == 0) {
    uVar2 = 0xfff4;
  }
  else {
    uVar2 = 0xfff0;
  }
  *(short *)(iVar3 + 0x28) = uVar1 * 0x10 + *(short *)(iVar3 + 0x28);
  *(undefined2 *)(iVar3 + 0x2c) = uVar2;
  *(undefined2 *)(iVar3 + 0x82) = in_D1w;
  if (uVar1 != 0) {
    bVar4 = false;
    candidate_40bc();
    iVar3 = extraout_A0_01;
    if (bVar4) {
      candidate_1b220();
      iVar3 = extraout_A0_02;
    }
    *(short *)(iVar3 + 0x28) = *(short *)(iVar3 + 0x28) + uVar1 * -0x10;
    *(undefined2 *)(iVar3 + 0x2c) = uVar2;
    *(undefined2 *)(iVar3 + 0x82) = in_D1w;
  }
  return;
}
