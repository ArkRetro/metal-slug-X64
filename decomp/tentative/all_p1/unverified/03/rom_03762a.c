/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03762a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3762a(void)

{
  code *pcVar1;
  int iVar2;
  undefined2 uVar3;
  short sVar4;
  undefined2 extraout_D1w;
  undefined2 extraout_D1w_00;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar5;
  undefined1 uVar6;
  undefined8 uVar7;

  *(undefined1 *)((int)unaff_A6 + 0x97) = 0;
  candidate_375a8();
  candidate_374ba();
  uVar3 = candidate_375a8();
  *(undefined2 *)(unaff_A6 + 0x23) = uVar3;
  *(undefined2 *)((int)unaff_A6 + 0x8e) = extraout_D1w;
  candidate_9ea8();
  sVar4 = candidate_29f2();
  *(int *)((int)unaff_A6 + 0x6e) = (int)(short)(sVar4 + 0x280) << 8;
  candidate_37602();
  candidate_4256();
  *(undefined2 *)(unaff_A6 + 0x22) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x9a) = 0;
  *unaff_A6 = 0x3768e;
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
    candidate_378d2();
    if (*(short *)(unaff_A6 + 0x22) == 0) {
      candidate_2eb4();
      uVar7 = candidate_2e7c();
      *(undefined8 *)(unaff_A6 + 0xc) = uVar7;
    }
    else {
      unaff_A6[0xc] = 0;
      unaff_A6[0xd] = 0;
    }
    candidate_1ac38();
    candidate_427a();
    bVar5 = false;
    if (*(short *)(unaff_A6 + 0x22) == 0) {
      candidate_375ca();
      if (bVar5) {
        sVar4 = candidate_29f2();
        *(short *)(unaff_A6 + 0x22) = sVar4 + 0x14;
        uVar3 = candidate_375a8();
        *(undefined2 *)(unaff_A6 + 0x23) = uVar3;
        *(undefined2 *)((int)unaff_A6 + 0x8e) = extraout_D1w_00;
      }
    }
    else {
      *(short *)(unaff_A6 + 0x22) = *(short *)(unaff_A6 + 0x22) + -1;
    }
    sVar4 = *(short *)((int)unaff_A6 + 0x8a);
    uVar6 = sVar4 == 0;
    sVar4 = sVar4 + -1;
    *(short *)((int)unaff_A6 + 0x8a) = sVar4;
    if (sVar4 < 0) {
      uVar6 = false;
      candidate_40bc();
      if ((bool)uVar6) {
        candidate_1b220();
        uVar6 = 1;
      }
      candidate_37602();
    }
    candidate_374d8();
    if (!(bool)uVar6) {
      return;
    }
    candidate_37acc();
  }
  else {
    candidate_37acc();
    candidate_9f88();
    candidate_9ea8();
    candidate_4256();
    *unaff_A6 = 0x37ab4;
    uVar6 = 0;
    candidate_1abc8();
    candidate_427a();
    if (!(bool)uVar6) {
      return;
    }
  }
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
