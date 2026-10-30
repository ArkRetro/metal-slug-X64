/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d9eee.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d9eee(void)

{
  code *pcVar1;
  undefined2 uVar2;
  ushort uVar3;
  short sVar4;
  int extraout_A0;
  int extraout_A0_00;
  int iVar5;
  int extraout_A0_01;
  int extraout_A0_02;
  int extraout_A0_03;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar6;
  undefined1 uVar7;

  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  candidate_da3e8();
  *(short *)((int)unaff_A6 + 0x1e) = *(short *)((int)unaff_A6 + 0x1e) + 2;
  *(undefined1 *)((int)unaff_A6 + 0x7a) = 0;
  *(undefined2 *)(unaff_A6 + 0x1c) = 0xffff;
  uVar2 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x82) = uVar2;
  candidate_da3a8();
  candidate_4256();
  uVar2 = *(undefined2 *)(unaff_A6 + 0x22);
  sVar4 = 0x99;
  do {
    *(char *)((int)unaff_A6 + (int)sVar4) = (char)uVar2;
    sVar4 = sVar4 + -1;
  } while (0x8a < sVar4);
  candidate_da440();
  *unaff_A6 = 0xd9f4a;
  *(short *)(unaff_A6 + 0x1c) = *(short *)(unaff_A6 + 0x1c) + 1;
  if (*(short *)(unaff_A6 + 0x1c) < 0x259) {
    if ((*(ushort *)(unaff_A6 + 0x1c) & 0xf) == 0) {
      candidate_da292();
      bVar6 = *(char *)((int)unaff_A6 + 0x7a) != -1;
      if (*(char *)((int)unaff_A6 + 0x7a) != -1) {
        candidate_da344();
        bVar6 = *(ushort *)(unaff_A6 + 0x10) < 0x1c0;
        if (0x1bf < (short)*(ushort *)(unaff_A6 + 0x10)) {
          *(undefined1 *)((int)unaff_A6 + 0x7a) = 0xff;
          bVar6 = false;
        }
      }
      uVar2 = candidate_da42c();
      if (bVar6) {
        uVar2 = 8;
      }
      *(undefined2 *)(unaff_A6 + 0x1e) = uVar2;
      goto LAB_000d9f9a;
    }
  }
  else {
LAB_000d9f9a:
    uVar3 = candidate_da272();
    if ((*(ushort *)(unaff_A6 + 0x1c) & 0x1f) == 0) {
      *(ushort *)((int)unaff_A6 + 0x76) = uVar3 | 1;
    }
  }
  if ((*(ushort *)(unaff_A6 + 0x1c) & 3) == 0) {
    *(short *)(unaff_A6 + 0x22) = *(short *)((int)unaff_A6 + 0x76) + *(short *)(unaff_A6 + 0x22);
    *(ushort *)(unaff_A6 + 0x22) = *(ushort *)(unaff_A6 + 0x22) & 0x1f;
  }
  uVar7 = 0;
  candidate_da2d4();
  candidate_da1f4();
  if (!(bool)uVar7) {
    candidate_427a();
    if ((*(ushort *)(unaff_A6 + 0x1c) & 7) == 0) {
      if (*(short *)(unaff_A6 + 0x22) < 0x11) {
        bVar6 = false;
        candidate_40bc();
        if (bVar6) {
          candidate_1b220();
        }
      }
      else {
        bVar6 = false;
        candidate_40bc();
        iVar5 = extraout_A0;
        if (bVar6) {
          candidate_1b220();
          iVar5 = extraout_A0_00;
        }
        *(short *)(iVar5 + 0x1e) = *(short *)(iVar5 + 0x1e) + -1;
      }
      candidate_da3b4();
      *(ushort *)(extraout_A0_01 + 0x88) = *(byte *)((int)unaff_A6 + 0x99) & 0x1f;
    }
    sVar4 = candidate_49e8();
    uVar7 = false;
    if (sVar4 == 0) {
      candidate_4c24();
      if ((bool)uVar7) {
        *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
        uVar7 = 0;
      }
      if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
        candidate_da38c();
        if (!(bool)uVar7) {
          return;
        }
        goto LAB_000da0c4;
      }
    }
  }
  bVar6 = false;
  candidate_40bc();
  iVar5 = extraout_A0_02;
  if (bVar6) {
    candidate_1b220();
    iVar5 = extraout_A0_03;
  }
  *(undefined2 *)(iVar5 + 0x66) = 0x1001;
  *(undefined2 *)(iVar5 + 0x1e) = 0x1001;
  candidate_1da40();
LAB_000da0c4:
  *(undefined1 *)(unaff_A6[3] + 0x7d) = 0xff;
  candidate_4126();
  iVar5 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar5 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
