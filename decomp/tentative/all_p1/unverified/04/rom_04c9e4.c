/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04c9e4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4c9e4(void)

{
  code *pcVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  short sVar5;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar6;

  *(undefined2 *)(unaff_A6 + 0x22) = *(undefined2 *)((int)unaff_A6 + 0x1e);
  func_0x000143e4();
  uVar3 = unaff_A6[0xf];
  uVar4 = unaff_A6[0x10];
  candidate_4ce16();
  candidate_427a();
  *(short *)(unaff_A6 + 0x21) = *(short *)((int)unaff_A6 + 0x96) + *(short *)(unaff_A6 + 0x21);
  if (0x11ff < *(short *)(unaff_A6 + 0x21)) {
    *(short *)(unaff_A6 + 0x21) = *(short *)(unaff_A6 + 0x21) + -0x1200;
    candidate_40bc();
    candidate_1acf2();
  }
  if (*(short *)((int)unaff_A6 + 0x86) == 0) {
    sVar5 = candidate_49e8();
    uVar6 = 0;
    if (sVar5 != 0) {
      *unaff_A6 = candidate_1c706;
      uVar6 = 0;
      candidate_120f8();
    }
  }
  else {
    sVar5 = *(short *)((int)unaff_A6 + 0x86);
    uVar6 = sVar5 == 0;
    *(short *)((int)unaff_A6 + 0x86) = sVar5 + -1;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_4cce4;
    uVar6 = 0;
  }
  if ((*(byte *)(unaff_A6[3] + 0x61) & 2) != 0) {
    *unaff_A6 = candidate_1c706;
    uVar6 = 0;
  }
  candidate_4e092();
  if (!(bool)uVar6) {
    unaff_A6[0x10] = uVar4;
    unaff_A6[0xf] = uVar3;
    return;
  }
  unaff_A6[0x10] = uVar4;
  unaff_A6[0xf] = uVar3;
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
