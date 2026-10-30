/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d14aa.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d14aa(void)

{
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int extraout_A0_02;
  int extraout_A0_03;
  int extraout_A0_04;
  int extraout_A0_05;
  int extraout_A0_06;
  int iVar1;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar2;

  candidate_1ab98();
  candidate_146e8();
  *(short *)(unaff_A6 + 10) = *(short *)(unaff_A6 + 10) + 0x130;
  bVar2 = false;
  candidate_40bc();
  iVar1 = extraout_A0;
  if (bVar2) {
    candidate_1b220();
    iVar1 = extraout_A0_00;
  }
  *(undefined2 *)(iVar1 + 0x74) = 0;
  bVar2 = false;
  candidate_40bc();
  iVar1 = extraout_A0_01;
  if (bVar2) {
    candidate_1b220();
    iVar1 = extraout_A0_02;
  }
  *(undefined2 *)(iVar1 + 0x74) = 0;
  bVar2 = false;
  candidate_40bc();
  iVar1 = extraout_A0_03;
  if (bVar2) {
    candidate_1b220();
    iVar1 = extraout_A0_04;
  }
  *(undefined2 *)(iVar1 + 0x74) = 0x70;
  bVar2 = false;
  candidate_40bc();
  iVar1 = extraout_A0_05;
  if (bVar2) {
    candidate_1b220();
    iVar1 = extraout_A0_06;
  }
  *(undefined2 *)(iVar1 + 0x74) = 0xe0;
  *unaff_A6 = 0xd156c;
  if (*(short *)(unaff_A5 + 0x699e) != 0x1000) {
    candidate_1abc8();
    candidate_d1b80();
    if (*(short *)(unaff_A6 + 0xf) < 0x30) {
      *(short *)(unaff_A6 + 10) = (0x30 - *(short *)(unaff_A6 + 0xf)) + *(short *)(unaff_A6 + 10);
    }
    return;
  }
  candidate_1ab98();
  *unaff_A6 = 0xd15b2;
  candidate_1abc8();
  candidate_d1b80();
  return;
}
