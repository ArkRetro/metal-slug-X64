/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0daabe.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_daabe(void)

{
  code *pcVar1;
  short sVar2;
  byte bVar3;
  int extraout_A0;
  int extraout_A0_00;
  int iVar4;
  int extraout_A0_01;
  int extraout_A0_02;
  int extraout_A0_03;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar5;
  undefined1 uVar6;

  bVar5 = false;
  candidate_40bc();
  iVar4 = extraout_A0;
  if (bVar5) {
    candidate_1b220();
    iVar4 = extraout_A0_00;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    *(short *)(iVar4 + 0x28) = *(short *)(iVar4 + 0x28) + 0x2b;
  }
  else {
    *(short *)(iVar4 + 0x28) = *(short *)(iVar4 + 0x28) + -0x14;
  }
  *(short *)(iVar4 + 0x2c) = *(short *)(iVar4 + 0x2c) + -0x17;
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 0x3c;
  *(undefined2 *)(unaff_A6 + 0x1c) = 0x70;
  unaff_A6[0x14] = 0xdc100;
  *unaff_A6 = 0xdab1e;
  uVar6 = 0;
  candidate_1abc8();
  candidate_4c24();
  if ((bool)uVar6) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
    bVar5 = false;
    candidate_da488();
    if (!bVar5) {
      return;
    }
  }
  else {
    unaff_A6[0x14] = 0;
    candidate_1da40();
    *(short *)(extraout_A0_01 + 0x2c) = *(short *)(extraout_A0_01 + 0x2c) + -0x20;
    candidate_1da40();
    *(short *)(extraout_A0_02 + 0x2c) = *(short *)(extraout_A0_02 + 0x2c) + -0x20;
    candidate_1da40();
    sVar2 = 0x20;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
      sVar2 = -0x20;
    }
    *(short *)(extraout_A0_03 + 0x28) = sVar2 + *(short *)(extraout_A0_03 + 0x28);
    candidate_dabbe();
    bVar3 = 0x10;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
      bVar3 = 1;
    }
    *(byte *)(unaff_A6[3] + 0x61) = bVar3 | *(byte *)(unaff_A6[3] + 0x61);
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
  }
  candidate_4126();
  iVar4 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar4 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
