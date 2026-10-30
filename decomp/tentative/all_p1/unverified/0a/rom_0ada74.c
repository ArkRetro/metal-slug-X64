/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ada74.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_ada74(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  ushort uVar4;
  short sVar5;
  undefined1 uVar6;
  ushort uVar7;
  undefined2 unaff_D2w;
  undefined1 uVar8;
  undefined2 unaff_D3w;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int extraout_A0_02;
  int extraout_A0_03;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar9;
  bool bVar10;
  undefined2 uVar12;
  undefined4 uVar11;
  undefined2 uVar13;

  if (((*(char *)(unaff_A6 + 0x27) != '\0') && (sVar5 = candidate_49e8(), extraout_A0 != 0)) &&
     (sVar5 != 0)) {
    sVar5 = *(short *)(extraout_A0 + 0x40) +
            (*(short *)(*(int *)(extraout_A0 + 0x50) + 4) -
            (*(ushort *)(*(int *)(extraout_A0 + 0x50) + 8) >> 1));
    if (sVar5 < (short)(*(short *)(unaff_A6 + 0x10) + 4)) {
      unaff_A6[0x24] = unaff_A6[0x24] + -0x3000;
    }
    else if ((short)(*(short *)(unaff_A6 + 0x10) + -4) < sVar5) {
      unaff_A6[0x24] = unaff_A6[0x24] + 0x3000;
    }
    else {
      unaff_A6[0x24] = 0;
    }
  }
  uVar11 = unaff_A6[0xd];
  unaff_A6[0xd] = unaff_A6[0x24] + unaff_A6[0xd];
  uVar12 = 0xdaf0;
  func_0x000143e4();
  unaff_A6[0xd] = uVar11;
  bVar10 = false;
  candidate_1945c();
  uVar6 = (undefined1)((ushort)unaff_D2w >> 8);
  uVar8 = (undefined1)((ushort)unaff_D3w >> 8);
  if (bVar10) {
    candidate_adc80();
    uVar7 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar7 + 0x3ef;
    if (uVar7 < 0xfc11) goto candidate_4188;
    uVar7 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar4 = uRam002fffe8;
    uVar3 = ~uVar7 & 0xfff;
    uVar7 = CONCAT11(uVar6,-((*(byte *)((short)(uVar7 >> 3 & 0xfff) + 0xdedd2) &
                             '\x01' << (uVar3 & 7)) != 0));
    uRam002fffe0 = uVar7 & CONCAT11(uVar8,-(0x7ff < uVar3));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar3 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    uVar7 = (ushort)(byte)-(0xfbff < (ushort)(uVar3 - 0x800)) & (uVar7 ^ uVar4);
  }
  else {
    sVar5 = candidate_49e8();
    if ((sVar5 == 0) && ((*(byte *)((int)unaff_A6 + 0x5f) & 1) == 0)) {
      if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
        *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(unaff_A6[3] + 0x1e);
        *(short *)((int)unaff_A6 + 0x1e) = *(short *)((int)unaff_A6 + 0x1e) + 3;
        uVar13 = 0xdbe4;
        candidate_427a();
        uVar11 = CONCAT22(0xffec,uVar12);
        candidate_40bc(uVar11,CONCAT22(4,uVar13));
        *(short *)(extraout_A0_03 + 0x6e) = (short)((uint)uVar11 >> 0x10);
        *(short *)(extraout_A0_03 + 0x70) = (short)uVar11;
        return;
      }
      candidate_ad9e6();
      *(undefined4 *)(extraout_A0_02 + 0x30) = 0;
      *(undefined4 *)(extraout_A0_02 + 0x34) = 0;
      *(undefined2 *)(extraout_A0_02 + 0x4e) = 0;
    }
    else {
      candidate_ad9e6();
      *(undefined4 *)(extraout_A0_00 + 0x30) = 0x200000;
      *(undefined4 *)(extraout_A0_00 + 0x34) = 0x180000;
      *(undefined2 *)(extraout_A0_00 + 0x4e) = 7;
      candidate_ad9e6();
      *(undefined4 *)(extraout_A0_01 + 0x30) = 0x1a0000;
      *(undefined4 *)(extraout_A0_01 + 0x34) = 0x30000;
      *(undefined2 *)(extraout_A0_01 + 0x4e) = 6;
      candidate_411c();
      candidate_1acf2();
    }
    candidate_120f8();
    candidate_adc80();
    candidate_9f88();
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ed;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ed;
    candidate_4256();
    *(undefined4 *)((int)unaff_A6 + 0x82) = 0x1deb8;
    candidate_9ea8();
    candidate_1dea2();
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfd;
    unaff_A6[0x14] = 0;
    *unaff_A6 = 0x1c9e6;
    uVar9 = 0;
    candidate_1abc8();
    candidate_427a();
    if ((bool)uVar9) goto candidate_4188;
    candidate_19504();
    if (!(bool)uVar9) {
      return;
    }
    uVar7 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar7 + 0x3ef;
    if (uVar7 < 0xfc11) goto candidate_4188;
    uVar7 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar4 = uRam002fffe8;
    uVar3 = ~uVar7 & 0xfff;
    uVar7 = CONCAT11(uVar6,-((*(byte *)((short)(uVar7 >> 3 & 0xfff) + 0xdedd2) &
                             '\x01' << (uVar3 & 7)) != 0));
    uRam002fffe0 = uVar7 & CONCAT11(uVar8,-(0x7ff < uVar3));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar3 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    uVar7 = (ushort)(byte)-(0xfbff < (ushort)(uVar3 - 0x800)) & (uVar7 ^ uVar4);
  }
  if ((uVar7 & 1) != 0) {
    *(undefined2 *)(unaff_A5 + -0x7fec) = 1;
    _DAT_002ffff0 = 1;
    do {
    } while (DAT_002fffff != '\x01');
    uRam00400000 = 0;
    uRam00400002 = *(undefined2 *)((short)(sRam002112e2 * 2) + 0x224000);
    uRam00400004 = *(undefined2 *)((short)(sRam002112e4 * 2) + 0x224000);
    uRam00400006 = *(undefined2 *)((short)(sRam002112e6 * 2) + 0x224000);
    uRam00400008 = *(undefined2 *)((short)(sRam002112e8 * 2) + 0x224000);
    uRam0040000a = *(undefined2 *)((short)(sRam002112ea * 2) + 0x224000);
    uRam0040000c = *(undefined2 *)((short)(sRam002112ec * 2) + 0x224000);
    uRam0040000e = *(undefined2 *)((short)(sRam002112ee * 2) + 0x224000);
    uRam00400010 = *(undefined2 *)((short)(sRam002112f0 * 2) + 0x224000);
    uRam00400012 = *(undefined2 *)((short)(sRam002112f2 * 2) + 0x224000);
    uRam00400014 = *(undefined2 *)((short)(sRam002112f4 * 2) + 0x224000);
    uRam00400016 = *(undefined2 *)((short)(sRam002112f6 * 2) + 0x224000);
    uRam00400018 = *(undefined2 *)((short)(sRam002112f8 * 2) + 0x224000);
    uRam0040001a = *(undefined2 *)((short)(sRam002112fa * 2) + 0x224000);
    uRam0040001c = *(undefined2 *)((short)(sRam002112fc * 2) + 0x224000);
    uRam0040001e = *(undefined2 *)((short)(sRam002112fe * 2) + 0x224000);
    _DAT_00401ffe = 0;
    func_0x0003cf12();
    candidate_3e2a8();
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
candidate_4188:
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
