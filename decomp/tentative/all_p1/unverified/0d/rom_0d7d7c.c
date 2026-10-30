/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d7d7c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d7d7c(void)

{
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int extraout_A0_02;
  int extraout_A0_03;
  int extraout_A0_04;
  int extraout_A0_05;
  int extraout_A0_06;
  int extraout_A0_07;
  int iVar1;
  int extraout_A0_08;
  undefined4 *unaff_A6;
  bool bVar2;

  candidate_18f92();
  candidate_40bc();
  *(undefined1 *)(extraout_A0 + 0x82) = 1;
  unaff_A6[0x14] = 0;
  *(undefined1 *)(unaff_A6 + 0x22) = 0x80;
  bVar2 = false;
  candidate_40bc();
  if (bVar2) {
    candidate_1b220();
  }
  bVar2 = false;
  candidate_40bc();
  if (bVar2) {
    candidate_1b220();
  }
  bVar2 = false;
  candidate_40bc();
  iVar1 = extraout_A0_00;
  if (bVar2) {
    candidate_1b220();
    iVar1 = extraout_A0_01;
  }
  *(undefined2 *)(iVar1 + 0x66) = 0xfff1;
  *(undefined2 *)(iVar1 + 0x1e) = 0xfff1;
  bVar2 = false;
  candidate_40bc();
  iVar1 = extraout_A0_02;
  if (bVar2) {
    candidate_1b220();
    iVar1 = extraout_A0_03;
  }
  *(undefined2 *)(iVar1 + 0x66) = 0xfff1;
  *(undefined2 *)(iVar1 + 0x1e) = 0xfff1;
  bVar2 = false;
  candidate_40bc();
  iVar1 = extraout_A0_04;
  if (bVar2) {
    candidate_1b220();
    iVar1 = extraout_A0_05;
  }
  *(undefined2 *)(iVar1 + 0x66) = 0xfff1;
  *(undefined2 *)(iVar1 + 0x1e) = 0xfff1;
  bVar2 = false;
  candidate_40bc();
  iVar1 = extraout_A0_06;
  if (bVar2) {
    candidate_1b220();
    iVar1 = extraout_A0_07;
  }
  *(undefined2 *)(iVar1 + 0x66) = 0xfff1;
  *(undefined2 *)(iVar1 + 0x1e) = 0xfff1;
  *(undefined2 *)((int)unaff_A6 + 0x76) = 0x1e;
  candidate_1b1dc();
  *unaff_A6 = 0xd7eb4;
  bVar2 = false;
  candidate_19504();
  if (bVar2) {
    *unaff_A6 = 0xd7eca;
  }
  func_0x000143e4();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_d7f2c;
  }
  candidate_d90ac();
  *(short *)((int)unaff_A6 + 0x76) = *(short *)((int)unaff_A6 + 0x76) + -1;
  if (*(short *)((int)unaff_A6 + 0x76) == 0) {
    unaff_A6[0xc] = 0xc000;
    unaff_A6[0xd] = 0xffffc000;
    candidate_1da40();
    *(undefined2 *)(extraout_A0_08 + 0x66) = 0x1065;
    *(undefined2 *)(extraout_A0_08 + 0x1e) = 0x1065;
  }
  if (0x11f < *(short *)(unaff_A6 + 0xf)) {
    *unaff_A6 = candidate_d7f2c;
  }
  return;
}
