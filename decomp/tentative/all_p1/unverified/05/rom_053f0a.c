/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x053f0a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_53f0a(void)

{
  code *pcVar1;
  short sVar2;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int extraout_A0_02;
  int extraout_A0_03;
  int extraout_A0_04;
  int extraout_A0_05;
  int extraout_A0_06;
  int extraout_A0_07;
  int extraout_A0_08;
  int extraout_A0_09;
  int extraout_A0_10;
  int iVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar4;

  candidate_120f8();
  bVar4 = false;
  candidate_40bc();
  iVar3 = extraout_A0;
  if (bVar4) {
    candidate_1b220();
    iVar3 = extraout_A0_00;
  }
  *(undefined2 *)(iVar3 + 0x66) = 0xfff1;
  *(undefined2 *)(iVar3 + 0x1e) = 0xfff1;
  *(undefined2 *)((int)unaff_A6 + 0x7a) = 2;
  *unaff_A6 = 0x53f52;
  candidate_1abc8();
  sVar2 = *(short *)((int)unaff_A6 + 0x7a) + -1;
  *(short *)((int)unaff_A6 + 0x7a) = sVar2;
  if (-1 < sVar2) {
    return;
  }
  bVar4 = false;
  candidate_40bc();
  iVar3 = extraout_A0_01;
  if (bVar4) {
    candidate_1b220();
    iVar3 = extraout_A0_02;
  }
  *(undefined2 *)(iVar3 + 0x66) = 0xfff1;
  *(undefined2 *)(iVar3 + 0x1e) = 0xfff1;
  *(undefined2 *)((int)unaff_A6 + 0x7a) = 2;
  *unaff_A6 = 0x53fa0;
  candidate_1abc8();
  sVar2 = *(short *)((int)unaff_A6 + 0x7a) + -1;
  *(short *)((int)unaff_A6 + 0x7a) = sVar2;
  if (-1 < sVar2) {
    return;
  }
  bVar4 = false;
  candidate_40bc();
  iVar3 = extraout_A0_03;
  if (bVar4) {
    candidate_1b220();
    iVar3 = extraout_A0_04;
  }
  *(undefined2 *)(iVar3 + 0x66) = 0xfff1;
  *(undefined2 *)(iVar3 + 0x1e) = 0xfff1;
  *unaff_A6 = 0x53fe8;
  candidate_1abc8();
  if (-1 < *(int *)(unaff_A6[3] + 0x34)) {
    return;
  }
  if (-4 < *(short *)(unaff_A6[3] + 0x2c)) {
    return;
  }
  candidate_120f8();
  bVar4 = false;
  candidate_40bc();
  iVar3 = extraout_A0_05;
  if (bVar4) {
    candidate_1b220();
    iVar3 = extraout_A0_06;
  }
  *(undefined2 *)(iVar3 + 0x66) = 0xfff1;
  *(undefined2 *)(iVar3 + 0x1e) = 0xfff1;
  *(undefined2 *)((int)unaff_A6 + 0x7a) = 2;
  *unaff_A6 = 0x54054;
  candidate_1abc8();
  sVar2 = *(short *)((int)unaff_A6 + 0x7a) + -1;
  *(short *)((int)unaff_A6 + 0x7a) = sVar2;
  if (-1 < sVar2) {
    return;
  }
  bVar4 = false;
  candidate_40bc();
  iVar3 = extraout_A0_07;
  if (bVar4) {
    candidate_1b220();
    iVar3 = extraout_A0_08;
  }
  *(undefined2 *)(iVar3 + 0x66) = 0xfff1;
  *(undefined2 *)(iVar3 + 0x1e) = 0xfff1;
  *(undefined2 *)((int)unaff_A6 + 0x7a) = 2;
  *unaff_A6 = 0x540a2;
  candidate_1abc8();
  sVar2 = *(short *)((int)unaff_A6 + 0x7a) + -1;
  *(short *)((int)unaff_A6 + 0x7a) = sVar2;
  if (-1 < sVar2) {
    return;
  }
  bVar4 = false;
  candidate_40bc();
  iVar3 = extraout_A0_09;
  if (bVar4) {
    candidate_1b220();
    iVar3 = extraout_A0_10;
  }
  *(undefined2 *)(iVar3 + 0x66) = 0xfff1;
  *(undefined2 *)(iVar3 + 0x1e) = 0xfff1;
  candidate_4126();
  iVar3 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar3 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
