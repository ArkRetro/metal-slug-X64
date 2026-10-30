/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a29ce.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a29ce(void)

{
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int extraout_A0_02;
  int extraout_A0_03;
  int extraout_A0_04;
  int iVar1;
  bool bVar2;

  bVar2 = false;
  candidate_40bc();
  iVar1 = extraout_A0;
  if (bVar2) {
    candidate_1b220();
    iVar1 = extraout_A0_00;
  }
  *(undefined2 *)(iVar1 + 0x2c) = 0;
  bVar2 = false;
  candidate_40bc();
  iVar1 = extraout_A0_01;
  if (bVar2) {
    candidate_1b220();
    iVar1 = extraout_A0_02;
  }
  *(undefined2 *)(iVar1 + 0x2c) = 0xfff8;
  *(undefined2 *)(iVar1 + 0x82) = 5;
  bVar2 = false;
  candidate_40bc();
  iVar1 = extraout_A0_03;
  if (bVar2) {
    candidate_1b220();
    iVar1 = extraout_A0_04;
  }
  *(undefined2 *)(iVar1 + 0x2c) = 0xfffc;
  *(undefined2 *)(iVar1 + 0x82) = 10;
  return;
}
