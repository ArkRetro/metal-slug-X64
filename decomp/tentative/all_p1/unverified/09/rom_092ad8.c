/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x092ad8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_92ad8(void)

{
  code *pcVar1;
  int iVar2;
  short *psVar3;
  ushort uVar4;
  short sVar5;
  ushort uVar6;
  undefined2 uVar7;
  undefined4 unaff_D2;
  ushort uVar8;
  undefined4 unaff_D3;
  short *psVar9;
  ushort *extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int extraout_A0_02;
  int extraout_A0_03;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar10;
  bool bVar11;

  *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1b);
  unaff_A6[0x24] = 0;
  candidate_4256();
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  candidate_353a4();
  *unaff_A6 = 0x92b0e;
  if (*(short *)((int)unaff_A6 + 0x4e) == 0) {
    candidate_120f8();
    candidate_120f8();
    candidate_4256();
    unaff_A6[0x24] = 0x93882;
    candidate_353c2();
    *(undefined2 *)((int)unaff_A6 + 0x8a) = 0x60;
    *unaff_A6 = 0x9391e;
    psVar3 = (short *)unaff_A6[0x24];
    if (psVar3 != (short *)0x0) {
      *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
      psVar9 = psVar3 + 1;
      if (*psVar3 == -1) {
        psVar9 = (short *)((int)psVar9 + *psVar9 + 2);
      }
      unaff_A6[0x24] = psVar9;
      candidate_9d1e();
    }
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x7e) = 0xff;
    if (*(short *)(unaff_A6 + 0x20) < 0) {
      candidate_93846();
    }
    else {
      candidate_ca2ce();
      if ((*extraout_A0 & 0x20) != 0) {
        candidate_aa7fc();
      }
      bVar11 = false;
      candidate_aa760();
      if (!bVar11) {
        *(undefined1 *)(unaff_A6 + 0x20) = 0xff;
      }
      candidate_9380a();
    }
    candidate_92832();
    sVar5 = *(short *)((int)unaff_A6 + 0x8a) + -1;
    *(short *)((int)unaff_A6 + 0x8a) = sVar5;
    if (sVar5 < 0) {
      *unaff_A6 = 0x93afa;
    }
LAB_000939ba:
    *(undefined1 *)((int)unaff_A6 + 0x86) = *(undefined1 *)unaff_A6[0x1c];
    *(undefined1 *)((int)unaff_A6 + 0x87) = ((undefined1 *)unaff_A6[0x1c])[1];
    return;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    *(undefined2 *)((int)unaff_A6 + 0x7e) = 8;
    uVar10 = 0;
    candidate_93846();
    candidate_92832();
    candidate_c8bec();
    if (!(bool)uVar10) {
      uVar7 = candidate_aa610();
      if ((bool)uVar10) {
        *(undefined2 *)(unaff_A6 + 0x20) = uVar7;
        bVar11 = false;
        uVar7 = candidate_aa760();
        if (bVar11) {
          *(undefined2 *)((int)unaff_A6 + 0x82) = uVar7;
          candidate_aa83a();
          *unaff_A6 = candidate_92c18;
        }
      }
      goto LAB_000939ba;
    }
    uVar8 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar8 + 0x3ef;
    if (0xfc10 < uVar8) {
      uVar8 = *(ushort *)(unaff_A5 + 0x700a);
      *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
      uVar6 = uRam002fffe8;
      uVar4 = ~uVar8 & 0xfff;
      uVar8 = (ushort)CONCAT31((int3)((uint)unaff_D2 >> 8),
                               -((*(byte *)((short)(uVar8 >> 3 & 0xfff) + 0xdedd2) &
                                 '\x01' << (uVar4 & 7)) != 0));
      uRam002fffe0 = uVar8 & (ushort)CONCAT31((int3)((uint)unaff_D3 >> 8),-(0x7ff < uVar4));
      uRam002fffe2 = 0;
      uRam002fffe4 = 0;
      uRam002fffe6 = 0;
      *(short *)(unaff_A5 + 0x700a) =
           ((short)-(uVar4 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
           *(short *)(unaff_A5 + 0x700a);
      if (((ushort)(byte)-(0xfbff < (ushort)(uVar4 - 0x800)) & (uVar8 ^ uVar6) & 1) != 0) {
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
    }
  }
  else {
    candidate_120f8();
    candidate_49e8();
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 7;
    candidate_aa7fc();
    candidate_ae220();
    candidate_ad9e6();
    *(undefined4 *)(extraout_A0_00 + 0x30) = 0xfff20000;
    *(undefined4 *)(extraout_A0_00 + 0x34) = 0x280000;
    *(undefined2 *)(extraout_A0_00 + 0x4e) = 10;
    candidate_ad9e6();
    *(undefined4 *)(extraout_A0_01 + 0x30) = 0xa0000;
    *(undefined4 *)(extraout_A0_01 + 0x34) = 0x250000;
    *(undefined2 *)(extraout_A0_01 + 0x4e) = 7;
    candidate_ad9e6();
    *(undefined4 *)(extraout_A0_02 + 0x30) = 0xfff70000;
    *(undefined4 *)(extraout_A0_02 + 0x34) = 0x10000;
    *(undefined2 *)(extraout_A0_02 + 0x4e) = 7;
    candidate_ad9e6();
    *(undefined4 *)(extraout_A0_03 + 0x30) = 0x70000;
    *(undefined4 *)(extraout_A0_03 + 0x34) = 0xfffc0000;
    *(undefined2 *)(extraout_A0_03 + 0x4e) = 5;
    candidate_411c();
    candidate_1acf2();
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
